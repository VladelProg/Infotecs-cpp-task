#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include "lib.h"

std::mutex mtx;
std::condition_variable cv;
std::queue<std::string> buffer;
bool running = true;

void inputThread() {
    std::string input;
    while (running) {
        std::cout << "Введите строку чисел (макс. 64 символа): ";
        std::cin >> input;
        if (input.length() > 64 || input.find_first_not_of("0123456789") != std::string::npos) {
            std::cout << "Неверный ввод. Попробуйте ещё раз." << std::endl;
            continue;
        }
        sortAndReplaceEvenWithKB(input);
        {
            std::lock_guard<std::mutex> lock(mtx);
            buffer.push(input);
        }
        cv.notify_one();
    }
}

void processingThread(int serverSocket) {
    while (running) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []{ return !buffer.empty() || !running; });
        if (!running) break;
        std::string data = buffer.front();
        buffer.pop();
        lock.unlock();

        std::cout << "Обработанная строка: " << data << std::endl;
        int sum = calculateSumOfNumbers(data);

        // Отправляем сумму в программу №2
        if (send(serverSocket, &sum, sizeof(sum), 0) < 0) {
            std::cerr << "Ошибка отправки" << std::endl;
        } else {
            std::cout << "Сумма: " << sum << std::endl;
        }
    }
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Ошибка создания сокета" << std::endl;
        return 1;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Ошибка создания сервера (возможно программа уже запущена)" << std::endl;
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, 1) < 0) {
        std::cerr << "Fail (listen)" << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Ожидание подключения программы 2..." << std::endl;
    sockaddr_in clientAddress{};
    socklen_t clientAddressLength = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
    if (clientSocket < 0) {
        std::cerr << "Неудача" << std::endl;
        close(serverSocket);
        return 1;
    }
    std::cout << "Соединение с программой 2 прошло успешно!" << std::endl;

    std::thread inputT(inputThread);
    std::thread processingT(processingThread, clientSocket);

    inputT.join();
    processingT.join();

    close(clientSocket);
    close(serverSocket);
    return 0;
}

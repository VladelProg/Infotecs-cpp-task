#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include "lib.h"

int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        std::cerr << "Ошибка создани сокета" << std::endl;
        return 1;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

    std::cout << "Подключение к программе 1..." << std::endl;
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Ошибка подключения" << std::endl;
        close(clientSocket);
        return 1;
    }
    std::cout << "Успешное подключение!" << std::endl;

    while (true) {
        int sum;
        int bytesReceived = recv(clientSocket, &sum, sizeof(sum), 0);
        if (bytesReceived < 0) {
            std::cerr << "Ошибка получения" << std::endl;
            break;
        } else if (bytesReceived == 0) {
            std::cout << "Программа 1 отключена. Ожидание переподключения..." << std::endl;
            close(clientSocket);

            // Попытка переподключения
            clientSocket = socket(AF_INET, SOCK_STREAM, 0);
            if (clientSocket < 0) {
                std::cerr << "Ошибка создания сокета" << std::endl;
                return 1;
            }

            while (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
                std::cout << "Попытка подключения к программе 1..." << std::endl;
                sleep(2); // Ждем 2 секунды перед повторной попыткой
            }
            std::cout << "Переподключение прошло успешно!" << std::endl;
            continue;
        }

        std::string data = std::to_string(sum);
        if (isLengthMultipleOf32(data)) {
            std::cout << "Получены данные " << data << std::endl;
        } else {
            std::cout << "Ошибка получения" << data.length() << std::endl;
        }
    }

    close(clientSocket);
    return 0;
}

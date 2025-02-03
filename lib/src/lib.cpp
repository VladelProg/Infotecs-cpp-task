#include "lib.h"
#include <algorithm>
#include <cctype>

void sortAndReplaceEvenWithKB(std::string &input) {
    std::sort(input.begin(), input.end(), std::greater<char>());
    for (size_t i = 0; i < input.length(); ++i) {
        if (std::isdigit(input[i]) && (input[i] - '0') % 2 == 0) {
            input.replace(i, 1, "KB");
        }
    }
}

int calculateSumOfNumbers(const std::string &input) {
    int sum = 0;
    for (char ch : input) {
        if (std::isdigit(ch)) {
            sum += ch - '0';
        }
    }
    return sum;
}

bool isLengthMultipleOf32(const std::string &input) {
    return input.length() > 2 && stoi(input) % 32 == 0;
}

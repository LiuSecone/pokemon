#include "client_terminal.h"

int main() {
    client_terminal client;
    while (true) {
        std::string str;
        std::cin >> str;
        const auto init_result = client.init();
        if (init_result) {
            std::cout << "TCP client init Error";
            return init_result;
        }
        std::cout << client.request(str) << std::endl;
    }
}
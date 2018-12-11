#include "client_terminal.h"
#include "../pokemon/my_algo_lib.h"

int main() {
    client_terminal client;
    while (true) {
        std::string str;
        std::getline(std::cin, str);
        auto elem = my_algo_lib::split(str, ' ');
        std::cout << elem.size() << std::endl;
        const auto init_result = client.init();
        if (init_result) {
            std::cout << "TCP client init Error";
            return init_result;
        }
        std::cout << client.request(str) << std::endl;
    }
}
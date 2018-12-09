#include "server_terminal.h"

int main(void) {
    server_terminal server;
    const auto init_result = server.init();
    if (init_result) {
        std::cout << "TCP server init Error";
        return init_result;
    }
    return server.run();
}
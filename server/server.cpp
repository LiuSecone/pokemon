#include "tcp_server.h"

int main(void) {
    tcp_server server;
    const auto init_result = server.init();
    if (init_result) {
        std::cout << "TCP server init Error";
        return init_result;
    }
    return server.run();
}
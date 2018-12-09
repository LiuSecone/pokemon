#pragma once

#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <string>
#include <ws2tcpip.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <thread>
#include <mutex>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

class tcp_server {
private:
    static constexpr int default_buff_len = 10;
    static constexpr char default_prot[] = "60000";
    SOCKET listen_socket_ = INVALID_SOCKET;
    SOCKET client_socket_ = INVALID_SOCKET;

    struct addrinfo *result_ = nullptr;
    struct addrinfo hints_;
public:
    tcp_server();
    int init();
    int run();
    int process_client_socket(const SOCKET client_socket);
};


#endif TCP_SERVER_H

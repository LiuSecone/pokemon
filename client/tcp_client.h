#pragma once

#ifndef TCP_CLIENT_H
#define TCP_CLINET_H

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

class tcp_client {
private:
    static constexpr int default_buff_len = 512;
    static constexpr char default_prot[] = "60000";
    SOCKET connect_socket_ = INVALID_SOCKET;

    struct addrinfo *result_ = nullptr;
    struct addrinfo hints_;
public:
    tcp_client();
    int init();
    std::string request(const std::string & str);
    ~tcp_client();
};


#endif //TCPCLIENT_H

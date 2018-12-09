#pragma once

#ifndef SERVER_TERMINAL_H
#define SERVER_TERMINAL_H

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <string>
#include <ws2tcpip.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <thread>
#include <mutex>

#include "../pokemon/user_server.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

class server_terminal {
private:
    static constexpr int default_buff_len = 512;
    static constexpr char default_prot[] = "60000";
    SOCKET listen_socket_ = INVALID_SOCKET;

    struct addrinfo *result_ = nullptr;
    struct addrinfo hints_;

    std::vector<user_server> users_;
public:
    server_terminal();
    int init();
    int init_tcp();
    int init_users();
    int init_heros();
    std::string process_request(const std::string &str);
    int run();
    int process_client_socket(const SOCKET client_socket);
    ~server_terminal();
};


#endif SERVER_TERMINAL_H

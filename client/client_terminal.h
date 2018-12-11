#pragma once

#ifndef CLIENT_TERMINAL_H
#define CLIENT_TERMINAL_H

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

enum class state {
    not_login,
    logged,
    view_user,
    view_hero,
    duel,
    upgrade,
};


class client_terminal {
private:
    static constexpr int default_buff_len = 512;
    static constexpr char default_port[] = "60000";
    SOCKET connect_socket_ = INVALID_SOCKET;

    struct addrinfo *result_ = nullptr;
    struct addrinfo hints_;

    state state_ = state::not_login;

    bool exit_flag_ = false;
    int user_id_ = -1;
    int viewing_user_ = -1;
    int viewing_hero_ = -1;
public:
    client_terminal();
    int init();
    int run();
    void login_or_signin();
    void logout_or_select_user();
    void select_hero();
    std::string request(const std::string & str);
    ~client_terminal();
};


#endif //CLIENT_TERMINAL_H

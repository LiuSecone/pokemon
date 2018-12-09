#include <fstream>

#include "server_terminal.h"
#include "../pokemon/hero.h"

server_terminal::server_terminal() {
    ZeroMemory(&hints_, sizeof(hints_));
    hints_.ai_family = AF_INET;
    hints_.ai_socktype = SOCK_STREAM;
    hints_.ai_protocol = IPPROTO_TCP;
    hints_.ai_flags = AI_PASSIVE;
}

int server_terminal::init() {
    auto return_val = init_tcp();
    if (return_val) {
        return return_val;
    }
    return_val = init_heros();
    if (return_val) {
        return return_val;
    }
    return_val = init_users();
    if (return_val) {
        return return_val;
    }
    return 0;
}

int server_terminal::init_tcp() {
    WSADATA wsa_data;
    auto i_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (i_result != 0) {
        printf("WSAStartup failed with error: %d\n", i_result);
        return 1;
    }
    // Resolve the server address and port
    i_result = getaddrinfo(nullptr, default_prot, &hints_, &result_);
    if (i_result != 0) {
        printf("getaddrinfo failed with error: %d\n", i_result);
        return 1;
    }

    // Create a SOCKET for connecting to server
    listen_socket_ = socket(result_->ai_family, result_->ai_socktype, result_->ai_protocol);
    if (listen_socket_ == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result_);
        return 1;
    }

    // Setup the TCP listening socket
    i_result = bind(listen_socket_, result_->ai_addr, static_cast<int>(result_->ai_addrlen));
    if (i_result == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result_);
        closesocket(listen_socket_);
        return 1;
    }

    freeaddrinfo(result_);

    i_result = listen(listen_socket_, SOMAXCONN);
    if (i_result == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(listen_socket_);
        return 1;
    }
    return 0;
}

int server_terminal::init_users() {
    const std::string file_name = "users.txt";
    std::ifstream input(file_name);
    if (input.is_open()) {
        int number_of_users;
        input >> number_of_users;
        for (auto i = 0; i != number_of_users; ++i) {
            std::string user_name;
            std::string password_hash;
            auto number_of_heros = 0;
            input >> user_name >> password_hash >> number_of_heros;
            user_server user(std::move(user_name), std::move(password_hash));
            for (auto i = 0; i != number_of_heros; ++i) {
                auto hero_number = 0;
                input >> hero_number;
                user.heros.push_back(hero_number);
            }
            users_.push_back(user);
        }
        input.close();
    }
    else {
        std::cout << "Could not open the file: " << file_name;
        return 1;
    }
    return 0;
}

int server_terminal::init_heros() {
    const std::string file_name = "heros.txt";
    std::ifstream input(file_name);
    if (input.is_open()) {
        int number_of_heros;
        input >> number_of_heros;
        for (auto i = 0; i != number_of_heros; ++i) {
            std::string hero_type;
            //TODO: add the init heros
        }
        input.close();
    }
    else {
        std::cout << "Could not open the file: " << file_name;
        return 1;
    }
    return 0;
}

std::string server_terminal::process_request(const std::string &str) {
    return str;
}

int server_terminal::run() {
    while (true) { 
        // Accept a client socket
        const auto client_socket = accept(listen_socket_, NULL, NULL);
        if (client_socket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(listen_socket_);
        }
        else {
            process_client_socket(client_socket);
        }
    }
}

int server_terminal::process_client_socket(const SOCKET client_socket) {
    // Receive until the peer shuts down the connection
    char recvbuf[default_buff_len];
    int i_result;
    do {
        i_result = recv(client_socket, recvbuf, default_buff_len, 0);
        if (i_result > 0) {
            printf("Bytes received: %d\n", i_result);
            recvbuf[i_result] = '\0';
            const std::string recv_string = recvbuf;
            const auto requ_string = process_request(recv_string);
            const auto send_len = static_cast<int>(requ_string.length());
            char sendbuf[default_buff_len];
            strcpy_s(sendbuf, requ_string.c_str());
            // Echo the buffer back to the sender
            const auto i_send_result = send(client_socket, sendbuf, send_len, 0);
            if (i_send_result == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(client_socket);
                return 1;
            }
            printf("Bytes sent: %d\n", i_send_result);
        }
        else if (i_result == 0)
            printf("Connection closing...\n");
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(client_socket);
            return 1;
        }
    } while (i_result > 0);

    // shutdown the connection since we're done
    i_result = shutdown(client_socket, SD_SEND);
    if (i_result == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(client_socket);
        return 1;
    }

    // cleanup
    closesocket(client_socket);

    return 0;
}

server_terminal::~server_terminal() {
    closesocket(listen_socket_);
    WSACleanup();
}

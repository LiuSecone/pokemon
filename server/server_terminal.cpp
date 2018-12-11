#include <fstream>

#include "server_terminal.h"
#include "../pokemon/hero.h"
#include "../pokemon/my_algo_lib.h"

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
    return_val = init_heroes();
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
    i_result = getaddrinfo(nullptr, default_port, &hints_, &result_);
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
        std::cout << "Init users sucessful!" << std::endl;
    }
    else {
        std::cout << "Could not open the file: " << file_name;
        return 1;
    }
    return 0;
}

int server_terminal::init_heroes() {
    const std::string file_name = "heros.txt";
    std::ifstream input(file_name);
    if (input.is_open()) {
        int number_of_heroes;
        input >> number_of_heroes;
        for (auto i = 0; i != number_of_heroes; ++i) {
            /*
            std::string hero_type;
            std::string name;
            double strength;
            double agility;
            double intelligence;
            double strength_growth;
            double agility_growth;
            double intelligence_growth;
            std::size_t level;
            std::size_t exp;
            double damage;
            double health;
            double attack_time;
            double armor;
            input >> hero_type >> name >> strength >> agility >> intelligence
                >> strength_growth >> agility_growth >> intelligence_growth
                >> level >> exp >> damage >> health >> attack_time >> armor;
            */
            std::string hero;
            std::getline(input, hero);
            heroes_.push_back(hero);
        }
        input.close();
        std::cout << "Init heros sucessful!" << std::endl;
    }
    else {
        std::cout << "Could not open the file: " << file_name;
        return 1;
    }
    return 0;
}

int server_terminal::login(const std::string & name, const std::string & hash) {
    for (auto iter = users_.begin(); iter != users_.end(); ++ iter) {
        if (iter->user_name == name && iter->password_hash == hash) {
            return static_cast<int>(users_.begin() - iter);
        }
    }
    return -1;
}

bool server_terminal::signin(const std::string & name, const std::string & hash) {
    auto name_used = false;
    for (auto & user : users_) {
        if (user.user_name == name) {
            name_used = true;
            break;
        }
    }
    return true;
}

std::string server_terminal::process_request(const std::string &str) {
    std::string replay_string;
    auto request_vector = my_algo_lib::split(str, ' ');
    if (request_vector.empty()) {
        return "Empty request!";
    }
    if (request_vector[0] == "login") {
        if (request_vector.size() != 3) {
            replay_string = "Loging fialed, too less or too many param.";
        }
        auto login_reply = login(request_vector[1], request_vector[2]);
        if (login_reply != -1) {
            //TODO: reply the user info
        }
        else {
            replay_string = "Wrong username or password.";
        }
    }
    //TODO: process request
    return replay_string;
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

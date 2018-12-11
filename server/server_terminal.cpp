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
            user_server user(user_name, password_hash);
            for (auto i = 0; i != number_of_heros; ++i) {
                auto hero_number = 0;
                input >> hero_number;
                user.heroes.push_back(hero_number);
            }
            users_.push_back(user);
        }
        input.close();
        std::cout << "Init users successful!" << std::endl;
    }
    else {
        std::cout << "Could not open the file: " << file_name;
        return 1;
    }
    return 0;
}

int server_terminal::init_heroes() {
    const std::string file_name = "heroes.txt";
    std::ifstream input(file_name);
    if (input.is_open()) {
        int number_of_heroes;
        input >> number_of_heroes;
        std::string hero;
        std::getline(input, hero);
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
            std::getline(input, hero);
            heroes_.push_back(hero);
        }
        input.close();
        std::cout << "Init heroes successful!" << std::endl;
    }
    else {
        std::cout << "Could not open the file: " << file_name;
        return 1;
    }
    return 0;
}

int server_terminal::save_users() {
    const std::string file_name = "users.txt";
    std::ofstream output(file_name);
    if (output.is_open()) {
        output << users_.size() << std::endl;
        for (auto &user : users_) {
            output << user.serialize_the_user() << std::endl;
        }
        output.close();
        std::cout << "Save users successful!" << std::endl;
    }
    else {
        std::cout << "Could not open the file: " << file_name;
        return 1;
    }
    return 0;
}

int server_terminal::save_heroes() {
    const std::string file_name = "heroes.txt";
    std::ofstream output(file_name);
    if (output.is_open()) {
        output << heroes_.size() << std::endl;
        for (auto &hero : heroes_) {
            output << hero << std::endl;
        }
        output.close();
        std::cout << "Save users successful!" << std::endl;
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
            if (iter->online) {
                return -2;
            }
            iter->online = true;
            return static_cast<int>(users_.begin() - iter);
        }
    }
    return -1;
}

void server_terminal::logout(const size_t & user_idx) {
    users_[user_idx].online = false;
}

bool server_terminal::signin(const std::string & name, const std::string & hash) {
    auto name_used_flag = false;
    for (auto & user : users_) {
        if (user.user_name == name) {
            name_used_flag = true;
            break;
        }
    }
    if (!name_used_flag) {
        user_server new_user(name, hash);
        for (auto i = 0; i != 3; ++i) {
            std::shared_ptr<hero> new_hero = nullptr;
            const auto random_val = u_(e_);
            if (random_val == 0) {
                new_hero.reset(new power("new_hero"));
            }
            if (random_val == 1) {
                new_hero.reset(new agile("new_hero"));
            }
            if (random_val == 2) {
                new_hero.reset(new intellectual("new_hero"));
            }
            if (random_val == 3) {
                new_hero.reset(new meat("new_hero"));
            }
            heroes_.push_back(new_hero->serialize_the_hero());
            new_user.heroes.push_back(static_cast<int>(heroes_.size()));
        }
        users_.push_back(new_user);
    }
    return !name_used_flag;
}

std::string server_terminal::get_all_heroes() {
    std::string all_heroes;
    for (auto iter = heroes_.begin(); iter != heroes_.end(); ++ iter) {
        all_heroes += std::to_string(iter - heroes_.begin());
        all_heroes += '-';
        all_heroes += my_algo_lib::split(*iter, ' ')[0];
        all_heroes += '\n';
    }
    return all_heroes;
}

std::string server_terminal::get_all_users() {
    std::string all_users;
    for (auto iter = users_.begin(); iter != users_.end(); ++iter) {
        all_users += std::to_string(iter - users_.begin());
        all_users += '-';
        all_users += iter->user_name;
        all_users += '\n';
    }
    return all_users;
}

std::string server_terminal::get_all_online_users() {
    std::string all_users;
    for (auto iter = users_.begin(); iter != users_.end(); ++iter) {
        if (iter->online) {
            all_users += std::to_string(iter - users_.begin());
            all_users += '-';
            all_users += iter->user_name;
            all_users += '\n';
        }
    }
    return all_users;
}

std::string server_terminal::get_ith_user_heroes(const int & ith) {
    std::string all_heroes;
    auto hero_ids = users_[ith].heroes;
    for (auto & hero_id : hero_ids) {
        all_heroes += std::to_string(hero_id);
        all_heroes += '-';
        all_heroes += my_algo_lib::split(heroes_[hero_id], ' ')[0];
        all_heroes += '\n';
    }
    return all_heroes;
}

std::string server_terminal::process_request(const std::string &str) {
    std::string reply_string;
    auto need_save_users = false;
    auto need_save_heroes = false;
    auto request_vector = my_algo_lib::split(str, '/');
    if (request_vector.empty()) {
        return "Empty request!";
    }
    if (request_vector[0] == "login") {
        std::cout << request_vector.size() << std::endl;
        if (request_vector.size() != 3) {
            reply_string = "Login failed, too less or too many param.";
        }
        else {
            const auto login_reply = login(request_vector[1], request_vector[2]);
            if (login_reply >= 0) {
                reply_string = "Successful!";
                reply_string += ' ';
                reply_string += std::to_string(login_reply);
            }
            if (login_reply == -1) {
                reply_string = "Wrong username or password.";
            }
            if (login_reply == -2) {
                reply_string = "Already logged in.";
            }
        }
    }
    if (request_vector[0] == "logout") {
        if (request_vector.size() != 2) {
            reply_string = "Logout failed, too less or too many param.";
        }
        else {
            logout(std::stoi(request_vector[1]));
            reply_string = "Successful!";
        }
    }
    if (request_vector[0] == "signin") {
        if (request_vector.size() != 3) {
            reply_string = "Login failed, too less or too many param.";
        }
        else {
            const auto signin_reply = signin(request_vector[1], request_vector[2]);
            if (signin_reply) {
                reply_string = "Successful!";
                need_save_heroes = true;
                need_save_users = true;
            }
            else {
                reply_string = "Used username.";
            }
        }
    }
    if (request_vector[0] == "get_all_heroes") {
        reply_string = get_all_heroes();
    }
    if (request_vector[0] == "get_all_users") {
        reply_string = get_all_users();
    }
    if (request_vector[0] == "get_all_online_users") {
        reply_string = get_all_online_users();
    }
    if (request_vector[0] == "get_ith_user_heroes") {
        if (request_vector.size() != 2) {
            reply_string = "Get failed, too less or too many param.";
        }
        else {
            reply_string = get_ith_user_heroes(std::stoi(request_vector[1]));
        }
    }
    if (request_vector[0] == "get_ith_user_ith_hero") {
        if (request_vector.size() != 3) {
            reply_string = "Get failed, too less or too many param.";
        }
        else {
            reply_string = users_[std::stoi(request_vector[1])].heroes[std::stoi(request_vector[2])];
        }
    }
    //TODO: process request
    if (need_save_users) {
        save_users();
    }
    if (need_save_heroes) {
        save_heroes();
    }
    return reply_string;
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

std::default_random_engine server_terminal::e_;
std::uniform_int_distribution<int> server_terminal::u_(0, 3);
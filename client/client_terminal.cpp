#include "client_terminal.h"
#include "../pokemon/my_algo_lib.h"

client_terminal::client_terminal() {
    ZeroMemory(&hints_, sizeof(hints_));
    hints_.ai_family = AF_INET;
    hints_.ai_socktype = SOCK_STREAM;
    hints_.ai_protocol = IPPROTO_TCP;
    hints_.ai_flags = AI_PASSIVE;
}

int client_terminal::init() {
    WSADATA wsa_data;
    auto i_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (i_result != 0) {
        printf("WSAStartup failed with error: %d\n", i_result);
        return 1;
    }
    // Resolve the server address and port
    i_result = getaddrinfo("127.0.0.1", default_port, &hints_, &result_);
    if (i_result != 0) {
        printf("getaddrinfo failed with error: %d\n", i_result);
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for (auto ptr = result_; ptr != nullptr; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        connect_socket_ = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (connect_socket_ == INVALID_SOCKET) {
            printf("socket failed with error: %d\n", WSAGetLastError());
            return 1;
        }

        // Connect to server.
        i_result = connect(connect_socket_, ptr->ai_addr, static_cast<int>(ptr->ai_addrlen));
        if (i_result == SOCKET_ERROR) {
            closesocket(connect_socket_);
            connect_socket_ = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result_);

    if (connect_socket_ == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        return 1;
    }

    return 0;
}

int client_terminal::run() {
    while (!exit_flag_) {
        switch (state_) {
        case state::not_login:
            login_or_signin();
            break;
        case state::logged:
            logout_or_select_user();
            break;
        case state::view_user:
            break;
        case state::view_hero:
            break;
        case state::duel:
            break;
        case state::upgrade:
            break;
        }
    }
    return 0;
}

void client_terminal::login_or_signin() {
    std::string request_string;
    int ans;
    std::cout << "1.login" << std::endl
        << "2.signin" << std::endl
        << "0.quit" << std::endl;
    std::cin >> ans;
    if (ans == 1) {
        request_string += "login/";
    }
    if (ans == 2) {
        request_string += "signin/";
    }
    if (ans == 0 ) {
        exit_flag_ = true;
        return;
    }
    std::cout << "Please input your username and password, split with space or enter." << std::endl;
    std::string username, password;
    std::cin >> username >> password;
    request_string += username + '/' + password;
    const auto reply_string = request(request_string);
    auto reply_vector = my_algo_lib::split(reply_string, ' ');
    std::cout << reply_string << std::endl;
    if (reply_vector[0] == "Successful!") {
        state_ = state::logged;
        user_id_ = std::stoi(reply_vector[1]);
    }
}

void client_terminal::logout_or_select_user() {
    std::string request_string;
    int ans;
    std::cout << "1.view all user" << std::endl
        << "2.view all online user" << std::endl
        << "3.select a user by id" << std::endl
        << "0.logout" << std::endl;
    std::cin >> ans;
    if (ans == 1) {
        request_string += "get_all_users";
    }
    if (ans == 2) {
        request_string += "get_all_online_users";
    }
    if (ans == 0) {
        request_string += "logout/";
        request_string += std::to_string(user_id_);
    }
    const auto reply_string = request(request_string);
    std::cout << reply_string << std::endl;
    if (ans == 3) {
        std::cout << "Please input the user ID:" << std::endl;
        int selected_id;
        std::cin >> selected_id;
        viewing_user_ = selected_id;
        state_ = state::view_user;
    }
    if (ans == 0) {
        state_ = state::not_login;
    }
}

void client_terminal::select_hero() {
    std::string request_string;
    int ans;
    std::cout << "1.view all heroes" << std::endl
        << "0.back to select user" << std::endl;
    std::cin >> ans;
    if (ans == 1) {
        request_string += "get_all_heroes/";
        request_string += std::to_string(viewing_user_);
    }
    const auto reply_string = request(request_string);
    std::cout << reply_string << std::endl;
    if (ans == 1) {
        std::cout << "Please input the user ID:" << std::endl;
        int selected_id;
        std::cin >> selected_id;
        viewing_hero_ = selected_id;
        state_ = state::view_hero;
    }
    if (ans == 0) {
        state_ = state::view_user;
    }
}

std::string client_terminal::request(const std::string & str) {
    const auto init_result = init();
    if (init_result) {
        return "TCP client init Error";
    }

    if (str.length() > default_buff_len) {
        std::cout << "The string is too long" << std::endl;
        return str;
    }
    char sendbuf[default_buff_len];
    char recvbuf[default_buff_len];
    strcpy_s(sendbuf, str.c_str());
    auto i_result = send(connect_socket_, sendbuf, static_cast<char>(strlen(sendbuf)), 0);
    if (i_result == SOCKET_ERROR) {
        printf("Send failed with error: %d\n", WSAGetLastError());
        closesocket(connect_socket_);
        return str;
    }

    // shutdown the connection since no more data will be sent
    i_result = shutdown(connect_socket_, SD_SEND);
    if (i_result == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(connect_socket_);
        return str;
    }

    // Receive
    i_result = recv(connect_socket_, recvbuf, default_buff_len, 0);
    if (i_result < 0) {
        printf("recv failed with error: %d\n", WSAGetLastError());
    }
    recvbuf[i_result] = '\0';
    return recvbuf;
}

client_terminal::~client_terminal() {
    closesocket(connect_socket_);
    WSACleanup();
}

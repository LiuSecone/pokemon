#include "client_terminal.h"

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

std::string client_terminal::request(const std::string & str) {
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

    printf("Bytes Sent: %d\n", i_result);

    // shutdown the connection since no more data will be sent
    i_result = shutdown(connect_socket_, SD_SEND);
    if (i_result == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(connect_socket_);
        return str;
    }

    // Receive
    i_result = recv(connect_socket_, recvbuf, default_buff_len, 0);
    if (i_result > 0)
        printf("Bytes received: %d\n", i_result);
    else if (i_result == 0)
        printf("Connection closed\n");
    else
        printf("recv failed with error: %d\n", WSAGetLastError());
    recvbuf[i_result] = '\0';
    return recvbuf;
}

client_terminal::~client_terminal() {
    closesocket(connect_socket_);
    WSACleanup();
}

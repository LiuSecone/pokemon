#include "tcp_server.h"

tcp_server::tcp_server() {
    ZeroMemory(&hints_, sizeof(hints_));
    hints_.ai_family = AF_INET;
    hints_.ai_socktype = SOCK_STREAM;
    hints_.ai_protocol = IPPROTO_TCP;
    hints_.ai_flags = AI_PASSIVE;
}

int tcp_server::init() {
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
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    listen_socket_ = socket(result_->ai_family, result_->ai_socktype, result_->ai_protocol);
    if (listen_socket_ == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result_);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    i_result = bind(listen_socket_, result_->ai_addr, static_cast<int>(result_->ai_addrlen));
    if (i_result == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result_);
        closesocket(listen_socket_);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result_);

    i_result = listen(listen_socket_, SOMAXCONN);
    if (i_result == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(listen_socket_);
        WSACleanup();
        return 1;
    }
    return 0;
}

int tcp_server::run() {
    while (true) {
        // Accept a client socket
        client_socket_ = accept(listen_socket_, NULL, NULL);
        if (client_socket_ == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(listen_socket_);
            WSACleanup();
            return 1;
        }
        else {
            process_client_socket(client_socket_);
        }
    }
}

int tcp_server::process_client_socket(const SOCKET client_socket) {
    // Receive until the peer shuts down the connection
    char recvbuf[default_buff_len];
    int i_result;
    do {
        i_result = recv(client_socket, recvbuf, default_buff_len, 0);
        if (i_result > 0) {
            printf("Bytes received: %d\n", i_result);

            // Echo the buffer back to the sender
            auto iSendResult = send(client_socket, recvbuf, i_result, 0);
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(client_socket);
                WSACleanup();
                return 1;
            }
            printf("Bytes sent: %d\n", iSendResult);
        }
        else if (i_result == 0)
            printf("Connection closing...\n");
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(client_socket);
            WSACleanup();
            return 1;
        }

    } while (i_result > 0);

    // shutdown the connection since we're done
    i_result = shutdown(client_socket, SD_SEND);
    if (i_result == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(client_socket);
    WSACleanup();

    return 0;
}

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
            select_hero();
            break;
        case state::view_hero:
            show_detail_or_fight();
            break;
        case state::duel:
            duel_fight();
            break;
        case state::upgrade:
            upgrade_fight();
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
        std::cout << "Please input your username and password, split with space or enter." << std::endl;
        std::string username, password;
        std::cin >> username >> password;
        request_string += "login/";
        request_string += username + '/' + password;
        const auto reply_string = post_request(request_string);
        std::cout << reply_string << std::endl;
        auto reply_vector = my_algo_lib::split(reply_string, ' ');
        if (reply_vector[0] == "Successful!") {
            state_ = state::logged;
            user_id_ = std::stoi(reply_vector[1]);
        }
    }
    if (ans == 2) {
        std::cout << "Please input your username and password, split with space or enter." << std::endl;
        std::string username, password;
        std::cin >> username >> password;
        request_string += "signin/";
        request_string += username + '/' + password;
        const auto reply_string = post_request(request_string);
        std::cout << reply_string << std::endl;
    }
    if (ans == 0 ) {
        exit_flag_ = true;
        return;
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
        const auto reply_string = post_request(request_string);
        std::cout << reply_string << std::endl;
    }
    if (ans == 2) {
        request_string += "get_all_online_users";
        const auto reply_string = post_request(request_string);
        std::cout << reply_string << std::endl;
    }
    if (ans == 3) {
        std::cout << "Please input the user ID:" << std::endl;
        int selected_id;
        std::cin >> selected_id;
        viewing_user_ = selected_id;
        state_ = state::view_user;
    }
    if (ans == 0) {
        request_string += "logout/";
        request_string += std::to_string(user_id_);
        const auto reply_string = post_request(request_string);
        std::cout << reply_string << std::endl;
        state_ = state::not_login;
    }
}

void client_terminal::select_hero() {
    std::string request_string;
    int ans;
    std::cout << "1.view all hero" << std::endl
        << "2.select a hero" << std::endl
        << "0.back to select user" << std::endl;
    std::cin >> ans;
    if (ans == 1) {
        request_string += "get_ith_user_heroes/";
        request_string += std::to_string(viewing_user_);
        const auto reply_string = post_request(request_string);
        std::cout << reply_string << std::endl;
    }
    if (ans == 2) {
        std::cout << "Please input the hero ID:" << std::endl;
        int selected_id;
        std::cin >> selected_id;
        viewing_hero_ = selected_id;
        state_ = state::view_hero;
    }
    if (ans == 0) {
        state_ = state::logged;
    }
}

void client_terminal::show_detail_or_fight() {
    std::string request_string;
    int ans;
    if (viewing_user_ == user_id_) {
        std::cout << "1.duel fight" << std::endl
            << "2.upgrade fight" << std::endl
            << "3.rename hero" << std::endl;
    }
    std::cout << "4.view hero detail" << std::endl
        << "0.back to select heroes" << std::endl;
    std::cin >> ans;
    if (viewing_user_ == user_id_ && ans == 1) {
        state_ = state::duel;
    }
    if (viewing_user_ == user_id_ && ans == 2) {
        state_ = state::upgrade;
    }
    if (viewing_user_ == user_id_ && ans == 3) {
        //TODO: rename the hero
    }
    if (ans == 4) {
        request_string += "get_ith_user_ith_hero/";
        request_string += std::to_string(viewing_user_);
        request_string += '/';
        request_string += std::to_string(viewing_hero_);
        const auto reply_string = post_request(request_string);
        std::cout << reply_string << std::endl;
    }
    if (ans == 0) {
        state_ = state::view_user;
    }
}

void client_terminal::duel_fight() {
    const auto cur_hero = get_selected_hero();
    const auto opponent = choose_opponent();
    const auto winner = fight_and_get_winner(cur_hero, opponent);
    std::cout << winner << std::endl;
    state_ = state::view_hero;
}

void client_terminal::upgrade_fight() {
    auto opponent = choose_opponent();
    state_ = state::view_hero;
}

int client_terminal::fight_and_get_winner(const std::shared_ptr<hero> &h1, const std::shared_ptr<hero> &h2) {
    auto attack_flag = false;
    std::cout << h1->get_health() << '/' << h2->get_health() << std::endl;
    while (h1->get_health() != 0 && h2->get_health() != 0) {
        std::cout << h1->get_health() << ' ' << h2->get_health() << std::endl;
        if (attack_flag) {
            attack_trajectory trajectory;
            h1->generate_damage(trajectory);
            h2->get_damage(trajectory);
        }
        else {
            attack_trajectory trajectory;
            h2->generate_damage(trajectory);
            h1->get_damage(trajectory);
        }
        attack_flag = !attack_flag;
    }
    if (h2->get_health() == 0) {
        return 1;
    }
    else {
        return 0;
    }
}

std::shared_ptr<hero> client_terminal::get_selected_hero() {
    std::string request_string = "get_ith_user_ith_hero/";
    request_string += std::to_string(viewing_user_);
    request_string += '/';
    request_string += std::to_string(viewing_hero_);
    std::cout << request_string << std::endl;
    const auto reply_string = post_request(request_string);
    std::cout << reply_string << std::endl;
    auto const element = my_algo_lib::split(reply_string, ' ');
    std::shared_ptr<hero> selected_hero = nullptr;
    if (element.back() == "power") {
        selected_hero.reset(new power(element[0], std::stoi(element[1]), std::stoi(element[2]), 
            std::stoi(element[3]), std::stoi(element[4]), std::stoi(element[5]), std::stoi(element[6]), 
            std::stoi(element[7]), std::stoi(element[8]), std::stoi(element[9]), std::stoi(element[10]),
            std::stoi(element[11]), std::stoi(element[12]), element[13]));
    }
    if (element.back() == "agile") {
        selected_hero.reset(new agile(element[0], std::stoi(element[1]), std::stoi(element[2]),
            std::stoi(element[3]), std::stoi(element[4]), std::stoi(element[5]), std::stoi(element[6]),
            std::stoi(element[7]), std::stoi(element[8]), std::stoi(element[9]), std::stoi(element[10]),
            std::stoi(element[11]), std::stoi(element[12]), element[13]));
    }
    if (element.back() == "intellectual") {
        selected_hero.reset(new intellectual(element[0], std::stoi(element[1]), std::stoi(element[2]),
            std::stoi(element[3]), std::stoi(element[4]), std::stoi(element[5]), std::stoi(element[6]),
            std::stoi(element[7]), std::stoi(element[8]), std::stoi(element[9]), std::stoi(element[10]),
            std::stoi(element[11]), std::stoi(element[12]), element[13]));
    }
    if (element.back() == "meat") {
        selected_hero.reset(new meat(element[0], std::stoi(element[1]), std::stoi(element[2]),
            std::stoi(element[3]), std::stoi(element[4]), std::stoi(element[5]), std::stoi(element[6]),
            std::stoi(element[7]), std::stoi(element[8]), std::stoi(element[9]), std::stoi(element[10]),
            std::stoi(element[11]), std::stoi(element[12]), element[13]));
    }
    return selected_hero;
}

std::shared_ptr<hero> client_terminal::choose_opponent() {
    std::default_random_engine e;
    std::uniform_int_distribution<int> u(0, 3);
    std::vector<std::shared_ptr<hero>> opponents;
    for (auto i = 0; i != 3; ++i) {
        std::shared_ptr<hero> new_hero = nullptr;
        const auto random_val = u(e);
        std::string new_hero_name = "system";
        if (random_val == 0) {
            new_hero.reset(new power(new_hero_name));
        }
        if (random_val == 1) {
            new_hero.reset(new agile(new_hero_name));
        }
        if (random_val == 2) {
            new_hero.reset(new intellectual(new_hero_name));
        }
        if (random_val == 3) {
            new_hero.reset(new meat(new_hero_name));
        }
        opponents.push_back(new_hero);
    }
    for (auto iter = opponents.begin(); iter != opponents.end(); ++iter) {
        std::cout << iter - opponents.begin() << ' ';
        std::cout << (*iter)->serialize_the_hero() << std::endl;
    }
    std::cout << "Please select the hero you want to fight with:" << std::endl;
    int ans;
    std::cin >> ans;
    return opponents[ans];
}

std::string client_terminal::post_request(const std::string & str) {
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

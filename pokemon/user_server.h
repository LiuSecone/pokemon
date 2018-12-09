#pragma once

#ifndef USER_SERVER_H
#define USER_SERVER_H

#include <string>
#include <vector>

#include "hero.h"

class user_server {
public:
    const std::string user_name;
    const std::string password_hash;
    std::vector<std::shared_ptr<hero>> heros;
    explicit user_server(std::string &&name, std::string &&hash) :
    user_name(name),
    password_hash(hash) {
    }
};

#endif //USER_SERVER_H

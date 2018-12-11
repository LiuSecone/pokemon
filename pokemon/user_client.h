#pragma once
#ifndef USER_CLIENT_H
#define USER_CLIENT_H

#include <string>
#include <vector>

#include "hero.h"

class user_client {
public:
    const std::string user_name;
    std::vector<std::shared_ptr<hero>> heroes;
    explicit user_client(std::string &&name) :user_name(name) {}
};

#endif //USER_CLIENT_H

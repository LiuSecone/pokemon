#pragma once
#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

#include "hero.h"

class user {
public:
    const std::string user_name;
    std::vector<std::shared_ptr<hero>> heros;
    explicit user(std::string &&name) :user_name(name) {}
};

#endif //USER_H

#pragma once

#ifndef USER_SERVER_H
#define USER_SERVER_H

#include <string>
#include <vector>

class user_server {
public:
    const std::string user_name;
    const std::string password_hash;
    std::vector<int> heroes;
    std::size_t win = 0;
    std::size_t all = 0;
    bool online = false;
    std::string serialize_the_user() const;
    explicit user_server(const std::string &name, const std::string &hash,
        const std::size_t & win = 0, const std::size_t & all = 0) :
    user_name(name),
    password_hash(hash) {
    }
};

#endif //USER_SERVER_H

#include "user_server.h"

std::string user_server::serialize_the_user() const {
    auto serialization = user_name;
    serialization += ' ';
    serialization += password_hash;
    serialization += ' ';
    serialization += std::to_string(heroes.size());
    for (auto i = 0; i != heroes.size(); ++i) {
        serialization += ' ';
        serialization += std::to_string(heroes[i]);
    }
    return serialization;
}

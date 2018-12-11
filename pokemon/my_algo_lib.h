#pragma once
#ifndef MY_ALGO_LIB_H
#define MY_ALGO_LIB_H

#include <string>
#include <sstream>
#include <vector>

class my_algo_lib {
public:
    static std::vector<std::string> split(const std::string &s, const char delim) {
        std::stringstream ss(s);
        std::string item;
        std::vector<std::string> elems;
        while (std::getline(ss, item, delim)) {
            if (!item.empty()) {
                elems.push_back(std::move(item));
            }
        }
        return elems;
    }
};

#endif MY_ALGO_LIB_H

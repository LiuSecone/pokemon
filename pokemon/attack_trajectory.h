#pragma once

#ifndef ATTACK_TRAJECTORY_H
#define ATTACK_TRAJECTORY_H

#include <vector>

enum class damage_type {
    none,
    melee_damage,
    remote_damage,
    soul_damage,
    punch_damage,
    pure_damage,
};

enum class damage_special_effect {
    crit,
    bloodsucking,
};

class attack_trajectory {
public:
    damage_type the_type_of_attack = damage_type::none;
    std::vector<damage_special_effect> the_special_damage_effects;
    double generated_damage_number = 0;
    double caused_damage_number = 0;
    double crit_factor = 1.0;
};

#endif //ATTACK_TRAJECTORY_H
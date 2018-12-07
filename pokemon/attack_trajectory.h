#pragma once

#ifndef ATTACK_TRAJECTORY_H
#define ATTACK_TRAJECTORY_H

#include <vector>

enum class damage_type {
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
    damage_type the_type_of_attack;
    std::vector<damage_special_effect> the_special_damage_effects;
    double generated_damage_number;
    double caused_damage_number;
    double crit_factor;
};

#endif //ATTACK_TRAJECTORY_H
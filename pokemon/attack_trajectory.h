﻿#pragma once

#ifndef ATTACK_TRAJECTORY_H
#define ATTACK_TRAJECTORY_H

enum class attack_type {
    melee_attack,
    remote_attack,
    soul_attack,
    punch_attack,
};

enum class attack_special_effect {
    crit,
    bloodsucking,
};

class attack_trajectory {
public:
    attack_type the_type_of_attack;
    attack_special_effect the_special_attack_effect;
    double damage_number;
};

#endif //ATTACK_TRAJECTORY_H
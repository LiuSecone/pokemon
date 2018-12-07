#include "pch.h"
#include "hero.h"

hero & hero::basic_level_up() {
    ++level_;
    strength_ += strength_growth_;
    agility_ += agility_growth_;
    intelligence_ += intelligence_growth_;
    health_ = count_filled_health(strength_);
    return *this;
}

double hero::count_filled_health(const double & strength) {
    const auto health = basic_health + strength;
    return health;
}

std::size_t hero::get_level() const {
    return level_;
}

std::size_t hero::get_exp() const {
    return exp_;
}

std::size_t hero::set_hero_cnt(const std::size_t & cnt) {
    hero_cnt_ = cnt;
    return hero_cnt_;
}

double hero::get_health() const {
    return health_;
}

hero & hero::gain_exp(const std::size_t & exp) {
    exp_ += exp;
    while (level_ != max_level && exp_ >= max_exp) {
        exp_ -= max_exp;
        this->level_up();
    }
    while (level_ == max_level && exp_ > max_exp) {
        exp_ -= max_exp;
    }
    return *this;
}

hero & power::level_up() {
    if (level_ < max_level) {
        strength_ += basic_main_properties_growth;
        this->basic_level_up();
    }
    return *this;
}

hero & power::get_damage(attack_trajectory & trajectory) {
    const auto actual_armor = basic_armor + armor_ + intelligence_ * armor_get_by_agility_factor;
    const auto damage_factor =
        1.0 - (armor_count_factor * actual_armor) / (1.0 + armor_count_factor * abs(actual_armor));
    if (trajectory.the_type_of_attack == damage_type::pure_damage) {
        trajectory.caused_damage_number = trajectory.generated_damage_number;
    }
    if (trajectory.the_type_of_attack == damage_type::melee_damage) {
        trajectory.caused_damage_number = trajectory.generated_damage_number * damage_factor;
    }
    if (trajectory.the_type_of_attack == damage_type::remote_damage) {
        const auto miss_prohibit = u_(e_);
        if (miss_prohibit >= remote_attack_miss_prohibit) {
            trajectory.caused_damage_number = trajectory.generated_damage_number * damage_factor;
        }
        else {
            trajectory.caused_damage_number = 0;
        }
    }
    if (health_ >= trajectory.caused_damage_number) {
        health_ -= trajectory.caused_damage_number;
    }
    else {
        trajectory.caused_damage_number = health_;
        health_ = 0;
    }
    return *this;
}

hero & power::generate_damage(attack_trajectory & trajectory) {
    trajectory.generated_damage_number = damage_ + strength_;
    trajectory.the_type_of_attack = damage_type::melee_damage;
    return *this;
}

hero & agile::level_up() {
    if (level_ < max_level) {
        agility_ += basic_main_properties_growth;
        this->basic_level_up();
    }
    return *this;
}

hero & agile::get_damage(attack_trajectory & trajectory) {
    const auto actual_armor = basic_armor + armor_ + intelligence_ * armor_get_by_agility_factor;
    const auto damage_factor =
        1.0 - (armor_count_factor * actual_armor) / (1.0 + armor_count_factor * abs(actual_armor));
    if (trajectory.the_type_of_attack == damage_type::pure_damage) {
        trajectory.caused_damage_number = trajectory.generated_damage_number;
    }
    if (trajectory.the_type_of_attack == damage_type::melee_damage) {
        trajectory.caused_damage_number = trajectory.generated_damage_number * damage_factor;
    }
    if (trajectory.the_type_of_attack == damage_type::remote_damage) {
        const auto miss_prohibit = u_(e_);
        if (miss_prohibit >= remote_attack_miss_prohibit) {
            trajectory.caused_damage_number = trajectory.generated_damage_number * damage_factor;
        }
        else {
            trajectory.caused_damage_number = 0;
        }
    }
    if (health_ >= trajectory.caused_damage_number) {
        health_ -= trajectory.caused_damage_number;
    }
    else {
        trajectory.caused_damage_number = health_;
        health_ = 0;
    }
    return *this;
}

hero & agile::generate_damage(attack_trajectory & trajectory) {
    trajectory.generated_damage_number = damage_ + agility_;
    trajectory.the_type_of_attack = damage_type::remote_damage;
    return *this;
}

hero & intellectual::level_up() {
    if (level_ < max_level) {
        intelligence_ += basic_main_properties_growth;
        this->basic_level_up();
    }
    return *this;
}

hero & intellectual::get_damage(attack_trajectory & trajectory) {
    const auto actual_armor = basic_armor + armor_ + intelligence_ * armor_get_by_agility_factor;
    const auto damage_factor =
        1.0 - (armor_count_factor * actual_armor) / (1.0 + armor_count_factor * abs(actual_armor));
    if (trajectory.the_type_of_attack == damage_type::pure_damage) {
        trajectory.caused_damage_number = trajectory.generated_damage_number;
    }
    if (trajectory.the_type_of_attack == damage_type::melee_damage) {
        trajectory.caused_damage_number = trajectory.generated_damage_number * damage_factor;
    }
    if (trajectory.the_type_of_attack == damage_type::remote_damage) {
        const auto miss_prohibit = u_(e_);
        if (miss_prohibit >= remote_attack_miss_prohibit) {
            trajectory.caused_damage_number = trajectory.generated_damage_number * damage_factor;
        }
        else {
            trajectory.caused_damage_number = 0;
        }
    }
    if (health_ >= trajectory.caused_damage_number) {
        health_ -= trajectory.caused_damage_number;
    }
    else {
        trajectory.caused_damage_number = health_;
        health_ = 0;
    }
    return *this;
}

hero & intellectual::generate_damage(attack_trajectory & trajectory) {
    trajectory.generated_damage_number = damage_ + intelligence_;
    trajectory.the_type_of_attack = damage_type::remote_damage;
    return *this;
}

hero & meat::level_up() {
    if (level_ < max_level) {
        ++armor_;
        this->basic_level_up();
    }
    return *this;
}

hero & meat::get_damage(attack_trajectory & trajectory) {
    const auto actual_armor = basic_armor + armor_ + intelligence_ * armor_get_by_agility_factor;
    const auto damage_factor =
        1.0 - (armor_count_factor * actual_armor) / (1.0 + armor_count_factor * abs(actual_armor));
    if (trajectory.the_type_of_attack == damage_type::pure_damage) {
        trajectory.caused_damage_number = trajectory.generated_damage_number;
    }
    if (trajectory.the_type_of_attack == damage_type::melee_damage) {
        trajectory.caused_damage_number = trajectory.generated_damage_number * damage_factor;
    }
    if (trajectory.the_type_of_attack == damage_type::remote_damage) {
        const auto miss_prohibit = u_(e_);
        if (miss_prohibit >= remote_attack_miss_prohibit) {
            trajectory.caused_damage_number = trajectory.generated_damage_number * damage_factor;
        }
        else {
            trajectory.caused_damage_number = 0;
        }
    }
    if (health_ >= trajectory.caused_damage_number) {
        health_ -= trajectory.caused_damage_number;
    }
    else {
        trajectory.caused_damage_number = health_;
        health_ = 0;
    }
    return *this;
}

hero & meat::generate_damage(attack_trajectory & trajectory) {
    trajectory.generated_damage_number = damage_ + strength_;
    trajectory.the_type_of_attack = damage_type::melee_damage;
    return *this;
}

std::default_random_engine hero::e_;
std::size_t hero::hero_cnt_ = 1;
std::uniform_real_distribution<double> hero::u_(0, 1);
std::normal_distribution<double> hero::n_(5, 3);

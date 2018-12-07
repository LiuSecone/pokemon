#include "pch.h"
#include "hero.h"

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

double hero::get_health() const{
    return health_;
}

hero & hero::gain_exp(const std::size_t & exp) {
    exp_ += exp;
    while (level_ != max_level && exp_ >= max_exp) {
        exp_ -= max_exp;
        this->level_up();
        health_ = count_filled_health(strength_);
    }
    while (level_ == max_level && exp_ > max_exp) {
        exp_ -= max_exp;
    }
    return *this;
}

hero & power::level_up() {
    if (level_ < max_level) {
        ++level_;
        strength_ += strength_growth_ + basic_main_properties_growth;
        agility_ += agility_growth_;
        intelligence_ += intelligence_growth_;
    }
    return *this;
}

hero & power::get_damage(const double & attack_trajectory) {
    return *this;
}

hero & power::generate_damage() {
    return *this;
}

hero & agile::level_up() {
    if (level_ < max_level) {
        ++level_;
        strength_ += strength_growth_;
        agility_ += agility_growth_ + basic_main_properties_growth;
        intelligence_ += intelligence_growth_;
    }
    return *this;
}

hero & agile::get_damage(const double & attack_trajectory) {
    return *this;
}

hero & agile::generate_damage() {
    return *this;
}

hero & intellectual::level_up() {
    if (level_ < max_level) {
        ++level_;
        strength_ += strength_growth_;
        agility_ += agility_growth_;
        intelligence_ += intelligence_growth_ + basic_main_properties_growth;
    }
    return *this;
}

hero & intellectual::get_damage(const double & attack_trajectory) {
    return *this;
}

hero & intellectual::generate_damage() {
    return *this;
}

hero & meat::level_up() {
    if (level_ < max_level) {
        ++level_;
        strength_ += strength_growth_;
        agility_ += agility_growth_;
        intelligence_ += intelligence_growth_;
    }
    return *this;
}

hero & meat::get_damage(const double & attack_trajectory) {
    return *this;
}

hero & meat::generate_damage() {
    return *this;
}

std::default_random_engine hero::e_;
std::size_t hero::hero_cnt_ = 1;
std::uniform_real_distribution<double> hero::u_(0, 1);
std::normal_distribution<double> hero::n_(5, 3);

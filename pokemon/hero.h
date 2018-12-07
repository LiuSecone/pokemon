#pragma once
#ifndef HERO_H
#define HERO_H

#include <random>
#include <string>

#include "attack_trajectory.h"

class hero {
private:
    static std::size_t hero_cnt_;
protected:
    static constexpr double basic_properties = 10;
    static constexpr double basic_properties_growth = 3;
    static constexpr double basic_main_properties_growth = 2;
    static constexpr double basic_damage = 10;
    static constexpr double basic_health = 100;
    static constexpr double basic_attack_time = 1.2;
    static constexpr double basic_armor = 1;
    static constexpr std::size_t max_level = 15;
    static constexpr std::size_t max_exp = 1000;
    static std::default_random_engine e_;
    static std::uniform_real_distribution<double> u_;
    static std::normal_distribution<double> n_;
    const std::string name_;
    std::size_t id_;
    double strength_;
    double agility_;
    double intelligence_;
    double strength_growth_;
    double agility_growth_;
    double intelligence_growth_;
    std::size_t level_;
    std::size_t exp_;
    double damage_;
    double health_;
    double attack_time_;
    double armor_;
    virtual hero & level_up() = 0;
public:
    hero() = delete;
    explicit hero(
        std::string &&name,
        const double &strength,
        const double &agility,
        const double &intelligence,
        const double &strength_growth,
        const double &agility_growth,
        const double &intelligence_growth,
        const std::size_t &level,
        const std::size_t &exp,
        const double &damage,
        const double &health,
        const double &attack_time,
        const double &armor)
        :name_(name),
        id_(hero_cnt_),
        strength_(strength),
        agility_(agility),
        intelligence_(intelligence),
        strength_growth_(strength_growth),
        agility_growth_(agility_growth),
        intelligence_growth_(intelligence_growth),
        level_(level),
        exp_(exp),
        damage_(damage),
        health_(health),
        attack_time_(attack_time),
        armor_(armor) {
        ++hero_cnt_;
    }
    explicit hero(
        std::string &&name,
        const std::string &type)
        :name_(name),
        id_(hero_cnt_),
        strength_(basic_properties),
        agility_(basic_properties),
        intelligence_(basic_properties),
        strength_growth_(basic_properties_growth),
        agility_growth_(basic_properties_growth),
        intelligence_growth_(basic_main_properties_growth),
        level_(1),
        exp_(0),
        damage_(basic_damage),
        attack_time_(basic_attack_time),
        armor_(basic_armor) {
        ++hero_cnt_;
        strength_ += u_(e_);
        agility_ += u_(e_);
        intelligence_ += u_(e_);
        strength_growth_ += u_(e_);
        agility_growth_ += u_(e_);
        strength_growth_ += u_(e_);
        damage_ += n_(e_) + u_(e_);
        health_ = count_filled_health(strength_);
        if (type == std::string("power")) {
            strength_ += n_(e_);
            strength_growth_ += basic_main_properties_growth;
        }
        if (type == std::string("agility")) {
            agility_ += n_(e_);
            agility_growth_ += basic_main_properties_growth;
        }
        if (type == std::string("intelligence")) {
            intelligence_ += n_(e_);
            intelligence_growth_ += basic_main_properties_growth;
        }
    }
    hero(const hero&) = delete;
    hero& operator=(const hero&) = delete;
    hero& operator=(hero &&) noexcept = delete;
    hero(hero &&) noexcept = delete;
    static double count_filled_health(const double &strength);
    std::size_t get_level() const;
    std::size_t get_exp() const;
    static std::size_t set_hero_cnt(const std::size_t &cnt);
    double get_health() const;
    hero & gain_exp(const std::size_t &exp);
    virtual hero & get_damage(const double &attack_trajectory) = 0;
    virtual hero & generate_damage() = 0;
    virtual ~hero() = default;
};

class power : public hero {
protected:
    hero & level_up() override;
public:
    power() = delete;
    explicit power(std::string &&name)
        :hero(std::move(name), std::string("power")) {
    }
    explicit power(
        std::string &&name,
        const double &strength,
        const double &agility,
        const double &intelligence,
        const double &strength_growth,
        const double &agility_growth,
        const double &intelligence_growth,
        const std::size_t &level,
        const std::size_t &exp,
        const double &damage,
        const double &health,
        const double &attack_time,
        const double &armor)
        :hero(
            std::move(name),
            strength,
            agility,
            intelligence,
            strength_growth,
            agility_growth,
            intelligence_growth,
            level,
            exp,
            damage,
            health,
            attack_time,
            armor) {
    }
    hero & get_damage(const double &attack_trajectory) override;
    hero & generate_damage() override;
};

class agile : public hero {
protected:
    hero & level_up() override;
public:
    agile() = delete;
    explicit agile(std::string &&name)
        :hero(std::move(name), std::string("agile")) {
    }
    explicit agile(
        std::string &&name,
        const double &strength,
        const double &agility,
        const double &intelligence,
        const double &strength_growth,
        const double &agility_growth,
        const double &intelligence_growth,
        const std::size_t &level,
        const std::size_t &exp,
        const double &damage,
        const double &health,
        const double &attack_time,
        const double &armor)
        :hero(
            std::move(name),
            strength,
            agility,
            intelligence,
            strength_growth,
            agility_growth,
            intelligence_growth,
            level,
            exp,
            damage,
            health,
            attack_time,
            armor) {
    }
    hero & get_damage(const double &attack_trajectory) override;
    hero & generate_damage() override;
};

class intellectual : public hero {
protected:
    hero & level_up() override;
public:
    intellectual() = delete;
    explicit intellectual(std::string &&name)
        :hero(std::move(name), std::string("intellectual")) {
    }
    explicit intellectual(
        std::string &&name,
        const double &strength,
        const double &agility,
        const double &intelligence,
        const double &strength_growth,
        const double &agility_growth,
        const double &intelligence_growth,
        const std::size_t &level,
        const std::size_t &exp,
        const double &damage,
        const double &health,
        const double &attack_time,
        const double &armor)
        :hero(
            std::move(name),
            strength,
            agility,
            intelligence,
            strength_growth,
            agility_growth,
            intelligence_growth,
            level,
            exp,
            damage,
            health,
            attack_time,
            armor) {
    }
    hero & get_damage(const double &attack_trajectory) override;
    hero & generate_damage() override;
};

class meat : public hero {
protected:
    hero & level_up() override;
public:
    meat() = delete;
    explicit meat(std::string &&name)
        :hero(std::move(name), std::string("meat")) {
    }
    explicit meat(
        std::string &&name,
        const double &strength,
        const double &agility,
        const double &intelligence,
        const double &strength_growth,
        const double &agility_growth,
        const double &intelligence_growth,
        const std::size_t &level,
        const std::size_t &exp,
        const double &damage,
        const double &health,
        const double &attack_time,
        const double &armor)
        :hero(
            std::move(name),
            strength,
            agility,
            intelligence,
            strength_growth,
            agility_growth,
            intelligence_growth,
            level,
            exp,
            damage,
            health,
            attack_time,
            armor) {
    }
    hero & get_damage(const double &attack_trajectory) override;
    hero & generate_damage() override;
};

/*
class sk : public power {
public:
    sk() = delete;
    explicit sk(std::string &&name) :power(std::move(name)) {
    }
    explicit sk(
        std::string &&name,
        const double &strength,
        const double &agility,
        const double &intelligence,
        const double &strength_growth,
        const double &agility_growth,
        const double &intelligence_growth,
        const std::size_t &level,
        const std::size_t &exp,
        const double &damage,
        const double &health,
        const double &attack_time,
        const double &armor)
        :power(
            std::move(name),
            strength,
            agility,
            intelligence,
            strength_growth,
            agility_growth,
            intelligence_growth,
            level,
            exp,
            damage,
            health,
            attack_time,
            armor) {
    }
};

class ck : public power {
public:
    ck() = delete;
    explicit ck(std::string &&name) :power(std::move(name)) {
    }
    explicit ck(
        std::string &&name,
        const double &strength,
        const double &agility,
        const double &intelligence,
        const double &strength_growth,
        const double &agility_growth,
        const double &intelligence_growth,
        const std::size_t &level,
        const std::size_t &exp,
        const double &damage,
        const double &health,
        const double &attack_time,
        const double &armor)
        :power(
            std::move(name),
            strength,
            agility,
            intelligence,
            strength_growth,
            agility_growth,
            intelligence_growth,
            level,
            exp,
            damage,
            health,
            attack_time,
            armor) {
    }
};

class pa : public agile {
public:
    pa() = delete;
    explicit pa(std::string &&name) :agile(std::move(name)) {
    }
    explicit pa(
        std::string &&name,
        const double &strength,
        const double &agility,
        const double &intelligence,
        const double &strength_growth,
        const double &agility_growth,
        const double &intelligence_growth,
        const std::size_t &level,
        const std::size_t &exp,
        const double &damage,
        const double &health,
        const double &attack_time,
        const double &armor)
        :agile(
            std::move(name),
            strength,
            agility,
            intelligence,
            strength_growth,
            agility_growth,
            intelligence_growth,
            level,
            exp,
            damage,
            health,
            attack_time,
            armor) {
    }
};

class sf : public agile {
public:
    sf() = delete;
    explicit sf(std::string &&name) :agile(std::move(name)) {
    }
    explicit sf(
        std::string &&name,
        const double &strength,
        const double &agility,
        const double &intelligence,
        const double &strength_growth,
        const double &agility_growth,
        const double &intelligence_growth,
        const std::size_t &level,
        const std::size_t &exp,
        const double &damage,
        const double &health,
        const double &attack_time,
        const double &armor)
        :agile(
            std::move(name),
            strength,
            agility,
            intelligence,
            strength_growth,
            agility_growth,
            intelligence_growth,
            level,
            exp,
            damage,
            health,
            attack_time,
            armor) {
    }
};

class nec : public intellectual {
public:
    nec() = delete;
    explicit nec(std::string &&name) :intellectual(std::move(name)) {
    }
    explicit nec(
        std::string &&name,
        const double &strength,
        const double &agility,
        const double &intelligence,
        const double &strength_growth,
        const double &agility_growth,
        const double &intelligence_growth,
        const std::size_t &level,
        const std::size_t &exp,
        const double &damage,
        const double &health,
        const double &attack_time,
        const double &armor)
        :intellectual(
            std::move(name),
            strength,
            agility,
            intelligence,
            strength_growth,
            agility_growth,
            intelligence_growth,
            level,
            exp,
            damage,
            health,
            attack_time,
            armor) {
    }
};

class aa : public intellectual {
public:
    aa() = delete;
    explicit aa(std::string &&name) :intellectual(std::move(name)) {
    }
    explicit aa(
        std::string &&name,
        const double &strength,
        const double &agility,
        const double &intelligence,
        const double &strength_growth,
        const double &agility_growth,
        const double &intelligence_growth,
        const std::size_t &level,
        const std::size_t &exp,
        const double &damage,
        const double &health,
        const double &attack_time,
        const double &armor)
        :intellectual(
            std::move(name),
            strength,
            agility,
            intelligence,
            strength_growth,
            agility_growth,
            intelligence_growth,
            level,
            exp,
            damage,
            health,
            attack_time,
            armor) {
    }
};

class axe : public meat {
public:
    axe() = delete;
    explicit axe(std::string &&name) :meat(std::move(name)) {
    }
    explicit axe(
        std::string &&name,
        const double &strength,
        const double &agility,
        const double &intelligence,
        const double &strength_growth,
        const double &agility_growth,
        const double &intelligence_growth,
        const std::size_t &level,
        const std::size_t &exp,
        const double &damage,
        const double &health,
        const double &attack_time,
        const double &armor)
        :meat(
            std::move(name),
            strength,
            agility,
            intelligence,
            strength_growth,
            agility_growth,
            intelligence_growth,
            level,
            exp,
            damage,
            health,
            attack_time,
            armor) {
    }
};

class io : public meat {
public:
    io() = delete;
    explicit io(std::string &&name) :meat(std::move(name)) {
    }
    explicit io(
        std::string &&name,
        const double &strength,
        const double &agility,
        const double &intelligence,
        const double &strength_growth,
        const double &agility_growth,
        const double &intelligence_growth,
        const std::size_t &level,
        const std::size_t &exp,
        const double &damage,
        const double &health,
        const double &attack_time,
        const double &armor)
        :meat(
            std::move(name),
            strength,
            agility,
            intelligence,
            strength_growth,
            agility_growth,
            intelligence_growth,
            level,
            exp,
            damage,
            health,
            attack_time,
            armor) {
    }
};
*/

#endif

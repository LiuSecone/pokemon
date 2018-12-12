#include "stdafx.h"
#include "CppUnitTest.h"
#include "../pokemon/hero.h"
#include "../pokemon/my_algo_lib.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{
    TEST_CLASS(hero_tests)
    {
    public:
        TEST_METHOD(init) {
            std::string pa = "PA";
            const std::shared_ptr<hero> ppa(new agile(pa));
            Assert::AreNotEqual(double(0), ppa->get_health());
            Assert::AreEqual(std::size_t(0), ppa->get_exp());
            Assert::AreEqual(std::size_t(1), ppa->get_level());
        }

        TEST_METHOD(gain_exp) {
            std::string pa = "PA";
            const std::shared_ptr<hero> ppa(new agile(pa));
            const auto health = ppa->get_health();
            Assert::AreEqual(std::size_t(0), ppa->get_exp());
            ppa->gain_exp(500);
            Assert::AreEqual(std::size_t(500), ppa->get_exp());
            Assert::AreEqual(std::size_t(1), ppa->get_level());
            ppa->gain_exp(500);
            Assert::AreEqual(std::size_t(0), ppa->get_exp());
            Assert::AreEqual(std::size_t(2), ppa->get_level());
            ppa->gain_exp(500);
            Assert::AreEqual(std::size_t(500), ppa->get_exp());
            Assert::AreEqual(std::size_t(2), ppa->get_level());
            ppa->gain_exp(12500);
            Assert::AreEqual(std::size_t(0), ppa->get_exp());
            Assert::AreEqual(std::size_t(15), ppa->get_level());
            ppa->gain_exp(1000);
            Assert::AreEqual(std::size_t(1000), ppa->get_exp());
            Assert::AreEqual(std::size_t(15), ppa->get_level());
            Assert::AreNotEqual(health, ppa->get_health());
        }

        TEST_METHOD(generate_damage_test) {
            std::string pa = "PA";
            const std::shared_ptr<hero> ppa(new agile(pa));
            std::string sk = "SK";
            std::shared_ptr<hero> psk(new power(sk));
            attack_trajectory trajectory;
            ppa->generate_damage(trajectory);
            const auto health = psk->get_health();
            psk->get_damage(trajectory);
            Assert::AreNotEqual(health, psk->get_health());
        }

        TEST_METHOD(split) {
            std::string str = "2134 213465 462grsg 54ywg";
            auto elem = my_algo_lib::split(str, ' ');
            Assert::AreEqual(std::size_t(4), elem.size());
        }
    };
}
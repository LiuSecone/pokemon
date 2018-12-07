#include "stdafx.h"
#include "CppUnitTest.h"
#include "../pokemon/hero.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{
    TEST_CLASS(hero_tests)
    {
    public:
        TEST_METHOD(init) {
            const std::shared_ptr<hero> ppa(new agile("PA"));
            Assert::AreNotEqual(double(0), ppa->get_health());
            Assert::AreEqual(std::size_t(0), ppa->get_exp());
            Assert::AreEqual(std::size_t(1), ppa->get_level());
        }

        TEST_METHOD(gain_exp) {
            std::shared_ptr<hero> ppa(new agile("PA"));
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
    };
}
#pragma once

#include "Test.h"

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <type_traits>


namespace test
{

    struct TestMenuEntry
    {
        explicit TestMenuEntry(std::string label, std::function<TestPtr()> makeTestFunction)
            : label(std::move(label))
            , makeTest(std::move(makeTestFunction))
        {}

        const std::string& Label() const { return label; }
        TestPtr MakeTest() const { return makeTest(); }

    private:
        std::string label;
        std::function<TestPtr()> makeTest;
    };


    class TestMenu : public Test
    {
    public:
        TestMenu();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

        bool HasCurrentTest() const;
        Test& CurrentTest();

        void BackToMenu();

        template<class TestType, class = std::enable_if_t<std::is_base_of_v<Test, TestType>>>
        void AddTest(std::string name)
        {
            std::cout << "Registering test \"" << name << '\"' << std::endl;
            tests_.emplace_back(std::move(name), []{ return std::make_unique<TestType>(); });
        }

    private:
        TestPtr pCurrent_;
        std::vector<TestMenuEntry> tests_;
    };

}

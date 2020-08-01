#pragma once


namespace test
{
    class Test
    {
    public:
        Test() = default;
        virtual ~Test() noexcept = default;

        virtual void OnUpdate(float deltaTime) = 0;
        virtual void OnRender() = 0;
        virtual void OnImGuiRender() = 0;
    };
}

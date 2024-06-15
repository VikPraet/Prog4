#pragma once
#include <memory>
#include <stdexcept>

namespace dae
{
    template <typename T>
    class Singleton
    {
    public:
        template <typename... Args>
        static T& GetInstance(Args&&... args)
        {
            static T instance(std::forward<Args>(args)...);
            return instance;
        }

        virtual ~Singleton() = default;
        Singleton(const Singleton& other) = delete;
        Singleton(Singleton&& other) = delete;
        Singleton& operator=(const Singleton& other) = delete;
        Singleton& operator=(Singleton&& other) = delete;

    protected:
        Singleton() = default;
    };
}

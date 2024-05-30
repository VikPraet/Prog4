#pragma once

#include <unordered_map>
#include <memory>
#include <typeindex>
#include "Singleton.h"
#include "SoundSystem.h"

class IService
{
public:
    virtual ~IService() = default;
};

template <typename T>
class ServiceWrapper : public IService
{
public:
    explicit ServiceWrapper(std::unique_ptr<T> service)
        : m_Service(std::move(service))
    {
    }

    T* GetService() { return m_Service.get(); }

private:
    std::unique_ptr<T> m_Service;
};

class ServiceLocator final : public dae::Singleton<ServiceLocator>
{
public:
    template <typename T>
    static void RegisterService(std::unique_ptr<T> service)
    {
        if (service == nullptr) GetInstance().m_Services.erase(typeid(T));
        else GetInstance().m_Services[typeid(T)] = std::make_unique<ServiceWrapper<T>>(std::move(service));
    }

    template <typename T>
    static T* GetService()
    {
        auto& instance = GetInstance();
        const auto it = instance.m_Services.find(typeid(T));
        if (it != instance.m_Services.end())
        {
            return static_cast<ServiceWrapper<T>*>(it->second.get())->GetService();
        }
        return GetDefaultService<T>();
    }

private:
    friend class Singleton<ServiceLocator>;

    ServiceLocator() = default;

    template <typename T>
    static T* GetDefaultService()
    {
        static std::unique_ptr<T> defaultService = std::make_unique<typename DefaultService<T>::type>();
        return defaultService.get();
    }

    std::unordered_map<std::type_index, std::unique_ptr<IService>> m_Services;

    template <typename T>
    struct DefaultService;
};

template <>
struct ServiceLocator::DefaultService<ISoundService>
{
    using type = NullSoundService;
};

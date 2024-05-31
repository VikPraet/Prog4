#pragma once

#include <algorithm>
#include <concepts>
#include <functional>
#include <unordered_set>
#include <vector>
#include <optional>
#include <tuple>

namespace dae
{
    class BaseEvent;

    class EventListener
    {
    public:
        virtual ~EventListener();

        EventListener(EventListener&&) = delete;
        EventListener(const EventListener&) = delete;
        EventListener& operator=(EventListener&&) = delete;
        EventListener& operator=(const EventListener&) = delete;

        void AddEvent(BaseEvent* event);
        void RemoveEvent(BaseEvent* event);

    protected:
        EventListener() = default;

    private:
        std::unordered_set<BaseEvent*> m_BoundEvents;
    };

    class BaseEvent
    {
    public:
        virtual ~BaseEvent() = default;

        BaseEvent(BaseEvent&&) = delete;
        BaseEvent(const BaseEvent&) = delete;
        BaseEvent& operator=(BaseEvent&&) = delete;
        BaseEvent& operator=(const BaseEvent&) = delete;

        void RegisterListener(EventListener* listener);
        void UnregisterListener(EventListener* listener);

    protected:
        BaseEvent() = default;
        virtual void RemoveListener(EventListener* listener) = 0;
    };

    inline EventListener::~EventListener()
    {
        for (auto* event : m_BoundEvents)
        {
            event->UnregisterListener(this);
        }
    }

    inline void EventListener::AddEvent(BaseEvent* event)
    {
        m_BoundEvents.insert(event);
    }

    inline void EventListener::RemoveEvent(BaseEvent* event)
    {
        m_BoundEvents.erase(event);
    }

    inline void BaseEvent::RegisterListener(EventListener* listener)
    {
        listener->AddEvent(this);
        this->RemoveListener(listener);
    }

    inline void BaseEvent::UnregisterListener(EventListener* listener)
    {
        listener->RemoveEvent(this);
        this->RemoveListener(listener);
    }

    template <typename... EventArgs>
    class Event : public BaseEvent
    {
        using FunctionBind = std::pair<EventListener*, std::function<void(EventArgs...)>>;

    public:
        Event() = default;

        ~Event() override
        {
            for (auto* listener : m_EventListeners)
            {
                listener->RemoveEvent(this);
            }
        }

        Event(Event&&) = delete;
        Event(const Event&) = delete;
        Event& operator=(Event&&) = delete;
        Event& operator=(const Event&) = delete;

        template <typename Function>
        void addListener(Function function)
        {
            m_FunctionBinds.emplace_back(nullptr, [function](EventArgs... args) { function(args...); });
        }

        template <typename ObjectType>
            requires std::derived_from<ObjectType, EventListener>
        void addListener(ObjectType* object, void (ObjectType::* memberFunction)(EventArgs...))
        {
            auto* listener = static_cast<EventListener*>(object);
            this->RegisterListener(listener);
            m_EventListeners.insert(listener);

            m_FunctionBinds.emplace_back(listener, [object, memberFunction](EventArgs... args) { (object->*memberFunction)(args...); });

            if (m_CurrentState)
            {
                InvokeForListener(object, memberFunction, *m_CurrentState);
            }
            else
            {
                InvokeForListener(object, memberFunction, std::tuple<EventArgs...>{});
            }
        }

        template <typename... Args>
        void Invoke(Args&&... args)
        {
            m_CurrentState = std::make_tuple(std::forward<Args>(args)...);
            for (const auto& [listener, function] : m_FunctionBinds)
            {
                function(std::forward<Args>(args)...);
            }
        }

    private:
        void RemoveListener(EventListener* listener) override
        {
            m_EventListeners.erase(listener);

            m_FunctionBinds.erase(std::remove_if(m_FunctionBinds.begin(), m_FunctionBinds.end(),
                [listener](const FunctionBind& bind) { return bind.first == listener; }),
                m_FunctionBinds.end());
        }

        template <typename ObjectType, typename... Args>
        void InvokeForListener(ObjectType* object, void (ObjectType::* memberFunction)(EventArgs...), const std::tuple<Args...>& args)
        {
            std::apply([object, memberFunction](EventArgs... unpackedArgs) { (object->*memberFunction)(unpackedArgs...); }, args);
        }

        std::vector<FunctionBind> m_FunctionBinds;
        std::unordered_set<EventListener*> m_EventListeners;
        std::optional<std::tuple<EventArgs...>> m_CurrentState;
    };
}

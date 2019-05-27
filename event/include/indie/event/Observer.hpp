#pragma once

#include <vector>
#include <functional>
#include <cstdint>

namespace indie::event
{
    template <typename SubjectType>
    class Observer
    {
    public:
        using CallbackType = std::function<void(SubjectType &)>;

    public:
        explicit Observer(CallbackType callback) noexcept :
            _callback(callback)
        {}

        void operator()(SubjectType &args) const
        {
            _callback(args);
        }

    private:
        CallbackType _callback;
    };

    template <typename SubjectType>
    class ObserverArray : std::vector<uintptr_t>
    {
    public:
        using ObserverType = Observer<SubjectType>;

    public:
        template <typename TObserver>
        void Register(TObserver *observer)
        {

        }

        void Notify(SubjectType &args)
        {

        }

    private:
        std::vector<ObserverType> _observers;
    };
}
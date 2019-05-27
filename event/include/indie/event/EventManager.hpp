#pragma once

#include <memory>

#include "Observer.hpp"

namespace indie::event
{
    class EventManager
    {
    private:
        struct ObserverArrayData
        {
            using ObserverArrayId = std::size_t;

            std::unique_ptr<std::vector<uintptr_t>> _pool;

            /**
             * @brief Generates a compile time unique identifier for an observers array.
             * 
             * @tparam SubjectType Type of the subject handled by the array.
             * @return A unique array identifier.
             */
            template <typename SubjectType>
            static inline ObserverArrayId GetArrayId() noexcept
            {
                static ObserverArrayId id{GenerateArrayId()};

                return id;
            }

        private:
            /**
             * @brief Used by `GetArrayId()`
             * 
             * Used to increment identifier at each call.
             * 
             * @param reset If reset is set to true, the id counter is set to 0.
             * @return The current id counter.
             */
            static inline ObserverArrayId GenerateArrayId(bool reset = false) noexcept
            {
                static ObserverArrayId cur{0};

                if (reset) {
                    cur = 0;
                }
                return cur++;
            }
        };
    
    public:
        template <typename SubjectType>
        using ObserverPoolType = ObserverArray<SubjectType>;

    public:
        EventManager() = default;
        ~EventManager() = default;

        EventManager(EventManager &other) = delete;
        EventManager(EventManager &&other) = delete;
        EventManager &operator=(EventManager &other) = delete;
        EventManager &operator=(EventManager &&other) = delete;

    private:
        template <typename SubjectType>
        const ObserverPoolType<SubjectType> *GetObserverPool() const noexcept
        {

        }

    public:
        /**
         * @brief Registers an observer to given subjects.
         * 
         * @warning
         * If the observer is already watching for the desired subjects,
         * the behavior is undefined.
         * 
         * @tparam Subject Type of the subject.
         * @tparam Subjects Type of the next subjects if exist.
         * @tparam ObserverType Type of the observer.
         * @param observer The observer object pointer/reference.
         */
        template <typename Subject, typename ...Subjects, typename ObserverType>
        void Register(ObserverType *observer) noexcept
        {

        }

        /*! @copydoc EventManager::Register(ObserverType*) */
        template <typename Subject, typename ...Subjects, typename ObserverType>
        void Register(ObserverType &observer) noexcept
        {
            Register<Subject, Subjects...>(&observer);
        }

        /**
         * @brief Unregisters an observer from specified subjects.
         * 
         * @warning
         * If the observer is not watching for the desired subjects,
         * the behavior is undefined.
         * 
         * @tparam Subject Type of the subject.
         * @tparam Subjects Type of the next subjects if exist.
         * @tparam ObserverType Type of the observer.
         * @param observer The observer object pointer/reference
         */
        template <typename Subject, typename ...Subjects, typename ObserverType>
        void Unregister(ObserverType *observer) noexcept
        {

        }

        /*! @copydoc EventManager::Unregister(ObserverType*) */
        template <typename Subject, typename ...Subjects, typename ObserverType>
        void Unregister(ObserverType &observer) noexcept
        {
            Unregister<ObserverType, Subject, Subjects...>(&observer);
        }

        /**
         * @brief Unregisters an observer from every subject it has subscribed to.
         * 
         * If the observer is not subscribed to any subject,
         * this method will do nothing.
         * 
         * @tparam ObserverType 
         * @param observer 
         */
        template <typename ObserverType>
        void Unregister(ObserverType *observer) noexcept
        {

        }

        /*! @copydoc EventManager::Unregister(ObserverType*) */
        template <typename ObserverType>
        void Unregister(ObserverType &observer) noexcept
        {
            Unregister(&observer);
        }

        /**
         * @brief Notifies observers registered to given subjects.
         * 
         * @note
         * The observer should be watching to every given subject to be notified.
         * 
         * @tparam Subject Type of the first subject.
         * @tparam Subjects Type of the next subjects, if exist.
         * @param args Argument to pass to observers callback.
         */
        template <typename Subject, typename ...Subjects> const
        void Notify(Subject &args)
        {
            if constexpr(sizeof...(Subjects) >= 1) {
                Notify<Subjects...>(args);
            }
        }

        /**
         * @brief Notifies all observers on all subjects.
         * 
         */
        template <typename ...Args>
        void Notify(Args &&...args) const
        {

        }

        /**
         * @brief Removes observers from given subjects.
         * 
         * @tparam Subject Type of the subject.
         * @tparam Subjects Type of the next subjects, if exist.
         */
        template <typename Subject, typename ...Subjects>
        void Reset() noexcept
        {

        }

        /**
         * @brief Tells if an observer is watching for given subjects.
         * 
         * @tparam Subjects Type of the subjects.
         * @tparam ObserverType Type of the observer.
         * @param observer The observer object pointer/reference
         * @return True if the observer is watching for given subjects, false otherwise.
         */
        template <typename ...Subjects, typename ObserverType>
        bool Has(ObserverType *observer) const noexcept
        {
            return false;
        }
        
        /*! @copydoc EventManager::Has(ObserverType*) */
        template <typename ...Subjects, typename ObserverType>
        bool Has(ObserverType &observer) const noexcept
        {
            return Has<Subjects...>(&observer);
        }

    private:
    };
}
#pragma once

#include <memory>
#include <map>
#include <cstddef>

#include "EntityManager.hpp"

namespace indie::ecs
{
    template <typename EntityType = Entity>
    class System
    {
    public:
        using EntityManagerType = EntityManager<EntityType>;

    public:
        /**
         * @brief Default constructor.
         *
         */
        System() = default;

        /**
         * @brief Destructor.
         *
         */
        virtual ~System() = default;

        /**
         * @brief This method is to be called each time the system manager instance is updated.
         *
         */
        virtual void Update() = 0;

        /**
         * @brief Activate the current system instance.
         *
         * If a system is active, the system manager instance which holds this system will update it.
         *
         * Never throws
         */
        void Activate() noexcept
        {
            _is_active = true;
        }

        /**
         * @brief Deactivate the current system instance.
         *
         * If a system is not active, the system manger instance which holds this system will not update it.
         *
         * Never throws
         */
        void Deactivate() noexcept
        {
            _is_active = false;
        }

        /**
         * @brief Tells if the current system instance is active
         *
         * Do not distort the current instance
         * Never throws
         *
         * @return true If the system is active
         * @return false If the system isn't active
         */
        bool IsActive() const noexcept
        {
            return _is_active;
        }

        /**
         * @brief This method exists to be called by the system manager instance to pass its entity manager instance to this system instance
         *
         * Never throws
         *
         * @param em Entity manager instance pointer hold by the SystemManager
         */
        void __init(EntityManagerType *em) noexcept
        {
            _em = em;
        }

    protected:
        EntityManagerType *_em;

    private:
        bool _is_active{true};
    };

    template <typename EntityType = Entity>
    class SystemManager
    {
    public:
        /*! Alias for `EntityManager<EntityType>` */
        using EntityManagerType = EntityManager<EntityType>;

        /*! Alias for `System<EntityType>` */
        using SystemType = System<EntityType>;

        /*! Alias for `std::size_t` */
        using SystemId = std::size_t;

        /*! Alias for `std::shared_ptr<SystemType>` */
        using SystemPtr = std::shared_ptr<SystemType>;

        /*! Alias for `std::map<SystemId, SystemPtr>` */
        using SystemMap = std::map<SystemId, SystemPtr>;

    public:
        /**
         * @brief Constructor
         * @param em A valid entity manager
         */
        explicit SystemManager(EntityManagerType &em) :
            _em(em)
        {}

        /**
         * @brief Destructor
         * Destroy the System Manager object
         */
        ~SystemManager() = default;

    public:
        /**
         * @brief Construct a new system and register it into the current SystemManager instance
         *
         * @tparam TSystem Type of the system you want to activate
         * @tparam Args Types of the arguments to construct the system
         * @param args Arguments to pass to TSystem constructor
         * @return std::shared_ptr<TSystem> Return a pointer to the newly constructed TSystem
         */
        template <typename TSystem, typename ...Args>
        std::shared_ptr<TSystem> Add(Args &&...args)
        {
            IsValidSystem<TSystem>();

            auto system = std::make_shared<TSystem>(std::forward<Args>(args)...);
            auto id = GetSystemId<TSystem>();

            system->__init(&_em);

            _systems.emplace(id, system);

            return system;
        }


        /**
         * @brief Remove a registered system
         *
         * @tparam TSystem Type of the system you want to remove
         */
        template <typename TSystem>
        void Remove()
        {
            IsValidSystem<TSystem>();

            if (Has<TSystem>()) {
                _systems[GetSystemId<TSystem>()].reset();
            }
        }

        /**
         * @brief Activate a registered system
         *
         * @tparam TSystem Type of the system you want to activate
         */
        template <typename TSystem>
        void Activate()
        {
            IsValidSystem<TSystem>();

            if (Has<TSystem>()) {
                _systems[GetSystemId<TSystem>()]->Activate();
            }
        }

        /**
         * @brief Deactivate a registered system
         *
         * @tparam TSystem Type of the system you want to deactivate
         */
        template <typename TSystem>
        void Deactivate()
        {
            IsValidSystem<TSystem>();

            if (Has<TSystem>()) {
                _systems[GetSystemId<TSystem>()]->Deactivate();
            }
        }

        /**
         * @brief Check if the current SystemManager instance holds a specific system
         * Do not distort the current instance
         * Never throws
         * @tparam TSystem Type of the system
         * @return true If the current instance holds the specified system type
         * @return false If the current instance does not hold the specified system type
         */
        template <typename TSystem>
        bool Has() const noexcept
        {
            IsValidSystem<TSystem>();

            if (_systems[GetSystemId<TSystem>()]) {
                return true;
            }
            return false;
        }

        /**
         * @brief Update every registered systems
         *
         */
        void Update()
        {
            for (auto &system : _systems)
            {
                if (system.second->IsActive())
                {
                    system.second->Update();
                }
            }
        }
    
    private:
        template <typename TSystem>
        static constexpr void IsValidSystem()
        {
            static_assert(std::is_base_of<SystemType, TSystem>(), "TSystem not derived from System");
        }

        static inline SystemId GenerateTypeId() noexcept
        {
            static SystemId cur{0};

            return cur++;
        }

        template <typename TSystem>
        static inline SystemId GetSystemId() noexcept
        {
            IsValidSystem<TSystem>();

            static SystemId id{GenerateTypeId()};

            return id;
        }

    private:
        SystemMap _systems;

        EntityManagerType &_em;
    };

}
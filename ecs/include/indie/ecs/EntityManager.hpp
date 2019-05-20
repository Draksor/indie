#pragma once

#include <vector>
#include <cstddef>
#include <algorithm>
#include <tuple>
#include <functional>
#include <exception>

#include <indie/meta/Tuple.hpp>

#include "./Entity.hpp"
#include "./Pool.hpp"
#include "./details/SparseSet.hpp"

namespace indie::ecs
{
    /**
     * @brief Represents a temporary pools container used by the entity manager.
     * 
     * This class is mostly used by `Get` and `ForEach` methods of the entity manager.
     * 
     * @tparam Entity The type of the entity identifier
     * @tparam Pools Types of the pools to store.
     */
    template <typename EntityType = Entity, typename ...Pools>
    class Filter
    {
    public:
        template <typename Component>
        using PoolType = Pool<Component, EntityType>;

        using PoolsTuple = typename indie::meta::Tuple<Pools * ...>;

        using SizeType = EntityType;

    public:
        Filter(Pools *...pools) : _pools(pools...) {}
        ~Filter() = default;

        /**
         * @brief Get components of an entity.
         * 
         * @warning
         * Getting an unassigned component is undefined behavior.
         * 
         * @tparam Components 
         * @param et 
         * @return decltype(auto) 
         */
        template <typename ...Components>
        decltype(auto) Get(EntityType et)
        {
            if constexpr(sizeof...(Components) == 1) {
                return (*std::get<PoolType<Components> *>(_pools)->Get(et), ...);
            }
            else {
                return std::make_tuple(Get<Components>(et)...);
            }
        }

        /**
         * @brief Tells if an entity own specified components.
         * 
         * @tparam Components Types of the components.
         * @param et A valid entity.
         * @return True if the entity own every specified component, false otherwise.
         */
        template <typename ...Components>
        bool Has(EntityType et)
        {
            CheckComponents<Components...>();
            return (_pools.template Get<PoolType<Components> *>()->Has(et) && ...);
        }

        /**
         * @brief Checks if given components are part of the filter instance.
         * 
         * @note
         * Will throw an error at compile-time if a component does not belong
         * to the filter instance.
         * 
         * @tparam Components Types of the components to test.
         */
        template <typename ...Components>
        constexpr void CheckComponents()
        {
            static_assert(
                (indie::meta::TupleHas<PoolType<Components> *, PoolsTuple>() && ...),
                "Given component(s) do(es) not belong to this filter!");
        }

    private:
        PoolsTuple _pools;
    };

    template <typename EntityType = Entity>
    class EntityManager
    {
    private:
        struct PoolData
        {
            using PoolId = std::size_t;

            std::unique_ptr<details::SparseSet<EntityType>> Pool{nullptr};
            std::function<void(EntityType et)> Delete;
            std::function<bool(EntityType et)> Has;
            PoolId ID;

            /**
             * @brief Generate a compile time unique identifier for a pool.
             * 
             * @tparam Component Type of the component stored by the pool.
             * @return A unique pool identifier.
             */
            template <typename Component>
            static inline PoolId GetPoolId() noexcept
            {
                static PoolId id{GeneratePoolId()};

                return id;
            }

        private:
            /**
             * @brief Used by `GetPoolId()`
             * 
             * Used to increment identifier at each call.
             * 
             * @param reset If reset is set to true, the id counter is set to 0.
             * @return The current id counter.
             */
            static inline PoolId GeneratePoolId(bool reset = false) noexcept
            {
                static PoolId cur{0};

                if (reset) {
                    cur = 0;
                }
                return cur++;
            }
        };
    
    public:
        using SizeType = EntityType;

        template <typename Component>
        using PoolType = Pool<Component, EntityType>;

    public:
        EntityManager() = default;
        ~EntityManager() = default;

        EntityManager(EntityManager &other) = delete;
        EntityManager(EntityManager &&other) = delete;
        EntityManager &operator=(EntityManager &other) = delete;
        EntityManager &operator=(EntityManager &&other) = delete;

    private:
        /**
         * @brief Gets an immutable pool of components.
         * 
         * Returns `nullptr` if none is found.
         * 
         * @tparam Component Type of the components pool.
         * @return An immutable components pool.
         */
        template <typename Component>
        const PoolType<Component> *GetPool() const noexcept
        {
            auto pool_id = PoolData::template GetPoolId<Component>();

            auto it = std::find_if(_pools.begin(), _pools.end(), [pool_id](const auto &pool) {
                return pool.ID == pool_id;
            });

            if (it != _pools.end()) {
                auto pool = &(*it);
                return static_cast<PoolType<Component> *>(pool->Pool.get());
            }
            return nullptr;
        }
        /**
         * @brief Gets a mutable pool of components.
         * 
         * Returns `nullptr` if none is found.
         * 
         * @tparam Component Type of the components pool.
         * @return A mutable components pool.
         */
        template <typename Component>
        PoolType<Component> *GetPool() noexcept
        {
            return const_cast<PoolType<Component> *>(std::as_const(*this).template GetPool<Component>());
        }

        /**
         * @brief Gets a pool of components.
         * Allocates it if the pool does not exist.
         * 
         * If the pool does not exist yet, this method will allocate it.
         * Never returns null.
         * Throws `std::runtime_error` if pool allocation failed.
         * 
         * @tparam Component Type of the component pool.
         * @return A components pool.
         */
        template <typename Component>
        PoolType<Component> *TryAllocatePool()
        {
            auto pool_id = PoolData::template GetPoolId<Component>();
            PoolData *pool = nullptr;

            auto it = std::find_if(_pools.begin(), _pools.end(), [pool_id](const auto &pool) {
                return pool.ID == pool_id;
            });

            if (it == _pools.end()) {
                PoolData pool_data;
                pool_data.Pool = std::make_unique<PoolType<Component>>();
                pool_data.ID = pool_id;
                pool = &_pools.emplace_back(std::move(pool_data));
                pool->Delete = [=](EntityType et) {
                    this->GetPool<Component>()->Delete(et);
                };
                pool->Has = [=](EntityType et) {
                    return this->GetPool<Component>()->Has(et);
                };
            }
            else {
                pool = &(*it);
            }

            if (!pool || !pool->Pool) {
                throw std::runtime_error("Allocation failed for pool: " + std::to_string(pool_id));
            }

            return static_cast<PoolType<Component> *>(pool->Pool.get());
        }

    public:
        /**
         * @brief Creates a new entity.
         * 
         * @return A valid entity.
         */
        EntityType Create()
        {
            static EntityType ett{0};

            if (_deleted_et.Size() == 0) {
                _entities.Insert(ett);
                return ett++;
            }
            else {
                _entities.Insert(*_deleted_et.Begin());
                auto et = *_deleted_et.Begin();
                _deleted_et.Erase(_deleted_et.Begin());
                return et;
            }
        }

        /**
         * @brief Destroys entities and their associated components.
         * 
         * @warning
         * Destroying an invalid entity is undefined behavior.
         * 
         * @tparam Entities Valid entities.
         */
        template <typename Entity, typename ...Entities>
        void Destroy(const Entity et, const Entities ...ets)
        {
            for (auto &pool : _pools) {
                if (pool.Has(et)) {
                    pool.Delete(et);
                }
            }
            _entities.Erase(et);
            _deleted_et.Insert(et);
            if constexpr(sizeof...(Entities) >= 1) {
                Destroy(ets...);
            }
        }
        /**
         * @brief Destroys entities which own all specified components.
         * 
         * @tparam Components Types of the components.
         */
        template <typename Component, typename ...Components>
        void Destroy()
        {
            for (auto et : _entities) {
                if (Has<Component, Components...>(et)) {
                    Destroy(et);
                }
            }
        }

        /**
         * @brief Allocates a new component and assignes it to an entity.
         * 
         * @warning
         * Using an invalid entity is undefined behavior.
         * 
         * If a component is already assigned to the entity,
         * this method does nothing,
         * you can replace it by using the `Replace` method instead.
         * 
         * @tparam Args Types of the arguments used to construct the component.
         * @param et A valid entity.
         * @param args Arguments to pass to component's constructor.
         */
        template <typename Component, typename ...Args>
        void Assign(const EntityType et, Args &&...args)
        {
            TryAllocatePool<Component>()->Assign(et, std::forward<Args>(args)...);
        }

        /**
         * @brief Replaces a component of an entity.
         * 
         * @warning
         * Using an invalid entity or is undefined behavior.
         * 
         * Replacing a component not existing component does nothing.
         * 
         * @tparam Component Component type.
         * @tparam Args Types of the arguments to use to construct the component.
         * @param et A valid entity.
         * @param args Arguments used to construct the component.
         */
        template <typename Component, typename ...Args>
        void Replace(const EntityType et, Args &&...args)
        {
            GetPool<Component>()->Replace(et, std::forward<Args>(args)...);
        }
    
        /**
         * @brief Assigns or replaces a component of an entity.
         * 
         * @warning
         * Using an invalid entity is undefined behavior.
         * 
         * @tparam Component Component type.
         * @tparam Args Types of the arguments to use to construct the component.
         * @param et A valid entity.
         * @param args Arguments used to construct the component.
         */
        template <typename Component, typename ...Args>
        void AssignOrReplace(const EntityType et, Args &&...args)
        {
            TryAllocatePool<Component>()->AssignOrReplace(et, std::forward<Args>(args)...);
        }

        /**
         * @brief Removes a components of a valid entity.
         * 
         * @warning
         * Using an invalid entity or removing a component 
         * not own by the entity is undefined behavior.
         * 
         * @tparam Component Type of the component to remove.
         * @param et A valid entity.
         */
        template <typename Component, typename ...Components>
        void Delete(const EntityType et) noexcept
        {
            GetPool<Component>()->Delete(et);
            if constexpr (sizeof...(Components) >= 1) {
                Delete<Components...>(et);
            }
        }
        /**
         * @brief Resets specified components pools.
         * 
         * @warning
         * Attempting to reset a component pool not registered
         * is undefined behavior.
         * 
         * @tparam Components Components stored by the pools to reset.
         */
        template <typename Component, typename ...Components>
        void Reset() noexcept
        {
            GetPool<Component>()->Reset();
            if constexpr (sizeof...(Components) >= 1) {
                Reset<Components...>();
            }
        }

        /**
         * @brief Destroy every entity and assiocated components.
         * 
         */
        void Reset() noexcept
        {
            for (auto &et : _entities) {
                Destroy(et);
            }
        }

        /**
         * @brief Filters entities which own all specified components and
         * returns an object of class `Filter`.
         * 
         * @note See `Filter` documentation.
         * 
         * @warning
         * Using a component that has not been registered yet is undefined behavior.
         * 
         * @tparam Components Types of the components to search.
         * @return A filter object with entities components stored.
         */
        template <typename ...Components>
        Filter<EntityType, PoolType<Components> ...> Get()
        {
            Filter<EntityType, PoolType<Components> ...> filter{(GetPool<Components>())...};
            return filter;
        }
        /**
         * @brief Gets components of an entity.
         * 
         * @warning
         * Using a component that has not been registered yet or,
         * getting an unassigned component or,
         * using a not existing entity,
         * is undefined behavior.
         * 
         * @tparam Components Types of the components to get.
         * @param et A valid entity.
         * @return A component or a tuple of components.
         */
        template <typename ...Components>
        decltype(auto) Get(const Entity et)
        {
            if constexpr(sizeof...(Components) == 1) {
                return GetPool<Components...>()->Get(et);
            }
            else {
                return std::make_tuple(GetPool<Components>()->Get(et)...);
            }
        }

        /**
         * @brief Iterates through each entities.
         *
         * Example:
         * @code
         * {
         *     em.ForEach([](Entity et) {
         *         do_stuff;
         *     });
         * }
         * @endcode
         *
         * @param et A valid entity.
         */
        template <typename Func>
        void ForEach(Func &&func)
        {
            for (auto et : _entities) {
                func(et);
            }
        }
        /**
         * @brief Iterates through each entities which own all specified components.
         *
         * Example:
         * @code
         * {
         *     em.ForEach<Comp1, Comp2, CompN>([](const auto et, auto &comp1, auto &comp2, auto &compn) {
         *         do_stuff;
         *     });
         * }
         * @endcode
         *
         * @tparam Func The type of the function to apply.
         * @tparam Components Types of the components.
         * @param func A valid function.
         */
        template <typename ...Components, typename Func>
        void ForEach(Func &&func)
        {
            auto filter = Get<Components...>();
            for (auto et : _entities) {
                if (filter.template Has<Components...>(et)) {
                    func(et, filter.template Get<Components>(et)...);
                }
            }
        }


        /**
         * @brief Tells if an entity holds every specified component.
         * 
         * @tparam Components Types of the components.
         * @param et An entity.
         * @return False if the entity does not belong to this registry. 
         * @return True if the entity owns component(s), false otherwise.
         */
        template <typename ...Components>
        bool Has(const EntityType et) const noexcept
        {
            const auto pools = std::make_tuple(GetPool<Components>()...);
            return ((std::get<const PoolType<Components> *>(pools) ? std::get<const PoolType<Components> *>(pools)->Has(et) : false) && ...);
        }

        /**
         * @brief Tells if an entity exists in this registry instance.
         * 
         * @param et An entity.
         * @return True if the entity belongs to this registry, false otherwise.
         */
        bool Exists(const EntityType et) const noexcept
        {
            return _entities.Has(et);
        }

        /**
         * @brief Get the current number of valid entities.
         * 
         * @return Valid entities number.
         */
        SizeType Size() const noexcept
        {
            return _entities.Size();
        }
        /**
         * @brief Gets the number of valid entities which own all specified components.
         * 
         * @tparam Components Types of the components that entities should own.
         * @return Valid entities number that match with the specified components.
         */
        template <typename ...Components>
        SizeType Size() const noexcept
        {
            if constexpr (sizeof...(Components) == 0) {
                return _entities.Size();
            }
            else {
                auto result = 0;
                for (const auto &et : _entities) {
                    if (Has<Components...>(et)) {
                        result++;
                    }
                }
                return result;
            }
        }

        /**
         * @brief Tells if the registry actually contains entities.
         * 
         * @return True if at least one entity belongs to this registry, false otherwise.
         */
        bool IsEmpty() const noexcept
        {
            return _entities.Size() == 0;
        }
        /**
         * @brief Tells if the registry actually contains entities that match specified components.
         * 
         * @return True if at least one entity with specified components belongs to this registry, false otherwise.
         */
        template <typename ...Components>
        bool IsEmpty() const noexcept
        {
            return Size<Components...>() == 0;
        }

        /**
         * @brief Gets the current entities storage capacity.
         * 
         * @return Capacity of the entities storage.
         */
        SizeType Capacity() const noexcept
        {
            return _entities.Capacity();
        }
        /**
         * @brief Gets a component pool storage capacity
         * 
         * @warning
         * Using an invalid component pool is undefined behavior.
         * 
         * @return Capacity of the pool storage
         */
        template <typename Component>
        SizeType Capacity() const noexcept
        {
            return GetPool<Component>()->Capacity();
        }

        /**
         * @brief Increases the capacity used to store entities.
         * 
         * If the new capacity is lower than the previous one,
         * this method does nothing.
         * 
         * @param count Wished capacity.
         */
        void Reserve(SizeType count)
        {
            _entities.Reserve(count);
        }
        /**
         * @brief Increases a component pool space.
         * 
         * If the pool does not exist yet, allocates a new one.
         * 
         * @tparam Type of the component stored by the pool
         * @param count Wished capacity.
         */
        template <typename Component>
        void Reserve(SizeType count)
        {
            TryAllocatePool<Component>()->Reserve(count);
        }

    private:
        details::SparseSet<EntityType> _entities;
        details::SparseSet<EntityType> _deleted_et;

        std::vector<PoolData> _pools;
    };
}
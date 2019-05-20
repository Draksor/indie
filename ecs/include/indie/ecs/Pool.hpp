#pragma once

#include <set>
#include <memory>
#include <algorithm>

#include "./Entity.hpp"
#include "./details/SparseSet.hpp"

namespace indie::ecs
{
    template <typename Component, typename EntityType = Entity>
    class Pool : public details::SparseSet<EntityType>
    {
    public:
        using BaseType = typename details::SparseSet<EntityType>;
        using SizeType = typename BaseType::SizeType;

    public:
        Pool() = default;
        ~Pool() = default;

        Pool(const Pool<Component, EntityType> &other) = delete;
        Pool(const Pool<Component, EntityType> &&other) = delete;
        Pool &operator=(const Pool<Component, EntityType> &other) = delete;
        Pool &operator=(const Pool<Component, EntityType> &&other) = delete;

        /**
         * @brief Allocates a new component and assignes it to an entity.
         * 
         * @warning
         * Assigning an already assigned component is undefined behavior.
         * 
         * @tparam Args Types of the arguments used to construct the component.
         * @param et A valid entity.
         * @param args Arguments to pass to component's constructor.
         */
        template <typename ...Args>
        Component &Assign(EntityType et, Args &&...args)
        {
            BaseType::Insert(et);
            if (BaseType::IndexOf(et) >= Capacity()) {
                Reserve(BaseType::IndexOf(et) + 1);
            }
            return (_components.emplace_back(Component(std::forward<Args>(args)...)));
        }

        /**
         * @brief Replaces an already assigned component.
         * 
         * @warning
         * Replacing an unassigned component is undefined behavior.
         * 
         * @tparam Args Types of the arguments used to construct the component.
         * @param et A valid entity.
         * @param args Arguments to pass to component's constructor.
         */
        template <typename ...Args>
        Component &Replace(EntityType et, Args &&...args)
        {
            return (_components[BaseType::IndexOf(et)] = Component(std::forward<Args>(args)...));
        }

        /**
         * @brief Replaces an assigned component or assignes a new one.
         * 
         * @tparam Args Types of the arguments used to construct the component.
         * @param et A valid entity.
         * @param args Arguments to pass to component's constructor.
         */
        template <typename ...Args>
        Component &AssignOrReplace(EntityType et, Args &&...args)
        {
            if (!Has(et)) {
                return Assign(et, std::forward<Args>(args)...);
            }
            else {
                return Replace(et, std::forward<Args>(args)...);
            }
        }

        /**
         * @brief Removes an assigned component.
         * 
         * @warning
         * If the entity has no component in this pool,
         * the behavior is undefined.
         * 
         * @param et A valid entity.
         */
        void Delete(EntityType et)
        {
            BaseType::Erase(et);
        }

        /**
         * @brief Finds in this pool the associated component of an entity.
         * 
         * 
         * @param et A valid entity.
         * @return A pointer to the component, null if it doesn't exist.
         */
        Component *Get(EntityType et)
        {
            if (Has(et)) {
                return &(_components[BaseType::IndexOf(et)]);
            }
            else {
                return nullptr;
            }
        }

        /**
         * @brief Behaves like `Get` method.
         * 
         * @param et A valid entity.
         * @return A pointer to the component, null if it doesn't exist.
         */
        Component *operator[](EntityType et)
        {
            return Get(et);
        }

        /**
         * @brief Tells if an entity is assigned to a component of this pool.
         * 
         * @param et A valid entity.
         * @return True if the entity owns a component of this pool,
         * false otherwise.
         */
        bool Has(EntityType et) const
        {
            return BaseType::Has(et);
        }

        /**
         * @brief Deletes every assigned component.
         * 
         */
        void Reset()
        {
            _components.clear();
            BaseType::Clear();
        }

        /**
         * @brief Gets the number of allocated components.
         * 
         * @return The number of components in this pool.
         */
        SizeType Size() const noexcept
        {
            return BaseType::Size();
        }

        /**
         * @brief Gets the storage capacity of this instance.
         * 
         * @return Storage capacity.
         */
        SizeType Capacity() const noexcept
        {
            return _components.capacity();
        }

        /**
         * @brief Increases storage space.
         * 
         * If given size is lower than previous capacity,
         * this method does nothing.
         * 
         * @param count New size of the pool.
         */
        void Reserve(SizeType count) noexcept
        {
            if (count > Capacity()) {
                _components.reserve(count);
            }
        }

        /**
         * @brief Tells if this pool is empty (0 component allocated).
         * 
         * @return True if the pool is empty, false otherwise.
         */
        bool IsEmpty() const noexcept
        {
            return BaseType::IsEmpty();
        }

        /**
         * @brief Iterates through each entities holding a component of this pool
         * 
         * Example:
         * @code
         * int main()
         * {
         *     indie::ecs::Pool<MyComponent> pool;
         *     pool.ForEach([](const auto et, const auto &component) {
         *         do_stuff;
         *     });
         * }
         * @endcode
         * 
         * @tparam Func 
         * @param func 
         */
        template <typename Func>
        void ForEach(Func &&func)
        {
            for (auto it = BaseType::Begin(); it != BaseType::End(); it++) {
                auto component = _components[BaseType::IndexOf(*it)];
                func(*it, component);
            }
        }

    private:
        std::vector<Component> _components;
    };
}
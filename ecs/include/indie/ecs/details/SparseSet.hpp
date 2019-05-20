#pragma once

#include <type_traits>
#include <vector>

namespace indie::ecs::details
{
    template <typename T>
    class SparseSet
    {
        static_assert(std::is_unsigned<T>::value, "SparseSet can only contain unsigned integers");
    
    public:
        using SizeType = T;
        using ValueType = T;

        using ConstIterator = typename std::vector<T>::const_iterator;

    public:
        /**
         * @brief Gets the number of elements stored.
         * 
         * @return The number of elements stored.
         */
        SizeType Size() const noexcept { return _size; }
        /**
         * @brief Gets the storage capacity.
         * 
         * @return The storage capacity. 
         */
        SizeType Capacity() const noexcept { return _capacity; }

        /**
         * @brief Tells if a sparse set do not contain any element.
         * 
         * @return True if the sparse set contains 0 element, false otherwise.
         */
        bool IsEmpty() const { return _size == 0; }

        /**
         * @brief Makes the sparse set empty.
         * 
         */
        void Clear() noexcept { _size = 0; }

        /**
         * @brief Increases storage capacity.
         * 
         * @param count New size.
         */
        void Reserve(SizeType count)
        {
            if (count > _capacity) {
                _dense.resize(count, 0);
                _sparse.resize(count, 0);

                _capacity = count;
            }
        }

        /**
         * @brief Tells if the sparse set contains an element.
         * 
         * @param val Element to search.
         * @return True if the element exists, false otherwise.
         */
        bool Has(const ValueType &val) const noexcept
        {
            return val < _capacity &&
                   _sparse[val] < _size &&
                   _dense[_sparse[val]] == val;
        }

        /**
         * @brief Inserts an element in the sparse set.
         * 
         * @param val Element to insert.
         */
        void Insert(const ValueType &val)
        {
            if (!Has(val)) {
                if (val >= _capacity) {
                    Reserve(val + 1);
                }

                _dense[_size] = val;
                _sparse[val] = _size;

                ++_size;
            }
        }

        /**
         * @brief Erases an element.
         * 
         * @param val Element to erase.
         */
        void Erase(const ValueType &val)
        {
            if (Has(val)) {
                _dense[_sparse[val]] = _dense[_size - 1];
                _sparse[_dense[_size - 1]] = _sparse[val];

                --_size;
            }
        }
        /**
         * @brief Erases an element.
         * 
         * @param it Iterator.
         */
        void Erase(ConstIterator it)
        {
            Erase(*it);
        }

        /**
         * @brief Gets the index of an element.
         * 
         * @param val Element.
         * @return Index of the element.
         */
        SizeType IndexOf(const ValueType &val)
        {
            return _sparse[val];
        }

        /**
         * @brief Gets an iterator to the beginning of the sparse set.
         * 
         * @return An iterator to the beginning.
         */
        ConstIterator Begin() const { return _dense.begin(); }
        ConstIterator begin() const { return Begin(); }
        
        /**
         * @brief Gets an iterator to the ending of the sparse set.
         * 
         * @return An iterator to the ending.
         */
        ConstIterator End() const { return _dense.begin() + _size; }
        ConstIterator end() const { return End(); }

    private:
        std::vector<ValueType> _dense;
        std::vector<ValueType> _sparse;

        SizeType _size{0};
        SizeType _capacity{0};
    };

}
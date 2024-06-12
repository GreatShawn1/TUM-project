#pragma once

#include <stdexcept>
#include <utility>
#include <array>
#include <type_traits>

/**
 * Compiletime generic lookup map.
 *
 * storage: std::array
 * search-time: linear
 *
 * when the compiler errors, make sure no duplicate keys exist,
 * and that existing keys are requested.
 * exceptions are thrown, which lead to compiler errors otherwise
 *
 * The message may be: "error: ‘*0u’ is not a constant expression"
 * -> nonexistant key
 */

template<typename KeyType, typename ValueType, size_t count>
class CexprMap {
public:
    using pair_type = std::pair<KeyType, ValueType>;
    using value_type = ValueType;
    /*template<typename Entry, typename... Rest>
requires std::conjunction_v<std::is_same<Entry, Rest>...>
CexprMap(Entry, Rest&&...) ->  TODO */;

    using key_type = KeyType;
    
    using storage_type = std::array<pair_type, count>;

    template<class... Entries>
    constexpr CexprMap(Entries&&... entries) : values{{std::forward<Entries>(entries)...}} {
        static_assert(sizeof...(Entries) == count, "Incorrect number of key-value pairs provided");
        verify_no_duplicates();
    }    

    constexpr const ValueType &operator [](const KeyType &key) const {
        return get(key);
    }

    constexpr size_t size() const {
        return count;
    }
     /**
     * Get a key's value
     */

    constexpr const ValueType &get(const KeyType &key) const {
        auto member_of_values = find(key);
        if (member_of_values != values.end()) {
            return member_of_values->second;
        } else {
            throw std::out_of_range("Specified key does not exist");
        }
    }

    constexpr bool contains(const KeyType &key) const {
        return find(key) != values.end();
    }

private:
    /**
     * Checks if keys are duplicated.
     * Throws std::invalid_argument on duplicate key.
     */

    constexpr auto find(const KeyType &key) const -> typename storage_type::const_iterator {
        for (auto member_of_values = values.begin(); member_of_values != values.end(); ++member_of_values) {
            if (member_of_values->first == key) {
                return member_of_values;
            }
        }
        return values.end();
    }

    constexpr void verify_no_duplicates() const {
        for (size_t i = 0; i < count; ++i) {
            for (size_t j = i + 1; j < count; ++j) {
                if (values[i].first == values[j].first) {
                    throw std::invalid_argument("Duplicate keys detected");
                }
            }
        }
    }
    storage_type values;
//std::array<std::pair<K, V>, count> values;
};

/**
 * Returns CexprMap, a compiletime lookup table from
 * K to V, where all entries of K must be unique.
 *
 * usage: constexpr auto bla = create_cexpr_map<type0, type1>(entry0, entry1, ...);
 */

template<typename KeyType, typename ValueType, typename... Entries>
constexpr auto create_cexpr_map(Entries&&... entries) {
    return CexprMap<KeyType, ValueType, sizeof...(entries)>(std::forward<Entries>(entries)...);
}

// Template deduction guide
template<typename... Pairs>
CexprMap(Pairs...) -> CexprMap<typename std::tuple_element<0, std::tuple<Pairs...>>::type::first_type, 
                               typename std::tuple_element<0, std::tuple<Pairs...>>::type::second_type, 
                               sizeof...(Pairs)>;

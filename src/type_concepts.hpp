#ifndef INCLUDED_TYPE_CONCEPTS_HPP
#define INCLUDED_TYPE_CONCEPTS_HPP

#include <concepts>
#include <type_traits>
#include <utility>

template<typename T>
concept is_flag = (std::is_enum_v<T> || std::is_scoped_enum_v<T>) && requires {
    { T::NO_FLAG } -> std::same_as<T>;
    { T::FLAG_END } -> std::same_as<T>;
    (std::to_underlying(T::NO_FLAG) == 0);
};

template<is_flag FLAG_T>
constexpr FLAG_T operator&(const FLAG_T& flag_a, const FLAG_T& flag_b)
{
    return FLAG_T{std::to_underlying(flag_a) & std::to_underlying(flag_b)};
}

template<is_flag FLAG_T>
constexpr FLAG_T operator|(const FLAG_T& flag_a, const FLAG_T& flag_b)
{
    return FLAG_T{std::to_underlying(flag_a) | std::to_underlying(flag_b)};
}

template<is_flag FLAG_T>
constexpr FLAG_T operator^(const FLAG_T& flag_a, const FLAG_T& flag_b)
{
    return FLAG_T{std::to_underlying(flag_a) | std::to_underlying(flag_b)};
}

template<is_flag FLAG_T>
constexpr FLAG_T operator&=(FLAG_T& flag_a, const FLAG_T& flag_b)
{
    return flag_a = FLAG_T{std::to_underlying(flag_a) & std::to_underlying(flag_b)};
}

template<is_flag FLAG_T>
constexpr FLAG_T operator|=(FLAG_T& flag_a, const FLAG_T& flag_b)
{
    return flag_a = FLAG_T{std::to_underlying(flag_a) | std::to_underlying(flag_b)};
}

template<is_flag FLAG_T>
constexpr FLAG_T operator^=(FLAG_T& flag_a, const FLAG_T& flag_b)
{
    return flag_a = FLAG_T{std::to_underlying(flag_a) | std::to_underlying(flag_b)};
}

template<is_flag FLAG_T>
constexpr FLAG_T operator~(const FLAG_T& flag) 
{
    return FLAG_T{(std::to_underlying(FLAG_T::FLAG_END) - 1) & (~std::to_underlying(flag)) };
}

template<is_flag FLAG_T>
constexpr bool operator!(const FLAG_T& flag) 
{
    return flag == FLAG_T::NO_FLAG;
}

#endif // INCLUDED_TYPE_CONCEPTS_HPP

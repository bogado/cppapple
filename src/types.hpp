#ifndef INCLUDED_TYPES_HPP
#define INCLUDED_TYPES_HPP

#include <concepts>
#include <cstdint>
#include <cassert>
#include <limits>
#include <tuple>
#include <type_traits>
#include <variant>

constexpr std::size_t MAX_PATH=4096;

using COLORREF = std::uint16_t;
using LPCOLORREF = COLORREF*;


template <typename VALUE_T>
concept word_type = std::integral<VALUE_T> && sizeof(VALUE_T) >= 2;

template <typename VALUE_T>
concept long_type = std::integral<VALUE_T> && sizeof(VALUE_T) > 2;

template <typename VALUE_T, typename OTHER_T>
concept smaller_type = std::integral<VALUE_T> && std::integral<OTHER_T> &&
                       sizeof(OTHER_T) >= sizeof(VALUE_T);

template <typename VALUE_T, typename OTHER_T>
concept larger_type = std::integral<VALUE_T> && std::integral<OTHER_T> &&
                       sizeof(OTHER_T) <= sizeof(VALUE_T);

constexpr auto make_word(std::integral auto a, std::integral auto b)
    -> std::int16_t {
  return std::int16_t{static_cast<std::int8_t>(a)} |
         std::int16_t{static_cast<std::int8_t>(b) << 8};
}

constexpr auto make_long(std::integral auto a, std::integral auto b) {
  return std::int32_t{static_cast<std::int16_t>(a) |
                      (static_cast<std::int16_t>(b) << 16)};
}

template <std::integral T>
static constexpr auto bit_size = std::intmax_t{sizeof(T)*8 - std::is_signed_v<T>};

namespace details {

template <bool is_signed>
using all_integer = std::conditional_t<
    is_signed,
    std::tuple<std::int8_t, std::int16_t, std::int32_t, std::int64_t>,
    std::tuple<std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t>>;

template <std::integral auto INT_T>
constexpr unsigned index = (INT_T <= 1) ? 0 : (index<(INT_T >> 1)> + 1);
static_assert(index<1> == 0 && index<2> == 1 && index<4> == 2);

template <bool is_signed, unsigned index>
using integer = std::tuple_element_t<index, all_integer<is_signed>>;

static constexpr auto last_bit = bit_size<std::uintmax_t>-1;

} // namespace details

template <std::integral T>
using half_int_t = details::integer<std::is_signed_v<T>, details::index<sizeof(T)>-1>;

static_assert(std::same_as<half_int_t<std::uint16_t>, std::uint8_t>);
static_assert(std::same_as<half_int_t<std::int32_t>, std::int16_t>);

template <unsigned POS>
static constexpr auto bit = (std::uintmax_t{1} << POS);
static_assert(bit<0> == 1);
static_assert(bit<2> == 4);
static_assert(bit<7> == 128);
static_assert(bit<63> == 0b1000000000000000000000000000000000000000000000000000000000000000);

static_assert(std::same_as<decltype(bit<1>), const std::uintmax_t>);

template <unsigned N>
requires(N >= 0 && N <= details::last_bit)
static constexpr std::uintmax_t mask = []() {
    if constexpr (N == details::last_bit) {
        return std::numeric_limits<uintmax_t>::max();
    } else {
        return bit<N+1> - 1;
    }
}();

static_assert(mask<0> == 1);
static_assert(mask<1> == 0b11);
static_assert(mask<3> == 0b1111);
static_assert(mask<63> == 0xffff'ffff'ffff'ffff);

template <std::integral INT_T>
static constexpr auto mask_for = mask<bit_size<INT_T>-1>;

static_assert(mask_for<std::uint8_t> == 255);
static_assert(bit_size<std::int16_t> == 15);

template <std::unsigned_integral UINT_T>
constexpr auto split(UINT_T value) {
    using namespace details;    
    using half_t = half_int_t<UINT_T>;

    return std::pair{
        static_cast<half_t>(value % (UINT_T{1} << bit_size<half_t>)),
        static_cast<half_t>(value >> bit_size<half_t>)
    };
}

static_assert(split(uint64_t{1}).first == 1);

constexpr auto high_part(std::integral auto value) {
    return split(value).first;
}

constexpr auto low_part(std::integral auto value) {
    return split(value).second;
}

  ///////////////////
 // TO BE UPDATED //
///////////////////

using HANDLE = void *; 

struct OVERLAPPED {
	std::uint32_t   Internal;
	std::uint32_t   InternalHigh;
	std::uint32_t   Offset;
	std::uint32_t   OffsetHigh;
	HANDLE  hEvent;
};

struct POINT
{
	long  x;
	long  y;
};

#endif // INCLUDED_TYPES_HPP

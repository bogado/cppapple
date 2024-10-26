#ifndef INCLUDED_TYPES_HPP
#define INCLUDED_TYPES_HPP

#include <concepts>
#include <cstdint>
#include <cassert>

constexpr std::size_t MAX_PATH=4096;

using COLORREF = std::uint16_t;
using LPCOLORREF = COLORREF*;

template <unsigned BITS>
constexpr auto mask = (1 << BITS) - 1;
static_assert(mask<8> == 255);

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

template <std::integral INTEGER_T>
constexpr auto low_part(larger_type<INTEGER_T> auto value) {
    return int{value & mask<sizeof(INTEGER_T)>};
}

template <std::integral INTEGER_T>
constexpr auto high_part(larger_type<INTEGER_T> auto value) {
    return int{(value >> 16) & mask<16>};
}

constexpr auto low_word(larger_type<std::int16_t *> auto val) {
    return low_part<std::int8_t *>(val);
}

constexpr auto high_word(larger_type<std::int16_t *> auto val) {
    return high_part<std::int8_t *>(val);
}

constexpr auto low_byte(larger_type<std::int8_t *> auto val) {
    return low_part<std::int8_t *>(val);
}

constexpr auto high_byte(larger_type<std::int8_t *> auto val) {
    return high_part<std::int8_t *>(val);
}

//// TO BE UPDATED ////
///
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

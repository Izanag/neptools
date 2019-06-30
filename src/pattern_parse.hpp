#ifndef UUID_5F4B4145_C89A_4241_8A9C_B8DBBB568F43
#define UUID_5F4B4145_C89A_4241_8A9C_B8DBBB568F43
#pragma once

#include "pattern.hpp"
#include <libshit/meta_utils.hpp>

namespace Neptools
{

  namespace PatternParse
  {

    template <char C, typename Enable = void>
    struct HexValue;

    template <char C>
    struct HexValue<C, typename std::enable_if_t<C >= '0' && C <= '9'>>
      : std::integral_constant<unsigned, C - '0'> {};

    template <char C>
    struct HexValue<C, typename std::enable_if_t<C >= 'A' && C <= 'F'>>
      : std::integral_constant<unsigned, C + 10 - 'A'> {};

    template <char C>
    struct HexValue<C, typename std::enable_if_t<C >= 'a' && C <= 'f'>>
      : std::integral_constant<unsigned, C + 10 - 'a'> {};

    template <char C>
    constexpr auto HexValueV = HexValue<C>::value;

    template <unsigned Acc, char... Chars> struct FromHex;
    template <unsigned Acc, char A, char... Chars>
    struct FromHex<Acc, A, Chars...>
      : std::integral_constant<
          unsigned, FromHex<
                      Acc * 16 + HexValueV<A>, Chars...>::value> {};

    template <unsigned Acc>
    struct FromHex<Acc> : std::integral_constant<unsigned, Acc> {};

    template <char... Chars>
    constexpr auto FromHexV = FromHex<0, Chars...>::value;

    template <Byte... Bytes> struct ByteSequence
    {
      static constexpr size_t size = sizeof...(Bytes);
      // fucking retards, why can't I have a zero sized stack based array
      static constexpr Byte seq[size==0?1:size] = { Bytes... };

      template <Byte C>
      using Append = ByteSequence<Bytes..., C>;
    };
    template <Byte... Bytes> constexpr Byte ByteSequence<Bytes...>::seq[];

    template <typename PatternBytes, typename MaskBytes>
    struct Pattern : public ::Neptools::Pattern
    {
      template <Byte Pat, Byte Mask>
      using Append = Pattern<typename PatternBytes::template Append<Pat>,
                             typename MaskBytes::template Append<Mask>>;

      constexpr Pattern() noexcept : ::Neptools::Pattern{pattern, mask, size} {}

      static constexpr auto pattern = PatternBytes::seq;
      static constexpr auto mask    = MaskBytes::seq;
      static constexpr auto size    = PatternBytes::size;
    };

    template <typename Pat, char... Args> struct PatternParse;

    template <typename Pat, char... Args>
    struct PatternParse<Pat, '?', '?', ' ', Args...>
    {
      using value = typename PatternParse<
        typename Pat::template Append<0x00, 0x00>,
        Args...>::value;
    };

    template <typename Pat, char... Args>
    struct PatternParse<Pat, '?', ' ', Args...>
    {
      using value = typename PatternParse<
        typename Pat::template Append<0x00, 0x00>,
        Args...>::value;
    };

    template <typename Pat, char A, char B, char... Args>
    struct PatternParse<Pat, A, B, ' ', Args...>
    {
      using value = typename PatternParse<
        typename Pat::template Append<FromHexV<A, B>, 0xff>,
        Args...>::value;
    };

    template <typename Pat, char A, char... Args>
    struct PatternParse<Pat, A, ' ', Args...>
    {
      using value = typename PatternParse<
        typename Pat::template Append<FromHexV<A>, 0xff>,
        Args...>::value;
    };

    template <typename Pat, char A, char B, char C, char D, char... Args>
    struct PatternParse<Pat, A, B, '/', C, D, ' ', Args...>
    {
      static constexpr auto Val = FromHexV<A, B>;
      static constexpr auto Mask = FromHexV<C, D>;
      using value = typename PatternParse<
        typename Pat::template Append<Val & Mask, Mask>,
        Args...>::value;
    };

    template <typename Pat, char... Args>
    struct PatternParse<Pat, ' ', Args...>
    {
      using value = typename PatternParse<Pat, Args...>::value;
    };

    template <typename Pat>
    struct PatternParse<Pat>
    {
      using value = Pat;
    };

    template <char... Bytes>
    using DoPatternParse = typename PatternParse<
      Pattern<ByteSequence<>, ByteSequence<>>, Bytes..., ' '>::value;

    template <typename X, size_t... Idx>
    DoPatternParse<X::Get(Idx)...> ToPattern(std::index_sequence<Idx...>)
    { return {}; }

  }

#define NEPTOOLS_PATTERN(str) \
  LIBSHIT_LITERAL_CHARPACK(::Neptools::PatternParse::DoPatternParse, str)

}
#endif

// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//

#pragma once

#include <iostream>
#include <type_traits>
#include <format>
#include <string_view>
#include <string>
#include <cstdint>

namespace colorized {
enum Style: std::uint8_t {
  Standard,
  Bold,
  Dim,
  Italic,
  Underline,
  Blink
};

enum _4BitForeground : std::uint8_t {
  FgBlack = 30,
  FgRed,
  FgGreen,
  FgYellow,
  FgBlue,
  FgMagenta,
  FgCyan,
  FgWhite,
  FgBrBlack = 90,
  FgBrRed,
  FgBrGreen,
  FgBrYellow,
  FgBrBlue,
  FgBrMagenta,
  FgBrCyan,
  FgBrWhite,
  FgDefault = 39
};

enum _4BitBackground : std::uint8_t {
  BgBlack = 40,
  BgRed,
  BgGreen,
  BgYellow,
  BgBlue,
  BgMagenta,
  BgCyan,
  BgWhite,
  BgBrBlack = 100,
  BgBrRed,
  BgBrGreen,
  BgBrYellow,
  BgBrBlue,
  BgBrMagenta,
  BgBrCyan,
  BgBrWhite,
  BgDefault = 49
};

enum _8BitColor : std::uint8_t;

// checks if given value is ostreamable (printable, outputable)
template<typename Q, typename T>
concept Outputable = requires(Q& stream, T t)
{
  { stream << t } -> std::convertible_to<Q&>;
};

// checks if given value is istreamable (inputable)
template<typename Q, typename T>
concept Inputable = requires(Q& stream, T t)
{
  { stream >> t } -> std::convertible_to<Q&>;
};

// checks if given value is istreamable or/and ostreamable (output or input)
template<typename Q, typename T>
concept InorOutable = Outputable<Q, T> || Inputable<Q, T>;

// there will be std::arithmetic concept (submitted proposal), but this'll do the same.
template<typename Q>
concept Arithmetic = std::is_arithmetic_v<Q>;

// checks if given type overloads operator<<
template<typename Q, typename T>
struct IsOstream : std::bool_constant<Outputable<Q, T>> {};

// checks if given type overloads operator>>
template<typename Q, typename T>
struct IsIstream : std::bool_constant<Inputable<Q, T>> {};

// checks if given type overloads operator<< or/and operator>>.
template<typename Q, typename T>
struct IsStream : std::bool_constant<std::disjunction_v<typename IsOstream<Q, T>::type, typename IsIstream<Q, T>::type>> {};

// aliases for type traits.
template<typename Q, typename T>
using IsOstreamType = typename IsOstream<Q, T>::type;

template<typename Q, typename T>
using IsIstreamType = typename IsIstream<Q, T>::type;

template<typename Q, typename T>
using IsStreamType = typename IsStream<Q, T>::type;

template<Arithmetic T>
struct _RGBA;

using RGBA = _RGBA<std::uint8_t>;
using Foreground = _4BitForeground;
using Background = _4BitBackground;

namespace detail {
template<typename... Args>
static constexpr auto format_generate_str(std::string_view context, Args&&... args) noexcept {
  return std::vformat(context, std::make_format_args(args...));
}
} // namespace detail

template<typename _Style, typename Stream, typename T>
static constexpr void print(_Style style, Foreground foreground, Background background, Stream& stream, T&& t) noexcept;

template<typename _Style, typename Stream, typename T>
static constexpr void print(_Style style, RGBA foreground, RGBA background, Stream& stream, T&& t) noexcept;

template<typename _Style, typename Stream, typename T>
static constexpr void print(_Style style, _8BitColor foreground, _8BitColor background, Stream& stream, T&& t) noexcept;

template<Arithmetic T>
struct _RGBA {
  T r, g, b;
  constexpr _RGBA(T r, T b, T g) : r{r}, g{g}, b{b} {}
};

template<typename _Style, typename _Foreground, typename _Background, typename Str, typename... InArgs>
struct Pack {
  template<typename Stream>
  constexpr Pack(_Style style, _Foreground fg, _Background bg, Stream& stream, Str str, InArgs&&... args) noexcept {
    print(style, fg, bg, stream, detail::format_generate_str(str, std::forward<InArgs>(args)...));
  }
};

// we don't need to mark `constexpr` since std::cout is runtime operation, but why not?
template<typename _Style, typename Stream, typename T>
static constexpr void print(_Style style, Foreground foreground, Background background, Stream& stream, T&& t) noexcept {
  if constexpr(std::is_same_v<IsOstreamType<Stream, T>, std::true_type>) {
    stream << "\x1b[0m\x1b[" << +style << ";" << +background << "m"
          << "\x1b[" << +style << ";" << +foreground << "m"
          << std::forward<T>(t);
  } else if constexpr(std::is_same_v<IsOstreamType<std::ostream, T>, std::true_type>) {
    print(style, foreground, background, std::cout, std::forward<T>(t));
  }
}

template<typename _Style, typename Stream, typename T>
static constexpr void print(_Style style, RGBA foreground, RGBA background, Stream& stream, T&& t) noexcept {
  if constexpr(std::is_same_v<IsOstreamType<Stream, T>, std::true_type>) {
    stream << "\x1b[0m\x1b[" << +style << ";49m"
           << "\x1b[48;2;" << +background.r << ";" << +background.g << ";" << +background.b << "m"
           << "\x1b[38;2;" << +foreground.r << ";" << +foreground.g << ";" << +foreground.b << "m"
           << std::forward<T>(t);
  } else if constexpr(std::is_same_v<IsOstreamType<std::ostream, T>, std::true_type>) {
    print(style, foreground, background, std::cout, std::forward<T>(t));
  }
}

template<typename _Style, typename Stream, typename T>
static constexpr void print(_Style style, _8BitColor foreground, _8BitColor background, Stream& stream, T&& t) noexcept {
  if constexpr(std::is_same_v<IsOstreamType<Stream, T>, std::true_type>) {
    stream << "\x1b[0m\x1b[" << +style << ";49m"
           << "\x1b[48;5;" << +background << "m"
           << "\x1b[38;5;" << +foreground << "m"
           << std::forward<T>(t);
  } else if constexpr(std::is_same_v<IsOstreamType<std::ostream, T>, std::true_type>) {
    print(style, foreground, background, std::cout, std::forward<T>(t));
  }
}

template<typename _Style, typename _Foreground, typename _Background, typename Str>
static constexpr void print_cout(_Style style, _Foreground foreground, _Background background, Str&& t) noexcept {
  print(style, foreground, background, std::cout, std::forward<Str>(t));
}

template<typename _Style, typename _Foreground, typename _Background, typename Str>
static constexpr void print_cerr(_Style style, _Foreground foreground, _Background background, Str&& t) noexcept {
  print(style, foreground, background, std::cerr, std::forward<Str>(t));
}

template<typename _Style, typename _Foreground, typename _Background, typename Stream, typename Str, typename... Args>
static constexpr void print_format(_Style style, _Foreground foreground, _Background background, Stream& stream, Str ctx, Args&&... args) noexcept {
  colorized::print(style, foreground, background, stream, detail::format_generate_str(ctx, std::forward<Args>(args)...));
}

template<typename _Style, typename _Foreground, typename _Background, typename Str, typename... Args>
static constexpr void print_cout_format(_Style style, _Foreground foreground, _Background background, Str ctx, Args&&... args) noexcept {
  colorized::print_format(style, foreground, background, std::cout, detail::format_generate_str(ctx, std::forward<Args>(args)...));
}

template<typename _Style, typename _Foreground, typename _Background, typename Str, typename... Args>
static constexpr void print_cerr_format(_Style style, _Foreground foreground, _Background background, Str ctx, Args&&... args) noexcept {
  colorized::print_format(style, foreground, background, std::cout, detail::format_generate_str(ctx, std::forward<Args>(args)...));
}

namespace detail {
template<typename Str, typename... InArgs>
constexpr void handle_one_pack(Pack<Style, Foreground, Background, Str, InArgs...>&& pack) noexcept {}

template<typename Str, typename... InArgs>
constexpr void handle_one_pack(Pack<Style, RGBA, RGBA, Str, InArgs...>&& pack) noexcept {}

template<typename Str, typename... InArgs>
constexpr void handle_one_pack(Pack<Style, _8BitColor, _8BitColor, Str, InArgs...>&& pack) noexcept {}
} // namespace detail

// it unpacks args, then calls Pack constructor. if we need that non-recursive version;
// we might use tuple but who cares? this library will used at runtime.
template<typename... Args>
constexpr void print_formats_recursive(Args&&... args) noexcept {
  (detail::handle_one_pack(std::forward<Args>(args)), ...);
}

// there is no known macro for C++23
#if __cplusplus > 202002L
static constexpr void print_cout_reset() noexcept
#else
  static void print_cout_reset() noexcept
#endif
{
  std::cout << "\x1b[0m";
}

#if __cplusplus > 202002L
static constexpr void print_cout_reset() noexcept
#else
  static void print_cerr_reset() noexcept
#endif
{
  std::cerr << "\x1b[0m";
}

namespace runtime {
[[nodiscard]] static std::string generate_colors(Style style, Foreground fg, Background bg) noexcept {
  return "\x1b[" + std::to_string(style) + ";" + std::to_string(bg) + "m\x1b[" + std::to_string(style) + ";" + std::to_string(fg) + "m";
}

[[nodiscard]] static std::string generate_colors(Style style, RGBA fg, RGBA bg) noexcept {
  return "\x1b[" + std::to_string(style) + ";49m\x1b[48;2;" + std::to_string(bg.r) + ";" + std::to_string(bg.g) + ";" + std::to_string(bg.b) + "m" +
         "\x1b[38;2;" + std::to_string(fg.r) + ";" + std::to_string(fg.g) + ";" + std::to_string(fg.b) + "m";
}

[[nodiscard]] static std::string generate_colors(Style style, _8BitColor fg, _8BitColor bg) noexcept {
  return "\x1b[0m" + std::to_string(style) + ";49m\x1b[48;5;" + std::to_string(bg) + "m" +
         "\x1b[38;5;" + std::to_string(fg) + "m";
}
} // namespace runtime

namespace constants {
// those constants are sometimes useful for quickly test something.
static constexpr std::string_view black_color { "\x1b[0;30m" };
static constexpr std::string_view red_color { "\x1b[0;31m" };
static constexpr std::string_view green_color { "\x1b[0;32m" };
static constexpr std::string_view yellow_color { "\x1b[0;33m" };
static constexpr std::string_view blue_color { "\x1b[0;34m" };
static constexpr std::string_view magenta_color { "\x1b[0;35m" };
static constexpr std::string_view cyan_color { "\x1b[0;36m" };
static constexpr std::string_view white_color { "\x1b[0;37m" };
static constexpr std::string_view light_black_color { "\x1b[0;90m" };
static constexpr std::string_view light_red_color { "\x1b[0;91m" };
static constexpr std::string_view light_green_color { "\x1b[0;92m" };
static constexpr std::string_view light_yellow_color { "\x1b[0;93m" };
static constexpr std::string_view light_blue_color { "\x1b[0;94m" };
static constexpr std::string_view light_magenta_color { "\x1b[0;95m" };
static constexpr std::string_view light_cyan_color { "\x1b[0;96m" };
static constexpr std::string_view light_white_color { "\x1b[0;97m" };

static constexpr std::string_view bold_black_color { "\x1b[1;30m" };
static constexpr std::string_view bold_red_color { "\x1b[1;31m" };
static constexpr std::string_view bold_green_color { "\x1b[1;32m" };
static constexpr std::string_view bold_yellow_color { "\x1b[01;33m" };
static constexpr std::string_view bold_blue_color { "\x1b[1;34m" };
static constexpr std::string_view bold_magenta_color { "\x1b[1;35m" };
static constexpr std::string_view bold_cyan_color { "\x1b[1;36m" };
static constexpr std::string_view bold_white_color { "\x1b[1;37m" };
static constexpr std::string_view light_bold_black_color { "\x1b[1;90m" };
static constexpr std::string_view light_bold_red_color { "\x1b[1;91m" };
static constexpr std::string_view light_bold_green_color { "\x1b[1;92m" };
static constexpr std::string_view light_bold_yellow_color { "\x1b[1;93m" };
static constexpr std::string_view light_bold_blue_color { "\x1b[1;94m" };
static constexpr std::string_view light_bold_magenta_color { "\x1b[1;95m" };
static constexpr std::string_view light_bold_cyan_color { "\x1b[1;96m" };
static constexpr std::string_view light_bold_white_color { "\x1b[1;97m" };

static constexpr std::string_view reset_color { "\x1b[0m" };
} // namespace constants

// X11 8 bit color namings
enum _8BitColor : std::uint8_t {
  x016_Grey0              ,
  x017_NavyBlue           ,
  x018_DarkBlue           ,
  x019_Blue3              ,
  x020_Blue3              ,
  x021_Blue1              ,
  x022_DarkGreen          ,
  x023_DeepSkyBlue4       ,
  x024_DeepSkyBlue4       ,
  x025_DeepSkyBlue4       ,
  x026_DodgerBlue3        ,
  x027_DodgerBlue2        ,
  x028_Green4             ,
  x029_SpringGreen4       ,
  x030_Turquoise4         ,
  x031_DeepSkyBlue3       ,
  x032_DeepSkyBlue3       ,
  xx1b_DodgerBlue1        ,
  x034_Green3             ,
  x035_SpringGreen3       ,
  x036_DarkCyan           ,
  x037_LightSeaGreen      ,
  x038_DeepSkyBlue2       ,
  x039_DeepSkyBlue1       ,
  x040_Green3             ,
  x041_SpringGreen3       ,
  x042_SpringGreen2       ,
  x043_Cyan3              ,
  x044_DarkTurquoise      ,
  x045_Turquoise2         ,
  x046_Green1             ,
  x047_SpringGreen2       ,
  x048_SpringGreen1       ,
  x049_MediumSpringGreen  ,
  x050_Cyan2              ,
  x051_Cyan1              ,
  x052_DarkRed            ,
  x053_DeepPink4          ,
  x054_Purple4            ,
  x055_Purple4            ,
  x056_Purple3            ,
  x057_BlueViolet         ,
  x058_Orange4            ,
  x059_Grey37             ,
  x060_MediumPurple4      ,
  x061_SlateBlue3         ,
  x062_SlateBlue3         ,
  x063_RoyalBlue1         ,
  x064_Chartreuse4        ,
  x065_DarkSeaGreen4      ,
  x066_PaleTurquoise4     ,
  x067_SteelBlue          ,
  x068_SteelBlue3         ,
  x069_CornflowerBlue     ,
  x070_Chartreuse3        ,
  x071_DarkSeaGreen4      ,
  x072_CadetBlue          ,
  x073_CadetBlue          ,
  x074_SkyBlue3           ,
  x075_SteelBlue1         ,
  x076_Chartreuse3        ,
  x077_PaleGreen3         ,
  x078_SeaGreen3          ,
  x079_Aquamarine3        ,
  x080_MediumTurquoise    ,
  x081_SteelBlue1         ,
  x082_Chartreuse2        ,
  x083_SeaGreen2          ,
  x084_SeaGreen1          ,
  x085_SeaGreen1          ,
  x086_Aquamarine1        ,
  x087_DarkSlateGray2     ,
  x088_DarkRed            ,
  x089_DeepPink4          ,
  x090_DarkMagenta        ,
  x091_DarkMagenta        ,
  x092_DarkViolet         ,
  x093_Purple             ,
  x094_Orange4            ,
  x095_LightPink4         ,
  x096_Plum4              ,
  x097_MediumPurple3      ,
  x098_MediumPurple3      ,
  x099_SlateBlue1         ,
  x100_Yellow4            ,
  x101_Wheat4             ,
  x102_Grey53             ,
  x103_LightSlateGrey     ,
  x104_MediumPurple       ,
  x105_LightSlateBlue     ,
  x106_Yellow4            ,
  x107_DarkOliveGreen3    ,
  x108_DarkSeaGreen       ,
  x109_LightSkyBlue3      ,
  x110_LightSkyBlue3      ,
  x111_SkyBlue2           ,
  x112_Chartreuse2        ,
  x113_DarkOliveGreen3    ,
  x114_PaleGreen3         ,
  x115_DarkSeaGreen3      ,
  x116_DarkSlateGray3     ,
  x117_SkyBlue1           ,
  x118_Chartreuse1        ,
  x119_LightGreen         ,
  x120_LightGreen         ,
  x121_PaleGreen1         ,
  x122_Aquamarine1        ,
  x123_DarkSlateGray1     ,
  x124_Red3               ,
  x125_DeepPink4          ,
  x126_MediumVioletRed    ,
  x127_Magenta3           ,
  x128_DarkViolet         ,
  x129_Purple             ,
  x130_DarkOrange3        ,
  x131_IndianRed          ,
  x132_HotPink3           ,
  x133_MediumOrchid3      ,
  x134_MediumOrchid       ,
  x135_MediumPurple2      ,
  x136_DarkGoldenrod      ,
  x137_LightSalmon3       ,
  x138_RosyBrown          ,
  x139_Grey63             ,
  x140_MediumPurple2      ,
  x141_MediumPurple1      ,
  x142_Gold3              ,
  x143_DarkKhaki          ,
  x144_NavajoWhite3       ,
  x145_Grey69             ,
  x146_LightSteelBlue3    ,
  x147_LightSteelBlue     ,
  x148_Yellow3            ,
  x149_DarkOliveGreen3    ,
  x150_DarkSeaGreen3      ,
  x151_DarkSeaGreen2      ,
  x152_LightCyan3         ,
  x153_LightSkyBlue1      ,
  x154_GreenYellow        ,
  x155_DarkOliveGreen2    ,
  x156_PaleGreen1         ,
  x157_DarkSeaGreen2      ,
  x158_DarkSeaGreen1      ,
  x159_PaleTurquoise1     ,
  x160_Red3               ,
  x161_DeepPink3          ,
  x162_DeepPink3          ,
  x163_Magenta3           ,
  x164_Magenta3           ,
  x165_Magenta2           ,
  x166_DarkOrange3        ,
  x167_IndianRed          ,
  x168_HotPink3           ,
  x169_HotPink2           ,
  x170_Orchid             ,
  x171_MediumOrchid1      ,
  x172_Orange3            ,
  x173_LightSalmon3       ,
  x174_LightPink3         ,
  x175_Pink3              ,
  x176_Plum3              ,
  x177_Violet             ,
  x178_Gold3              ,
  x179_LightGoldenrod3    ,
  x180_Tan                ,
  x181_MistyRose3         ,
  x182_Thistle3           ,
  x183_Plum2              ,
  x184_Yellow3            ,
  x185_Khaki3             ,
  x186_LightGoldenrod2    ,
  x187_LightYellow3       ,
  x188_Grey84             ,
  x189_LightSteelBlue1    ,
  x190_Yellow2            ,
  x191_DarkOliveGreen1    ,
  x192_DarkOliveGreen1    ,
  x193_DarkSeaGreen1      ,
  x194_Honeydew2          ,
  x195_LightCyan1         ,
  x196_Red1               ,
  x197_DeepPink2          ,
  x198_DeepPink1          ,
  x199_DeepPink1          ,
  x200_Magenta2           ,
  x201_Magenta1           ,
  x202_OrangeRed1         ,
  x203_IndianRed1         ,
  x204_IndianRed1         ,
  x205_HotPink            ,
  x206_HotPink            ,
  x207_MediumOrchid1      ,
  x208_DarkOrange         ,
  x209_Salmon1            ,
  x210_LightCoral         ,
  x211_PaleVioletRed1     ,
  x212_Orchid2            ,
  x213_Orchid1            ,
  x214_Orange1            ,
  x215_SandyBrown         ,
  x216_LightSalmon1       ,
  x217_LightPink1         ,
  x218_Pink1              ,
  x219_Plum1              ,
  x220_Gold1              ,
  x221_LightGoldenrod2    ,
  x222_LightGoldenrod2    ,
  x223_NavajoWhite1       ,
  x224_MistyRose1         ,
  x225_Thistle1           ,
  x226_Yellow1            ,
  x227_LightGoldenrod1    ,
  x228_Khaki1             ,
  x229_Wheat1             ,
  x230_Cornsilk1          ,
  x231_Grey100            ,
  x232_Grey3              ,
  x233_Grey7              ,
  x234_Grey11             ,
  x235_Grey15             ,
  x236_Grey19             ,
  x237_Grey23             ,
  x238_Grey27             ,
  x239_Grey30             ,
  x240_Grey35             ,
  x241_Grey39             ,
  x242_Grey42             ,
  x243_Grey46             ,
  x244_Grey50             ,
  x245_Grey54             ,
  x246_Grey58             ,
  x247_Grey62             ,
  x248_Grey66             ,
  x249_Grey70             ,
  x250_Grey74             ,
  x251_Grey78             ,
  x252_Grey82             ,
  x253_Grey85             ,
  x254_Grey89             ,
  x255_Grey93             
};
} // namespace colorized
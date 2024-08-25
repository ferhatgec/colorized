#include "colorized.hh"
#include <iostream>

using namespace std::literals;
using namespace colorized;

int main() {
	// checks for if int has operator overload for std::ostream.
	std::cout << std::boolalpha << std::is_same_v<IsStreamType<std::ostream, int>, std::true_type> << '\n';

	std::string str = "Hello";
	
	print_cout(Style::Bold, Foreground::FgRed, Background::BgDefault, "Hello world\n");
	print_cout(Style::Bold, RGBA{255, 0, 0 }, RGBA{255, 255, 255}, "Hi!\n");

	print_cout(Style::Bold, RGBA{255, 0, 0 }, RGBA{255, 255, 255}, str + "\n");
	print_cout_format(Style::Bold, Foreground::FgBrBlue, Background::BgDefault, "Hello world: {}\n"sv, 1 + 2);

	print_formats_recursive(
		Pack{Style::Bold, Foreground::FgBrRed, Background::BgDefault, std::cerr, "Hi world: {}\n"sv, 1 + 2},
		Pack{Style::Bold, Foreground::FgBrBlue, Background::BgDefault, std::cout, "Hello world: {}\n"sv, 1 + 2}
	);

	print_formats_recursive(
		// we cannot deduce std::string as template argument in C++20, use traditional way /s.
		Pack{ Style::Bold, Foreground::FgBrRed, Background::BgDefault, std::cerr, "Hi world: {}\n"sv, str.data() },
		Pack{ Style::Bold, Foreground::FgBrBlue, Background::BgDefault, std::cout, "Hello world: {}\n"sv, 1 + 2 }
	);

	print_cout_reset();

	return 0;
}

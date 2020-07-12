/* MIT License
#
# Copyright (c) 2020 Ferhat Geçdoğan All Rights Reserved.
# Distributed under the terms of the MIT License.
#
# */

#include "Colorized.hpp"
#include <iostream>

int main() {
	std::cout << Templatestr + colorized::IntToString(1) + Semicolonstr + colorized::IntToString(RED) + "m" << "Hello!\n";
	colorized::PrintWith(colorized::Colorize(BOLD, LIGHT_MAGENTA).c_str(), "Hello, Colorized!");
	colorized::PrintWith(colorized::ColorizeChar(BOLD, CYAN), "\nHello, Colorized (Char)!");
	return 0;
}

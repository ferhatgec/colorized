/* MIT License
#
# Copyright (c) 2020 Ferhat Geçdoğan All Rights Reserved.
# Distributed under the terms of the MIT License.
#
# */

#include "colorized.hpp"
#include <iostream>

int
main() {
	//std::cout << Templatestr + intToString(1) + Semicolonstr + intToString(RED) + "m" << "Hello!\n";
	printfc(colorize(TYPE::BOLD, LIGHT_MAGENTA).c_str(), "Hello, Colorized!");
	printfc(colorizec(TYPE::BOLD, CYAN), "\nHello, Colorized (Char)!");
	return 0;
}

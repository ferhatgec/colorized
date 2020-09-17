/* MIT License
#
# Copyright (c) 2020 Ferhat Geçdoğan All Rights Reserved.
# Distributed under the terms of the MIT License.
#
# */

#include "Colorized.hpp"
#include <iostream>

int main() {
	printfc(colorizeChar(BOLD, CYAN),true,"Test\n");
	printf("Hello");
	exit(0);
	std::cout << Templatestr + intToString(1) + Semicolonstr + intToString(RED) + "m" << "Hello!\n";
	printfc(colorize(BOLD, LIGHT_MAGENTA).c_str(), "Hello, Colorized!");
	printfc(colorizeChar(BOLD, CYAN), "\nHello, Colorized (Char)!");
	return 0;
}

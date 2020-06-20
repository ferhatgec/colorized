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
	colorized::PrintWith(colorized::Colorize(BOLD, CYAN).c_str(), "Hello, Colorized!");
	return 0;
}

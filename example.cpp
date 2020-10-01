/* MIT License
#
# Copyright (c) 2020 Ferhat Geçdoğan All Rights Reserved.
# Distributed under the terms of the MIT License.
#
# */

#include <Colorized.hpp>
#include <iostream>

int main(void) {
	setFColor(COLOR{100, 12, 12});
 	setColor(COLOR{255, 0, 0});
  	std::cout << "TEST\n";
	RESETB();
	
	setColor(COLOR{127, 255, 77});
	std::cout << "Hello, world!\n";
	RESETB();
  	return 0;
}

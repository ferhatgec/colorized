/* MIT License
#
# Copyright (c) 2020 Ferhat Geçdoğan All Rights Reserved.
# Distributed under the terms of the MIT License.
#
# */

#include <Colorized.hpp>
#include <iostream>

int main(void) {
 	setColor(COLOR{255, 0, 0});
  	std::cout << "TEST\n";
  	setColor(TCOLOR{TYPE::LIGHT, GREEN});
  	std::cout << "TEST\n";
  	return 0;
}

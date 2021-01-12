#include <Colorized.hpp>
#include <iostream>

int main(void) {
    setColor(COLOR{127, 255, 77});
    std::cout << "Hello, world!\n";
    RESETB
    
    
  	colorized::PrintWith(colorized::Colorize(BOLD, LIGHT_RED), "Hi, Colorized!\n");

  	return 0;
}

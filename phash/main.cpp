#include <iostream>
#include "phash.hpp"

int main(int argc, const char * argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: phash <image file>" << std::endl;
        return 1;
    }
    try {
        Magick::InitializeMagick(*argv);
        std::cout << (std::string) phash(Magick::Image(argv[1])) << std::endl;
    } catch(std::exception &err) {
        std::cerr << "Error: " << err.what() << std::endl;
        return 1;
    }
    return 0;
}

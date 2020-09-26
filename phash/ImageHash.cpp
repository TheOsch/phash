#include "ImageHash.hpp"

#include <new>

ImageHash::ImageHash(const std::vector<bool> &binary_array) {
    _hash = binary_array;
}

static char hex_digit(uint8_t ch) {
    return ch < 10 ? '0' + ch : 'a' + ch - 10;
}

ImageHash::operator std::string() {
    if (_hash.empty()) {
        return std::string("0");
    }
    char * hex = new char[(_hash.size() >> 2) + (_hash.size() & 3) ? 1 : 0];
    if (hex == NULL) {
        throw std::bad_alloc();
    }
    char * symbol = hex;
    uint8_t ch = 0;
    size_t i;
    for (i = 0; i < _hash.size(); ++ i) {
        if (i && i % 4 == 0) {
            *(symbol++) = hex_digit(ch);
            ch = 0;
        } else {
            ch <<= 1;
        }
        if (_hash[i]) {
            ++ ch;
        }
    }
    if (i & 3) {
        ch <<= 4 - (i & 3);
    }
    *(symbol++) = hex_digit(ch);
    std::string s = hex;
    delete[] hex;
    return s;
}

ImageHash::operator const std::vector<bool> &() { 
    return _hash;
}

ImageHash::operator std::vector<bool>() { 
    return _hash;
}

size_t ImageHash::operator-(const std::vector<bool> &other) { 
    if (_hash.size() != other.size()) {
        throw std::length_error("ImageHashes must be of the same size");
    }
    size_t n = 0;
    for (size_t i = 0; i < _hash.size(); ++ i) {
        if (_hash[i] != other[i]) {
            ++n;
        }
    }
    return n;
}

bool ImageHash::operator==(const std::vector<bool> &other) {
    return _hash == other;
}

bool ImageHash::operator!=(const std::vector<bool> &other) {
    return _hash != other;
}

unsigned ImageHash::hash() {
    unsigned h = 0;
    for (size_t i = 0; i < _hash.size(); ++ i) {
        if (_hash[i]) {
            h += 1 << (i & 7);
        }
    }
    return h;
}

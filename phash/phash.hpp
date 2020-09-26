#ifndef phash_hpp
#define phash_hpp

#include <Magick++.h>
#include "ImageHash.hpp"

ImageHash phash(const Magick::Image &image,
                size_t hash_size = 8,
                size_t highfreq_factor = 4);

#endif /* phash_hpp */

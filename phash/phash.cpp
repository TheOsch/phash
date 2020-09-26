#include "phash.hpp"

#include <cmath>

static void to_grayscale(Magick::Image &img);

static void to_small_square(Magick::Image &img, size_t square_size);

static void dct_axis_0(const Magick::Image &img,
                       size_t img_size,
                       std::vector<double> &dct0);

static void dct_axis_1(const std::vector<double> &dct0,
                       size_t img_size,
                       std::vector<double> &dct);

static void hash_from_dct(const std::vector<double> &dct,
                          size_t img_size,
                          size_t hash_size,
                          std::vector<bool> &hash);

ImageHash phash(const Magick::Image &image,
                size_t hash_size,
                size_t highfreq_factor) {
    if (hash_size < 2) {
        throw std::invalid_argument("Hash size must be greater than or equal to 2");
    }
    size_t img_size = hash_size * highfreq_factor;
    
    // Copy the image because further operations are destructive
    Magick::Image img = image;
    
    to_grayscale(img);
    to_small_square(img, img_size);
    
    std::vector<double> dct0, dct1;
    dct_axis_0(img, img_size, dct0);
    dct_axis_1(dct0, img_size, dct1);
    
    std::vector<bool> hash;
    hash_from_dct(dct1, img_size, hash_size, hash);
    
    return ImageHash(hash);
}

static void to_grayscale(Magick::Image &img) {
    static const double color_matrix[] = {
        0.299, 0.587, 0.114,
        0.299, 0.587, 0.114,
        0.299, 0.587, 0.114
    };
    img.colorMatrix(3, color_matrix);
    img.quantizeColorSpace(Magick::GRAYColorspace);
    img.quantize();
}

static void to_small_square(Magick::Image &img,
                            size_t square_size) {
    img.filterType(Magick::LanczosFilter);
    Magick::Geometry newSize(square_size, square_size);
    newSize.aspect(true);
    img.resize(newSize);
}

static void dct_axis_0(const Magick::Image &img,
                       size_t img_size,
                       std::vector<double> &dct0) {
    const Magick::Quantum *pixels = img.getConstPixels(
                                                       0,
                                                       0,
                                                       img_size,
                                                       img_size);
    dct0.resize(img_size * img_size, 0.0);
    std::vector<double> row;
    row.resize(img_size);
    size_t i, n, k;
    double factor = M_PI / (double)img_size;
    for (i = 0; i < img_size; ++ i) {
        // Transfer the row so that values are like PIL's output -
        // integers from 0 to 256 instead of doubles
        for (n = 0; n < img_size; ++ n) {
            row[n] = std::floor(pixels[img_size * n + i]
                                * 256.0
                                / (double)QuantumRange);
        }
        for (k = 0; k < n; ++ k) {
            double & y = dct0[img_size * k + i];
            for (n = 0; n < img_size; ++ n) {
                y += row[n] * std::cos(k * (n + 0.5) * factor);
            }
            y *= 2;
        }
    }
}

static void dct_axis_1(const std::vector<double> &dct0,
                       size_t img_size,
                       std::vector<double> &dct) {
    size_t i, n, k;
    double factor = M_PI / (double)img_size;
    dct.resize(img_size * img_size, 0.0);
    for (i = 0; i < img_size; ++ i) {
        for (k = 0; k < img_size; ++ k) {
            double & y = dct[img_size * i + k];
            for (n = 0; n < img_size; ++n) {
                y += dct0[img_size * i + n] * std::cos(k * (n + 0.5) * factor);
            }
            y *= 2;
        }
    }
}

static void hash_from_dct(const std::vector<double> &dct,
                          size_t img_size,
                          size_t hash_size,
                          std::vector<bool> &hash){
    std::vector<double> lowfreq;
    lowfreq.resize(hash_size * hash_size);
    size_t i, n;
    for (i = 0; i < hash_size; ++ i) {
        for (n = 0; n < hash_size; ++ n) {
            lowfreq[hash_size * i + n] = dct[img_size * i + n];
        }
    }
    std::vector<double> lowfreq_sorted = lowfreq;
    std::sort(lowfreq_sorted.begin(), lowfreq_sorted.end());
    size_t mid = (hash_size * hash_size) >> 1;
    double median = hash_size & 1 ?
        lowfreq_sorted[mid]
    :
        (lowfreq_sorted[mid - 1] + lowfreq_sorted[mid]) / 2.0
    ;
    hash.resize(hash_size * hash_size);
    for (i = 0; i < hash_size; ++ i) {
        for (n = 0; n < hash_size; ++ n) {
            hash[hash_size * i + n] = lowfreq[hash_size * i + n] > median;
        }
    }

}

#ifndef ImageHash_hpp
#define ImageHash_hpp

#include <vector>
#include <string>

class ImageHash {
    private:
    std::vector<bool> _hash;
    public:
    ImageHash(const std::vector<bool> &binary_array);
    operator std::string();
    operator const std::vector<bool>&();
    operator std::vector<bool>();
    size_t operator -(const std::vector<bool> &other);
    bool operator ==(const std::vector<bool> &other);
    bool operator !=(const std::vector<bool> &other);
    unsigned hash();
};

#endif /* ImageHash_hpp */

#ifndef HASHUTILS_H
#define HASHUTILS_H

#include <string>

#define HASH_META "000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"

class HashUtils {
public:
    // Computes the SHA-256 of the input string and returns it as a lowercase hex string.
    static std::string SHA256(const std::string& input);
};

#endif // HASHUTILS_H


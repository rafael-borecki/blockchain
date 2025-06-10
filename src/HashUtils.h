#ifndef HASHUTILS_H
#define HASHUTILS_H

#include <string>

const std::string HASH_META = "00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff";

class HashUtils {
public:
    // Computes the SHA-256 of the input string and returns it as a lowercase hex string.
    static std::string SHA256(const std::string& input);
};

#endif // HASHUTILS_H


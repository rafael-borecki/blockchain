#include "HashUtils.h"
#include <openssl/evp.h>      
#include <openssl/err.h>
#include <sstream>
#include <iomanip>

std::string HashUtils::SHA256(const std::string& input) {
    // Create & initialize an EVP_MD_CTX
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Failed to create EVP_MD_CTX");
    }

    // Initialize the digest operation for SHA-256
    if (EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("EVP_DigestInit_ex failed");
    }

    // Feed the data.
    if (EVP_DigestUpdate(ctx, input.data(), input.size()) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("EVP_DigestUpdate failed");
    }

    // Finalize and get the raw hash bytes
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLen = 0;
    if (EVP_DigestFinal_ex(ctx, hash, &hashLen) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("EVP_DigestFinal_ex failed");
    }

    // Clean up
    EVP_MD_CTX_free(ctx);

    // Convert to lowercase hex string
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (unsigned int i = 0; i < hashLen; ++i) {
        oss << std::setw(2) << static_cast<int>(hash[i]);
    }

    return oss.str();
}

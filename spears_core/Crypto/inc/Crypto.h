#pragma once

#include "CommonTypes.h"

#include <string>

enum CryptoKeys { EXTERNAL_KEY, PRIVATE_KEY, PUBLIC_KEY, PASSPHRASE, EXTERNAL_PASSPHRASE };

class Crypto {

private:
    void generate_passphrase();

    std::string external_key;
    std::string private_key;
    std::string public_key;
    std::string passphrase;
    std::string external_passphrase;

public:
    void initialize();
    std::string encrypt(const std::string) const;
    std::string decrypt(const std::string) const;
    void set_key(CryptoKeys, const std::string);
    std::string get_key(CryptoKeys) const;
    std::string hash(const std::string) const;
};

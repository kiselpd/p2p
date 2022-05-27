#include "Crypto.h"

#include <string>
#include <sstream>
#include <iomanip>
#include <utility>
#include <openssl/rsa.h>
#include <openssl/crypto.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/sha.h>
#include <openssl/rand.h>

#define RSA_KEY_LENGTH 2048

void Crypto::initialize() {

    size_t privkey_len = 0;
    size_t pubkey_len = 0;

    RSA *keypair = RSA_generate_key(RSA_KEY_LENGTH, RSA_3, NULL, NULL);

    BIO *privkey = BIO_new(BIO_s_mem());
    BIO *pubkey = BIO_new(BIO_s_mem());

    PEM_write_bio_RSAPrivateKey(privkey, keypair, NULL, NULL, 0, NULL, NULL);
    PEM_write_bio_RSA_PUBKEY(pubkey, keypair);

    privkey_len = BIO_pending(privkey);
    pubkey_len = BIO_pending(pubkey);

    char* private_key = (char*)malloc(privkey_len + 1);
    char* public_key = (char*)malloc(pubkey_len + 1);

    BIO_read(privkey, private_key, privkey_len);
    BIO_read(pubkey, public_key, pubkey_len);

    private_key[privkey_len] = '\0';
    public_key[pubkey_len] = '\0';

    this->private_key = private_key;
    this->public_key = public_key;

    RSA_free(keypair);
    BIO_free_all(pubkey);
    BIO_free_all(privkey);

    free(private_key);
    free(public_key);

    generate_passphrase();
}

std::string Crypto::decrypt(const std::string source) const {

    std::string result = "";
    BIO *keybio = BIO_new_mem_buf((unsigned char *)public_key.c_str(), -1);
    RSA *rsa = RSA_new();

    rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);

    if (!rsa) {
        BIO_free_all(keybio);
        return result;
    }

    int len = RSA_size(rsa);
    char *text = new char[len + 1]{0};

    int ret = RSA_public_decrypt(source.length(), (const unsigned char*)source.c_str(), (unsigned char*)text, rsa, RSA_PKCS1_PADDING);
    if (ret >= 0) {
        result.append(std::string(text, ret));
    }


    delete text;
    BIO_free_all(keybio);
    RSA_free(rsa);

    return result;
}

std::string Crypto::encrypt(const std::string source) const {

    std::string result = "";
    BIO *keybio = BIO_new_mem_buf((unsigned char *)private_key.c_str(), -1);
    RSA* rsa = RSA_new();
    rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);


    if (!rsa) {
        BIO_free_all(keybio);
        return result;
    }

    int key_len = RSA_size(rsa);
    int block_len = key_len - 11;

    char *subtext = new char[key_len + 1]{0};
    int ret = 0;
    int pos = 0;
    std::string substr;

    while (pos < source.length()) {
        substr = source.substr(pos, block_len);

        ret = RSA_private_encrypt(substr.length(), (const unsigned char*)substr.c_str(), (unsigned char*)subtext, rsa, RSA_PKCS1_PADDING);
        if (ret >= 0) {
            result.append(std::string(subtext, ret));
        }
        pos += block_len;
    }

    delete subtext;
    BIO_free_all(keybio);
    RSA_free(rsa);

    return result;
}

void Crypto::set_key(CryptoKeys key_type, const std::string key) {

    switch (key_type) {

        case EXTERNAL_KEY:
            external_key = key;
            break;
        case EXTERNAL_PASSPHRASE:
            external_passphrase = key;
            break;
        default:
            break;

    };
}

std::string Crypto::get_key(CryptoKeys key_type) const {
    switch (key_type) {
        case PUBLIC_KEY:
            return public_key;
        case PASSPHRASE:
            return passphrase;
        default:
            return "";
    };
}

std::string Crypto::hash(const std::string password) const {

    unsigned char hash[SHA512_DIGEST_LENGTH];

    SHA512_CTX sha512;
    SHA512_Init(&sha512);
    SHA512_Update(&sha512, password.c_str(), password.size());
    SHA512_Final(hash, &sha512);

    std::stringstream ss;
    for(size_t i = 0; i < SHA512_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    return ss.str();
}

void Crypto::generate_passphrase() {

    union {
        unsigned int num;
        unsigned char buff[sizeof(unsigned int)];
    } u;

    RAND_bytes(u.buff, sizeof(u.buff));

    this->passphrase = hash(std::to_string(u.num));
}

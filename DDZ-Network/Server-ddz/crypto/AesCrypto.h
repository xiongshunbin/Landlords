#ifndef SERVER_DDZ_AESCRYPTO_H
#define SERVER_DDZ_AESCRYPTO_H

#include <string>
#include <map>
#include <openssl/evp.h>

class AesCrypto
{
public:
    enum AlgorithmType
    {
        // 16 byte
        AES_ECB_128 = 80,
        AES_CBC_128 = 81,
        AES_CFB_128 = 82,
        AES_OFB_128 = 83,
        AES_CTR_128 = 84,

        // 24 byte
        AES_ECB_192 = 120,
        AES_CBC_192 = 121,
        AES_CFB_192 = 122,
        AES_OFB_192 = 123,
        AES_CTR_192 = 124,

        // 32 byte
        AES_ECB_256 = 160,
        AES_CBC_256 = 161,
        AES_CFB_256 = 162,
        AES_OFB_256 = 163,
        AES_CTR_256 = 164,
    };

    enum CryptoType
    {
        DECRYPTO = 0,
        ENCRYPTO = 1
    };

    // 定义函数指针
    using AlgorithmFunc = const EVP_CIPHER *(*)(void);

    AesCrypto(AlgorithmType type, const std::string& key);
    ~AesCrypto();

    // 加密
    std::string encrypt(std::string text);

    // 解密
    std::string decrypt(std::string text);

private:
    std::string aesCrypto(std::string text, CryptoType type);
    void generateIvec(unsigned char* ivec);

private:
    AlgorithmType m_type;
    std::string m_key;
    static const std::map<AlgorithmType, std::pair<int, AlgorithmFunc>> m_algorithms;
};

#endif //SERVER_DDZ_AESCRYPTO_H

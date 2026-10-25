#ifndef SERVER_DDZ_RSACRYPTO_H
#define SERVER_DDZ_RSACRYPTO_H

/*
 * 非对称加密可加密的最大数据长度为: 密钥的位数 / 8 - 11; (11为填充数据)
 */

#include <string>
#include <map>
#include <openssl/evp.h>
#include "Hash.h"

class RsaCrypto
{
public:
    enum KeyLength
    {
        BITS_1K = 1024,
        BITS_2K = 2048,
        BITS_3K = 3072,
        BITS_4K = 4096
    };

    enum KeyType
    {
        PUBLIC_KEY,
        PRIVATE_KEY
    };

    RsaCrypto() = default;

    // 构造对象并加载密钥文件中的数据到内存中
    RsaCrypto(std::string fileName, KeyType type);

    ~RsaCrypto();

    // 将密钥字符串解析为密钥类型
    void parseStringtoKey(std::string data, KeyType type);

    // 生成密钥对
    void generateRsaKey(KeyLength bits, std::string pub = "public.pem", std::string pri = "private.pem");

    // 通过公钥进行加密
    std::string pubKeyEncrypt(std::string data);

    // 通过私钥进行解密
    std::string priKeyDecrypt(std::string data);

    // 数据签名
    std::string sign(std::string data, HashAlgorithm hash = HashAlgorithm::Sha256);

    // 签名校验
    bool verify(std::string sign, std::string data, HashAlgorithm hash = HashAlgorithm::Sha256);

private:
    EVP_PKEY* m_pubKey = NULL;
    EVP_PKEY* m_priKey = NULL;
};

#endif //SERVER_DDZ_RSACRYPTO_H

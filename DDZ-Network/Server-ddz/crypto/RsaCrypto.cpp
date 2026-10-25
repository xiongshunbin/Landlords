#include "RsaCrypto.h"
#include "Base64.h"
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <cassert>

RsaCrypto::RsaCrypto(std::string fileName, KeyType type)
{
    BIO* bio = BIO_new_file(fileName.data(), "rb");
    assert(bio != NULL);

    switch (type)
    {
    case KeyType::PUBLIC_KEY:
        PEM_read_bio_PUBKEY(bio, &m_pubKey, NULL, NULL);
        break;
    case KeyType::PRIVATE_KEY:
        PEM_read_bio_PrivateKey(bio, &m_priKey, NULL, NULL);
        break;
    default:
        break;
    }

    BIO_free(bio);
}

RsaCrypto::~RsaCrypto()
{
    if (m_pubKey != NULL)
        EVP_PKEY_free(m_pubKey);
    if (m_priKey != NULL)
        EVP_PKEY_free(m_priKey);
}

void RsaCrypto::parseStringtoKey(std::string data, KeyType type)
{
    BIO* bio = BIO_new_mem_buf(data.data(), data.size());
    assert(bio != NULL);

    switch (type)
    {
    case KeyType::PUBLIC_KEY:
        PEM_read_bio_PUBKEY(bio, &m_pubKey, NULL, NULL);
        break;
    case KeyType::PRIVATE_KEY:
        PEM_read_bio_PrivateKey(bio, &m_priKey, NULL, NULL);
        break;
    default:
        break;
    }

    BIO_free(bio);
}

void RsaCrypto::generateRsaKey(KeyLength bits, std::string pub, std::string pri)
{
    // 创建密钥上下文 ctx
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    assert(ctx != NULL);

    // 初始化密钥上下文 ctx
    int ret = EVP_PKEY_keygen_init(ctx);
    assert(ret == 1);

    // 指定密钥对长度
    ret = EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, static_cast<int>(bits));
    assert(ret == 1);

    // 生成密钥对
    ret = EVP_PKEY_generate(ctx, &m_priKey);
    assert(ret == 1);

    // 释放密钥上下文 ctx
    EVP_PKEY_CTX_free(ctx);

    // 将密钥写入到磁盘文件
    BIO * bio = BIO_new_file(pri.data(), "wb");
    assert(bio != NULL);
    ret = PEM_write_bio_PrivateKey(bio, m_priKey, NULL, NULL, 0, NULL, NULL);
    assert(ret == 1);
    BIO_flush(bio);
    BIO_free(bio);

    bio = BIO_new_file(pub.data(), "wb");
    assert(bio != NULL);
    ret = PEM_write_bio_PUBKEY(bio, m_priKey);
    assert(ret == 1);
    BIO_flush(bio);
    BIO_free(bio);
}

std::string RsaCrypto::pubKeyEncrypt(std::string data)
{
    // 创建上下文 ctx
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(m_pubKey, NULL);
    assert(ctx != NULL);

    // 初始化上下文 ctx
    int ret = EVP_PKEY_encrypt_init(ctx);
    assert(ret == 1);

    // 设置填充方式
    ret = EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING);
    assert(ret == 1);

    // 使用密钥进行加密
    size_t outlen = 0;
    ret = EVP_PKEY_encrypt(ctx, NULL, &outlen, reinterpret_cast<const unsigned char *>(data.data()), data.size());
    assert(ret == 1);
    unsigned char* out = new unsigned char[outlen];
    ret = EVP_PKEY_encrypt(ctx, out, &outlen, reinterpret_cast<const unsigned char *>(data.data()), data.size());
    assert(ret == 1);

    Base64 base64;
    std::string retStr = base64.encode(reinterpret_cast<const char*>(out), outlen);

    // 释放资源
    delete [] out;
    EVP_PKEY_CTX_free(ctx);

    return retStr;
}

std::string RsaCrypto::priKeyDecrypt(std::string data)
{
    // 创建上下文 ctx
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(m_priKey, NULL);
    assert(ctx != NULL);

    // 初始化上下文 ctx
    int ret = EVP_PKEY_decrypt_init(ctx);
    assert(ret == 1);

    // 设置填充方式
    ret = EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING);
    assert(ret == 1);

    Base64 base64;
    data = base64.decode(data);

    // 使用密钥进行解密
    size_t outlen = 0;
    ret = EVP_PKEY_decrypt(ctx, NULL, &outlen, reinterpret_cast<const unsigned char *>(data.data()), data.size());
    assert(ret == 1);
    unsigned char* out = new unsigned char[outlen];
    ret = EVP_PKEY_decrypt(ctx, out, &outlen, reinterpret_cast<const unsigned char *>(data.data()), data.size());
    assert(ret == 1);

    std::string retStr(reinterpret_cast<const char*>(out), outlen);

    // 释放资源
    delete [] out;
    EVP_PKEY_CTX_free(ctx);

    return retStr;
}

std::string RsaCrypto::sign(std::string data, HashAlgorithm hash)
{
    // 计算哈希值
    Hash h(hash);
    h.addData(data);
    std::string md = h.result();

    // 创建上下文 ctx
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(m_priKey, NULL);
    assert(ctx != NULL);

    // 初始化上下文 ctx
    int ret = EVP_PKEY_sign_init(ctx);
    assert(ret == 1);

    // 设置填充方式
    ret = EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING);
    assert(ret == 1);

    // 设置签名算法(设置签名使用的哈希算法)
    ret = EVP_PKEY_CTX_set_signature_md(ctx, (HashMethods.at(hash)).first());
    assert(ret == 1);

    // 使用密钥进行签名
    size_t outlen = 0;
    ret = EVP_PKEY_sign(ctx, NULL, &outlen, reinterpret_cast<const unsigned char *>(md.data()), md.size());
    assert(ret == 1);
    unsigned char* out = new unsigned char[outlen];
    ret = EVP_PKEY_sign(ctx, out, &outlen, reinterpret_cast<const unsigned char *>(md.data()), md.size());
    assert(ret == 1);

    Base64 base64;
    std::string retStr = base64.encode(reinterpret_cast<const char*>(out), outlen);

    // 释放资源
    delete [] out;
    EVP_PKEY_CTX_free(ctx);

    return retStr;
}

bool RsaCrypto::verify(std::string sign, std::string data, HashAlgorithm hash)
{
    Base64 base64;
    sign = base64.decode(sign);

    // 计算哈希值
    Hash h(hash);
    h.addData(data);
    std::string md = h.result();

    // 创建上下文 ctx
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(m_pubKey, NULL);
    assert(ctx != NULL);

    // 初始化上下文 ctx
    int ret = EVP_PKEY_verify_init(ctx);
    assert(ret == 1);

    // 设置填充方式, 注: 不能使用 RSA_PKCS1_OAEP_PADDING 填充方式, 否则生成的签名将不可控
    ret = EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING);
    assert(ret == 1);

    // 设置签名校验算法(设置签名校验使用的哈希算法)
    ret = EVP_PKEY_CTX_set_signature_md(ctx, (HashMethods.at(hash).first)());
    assert(ret == 1);

    // 使用密钥进行签名校验
    ret = EVP_PKEY_verify(ctx, reinterpret_cast<const unsigned char *>(sign.data()), sign.size(),
                            reinterpret_cast<const unsigned char *>(md.data()), md.size());

    // 释放资源
    EVP_PKEY_CTX_free(ctx);

    bool is_success = false;
    if (ret == 1)
        is_success = true;

    return is_success;
}

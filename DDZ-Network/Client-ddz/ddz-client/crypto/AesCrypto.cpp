#include "AesCrypto.h"
#include <openssl/aes.h>
#include <QCryptographicHash>

const QMap<AesCrypto::AlgorithmType, QPair<int, AesCrypto::AlgorithmFunc>> AesCrypto::m_algorithms = {
    {AesCrypto::AES_ECB_128, {16, EVP_aes_128_ecb}},
    {AesCrypto::AES_CBC_128, {16, EVP_aes_128_cbc}},
    {AesCrypto::AES_CFB_128, {16, EVP_aes_128_cfb128}},
    {AesCrypto::AES_OFB_128, {16, EVP_aes_128_ofb}},
    {AesCrypto::AES_CTR_128, {16, EVP_aes_128_ctr}},

    {AesCrypto::AES_ECB_192, {24, EVP_aes_192_ecb}},
    {AesCrypto::AES_CBC_192, {24, EVP_aes_192_cbc}},
    {AesCrypto::AES_CFB_192, {24, EVP_aes_192_cfb128}},
    {AesCrypto::AES_OFB_192, {24, EVP_aes_192_ofb}},
    {AesCrypto::AES_CTR_192, {24, EVP_aes_192_ctr}},

    {AesCrypto::AES_ECB_256, {32, EVP_aes_256_ecb}},
    {AesCrypto::AES_CBC_256, {32, EVP_aes_256_cbc}},
    {AesCrypto::AES_CFB_256, {32, EVP_aes_256_cfb128}},
    {AesCrypto::AES_OFB_256, {32, EVP_aes_256_ofb}},
    {AesCrypto::AES_CTR_256, {32, EVP_aes_256_ctr}},
};

AesCrypto::AesCrypto(AlgorithmType type, const QByteArray &key, QObject *parent) : QObject(parent)
{
    assert(m_algorithms.value(type).first == key.size());

    m_type = type;
    m_key = key;
}

AesCrypto::~AesCrypto()
{

}

QByteArray AesCrypto::encrypt(QByteArray text)
{
    return aesCrypto(text, CryptoType::ENCRYPTO);
}

QByteArray AesCrypto::decrypt(QByteArray text)
{
    return aesCrypto(text, CryptoType::DECRYPTO);
}

QByteArray AesCrypto::aesCrypto(QByteArray text, CryptoType type)
{
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    unsigned char ivec[AES_BLOCK_SIZE];
    generateIvec(ivec);
    int ret = EVP_CipherInit_ex(ctx, (m_algorithms.value(m_type).second)(), NULL,
                            reinterpret_cast<unsigned char*>(m_key.data()), ivec, static_cast<int>(type));
    assert(ret == 1);

    // 准备存储数据的内存
    int length = text.size() + 1;
    if (length % AES_BLOCK_SIZE != 0)
        length = ((length / AES_BLOCK_SIZE) + 1) * AES_BLOCK_SIZE;
    unsigned char* out = new unsigned char[length];
    int outLen = 0, totalLen = 0;
    ret = EVP_CipherUpdate(ctx, out, &outLen, reinterpret_cast<const unsigned char*>(text.data()), text.size());
    totalLen += outLen;
    assert(ret == 1);

    ret = EVP_CipherFinal_ex(ctx, out + outLen, &outLen);
    totalLen += outLen;
    assert(ret == 1);

    QByteArray res(reinterpret_cast<const char*>(out), totalLen);
    delete [] out;
    EVP_CIPHER_CTX_free(ctx);

    return res;
}

void AesCrypto::generateIvec(unsigned char *ivec)
{
    QCryptographicHash hs(QCryptographicHash::Md5);
    hs.addData(m_key);
    std::string res = hs.result().toStdString();
    memcpy(ivec, res.data(), AES_BLOCK_SIZE);
}

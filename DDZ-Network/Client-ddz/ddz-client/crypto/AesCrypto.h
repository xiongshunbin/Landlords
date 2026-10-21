#ifndef __AESCRYPTO_H__
#define __AESCRYPTO_H__

#include <QObject>
#include <QByteArray>
#include <QMap>
#include <openssl/evp.h>

enum class AlgorithmType : uint8_t
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

/// 定义函数指针
using AlgorithmFunc = const EVP_CIPHER *(*)(void);

class AesCrypto : public QObject
{
    Q_OBJECT
public:
    enum class CryptoType : uint8_t
    {
        DECRYPTO = 0,
        ENCRYPTO = 1
    };

    AesCrypto(AlgorithmType type, const QByteArray& key, QObject* parent = nullptr);
    ~AesCrypto();

    // 加密
    QByteArray encrypt(QByteArray text);

    // 解密
    QByteArray decrypt(QByteArray text);

private:
    QByteArray aesCrypto(QByteArray text, CryptoType type);
    void generateIvec(unsigned char* ivec);

private:
    AlgorithmType m_type;
    QByteArray m_key;
    static const QMap<AlgorithmType, QPair<int, AlgorithmFunc>> m_algorithms;
};

#endif  // __AESCRYPTO_H__

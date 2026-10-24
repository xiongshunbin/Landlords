#ifndef RSACRYPTO_H
#define RSACRYPTO_H

/**
 * 非对称加密可加密的最大数据长度为: 密钥的位数 / 8 - 11; (11为填充数据)
 */

#include <QObject>
#include <QCryptographicHash>
#include <openssl/evp.h>
#include <QMap>

class RsaCrypto : public QObject
{
    Q_OBJECT
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

    using HashFunc = const EVP_MD *(*)(void);

    explicit RsaCrypto(QObject* parent = nullptr);

    // 构造对象并加载密钥文件中的数据到内存中
    explicit RsaCrypto(QByteArray fileName, KeyType type, QObject* parent = nullptr);

    ~RsaCrypto();

    // 将密钥字符串解析为密钥类型
    void parseStringtoKey(QByteArray data, KeyType type);

    // 生成密钥对
    void generateRsaKey(KeyLength bits, QByteArray pub = "public.pem", QByteArray pri = "private.pem");

    // 通过公钥进行加密
    QByteArray pubKeyEncrypt(QByteArray data);

    // 通过私钥进行解密
    QByteArray priKeyDecrypt(QByteArray data);

    // 数据签名
    QByteArray sign(QByteArray data, QCryptographicHash::Algorithm hash = QCryptographicHash::Sha256);

    // 签名校验
    bool verify(QByteArray sign, QByteArray data, QCryptographicHash::Algorithm hash = QCryptographicHash::Sha256);

private:
    EVP_PKEY* m_pubKey = NULL;
    EVP_PKEY* m_priKey = NULL;

    static const QMap<QCryptographicHash::Algorithm, HashFunc> m_hashMethods;
};

#endif // RSACRYPTO_H

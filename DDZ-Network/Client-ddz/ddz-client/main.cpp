#include <QtWidgets/QApplication>
#include <QResource>
#include "Cards.h"
// #include "Loading.h"
#include "Login.h"
#include "AesCrypto.h"
#include "RsaCrypto.h"
#include "Base64.h"
#include <QDebug>

void test()
{
    AesCrypto aes(AesCrypto::AES_CBC_128, "1234567887654321");
    QByteArray origin = "习近平强调，各级党委和政府要坚定不移贯彻落实党中央关于“三农”工作的决策部署，坚持农业农村优先发展，坚持城乡融合发展，把责任扛在肩上、抓在手上，结合实际创造性开展工作，有力有效推进乡村全面振兴，以加快农业农村现代化更好推进中国式现代化建设。";
    QByteArray text = aes.encrypt(origin);

    text = aes.decrypt(text);
    qDebug() << text.data();
    if (text == origin)
    {
        qDebug() << "AES 加解密成功!";
    }

    RsaCrypto rsa;
    rsa.generateRsaKey(RsaCrypto::BITS_2K);

    RsaCrypto rsa1("public.pem", RsaCrypto::PUBLIC_KEY);
    // 非对称加密可加密的最大数据长度为: 密钥的位数 / 8 - 11; (11为填充数据)
    QByteArray temp = "天长地久有时尽，此恨绵绵无绝期。";
    QByteArray ciphertext = rsa1.pubKeyEncrypt(temp);
    qDebug() << "公钥加密的密文: " << ciphertext.data();

    RsaCrypto rsa2("private.pem", RsaCrypto::PRIVATE_KEY);
    QByteArray plaintext = rsa2.priKeyDecrypt(ciphertext);
    qDebug() << plaintext.data();
    if (plaintext == temp)
    {
        qDebug() << "RSA 加解密成功!";
    }

    QByteArray sig = rsa2.sign(origin);
    qDebug() << "私钥签名的密文: " << sig.data();

    bool flag = rsa1.verify(sig, origin);
    qDebug() << "签名校验结果: flag = " << flag;

    const char data[] = {0x01, 0x02, 0x03, 0x04};
    QByteArray str(data, sizeof(data));
    Base64 b64;
    QByteArray en_str = b64.encode(str);
    qDebug() << "encode str: " << en_str.data();
    QByteArray de_str = b64.decode(en_str);
    if (de_str == str)
    {
        qDebug() << "Base64 编解码成功!";
    }
}

int main(int argc, char *argv[])
{
    test();
    QApplication a(argc, argv);
    qRegisterMetaType<Cards>("Cards&");
    qRegisterMetaType<Cards>("Cards");
    // QResource::registerResource("./resource.rcc");
    // Loading w;
    Login w;
    w.show();
    return a.exec();
}

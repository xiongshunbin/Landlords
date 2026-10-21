#include <QtWidgets/QApplication>
#include <QResource>
#include "Cards.h"
// #include "Loading.h"
#include "Login.h"
#include "AesCrypto.h"
#include <QDebug>

void test()
{
    AesCrypto aes(AlgorithmType::AES_CBC_128, "1234567887654321");
    QByteArray origin = "习近平强调，各级党委和政府要坚定不移贯彻落实党中央关于“三农”工作的决策部署，坚持农业农村优先发展，坚持城乡融合发展，把责任扛在肩上、抓在手上，结合实际创造性开展工作，有力有效推进乡村全面振兴，以加快农业农村现代化更好推进中国式现代化建设。";
    QByteArray text = aes.encrypt(origin);

    text = aes.decrypt(text);
    qDebug() << text.data();
    if (text == origin)
    {
        qDebug() << "AES 加解密成功!";
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

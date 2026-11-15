#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QObject>
#include <QRunnable>
#include "TcpSocket.h"
#include "Codec.h"
#include "AesCrypto.h"

class Communication : public QObject, public QRunnable
{
    Q_OBJECT
public:
    enum KeyLen
    {
        Len16 = 16,
        Len24 = 24,
        Len32 = 32
    };

    explicit Communication(const Message& msg, QObject *parent = nullptr);
    ~Communication();

    inline void stopLoop() { m_stop = true; }

    // 发送数据
    bool sendMessage(Message *msg, bool encrypt = true);

    // 解析接收到的服务器发送的数据
    void parseRecvMessage();

    // 密钥分发
    void handleRsaFenFa(Message *msg);

    // 密钥分发成功
    void handleAesVerifyOK();

    // 生成对称加密的密钥(AES)
    QByteArray generateAesKey(KeyLen len);

protected:
    void run() override;

signals:
    void connectFailed();
    void loginOK();
    void registerOK();
    void requestFailed(QByteArray msg);

private:
    bool m_stop = false;
    Message m_msgInfo;
    TcpSocket* m_socket = nullptr;
    QByteArray m_aesKey;
    AesCrypto* m_aes = nullptr;
};

#endif // COMMUNICATION_H

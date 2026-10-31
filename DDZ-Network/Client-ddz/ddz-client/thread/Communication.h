#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QObject>
#include <QRunnable>
#include "TcpSocket.h"
#include "Codec.h"

class Communication : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit Communication(Message *msg, QObject *parent = nullptr);

    inline void stopLoop() { m_stop = true; }

    // 发送数据
    bool sendMessage(Message *msg);

    // 解析接收到的服务器发送的数据
    void parseRecvMessage();

protected:
    void run() override;

signals:
    void connectFailed();

private:
    bool m_stop = false;
    Message* m_msgInfo = nullptr;
    TcpSocket* m_socket = nullptr;
};

#endif // COMMUNICATION_H

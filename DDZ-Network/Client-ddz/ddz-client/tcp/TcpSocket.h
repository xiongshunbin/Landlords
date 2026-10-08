#ifndef __TCPSOCKET_H__
#define __TCPSOCKET_H__

#include <QObject>

#ifdef Q_OS_WIN
#include <winsock2.h>
#elif
#include <sys/socket.h>
#endif

class TcpSocket : public QObject
{
    Q_OBJECT
public:
    explicit TcpSocket(QObject* parent = nullptr);
    TcpSocket(QByteArray ip, unsigned short port, QObject* parent = nullptr);
    ~TcpSocket();

    // 连接服务器
    bool connectToServer(QByteArray ip, unsigned short port);
    // 接收数据
    QByteArray receiveMessage(int timeout = -1);    // 单位: 秒
    // 发送数据
    void sendMessage(QByteArray msg, int timeout = -1);
    // 断开连接
    void disConnect();

private:
    bool readTimeout(int timeout_sec);
    bool writeTimeout(int timeout_sec);
private:
#ifdef Q_OS_WIN
    SOCKET m_socket;
#elif Q_OS_LINUX
    int m_socket;
#endif
};

#endif  // __TCPSOCKET_H__
#include "TcpSocket.h"

#ifdef Q_OS_LINUX
#include <sys/select.h>
#include <sys/time.h>
#endif

TcpSocket::TcpSocket(QObject *parent)
{
#ifdef Q_OS_WIN
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
}

TcpSocket::TcpSocket(QByteArray ip, unsigned short port, QObject *parent) : TcpSocket(parent)
{
    connectToServer(ip, port);
}

TcpSocket::~TcpSocket()
{
#ifdef Q_OS_WIN
    WSACleanup();
#endif
}

bool TcpSocket::connectToServer(QByteArray ip, unsigned short port)
{
    assert(port > 0);
    m_socket = socket(PF_INET, SOCK_STREAM, 0);
#ifdef Q_OS_WIN
    assert(m_socket != INVALID_SOCKET);
#elif Q_OS_LINUX
    assert(m_socket != -1);
#endif
    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(ip.data());
    servAddr.sin_port = htons(port);
    int ret = ::connect(m_socket, reinterpret_cast<struct sockaddr*>(&servAddr), sizeof(servAddr));
    bool flag = (ret == 0) ? true : false;
    return flag;
}

QByteArray TcpSocket::receiveMessage(int timeout)
{

}

void TcpSocket::sendMessage(QByteArray msg, int timeout)
{

}

void TcpSocket::disConnect()
{
#ifdef Q_OS_WIN
    if (m_socket != INVALID_SOCKET)
    {
        closesocket(m_socket);
    }
#elif Q_OS_LINUX
    if (m_socket != -1)
    {
        close(m_socket);
    }
#endif
}

bool TcpSocket::readTimeout(int timeout_sec)
{
    if (timeout_sec == -1)
    {
        return true;    // 阻塞读数据
    }

    // 检测读缓冲区
#ifdef Q_OS_WIN
    int nfds = 0;
#elif !_OS_LINUX
    int nfds = m_socket + 1;
#endif
    fd_set reads;
    FD_ZERO(&reads);
    FD_SET(m_socket, &reads);
    struct timeval timeout;
    timeout.tv_sec = timeout_sec;
    timeout.tv_usec = 0;
    int ret = select(nfds, &reads, NULL, NULL, &timeout);
    bool flag = (ret == -1) ? true : false;
    return flag;
}

bool TcpSocket::writeTimeout(int timeout_sec)
{
    if (timeout_sec == -1)
    {
        return true;    // 阻塞写数据
    }

    // 检测写缓冲区
#ifdef Q_OS_WIN
    int nfds = 0;
#elif !_OS_LINUX
    int nfds = m_socket + 1;
#endif
    fd_set writes;
    FD_ZERO(&writes);
    FD_SET(m_socket, &writes);
    struct timeval timeout;
    timeout.tv_sec = timeout_sec;
    timeout.tv_usec = 0;
    int ret = select(nfds, NULL, &writes, NULL, &timeout);
    bool flag = (ret == -1) ? true : false;
    return flag;
}

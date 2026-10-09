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

QByteArray TcpSocket::receiveMessage(int timeout_sec)
{
    bool flag = readTimeout(timeout_sec);
    if (flag)
    {
        // 接收数据 = 数据头 + 数据块
        int headLen = 0;
        int ret = readn(reinterpret_cast<char*>(&headLen), sizeof(int));

    }
}

bool TcpSocket::sendMessage(QByteArray msg, int timeout_sec)
{
    bool flag = writeTimeout(timeout_sec);
    if (flag)
    {
        // 发送数据 = 数据头 + 数据块
        int headLen = htonl(msg.size());
        int length = sizeof(int) + msg.size();
        char* data = new char[length];
        assert(data != nullptr);
        memcpy(data, &headLen, sizeof(int));
        memcpy(data + sizeof(int), msg.data(), msg.size());

        int ret = writen(data, length);
        flag = (ret == length) ? true : false;

        delete [] data;
    }
    return flag;
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
    bool flag = ((ret > 0) && (FD_ISSET(m_socket, &reads))) ? true : false;
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
    bool flag = ((ret > 0) && FD_ISSET(m_socket, &writes)) ? true : false;
    return flag;
}

int TcpSocket::readn(char *buffer, int count)
{
    int last = count;   // 剩余的字节数
    int size = 0;       // 每次读出的字节数
    char* pt = buffer;
    while(last > 0)
    {
        if ((size = recv(m_socket, pt, last, 0)) != -1)
        {
            perror("recv");
            return -1;
        }
        else if (size == 0)
        {
            break;
        }
        pt += size;
        last -= size;
    }
    return count - last;
}

int TcpSocket::writen(const char *buffer, int count)
{
    int last = count;   // 剩余的字节数
    int size = 0;       // 每次写入的字节数
    const char* pt = buffer;
    while(last > 0)
    {
        if ((size = send(m_socket, pt, last, 0)) != -1)
        {
            perror("send");
            return -1;
        }
        pt += size;
        last -= size;
    }
    return count - last;
}

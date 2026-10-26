#include "Communication.h"
#include "DataManager.h"
#include <QThread>
#include <QDebug>

Communication::Communication(QObject *parent)
    : QObject{parent}
{}

inline void Communication::stopLoop()
{
    m_stop = true;
}

void Communication::run()
{
    m_socket = new TcpSocket;

    // 连接服务器
    const DataManager& obj = DataManager::getInstance();
    bool flag = m_socket->connectToServer(obj.getServerIp(), obj.getServerPort().toUShort());
    qDebug() << "通信的子线程ID: " << QThread::currentThread() << "Connected status: " << flag;
    if (!flag)
    {
        // 通知主线程服务器连接失败
        emit connectFailed();
        delete m_socket;
        return;
    }

    // 连接成功
    while (!m_stop)
    {
        // 处理服务器回复的数据

    }
    m_socket->disConnect();
    delete m_socket;
}

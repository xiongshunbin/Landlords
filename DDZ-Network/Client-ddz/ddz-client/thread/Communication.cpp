#include "Communication.h"
#include "DataManager.h"
#include <QThread>
#include <QDebug>

Communication::Communication(Message *msg, QObject *parent)
    : QObject{parent}, m_msgInfo{msg}
{
    this->setAutoDelete(true);
}

bool Communication::sendMessage(Message *msg)
{
    Codec codec(msg);
    QByteArray data = codec.encodeMessage();
    // 数据加密

    bool flag = m_socket->sendMessage(data);
    return flag;
}

void Communication::parseRecvMessage()
{
    // 接收数据
    QByteArray data = m_socket->receiveMessage(2);

    // 数据反序列化
    Codec codec(data);
    std::unique_ptr<Message> ptr = codec.decodeMessage();

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

        /*
         * 1.得到服务器分发的非对称加密的公钥
         * 2.生成对称加密的密钥
         * 3.使用得到的非对称加密的公钥对生成的对称加密的密钥进行加密
         * 4.发送加密之后的对称加密的密钥给服务器
         * 5.得到服务器回复接收密钥成功的消息
         * 6.发送客户端请求(登录、注册)
         */
        parseRecvMessage();
    }
    m_socket->disConnect();
    delete m_socket;
}

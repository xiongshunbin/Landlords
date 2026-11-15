#include "Communication.h"
#include "DataManager.h"
#include "RsaCrypto.h"
#include <QThread>
#include <QDebug>
#include <QDateTime>

Communication::Communication(const Message &msg, QObject *parent)
    : QObject{parent}, m_msgInfo{msg}
{
    this->setAutoDelete(true);
}

Communication::~Communication()
{
    if (m_aes != nullptr)
        delete m_aes;
}

bool Communication::sendMessage(Message *msg, bool encrypt)
{
    Codec codec(msg);
    QByteArray data = codec.encodeMessage();

    // 数据加密
    if (encrypt)
    {
        data = m_aes->encrypt(data);
    }

    bool flag = m_socket->sendMsg(data);
    return flag;
}

void Communication::parseRecvMessage()
{
    // 接收数据
    QByteArray data = m_socket->recvMsg(2);
    if (data.isEmpty())
        return;

    // 数据反序列化
    Codec codec(data);
    std::unique_ptr<Message> ptr = codec.decodeMessage();
    qDebug() << "Received server data: rescode =" << ptr->rescode << "data1 ="
             << ptr->data1.data() << "data2 =" << ptr->data2.data() << "data3 =" << ptr->data3.data();
    switch(ptr->rescode)
    {
    case ResponseCode::LoginOK:
        emit loginOK();
        break;
    case ResponseCode::RegisterOK:
        emit registerOK();
        break;
    case ResponseCode::RsaFenFa:
        handleRsaFenFa(ptr.get());
        break;
    case ResponseCode::AesVerifyOK:
        handleAesVerifyOK();
        break;
    case ResponseCode::Failed:
        emit requestFailed(ptr->data1);
        break;
    default:
        break;
    }
}

/**
 * 1.得到服务器分发的非对称加密的公钥后进行签名校验
 * 2.签名校验成功后, 生成对称加密的密钥
 * 3.使用得到的非对称加密的公钥对生成的对称加密的密钥进行加密
 * 4.发送加密之后的对称加密的密钥给服务器
 * 5.得到服务器回复接收密钥成功的消息
 * 6.发送客户端请求(登录、注册)
*/
void Communication::handleRsaFenFa(Message* msg)
{
    RsaCrypto rsa;
    rsa.parseStringtoKey(msg->data1, RsaCrypto::PUBLIC_KEY);

    // 签名校验
    bool flag = rsa.verify(msg->data2, msg->data1);
    if (!flag)
    {
        qDebug() << "签名校验失败";
    }

    // 生成对称加密的密钥
    m_aesKey = generateAesKey(KeyLen::Len32);

    Message reqMsg;
    reqMsg.reqcode = RequestCode::AesFenFa;
    reqMsg.data1 = rsa.pubKeyEncrypt(m_aesKey);
    reqMsg.data2 = QCryptographicHash::hash(m_aesKey, QCryptographicHash::Sha224).toHex();    // 16进制数字串格式的哈希值
    sendMessage(&reqMsg, false);
}

void Communication::handleAesVerifyOK()
{
    qDebug() << "AES密钥分发成功!";
    m_aes = new AesCrypto(AesCrypto::AES_CBC_256, m_aesKey);
    sendMessage(&m_msgInfo);
}

/**
 * 生成规则: 对当前的时间使用Sha3_384进行哈希计算截取相应长度的数据作为对称加密的密钥
 */
QByteArray Communication::generateAesKey(KeyLen len)
{
    QByteArray time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz").toUtf8();
    // 进行哈希计算
    QCryptographicHash hash(QCryptographicHash::Sha3_384);
    hash.addData(time);
    QByteArray res = hash.result();
    return res.left(len);
}

void Communication::run()
{
    m_socket = new TcpSocket;

    // 连接服务器
    const DataManager& obj = DataManager::getInstance();
    bool flag = m_socket->connectToServer(obj.getServerIp(), obj.getServerPort().toUShort());
    qDebug() << "通信的子线程ID:" << QThread::currentThread() << "Connected status:" << flag;
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
        parseRecvMessage();
    }
    m_socket->disConnect();
    delete m_socket;
}

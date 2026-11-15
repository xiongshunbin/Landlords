#include "Communication.h"
#include <netinet/in.h>
#include "RsaCrypto.h"
#include "spdlog/spdlog.h"

Communication::Communication()
{
    m_mysql = new MySQLConn;
    bool flag = m_mysql->connect("localhost", "root", "393412", "happy-ddz");
    assert(flag);
}

Communication::~Communication()
{
    if (m_mysql != nullptr)
        delete m_mysql;

    if (m_aes != nullptr)
        delete m_aes;
}

void Communication::parseRequest(Buffer *buf)
{
    // 读数据头
    std::string headLen = buf->data(sizeof(int));
    int net_length = *(reinterpret_cast<int*>(headLen.data()));

    // 读数据块
    int length = ntohl(net_length);
    std::string data = buf->data(length);

    if (m_aes != nullptr)
    {
        data = m_aes->decrypt(data);
    }

    // 数据反序列化
    Codec codec(data);
    std::unique_ptr<Message> ptr = codec.decodeMessage();
    Message resMsg;
    switch (ptr->reqcode)
    {
        case RequestCode::UserLogin:
            handleUserLogin(ptr.get(), resMsg);
            break;
        case RequestCode::UserRegister:
            handleRegister(ptr.get(), resMsg);
            break;
        case RequestCode::AesFenFa:
            handleAesFenFa(ptr.get(), resMsg);
            break;
        default:
            break;
    }
    codec.reloadData(&resMsg);
    f_sendMessage(codec.encodeMessage());
}

void Communication::handleAesFenFa(Message *reqMsg, Message& resMsg)
{
    RsaCrypto rsa("private.pem", RsaCrypto::PRIVATE_KEY);
    std::string aesKey = rsa.priKeyDecrypt(reqMsg->data1);

    // 对得到的对称加密的密钥进行哈希校验
    std::string res = Hash::hash(aesKey, HashAlgorithm::Sha224, Hash::Type::Hex);
    resMsg.rescode = ResponseCode::AesVerifyOK;
    if (reqMsg->data2 != res)
    {
        spdlog::get("basic_logger")->info("AES密钥哈希校验失败!");
        resMsg.rescode = ResponseCode::Failed;
        resMsg.data1 = "AES密钥哈希校验失败!";
    }
    else
    {
        spdlog::get("basic_logger")->info("AES密钥哈希校验成功!");
        m_aes = new AesCrypto(AesCrypto::AES_CBC_256, aesKey);
    }
}

void Communication::handleUserLogin(Message *reqMsg, Message &resMsg)
{
    char sql[1024];
    sprintf(sql, "SELECT * FROM `user` WHERE name='%s' AND passwd='%s' AND (SELECT COUNT(*) FROM `information` WHERE name='%s' AND status=0);",
        reqMsg->userName.data(), reqMsg->data1.data(), reqMsg->userName.data());
    bool flag = m_mysql->query(sql);
    if (flag && m_mysql->next())
    {
        m_mysql->transaction();
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "UPDATE `information` SET status=1 WHERE name='%s';", reqMsg->userName.data());
        flag = m_mysql->executeSQL(sql);
        if (flag)
        {
            m_mysql->commit();
            resMsg.rescode = ResponseCode::LoginOK;
            return;
        }
        m_mysql->rollback();
    }
    resMsg.rescode = ResponseCode::Failed;
    resMsg.data1 = "用户名不存在或密码错误，或者当前用户已在线, 用户登录失败!";
}

void Communication::handleRegister(Message *reqMsg, Message &resMsg)
{
    // 查询数据库: SELECT 字段 FROM 数据库表 WHERE 条件
    char sql[1024];
    sprintf(sql, "SELECT * FROM `user` WHERE name='%s';", reqMsg->userName.data());
    bool flag = m_mysql->query(sql);
    if (flag && !m_mysql->next())
    {
        // 将注册信息写入到数据库
        m_mysql->transaction();
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "INSERT INTO `user`(name, passwd, phone, date) VALUES('%s', '%s', '%s', NOW());",
            reqMsg->userName.data(), reqMsg->data1.data(), reqMsg->data2.data());
        bool flag1 = m_mysql->executeSQL(sql);

        memset(sql, 0, sizeof(sql));
        sprintf(sql, "INSERT INTO `information`(name, score, status) VALUES('%s', 0, 0);", reqMsg->userName.data());
        bool flag2 = m_mysql->executeSQL(sql);

        if (flag1 && flag2)
        {
            m_mysql->commit();
            resMsg.rescode = ResponseCode::RegisterOK;
        }
        else
        {
            m_mysql->rollback();
            resMsg.rescode = ResponseCode::Failed;
            resMsg.data1 = "数据库插入数据失败!";
        }
    }
    else
    {
        resMsg.rescode = ResponseCode::Failed;
        resMsg.data1 = "用户名已经存在或查询数据库失败, 无法进行注册!";
    }
}

void Communication::setCallback(sendCallback func1, deleteCallback func2)
{
    f_sendMessage = func1;
    f_disconnect = func2;
}

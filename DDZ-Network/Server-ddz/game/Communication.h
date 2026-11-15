#ifndef SERVER_DDZ_COMMUNICATION_H
#define SERVER_DDZ_COMMUNICATION_H

#include "Buffer.h"
#include "Codec.h"
#include <string>
#include "AesCrypto.h"
#include "MySQLConn.h"

class Communication
{
public:
    using sendCallback = std::function<void(std::string)>;
    using deleteCallback = std::function<void()>;

    Communication();
    ~Communication();

    // 解析客户端发送的数据
    void parseRequest(Buffer* buf);

    // 处理密钥分发
    void handleAesFenFa(Message* reqMsg, Message& resMsg);

    // 处理用户登录
    void handleUserLogin(Message* reqMsg, Message& resMsg);

    // 处理用户注册
    void handleRegister(Message* reqMsg, Message& resMsg);

    // 设置回调
    void setCallback(sendCallback func1, deleteCallback func2);

private:
    sendCallback f_sendMessage;
    deleteCallback f_disconnect;
    AesCrypto* m_aes = nullptr;
    MySQLConn* m_mysql = nullptr;
};


#endif //SERVER_DDZ_COMMUNICATION_H

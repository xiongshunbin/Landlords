#ifndef SERVER_DDZ_COMMUNICATION_H
#define SERVER_DDZ_COMMUNICATION_H

#include "Buffer.h"
#include "Codec.h"
#include <string>

class Communication
{
public:
    using sendCallback = std::function<void(std::string)>;
    using deleteCallback = std::function<void()>;

    // 解析客户端发送的数据
    void parseRequest(Buffer* buf);

    // 处理密钥分发
    void handleAesFenFa(Message* reqMsg, Message& resMsg);

    // 设置回调
    void setCallback(sendCallback func1, deleteCallback func2);

private:
    std::string m_aesKey;
    sendCallback f_sendMessage;
    deleteCallback f_disconnect;
};


#endif //SERVER_DDZ_COMMUNICATION_H

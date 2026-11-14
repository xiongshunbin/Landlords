#ifndef SERVER_DDZ_COMMUNICATION_H
#define SERVER_DDZ_COMMUNICATION_H

#include "Buffer.h"
#include "Codec.h"
#include <string>
#include "AesCrypto.h"

class Communication
{
public:
    using sendCallback = std::function<void(std::string)>;
    using deleteCallback = std::function<void()>;

    Communication() = default;
    ~Communication();

    // 解析客户端发送的数据
    void parseRequest(Buffer* buf);

    // 处理密钥分发
    void handleAesFenFa(Message* reqMsg, Message& resMsg);

    // 设置回调
    void setCallback(sendCallback func1, deleteCallback func2);

private:
    sendCallback f_sendMessage;
    deleteCallback f_disconnect;
    AesCrypto* m_aes = nullptr;
};


#endif //SERVER_DDZ_COMMUNICATION_H

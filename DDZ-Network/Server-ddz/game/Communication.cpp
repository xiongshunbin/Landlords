#include "Communication.h"
#include <netinet/in.h>
#include "RsaCrypto.h"
#include "spdlog/spdlog.h"

Communication::~Communication()
{
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
            break;
        case RequestCode::UserRegister:
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

void Communication::setCallback(sendCallback func1, deleteCallback func2)
{
    f_sendMessage = func1;
    f_disconnect = func2;
}

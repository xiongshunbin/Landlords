#ifndef CODEC_H
#define CODEC_H

#include <QByteArray>
#include <memory>
#include "Information.pb.h"

/**
 * 数据和protobuf消息体中的数据是一一对应
 * 序列化   -> 基于结构体中的数据进行
 * 反序列化 -> 将解析出的数据存储到结构体中
 */
struct Message
{
    QByteArray userName;
    QByteArray data1;
    QByteArray data2;
    QByteArray data3;
    RequestCode reqcode;
    ResponseCode rescode;
};

class Codec
{
public:
    // 序列化
    Codec(Message* msg);

    // 反序列化
    Codec(QByteArray msg);

    // 数据编码
    QByteArray encodeMessage();

    // 数据解码
    std::unique_ptr<Message> decodeMessage();

    // 重新加载数据
    void reloadData(QByteArray msg);
    void reloadData(Message* msg);

private:
    QByteArray m_msg;
    Information m_obj;       // 处理数据的序列化和反序列化
};

#endif // CODEC_H

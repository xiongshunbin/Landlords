#include "Codec.h"

Codec::Codec(Message *msg)
{
    reloadData(msg);
}

Codec::Codec(std::string msg)
{
    reloadData(msg);
}

std::string Codec::encodeMessage()
{
    m_obj.SerializeToString(&m_msg);
    return m_msg;
}

std::unique_ptr<Message> Codec::decodeMessage()
{
    m_obj.ParseFromString(m_msg);
    std::unique_ptr<Message> msg = std::make_unique<Message>();
    msg->userName = m_obj.username();
    msg->data1 = m_obj.data1();
    msg->data2 = m_obj.data2();
    msg->data3 = m_obj.data3();
    msg->reqcode = m_obj.reqcode();
    msg->rescode = m_obj.rescode();
    return msg;
}

void Codec::reloadData(std::string msg)
{
    m_msg = msg;
}

void Codec::reloadData(Message *msg)
{
    m_obj.set_username(msg->userName);
    m_obj.set_data1(msg->data1);
    m_obj.set_data2(msg->data2);
    m_obj.set_data3(msg->data3);
    m_obj.set_reqcode(msg->reqcode);
    m_obj.set_rescode(msg->rescode);
}

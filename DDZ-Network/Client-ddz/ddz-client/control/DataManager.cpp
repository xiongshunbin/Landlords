#include "DataManager.h"

DataManager &DataManager::getInstance()
{
    static DataManager instance;
    return instance;
}

void DataManager::setUserName(const QByteArray &name)
{
    m_userName = name;
}

void DataManager::setServerIp(const QByteArray &ip)
{
    m_serverIp = ip;
}

void DataManager::setServerPort(const QByteArray &port)
{
    m_serverPort = port;
}

void DataManager::setCommunication(Communication *comm)
{
    m_comm = comm;
}

const QByteArray &DataManager::getUserName() const
{
    return m_userName;
}

const QByteArray &DataManager::getServerIp() const
{
    return m_serverIp;
}

const QByteArray &DataManager::getServerPort() const
{
    return m_serverPort;
}

Communication *DataManager::getCommunication() const
{
    return m_comm;
}

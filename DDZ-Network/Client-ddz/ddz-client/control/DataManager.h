#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QByteArray>

#include "Communication.h"

// 单例模式
class DataManager
{
public:
    static DataManager& getInstance();

    DataManager(const DataManager&) = delete;
    DataManager(const DataManager&&) = delete;
    DataManager& operator=(const DataManager&) = delete;
    DataManager& operator=(const DataManager&&) = delete;

    // 设置数据
    void setUserName(const QByteArray& name);
    void setServerIp(const QByteArray& ip);
    void setServerPort(const QByteArray& port);
    void setCommunication(Communication* comm);

    // 获取数据
    const QByteArray& getUserName() const;
    const QByteArray& getServerIp() const;
    const QByteArray& getServerPort() const;
    Communication* getCommunication() const;

private:
    DataManager() = default;
    ~DataManager() = default;

private:
    QByteArray m_userName;
    QByteArray m_serverIp;
    QByteArray m_serverPort;

    Communication* m_comm;
};

#endif // DATAMANAGER_H

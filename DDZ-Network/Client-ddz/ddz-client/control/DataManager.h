#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QByteArray>


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

    // 获取数据
    const QByteArray& getUserName() const;
    const QByteArray& getServerIp() const;
    const QByteArray& getServerPort() const;

private:
    DataManager() = default;
    ~DataManager() = default;

private:
    QByteArray m_userName;
    QByteArray m_serverIp;
    QByteArray m_serverPort;
};

#endif // DATAMANAGER_H

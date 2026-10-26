#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QObject>
#include <QRunnable>
#include "TcpSocket.h"

class Communication : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit Communication(QObject *parent = nullptr);

    void stopLoop();

protected:
    void run() override;

signals:
    void connectFailed();

private:
    bool m_stop = false;
    TcpSocket* m_socket = nullptr;
};

#endif // COMMUNICATION_H

#ifndef SocketClient_H
#define SocketClient_H

#include <QObject>
#include <QDebug>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include "packet.h"

#define IP "127.0.0.1"
#define PORT 5000

class SocketClient : public QObject
{
    Q_OBJECT
public:
    explicit SocketClient(QObject *parent = nullptr);
    ~SocketClient();
    int connect();
    void disconnect();

private:
    int sockfd = 0;

signals:
    void new_status_message(QString message);
};

#endif // SocketClient_H

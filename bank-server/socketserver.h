#ifndef SocketServer_H
#define SocketServer_H

#include <QObject>
#include <QDebug>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#include "packet.h"

class SocketServer : public QObject
{
    Q_OBJECT
public:
    explicit SocketServer(QObject *parent = nullptr);
    ~SocketServer();
    void start_server();
    void stop_server();
    void handle();

private:
    bool isHandling = true;
    int listenfd = 0;
    int connfd = 0;

signals:
    void new_connection(int descriptor);
};

#endif // SocketServer_H

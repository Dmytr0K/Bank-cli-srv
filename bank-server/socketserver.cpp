#include "socketserver.h"

SocketServer::SocketServer(QObject *parent) : QObject(parent)
{

}

SocketServer::~SocketServer()
{
    close(listenfd);
    close (connfd);
}

void SocketServer::start_server()
{
    struct sockaddr_in server_address;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        qDebug("Couldn't create socket!");
    }

    memset(&server_address, '0', sizeof (server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(5000);

    if (bind(listenfd, (struct sockaddr *) &server_address, sizeof (server_address)) < 0) {
        qDebug("Error binding");
    }

    if (listen(listenfd, 10) < 0) {
        qDebug("Error listening");
    }

    handle();
}

void SocketServer::stop_server()
{
    isHandling = false;
    close(listenfd);
    qDebug() << "Server closed!";
}

void SocketServer::handle()
{
    while (isHandling) {
        connfd = accept(listenfd, (struct sockaddr*) NULL, NULL);
        if (connfd > 0) {
            emit new_connection(connfd);
            connfd = 0;
        }
    }
}

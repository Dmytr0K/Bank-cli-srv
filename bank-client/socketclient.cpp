#include "socketclient.h"

SocketClient::SocketClient(QObject *parent) : QObject(parent)
{

}

SocketClient::~SocketClient()
{
    close(sockfd);
}

int SocketClient::connect()
{
    struct sockaddr_in server_address;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        new_status_message("Couldn't create socket!");
        return 0;
    }

    memset(&server_address, '0', sizeof (server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    if (inet_pton(AF_INET, IP, &server_address.sin_addr) <= 0) {
        new_status_message("inet_pton error occured");
        return 0;
    }

    if (::connect(sockfd, (struct sockaddr *) &server_address, sizeof (server_address)) < 0) {
        new_status_message("Error connecting");
        return 0;
    }
    new_status_message("Connected!");
    return sockfd;
}

void SocketClient::disconnect()
{
    close(sockfd);
    new_status_message("Disconnected!");
}

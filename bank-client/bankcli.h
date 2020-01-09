#ifndef BANKCLI_H
#define BANKCLI_H

#include <QObject>
#include <sys/socket.h>
#include <QDateTime>

#include "packet.h"
#include "socketclient.h"
#include "account.h"

class bankCLI : public QObject
{
    Q_OBJECT
public:
    explicit bankCLI(QObject *parent = nullptr);

    Account & getAuthorized_account();

private:
    char sendBuffer[1025];
    char readBuffer[1025];
    QString packetBuffer;
    int descriptor = 0;

    SocketClient *socketCLI;

    int lastCommand = -1;

    bool wait_reply();
    void packet_handler(Packet &packet);
    bool reply_handler();

    Account authorized_account;

public slots:
    void connect_to_server();
    void authorization(QString login, int pin);
    void new_account(QString login, int pin, int money);
    void transfaction(int from_number, int to_number, int money);
    void get_money(int number, int money);
    void upload_money(int number, int money);
    void close_account(int number);
    void disconnect_from_server();

signals:
    void new_status_message(QString message);
    void new_info(Account acc);
    void connected(bool status);
};

#endif // BANKCLI_H

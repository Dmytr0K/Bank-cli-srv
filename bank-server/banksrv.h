#ifndef BANKSRV_H
#define BANKSRV_H

#include <QObject>

#include "socketserver.h"
#include "db_helper.h"
#include "packet.h"

class BankSRV : public QObject
{
    Q_OBJECT
public:
    explicit BankSRV(QObject *parent = nullptr);

public slots:
    void new_connect_handler(int descriptor);

private:
    int user_num = 0;
    char sendBuffer[1025];
    char readBuffer[1025];
    QString packetBuffer;
    DB_helper *db;

    QString wait_paket(int descriptor);
    void packet_handler(Packet &packet, int descriptor, bool &connected);
};

#endif // BANKSRV_H

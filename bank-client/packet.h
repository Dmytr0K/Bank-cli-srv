#ifndef PACKET_H
#define PACKET_H

#include <QObject>
#include <QByteArray>
#include <QDebug>

#define FLAG 0x7E
#define ESC 0x7D
#define SPLIT "%"

enum {/*TO SRV*/NEW, SEARCH, CLOSE, TRANSFACTION, GET, UPLOAD, DISCONNECT,
    /*TO CLI*/ OK, ERROR, INFO};

class Packet : public QObject
{
    Q_OBJECT
public:
    explicit Packet(QObject *parent = nullptr);

    void packet_parser(QString packet);
    QByteArray get_packet();
    QByteArray byte_stuffing(QByteArray bytes);

    int getCommand() const;
    void setCommand(int value);

    int getNumber() const;
    void setNumber(int value);

    QString getLogin() const;
    void setLogin(const QString &value);

    int getPassw() const;
    void setPassw(int value);

    int getMoney() const;
    void setMoney(int value);

    int getFrom() const;
    void setFrom(const int &value);

    int getTo() const;
    void setTo(const int &value);

    QString getMessage() const;
    void setMessage(const QString &value);

private:
    int command = 0;

    int number;
    QString login;
    int passw;
    int money;

    int from;
    int to;

    QString message;
};

#endif // PACKET_H

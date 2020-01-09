#include "packet.h"

Packet::Packet(QObject *parent) : QObject(parent)
{

}

void Packet::packet_parser(QString packet)
{
    QStringList list = packet.split(SPLIT);
    this->command = list[0].toInt();

    switch (command) {
        /*TO SRV*/
    case NEW:
        this->login = list[1];
        this->passw = list[2].toInt();
        this->money = list[3].toInt();
        break;
    case SEARCH:
        this->login = list[1];
        this->passw = list[2].toInt();
        break;
    case CLOSE:
        this->number = list[1].toInt();
        break;
    case TRANSFACTION:
        this->from = list[1].toInt();
        this->to = list[2].toInt();
        this->money = list[3].toInt();
        break;
    case GET:
        this->number = list[1].toInt();
        this->money = list[2].toInt();
        break;
    case UPLOAD:
        this->number = list[1].toInt();
        this->money = list[2].toInt();
        break;

        /*TO CLI*/
    case ERROR:
        this->message = list[1];
        break;
    case INFO:
        this->number = list[1].toInt();
        this->login = list[2];
        this->money = list[3].toInt();
        break;
    }
}

QByteArray Packet::get_packet()
{
    QByteArray beforBS;
    beforBS.append(QString::number(command));
    switch (command) {
        /*TO SRV*/
    case NEW:
        beforBS.append(SPLIT);
        beforBS.append(login);
        beforBS.append(SPLIT);
        beforBS.append(QString::number(passw));
        beforBS.append(SPLIT);
        beforBS.append(QString::number(money));
        break;
    case SEARCH:
        beforBS.append(SPLIT);
        beforBS.append(login);
        beforBS.append(SPLIT);
        beforBS.append(QString::number(passw));
        break;
    case CLOSE:
        beforBS.append(SPLIT);
        beforBS.append(QString::number(number));
        break;
    case TRANSFACTION:
        beforBS.append(SPLIT);
        beforBS.append(QString::number(from));
        beforBS.append(SPLIT);
        beforBS.append(QString::number(to));
        beforBS.append(SPLIT);
        beforBS.append(QString::number(money));
        break;
    case GET:
        beforBS.append(SPLIT);
        beforBS.append(QString::number(number));
        beforBS.append(SPLIT);
        beforBS.append(QString::number(money));
        break;
    case UPLOAD:
        beforBS.append(SPLIT);
        beforBS.append(QString::number(number));
        beforBS.append(SPLIT);
        beforBS.append(QString::number(money));
        break;

        /*TO CLI*/
    case ERROR:
        beforBS.append(SPLIT);
        beforBS.append(message);
        break;
    case INFO:
        beforBS.append(SPLIT);
        beforBS.append(QString::number(number));
        beforBS.append(SPLIT);
        beforBS.append(login);
        beforBS.append(SPLIT);
        beforBS.append(QString::number(money));
        break;
    }

    QByteArray afterBS;
    afterBS.append(FLAG);
    afterBS.append(byte_stuffing(beforBS));
    afterBS.append(FLAG);
    return afterBS;
}

QByteArray Packet::byte_stuffing(QByteArray bytes)
{
    QByteArray res;
    for (auto i : bytes)
    {
        if (static_cast<unsigned char>(i) == FLAG || static_cast<unsigned char>(i) == ESC)
        {
            res.append(ESC);
        }
        res.append(i);
    }

    return res;
}

int Packet::getCommand() const
{
    return command;
}

void Packet::setCommand(int value)
{
    command = value;
}

int Packet::getNumber() const
{
    return number;
}

void Packet::setNumber(int value)
{
    number = value;
}

QString Packet::getLogin() const
{
    return login;
}

void Packet::setLogin(const QString &value)
{
    login = value;
}

int Packet::getPassw() const
{
    return passw;
}

void Packet::setPassw(int value)
{
    passw = value;
}

int Packet::getMoney() const
{
    return money;
}

void Packet::setMoney(int value)
{
    money = value;
}

int Packet::getFrom() const
{
    return from;
}

void Packet::setFrom(const int &value)
{
    from = value;
}

int Packet::getTo() const
{
    return to;
}

void Packet::setTo(const int &value)
{
    to = value;
}

QString Packet::getMessage() const
{
    return message;
}

void Packet::setMessage(const QString &value)
{
    message = value;
}

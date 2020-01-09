#include "banksrv.h"

BankSRV::BankSRV(QObject *parent) : QObject(parent)
{
    SocketServer socketSRV;
    db = new DB_helper("bank.db");

    connect(&socketSRV, &SocketServer::new_connection, this, &BankSRV::new_connect_handler);
    socketSRV.start_server();

}

void BankSRV::new_connect_handler(int descriptor)
{
    qDebug() << "New user request!";
    if (fork() == 0) {
        qDebug() << "User " << user_num << " succesfully connected!";
        bool connected = true;
        while (connected) {
            Packet packet;
            packet.packet_parser(wait_paket(descriptor));
            packet_handler(packet, descriptor, connected);
        }
        qDebug() << "User " << user_num << " disconnected!";
        close(descriptor);
        exit(0);
    } else {
        user_num++;
        close(descriptor);
    }
}

QString BankSRV::wait_paket(int descriptor)
{
    bool packet_reception = false;
    bool is_last_esc = false;
    while (1) {
        memset(readBuffer, 0, sizeof (readBuffer));
        read(descriptor, readBuffer, sizeof(readBuffer));
        for (char * i = readBuffer; *i; i++) {
            switch (static_cast<uint8_t>(*i)) {
            case FLAG:
                if (packet_reception) {
                    if (is_last_esc) {
                        packetBuffer += *i;
                        is_last_esc = false;
                    } else {
                        packet_reception = false;
                        qDebug() << "Packet!";
                        return packetBuffer;
                    }
                } else {
                    packetBuffer.clear();
                    packet_reception = true;
                }
                break;
            case ESC:
                if (packet_reception) {
                    if (is_last_esc) {
                        packetBuffer += *i;
                        is_last_esc = false;
                    } else {
                        is_last_esc = true;
                    }
                }
                break;
            default:
                if (packet_reception) {
                    packetBuffer += *i;
                }
            }
        }
    }
}

void BankSRV::packet_handler(Packet &packet, int descriptor, bool &connected)
{
    qDebug() << "Recieved command: " << static_cast<int>(packet.getCommand()) << "from user " << user_num;
    switch (static_cast<int>(packet.getCommand())) {
        case NEW:
        {
            qDebug() << "(" << user_num <<") Creating new...";
            Account acc;
            acc.setLogin(packet.getLogin());
            acc.setPin(packet.getPassw());
            acc.setMoney(packet.getMoney());
            if (db->new_account(acc)) {
                qDebug() << "(" << user_num <<") Success new account!";
                Packet reply;
                reply.setCommand(OK);
                write(descriptor, reply.get_packet(), static_cast<size_t>(reply.get_packet().length()));
            } else {
                QString message = "Error new account!";
                qDebug() << "(" << user_num <<") " << message;
                Packet reply;
                reply.setCommand(ERROR);
                reply.setMessage(message);
                write(descriptor, reply.get_packet(), static_cast<size_t>(reply.get_packet().length()));
            }
            break;
        }
        case SEARCH:
        {
            Account acc = (db->search(packet.getLogin()));
            if (acc.getIsFound() && packet.getPassw() == acc.getPin() && acc.getIsOpen()) {
                Packet reply;
                reply.setCommand(INFO);
                reply.setNumber(acc.getNumber());
                reply.setLogin(acc.getLogin());
                reply.setMoney(acc.getMoney());
                write(descriptor, reply.get_packet(), static_cast<size_t>(reply.get_packet().length()));
            } else {
                QString message = "Error user or password!";
                qDebug() << "(" << user_num <<") " << message;
                Packet reply;
                reply.setCommand(ERROR);
                write(descriptor, reply.get_packet(), static_cast<size_t>(reply.get_packet().length()));
            }
            break;
        }
        case CLOSE:
        {
            Account acc;
            acc.setNumber(packet.getNumber());
            if (db->close_account(acc)) {
                qDebug() << "(" << user_num <<") Success closing account!";
                Packet reply;
                reply.setCommand(OK);
                write(descriptor, reply.get_packet(), static_cast<size_t>(reply.get_packet().length()));
            } else {
                QString message = "Error closing account!";
                qDebug() << "(" << user_num <<") " << message;
                Packet reply;
                reply.setCommand(ERROR);
                reply.setMessage(message);
                write(descriptor, reply.get_packet(), static_cast<size_t>(reply.get_packet().length()));
            }
            break;
        }
        case TRANSFACTION:
        {
            Account from = db->search(packet.getFrom());
            Account to = db->search(packet.getTo());
            if (db->transfaction(from, to, packet.getMoney())) {
                qDebug() << "(" << user_num <<") Success transfaction!";
                Packet reply;
                reply.setCommand(OK);
                write(descriptor, reply.get_packet(), static_cast<size_t>(reply.get_packet().length()));
            } else {
                QString message = "Error transfaction!";
                qDebug() << "(" << user_num <<") " << message;
                Packet reply;
                reply.setCommand(ERROR);
                reply.setMessage(message);
                write(descriptor, reply.get_packet(), static_cast<size_t>(reply.get_packet().length()));
            }
            break;
        }
        case GET:
        {
            Account acc = db->search(packet.getNumber());
            if (db->get_cash(acc, packet.getMoney())) {
                qDebug() << "(" << user_num <<") Successful cash withdrawal!";
                Packet reply;
                reply.setCommand(OK);
                write(descriptor, reply.get_packet(), static_cast<size_t>(reply.get_packet().length()));
            } else {
                QString message = "Cache Issue Error!";
                qDebug() << "(" << user_num <<") " << message;
                Packet reply;
                reply.setCommand(ERROR);
                reply.setMessage(message);
                write(descriptor, reply.get_packet(), static_cast<size_t>(reply.get_packet().length()));
            }
            break;
        }
        case UPLOAD:
        {
            Account acc;
            acc.setNumber(packet.getNumber());
            if (db->upload_cash(acc, packet.getMoney())) {
                qDebug() << "(" << user_num <<") Successful cash replenishment!";
                Packet reply;
                reply.setCommand(OK);
                write(descriptor, reply.get_packet(), static_cast<size_t>(reply.get_packet().length()));
            } else {
                QString message = "Cache Issue Error!";
                qDebug() << "(" << user_num <<") " << message;
                Packet reply;
                reply.setCommand(ERROR);
                reply.setMessage(message);
                write(descriptor, reply.get_packet(), static_cast<size_t>(reply.get_packet().length()));
            }
            break;
        }
        case DISCONNECT:
        {
            connected = false;
            Packet reply;
            reply.setCommand(OK);
            write(descriptor, reply.get_packet(), static_cast<size_t>(reply.get_packet().length()));
            break;
        }
        default:
        {
            QString message = "Unknown command!";
            qDebug() << "(" << user_num <<") " << message;
            Packet reply;
            reply.setCommand(ERROR);
            reply.setMessage(message);
            write(descriptor, reply.get_packet(), static_cast<size_t>(reply.get_packet().length()));
            break;
        }
    }
}




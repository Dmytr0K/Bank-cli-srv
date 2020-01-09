#include "bankcli.h"

bankCLI::bankCLI(QObject *parent) : QObject(parent)
{
    socketCLI = new SocketClient();

    connect(socketCLI, &SocketClient::new_status_message, this, &bankCLI::new_status_message);
}

bool bankCLI::wait_reply()
{
    emit new_status_message("Wait for responding...");
    bool packet_reception = false;
    bool is_last_esc = false;
    QDateTime start = QDateTime::currentDateTime();
    bool timeout = false;
    while (!timeout) {
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
                        emit new_status_message("Respond accepted...");
                        return true;
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
        if (start.secsTo(QDateTime::currentDateTime()) >= 2) {
            timeout = true;
        }
    }
    emit new_status_message("Server timeout!");
    return false;
}

void bankCLI::packet_handler(Packet &packet)
{
    switch (packet.getCommand()) {
    case OK:
        switch (lastCommand) {
        case NEW:
            emit new_status_message("Successful new account!");
            break;
        case CLOSE:
            emit new_status_message("Successful closing account!");
            disconnect_from_server();
            break;
        case TRANSFACTION:
            emit new_status_message("Successful transfaction!");
            authorization(authorized_account.getLogin(), authorized_account.getPin());
            break;
        case GET:
            emit new_status_message("Successful cash withdrawal!");
            authorization(authorized_account.getLogin(), authorized_account.getPin());
            break;
        case UPLOAD:
            emit new_status_message("Successful cash replenishment!");
            authorization(authorized_account.getLogin(), authorized_account.getPin());
            break;
        case DISCONNECT:
            emit new_status_message("Disconnected!");
            break;
        }
        break;
    case ERROR:
        emit new_status_message(packet.getMessage());
        break;
    case INFO:
        Account acc;
        acc.setNumber(packet.getNumber());
        acc.setLogin(packet.getLogin());
        acc.setMoney(packet.getMoney());
//        emit new_status_message("Updated!");
        emit new_info(acc);
        break;
    }

}

bool bankCLI::reply_handler()
{
    Packet reply;
    if (wait_reply()) {
        reply.packet_parser(packetBuffer);
        packet_handler(reply);
        return true;
    } else {
        return false;
    }
}

Account & bankCLI::getAuthorized_account()
{
    return authorized_account;
}

void bankCLI::connect_to_server()
{
    descriptor = socketCLI->connect();
    if (descriptor > 0) {
        emit connected(true);
    }
}

void bankCLI::authorization(QString login, int pin)
{
    Packet packet;
    packet.setCommand(SEARCH);
    lastCommand = SEARCH;
    packet.setLogin(login);
    packet.setPassw(pin);
    write(descriptor, packet.get_packet(), static_cast<size_t>(packet.get_packet().length()));
    reply_handler();
}

void bankCLI::new_account(QString login, int pin, int money)
{
    Packet packet;
    packet.setCommand(0);
    lastCommand = NEW;
    packet.setLogin(login);
    packet.setPassw(pin);
    packet.setMoney(money);
    write(descriptor, packet.get_packet(), static_cast<size_t>(packet.get_packet().length()));
    reply_handler();
}

void bankCLI::transfaction(int from_number, int to_number, int money)
{
    Packet packet;
    packet.setCommand(TRANSFACTION);
    lastCommand = TRANSFACTION;
    packet.setFrom(from_number);
    packet.setTo(to_number);
    packet.setMoney(money);
    write(descriptor, packet.get_packet(), static_cast<size_t>(packet.get_packet().length()));
    reply_handler();
}

void bankCLI::get_money(int number, int money)
{
    Packet packet;
    packet.setCommand(GET);
    lastCommand = GET;
    packet.setNumber(number);
    packet.setMoney(money);
    write(descriptor, packet.get_packet(), static_cast<size_t>(packet.get_packet().length()));
    reply_handler();
}

void bankCLI::upload_money(int number, int money)
{
    Packet packet;
    packet.setCommand(UPLOAD);
    lastCommand = UPLOAD;
    packet.setNumber(number);
    packet.setMoney(money);
    write(descriptor, packet.get_packet(), static_cast<size_t>(packet.get_packet().length()));
    reply_handler();
}

void bankCLI::close_account(int number)
{
    Packet packet;
    packet.setCommand(CLOSE);
    lastCommand = CLOSE;
    packet.setNumber(number);
    write(descriptor, packet.get_packet(), static_cast<size_t>(packet.get_packet().length()));
    reply_handler();
}

void bankCLI::disconnect_from_server()
{
    Packet packet;
    packet.setCommand(DISCONNECT);
    lastCommand = DISCONNECT;
    write(descriptor, packet.get_packet(), static_cast<size_t>(packet.get_packet().length()));
    if (reply_handler()) {
        descriptor = 0;
        close(descriptor);
        emit connected(false);
    }
}

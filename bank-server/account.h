#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QString>

class Account
{
public:
    explicit Account();

    int getNumber() const;
    void setNumber(int value);

    QString getLogin() const;
    void setLogin(const QString &value);

    int getPin() const;
    void setPin(int value);

    int getMoney() const;
    void setMoney(int value);

    bool getIsOpen() const;
    void setIsOpen(bool value);

    bool getIsFound() const;
    void setIsFound(bool value);

private:
    int number;
    QString login;
    int pin;
    int money;
    bool isOpen;

    bool isFound;

};

#endif // ACCOUNT_H

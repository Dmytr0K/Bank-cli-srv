#include "account.h"

Account::Account()
{
    isOpen = true;
    isFound = true;
}

int Account::getNumber() const
{
    return number;
}

void Account::setNumber(int value)
{
    number = value;
}

QString Account::getLogin() const
{
    return login;
}

void Account::setLogin(const QString &value)
{
    login = value;
}

int Account::getPin() const
{
    return pin;
}

void Account::setPin(int value)
{
    pin = value;
}

int Account::getMoney() const
{
    return money;
}

void Account::setMoney(int value)
{
    money = value;
}

bool Account::getIsOpen() const
{
    return isOpen;
}

void Account::setIsOpen(bool value)
{
    isOpen = value;
}

bool Account::getIsFound() const
{
    return isFound;
}

void Account::setIsFound(bool value)
{
    isFound = value;
}

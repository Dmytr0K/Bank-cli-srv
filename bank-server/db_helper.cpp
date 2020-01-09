#include "db_helper.h"

DB_helper::DB_helper(QString db_path)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(db_path);
    db.open();
    if (db.isOpen()) {
        qDebug() << "Success connect to db...";
        query = new QSqlQuery(db);
        QString str_check = "SELECT count(*) FROM sqlite_master WHERE type='table' AND name='Accounts';";
        query->exec(str_check);
        while(query->next()) {
//            qDebug() << query->value(0).toInt();
            if(!query->value(0).toInt()) {
                QString str_create = "CREATE TABLE Accounts ("
                                     "number INTEGER PRIMARY KEY AUTOINCREMENT,"
                                     "login TEXT NOT NULL UNIQUE,"
                                     "pin INTEGER NOT NULL,"
                                     "is_open INTEGER NOT NULL,"
                                     "money INTEGER NOT NULL"
                                     ");";
                if (!query->exec(str_create)) {
                    qDebug() << db.lastError().text();
                }
            }
        }
    } else {
        qDebug() << db.lastError().text();
    }
}

bool DB_helper::new_account(const Account& account)
{
    QString str_insert = "INSERT INTO Accounts(login, pin, is_open, money) "
                         "VALUES ('%1',%2, %3, %4);";
    str_insert = str_insert.arg(account.getLogin())
            .arg(QString::number(account.getPin()))
            .arg(QString::number(1))
            .arg(QString::number(account.getMoney()));
    if (!query->exec(str_insert)) {
        qDebug() << "Error adding record! Maybe already exists!";
        return false;
    }
    return true;
}

Account DB_helper::search(QString login)
{
    Account acc;
    QString str_search = "SELECT * FROM Accounts WHERE login='%1'";
    str_search = str_search.arg(login);
    if (!query->exec(str_search)) {
        qDebug() << "Error selecting from db! Maybe login not exist";
        acc.setIsFound(false);
    }
    QSqlRecord rec = query->record();
    while(query->next()) {
        acc.setNumber(query->value(rec.indexOf("number")).toInt());
        acc.setLogin(query->value(rec.indexOf("login")).toString());
        acc.setPin(query->value(rec.indexOf("pin")).toInt());
        acc.setIsOpen(query->value(rec.indexOf("is_open")).toInt());
        acc.setMoney(query->value(rec.indexOf("money")).toInt());
    }
    return acc;
}

Account DB_helper::search(int number)
{
    Account acc;
    QString str_search = "SELECT * FROM Accounts WHERE number=%1";
    str_search = str_search.arg(number);
    if (!query->exec(str_search)) {
        qDebug() << "Error selecting from db! Maybe login not exist";
        acc.setIsFound(false);
    }
    QSqlRecord rec = query->record();
    while(query->next()) {
        acc.setNumber(query->value(rec.indexOf("number")).toInt());
        acc.setLogin(query->value(rec.indexOf("login")).toString());
        acc.setPin(query->value(rec.indexOf("pin")).toInt());
        acc.setIsOpen(query->value(rec.indexOf("is_open")).toInt());
        acc.setMoney(query->value(rec.indexOf("money")).toInt());
    }
    return acc;
}

bool DB_helper::close_account(const Account& account)
{
    QString str_close = "UPDATE Accounts SET is_open=0 WHERE number = %1";
    str_close = str_close.arg(QString::number(account.getNumber()));
    if (!query->exec(str_close)) {
        qDebug() << "Error closing account";
        return false;
    }
    return true;
}

bool DB_helper::transfaction(const Account& from, const Account& to, int money)
{
    if (from.getMoney() >= money && to.getIsOpen()) {
        QString str_set_from = "UPDATE Accounts SET money=money-%1 WHERE number = %2";
        str_set_from = str_set_from.arg(money)
                .arg(QString::number(from.getNumber()));
        QString str_set_to = "UPDATE Accounts SET money=money+%1 WHERE number = %2";
        str_set_to = str_set_to.arg(money)
                .arg(QString::number(to.getNumber()));
        if (!query->exec(str_set_from) || !query->exec(str_set_to)) {
            qDebug() << "Error transfaction!";
            return false;
        }
    } else {
        qDebug() << "Error transfaction!";
        return false;
    }
    return true;
}

bool DB_helper::get_cash(const Account &account, int money)
{
    if (account.getMoney() >= money && account.getIsOpen()) {
        QString str_get_cash = "UPDATE Accounts SET money=money-%1 WHERE number = %2";
        str_get_cash = str_get_cash.arg(money)
                .arg(QString::number(account.getNumber()));
        if (!query->exec(str_get_cash)) {
            qDebug() << "Error give cash occurred!";
            return false;
        }
    } else {
        qDebug() << "Not enougth money or account is closed";
        return false;
    }
    return true;
}

bool DB_helper::upload_cash(const Account &account, int money)
{
    if (account.getIsOpen()) {
        QString str_upload_cash = "UPDATE Accounts SET money=money+%1 WHERE number = %2";
        str_upload_cash = str_upload_cash.arg(money)
                .arg(QString::number(account.getNumber()));
        if (!query->exec(str_upload_cash)) {
            qDebug() << "Error upload cash occurred!";
            return false;
        }
    } else {
        qDebug() << "Error! Account is closed!";
        return false;
    }
    return true;
}

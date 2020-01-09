#ifndef DB_HELPER_H
#define DB_HELPER_H

#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QSqlQuery>

#include <account.h>

class DB_helper
{
public:
    explicit DB_helper(QString db_path);

    bool new_account(const Account& account);
    Account search (QString login);
    Account search(int number);
    bool close_account(const Account& account);
    bool transfaction(const Account& from, const Account&to, int money);
    bool get_cash (const Account& account, int money);
    bool upload_cash(const Account& account, int money);

private:
    QSqlDatabase db;
    QSqlQuery *query;

};

#endif // DB_HELPER_H

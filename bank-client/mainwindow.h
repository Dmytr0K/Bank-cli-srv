#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "socketclient.h"
#include "bankcli.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void print_status(QString message);
    void set_enable_after_connection(bool status);
    void set_enable_after_auth(bool status);

private:
    Ui::MainWindow *ui;

    bankCLI *client;
    bool authorization = false;
};
#endif // MAINWINDOW_H

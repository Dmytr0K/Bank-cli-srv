#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    client = new bankCLI();

    connect(client, &bankCLI::new_status_message, this, &MainWindow::print_status);

    connect(client, &bankCLI::connected, this, &MainWindow::set_enable_after_connection);

    connect(ui->button_connect, &QPushButton::clicked, client, &bankCLI::connect_to_server);

    connect(ui->button_disconnect, &QPushButton::clicked, [this] () -> void {
                client->disconnect_from_server();
                ui->label_number_info->clear();
                ui->label_login_info->clear();
                ui->label_money_info->clear();
                ui->frame_info->setEnabled(false);
                ui->frame_opt->setEnabled(false);
            });

    connect(ui->button_new, &QPushButton::clicked, [this] () -> void {
                client->new_account(ui->line_login_new->text(), ui->line_pin_new->text().toInt(), ui->line_money_new->text().toInt());
                ui->line_login_new->clear();
                ui->line_pin_new->clear();
                ui->line_money_new->clear();
            });

    connect(ui->button_auth, &QPushButton::clicked, [this] () -> void {
                authorization = true;
                client->getAuthorized_account().setLogin(ui->line_login_in->text());
                client->getAuthorized_account().setPin(ui->line_pin_in->text().toInt());
                client->authorization(client->getAuthorized_account().getLogin(), client->getAuthorized_account().getPin());
            });

    connect(client, &bankCLI::new_info, [this](Account acc) {
        if (authorization) {
            ui->statusBar->showMessage("Successful authorization!");
            authorization = false;
        }
        client->getAuthorized_account().setNumber(acc.getNumber());
        client->getAuthorized_account().setLogin(acc.getLogin());
        client->getAuthorized_account().setMoney(acc.getMoney());

        ui->label_number_info->setText(QString::number(acc.getNumber()));
        ui->label_login_info->setText(acc.getLogin());
        ui->label_money_info->setText(QString::number(acc.getMoney()));

        ui->line_login_in->clear();
        ui->line_pin_in->clear();

        ui->frame_auth->setEnabled(false);
        ui->frame_info->setEnabled(true);
        ui->frame_opt->setEnabled(true);
        ui->frame_new->setEnabled(false);
    });

    connect(ui->button_transf, &QPushButton::clicked, [this] () -> void {
        client->transfaction(client->getAuthorized_account().getNumber(), ui->line_transf_number->text().toInt(), ui->line_transf_money->text().toInt());
    });

    connect(ui->button_get, &QPushButton::clicked, [this] () -> void {
        client->get_money(client->getAuthorized_account().getNumber(), ui->line_get_money->text().toInt());
    });

    connect(ui->button_give, &QPushButton::clicked, [this] () -> void {
        client->upload_money(client->getAuthorized_account().getNumber(), ui->line_give_money->text().toInt());
    });

    connect(ui->button_close, &QPushButton::clicked, [this] () -> void {
        client->close_account(client->getAuthorized_account().getNumber());
        ui->label_number_info->clear();
        ui->label_login_info->clear();
        ui->label_money_info->clear();
        ui->frame_info->setEnabled(false);
        ui->frame_opt->setEnabled(false);
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::print_status(QString message)
{
    ui->statusBar->showMessage(message);
}

void MainWindow::set_enable_after_connection(bool status)
{
    ui->frame_auth->setEnabled(status);
    ui->frame_new->setEnabled(status);
    ui->button_disconnect->setEnabled(status);
    ui->button_connect->setEnabled(!status);
}

void MainWindow::set_enable_after_auth(bool status)
{

}


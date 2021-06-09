#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "server.h"
#include "ui_server.h"
#include "client.h"
#include "ui_client.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_goToServer_clicked()
{
    this->hide();
    server* s=new server;
    s->show();
}

void MainWindow::on_goToClient_clicked()
{
    this->hide();
    client* c=new client;
    c->show();
}

#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDateTime>
#include <QMessageBox>
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>

namespace Ui {
class client;
}

class client : public QMainWindow
{
    Q_OBJECT

public:
    explicit client(QWidget *parent = nullptr);
    ~client();

private slots:
    void on_connectButton_clicked();

    void on_sendMessage_clicked();

private:
    Ui::client *ui;
    SOCKET sockClient;
};

void ui_init(Ui::client *ui_ptr);
void* recv2(SOCKET sockConn);
void* send2(void* args);

#endif // CLIENT_H

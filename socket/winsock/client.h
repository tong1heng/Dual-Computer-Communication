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
#include <QDebug>


namespace Ui {
class client;
}

extern Ui::client *uic;

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
    static void* ctrlRecv(void* args);
};

void ui_init(Ui::client *ui_ptr);
#endif // CLIENT_H

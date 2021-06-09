#ifndef SERVER_H
#define SERVER_H

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
class server;
}

extern Ui::server *uis;

class server : public QMainWindow
{
    Q_OBJECT

public:
    explicit server(QWidget *parent = nullptr);
    ~server();

private slots:
    void on_start_clicked();

    void on_file_clicked();

    void on_sendMessage_clicked();

private:
    Ui::server *ui;
    SOCKET sockConn;
    static void* ctrlRecvS(void *args);
};
void i_init(Ui::server *ui_ptr);

#endif // SERVER_H

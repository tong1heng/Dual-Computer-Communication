#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDateTime>
#include <QMessageBox>
#include <WinSock2.h>
#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <iostream>
#include <QDebug>
#include <QTextCodec>


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

public slots:
    void getFilePath();

private slots:
    void on_start_clicked();

    void on_sendMessageButton_clicked();

    void on_findFileButton_clicked();

    void on_sendFileButton_clicked();

    void on_exitButton_clicked();

    void on_clearTextButton_clicked();

    void on_clearMessageButton_clicked();

    void on_goToVoiceChatButton_clicked();

    void addEndChatLog();

private:
    Ui::server *ui;
    WSADATA wsaMsg,wsaFile;
    SOCKET sockSrvMsg;
    SOCKET sockSrvFile;
    SOCKET sockConnMsg;
    QString clientIP;

    static void* ctrlRecvMsg(void *args);
    static void* ctrlRecvFile(void* args);
    static server* MyPointer;

signals:
    void createQFileDialog();
    void sendAddress(QString addr);
};

void ui_init(Ui::server *ui_ptr);   //初始化指针uis

#endif // SERVER_H

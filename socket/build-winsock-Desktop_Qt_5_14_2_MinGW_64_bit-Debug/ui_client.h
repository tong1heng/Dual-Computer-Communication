/********************************************************************************
** Form generated from reading UI file 'client.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENT_H
#define UI_CLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_client
{
public:
    QWidget *centralwidget;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents_3;
    QTextEdit *clientTextEdit;
    QLineEdit *port;
    QLabel *label;
    QPushButton *connectButton;
    QPushButton *sendFile;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *exit;
    QPushButton *file;
    QScrollArea *scrollArea_3;
    QWidget *scrollAreaWidgetContents_8;
    QTextEdit *clientFilePath;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_9;
    QLineEdit *clientIPAddress;
    QPushButton *sendMessage;
    QTextEdit *message;
    QLineEdit *serverIPAddress;
    QLabel *label_4;
    QPushButton *disconnectButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *client)
    {
        if (client->objectName().isEmpty())
            client->setObjectName(QString::fromUtf8("client"));
        client->resize(800, 600);
        centralwidget = new QWidget(client);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        scrollArea = new QScrollArea(centralwidget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setGeometry(QRect(110, 90, 570, 281));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents_3 = new QWidget();
        scrollAreaWidgetContents_3->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_3"));
        scrollAreaWidgetContents_3->setGeometry(QRect(0, 0, 568, 279));
        clientTextEdit = new QTextEdit(scrollAreaWidgetContents_3);
        clientTextEdit->setObjectName(QString::fromUtf8("clientTextEdit"));
        clientTextEdit->setEnabled(true);
        clientTextEdit->setGeometry(QRect(0, 0, 570, 281));
        QFont font;
        font.setPointSize(12);
        clientTextEdit->setFont(font);
        clientTextEdit->setReadOnly(true);
        scrollArea->setWidget(scrollAreaWidgetContents_3);
        port = new QLineEdit(centralwidget);
        port->setObjectName(QString::fromUtf8("port"));
        port->setGeometry(QRect(520, 50, 111, 21));
        port->setReadOnly(false);
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(360, 10, 78, 25));
        QFont font1;
        font1.setPointSize(15);
        label->setFont(font1);
        connectButton = new QPushButton(centralwidget);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));
        connectButton->setGeometry(QRect(660, 30, 93, 28));
        QFont font2;
        font2.setPointSize(11);
        connectButton->setFont(font2);
        sendFile = new QPushButton(centralwidget);
        sendFile->setObjectName(QString::fromUtf8("sendFile"));
        sendFile->setGeometry(QRect(660, 470, 61, 28));
        sendFile->setFont(font2);
        sendFile->setCheckable(false);
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(110, 40, 70, 20));
        label_2->setFont(font);
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(420, 50, 70, 20));
        label_3->setFont(font);
        exit = new QPushButton(centralwidget);
        exit->setObjectName(QString::fromUtf8("exit"));
        exit->setGeometry(QRect(110, 500, 93, 28));
        exit->setFont(font2);
        file = new QPushButton(centralwidget);
        file->setObjectName(QString::fromUtf8("file"));
        file->setGeometry(QRect(660, 440, 61, 28));
        file->setFont(font2);
        scrollArea_3 = new QScrollArea(centralwidget);
        scrollArea_3->setObjectName(QString::fromUtf8("scrollArea_3"));
        scrollArea_3->setGeometry(QRect(110, 440, 541, 51));
        scrollArea_3->setWidgetResizable(true);
        scrollAreaWidgetContents_8 = new QWidget();
        scrollAreaWidgetContents_8->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_8"));
        scrollAreaWidgetContents_8->setGeometry(QRect(0, 0, 539, 49));
        clientFilePath = new QTextEdit(scrollAreaWidgetContents_8);
        clientFilePath->setObjectName(QString::fromUtf8("clientFilePath"));
        clientFilePath->setGeometry(QRect(0, 0, 541, 50));
        scrollArea_3->setWidget(scrollAreaWidgetContents_8);
        scrollArea_2 = new QScrollArea(centralwidget);
        scrollArea_2->setObjectName(QString::fromUtf8("scrollArea_2"));
        scrollArea_2->setGeometry(QRect(110, 380, 541, 51));
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_9 = new QWidget();
        scrollAreaWidgetContents_9->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_9"));
        scrollAreaWidgetContents_9->setGeometry(QRect(0, 0, 539, 49));
        scrollArea_2->setWidget(scrollAreaWidgetContents_9);
        clientIPAddress = new QLineEdit(centralwidget);
        clientIPAddress->setObjectName(QString::fromUtf8("clientIPAddress"));
        clientIPAddress->setGeometry(QRect(200, 40, 111, 21));
        clientIPAddress->setReadOnly(true);
        sendMessage = new QPushButton(centralwidget);
        sendMessage->setObjectName(QString::fromUtf8("sendMessage"));
        sendMessage->setGeometry(QRect(660, 390, 61, 28));
        sendMessage->setFont(font2);
        sendMessage->setCheckable(false);
        message = new QTextEdit(centralwidget);
        message->setObjectName(QString::fromUtf8("message"));
        message->setGeometry(QRect(110, 380, 541, 50));
        serverIPAddress = new QLineEdit(centralwidget);
        serverIPAddress->setObjectName(QString::fromUtf8("serverIPAddress"));
        serverIPAddress->setGeometry(QRect(200, 70, 111, 21));
        serverIPAddress->setReadOnly(false);
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(110, 70, 90, 20));
        label_4->setFont(font);
        disconnectButton = new QPushButton(centralwidget);
        disconnectButton->setObjectName(QString::fromUtf8("disconnectButton"));
        disconnectButton->setGeometry(QRect(660, 60, 93, 28));
        disconnectButton->setFont(font2);
        client->setCentralWidget(centralwidget);
        menubar = new QMenuBar(client);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 26));
        client->setMenuBar(menubar);
        statusbar = new QStatusBar(client);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        client->setStatusBar(statusbar);

        retranslateUi(client);

        QMetaObject::connectSlotsByName(client);
    } // setupUi

    void retranslateUi(QMainWindow *client)
    {
        client->setWindowTitle(QCoreApplication::translate("client", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("client", "CLIENT", nullptr));
        connectButton->setText(QCoreApplication::translate("client", "\350\277\236\346\216\245", nullptr));
        sendFile->setText(QCoreApplication::translate("client", "\345\217\221\351\200\201", nullptr));
        label_2->setText(QCoreApplication::translate("client", "\346\234\254\346\234\272IP:", nullptr));
        label_3->setText(QCoreApplication::translate("client", "\347\253\257\345\217\243\345\217\267:", nullptr));
        exit->setText(QCoreApplication::translate("client", "\351\200\200\345\207\272", nullptr));
        file->setText(QCoreApplication::translate("client", "\346\265\217\350\247\210", nullptr));
        sendMessage->setText(QCoreApplication::translate("client", "\345\217\221\351\200\201", nullptr));
        serverIPAddress->setText(QString());
        label_4->setText(QCoreApplication::translate("client", "\346\234\215\345\212\241\345\231\250IP:", nullptr));
        disconnectButton->setText(QCoreApplication::translate("client", "\346\226\255\345\274\200", nullptr));
    } // retranslateUi

};

namespace Ui {
    class client: public Ui_client {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENT_H

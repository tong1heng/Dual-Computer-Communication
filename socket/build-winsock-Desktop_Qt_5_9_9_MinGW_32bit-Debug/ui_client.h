/********************************************************************************
** Form generated from reading UI file 'client.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENT_H
#define UI_CLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
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
    QTextEdit *textEdit;
    QLineEdit *port;
    QLabel *label;
    QPushButton *connectButton;
    QPushButton *sendFileButton;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *exitButton;
    QPushButton *findFileButton;
    QScrollArea *scrollArea_3;
    QWidget *scrollAreaWidgetContents_8;
    QTextEdit *filePath;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_9;
    QLineEdit *clientIPAddress;
    QPushButton *sendMessageButton;
    QTextEdit *message;
    QLineEdit *serverIPAddress;
    QLabel *label_4;
    QPushButton *disconnectButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *client)
    {
        if (client->objectName().isEmpty())
            client->setObjectName(QStringLiteral("client"));
        client->resize(800, 600);
        centralwidget = new QWidget(client);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        scrollArea = new QScrollArea(centralwidget);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setGeometry(QRect(110, 90, 570, 281));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents_3 = new QWidget();
        scrollAreaWidgetContents_3->setObjectName(QStringLiteral("scrollAreaWidgetContents_3"));
        scrollAreaWidgetContents_3->setGeometry(QRect(0, 0, 568, 279));
        textEdit = new QTextEdit(scrollAreaWidgetContents_3);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setEnabled(true);
        textEdit->setGeometry(QRect(0, 0, 570, 281));
        QFont font;
        font.setPointSize(11);
        textEdit->setFont(font);
        textEdit->setReadOnly(true);
        scrollArea->setWidget(scrollAreaWidgetContents_3);
        port = new QLineEdit(centralwidget);
        port->setObjectName(QStringLiteral("port"));
        port->setGeometry(QRect(520, 50, 111, 21));
        port->setFont(font);
        port->setReadOnly(false);
        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(360, 10, 78, 25));
        QFont font1;
        font1.setPointSize(15);
        label->setFont(font1);
        connectButton = new QPushButton(centralwidget);
        connectButton->setObjectName(QStringLiteral("connectButton"));
        connectButton->setGeometry(QRect(660, 30, 93, 28));
        connectButton->setFont(font);
        sendFileButton = new QPushButton(centralwidget);
        sendFileButton->setObjectName(QStringLiteral("sendFileButton"));
        sendFileButton->setGeometry(QRect(660, 470, 61, 28));
        sendFileButton->setFont(font);
        sendFileButton->setCheckable(false);
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(110, 40, 70, 20));
        QFont font2;
        font2.setPointSize(12);
        label_2->setFont(font2);
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(450, 50, 50, 20));
        label_3->setFont(font2);
        exitButton = new QPushButton(centralwidget);
        exitButton->setObjectName(QStringLiteral("exitButton"));
        exitButton->setGeometry(QRect(110, 500, 93, 28));
        exitButton->setFont(font);
        findFileButton = new QPushButton(centralwidget);
        findFileButton->setObjectName(QStringLiteral("findFileButton"));
        findFileButton->setGeometry(QRect(660, 440, 61, 28));
        findFileButton->setFont(font);
        scrollArea_3 = new QScrollArea(centralwidget);
        scrollArea_3->setObjectName(QStringLiteral("scrollArea_3"));
        scrollArea_3->setGeometry(QRect(110, 440, 541, 51));
        scrollArea_3->setWidgetResizable(true);
        scrollAreaWidgetContents_8 = new QWidget();
        scrollAreaWidgetContents_8->setObjectName(QStringLiteral("scrollAreaWidgetContents_8"));
        scrollAreaWidgetContents_8->setGeometry(QRect(0, 0, 539, 49));
        filePath = new QTextEdit(scrollAreaWidgetContents_8);
        filePath->setObjectName(QStringLiteral("filePath"));
        filePath->setGeometry(QRect(0, 0, 541, 50));
        filePath->setFont(font);
        scrollArea_3->setWidget(scrollAreaWidgetContents_8);
        scrollArea_2 = new QScrollArea(centralwidget);
        scrollArea_2->setObjectName(QStringLiteral("scrollArea_2"));
        scrollArea_2->setGeometry(QRect(110, 380, 541, 51));
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_9 = new QWidget();
        scrollAreaWidgetContents_9->setObjectName(QStringLiteral("scrollAreaWidgetContents_9"));
        scrollAreaWidgetContents_9->setGeometry(QRect(0, 0, 539, 49));
        scrollArea_2->setWidget(scrollAreaWidgetContents_9);
        clientIPAddress = new QLineEdit(centralwidget);
        clientIPAddress->setObjectName(QStringLiteral("clientIPAddress"));
        clientIPAddress->setGeometry(QRect(200, 40, 111, 21));
        clientIPAddress->setFont(font);
        clientIPAddress->setReadOnly(true);
        sendMessageButton = new QPushButton(centralwidget);
        sendMessageButton->setObjectName(QStringLiteral("sendMessageButton"));
        sendMessageButton->setGeometry(QRect(660, 390, 61, 28));
        sendMessageButton->setFont(font);
        sendMessageButton->setCheckable(false);
        message = new QTextEdit(centralwidget);
        message->setObjectName(QStringLiteral("message"));
        message->setGeometry(QRect(110, 380, 541, 50));
        message->setFont(font);
        serverIPAddress = new QLineEdit(centralwidget);
        serverIPAddress->setObjectName(QStringLiteral("serverIPAddress"));
        serverIPAddress->setGeometry(QRect(200, 70, 111, 21));
        serverIPAddress->setFont(font);
        serverIPAddress->setReadOnly(false);
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(110, 70, 90, 20));
        label_4->setFont(font2);
        disconnectButton = new QPushButton(centralwidget);
        disconnectButton->setObjectName(QStringLiteral("disconnectButton"));
        disconnectButton->setGeometry(QRect(660, 60, 93, 28));
        disconnectButton->setFont(font);
        client->setCentralWidget(centralwidget);
        menubar = new QMenuBar(client);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 26));
        client->setMenuBar(menubar);
        statusbar = new QStatusBar(client);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        client->setStatusBar(statusbar);

        retranslateUi(client);

        QMetaObject::connectSlotsByName(client);
    } // setupUi

    void retranslateUi(QMainWindow *client)
    {
        client->setWindowTitle(QApplication::translate("client", "MainWindow", Q_NULLPTR));
        label->setText(QApplication::translate("client", "CLIENT", Q_NULLPTR));
        connectButton->setText(QApplication::translate("client", "\350\277\236\346\216\245", Q_NULLPTR));
        sendFileButton->setText(QApplication::translate("client", "\345\217\221\351\200\201", Q_NULLPTR));
        label_2->setText(QApplication::translate("client", "\346\234\254\346\234\272IP:", Q_NULLPTR));
        label_3->setText(QApplication::translate("client", "\347\253\257\345\217\243:", Q_NULLPTR));
        exitButton->setText(QApplication::translate("client", "\351\200\200\345\207\272", Q_NULLPTR));
        findFileButton->setText(QApplication::translate("client", "\346\265\217\350\247\210", Q_NULLPTR));
        sendMessageButton->setText(QApplication::translate("client", "\345\217\221\351\200\201", Q_NULLPTR));
        serverIPAddress->setText(QString());
        label_4->setText(QApplication::translate("client", "\346\234\215\345\212\241\345\231\250IP:", Q_NULLPTR));
        disconnectButton->setText(QApplication::translate("client", "\346\226\255\345\274\200", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class client: public Ui_client {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENT_H

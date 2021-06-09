/********************************************************************************
** Form generated from reading UI file 'server.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVER_H
#define UI_SERVER_H

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

class Ui_server
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QPushButton *sendMessage;
    QLabel *label_2;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QTextEdit *textEdit;
    QLineEdit *IPAddress;
    QLabel *label_3;
    QLineEdit *port;
    QPushButton *exit;
    QPushButton *file;
    QPushButton *start;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_4;
    QTextEdit *message;
    QScrollArea *scrollArea_3;
    QWidget *scrollAreaWidgetContents_5;
    QTextEdit *filepath;
    QPushButton *sendFile;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *server)
    {
        if (server->objectName().isEmpty())
            server->setObjectName(QString::fromUtf8("server"));
        server->resize(800, 600);
        centralwidget = new QWidget(server);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(360, 10, 78, 25));
        QFont font;
        font.setPointSize(15);
        label->setFont(font);
        sendMessage = new QPushButton(centralwidget);
        sendMessage->setObjectName(QString::fromUtf8("sendMessage"));
        sendMessage->setGeometry(QRect(660, 390, 61, 28));
        QFont font1;
        font1.setPointSize(11);
        sendMessage->setFont(font1);
        sendMessage->setCheckable(false);
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(110, 50, 70, 20));
        QFont font2;
        font2.setPointSize(12);
        label_2->setFont(font2);
        scrollArea = new QScrollArea(centralwidget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setGeometry(QRect(110, 90, 570, 281));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 568, 279));
        textEdit = new QTextEdit(scrollAreaWidgetContents);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setEnabled(true);
        textEdit->setGeometry(QRect(0, 0, 570, 281));
        textEdit->setFont(font2);
        textEdit->setReadOnly(true);
        scrollArea->setWidget(scrollAreaWidgetContents);
        IPAddress = new QLineEdit(centralwidget);
        IPAddress->setObjectName(QString::fromUtf8("IPAddress"));
        IPAddress->setGeometry(QRect(200, 50, 111, 21));
        IPAddress->setReadOnly(true);
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(420, 50, 70, 20));
        label_3->setFont(font2);
        port = new QLineEdit(centralwidget);
        port->setObjectName(QString::fromUtf8("port"));
        port->setGeometry(QRect(520, 50, 111, 21));
        port->setReadOnly(false);
        exit = new QPushButton(centralwidget);
        exit->setObjectName(QString::fromUtf8("exit"));
        exit->setGeometry(QRect(110, 500, 93, 28));
        exit->setFont(font1);
        file = new QPushButton(centralwidget);
        file->setObjectName(QString::fromUtf8("file"));
        file->setGeometry(QRect(660, 440, 61, 28));
        file->setFont(font1);
        start = new QPushButton(centralwidget);
        start->setObjectName(QString::fromUtf8("start"));
        start->setGeometry(QRect(660, 50, 93, 28));
        start->setFont(font1);
        scrollArea_2 = new QScrollArea(centralwidget);
        scrollArea_2->setObjectName(QString::fromUtf8("scrollArea_2"));
        scrollArea_2->setGeometry(QRect(110, 380, 541, 51));
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_4 = new QWidget();
        scrollAreaWidgetContents_4->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_4"));
        scrollAreaWidgetContents_4->setGeometry(QRect(0, 0, 539, 49));
        scrollArea_2->setWidget(scrollAreaWidgetContents_4);
        message = new QTextEdit(centralwidget);
        message->setObjectName(QString::fromUtf8("message"));
        message->setGeometry(QRect(110, 380, 541, 50));
        scrollArea_3 = new QScrollArea(centralwidget);
        scrollArea_3->setObjectName(QString::fromUtf8("scrollArea_3"));
        scrollArea_3->setGeometry(QRect(110, 440, 541, 51));
        scrollArea_3->setWidgetResizable(true);
        scrollAreaWidgetContents_5 = new QWidget();
        scrollAreaWidgetContents_5->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_5"));
        scrollAreaWidgetContents_5->setGeometry(QRect(0, 0, 539, 49));
        filepath = new QTextEdit(scrollAreaWidgetContents_5);
        filepath->setObjectName(QString::fromUtf8("filepath"));
        filepath->setGeometry(QRect(0, 0, 541, 50));
        scrollArea_3->setWidget(scrollAreaWidgetContents_5);
        sendFile = new QPushButton(centralwidget);
        sendFile->setObjectName(QString::fromUtf8("sendFile"));
        sendFile->setGeometry(QRect(660, 470, 61, 28));
        sendFile->setFont(font1);
        sendFile->setCheckable(false);
        server->setCentralWidget(centralwidget);
        menubar = new QMenuBar(server);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 26));
        server->setMenuBar(menubar);
        statusbar = new QStatusBar(server);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        server->setStatusBar(statusbar);

        retranslateUi(server);

        QMetaObject::connectSlotsByName(server);
    } // setupUi

    void retranslateUi(QMainWindow *server)
    {
        server->setWindowTitle(QCoreApplication::translate("server", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("server", "SERVER", nullptr));
        sendMessage->setText(QCoreApplication::translate("server", "\345\217\221\351\200\201", nullptr));
        label_2->setText(QCoreApplication::translate("server", "\346\234\254\346\234\272IP:", nullptr));
        label_3->setText(QCoreApplication::translate("server", "\347\253\257\345\217\243\345\217\267:", nullptr));
        exit->setText(QCoreApplication::translate("server", "\351\200\200\345\207\272", nullptr));
        file->setText(QCoreApplication::translate("server", "\346\265\217\350\247\210", nullptr));
        start->setText(QCoreApplication::translate("server", "\345\220\257\345\212\250", nullptr));
        sendFile->setText(QCoreApplication::translate("server", "\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class server: public Ui_server {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVER_H

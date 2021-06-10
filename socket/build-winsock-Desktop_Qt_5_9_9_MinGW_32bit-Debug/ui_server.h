/********************************************************************************
** Form generated from reading UI file 'server.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVER_H
#define UI_SERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
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

class Ui_server
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QPushButton *sendMessageButton;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QTextEdit *textEdit;
    QPushButton *exitButton;
    QPushButton *findFileButton;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_4;
    QTextEdit *message;
    QScrollArea *scrollArea_3;
    QWidget *scrollAreaWidgetContents_5;
    QTextEdit *filePath;
    QPushButton *sendFileButton;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLineEdit *IPAddress;
    QLineEdit *port;
    QLabel *label_2;
    QPushButton *start;
    QLabel *label_3;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *server)
    {
        if (server->objectName().isEmpty())
            server->setObjectName(QStringLiteral("server"));
        server->resize(800, 600);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(server->sizePolicy().hasHeightForWidth());
        server->setSizePolicy(sizePolicy);
        centralwidget = new QWidget(server);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(360, 10, 78, 25));
        QFont font;
        font.setPointSize(15);
        label->setFont(font);
        sendMessageButton = new QPushButton(centralwidget);
        sendMessageButton->setObjectName(QStringLiteral("sendMessageButton"));
        sendMessageButton->setGeometry(QRect(660, 390, 61, 28));
        QFont font1;
        font1.setPointSize(11);
        sendMessageButton->setFont(font1);
        sendMessageButton->setCheckable(false);
        scrollArea = new QScrollArea(centralwidget);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setGeometry(QRect(110, 90, 570, 281));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 568, 279));
        textEdit = new QTextEdit(scrollAreaWidgetContents);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setEnabled(true);
        textEdit->setGeometry(QRect(-1, 0, 571, 281));
        textEdit->setFont(font1);
        textEdit->setReadOnly(true);
        scrollArea->setWidget(scrollAreaWidgetContents);
        exitButton = new QPushButton(centralwidget);
        exitButton->setObjectName(QStringLiteral("exitButton"));
        exitButton->setGeometry(QRect(110, 500, 93, 28));
        exitButton->setFont(font1);
        findFileButton = new QPushButton(centralwidget);
        findFileButton->setObjectName(QStringLiteral("findFileButton"));
        findFileButton->setGeometry(QRect(660, 440, 61, 28));
        findFileButton->setFont(font1);
        scrollArea_2 = new QScrollArea(centralwidget);
        scrollArea_2->setObjectName(QStringLiteral("scrollArea_2"));
        scrollArea_2->setGeometry(QRect(110, 380, 541, 51));
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_4 = new QWidget();
        scrollAreaWidgetContents_4->setObjectName(QStringLiteral("scrollAreaWidgetContents_4"));
        scrollAreaWidgetContents_4->setGeometry(QRect(0, 0, 539, 49));
        scrollArea_2->setWidget(scrollAreaWidgetContents_4);
        message = new QTextEdit(centralwidget);
        message->setObjectName(QStringLiteral("message"));
        message->setGeometry(QRect(110, 380, 541, 50));
        message->setFont(font1);
        scrollArea_3 = new QScrollArea(centralwidget);
        scrollArea_3->setObjectName(QStringLiteral("scrollArea_3"));
        scrollArea_3->setGeometry(QRect(110, 440, 541, 51));
        scrollArea_3->setWidgetResizable(true);
        scrollAreaWidgetContents_5 = new QWidget();
        scrollAreaWidgetContents_5->setObjectName(QStringLiteral("scrollAreaWidgetContents_5"));
        scrollAreaWidgetContents_5->setGeometry(QRect(0, 0, 539, 49));
        filePath = new QTextEdit(scrollAreaWidgetContents_5);
        filePath->setObjectName(QStringLiteral("filePath"));
        filePath->setGeometry(QRect(0, -1, 541, 51));
        filePath->setFont(font1);
        scrollArea_3->setWidget(scrollAreaWidgetContents_5);
        sendFileButton = new QPushButton(centralwidget);
        sendFileButton->setObjectName(QStringLiteral("sendFileButton"));
        sendFileButton->setGeometry(QRect(660, 470, 61, 28));
        sendFileButton->setFont(font1);
        sendFileButton->setCheckable(false);
        gridLayoutWidget = new QWidget(centralwidget);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(90, 30, 641, 57));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        IPAddress = new QLineEdit(gridLayoutWidget);
        IPAddress->setObjectName(QStringLiteral("IPAddress"));
        IPAddress->setFont(font1);
        IPAddress->setReadOnly(true);

        gridLayout->addWidget(IPAddress, 2, 1, 1, 1);

        port = new QLineEdit(gridLayoutWidget);
        port->setObjectName(QStringLiteral("port"));
        port->setFont(font1);
        port->setReadOnly(false);

        gridLayout->addWidget(port, 2, 3, 1, 1);

        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        QFont font2;
        font2.setPointSize(12);
        label_2->setFont(font2);

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        start = new QPushButton(gridLayoutWidget);
        start->setObjectName(QStringLiteral("start"));
        start->setFont(font1);

        gridLayout->addWidget(start, 2, 4, 1, 1);

        label_3 = new QLabel(gridLayoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font2);

        gridLayout->addWidget(label_3, 2, 2, 1, 1);

        server->setCentralWidget(centralwidget);
        menubar = new QMenuBar(server);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 26));
        server->setMenuBar(menubar);
        statusbar = new QStatusBar(server);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        server->setStatusBar(statusbar);

        retranslateUi(server);

        QMetaObject::connectSlotsByName(server);
    } // setupUi

    void retranslateUi(QMainWindow *server)
    {
        server->setWindowTitle(QApplication::translate("server", "MainWindow", Q_NULLPTR));
        label->setText(QApplication::translate("server", "SERVER", Q_NULLPTR));
        sendMessageButton->setText(QApplication::translate("server", "\345\217\221\351\200\201", Q_NULLPTR));
        exitButton->setText(QApplication::translate("server", "\351\200\200\345\207\272", Q_NULLPTR));
        findFileButton->setText(QApplication::translate("server", "\346\265\217\350\247\210", Q_NULLPTR));
        sendFileButton->setText(QApplication::translate("server", "\345\217\221\351\200\201", Q_NULLPTR));
        label_2->setText(QApplication::translate("server", "\346\234\254\346\234\272IP:", Q_NULLPTR));
        start->setText(QApplication::translate("server", "\345\220\257\345\212\250", Q_NULLPTR));
        label_3->setText(QApplication::translate("server", "\347\253\257\345\217\243:", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class server: public Ui_server {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVER_H

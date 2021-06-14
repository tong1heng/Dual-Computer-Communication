#ifndef CLIENTAUDIO_H
#define CLIENTAUDIO_H

#include <QMainWindow>
#include <QMainWindow>
#include <QMainWindow>
#include <QtNetwork/QUdpSocket>
#include <QAudio>
#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioOutput>
#include <QIODevice>

namespace Ui {
class clientAudio;
}

class clientAudio : public QMainWindow
{
    Q_OBJECT

public:
    explicit clientAudio(QWidget *parent = nullptr);
    ~clientAudio();

private:
    Ui::clientAudio *ui;
    QString address;
    QUdpSocket *udpSocketSend;      //用于音频信号的发送
    QUdpSocket *udpSocketReceive;   //用于音频信号的接收
    QAudioOutput *output;
    QIODevice *outputDevice;
    QAudioInput *input;
    QIODevice *inputDevice;
    struct video{
        char audiodata[1024];
        int lens;
    };

public slots:
    void receiveAddress(QString addr);

private slots:
    void onReadySend();
    void onReadyReceive();
    void on_exitButton_clicked();

signals:
    void endChat();
};

#endif // CLIENTAUDIO_H

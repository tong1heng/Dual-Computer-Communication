#ifndef SERVERAUDIO_H
#define SERVERAUDIO_H

#include <QMainWindow>
#include <QMainWindow>
#include <QtNetwork/QUdpSocket>
#include <QAudio>
#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioOutput>
#include <QIODevice>

namespace Ui {
class serverAudio;
}

class serverAudio : public QMainWindow {
    Q_OBJECT

public:
    explicit serverAudio(QWidget *parent = nullptr);
    ~serverAudio();

private:
    Ui::serverAudio *ui;
    QString address;
    QUdpSocket *udpSocketSend;      //用于音频信号的发送
    QUdpSocket *udpSocketReceive;   //用于音频信号的接收
    QAudioInput *input;
    QIODevice *inputDevice;
    QAudioOutput *output;
    QIODevice *outputDevice;
    struct video {
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

#endif // SERVERAUDIO_H

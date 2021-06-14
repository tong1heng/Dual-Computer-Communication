#include "clientAudio.h"
#include "ui_clientAudio.h"

clientAudio::clientAudio(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::clientAudio)
{
    ui->setupUi(this);

    /********************** SEND **********************/

    udpSocketSend= new QUdpSocket(this);
    udpSocketSend->bind(QHostAddress::Any,10005);   //绑定10005端口
    //设置采样参数
    QAudioFormat formatS;
    formatS.setSampleRate(8000);
    formatS.setChannelCount(1);
    formatS.setSampleSize(16);
    formatS.setCodec("audio/pcm");
    formatS.setSampleType(QAudioFormat::SignedInt);
    formatS.setByteOrder(QAudioFormat::LittleEndian);

    input = new QAudioInput(formatS,this);
    inputDevice = input->start();                   //input开始读入输入的音频信号，写入QIODevice，这里是inputDevice
    //槽函数，当inputDevice收到input写入的音频数据之后,调用onReadySend函数，发送数据到目标主机上
    connect(inputDevice,SIGNAL(readyRead()),this,SLOT(onReadySend()));

    /********************** RECEIVE **********************/

    udpSocketReceive = new QUdpSocket(this);
    udpSocketReceive->bind(QHostAddress::Any,10004);
    connect(udpSocketReceive,SIGNAL(readyRead()),this,SLOT(onReadyReceive()));  //收到网络数据报就开始往outputDevice写入，进行播放
    QAudioFormat formatR;
    formatR.setSampleRate(8000);
    formatR.setChannelCount(1);
    formatR.setSampleSize(16);
    formatR.setCodec("audio/pcm");
    formatR.setSampleType(QAudioFormat::SignedInt);
    formatR.setByteOrder(QAudioFormat::LittleEndian);

    output = new QAudioOutput(formatR,this);
    outputDevice = output->start();                 //开始播放
}

clientAudio::~clientAudio() {
    delete ui;
}

void clientAudio::onReadySend() {
    qDebug()<<"It's sending audio!"<<endl;
    video vp;
    memset(&vp,0,sizeof(vp));

    //读取音频
    vp.lens = inputDevice->read(vp.audiodata,1024); //读取音频
    QHostAddress destaddr;
    destaddr.setAddress(address);
    udpSocketSend->writeDatagram((const char*)&vp,sizeof(vp),destaddr,10005);   //将此结构体发送给目标主机，端口10005
}

void clientAudio::onReadyReceive() {
    while(udpSocketReceive->hasPendingDatagrams()) {
        QHostAddress senderip;
        quint16 senderport;
        qDebug()<<"audio is being received..."<<endl;
        video vp;
        memset(&vp,0,sizeof(vp));
        udpSocketReceive->readDatagram((char*)&vp,sizeof(vp),&senderip,&senderport);
        outputDevice->write(vp.audiodata,vp.lens);
    }
}

void clientAudio::receiveAddress(QString addr) {
    address=addr;
}

void clientAudio::on_exitButton_clicked() {
    emit endChat();
    this->close();
}

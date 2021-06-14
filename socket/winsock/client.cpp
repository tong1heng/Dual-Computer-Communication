#include "client.h"
#include "ui_client.h"
#include "clientAudio.h"

QString saveFilePathC;                  //保存文件路径
client* client::MyPointer = nullptr;    //用于静态成员函数发送信号
Ui::client* uic = nullptr;              //外部访问ui的指针

void ui_init(Ui::client *ui_ptr) { //初始化指针uic
    uic = ui_ptr;
}

client::client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::client) { //构造函数
    ui->setupUi(this);

    ui_init(ui);
    MyPointer=this;
    connect(client::MyPointer, &client::createQFileDialog, this, &client::getFilePath); //连接信号与槽
}

client::~client() { //析构函数
    closesocket(sockClientMsg);
    closesocket(sockClientFile);
    WSACleanup();
    delete ui;
}

void client::getFilePath() { //槽函数：主线程获取保存文件路径
    saveFilePathC = QFileDialog::getSaveFileName(NULL,tr(""),"",tr("All(*.*)")); //选择文件保存路径
}

void client::on_exitButton_clicked() {
    this->close();
}

void client::on_connectButton_clicked() { //客户端点击“连接”
    if(ui->serverIPAddress->text() == "") {
        QMessageBox::information(this, "提示", "请输入服务器IP地址");
        return;
    }
    if(ui->messagePort->text() == "") {
        QMessageBox::information(this, "提示", "请输入消息端口号");
        return;
    }
    ui->serverIPAddress->setReadOnly(true);
    ui->messagePort->setReadOnly(true);

    int portMsg = ui->messagePort->text().toInt();
    char *IPAddress;
    QByteArray ba = ui->serverIPAddress->text().toUtf8();
    IPAddress = ba.data();

    if(WSAStartup(MAKEWORD(2, 2), &wsaMsg) != 0 || WSAStartup(MAKEWORD(2, 2), &wsaFile) != 0) { //加载套接字库
        QMessageBox::information(this,"Error","Failed to load Winsock");
        return ;
    }

    SOCKADDR_IN addrSrvMsg; //服务器地址
    addrSrvMsg.sin_family = AF_INET;
    addrSrvMsg.sin_port = htons(portMsg);
    addrSrvMsg.sin_addr.S_un.S_addr = inet_addr(IPAddress);

    //创建客户端套接字
    sockClientMsg = socket(AF_INET, SOCK_STREAM, 0);   //流步套接字
    if((unsigned long long)SOCKET_ERROR == sockClientMsg) {
        QMessageBox::information(this,"Error", "Socket() error:" + QString(WSAGetLastError()));
        return ;
    }

    sockClientFile = socket(AF_INET, SOCK_STREAM, 0);   //流步套接字
    if((unsigned long long)SOCKET_ERROR == sockClientFile) {
        QMessageBox::information(this,"Error", "Socket() error:" + QString(WSAGetLastError()));
        return ;
    }

    //向服务器发出连接请求
    if((unsigned long long)::connect(sockClientMsg, (struct sockaddr*)&addrSrvMsg, sizeof(addrSrvMsg)) == INVALID_SOCKET) {
        QMessageBox::information(this, "Error", "Connect failed:" + QString(WSAGetLastError()));
        return ;
    }
    else {
        ui->textEdit->append("Connect Successfully");
    }

    //开一个不断循环等待接收消息的线程
    pthread_t tidsMsg[2];
    int retMsg = pthread_create(&tidsMsg[0], NULL, ctrlRecvMsg, (void*)&sockClientMsg); //参数：创建的线程id，线程参数，线程运行函数的起始地址，运行函数的参数
    if(retMsg != 0) { //创建线程成功返回0
        QMessageBox::information(this,"Error", "pthread_create error:error_code=" + QString(retMsg));
    }

    //文件相关
    //设置地址
    SOCKADDR_IN addrSrvFile;
    addrSrvFile.sin_family = AF_INET;
    addrSrvFile.sin_port = htons(9998); //1024以上的端口号
    addrSrvFile.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

    /* bind */
    int retValFile = bind(sockClientFile, (LPSOCKADDR)&addrSrvFile, sizeof(SOCKADDR_IN));
    if(retValFile == SOCKET_ERROR) {
        QMessageBox::information(this, "Error", "Bind failed:" + QString(WSAGetLastError()));
        return ;
    }
    /* listen */
    if(listen(sockClientFile, 10) ==SOCKET_ERROR) { //10代表允许连接的个数
        QMessageBox::information(this, "Error", "Listen failed:" + QString(WSAGetLastError()));
        return ;
    }
    //开一个不断循环等待接收文件的线程
    pthread_t tidsFile[2];
    int retFile = pthread_create(&tidsFile[0], NULL, ctrlRecvFile, (void*)&sockClientFile); //参数：创建的线程id，线程参数，线程运行函数的起始地址，运行函数的参数
    if(retFile != 0) { //创建线程成功返回0
        QMessageBox::information(this,"Error", "pthread_create error:error_code=" + QString(retFile));
    }
}

void client::on_sendMessageButton_clicked() {
    //获取待发送的消息
    char *buff;        //发送缓冲区
    QString tmp = ui->message->toPlainText();
    QByteArray ba = tmp.toUtf8();
    buff = ba.data();

    if (tmp == "") { //消息为空
        return;
    }
    //获取当前时间
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy年MM月dd日 hh:mm:ss");
    QString message="客户端>>" + ui->message->toPlainText();

    int e = send(sockClientMsg, buff, ba.size(), 0);
    if (e == SOCKET_ERROR) {
        QMessageBox::information(this, "Error", "Send failed");
        return;
    }
    ui->textEdit->append(current_date);
    ui->textEdit->append(message);
    ui->message->clear();
}

void* client::ctrlRecvMsg(void *args) {
    SOCKET sockConnMsg = *((SOCKET*)args);      //建立套接字
    //接收数据
    while (true) {
        char recvBuf[10000];                        //接收缓冲区
        memset(recvBuf, 0, sizeof(recvBuf));
        int nRecv = ::recv(sockConnMsg, recvBuf, sizeof(recvBuf), 0);
        if (nRecv > 0) {
            QDateTime current_date_time = QDateTime::currentDateTime();
            QString current_date = current_date_time.toString("yyyy年MM月dd日 hh:mm:ss");
            uic->textEdit->append(current_date);
            uic->textEdit->append("服务器>>" + QString(recvBuf));
        }
        else
            break;
    }
}

void client::on_findFileButton_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this,tr(""),"",tr("All(*.*)")); //选择要发送的文件
    ui->filePath->setText(fileName);
}

void client::on_sendFileButton_clicked() {
    if(ui->filePath->toPlainText() == "") {
        QMessageBox::information(this, "Error", "Empty file path");
        return ;
    }

    //服务器IP地址
    char *IPAddress;
    QByteArray ba0 = ui->serverIPAddress->text().toUtf8();
    IPAddress = ba0.data();

    //创建套接字去connect对面
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    //向服务器发起请求
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.S_un.S_addr = inet_addr(IPAddress);
    sockAddr.sin_port = htons(9999);

    if((unsigned long long)::connect(sock, (struct sockaddr*)&sockAddr, sizeof(SOCKADDR)) == INVALID_SOCKET) {
        QMessageBox::information(this, "Error", "Connect failed:" + QString(WSAGetLastError()));
        return ;
    }

    QString fP = ui->filePath->toPlainText();
    QByteArray ba = fP.toUtf8();
    QTextCodec *code = QTextCodec::codecForName("GB2312");      //解决中文路径问题
    FILE *fp = fopen(code->fromUnicode(ba).data(), "rb");       //以二进制方式打开文件
    if (fp == NULL) {
        QMessageBox::information(this, "Error", "Cannot open the selected file");
        return ;
    }

    //循环发送数据，直到文件结尾
    char buffer[100000] = {0};
    int nCount;
    while ((nCount = fread(buffer, 1, 100000, fp)) > 0) {
        send(sock, buffer, nCount, 0);
    }
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy年MM月dd日 hh:mm:ss");
    ui->textEdit->append(current_date);
    ui->textEdit->append("文件" + fP + "已发送");
    shutdown(sock,SD_SEND);         //文件读取完毕，断开输出流，向客户端发送FIN包
    recv(sock, buffer, 100000, 0);  //阻塞，等待客户端接收完毕

    //关闭套接字和文件
    fclose(fp);
    closesocket(sock);
}

void* client::ctrlRecvFile(void *args) {
    SOCKET sockLocFile = *( (SOCKET*)args );   //建立套接字
    while (true) {
        SOCKADDR_IN addrClientFile;
        int lenFile = sizeof(SOCKADDR);
        SOCKET sockConnFile = accept(sockLocFile, (SOCKADDR *) &addrClientFile, &lenFile); //会阻塞进程，直到有服务器连接上来为止
        if(sockConnFile == (unsigned long long)SOCKET_ERROR) {
            QMessageBox::information(NULL, "Error", "Accept failed:" + QString(WSAGetLastError()));
            continue;
        }

        qDebug() << "before emit.";
        emit MyPointer->createQFileDialog();    //发送信号，在主线程中打开另存为窗口获取保存文件路径
        qDebug() << "after emit.";
        while(saveFilePathC=="") {}             //等待主线程
        QTextCodec *code = QTextCodec::codecForName("GB2312");              //解决中文路径问题
        FILE *fp = fopen(code->fromUnicode(saveFilePathC).data(), "wb");    //以二进制方式创建文件
        if (fp == NULL) {
            QMessageBox::information(NULL, "Error", "Cannot open file");
            fclose(fp);
            closesocket(sockConnFile);
            saveFilePathC="";
            continue;
        }

        // 接收文件
        char buffer[100000] = {0};
        int nCount;
        while ((nCount = recv(sockConnFile, buffer, 100000, 0)) > 0) {
            fwrite(buffer, nCount, 1, fp);
        }
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_date = current_date_time.toString("yyyy年MM月dd日 hh:mm:ss");
        uic->textEdit->append(current_date);
        uic->textEdit->append("文件" + saveFilePathC + "已接收");
        fclose(fp);
        closesocket(sockConnFile);
        saveFilePathC="";
    }
}

void client::on_clearTextButton_clicked() {
    ui->textEdit->clear();
}

void client::on_clearMessageButton_clicked() {
    ui->message->clear();
}

void client::on_goToVoiceChatButton_clicked() {
    clientAudio* CA=new clientAudio;
    connect(this, SIGNAL(sendAddress(QString)), CA, SLOT(receiveAddress(QString)));
    connect(CA, SIGNAL(endChat()), this, SLOT(addEndChatLog()));
    emit this->sendAddress(ui->serverIPAddress->text());
    CA->show();
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy年MM月dd日 hh:mm:ss");
    ui->textEdit->append(current_date);
    ui->textEdit->append("语音通话开始");
}

void client::addEndChatLog() {
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy年MM月dd日 hh:mm:ss");
    ui->textEdit->append(current_date);
    ui->textEdit->append("语音通话结束");
}

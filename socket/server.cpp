#include "server.h"
#include "ui_server.h"
#include "serverAudio.h"

QString saveFilePathS;                  //保存文件路径
server* server::MyPointer = nullptr;    //用于静态成员函数发送信号
Ui::server* uis = nullptr;              //外部访问ui的指针

void ui_init(Ui::server *ui_ptr) { //初始化指针uis
    uis = ui_ptr;
}

server::server(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::server) { //构造函数
    ui->setupUi(this);

    ui_init(ui);
    MyPointer=this;
    connect(server::MyPointer, &server::createQFileDialog, this, &server::getFilePath); //连接信号与槽
}

server::~server() { //析构函数
    closesocket(sockSrvMsg);
    closesocket(sockSrvFile);
    closesocket(sockConnMsg);
    WSACleanup();
    delete ui;
}

void server::getFilePath() { //槽函数：主线程获取保存文件路径
    saveFilePathS = QFileDialog::getSaveFileName(NULL,tr(""),"",tr("All(*.*)")); //选择文件保存路径
}

void server::on_exitButton_clicked() {
    this->close();
}

void server::on_start_clicked() { //服务器点击“启动”
    if(ui->messagePort->text() == "") {
        QMessageBox::information(this, "提示", "请输入消息端口号");
        return;
    }
    ui->messagePort->setReadOnly(true);

    int portMsg = ui->messagePort->text().toInt();

    if(WSAStartup(MAKEWORD(2, 2), &wsaMsg) != 0 || WSAStartup(MAKEWORD(2, 2), &wsaFile) != 0) { //加载套接字库
        QMessageBox::information(this, "Error", "Failed to load Winsock");
        return ;
    }

    //创建用于监听的套接字，分别用于消息和文件
    sockSrvMsg = socket(AF_INET, SOCK_STREAM, 0);
    sockSrvFile = socket(AF_INET, SOCK_STREAM, 0);

    SOCKADDR_IN addrSrvMsg;
    addrSrvMsg.sin_family = AF_INET;
    addrSrvMsg.sin_port = htons(portMsg);   //1024以上的端口号
    addrSrvMsg.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //INADDR_ANY 代表任意ip

    SOCKADDR_IN addrSrvFile;
    addrSrvFile.sin_family = AF_INET;
    addrSrvFile.sin_port = htons(9999); //1024以上的端口号
    addrSrvFile.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //INADDR_ANY 代表任意ip

    /* bind */
    int retValMsg = bind(sockSrvMsg, (LPSOCKADDR)&addrSrvMsg, sizeof(SOCKADDR_IN));
    int retValFile = bind(sockSrvFile, (LPSOCKADDR)&addrSrvFile, sizeof(SOCKADDR_IN));
    if(retValMsg == SOCKET_ERROR || retValFile == SOCKET_ERROR) {
        QMessageBox::information(this, "Error", "Bind failed:" + QString(WSAGetLastError()));
        return ;
    }
    /* listen */
    if(listen(sockSrvMsg,10) ==SOCKET_ERROR || listen(sockSrvFile,10) ==SOCKET_ERROR) { //10代表允许连接的个数
        QMessageBox::information(this, "Error", "Listen failed:" + QString(WSAGetLastError()));
        return ;
    }
    ui->textEdit->append("启动成功，等待连接...");
    /* accept */
    SOCKADDR_IN addrClient;
    int len = sizeof(SOCKADDR);
    sockConnMsg = accept(sockSrvMsg, (SOCKADDR *) &addrClient, &len); //阻塞进程，直到有客户端连接上来为止

    if(sockConnMsg == (unsigned long long)SOCKET_ERROR) {
        QMessageBox::information(this, "Error", "Accept failed:" + QString(WSAGetLastError()));
        return ;
    }

    ui->textEdit->append("Accept client IP:" + QString(inet_ntoa(addrClient.sin_addr)));
    clientIP = QString(inet_ntoa(addrClient.sin_addr));
    
    //发送打招呼消息
    char buf[] = "Server: Hello, I'm a server!";
    int iSend = send(sockConnMsg, buf, sizeof(buf), 0);
    if (iSend == SOCKET_ERROR) {
        QMessageBox::information(this, "Error", "Send failed");
    }

    //开一个不断循环等待接收消息的线程
    pthread_t tidsMsg[2];
    int retMsg = pthread_create(&tidsMsg[0], NULL, ctrlRecvMsg, (void*)&sockConnMsg); //参数：创建的线程id，线程参数，线程运行函数的起始地址，运行函数的参数
    if(retMsg != 0) { //创建线程成功返回0
        QMessageBox::information(this,"Error", "pthread_create error:error_code=" + QString(retMsg));
    }
    
    //开一个不断循环等待接收文件的线程
    pthread_t tidsFile[2];
    int retFile = pthread_create(&tidsFile[0], NULL, ctrlRecvFile, (void*)&sockSrvFile); //参数：创建的线程id，线程参数，线程运行函数的起始地址，运行函数的参数
    if(retFile != 0) { //创建线程成功返回0
        QMessageBox::information(this,"Error", "pthread_create error:error_code=" + QString(retFile));
    }
}

void server::on_sendMessageButton_clicked() {
    //获取待发送的消息
    char *buff;         //发送缓冲区
    QString tmp = ui->message->toPlainText();
    QByteArray ba = tmp.toUtf8();
    buff = ba.data();

    if (tmp == "") { //消息为空
        return;
    }
    //获取当前时间
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy年MM月dd日 hh:mm:ss");
    QString message="服务器>>" + ui->message->toPlainText();

    int e = send(sockConnMsg, buff, ba.size(), 0);
    if (e == SOCKET_ERROR) {
        QMessageBox::information(this, "Error", "Send failed");
        return;
    }
    ui->textEdit->append(current_date);
    ui->textEdit->append(message);
    ui->message->clear();
}

void* server::ctrlRecvMsg(void* args) {
    SOCKET sockConnMsg = *((SOCKET*)args);      //建立套接字
    //接收数据
    while (true) {
        char recvBuf[10000];                        //接收缓冲区
        memset(recvBuf, 0, sizeof(recvBuf));
        int nRecv = ::recv(sockConnMsg, recvBuf, sizeof(recvBuf), 0);
        if (nRecv > 0) {
            QDateTime current_date_time = QDateTime::currentDateTime();
            QString current_date = current_date_time.toString("yyyy年MM月dd日 hh:mm:ss");
            uis->textEdit->append(current_date);
            uis->textEdit->append("客户端>>" + QString(recvBuf));
        }
        else
            break;
    }
}

void server::on_findFileButton_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this,tr(""),"",tr("All(*.*)")); //选择要发送的文件
    ui->filePath->setText(fileName);
}

void server::on_sendFileButton_clicked() {
    if(ui->filePath->toPlainText() == "") {
        QMessageBox::information(this, "Error", "Empty file path");
        return ;
    }

    //客户端IP地址
    char *Addr;
    QByteArray q = clientIP.toUtf8();
    Addr = q.data();

    // 创建套接字去connect对面
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    //向客户端发起请求
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.S_un.S_addr = inet_addr(Addr);
    sockAddr.sin_port = htons(9998);

    if((unsigned long long)::connect(sock, (struct sockaddr*)&sockAddr, sizeof(SOCKADDR)) == INVALID_SOCKET) {
        QMessageBox::information(this, "Error", "Connect failed:" + QString(WSAGetLastError()));
        return ;
    }
    //qDebug() << "connect";

    QString fP = ui->filePath->toPlainText();
    QByteArray ba = fP.toUtf8();
    QTextCodec *code = QTextCodec::codecForName("GB2312");  //解决中文路径问题
    FILE *fp = fopen(code->fromUnicode(ba).data(), "rb");   //以二进制方式打开文件
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

void* server::ctrlRecvFile(void* args) {
    SOCKET sockLocFile = *((SOCKET*)args);   //建立套接字
    while (true) {
    	SOCKADDR_IN addrClientFile;
    	int lenFile = sizeof(SOCKADDR);
        SOCKET sockConnFile = accept(sockLocFile, (SOCKADDR *) &addrClientFile, &lenFile); //会阻塞进程，直到有客户端连接上来为止
        if(sockConnFile == (unsigned long long)SOCKET_ERROR) {
            QMessageBox::information(NULL, "Error", "Accept failed:" + QString(WSAGetLastError()));
            continue;
        }

        emit MyPointer->createQFileDialog();    //发送信号，在主线程中打开另存为窗口获取保存文件路径
        while(saveFilePathS=="") {}             //等待主线程
        QTextCodec *code = QTextCodec::codecForName("GB2312");              //解决中文路径问题
        FILE *fp = fopen(code->fromUnicode(saveFilePathS).data(), "wb");    //以二进制方式创建文件
        if (fp == NULL) {
            QMessageBox::information(NULL, "Error", "Cannot open file");
            fclose(fp);
            closesocket(sockConnFile);
            saveFilePathS="";
            continue;
        }

        //接收文件
        char buffer[100000] = {0};
        int nCount;
        while ((nCount = recv(sockConnFile, buffer, 100000, 0)) > 0) {
            fwrite(buffer, nCount, 1, fp);
            //qDebug()<<"fwrite()";
        }
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_date = current_date_time.toString("yyyy年MM月dd日 hh:mm:ss");
        uis->textEdit->append(current_date);
        uis->textEdit->append("文件" + saveFilePathS + "已接收");
        fclose(fp);
        closesocket(sockConnFile);
        saveFilePathS = "";
    }
}

void server::on_clearTextButton_clicked() {
    ui->textEdit->clear();
}

void server::on_clearMessageButton_clicked() {
    ui->message->clear();
}

void server::on_goToVoiceChatButton_clicked() {
    serverAudio* SA=new serverAudio;
    connect(this, SIGNAL(sendAddress(QString)), SA, SLOT(receiveAddress(QString)));
    connect(SA, SIGNAL(endChat()), this, SLOT(addEndChatLog()));
    emit this->sendAddress(clientIP);
    SA->show();
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy年MM月dd日 hh:mm:ss");
    ui->textEdit->append(current_date);
    ui->textEdit->append("语音通话开始");
}

void server::addEndChatLog() {
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy年MM月dd日 hh:mm:ss");
    ui->textEdit->append(current_date);
    ui->textEdit->append("语音通话结束");
}

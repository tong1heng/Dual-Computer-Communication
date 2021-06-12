#include "server.h"
#include "ui_server.h"

QString saveFilePath;
server* server::MyPointer =nullptr;
Ui::server* uis = nullptr;

void ui_init(Ui::server *ui_ptr) {
    uis = ui_ptr;
}

server::server(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::server) {
    ui->setupUi(this);
    ui_init(ui);
    MyPointer=this;
    connect(server::MyPointer, &server::createQFileDialog, this, &server::getFilePath);
}

server::~server() {
    delete ui;
}

void server::getFilePath() {
    saveFilePath = QFileDialog::getSaveFileName(NULL,tr(""),"",tr("All(*.*)")); //选择文件保存路径
    qDebug()<<saveFilePath;
}

void server::on_start_clicked() {
    WSADATA wsaData,wsaFile;
    int port = ui->port->text().toInt();
    char buf[] = "Server: Hello, I'm a server!";

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0 || WSAStartup(MAKEWORD(2, 2), &wsaFile) != 0) { //加载套接字库
        QMessageBox::information(this, "Error", "Failed to load Winsock");
        return ;
    }

    //创建用于监听的套接字
    SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);
    SOCKET sockSrvFile = socket(AF_INET, SOCK_STREAM, 0);

    SOCKADDR_IN addrSrv;
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(port); //1024以上的端口号
    addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //INADDR_ANY 代表任意ip

    SOCKADDR_IN addrSrvFile;
    addrSrvFile.sin_family = AF_INET;
    addrSrvFile.sin_port = htons(9999); //1024以上的端口号
    addrSrvFile.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //INADDR_ANY 代表任意ip

    int retVal = bind(sockSrv, (LPSOCKADDR)&addrSrv, sizeof(SOCKADDR_IN));
    int retValFile = bind(sockSrvFile, (LPSOCKADDR)&addrSrvFile, sizeof(SOCKADDR_IN));
    if(retVal == SOCKET_ERROR || retValFile == SOCKET_ERROR) {
        QMessageBox::information(this, "Error", "Bind failed:" + QString(WSAGetLastError()));
        return ;
    }

    if(listen(sockSrv,10) ==SOCKET_ERROR || listen(sockSrvFile,10) ==SOCKET_ERROR) { //10代表允许连接的个数
        QMessageBox::information(this, "Error", "Listen failed:" + QString(WSAGetLastError()));
        return ;
    }

    ui->textEdit->append("启动成功，等待连接...");

    SOCKADDR_IN addrClient;
    int len = sizeof(SOCKADDR);
    sockConn = accept(sockSrv, (SOCKADDR *) &addrClient, &len); //会阻塞进程，直到有客户端连接上来为止

    
    if(sockConn == SOCKET_ERROR || sockConnFile == SOCKET_ERROR) {
        QMessageBox::information(this, "Error", "Accept failed:" + QString(WSAGetLastError()));
        return ;
    }

    ui->textEdit->append("Accept client IP:" + QString(inet_ntoa(addrClient.sin_addr)));
    
    int iSend = send(sockConn, buf, sizeof(buf), 0);
    if (iSend == SOCKET_ERROR) {
        QMessageBox::information(this, "Error", "Send failed");
    }

    pthread_t tids[2];
    int ret = pthread_create(&tids[0], NULL, ctrlRecvS, (void*)&sockConn); //参数：创建的线程id，线程参数，线程运行函数的起始地址，运行函数的参数
    if( ret != 0 ) { //创建线程成功返回0
        QMessageBox::information(this,"Error", "pthread_create error:error_code=" + QString(ret));
    }
    
    // 开一个不断循环等待接收的线程
    pthread_t tidsFile[2];
    int retFile = pthread_create(&tidsFile[0], NULL, ctrlRecvSFile, (void*)&sockSrvFile); //参数：创建的线程id，线程参数，线程运行函数的起始地址，运行函数的参数
    if( retFile != 0 ) { //创建线程成功返回0
        QMessageBox::information(this,"Error", "pthread_create error:error_code=" + QString(retFile));
    }
    

}

void server::on_sendMessageButton_clicked() {
    //SOCKET sockClient1 = sockConn;  //建立套接字
    char *buff;                    //发送缓冲区
    //convert into char*
    QString tmp = ui->message->toPlainText();
    QByteArray ba = tmp.toUtf8();
    buff = ba.data();

    if (buff == "") {
        return;
    }

    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy年MM月dd日 hh:mm:ss");
    QString message="服务器>>" + ui->message->toPlainText();

    int e = send(sockConn, buff, sizeof(buff), 0);
    if (e == SOCKET_ERROR) {
        QMessageBox::information(this, "Error", "Send failed");
        return;
    }
    ui->textEdit->append(current_date);
    ui->textEdit->append(message);
    ui->message->clear();
}

void* server::ctrlRecvS(void* args) {
    SOCKET sockConn = *( (SOCKET*)args );   //建立套接字
    char recvBuf[10000];                    //接收缓冲区
    memset(recvBuf, 0, sizeof(recvBuf));
    //接收数据
    while (true) {
        int nRecv = ::recv(sockConn, recvBuf, sizeof(recvBuf), 0);
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

void* server::ctrlRecvSFile(void* args) {
    SOCKET sockLocFile = *( (SOCKET*)args );   //建立套接字
    qDebug() << "not in the loop";
    
    while (true) {
    	SOCKADDR_IN addrClientFile;
    	int lenFile = sizeof(SOCKADDR);
        SOCKET sockConnFile = accept(sockLocFile, (SOCKADDR *) &addrClientFile, &lenFile); //会阻塞进程，直到有客户端连接上来为止
        if(sockConnFile == SOCKET_ERROR) {
            qDebug() << "accept() error";
        }

        emit MyPointer->createQFileDialog();

        while(saveFilePath=="") {}
//        qDebug()<<"fopen之前的path"<<saveFilePath;

        QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
        FILE *fp = fopen(code->fromUnicode(saveFilePath).data(), "wb");   //以二进制方式打开（创建）文件
        if (fp == NULL)
        {
            qDebug() << "Cannot open file,press any key to exit!\n";
            system("pause");
            exit(0);
        }

        // 接收文件
        char buffer[100000] = { 0 };
        int nCount;
        while ((nCount = recv(sockConnFile, buffer, 100000, 0)) > 0)
        {
            fwrite(buffer, nCount, 1, fp);
            qDebug()<<"fwrite()";
        }

        fclose(fp);
        closesocket(sockConnFile);
        saveFilePath="";
    }
}

void server::on_findFileButton_clicked() {
    QString FileName = QFileDialog::getOpenFileName(this,tr(""),"",tr("All(*.*)")); //选择路径
    ui->filePath->setText(FileName);
}

void server::on_sendFileButton_clicked() {
    // 创建套接字去connect对面
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);


    //向服务器发起请求
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = htons(9998);

    if(::connect(sock, (struct sockaddr*)&sockAddr, sizeof(SOCKADDR)) == INVALID_SOCKET) {
        QMessageBox::information(this, "Error", "Connect failed:" + QString(WSAGetLastError()));
        return ;
    }


    //先检查文件是否存在
    qDebug() << "connect";
    qDebug()<<ui->filePath->toPlainText().toUtf8().data();
    QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题

    FILE *fp = fopen(code->fromUnicode(ui->filePath->toPlainText().toUtf8()).data(), "rb");  //以二进制方式打开文件
    if (fp == NULL) {
        qDebug() << "Cannot open file,press any key to exit!\n";
        system("pause");
        exit(0);
    }


    //循环发送数据，直到文件结尾
    char buffer[100000] = {0};
    int nCount;
    while ((nCount = fread(buffer, 1, 100000, fp)) > 0) {
        send(sock, buffer, nCount, 0);
    }

    shutdown(sock,
             SD_SEND);  //文件读取完毕，断开输出流，向客户端发送FIN包
    recv(sock, buffer, 100000, 0);  //阻塞，等待客户端接收完毕


    //关闭套接字和文件
    fclose(fp);
    closesocket(sock);
}

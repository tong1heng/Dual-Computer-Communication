#include "client.h"
#include "ui_client.h"

QString saveFilePathC;
client* client::MyPointerC =nullptr;
Ui::client* uic = nullptr;

void ui_init(Ui::client *ui_ptr) {
    uic = ui_ptr;
}

client::client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::client) {
    ui->setupUi(this);
    ui_init(ui);

    MyPointerC=this;
    connect(client::MyPointerC, &client::createQFileDialogC, this, &client::getFilePath);
}

client::~client() {
    delete ui;
}

void client::on_connectButton_clicked() {
    WSADATA wsaData,wsaFile;
    int port=ui->port->text().toInt();
    char *IPAddress;
    QByteArray ba = ui->serverIPAddress->text().toUtf8();
    IPAddress = ba.data();

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0 || WSAStartup(MAKEWORD(2, 2), &wsaFile) != 0) {
        QMessageBox::information(this,"Error","Failed to load Winsock");
        return ;
    }

    SOCKADDR_IN addrSrv; //服务器地址
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(port);
//    addrSrv.sin_addr.S_un.S_addr = inet_addr(IPAddress);
    addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    //创建客户端套接字
    sockClient = socket(AF_INET, SOCK_STREAM, 0);   //流步套接字
    if(SOCKET_ERROR == sockClient) {
        QMessageBox::information(this,"Error", "Socket() error:" + QString(WSAGetLastError()));
        return ;
    }

    sockClientFile = socket(AF_INET, SOCK_STREAM, 0);   //流步套接字
    if(SOCKET_ERROR == sockClientFile) {
        QMessageBox::information(this,"Error", "Socket() error:" + QString(WSAGetLastError()));
        return ;
    }

    //向服务器发出连接请求
    if(::connect(sockClient, (struct sockaddr*)&addrSrv, sizeof(addrSrv)) == INVALID_SOCKET
//       || ::connect(sockClientFile, (struct sockaddr*)&addrSrvFile, sizeof(addrSrvFile)) == INVALID_SOCKET
      ) {
        QMessageBox::information(this, "Error", "Connect failed:" + QString(WSAGetLastError()));
        return ;
    }
    else {
        ui->textEdit->append("Connect Successfully");
    }
    pthread_t tids[2];
    int ret = pthread_create(&tids[0], NULL, ctrlRecv, (void*)&sockClient); //参数：创建的线程id，线程参数，线程运行函数的起始地址，运行函数的参数
    if(ret != 0) { //创建线程成功返回0
        QMessageBox::information(this,"Error", "pthread_create error:error_code=" + QString(ret));
    }


    // 设置地址
    SOCKADDR_IN addrSrvFile;
    addrSrvFile.sin_family = AF_INET;
    addrSrvFile.sin_port = htons(9998); //1024以上的端口号
    addrSrvFile.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");


    // 绑定
    int retValFile = bind(sockClientFile, (LPSOCKADDR)&addrSrvFile, sizeof(SOCKADDR_IN));
    if(retValFile == SOCKET_ERROR) {
        QMessageBox::information(this, "Error", "Bind failed:" + QString(WSAGetLastError()));
        return ;
    }


    // 监听
    if(listen(sockClientFile, 10) ==SOCKET_ERROR) { //10代表允许连接的个数
        QMessageBox::information(this, "Error", "Listen failed:" + QString(WSAGetLastError()));
        return ;
    }


    // 创建线程，不断accept
    pthread_t tidsFile[2];
    int retFile = pthread_create(&tidsFile[0], NULL, ctrlRecvFile, (void*)&sockClientFile); //参数：创建的线程id，线程参数，线程运行函数的起始地址，运行函数的参数
    if(retFile != 0) { //创建线程成功返回0
        QMessageBox::information(this,"Error", "pthread_create error:error_code=" + QString(ret));
    }
}

void client::on_sendMessageButton_clicked() {
    //SOCKET sockClient1 = sockClient;    //建立套接字
    char *buff1;                        //发送缓冲区
    //convert into char*
    QString tmp = ui->message->toPlainText();
    QByteArray ba = tmp.toUtf8();
    buff1 = ba.data();

    if (buff1 == "") {
        return;
    }

    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy年MM月dd日 hh:mm:ss");
    QString message="客户端>>" + ui->message->toPlainText();

    int e = send(sockClient, buff1, sizeof(buff1), 0);
    if (e == SOCKET_ERROR) {
        QMessageBox::information(this, "Error", "Send failed");
        return;
    }
    ui->textEdit->append(current_date);
    ui->textEdit->append(message);
    ui->message->clear();
}

void* client::ctrlRecv(void *args) {
    SOCKET sockConn = *( (SOCKET*)args );   //建立套接字
    char recvBuf[10000];                    //接收缓冲区
    memset(recvBuf, 0, sizeof(recvBuf));
    //接收数据
    while (true) {
        int nRecv = ::recv(sockConn, recvBuf, sizeof(recvBuf), 0);
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
    QString FileName = QFileDialog::getOpenFileName(this,tr(""),"",tr("All(*.*)")); //选择路径
    ui->filePath->setText(FileName);
}

void* client::ctrlRecvFile(void *args) {
    SOCKET sockLocFile = *( (SOCKET*)args );   //建立套接字

    while (true) {
        SOCKADDR_IN addrClientFile;
        int lenFile = sizeof(SOCKADDR);

        qDebug() << "client before accept()";

        SOCKET sockConnFile = accept(sockLocFile, (SOCKADDR *) &addrClientFile, &lenFile); //会阻塞进程，直到有客户端连接上来为止
        if(sockConnFile == SOCKET_ERROR) {
            qDebug() << "accept() error";
            continue;
        }

        qDebug() << "client after accept()";

        emit MyPointerC->createQFileDialogC();

        while(saveFilePathC=="") {}
//        qDebug()<<"fopen之前的path"<<saveFilePath;

        QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
        FILE *fp = fopen(code->fromUnicode(saveFilePathC).data(), "wb");   //以二进制方式打开（创建）文件
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
        saveFilePathC="";
    }
}

void client::on_sendFileButton_clicked()
{
    // 创建套接字去connect对面
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);


    //向服务器发起请求
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = htons(9999);

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


void client::getFilePath() {
    saveFilePathC = QFileDialog::getSaveFileName(NULL,tr(""),"",tr("All(*.*)")); //选择文件保存路径
    qDebug()<<saveFilePathC;
}

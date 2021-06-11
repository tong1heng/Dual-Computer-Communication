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
            uis->textEdit->append("客户端>>" + QString(QLatin1String(recvBuf)));
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
        qDebug() << "before the accept()";
        SOCKET sockConnFile = accept(sockLocFile, (SOCKADDR *) &addrClientFile, &lenFile); //会阻塞进程，直到有客户端连接上来为止
        if(sockConnFile == SOCKET_ERROR) {
            qDebug() << "accept() error";
        }
        qDebug() << "accept()";

        // 确定路径
<<<<<<< Updated upstream
        QString fileName = QFileDialog::getSaveFileName(NULL,tr(""),"",tr("All(*.*)")); //选择文件保存路径
//        QString fileName = "C:\\Users\\28320\\Desktop\\out.out";
        FILE *fp = fopen(fileName.toLatin1().data(), "wb");   //以二进制方式打开（创建）文件
=======


        //QString fileName = QFileDialog::getSaveFileName(NULL,tr(""),"",tr("All(*.*)")); //选择文件保存路径
        emit MyPointer->createQFileDialog();

        while(saveFilePath=="") {}
        qDebug()<<"fopen之前的path"<<saveFilePath;
        //QString fileName = "E:\\todo\\1.out";

        QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
        FILE *fp = fopen(code->fromUnicode(saveFilePath).data(), "wb");   //以二进制方式打开（创建）文件
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
=======
            qDebug()<<"fwrite()";
>>>>>>> Stashed changes
        }
//	puts("File transfer success!\n");

        fclose(fp);
        closesocket(sockConnFile);
<<<<<<< Updated upstream
=======
        saveFilePath="";
>>>>>>> Stashed changes
    }
    

//    //接收数据
//    while (true) {
//        int nRecv = ::recv(sockConnFile, recvBuf, sizeof(recvBuf), 0);
//        if (nRecv > 0) {
//            QString fileName = QFileDialog::getSaveFileName(NULL,tr(""),"",tr("All(*.*)")); //选择文件保存路径
//            if (!fileName.isNull()) {
<<<<<<< Updated upstream
//                FILE *fp = fopen(fileName.toLatin1().data(), "wb");
=======
//                FILE *fp = fopen(fileName.toUtf8().data(), "wb");
>>>>>>> Stashed changes
//                fwrite(recvBuf, nRecv, 1, fp);
//                while ((nRecv = ::recv(sockConnFile, recvBuf, sizeof(recvBuf), 0)) > 0) {
//                    fwrite(recvBuf, nRecv, 1, fp);
//                }

//                QDateTime current_date_time = QDateTime::currentDateTime();
//                QString current_date =current_date_time.toString("yyyy年MM月dd日 hh:mm:ss");
//                uis->textEdit->append(current_date);
//                uis->textEdit->append("文件已成功接收");
//            }
//            else {  //取消接收
//                QDateTime current_date_time = QDateTime::currentDateTime();
//                QString current_date =current_date_time.toString("yyyy年MM月dd日 hh:mm:ss");
//                uis->textEdit->append(current_date);
//                uis->textEdit->append("取消接收文件");
//                while ((nRecv = ::recv(sockConnFile, recvBuf, sizeof(recvBuf), 0)) > 0) {}
//            }
//        }
//        else
//            break;
//    }
}

void server::on_findFileButton_clicked() {
    QString FileName = QFileDialog::getOpenFileName(this,tr(""),"",tr("All(*.*)")); //选择路径
    ui->filePath->setText(FileName);
}

void server::on_sendFileButton_clicked() {
    char buff[10000];                    //发送缓冲区

    FILE *fp = fopen(ui->filePath->toPlainText().toUtf8().data(), "rb");   //以二进制方式打开文件
    if (fp == NULL) {
        QMessageBox::information(this, "Error", "Cannot open file");
        return ;
    }

    int nCount;
    while ((nCount = fread(buff, 1, sizeof(buff), fp)) > 0) {
        int e=send(sockConnFile, buff, nCount, 0);
        if (e == SOCKET_ERROR) {
            QMessageBox::information(this, "Error", "Send failed");
            return;
        }
    }

    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy年MM月dd日 hh:mm:ss");
    ui->textEdit->append(current_date);
    ui->textEdit->append("文件" + ui->filePath->toPlainText() + "发送成功");
    QMessageBox::information(this, "提示", "文件" + ui->filePath->toPlainText() + "发送成功");
    ui->filePath->clear();
}

#include "client.h"
#include "ui_client.h"

Ui::client* uic = nullptr;

void ui_init(Ui::client *ui_ptr) {
    uic = ui_ptr;
}

client::client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::client) {
    ui->setupUi(this);
    ui_init(ui);
}

client::~client() {
    delete ui;
}

void client::on_connectButton_clicked() {
    WSADATA wsaData,wsaFile;
    int port=ui->port->text().toInt();
    char *IPAddress;
    QByteArray ba = ui->serverIPAddress->text().toLatin1();
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

    SOCKADDR_IN addrSrvFile;
    addrSrvFile.sin_family = AF_INET;
    addrSrvFile.sin_port = htons(7777); //1024以上的端口号
    addrSrvFile.sin_addr.S_un.S_addr = inet_addr(IPAddress);

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

//    pthread_t tidsFile[2];
//    int retFile = pthread_create(&tidsFile[0], NULL, ctrlRecvFile, (void*)&sockClientFile); //参数：创建的线程id，线程参数，线程运行函数的起始地址，运行函数的参数
//    if(retFile != 0) { //创建线程成功返回0
//        QMessageBox::information(this,"Error", "pthread_create error:error_code=" + QString(ret));
//    }
}

void client::on_sendMessageButton_clicked() {
    //SOCKET sockClient1 = sockClient;    //建立套接字
    char *buff1;                        //发送缓冲区
    //convert into char*
    QString tmp = ui->message->toPlainText();
    QByteArray ba = tmp.toLatin1();
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
            uic->textEdit->append("服务器>>" + QString(QLatin1String(recvBuf)));
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
    SOCKET sockConnFile = *( (SOCKET*)args );   //建立套接字
    char recvBuf[10000];                        //接收缓冲区
    memset(recvBuf, 0, sizeof(recvBuf));
    //接收数据
    while (true) {
        int nRecv = ::recv(sockConnFile, recvBuf, sizeof(recvBuf), 0);
        if (nRecv > 0) {
            //QString fileName = QFileDialog::getSaveFileName(NULL,tr(""),"",tr("All(*.*)")); //选择文件保存路径
            QString fileName="E:\\Todo\\rec.mp4";
            if (!fileName.isNull()) {
                FILE *fp = fopen(fileName.toLatin1().data(), "wb");
                fwrite(recvBuf, nRecv, 1, fp);
                while ((nRecv = ::recv(sockConnFile, recvBuf, sizeof(recvBuf), 0x40)) > 0) {
                    fwrite(recvBuf, nRecv, 1, fp);
                }

                QDateTime current_date_time = QDateTime::currentDateTime();
                QString current_date =current_date_time.toString("yyyy年MM月dd日 hh:mm:ss");
                uic->textEdit->append(current_date);
                uic->textEdit->append("文件已成功接收");
            }
            else {  //取消接收
                QDateTime current_date_time = QDateTime::currentDateTime();
                QString current_date =current_date_time.toString("yyyy年MM月dd日 hh:mm:ss");
                uic->textEdit->append(current_date);
                uic->textEdit->append("取消接收文件");
                while ((nRecv = ::recv(sockConnFile, recvBuf, sizeof(recvBuf), 0)) > 0) {}
            }
        }
        else
            break;
    }
}

void client::on_sendFileButton_clicked()
{
    qDebug() << "clicked !!!!";
    // 创建套接字去connect对面
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    //向服务器发起请求
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = htons(9999);

    qDebug() << "request !!";
    if(::connect(sock, (struct sockaddr*)&sockAddr, sizeof(SOCKADDR)) == INVALID_SOCKET) {
        QMessageBox::information(this, "Error", "Connect failed:" + QString(WSAGetLastError()));
        return ;
    }
//    ::connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
    //先检查文件是否存在
    qDebug() << "connect";
//    const char *filename = "C:/Users/28320/Desktop/file/a.in";     //文件名

    FILE *fp = fopen(ui->filePath->toPlainText().toLatin1().data(), "rb");  //以二进制方式打开文件
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

    fclose(fp);

    //关闭套接字
    closesocket(sock);

//    char buff[10000];                    //发送缓冲区

//    FILE *fp = fopen(ui->filePath->toPlainText().toLatin1().data(), "rb");   //以二进制方式打开文件
//    if (fp == NULL) {
//        QMessageBox::information(this, "Error", "Cannot open file");
//        return ;
//    }

//    int nCount;
//    while ((nCount = fread(buff, 1, sizeof(buff), fp)) > 0) {
//        int e=send(sockClientFile, buff, nCount, 0);
//        if (e == SOCKET_ERROR) {
//            QMessageBox::information(this, "Error", "Send failed");
//            return;
//        }
//    }

//    QDateTime current_date_time =QDateTime::currentDateTime();
//    QString current_date =current_date_time.toString("yyyy年MM月dd日 hh:mm:ss");
//    ui->textEdit->append(current_date);
//    ui->textEdit->append("文件" + ui->filePath->toPlainText() + "发送成功");
//    QMessageBox::information(this, "提示", "文件" + ui->filePath->toPlainText() + "发送成功");
//    ui->filePath->clear();
}

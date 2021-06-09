#include "server.h"
#include "ui_server.h"

Ui::server *ui = nullptr;

void ui_init(Ui::server *ui_ptr)
{
    ui = ui_ptr;
}
/*
void* recv1(SOCKET sockConn)
{
    char recvBuf[10000];
    memset(recvBuf, 0, sizeof(recvBuf));
    //接收数据
    while(true)
    {
        int nRecv = ::recv(sockConn, recvBuf, sizeof(recvBuf), 0);
        //cout<<endl;
        if(nRecv>0)
        {
            //cout<<"他说："<<recvBuf<<endl;
            QDateTime current_date_time =QDateTime::currentDateTime();
            QString current_date =current_date_time.toString("yyyy年MM月dd日 hh:mm:ss");
            ui->textEdit->append(current_date);
            QString message(recvBuf);
            ui->textEdit->append(">>"+message);
        }
        else
            break;
    }
}*/

void* recv1(SOCKET sockConn)
{
    char recvBuf[10000];
    memset(recvBuf, 0, sizeof(recvBuf));
    //接收数据

    int nRecv = ::recv(sockConn, recvBuf, sizeof(recvBuf), 0);
    //cout<<endl;
    if(nRecv>0)
    {
        //cout<<"他说："<<recvBuf<<endl;
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy年MM月dd日 hh:mm:ss");
        ui->textEdit->append(current_date);
        QString message(recvBuf);
        ui->textEdit->append(">>"+message);
    }

}

void* send1(void* args)
{
    SOCKET sockClient1 = *( (SOCKET*)args );//建立套接字

    //char buff1[10000];
    //cout<<"请输入"<<endl;
    //cin>>buff1;
    if(ui->message->toPlainText() == "")
    {

    }
    QString str=ui->message->toPlainText();
    QByteArray byteArray=str.toUtf8();
    char *buff1=byteArray.data();

    int e = send(sockClient1, buff1, sizeof(buff1), 0);
    if(e == SOCKET_ERROR){
        //printf("send failed");
        QMessageBox::information(NULL,"Error", "Send failed");

    }
    //cout<<"我说："<<buff1<<endl;
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy年MM月dd日 hh:mm:ss");
    ui->textEdit->append(current_date);
    QString message=">>" + ui->message->toPlainText();
    ui->textEdit->append(message);

}



server::server(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::server)
{
    ui->setupUi(this);
    ui_init(ui);
}

server::~server()
{
    delete ui;
}

void server::on_start_clicked()
{
    WSADATA wsaData;
    QString p=ui->port->text();
    int port=p.toInt();



    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //加载套接字库
    {
        //printf("Failed to load Winsock");
        QMessageBox::information(this,"Error", "Failed to load Winsock");
        return ;
    }

    //创建用于监听的套接字
    SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);

    SOCKADDR_IN addrSrv;
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(port); //1024以上的端口号
    addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //INADDR_ANY 代表任意ip

    int retVal = bind(sockSrv, (LPSOCKADDR)&addrSrv, sizeof(SOCKADDR_IN));
    if(retVal == SOCKET_ERROR){
        //printf("Failed bind:%d\n", WSAGetLastError());
        QMessageBox::information(this,"Error", "Failed bind:"+QString(WSAGetLastError()));
        return ;
    }

    if(listen(sockSrv,10) ==SOCKET_ERROR){//10代表允许连接的个数
        //printf("Listen failed:%d", WSAGetLastError());
        QMessageBox::information(this,"Error", "Listen failed:"+QString(WSAGetLastError()));
        return ;
    }

    ui->textEdit->append("启动成功，等待连接...");

    SOCKADDR_IN addrClient;
    int len = sizeof(SOCKADDR);
    sockConn = accept(sockSrv, (SOCKADDR *) &addrClient, &len);//会阻塞进程，直到有客户端连接上来为止

    if(sockConn == SOCKET_ERROR){
        //printf("Accept failed:%d", WSAGetLastError());
        QMessageBox::information(this,"Error", "Accept failed:"+QString(WSAGetLastError()));
        return ;
    }

    //printf("Accept client IP:[%s]\n", inet_ntoa(addrClient.sin_addr));
    ui->textEdit->append("Accept client IP:" + QString(inet_ntoa(addrClient.sin_addr)));
}

void server::on_file_clicked()
{
    QString FileName = QFileDialog::getOpenFileName(this,tr(""),"",tr("All(*.*)")); //选择路径
    ui->filepath->setText(FileName);
}

void server::on_sendMessage_clicked()
{
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy年MM月dd日 hh:mm:ss");
    ui->textEdit->append(current_date);
    QString message=">>" + ui->message->toPlainText();
    ui->textEdit->append(message);
    ui->message->clear();

    char buf[] = "Server: hello, I am a server.....";

    //发送数据
    //send1(sockConn);
    int iSend = send(sockConn, buf, sizeof(buf) , 0);
    if(iSend == SOCKET_ERROR){
        //printf("send failed");
        QMessageBox::information(this,"Error", "Send failed");
        return ;
    }
    pthread_t tids[2];
    int ret = pthread_create( &tids[0], NULL, send1, (void*)&sockConn ); //参数：创建的线程id，线程参数，线程运行函数的起始地址，运行函数的参数
    if( ret != 0 ) //创建线程成功返回0
    {
        //std::cout << "pthread_create error:error_code=" << ret << std::endl;
        QMessageBox::information(this,"Error", "pthread_create error:error_code="+QString(ret));
    }
    recv1(sockConn);
    //rintf("*****receive\n");
    //if(recvBuf == "end")break;
    closesocket(sockConn);
}

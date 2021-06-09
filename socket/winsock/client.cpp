#include "client.h"
#include "ui_client.h"

Ui::client *ui_c = nullptr;

void ui_init(Ui::client *ui_ptr)
{
    ui_c = ui_ptr;
}
/*
void* recv2(SOCKET sockConn)
{
    char recvBuf[10000];
    memset(recvBuf, 0, sizeof(recvBuf));
    //接收数据
    while(true)
    {
        int nRecv = ::recv(sockConn, recvBuf, sizeof(recvBuf), 0);
        if(nRecv>0)
        {
            //cout<<recvBuf<<endl;
            QDateTime current_date_time =QDateTime::currentDateTime();
            QString current_date =current_date_time.toString("yyyy年MM月dd日 hh:mm:ss");
            ui_c->clientTextEdit->append(current_date);
            QString message(recvBuf);
            ui_c->clientTextEdit->append(">>"+message);
        }
        else
            break;
    }
}
*/
void* recv2(SOCKET sockConn)
{
    char recvBuf[10000];
    memset(recvBuf, 0, sizeof(recvBuf));
    //接收数据

    int nRecv = ::recv(sockConn, recvBuf, sizeof(recvBuf), 0);
    if(nRecv>0)
    {
        //cout<<recvBuf<<endl;
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy年MM月dd日 hh:mm:ss");
        ui_c->clientTextEdit->append(current_date);
        QString message(recvBuf);
        ui_c->clientTextEdit->append(">>"+message);
    }

}
/*
void* send2(void* args)
{
    SOCKET sockClient1 = *( (SOCKET*)args );
    while(true)
    {
        //char buff1[10000]  ;
        //cout<<"我说：";
        //cin>>buff1;
        if(ui_c->message->toPlainText() == "")
        {
            break;
        }
        QString str=ui_c->message->toPlainText();
        QByteArray byteArray=str.toUtf8();
        char *buff1=byteArray.data();

        int e = send(sockClient1, buff1, sizeof(buff1), 0);
        if(e == SOCKET_ERROR)
        {
            //printf("send failed");
            QMessageBox::information(NULL,"Error", "Send failed");
            break;
        }
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy年MM月dd日 hh:mm:ss");
        ui_c->clientTextEdit->append(current_date);
        QString message=">>" + ui_c->message->toPlainText();
        ui_c->clientTextEdit->append(message);
    }
}
*/
void* send2(void* args)
{
    SOCKET sockClient1 = *( (SOCKET*)args );

    //char buff1[10000]  ;
    //cout<<"我说：";
    //cin>>buff1;
    if(ui_c->message->toPlainText() == "")
    {

    }
    QString str=ui_c->message->toPlainText();
    QByteArray byteArray=str.toUtf8();
    char *buff1=byteArray.data();

    int e = send(sockClient1, buff1, sizeof(buff1), 0);
    if(e == SOCKET_ERROR)
    {
        //printf("send failed");
        QMessageBox::information(NULL,"Error", "Send failed");

    }
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy年MM月dd日 hh:mm:ss");
    ui_c->clientTextEdit->append(current_date);
    QString message=">>" + ui_c->message->toPlainText();
    ui_c->clientTextEdit->append(message);

}

client::client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::client)
{
    ui->setupUi(this);
    ui_init(ui);
}

client::~client()
{
    delete ui;
}

void client::on_connectButton_clicked()
{
    int port=ui->port->text().toInt();
    //加载套接字
    WSADATA wsaData;


    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        //printf("Failed to load Winsock");
        QMessageBox::information(this,"Error","Failed to load Winsock");
        return ;
    }

    SOCKADDR_IN addrSrv; //服务端地址
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(port);
    addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    //创建客户端套接字
    sockClient = socket(AF_INET, SOCK_STREAM, 0);//流步套接字
    if(SOCKET_ERROR == sockClient){
        printf("Socket() error:%d", WSAGetLastError());
        return ;
    }

    //向服务器发出连接请求
    if(::connect(sockClient, (struct  sockaddr*)&addrSrv, sizeof(addrSrv)) == INVALID_SOCKET)
    {
        //printf("Connect failed:%d", WSAGetLastError());
        QMessageBox::information(this,"Error", "Connect failed:"+QString(WSAGetLastError()));
        return ;
    }
    else
    {
        ui->clientTextEdit->append("Successfully Connect");
    }
}

void client::on_sendMessage_clicked()
{
    char buff[1024];
    memset(buff, 0, sizeof(buff));
    pthread_t tids[2];
    int ret = pthread_create( &tids[0], NULL, send2, (void*)&sockClient ); //参数：创建的线程id，线程参数，线程运行函数的起始地址，运行函数的参数
    if( ret != 0 ) //创建线程成功返回0
    {
        //cout << "pthread_create error:error_code=" << ret << endl;
    }

    //接收数据
    recv(sockClient, buff, sizeof(buff), 0);
    //printf("%s\n", buff);
    recv2(sockClient);
}

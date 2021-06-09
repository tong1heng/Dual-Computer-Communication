#include "client.h"
#include "ui_client.h"

Ui::client* uic = nullptr;

void ui_init(Ui::client *ui_ptr)
{
    uic = ui_ptr;
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
    pthread_t tids[2];
    int ret = pthread_create( &tids[0], NULL, ctrlRecv, (void*)&sockClient ); //参数：创建的线程id，线程参数，线程运行函数的起始地址，运行函数的参数
    if( ret != 0 ) //创建线程成功返回0
    {
        //std::cout << "pthread_create error:error_code=" << ret << std::endl;
        QMessageBox::information(this,"Error", "pthread_create error:error_code="+QString(ret));
    }
}

void client::on_sendMessage_clicked()
{
    SOCKET sockClient1 = sockClient;  //建立套接字
    char *buff1;

    // 当sendMessage clicked之后

    QString tmp = ui->message->toPlainText();
    QByteArray ba = tmp.toLatin1(); // must
    buff1 = ba.data();

    if (buff1 == "") {
        return;
    }

    int e = send(sockClient1, buff1, sizeof(buff1), 0);
    if (e == SOCKET_ERROR) {
        qDebug() << "send failed";
        return;
    }
    ui->clientTextEdit->append("我说：" + tmp);
}

void* client::ctrlRecv(void *args) {
    SOCKET sockConn = *( (SOCKET*)args );//建立套接字
    char recvBuf[10000];
    memset(recvBuf, 0, sizeof(recvBuf));
    //接收数据
    while (true) {
        int nRecv = ::recv(sockConn, recvBuf, sizeof(recvBuf), 0);
        if (nRecv > 0)
            uic->clientTextEdit->append(QString(QLatin1String(recvBuf)));
        else
            break;
    }
}

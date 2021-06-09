#include "server.h"
#include "ui_server.h"

Ui::server* uis = nullptr;

void ui_init(Ui::server *ui_ptr)
{
    uis = ui_ptr;
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

    char buf[] = "Server: hello, I'm a server!";



    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //加载套接字库
    {
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
        QMessageBox::information(this,"Error", "Failed bind:"+QString(WSAGetLastError()));
        return ;
    }

    if(listen(sockSrv,10) ==SOCKET_ERROR){//10代表允许连接的个数
        QMessageBox::information(this,"Error", "Listen failed:"+QString(WSAGetLastError()));
        return ;
    }

    ui->textEdit->append("启动成功，等待连接...");

    SOCKADDR_IN addrClient;
    int len = sizeof(SOCKADDR);
    sockConn = accept(sockSrv, (SOCKADDR *) &addrClient, &len);//会阻塞进程，直到有客户端连接上来为止

    if(sockConn == SOCKET_ERROR){
        QMessageBox::information(this,"Error", "Accept failed:"+QString(WSAGetLastError()));
        return ;
    }

    ui->textEdit->append("Accept client IP:" + QString(inet_ntoa(addrClient.sin_addr)));
    
//    int iSend = send(sockConn, buf, sizeof(buf), 0);
//    if (iSend == SOCKET_ERROR) {
//        qDebug() << "send failed";
//    }
    pthread_t tids[2];
    int ret = pthread_create( &tids[0], NULL, ctrlRecvS, (void*)&sockConn ); //参数：创建的线程id，线程参数，线程运行函数的起始地址，运行函数的参数
    if( ret != 0 ) //创建线程成功返回0
    {
        //std::cout << "pthread_create error:error_code=" << ret << std::endl;
        QMessageBox::information(this,"Error", "pthread_create error:error_code="+QString(ret));
    }
}

void server::on_file_clicked()
{
    QString FileName = QFileDialog::getOpenFileName(this,tr(""),"",tr("All(*.*)")); //选择路径
    ui->filepath->setText(FileName);
}

void server::on_sendMessage_clicked()
{
    SOCKET sockClient1 = sockConn;  //建立套接字
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
    ui->textEdit->append("我说：" + tmp);
}

void* server::ctrlRecvS(void* args) {
    SOCKET sockConn = *( (SOCKET*)args );//建立套接字
    char recvBuf[10000];
    memset(recvBuf, 0, sizeof(recvBuf));
    //接收数据
    while (true) {
        int nRecv = ::recv(sockConn, recvBuf, sizeof(recvBuf), 0);
        if (nRecv > 0)
            uis->textEdit->append(QString(QLatin1String(recvBuf)));
        else
            break;
    }
}

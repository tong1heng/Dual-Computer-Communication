# Dual-Computer-Communication
## Framework
- 操作系统：Windows10
- 编程语言：C++
- 界面设计：Qt
- 连接方式：局域网内通信
- 实现功能：双向消息通信；双向文件传输；双向语音通话。
## How to communicate
两台主机，一个为服务端（SERVER），一个为客户端（CLIENT），进行通信时，首先设置端口号启动服务器，然后在客户端填入服务器的IP地址和使用的端口号，连接到服务器，进行双向通信。
### 1. 双向消息通信和双向文件传输
**server**：封装server类，私有成员包括ui指针，分别用于消息通信和文件传输的wsaMsg和wsaFile，套接字sockSrvMsg，sockSrvFile，sockConnMsg，记录连接上来的客户端IP地址clientIP，静态成员函数包括接收消息和接收文件的线程函数ctrlRecvMsg和ctrlRecvFile，另外还有一些需要的信号和槽函数。
```cpp
class server : public QMainWindow
{
    Q_OBJECT

public:
    explicit server(QWidget *parent = nullptr);
    ~server();

public slots:
    void getFilePath();

private slots:
    void on_start_clicked();
    void on_sendMessageButton_clicked();
    void on_findFileButton_clicked();
    void on_sendFileButton_clicked();
    void on_exitButton_clicked();
    void on_clearTextButton_clicked();
    void on_clearMessageButton_clicked();
    void on_goToVoiceChatButton_clicked();
    void addEndChatLog();

private:
    Ui::server *ui;
    WSADATA wsaMsg,wsaFile;
    SOCKET sockSrvMsg;
    SOCKET sockSrvFile;
    SOCKET sockConnMsg;
    QString clientIP;

    static void* ctrlRecvMsg(void *args);
    static void* ctrlRecvFile(void* args);
    static server* MyPointer;

signals:
    void createQFileDialog();
    void sendAddress(QString addr);
};
```
- 当服务端点击“启动”按钮时，首先通过输入控件获取端口号，然后加载分别用于消息和文件的套接字库，并创建用于监听的套接字，将套接字与IP地址、端口号进行绑定，然后进入监听状态，等待连接（阻塞进程）。当有客户端连接成功时，服务器界面显示连接的客户端IP地址，并发送打招呼消息“Hello, I’m a server!”，然后开启两个线程，一个用于不断循环等待接收消息，一个用于不断循环等待接收文件。
- 当服务端向客户端发送消息时，首先从控件中获取待发送的消息，将其复制到发送缓冲区，通过socket中的send将消息发送出去。
- 当服务端接收到客户端发来的消息时，通过socket中的recv将消息接收到缓冲区中，将其显示在屏幕控件上。
- 当服务端向客户端发送文件时，首先创建一个套接字去连接客户端，向客户端发起连接请求，然后通过QFileDialog选择需要发送的文件，通过二进制格式将其打开，循环发送数据直到文件结尾，发送完毕后关闭套接字和文件。
- 当服务端接收到客户端发送的文件时，首先accept客户端发起的连接，然后通过QFileDialog选择文件的保存路径以及文件名，通过二进制将其打开，不断循环接收数据写入文件中，接收完毕后关闭文件和套接字。

**client**：封装client类，私有成员包括ui指针，分别用于消息通信和文件传输的wsaMsg和wsaFile，套接字sockClientMsg，sockClientFile，静态成员函数包括接收消息和接收文件的线程函数ctrlRecvMsg和ctrlRecvFile，另外还有一些需要的信号和槽函数。
```cpp
class client : public QMainWindow
{
    Q_OBJECT

public:
    explicit client(QWidget *parent = nullptr);

    ~client();

public slots:
    void getFilePath();

private slots:
    void on_connectButton_clicked();
    void on_sendMessageButton_clicked();
    void on_findFileButton_clicked();
    void on_sendFileButton_clicked();
    void on_exitButton_clicked();
    void on_clearTextButton_clicked();
    void on_clearMessageButton_clicked();
    void on_goToVoiceChatButton_clicked();
    void addEndChatLog();

private:
    Ui::client *ui;
    WSADATA wsaMsg,wsaFile;
    SOCKET sockClientMsg;
    SOCKET sockClientFile;
    static void* ctrlRecvMsg(void* args);
    static void* ctrlRecvFile(void* args);
    static client* MyPointer;
signals:
    void createQFileDialog();
    void sendAddress(QString addr);
};
```
- 当客户端点击“连接”按钮时，首先通过输入控件获取服务器的IP地址和使用的端口号，然后加载分别用于消息和文件的套接字库，并创建用于监听的客户端套接字，向服务器发出消息通信的连接请求，然后开一个不断循环等待接收消息的线程。文件传输方面，设置使用的端口号，将其与文件套接字绑定，然后进入监听状态，开一个不断循环等待接收文件的线程。
- 当客户端向服务端发送消息时，首先从控件中获取待发送的消息，将其复制到发送缓冲区，通过socket中的send将消息发送出去。
- 当客户端接收到服务端发来的消息时，通过socket中的recv将消息接收到缓冲区中，将其显示在屏幕控件上。
- 当客户端向服务端发送文件时，首先创建一个套接字去连接服务端，向服务端发起连接请求，然后通过QFileDialog选择需要发送的文件，通过二进制格式将其打开，循环发送数据直到文件结尾，发送完毕后关闭套接字和文件。
- 当客户端接收到服务端发送的文件时，首先accept服务端发起的连接，然后通过QFileDialog选择文件的保存路径以及文件名，通过二进制将其打开，不断循环接收数据写入文件中，接收完毕后关闭文件和套接字。

### 2.双向语音通话
**server**：封装serverAudio类，私有成员包括ui指针，客户端的IP地址，用于音频信号发送和接收的QUdpSocket对象udpSocketSend和udpSocketReceive，和音频信号相关的对象input, inputDevice, output, outputDevice，音频数据缓冲区video，槽函数包括发送就绪和接收到数据后的操作，还有一些其他需要的信号和槽函数。
```cpp
class serverAudio : public QMainWindow {
    Q_OBJECT

public:
    explicit serverAudio(QWidget *parent = nullptr);
    ~serverAudio();

private:
    Ui::serverAudio *ui;
    QString address;
    QUdpSocket *udpSocketSend;      //用于音频信号的发送
    QUdpSocket *udpSocketReceive;   //用于音频信号的接收
    QAudioInput *input;
    QIODevice *inputDevice;
    QAudioOutput *output;
    QIODevice *outputDevice;
    struct video {
        char audiodata[1024];
        int lens;
    };

public slots:
    void receiveAddress(QString addr);

private slots:
    void onReadySend();
    void onReadyReceive();
    void on_exitButton_clicked();

signals:
    void endChat();
};
```
- 在类的构造函数中，创建分别用于发送和接收的QUdpSocket对象udpSocketSend和udpSocketReceive。
发送：将udp套接字对象和IP地址、端口号绑定，通过QAudioFormat对象设置采样参数，当inputDevice准备发送就绪后，通过调用槽函数发送采样数据。
接收：将udp套接字对象和IP地址、端口号绑定，通过QAudioFormat对象设置采样参数，当接收到数据后，通过调用槽函数接收采样数据，并进行播放。
- 准备发送就绪后，通过音频采样不断发送采样数据。
- 当接收到数据后，不断接收音频数据，并实时播放。

**client**：封装clientAudio类，私有成员包括ui指针，客户端的IP地址，用于音频信号发送和接收的QUdpSocket对象udpSocketSend和udpSocketReceive，和音频信号相关的对象input, inputDevice, output, outputDevice，音频数据缓冲区video，槽函数包括发送就绪和接收到数据后的操作，还有一些其他需要的信号和槽函数。
```cpp
class clientAudio : public QMainWindow
{
    Q_OBJECT

public:
    explicit clientAudio(QWidget *parent = nullptr);
    ~clientAudio();

private:
    Ui::clientAudio *ui;
    QString address;
    QUdpSocket *udpSocketSend;      //用于音频信号的发送
    QUdpSocket *udpSocketReceive;   //用于音频信号的接收
    QAudioOutput *output;
    QIODevice *outputDevice;
    QAudioInput *input;
    QIODevice *inputDevice;
    struct video{
        char audiodata[1024];
        int lens;
    };

public slots:
    void receiveAddress(QString addr);

private slots:
    void onReadySend();
    void onReadyReceive();
    void on_exitButton_clicked();

signals:
    void endChat();
};
```
- 在类的构造函数中，创建分别用于发送和接收的QUdpSocket对象udpSocketSend和udpSocketReceive。
发送：将udp套接字对象和IP地址、端口号绑定，通过QAudioFormat对象设置采样参数，当inputDevice准备发送就绪后，通过调用槽函数发送采样数据。
接收：将udp套接字对象和IP地址、端口号绑定，通过QAudioFormat对象设置采样参数，当接收到数据后，通过调用槽函数接收采样数据，并进行播放。
- 准备发送就绪后，通过音频采样不断发送采样数据。
- 当接收到数据后，不断接收音频数据，并实时播放。
## UI design
### WelcomeWindow
![welcome](https://raw.githubusercontent.com/TongYiheng/MarkdownPictures/main/Dual-Computer-Communication/20210630091958.png?token=GHSAT0AAAAAABM2CPNENA6YCZTLV75WCC5IYSOJKMQ)
### ServerWindow
![server](https://raw.githubusercontent.com/TongYiheng/MarkdownPictures/main/Dual-Computer-Communication/20210630092127.png?token=GHSAT0AAAAAABM2CPNFSTVFIRSTLUOG3KYWYSOJKQA)
### ClientWindow
![client](https://raw.githubusercontent.com/TongYiheng/MarkdownPictures/main/Dual-Computer-Communication/20210630092141.png?token=GHSAT0AAAAAABM2CPNEHU4J2ZMA4R64GCBQYSOJOKA)
### VoIPWindow
![voice](https://raw.githubusercontent.com/TongYiheng/MarkdownPictures/main/Dual-Computer-Communication/20210630092156.png?token=GHSAT0AAAAAABM2CPNFOBYYIBZMJB7S4LAEYSOJOKQ)
## Problems solved
- 消息通信：服务器和客户端一直保持TCP连接，使用的端口号由用户输入指定。
- 文件传输：当某个方向需要传输时，先进行单向的TCP连接，传输完成后立刻断开这个连接，服务器和客户端都进入监听状态，等待下一次连接。服务端发送、客户端接收使用端口9998，客户端发送、服务端接收使用端口9999.
- 语音通话：在通话过程中，服务器和客户端保持UDP连接，服务器发送、客户端接收使用端口10004，客户端发送、服务端接收使用端口10005。
- 由于无法知道对方何时发送消息或者传输文件，所以接收数据和文件需要采用多线程，在主线程之外单独开一个接收数据和一个接收文件的线程，接收线程一直不断循环，不影响主线程的正常运行。
- 在代码实现中使用了Qt的信号槽机制，由于Qt要求对ui的操作必须在主线程中进行，而在接收线程中有些部分需要对ui进行控制，比如将接收到的消息显示在控件中、打开QFileDialog窗口等等，因此在不同线程之间需要用信号和槽进行通信。另外，从服务端和客户端的主界面跳转到语音通话界面时，需要将对方的IP地址从当前类传入到对应的Audio类中，因此在不同类之间也需要使用信号和槽进行通信。

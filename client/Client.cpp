#include <QMessageBox>
#include "client/Client.h"
#include "widget/GUI.h"
#include "logic/Logic.h"

Client::Client()
{
    forwarded=false;
    connect(this,SIGNAL(readyRead()),this,SLOT(readMessage()));
    connect(logic,SIGNAL(gameStart()),this,SIGNAL(readyToStart()));
    connect(logic,SIGNAL(sendCommand(QString)),this,SLOT(sendMessage(QString)));
    connect(this,SIGNAL(getMessage(QString)),logic,SLOT(getCommand(QString)));
    connect(this,SIGNAL(disconnected()),this,SLOT(onDisconnected()));
    logic->setClient(this);
}

void Client::onDisconnected()
{
    if(forwarded){
        QMessageBox*prop=new QMessageBox(QMessageBox::Critical,"Warning","你已断线，请退出后重新登录");
        prop->exec();
    }
}

void Client::link(QString addr,int port)
{
    abort();
    m_addr = addr;
    connectToHost(addr,port);
}
void Client::readMessage()
{
    quint32 nextBlockSize;

    while(1){
        if(bytesAvailable() < sizeof(quint32)){
            return;
        }
        read((char*)&nextBlockSize,sizeof(quint32));
        //如果没有得到全部的数据，则返回，继续接收数据
        if(bytesAvailable() < nextBlockSize){
            return;
        }
        QString message(read(nextBlockSize));
        //将接收到的数据存放到变量中
        emit getMessage(message);
    }
}

void Client::sendMessage(QString message)
{
    QByteArray packet;
    qint32 len = message.size();
    packet.append((char*)&len, sizeof(qint32));
    packet.append(message);
    write(packet);
}

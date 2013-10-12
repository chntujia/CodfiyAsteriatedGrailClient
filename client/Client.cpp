#include <QMessageBox>
#include "client/Client.h"
#include "widget/GUI.h"
#include "logic/Logic.h"

Client::Client()
{
    forwarded=false;
    connect(this,SIGNAL(readyRead()),this,SLOT(readMessage()));
    connect(logic,SIGNAL(gameStart()),this,SIGNAL(readyToStart()));
    connect(logic,SIGNAL(sendCommand(quint16, google::protobuf::Message*)),this,SLOT(sendMessage(quint16, google::protobuf::Message*)));
    connect(this,SIGNAL(getMessage(quint16, google::protobuf::Message*)),logic,SLOT(getCommand(quint16, google::protobuf::Message*)));
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
        QByteArray message(read(nextBlockSize));

        char *msg = message.data();

        quint16 proto_type;
        google::protobuf::Message* proto;
        proto = (google::protobuf::Message*)proto_decoder(msg, proto_type);
        //将接收到的数据存放到变量中
        emit getMessage(proto_type, proto);
    }
}

void Client::sendMessage(quint16 proto_type, google::protobuf::Message* proto)
{
    string message;
    proto_encoder(proto_type, *proto, message);
    delete proto;

    QByteArray packet;
    packet.append(QString::fromStdString(message));
    write(packet);
}

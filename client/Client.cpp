#include <QMessageBox>
#include "client/Client.h"
#include "widget/GUI.h"
#include "logic/Logic.h"

Client::Client()
{
    nextBlockSize = -1;
    connect(this,SIGNAL(readyRead()),this,SLOT(readMessage()));
    connect(logic,SIGNAL(sendCommand(unsigned short, google::protobuf::Message*)),this,SLOT(sendMessage(unsigned short, google::protobuf::Message*)));
    connect(this,SIGNAL(disconnected()),this,SLOT(onDisconnected()));   
}

void Client::onDisconnected()
{
    QMessageBox*prop=new QMessageBox(QMessageBox::Critical,"Warning",QStringLiteral("你已断线，请退出后重新登录"));
    prop->exec();
}

void Client::link(QString addr,int port)
{
    abort();
    m_addr = addr;
    connectToHost(addr,port);
}
void Client::readMessage()
{
    while(bytesAvailable() > 0){
        if(nextBlockSize == -1){
            if(bytesAvailable() < sizeof(quint32)){
                return;
            }
            read((char*)&nextBlockSize,sizeof(quint32));
        }
        //如果没有得到全部的数据，则返回，继续接收数据
        if(bytesAvailable() < nextBlockSize){
            return;
        }
        QByteArray message(read(nextBlockSize));

        char *msg = message.data();

        unsigned short proto_type;
        google::protobuf::Message* proto;
        proto = (google::protobuf::Message*)proto_decoder(msg, proto_type);
        nextBlockSize = -1;
        //将接收到的数据存放到变量中
        emit getMessage(proto_type, proto);
    }
}

void Client::sendMessage(unsigned short proto_type, google::protobuf::Message* proto)
{
    string message;
    proto_encoder(proto_type, proto, message);
    delete proto;
    QByteArray packet;
    qint32 len = message.size();
    packet.append((char*)&len, sizeof(qint32));
    packet.append(message.c_str(), message.size());
    write(packet);
}

#include "LogArea.h"
#include "DataInterface.h"
#include "gui.h"

LogArea::LogArea(QWidget *parent) :
    QTextEdit(parent)
{
    setReadOnly(true);
    setStyleSheet("background-color:transparent");
    setFixedSize(259,360);
}
void LogArea::append(QString text)
{
    text.replace("[","<font color=\'yellow\'>[");
    text.replace("]","]</font>");
    QTextEdit::append(QString("<p style=\"margin:3px p2x; line-height:120%;\"><font color=\'white\'>%1</font></p>").arg(text));
}
ChatArea::ChatArea(QWidget *parent):
    QTextEdit(parent)
{
    setReadOnly(true);
    setStyleSheet("background-color:transparent");
    setFixedSize(259,246);
}
void ChatArea::append(int id,QString text)
{
    if(logic->isMuted(id))
        return;
    QString name = id == -1 ? QStringLiteral("系统") : dataInterface->getPlayerList().at(id)->getRoleName();
    if(name==NULL)
        name=dataInterface->getPlayerList().at(id)->getNickname();
    if(name==NULL)
        name="玩家"+QString::number(id);
    QString msg="<p style=\"margin:3px p2x; line-height:120%;\"><font color=\'yellow\'>"+name+"</font>";
    QTextEdit::append(msg+QString("<font color=\'white\'>:%1</font></p>").arg(text));
}
ChatLine::ChatLine(QWidget *parent):
    QLineEdit(parent)
{
    setStyleSheet("color: white;"
                  "background-color:transparent");
    setFixedSize(259,25);
    connect(this,SIGNAL(returnPressed()),this,SLOT(onReturnPressed()));
    connect(this,SIGNAL(toChat(unsigned short, google::protobuf::Message*)),logic->getClient(),SLOT(sendMessage(unsigned short, google::protobuf::Message*)));
    connect(this,SIGNAL(textChanged(QString)),this,SLOT(onTextChanged(QString)));
}
void ChatLine::onReturnPressed()
{    
    if(!text().isEmpty())
    {
        QStringList args = text().split(" ");
        if(args.size()>1){
            bool ok;
            int id = args[1].toInt(&ok);
            ok &= id > -1 && id < dataInterface->getPlayerMax();
            if(ok && args[0] == "mute"){
                logic->muteUser(id);
                gui->chatAppend(-1, QStringLiteral("<font color=\'pink\'>玩家%1被关进小黑屋</font>").arg(QString::number(id)));
                clear();
                return;
            }
            else if(ok && args[0] == "unmute"){
                logic->unmuteUser(id);
                gui->chatAppend(-1, QStringLiteral("<font color=\'pink\'>玩家%1又放出来了</font>").arg(QString::number(id)));
                clear();
                return;
            }
        }
        network::Talk* talk = new network::Talk();
        talk->set_txt(text().toStdString());
        emit toChat(network::MSG_TALK, talk);
    }
    clear();
}


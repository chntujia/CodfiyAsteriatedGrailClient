#include <QMessageBox>
#include "Logic.h"
#include "client/ClientUI.h"
#include "client/ui_ClientUI.h"
#include "data/DataInterface.h"
#include <QTextStream>
#include <QSound>
ClientUI::ClientUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientUI)
{
    ui->setupUi(this);
    QFile *fp=new QFile("account");
    if(fp->exists())
    {
        QTextStream account(fp);
        fp->open(QIODevice::ReadOnly);
        ui->editLoginName->setText(account.readLine());
        ui->editPassword->setText(account.readLine());
        fp->close();
    }
    int port = 11228;
    QString ip = "127.0.0.1";
    fp=new QFile("config");
    if(fp->exists())
    {
        QTextStream config(fp);
        fp->open(QIODevice::ReadOnly);
        ip = config.readLine();
        port = config.readLine().toInt();
        fp->close();
    }
    tcpSocket=new Client;
    tcpSocket->link(ip, port);
    QRegExp rx1("^[A-Za-z][A-Za-z0-9_]{6,12}");
    ui->regLoginName->setValidator(new QRegExpValidator(rx1, this));
    ui->editLoginName->setValidator(new QRegExpValidator(rx1, this));
    QRegExp rx2("[\u4e00-\u9fa5A-Za-z0-9_]{2,10}");
    ui->regNickname->setValidator(new QRegExpValidator(rx2, this));
    QRegExp rx3("[A-Za-z0-9_]{6,12}");
    ui->regPassword->setValidator(new QRegExpValidator(rx3, this));
    ui->regPasswordAgain->setValidator(new QRegExpValidator(rx3, this));
    ui->editPassword->setValidator(new QRegExpValidator(rx3, this));

    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)), this,SLOT(displayError(QAbstractSocket::SocketError)));
    connect(tcpSocket,SIGNAL(getMessage(uint16_t, google::protobuf::Message*)),this,SLOT(showMessage(uint16_t, google::protobuf::Message*)));
    connect(tcpSocket,SIGNAL(getMessage(uint16_t, google::protobuf::Message*)),logic,SLOT(getCommand(uint16_t, google::protobuf::Message*)));
    //merged
    connect(ui->btnLogin, SIGNAL(clicked()), this, SLOT(UserLogin()));
    connect(ui->btnGuest, SIGNAL(clicked()), this, SLOT(GuestLogin()));
    connect(ui->btnRegist, SIGNAL(clicked()), this, SLOT(UserRegistShow()));
    connect(ui->btnRegistCommit, SIGNAL(clicked()), this, SLOT(UserRegist()));
    connect(ui->btnBackLogin, SIGNAL(clicked()), this, SLOT(UserBackLogin()));

    ui->frameRegist->hide();
    ui->frameLogin->show();

    ui->editPassword->setEchoMode(QLineEdit::Password);
    ui->regPassword->setEchoMode(QLineEdit::Password);
    ui->regPasswordAgain->setEchoMode(QLineEdit::Password);
}

ClientUI::~ClientUI()
{
    delete ui;
}

void ClientUI::showMessage(uint16_t proto_type, google::protobuf::Message* proto)
{
    network::LoginResponse* login_rep;
    switch (proto_type)
    {
    case network::MSG_LOGIN_REP:
    {
        login_rep = (network::LoginResponse*)proto;

        // TODO: 登陆失败
        int result = login_rep->state();
        switch (result)
        {
        case 0: // 成功
            ui->LabError->setText("登入成功");
            ui->frameLogin->hide();
            ui->frameRegist->hide();
            dataInterface->nickName = QString::fromStdString(login_rep->nickname());
            accept();
            break;
        case 1: // 帐号错误
            ui->LabError->setText("用户名或密码错误");
            break;
        case 2: // 帐号被封停
            ui->LabError->setText("您的帐号已被封停，请联系客服");
            break;
        default:// 其它错误
            ui->LabError->setText("未知错误");
        }
        break;
    }
    /*
// 注册返回
    case 9002:
    {
        int result = arg[1].toInt();
        switch (result)
        {
        case 0: // 成功
            ui->LabErrorReg->setText("注册成功");
            ui->regNickname->setText("");
            ui->regLoginName->setText("");
            ui->regEmail->setText("");
            ui->regPassword->setText("");
            ui->regPasswordAgain->setText("");
            ui->regMobile->setText("");
            // ui->frameLobby->hide();
            // ui->frameRegist->hide();
            // ui->frameLogin->show();
            break;
        case 1: // 帐号重复
            ui->LabErrorReg->setText("您所申请的用户名重复");
            break;
        case 3:
            ui->LabErrorReg->setText("您所申请的昵称重复");
            break;
        default:// 其它错误
            ui->LabErrorReg->setText("未知错误");
        }
        break;
    }
    */
    }
}

void ClientUI::displayError(QAbstractSocket::SocketError)
{
    ui->LabError->setText(tcpSocket->errorString());
    ui->LabError->setStyleSheet("color:red");
}

void ClientUI::UserLogin()
{
    QString username = ui->editLoginName->text().trimmed();
    QString password = ui->editPassword->text().trimmed();
    if(username.trimmed() == "")
    {
        ui->LabError->setText(QStringLiteral("用户名不能为空"));
        ui->LabError->setStyleSheet("color:red");
        return;
    }

    if(username.length() < 5)
    {
        ui->LabError->setText(QStringLiteral("用户名太短，请检查"));
        ui->LabError->setStyleSheet("color:red");
        return;
    }

    if(password.trimmed() == "")
    {
        ui->LabError->setText(QStringLiteral("密码不能为空"));
        ui->LabError->setStyleSheet("color:red");
        return;
    }

    if(password.length() < 6)
    {
        ui->LabError->setText(QStringLiteral("密码太短，请检查"));
        ui->LabError->setStyleSheet("color:red");
        return;
    }

    // 发送数据包
    ui->LabError->setText(QStringLiteral("正在验证，请稍后......"));

    network::LoginRequest* login_req = new network::LoginRequest();
    login_req->set_asguest(false);
    login_req->set_user_id(username.toStdString());
    login_req->set_user_password(password.toStdString());
    tcpSocket->sendMessage(network::MSG_LOGIN_REQ, login_req);
}

void ClientUI::GuestLogin()
{
    network::LoginRequest* login_req = new network::LoginRequest();
    login_req->set_asguest(true);
    tcpSocket->sendMessage(network::MSG_LOGIN_REQ, login_req);
}

void ClientUI::UserRegistShow()
{
    ui->frameLogin->hide();
    ui->frameRegist->show();
}

void ClientUI::UserRegist()
{
    QString username = ui->regLoginName->text().trimmed();
    QString nickname = ui->regNickname->text().trimmed();
    QString password = ui->regPassword->text().trimmed();
    QString passwordAgain = ui->regPasswordAgain->text().trimmed();
    QString mobile = ui->regMobile->text().trimmed();
    QString email = ui->regEmail->text().trimmed();
    if(username.length() < 5)
    {
        ui->LabErrorReg->setText("用户名太短");
        ui->LabErrorReg->setStyleSheet("color:red");
        return;
    }

    if(password.length() < 6)
    {
        ui->LabErrorReg->setText("密码太短");
        ui->LabErrorReg->setStyleSheet("color:red");
        return;
    }

    if(password != passwordAgain)
    {
        ui->LabErrorReg->setText("两次密码不相同");
        ui->LabErrorReg->setStyleSheet("color:red");
        return;
    }

    // 手机号和邮箱暂时不检查
    if(mobile == "")
    {
        mobile = " ";
    }
    if(email == "")
    {
        email = " ";
    }
    // 发送数据包
    ui->LabErrorReg->setText("正在验证，请稍后...");

    network::RegisterRequest* register_req = new network::RegisterRequest();
    register_req->set_user_id((const char*) username.toLocal8Bit());
    register_req->set_user_password((const char*) password.toLocal8Bit());
    register_req->set_nickname((const char*) nickname.toLocal8Bit());
    register_req->set_mobile((const char*) mobile.toLocal8Bit());
    register_req->set_email((const char*) email.toLocal8Bit());

    tcpSocket->sendMessage(network::MSG_REGISTER_REQ, register_req);
}

void ClientUI::UserBackLogin()
{
    ui->frameRegist->hide();
    ui->frameLogin->show();
    ui->editLoginName->setText("");
    ui->editPassword->setText("");
    ui->LabError->setText("");
}

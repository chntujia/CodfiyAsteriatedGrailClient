#include <QInputDialog>
#include "Lobby.h"
#include "ui_Lobby.h"
#include "data/DataInterface.h"
#include "logic/Logic.h"
#include "widget/RoomView.h"
#include "widget/GUI.h"
#include <QDebug>
Lobby::Lobby(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Lobby)
{
    roomSet = NULL;
    ui->setupUi(this);
    setWindowTitle("Welcome back, " + dataInterface->nickName);
    ui->label->setToolTip(QStringLiteral("联系方式：chntujia@gmail.com"));
    logic->setLobby(this);
    connect(ui->tableWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(onItemClicked(QModelIndex)));
    connect(ui->createButton, SIGNAL(clicked()), this, SLOT(onCreateRoom()));
    connect(ui->refreshButton, SIGNAL(clicked()), this, SLOT(onRefreshList()));
    onRefreshList();

	this->setWindowFlags(this->windowFlags()&Qt::WindowMaximizeButtonHint);
}

Lobby::~Lobby()
{
    delete ui;
}

void Lobby::fill(RoomListResponse* list)
{
    ui->tableWidget->setRowCount(0);
    for(int i = 0; i < list->rooms_size(); i++)
    {
        RoomListResponse_RoomInfo game = list->rooms(i);
        rooms[game.room_id()] = game;
        QTableWidgetItem *newItem;
        ui->tableWidget->insertRow(i);

        if(game.playing()){
            newItem = new QTableWidgetItem(QStringLiteral("（进行中）") + QString::number(game.room_id()));
        }
        else{
            newItem = new QTableWidgetItem(QString::number(game.room_id()));
        }
        ui->tableWidget->setItem(i, ROOM_ID, newItem);

        newItem = new QTableWidgetItem(QString::fromStdString(game.room_name()));
        ui->tableWidget->setItem(i, ROOM_NAME, newItem);

        QString roleNum = QStringLiteral("标准");
        if(game.first_extension())
            roleNum += QStringLiteral(",一扩");
        if(game.second_extension())
            roleNum += QStringLiteral(",三版");
        if(game.sp_mo_dao())
            roleNum += QStringLiteral(",sp魔导");
        newItem = new QTableWidgetItem(roleNum);
        ui->tableWidget->setItem(i, ROLE_NUM, newItem);

        switch(game.role_strategy()){
        case ROLE_STRATEGY_RANDOM:
            newItem = new QTableWidgetItem(QStringLiteral("随机"));
            break;
        case ROLE_STRATEGY_31:
            newItem = new QTableWidgetItem(QStringLiteral("3选1"));
            break;
        case ROLE_STRATEGY_BP:
            newItem = new QTableWidgetItem(QStringLiteral("BAN-PICK"));
            break;
        case ROLE_STRATEGY_CM:
            newItem = new QTableWidgetItem(QStringLiteral("CM01"));
            break;
        default:
            newItem = new QTableWidgetItem(QStringLiteral("Error"));
            break;
        }
        ui->tableWidget->setItem(i, ROLE_MODE, newItem);

        switch(game.seat_mode()){
        case SEAT_MODE_RANDOM:
            newItem = new QTableWidgetItem(QStringLiteral("随机"));
            break;
        case SEAT_MODE_2COMBO:
            newItem = new QTableWidgetItem(QStringLiteral("二连"));
            break;
        case SEAT_MODE_3COMBO:
            newItem = new QTableWidgetItem(QStringLiteral("三连"));
            break;
        case SEAT_MODE_INTERLACE:
            newItem = new QTableWidgetItem(QStringLiteral("间隔"));
            break;
        case SEAT_MODE_RBBRRB:
            newItem = new QTableWidgetItem(QStringLiteral("红蓝蓝红红蓝"));
            break;
        default:
            newItem = new QTableWidgetItem(QStringLiteral("Error"));
            break;
        }
        ui->tableWidget->setItem(i, SEAT_MODE, newItem);

        newItem = new QTableWidgetItem(QString::number(game.now_player()) + "/" + QString::number(game.max_player()));
        ui->tableWidget->setItem(i, PLAYER_NUM, newItem);
        newItem = new QTableWidgetItem(game.allow_guest() ? "N" : "Y");
        ui->tableWidget->setItem(i, WELCOME_GUEST, newItem);
        newItem = new QTableWidgetItem(game.has_password() ? "Y" : "N");
        ui->tableWidget->setItem(i, HAS_PASSWORD, newItem);
        newItem = new QTableWidgetItem(game.silence() ? "Y" : "N");
        ui->tableWidget->setItem(i, SILENCE, newItem);

    }
    ui->tableWidget->sortByColumn(0);
    ui->tableWidget->resizeColumnToContents (true);
}

void Lobby::newWindow(int playerNum, bool silence)
{
    RoomView* window = (RoomView*)gui->newWindow(playerNum);
    connect(window, SIGNAL(closed()), this, SLOT(onWindowClose()));
    setEnable(false);
    if(silence)
    {
        gui->logAppend(QStringLiteral("这是个禁言房"));
        gui->getChatLine()->setEnabled(false);
    }
}

void Lobby::setEnable(bool enable)
{
    ui->tableWidget->setEnabled(enable);
    ui->createButton->setEnabled(enable);
    ui->refreshButton->setEnabled(enable);
}

void Lobby::onItemClicked(QModelIndex index)
{
    QString raw = ui->tableWidget->item(index.row(), ROOM_ID)->data(Qt::DisplayRole).toString();
    QStringList data = raw.split(QStringLiteral("）"));
    int roomId;
    if(data.size()>1){
        roomId = data[1].toInt();
    }
    else{
        roomId = raw.toInt();
    }
    RoomListResponse_RoomInfo room = rooms[roomId];
    network::EnterRoomRequest *enter= new network::EnterRoomRequest();
    if(room.has_password()){
        QString pwd = QInputDialog::getText(this, QStringLiteral("切，结界么？"),
                                    QStringLiteral("大声告诉我你知道的秘密:"), QLineEdit::Normal);
        if(!pwd.isEmpty())
            enter->set_password(pwd.toStdString());
    }
    ui->tableWidget->setEnabled(false);

    newWindow(room.max_player(), room.silence());

    enter->set_room_id(room.room_id());
    logic->getClient()->sendMessage(network::MSG_ENTER_ROOM_REQ, enter);
}

void Lobby::onCreateRoom()
{
    //FIXME: popup
    //打开房间设置面板
    if(roomSet!= NULL){roomSet->close();}
    roomSet = new RoomSet(logic->getIdentity());
    roomSet->show();
    connect(roomSet, SIGNAL(createRoom()), this, SLOT(onOpenRoom()));
    connect(roomSet, SIGNAL(backToLobby()), this, SLOT(onBackToLobby()));
    this->setEnable(false); //禁用大厅

}
void Lobby::onBackToLobby(){
    setEnable(true);
}

void Lobby::onOpenRoom(){
    int playerNum = roomSet->getPlayerNum();
    int seatOrder = roomSet->getSeatOrder();
    int roleSelectionStrategy = roomSet->getRoleSelection();
    bool firstExtension = roomSet->getFirstExtension();
    bool secondExtension = roomSet->getSecondExtension();
    bool spmodao = roomSet->getspMoDao();
    bool allowGuest = roomSet->getAllowGuest();
    std::string password = roomSet->getPassword().toStdString();
    std::string roomName = roomSet->getRoomName().toStdString();
    bool slience = roomSet->getSilence();

    network::CreateRoomRequest* create = new network::CreateRoomRequest();
    create->set_role_strategy((network::ROLE_STRATEGY)roleSelectionStrategy);
    create->set_max_player(playerNum);
    create->set_seat_mode(seatOrder);
    create->set_first_extension(firstExtension);
    create->set_second_extension(secondExtension);
    create->set_sp_mo_dao(spmodao);
    create->set_allow_guest(allowGuest);
    create->set_password(password);
    create->set_room_name(roomName);
    create->set_silence(slience);

    logic->getClient()->sendMessage(network::MSG_CREATE_ROOM_REQ, create);
    roomSet->close();//注意此时调用onBackToLobby()
    newWindow(playerNum, slience);//打开游戏界面
}

void Lobby::onRefreshList()
{
    network::RoomListRequest* refresh = new network::RoomListRequest();
    refresh->set_role_strategy(ROLE_STRATEGY_ALL);
    logic->getClient()->sendMessage(network::MSG_ROOMLIST_REQ, refresh);
}

void Lobby::onWindowClose()
{
    setEnable(true);
    logic->destroyRoom();
}

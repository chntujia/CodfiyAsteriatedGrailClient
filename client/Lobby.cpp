#include "Lobby.h"
#include "ui_Lobby.h"
#include "data/DataInterface.h"
#include "logic/Logic.h"
#include "widget/RoomView.h"
#include "widget/GUI.h"

Lobby::Lobby(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Lobby)
{
    ui->setupUi(this);
    logic->setLobby(this);
    connect(ui->tableWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(onItemClicked(QModelIndex)));
    connect(ui->createButton, SIGNAL(clicked()), this, SLOT(onCreateRoom()));
    connect(ui->refreshButton, SIGNAL(clicked()), this, SLOT(onRefreshList()));
    onRefreshList();
}

Lobby::~Lobby()
{
    delete ui;
}

void Lobby::fill(RoomListResponse* list)
{
    ui->tableWidget->setRowCount(0);
    roomList.CopyFrom(*list);
    for(int i = 0; i < list->rooms_size(); i++)
    {
        ui->tableWidget->insertRow(i);
        RoomListResponse_RoomInfo game = list->rooms(i);
        QTableWidgetItem *newItem;
        newItem = new QTableWidgetItem(QString::number(game.room_id()));
        ui->tableWidget->setItem(i, ROOM_ID, newItem);
        newItem = new QTableWidgetItem(QString::fromStdString(game.room_name()));
        ui->tableWidget->setItem(i, ROOM_NAME, newItem);
        newItem = new QTableWidgetItem(QString::number(game.now_player()) + "/" + QString::number(game.max_player()));
        ui->tableWidget->setItem(i, PLAYER_NUM, newItem);
    }
}

void Lobby::newWindow(int playerNum)
{
    RoomView* window = (RoomView*)gui->newWindow(playerNum);
    connect(window, SIGNAL(closed()), this, SLOT(onWindowClose()));
    setEnable(false);
}

void Lobby::setEnable(bool enable)
{
    ui->tableWidget->setEnabled(enable);
    ui->createButton->setEnabled(enable);
    ui->refreshButton->setEnabled(enable);
}

void Lobby::onItemClicked(QModelIndex index)
{
    ui->tableWidget->setEnabled(false);
    int row = index.row();
    RoomListResponse_RoomInfo room = roomList.rooms(row);
    newWindow(room.max_player());
    network::EnterRoomRequest *enter= new network::EnterRoomRequest();
    enter->set_room_id(room.room_id());
    logic->getClient()->sendMessage(network::MSG_ENTER_ROOM_REQ, enter);
}

void Lobby::onCreateRoom()
{
    //FIXME: popup
    newWindow(4);
    network::CreateRoomRequest* create = new network::CreateRoomRequest();
    create->set_role_strategy(ROLE_STRATEGY_ANY);
    create->set_max_player(4);
    logic->getClient()->sendMessage(network::MSG_CREATE_ROOM_REQ, create);
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

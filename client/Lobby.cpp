#include <QInputDialog>
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
    roomSet = NULL;
    ui->setupUi(this);
    setWindowTitle("Welcome back, " + dataInterface->nickName);
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

        QString roleNum = QStringLiteral("标准");
        if(game.first_extension())
            roleNum += QStringLiteral(",一扩");
        if(game.second_extension())
            roleNum += QStringLiteral(",三版");
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
    int row = index.row();
    RoomListResponse_RoomInfo room = roomList.rooms(row);
    network::EnterRoomRequest *enter= new network::EnterRoomRequest();
    if(room.has_password()){
        QString pwd = QInputDialog::getText(this, QStringLiteral("切，结界么？"),
                                    QStringLiteral("请输入暗语:"), QLineEdit::Normal);
        if(!pwd.isEmpty())
            enter->set_password(pwd.toStdString());
    }
    ui->tableWidget->setEnabled(false);

    newWindow(room.max_player());

    enter->set_room_id(room.room_id());
    logic->getClient()->sendMessage(network::MSG_ENTER_ROOM_REQ, enter);
}

void Lobby::onCreateRoom()
{
    //FIXME: popup
    //打开房间设置面板
    if(roomSet!= NULL){roomSet->close();}
    roomSet = new RoomSet();
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
    bool allowGuest = roomSet->getAllowGuest();
    std::string password = roomSet->getPassword().toStdString();
    std::string roomName = roomSet->getRoomName().toStdString();

    network::CreateRoomRequest* create = new network::CreateRoomRequest();
    create->set_role_strategy((network::ROLE_STRATEGY)roleSelectionStrategy);
    create->set_max_player(playerNum);
    create->set_seat_mode(seatOrder);
    create->set_first_extension(firstExtension);
    create->set_second_extension(secondExtension);
    create->set_allow_guest(allowGuest);
    create->set_password(password);
    create->set_room_name(roomName);

    logic->getClient()->sendMessage(network::MSG_CREATE_ROOM_REQ, create);
    roomSet->close();//注意此时调用onBackToLobby()
    newWindow(playerNum);//打开游戏界面
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

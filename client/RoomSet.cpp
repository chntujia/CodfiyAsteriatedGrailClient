#include "RoomSet.h"
#include "ui_RoomSet.h"
#include "QTime"
#include "protocol/action_respond.pb.h"

RoomSet::RoomSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoomSet)
{
    ui->setupUi(this);
    connect(ui->okButton, SIGNAL(clicked()), this, SIGNAL(createRoom()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(onCancelRoom()));
    connect(ui->allowPassword, SIGNAL(clicked(bool)), ui->password, SLOT(setEnabled(bool)));
    connect(ui->radioButtonPlayer6, SIGNAL(toggled(bool)), ui->radioButtonSeatOrderSanLian, SLOT(setEnabled(bool)));
    InitializeSet();
}

RoomSet::~RoomSet()
{
    delete ui;
}

void RoomSet::InitializeSet(){

    groupPlayerNum = new QButtonGroup();
    groupSeatOrder = new QButtonGroup();
    groupRoleSelection = new QButtonGroup();

    //初始化选项组
    groupPlayerNum->addButton(ui->radioButtonPlayer6);
    groupPlayerNum->addButton(ui->radioButtonPlayer4);

    groupSeatOrder->addButton(ui->radioButtonSeatOrderSuiji);
    groupSeatOrder->addButton(ui->radioButtonSeatOrderSanLian);
    groupSeatOrder->addButton(ui->radioButtonSeatOrderErLian);
    groupSeatOrder->addButton(ui->radioButtonSeatOrderJianGe);

    groupRoleSelection->addButton(ui->radioButtonRoleSelectionSanXuanYi);
    groupRoleSelection->addButton(ui->radioButtonRoleSelectionSuiJi);
    groupRoleSelection->addButton(ui->radioButtonRoleSelectionBanPick);
    //设置按钮id
    groupPlayerNum->setId(ui->radioButtonPlayer6,6);
    groupPlayerNum->setId(ui->radioButtonPlayer4,4);

    groupSeatOrder->setId(ui->radioButtonSeatOrderSuiji, network::SEAT_MODE_RANDOM);
    groupSeatOrder->setId(ui->radioButtonSeatOrderSanLian, network::SEAT_MODE_3COMBO);
    groupSeatOrder->setId(ui->radioButtonSeatOrderErLian, network::SEAT_MODE_2COMBO);
    groupSeatOrder->setId(ui->radioButtonSeatOrderJianGe, network::SEAT_MODE_INTERLACE);

    groupRoleSelection->setId(ui->radioButtonRoleSelectionSanXuanYi, network::ROLE_STRATEGY_31);
    groupRoleSelection->setId(ui->radioButtonRoleSelectionSuiJi, network::ROLE_STRATEGY_RANDOM);
    groupRoleSelection->setId(ui->radioButtonRoleSelectionBanPick, network::ROLE_STRATEGY_BP);

    //默认选项
    ui->radioButtonPlayer6->setChecked(true);
    ui->radioButtonSeatOrderSuiji->setChecked(true);
    ui->radioButtonRoleSelectionSanXuanYi->setChecked(true);

    QTime time = QTime::currentTime();
    int rid = time.second()%4;
    QString roomNames[4];
    roomNames[0] = "·Asteriated··Grail··Loading·";
    roomNames[1] = "初心者召唤中";
    roomNames[2] = "一起体验星杯传说的魅力吧！";
    roomNames[3] = "水叔是好人！";
    ui->lineEditRoomName->setText(roomNames[rid]);

    QRegExp rx("[A-Za-z0-9_]{1,10}");
    ui->password->setValidator(new QRegExpValidator(rx, this));
}

int RoomSet::getPlayerNum(){
     return groupPlayerNum->checkedId();
}

int RoomSet::getSeatOrder(){
    return groupSeatOrder->checkedId();
}

int RoomSet::getRoleSelection(){
    return groupRoleSelection->checkedId();
}

bool RoomSet::getFirstExtension() { return ui->checkBoxRoleRangeYiKuo->isChecked(); }

bool RoomSet::getSecondExtension() { return ui->checkBoxRoleRangeSanBan->isChecked(); }

bool RoomSet::getAllowGuest()
{
    return ui->allowGuest->isChecked();
}

QString RoomSet::getPassword()
{
    if(ui->allowPassword->isChecked()){
        return ui->password->text();
    }
    else{
        return "";
    }
}

QString RoomSet::getRoomName(){
    return ui->lineEditRoomName->text();
}

void RoomSet::onCancelRoom(){
    delete groupPlayerNum;
    delete groupSeatOrder;
    delete groupRoleSelection;
    this->close();
}
void RoomSet::closeEvent(QCloseEvent * event){

    emit  backToLobby();
}

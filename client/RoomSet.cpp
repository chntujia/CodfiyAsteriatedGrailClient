#include "RoomSet.h"
#include "ui_RoomSet.h"
#include "QTime"
RoomSet::RoomSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoomSet)
{
    ui->setupUi(this);
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(onCreateRoom()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(onCancelRoom()));
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

    groupSeatOrder->setId(ui->radioButtonSeatOrderSuiji,0);
    groupSeatOrder->setId(ui->radioButtonSeatOrderSanLian,1);
    groupSeatOrder->setId(ui->radioButtonSeatOrderErLian,2);
    groupSeatOrder->setId(ui->radioButtonSeatOrderJianGe,3);

    groupRoleSelection->setId(ui->radioButtonRoleSelectionSanXuanYi,2);
    groupRoleSelection->setId(ui->radioButtonRoleSelectionSuiJi,1);
    groupRoleSelection->setId(ui->radioButtonRoleSelectionBanPick,3);

    //默认选项
    ui->radioButtonPlayer6->setChecked(true);
    ui->radioButtonSeatOrderSuiji->setChecked(true);
    ui->radioButtonRoleSelectionSanXuanYi->setChecked(true);

    ui->checkBoxRoleRangeBiaozhun->setEnabled(false);
    ui->checkBoxRoleRangeBiaozhun->setChecked(true);


    QTime time = QTime::currentTime();
    int rid = time.second()%4;
    QString* roomNames = new QString[4];
    roomNames[0] = "·Asteriated··Grail··Loading·";
    roomNames[1] = "初心者召唤中";
    roomNames[2] = "一起体验星杯传说的魅力吧！";
    roomNames[3] = "水叔是好人！";
    ui->lineEditRoomName->setText(roomNames[rid]);

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

int RoomSet::getRoleRange(){
    int res = 0;
    if(ui->checkBoxRoleRangeYiKuo->isChecked()){
        res += 1;
    }
    if(ui->checkBoxRoleRangeSanBan->isChecked()){
        res += 2;
    }
    return res;
}

QString RoomSet::getRoomName(){
    return ui->lineEditRoomName->text();
}

void RoomSet::onCreateRoom(){
    emit createRoom();

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

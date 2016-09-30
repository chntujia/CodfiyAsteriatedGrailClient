#include "RoomSet.h"
#include "ui_RoomSet.h"
#include "QTime"
#include "protocol/action_respond.pb.h"
#include "data/Common.h"

RoomSet::RoomSet(ACCOUNT_STATUS identity, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoomSet)
{
    ui->setupUi(this);
    connect(ui->okButton, SIGNAL(clicked()), this, SIGNAL(createRoom()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(onCancelRoom()));
    InitializeSet(identity);
}

RoomSet::~RoomSet()
{
    delete ui;
}

void RoomSet::InitializeSet(ACCOUNT_STATUS identity){

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
    groupSeatOrder->addButton(ui->radioButtonSeatOrderRBBRRB);

    groupRoleSelection->addButton(ui->radioButtonRoleSelectionSanXuanYi);
    groupRoleSelection->addButton(ui->radioButtonRoleSelectionSuiJi);
    groupRoleSelection->addButton(ui->radioButtonRoleSelectionBanPick);
    groupRoleSelection->addButton(ui->radioButtonRoleSelectionCM);
    //设置按钮id
    groupPlayerNum->setId(ui->radioButtonPlayer6,6);
    groupPlayerNum->setId(ui->radioButtonPlayer4,4);

    groupSeatOrder->setId(ui->radioButtonSeatOrderSuiji, network::SEAT_MODE_RANDOM);
    groupSeatOrder->setId(ui->radioButtonSeatOrderSanLian, network::SEAT_MODE_3COMBO);
    groupSeatOrder->setId(ui->radioButtonSeatOrderErLian, network::SEAT_MODE_2COMBO);
    groupSeatOrder->setId(ui->radioButtonSeatOrderJianGe, network::SEAT_MODE_INTERLACE);
    groupSeatOrder->setId(ui->radioButtonSeatOrderRBBRRB, network::SEAT_MODE_RBBRRB);

    groupRoleSelection->setId(ui->radioButtonRoleSelectionSanXuanYi, network::ROLE_STRATEGY_31);
    groupRoleSelection->setId(ui->radioButtonRoleSelectionSuiJi, network::ROLE_STRATEGY_RANDOM);
    groupRoleSelection->setId(ui->radioButtonRoleSelectionBanPick, network::ROLE_STRATEGY_BP);
    groupRoleSelection->setId(ui->radioButtonRoleSelectionCM, network::ROLE_STRATEGY_CM);

    //默认选项
    ui->radioButtonPlayer6->setChecked(true);
    ui->radioButtonSeatOrderSuiji->setChecked(true);
    ui->radioButtonRoleSelectionSanXuanYi->setChecked(true);

    QTime time = QTime::currentTime();
    int rid = time.second()%7;
    QString roomNames[7];
    roomNames[0] = QStringLiteral("Github注册网杯账号现已开放");
    roomNames[1] = QStringLiteral("微博注册已开启！");
    roomNames[2] = QStringLiteral("我才不告诉你官网是codifygrail.cn！");
    roomNames[3] = QStringLiteral("数字将要废除，请及时注册");
    roomNames[4] = QStringLiteral("来吧！动画设计师大神！");
    roomNames[5] = QStringLiteral("开发进度在QQ群184705348");
    roomNames[6] = QStringLiteral("win10/8需兼容性设置8位色");
    ui->lineEditRoomName->setText(roomNames[rid]);

    QRegExp rx("[A-Za-z0-9_]{1,10}");
    ui->password->setValidator(new QRegExpValidator(rx, this));
    QRegExp rx2(".{1,15}");
    ui->lineEditRoomName->setValidator(new QRegExpValidator(rx2, this));

#ifdef DEBUG
    ui->allowGuest->setChecked(true);
    identity = STATUS_VIP;
#endif

    switch(identity)
    {
    case STATUS_VIP:
        ui->radioButtonRoleSelectionBanPick->setEnabled(true);
        ui->checkBoxRoleRangespMoDao->setEnabled(true);
        ui->radioButtonRoleSelectionCM->setEnabled(true);
    case STATUS_NORMAL:
        ui->allowGuest->setEnabled(true);
        ui->allowPassword->setEnabled(true);
        break;
    }
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

bool RoomSet::getspMoDao() { return ui->checkBoxRoleRangespMoDao->isChecked(); }

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

bool RoomSet::getSilence()
{
    return ui->slience->isChecked();
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

#ifndef LOBBY_H
#define LOBBY_H

#include <QDialog>
#include <QTableWidget>
#include <QMessageBox>
#include "protocol/base.pb.h"
#include "RoomSet.h"

#define ROOM_ID    0
#define ROOM_NAME  1
#define ROLE_NUM   2
#define ROLE_MODE  3
#define PLAYER_NUM 4
#define WELCOME_GUEST 5
#define HAS_PASSWORD 6
#define SEAT_MODE  7

namespace Ui {
class Lobby;
}

class Lobby : public QDialog
{
    Q_OBJECT

public:
    explicit Lobby(QWidget *parent = 0);
    ~Lobby();
    void fill(network::RoomListResponse *list);
    void setEnable(bool enable);
public slots:
    void onItemClicked(QModelIndex index);
    void onCreateRoom();
    void onRefreshList();
    void onWindowClose();
    void onOpenRoom();
    void onBackToLobby();
private:
    RoomSet *roomSet;
    void newWindow(int playerNum);
    QMap<int, network::RoomListResponse_RoomInfo> rooms;
    Ui::Lobby *ui;

};

#endif // LOBBY_H

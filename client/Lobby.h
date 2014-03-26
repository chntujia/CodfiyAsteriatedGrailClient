#ifndef LOBBY_H
#define LOBBY_H

#include <QDialog>
#include <QTableWidget>
#include <QMessageBox>
#include "protocol/base.pb.h"
#define ROOM_ID    0
#define ROOM_NAME  1
#define ROLE_MODE  2
#define SEAT_MODE  3
#define PLAYER_NUM 4
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
private:
    void newWindow(int playerNum);
    network::RoomListResponse roomList;
    Ui::Lobby *ui;
    QMessageBox* prop;
};

#endif // LOBBY_H

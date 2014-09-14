#ifndef ROOMSET_H
#define ROOMSET_H

#include <QDialog>
#include "QButtonGroup"

namespace Ui {
class RoomSet;
}

class RoomSet : public QDialog
{
    Q_OBJECT

public:
    explicit RoomSet(QWidget *parent = 0);
    ~RoomSet();
    int getPlayerNum();
    int getSeatOrder();
    int getRoleSelection();
    int getRoleRange();
    bool getAllowGuest();
    QString getPassword();
    QString getRoomName();
public slots:
    void onCancelRoom();

protected:
    void closeEvent(QCloseEvent * event);
signals:
    void createRoom();
    void backToLobby();
private:
    void InitializeSet();

    QButtonGroup* groupPlayerNum;
    QButtonGroup* groupSeatOrder;
    QButtonGroup* groupRoleSelection;
    Ui::RoomSet *ui;
};

#endif // ROOMSET_H

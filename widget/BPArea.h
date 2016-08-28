#ifndef BPAREA_H
#define BPAREA_H

#include <QGraphicsObject>
#include "RoleItem.h"

class BPArea : public QGraphicsObject
{
    Q_OBJECT
    friend class GUI;
public:
    BPArea();
    ~BPArea();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void BPStart(int num, SafeList<int> roles, SafeList<int> options, uint op);
    void CMStart(int num, SafeList<int> roles, SafeList<int> options, uint op);
    void enableRoleItem(int roleID);
    void disableRoleItem(int roleID);
    void setMsg(QString msg);
    void setQuota(int howMany){least=most=howMany;}
    void setQuota(int least,int most){this->least=least;this->most=most;}
    SafeList<int> getSelectedRoles();
    void choose(int playerID, int roleID);
    RoleItem *getRoleByID(int ID);
    void reset();
    bool checkOver();
    void remove(int roleID);
    void disablaAll();
    void enableAll();
    int getColor(int playerID);
    int getOrderInTeam(int playerID);
    void hide();
public slots:
    void onRoleSelected(int id);
    void onRoleUnselected(int id);
private:
    int width;
    int height;
    int least;
    int most;
    int currentSum;
    bool justShow;
    QPixmap background;
    QString msg;
    SafeList<int> selectedRoles;
    SafeList<RoleItem*> roleItems;
    SafeList<int> left;
    SafeList<int> playerIDs;
    SafeList<int> color;
    SafeList<int> orderInTeam;
signals:
    void roleReady();
    void roleUnready();
};

#endif // BPAREA_H

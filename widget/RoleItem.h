#ifndef ROLEITEM_H
#define ROLEITEM_H

#include <QGraphicsObject>
#include "data/Player.h"
#include <QPushButton>
#include <QPixmap>

class RoleItem : public QGraphicsObject
{
    Q_OBJECT
    friend class GUI;
public:
    RoleItem(int roleID);
    ~RoleItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    int getRoleID();
    bool isSelected();
    void setSelected(bool yes);
    void setBPMsg(int step);
    void setCMMsg(int op);
signals:
    void roleSelected(int);
    void roleUnselected(int);
private:
    int roleID;
    int width;
    int height;
    int bpFlag;
    bool selected;
    QPixmap pixmap;
    QPixmap player;
    QPixmap action;
};

#endif // ROLEITEM_H

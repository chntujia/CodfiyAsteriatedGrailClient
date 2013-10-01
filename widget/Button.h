#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsObject>

class Button : public QGraphicsObject
{
    Q_OBJECT
    friend class ButtonArea;
    friend class DecisionArea;
public:
    Button(int id,QString msg);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    bool checkSelected(){return selected;}
signals:
    void buttonSelected(int id);
    void buttonUnselected(int id);

private:
    int width;
    int height;
    bool selected;
    int id;
    QPixmap background;
    QString msg;

};
#endif // BUTTON_H

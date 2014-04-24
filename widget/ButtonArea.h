#ifndef BUTTONAREA_H
#define BUTTONAREA_H

#include <QGraphicsObject>
#include "data/SafeList.h"
#include "Button.h"

class DecisionArea : public QGraphicsObject
{
    Q_OBJECT
public:
    DecisionArea();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void enable(int i);
    void disable(int i);
    void reset();
signals:
    void okClicked();
    void cancelClicked();
    void exchangeClicked();
    void resignClicked();
public slots:
    void onButtonSelected(int id);

private:
    Button*okButton,*cancelButton,*exchangeButton,*resignButton;
};

class ButtonArea : public QGraphicsObject
{
    Q_OBJECT
    friend class GUI;
public:
    ButtonArea();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void init(bool playing);
    void clear();
    void enable(int i);
    void disable(int i);
    void reset();
    void addButton(Button* button);
    SafeList<Button*> getButtons();
    void setEnabled(bool enable);
    void addOutsideTurnButton(Button* button);
signals:
    void buttonUnselected();
public slots:
    void onButtonSelected(int id);
    void onButtonUnselected(int id);

private:
    SafeList<Button*> buttons;
    Button* selectedButton;
};

#endif // BUTTONAREA_H

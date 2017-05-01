#include "ButtonArea.h"
#include "data/DataInterface.h"
#include "widget/GUI.h"

DecisionArea::DecisionArea()
{
    okButton=new Button(0,QStringLiteral("确 定"));
    okButton->setParentItem(this);
    okButton->setPos(0,0);
    connect(okButton,SIGNAL(buttonSelected(int)),this,SLOT(onButtonSelected(int)));

    cancelButton=new Button(1,QStringLiteral("取 消"));
    cancelButton->setParentItem(this);
    cancelButton->setPos(80,0);
    connect(cancelButton,SIGNAL(buttonSelected(int)),this,SLOT(onButtonSelected(int)));

    exchangeButton=new Button(2,QStringLiteral("无法行动"));
    exchangeButton->setParentItem(this);
    exchangeButton->setPos(-80,0);
    exchangeButton->setOpacity(0);

    connect(exchangeButton,SIGNAL(buttonSelected(int)),this,SLOT(onButtonSelected(int)));

    resignButton=new Button(3,QStringLiteral("放弃行动"));
    resignButton->setParentItem(this);
    resignButton->setPos(-80,0);
    resignButton->setOpacity(0);
    connect(resignButton,SIGNAL(buttonSelected(int)),this,SLOT(onButtonSelected(int)));
}
QRectF DecisionArea::boundingRect() const
{
    return QRectF(0, 0, 160, 50);
}

void DecisionArea::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}
void DecisionArea::onButtonSelected(int id)
{
    switch(id)
    {
    case 0:
        okButton->selected=0;
        emit okClicked();
        break;
    case 1:
        cancelButton->selected=0;
        emit cancelClicked();
        break;
    case 2:
        exchangeButton->selected=0;
        emit exchangeClicked();
        break;
    case 3:
        resignButton->selected=0;
        emit resignClicked();
        break;
    }
}

void DecisionArea::enable(int i)
{
    switch(i)
    {
    case 0:
        okButton->setEnabled(1);
        okButton->selected=0;
        break;
    case 1:
        cancelButton->setEnabled(1);
        cancelButton->selected=0;
        break;
    case 2:
        exchangeButton->setOpacity(1);
        exchangeButton->setEnabled(1);
        exchangeButton->selected=0;
        break;
    case 3:
        resignButton->setOpacity(1);
        resignButton->setEnabled(1);
        resignButton->selected=0;
        break;
    }

}

void DecisionArea::disable(int i)
{
    switch(i)
    {
    case 0:
        okButton->setEnabled(0);
        okButton->selected=0;
        break;
    case 1:
        cancelButton->setEnabled(0);
        cancelButton->selected=0;
        break;
    case 2:
        exchangeButton->setOpacity(0);
        exchangeButton->setEnabled(0);
        exchangeButton->selected=0;
        break;
    case 3:
        resignButton->setOpacity(0);
        resignButton->setEnabled(0);
        resignButton->selected=0;
        break;
    }
}

void DecisionArea::reset()
{
    okButton->setEnabled(0);
    okButton->selected=0;

    cancelButton->setEnabled(0);
    cancelButton->selected=0;

    exchangeButton->setOpacity(0);
    exchangeButton->setEnabled(0);
    exchangeButton->selected=0;

    resignButton->setOpacity(0);
    resignButton->setEnabled(0);
    resignButton->selected=0;
}

ButtonArea::ButtonArea()
{
}

void ButtonArea::init(bool roleInited)
{
    Button *button;
    clear();
    if(roleInited){
        button = new Button(0, QStringLiteral("购买"));
        addButton(button);

        button = new Button(1, QStringLiteral("合成"));
        addButton(button);

        button = new Button(2, QStringLiteral("提炼"));
        addButton(button);
    }
    else{
        button = new Button(0, QStringLiteral("准备"));
        addButton(button);

        button = new Button(1, QStringLiteral("选队"));
        addButton(button);

        foreach(Button* b, buttons){
            connect(b, SIGNAL(buttonSelected(int)), logic, SLOT(onButtonClicked(int)));
            connect(b, SIGNAL(buttonUnselected(int)), logic, SLOT(onButtonUnclicked(int)));
        }
    } 
}

void ButtonArea::clear()
{
    foreach(Button* b, buttons)
    {
        delete b;
    }
    buttons.clear();
}

QRectF ButtonArea::boundingRect() const
{
    return QRectF(0, 0, 750, 150);
}

void ButtonArea::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}

void ButtonArea::onButtonSelected(int id)
{
    selectedButton=buttons[id];
    foreach(Button*ptr,buttons)
    {
        if(ptr!=selectedButton)
            ptr->selected=0;
    }
    update();
}

void ButtonArea::addButton(Button* button)
{
    button->setParentItem(this);
    button->setPos(82*button->id,0);
    buttons<<button;

    connect(button,SIGNAL(buttonSelected(int)),this,SLOT(onButtonSelected(int)));
    connect(button,SIGNAL(buttonUnselected(int)),this,SLOT(onButtonUnselected(int)));
}

void ButtonArea::addOutsideTurnButton(Button* button)
{
    button->setParentItem(this);
    button->setPos(80*(button->id - 10),-200);
}

void ButtonArea::onButtonUnselected(int id)
{
    emit buttonUnselected();
}

void ButtonArea::reset()
{
    int i;
    for(i=0;i<buttons.count();i++)
        disable(i);
}

void ButtonArea::setEnabled(bool enable)
{
    for(int i = 0;i < this->buttons.count();i++)
        buttons.at(i)->setEnabled(enable);
}

void ButtonArea::enable(int i)
{
    buttons[i]->setEnabled(1);
}

void ButtonArea::disable(int i)
{
    buttons[i]->selected=0;
    buttons[i]->setEnabled(0);
}

SafeList<Button*> ButtonArea::getButtons()
{
    return buttons;
}

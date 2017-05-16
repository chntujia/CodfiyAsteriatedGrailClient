#include "TipArea.h"
#include <QPainter>
#include <QFont>
#include "data/DataInterface.h"
#include "widget/GUI.h"
#include <QGraphicsGridLayout>
#define scaleFactor 0.8
TipArea::TipArea():selectedCard(NULL)
{
    background = QPixmap("resource/tip.png");
    height = background.height();
    width = background.width();
    setVisible(0);

    box = new QComboBox;
    box->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    box->setMinimumContentsLength(37);
    boxItem = new QGraphicsProxyWidget;
    boxItem->setWidget(box);
    boxItem->setParentItem(this);
    boxItem->setPos(0.1*width,0.6*height);
    boxItem->setVisible(false);
    winflag = false;
}
QRectF TipArea::boundingRect() const
{
    return QRectF(0, 0, width, height);
}

void TipArea::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QFont font;
    font.setPixelSize(20);
    painter->setFont(font);
    painter->setPen(QColor(Qt::white));
    if(!winflag)
        painter->drawPixmap(0, 0, background);
    else
        painter->drawPixmap(-50, -15, winpix);
    painter->drawText(QRectF(0.05*width, 0, 0.9*width, height*0.85),Qt::AlignCenter | Qt::TextWordWrap,msg);
}

void TipArea::reset()
{
    setVisible(0);
    box->clear();
    boxItem->setVisible(0);
    winflag = false;
    clearStatus();
}

void TipArea::addBoxItem(QString text)
{
    box->addItem(text);
}

void TipArea::showBox()
{
    setVisible(1);
    boxItem->setVisible(1);
    update();
}

void TipArea::setMsg(QString msg)
{
    this->msg = msg;
    setVisible(1);
    update();
}
void TipArea::clearStatus()
{
    if(!cardItems.empty())
    {
        foreach(CardItem* ptr, cardItems)
            delete ptr;
        cardItems.clear();
    }
    selectedCard=NULL;
}

void TipArea::showStatus(int id)
{
    Player* player = dataInterface->getPlayerList().at(id);
    SafeList<BasicStatus*> status = player->getBasicStatus();
    Card* card;
    int i;
    reset();
    msg="";
    for(i=0; i<status.size(); i++)
    {
        card = status[i]->getCard();
        CardItem* cardItem = new CardItem(card);
        cardItem->setScale(scaleFactor);
        cardItem->setParentItem(this);
        connect(cardItem, SIGNAL(cardSelected(int)), this, SLOT(onCardSelected(int)));
        connect(cardItem, SIGNAL(cardUnselected(int)), this, SLOT(onCardUnselected(int)));
        cardItems << cardItem;
    }
    adjustCards();
    setVisible(1);
    update();
}

void TipArea::onCardSelected(int id)
{
    if(selectedCard != NULL)
        foreach(CardItem *ptr, cardItems)
            if (ptr->getCard() == selectedCard)
            {
                ptr->reset();
                break;
            }
    selectedCard = dataInterface->getCard(id);
    gui->getDecisionArea()->enable(0);
}

void TipArea::onCardUnselected(int id)
{
    selectedCard = NULL;
    gui->getDecisionArea()->disable(0);
}

void TipArea::adjustCards()
{
    if(cardItems.isEmpty())
        return;
    int n = cardItems.size();
    int card_skip;
    int offset;
    int card_width = 100 * scaleFactor;

    if(n * card_width > width)
    {
        card_skip = (width - card_width) / (n - 1);
        offset = 0;
    }
    else
    {
        card_skip = card_width;
        offset = width * 0.5 - n * 0.5 * card_width;
    }

    for(int i = 0; i<n; i++)
    {
        cardItems[i]->setZValue(i);
        cardItems[i]->setPos(QPointF(offset + i * card_skip, 25));
    }
}

void TipArea::win(int team)
{
    if(team==1)
        winpix.load("resource/GameWinRed.png");
    else
        winpix.load("resource/GameWinBlue.png");
    winflag = true;
    setMsg("");
    setVisible(1);
    update();
}

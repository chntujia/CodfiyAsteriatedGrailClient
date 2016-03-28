#include "TeamArea.h"
#include "data/DataInterface.h"
#include <QPainter>
#include <QFont>
#include "Logic.h"
TeamArea::TeamArea()
{
    round=0;
    droppedCardNum=0;
    leftCardNum=150;
    red=dataInterface->getRedTeam();
    blue=dataInterface->getBlueTeam();
    background=QPixmap("resource/teamArea.png");

    Rgrail=QPixmap("resource/Rgrail.png");
    Bgrail=QPixmap("resource/Bgrail.png");
    Pcrystal=QPixmap("resource/crystal.png");
    Pgem=QPixmap("resource/gem.png");

    Num[0]=QPixmap("resource/number/0.png");
    Num[1]=QPixmap("resource/number/1.png");
    Num[2]=QPixmap("resource/number/2.png");
    Num[3]=QPixmap("resource/number/3.png");
    Num[4]=QPixmap("resource/number/4.png");
    Num[5]=QPixmap("resource/number/5.png");
    Num[6]=QPixmap("resource/number/6.png");
    Num[7]=QPixmap("resource/number/7.png");
    Num[8]=QPixmap("resource/number/8.png");
    Num[9]=QPixmap("resource/number/9.png");

    width=background.width();
    height=background.height();
}

QRectF TeamArea::boundingRect() const
{
    return QRectF(0, 0, width*1.5, height);
}

void TeamArea::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    int i;

    int offset=21;
    int morale=red->getMorale();
    morale=morale>0?morale:0;
    painter->drawPixmap(0, 0, background);

    offset=100;
    painter->drawPixmap(offset,4,Num[morale/10]);
    painter->drawPixmap(offset+Num[0].width(),4,Num[morale%10]);

    offset=146;
    for(i=0;i<red->getGrail();i++) //杯
        painter->drawPixmap((Rgrail.width()+4)*i+offset,4,Rgrail);

    offset=390;
    for(i=0; i<red->getCrystal(); i++) //水晶
        painter->drawPixmap(offset-(Pcrystal.width()+4)*i,4,Pcrystal);

    offset-=(Pcrystal.width()+4)*i;
    for(i=0; i<red->getGem(); i++) //宝石
        painter->drawPixmap(offset-(Pgem.width()+4)*i,4,Pgem);

    offset=422;
    painter->drawPixmap(offset,2,Num[round/10]);
    painter->drawPixmap(offset+18,2,Num[round%10]);

    offset=470;
    for(i=0;i<blue->getCrystal();i++) //水晶
        painter->drawPixmap((Pcrystal.width()+4)*i+offset,4,Pcrystal);

    offset+=(Pcrystal.width()+4)*i;
    for(i=0;i<blue->getGem();i++) //宝石
        painter->drawPixmap((Pgem.width()+4)*i+offset,4,Pgem);

    offset=715;
    for(i=0; i<blue->getGrail(); i++) //杯
        painter->drawPixmap(offset-(Rgrail.width()+4)*i,4,Bgrail);

    morale=blue->getMorale();
    morale=morale>0?morale:0;
    offset=742;
    painter->drawPixmap(offset,4,Num[morale/10]);
    painter->drawPixmap(offset+Num[0].width(),4,Num[morale%10]);

    QFont font;
    font.setBold(1);
    painter->setFont(font);
    painter->drawText(width+43,17,QString::number(leftCardNum));
    painter->drawText(width+110,17,QString::number(droppedCardNum));
    painter->drawText(width+174,17,QString::number(round));
    painter->drawText(width+235,17,QString::number(roomID));
}

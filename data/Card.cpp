#include "data/Card.h"
Card::Card(QStringList* cardEntry)
{
    this->id=cardEntry->at(0).toInt();
    this->type=cardEntry->at(1);
    this->element=cardEntry->at(2);
    this->property=cardEntry->at(3);
    this->name=cardEntry->at(4);
    this->source=cardEntry->at(5);
    this->description=cardEntry->at(6);
    this->hasSpeceiality=cardEntry->at(7).toInt();
    int i;
    this->specialityList.clear();
    for(i=0;i<this->hasSpeceiality;i++)
        this->specialityList.append(cardEntry->at(i+8));

}

int Card::getID()
{
    return this->id;
}
int Card::getHasSpeciality()
{
    return this->hasSpeceiality;
}

QString Card::getType()
{
    return this->type;
}

QString Card::getElement()
{
    return this->element;
}
QString Card::getChineseElement()
{
    if (element=="earth")
        return "地";
    else if (element=="fire")
        return "火";
    else if (element=="water")
        return "水";
    else if (element=="thunder")
        return "雷";
    else if (element=="wind")
        return "风";
    else if (element=="darkness")
        return "暗";
    else if (element=="light")
        return "光";

}

QString Card::getInfo()
{
    QString msg;
    if(type=="attack" && element!="darkness")
        msg=name+"-"+property;
    else if(type=="magic" && element!="light")
        msg=name+"-"+getChineseElement();
    else if(element=="darkness")
        msg=name;
    else if(element=="light")
        msg=name+"-"+property;
    return msg;
}

QString Card::getProperty()
{
    return this->property;
}

QString Card::getName()
{
    return this->name;
}

QString Card::getSource()
{
    return this->source;
}

QStringList Card::getSpecialityList()
{
    return this->specialityList;
}

#include "AnSha.h"

AnSha::AnSha()
{
    makeConnection();
setMyRole(this);
}

void AnSha::ShuiYing()
{
    state=502;
    tipArea->setMsg(QStringLiteral("是否发动水影？"));
    handArea->setQuota(1,7);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableElement("water");
}

void AnSha::QianXing()
{
    state=503;
    gui->reset();
    tipArea->setMsg(QStringLiteral("是否发动潜行？"));
    QList<Card*> handcards=dataInterface->getHandCards();
    bool flag=true;
    int i;
    int n=handcards.size();
    decisionArea->enable(1);
    if(n<4)
    {
        flag=false;
        for(i=0;i<n;i++)
            if(handcards[i]->getElement()!="light")
            {
                flag=true;
                break;
            }
    }
    if(flag)
        decisionArea->enable(0);
}

void AnSha::askForSkill(QString skill)
{
    Role::askForSkill(skill);
    if(skill==QStringLiteral("水影"))
        ShuiYing();
    else if(skill==QStringLiteral("潜行"))
        QianXing();
}

void AnSha::cardAnalyse()
{
    Role::cardAnalyse();

    switch (state)
    {
    //水影询问
    case 502:
        decisionArea->enable(0);
        break;
    }
}

void AnSha::onOkClicked()
{
    Role::onOkClicked();
    QList<Card*>selectedCards;
    int howMany,i;
    QString command;

    selectedCards=handArea->getSelectedCards();
    switch(state)
    {
    //水影询问
    case 502:
        command="502;";
        howMany=selectedCards.size();
        command+=QString::number(howMany)+";";
        for(i=0;i<howMany-1;i++)
        {
            command+=QString::number(selectedCards[i]->getID())+",";
            dataInterface->removeHandCard(selectedCards[i]);
        }
        command+=QString::number(selectedCards[i]->getID())+";";
        dataInterface->removeHandCard(selectedCards[i]);
        emit sendCommand(command);
        gui->reset();
        break;
    //潜行询问
    case 503:
        command="503;1;";
        start=true;
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void AnSha::onCancelClicked()
{
    Role::onCancelClicked();
    QList<Card*>selectedCards;
    QString command;

    selectedCards=handArea->getSelectedCards();
    switch(state)
    {
    //水影询问
    case 502:
        command="502;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    //潜行询问
    case 503:
        command="503;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    }
}


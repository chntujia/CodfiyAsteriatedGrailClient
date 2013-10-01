#include "data/Team.h"

Team::Team(bool isRed,int moraleMax):grail(0),crystal(0),gem(0)
{
    this->moraleMax=moraleMax;
    morale=moraleMax;
    this->isRed=isRed;
}
void Team::setGrail(int howMany)
{
    grail=howMany;
}

void Team::setCrystal(int howMany)
{
    crystal=howMany;
}

void Team::setGem(int howMany)
{
    gem=howMany;
}

void Team::setMorale(int howMany)
{
    morale=howMany;
}

int Team::getGrail()
{
    return grail;
}

int Team::getCrystal()
{
    return crystal;
}

int Team::getGem()
{
    return gem;
}

int Team::getMorale()
{
    return morale;
}

int Team::getMoraleMax()
{
    return moraleMax;
}

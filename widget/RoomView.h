#ifndef ROOMVIEW_H
#define ROOMVIEW_H

#include <QGraphicsView>
#include "RoomScene.h"
#include <QPixmap>

class RoomView : public QGraphicsView
{
    Q_OBJECT

public:
    RoomView();
    RoomScene* getScene(){return scene;}
private:
    RoomScene *scene;
};

#endif // ROOMVIEW_H

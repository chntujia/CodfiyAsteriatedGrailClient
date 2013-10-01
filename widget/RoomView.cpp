#include "widget/RoomView.h"
#define gWidth 1280*0.9
#define gHeight 800*0.85
RoomView::RoomView()
{
    setFixedSize(QSize(gWidth+5, gHeight+5));

    scene = new RoomScene;
    setScene(scene);
    setSceneRect(0, 0, gWidth, gHeight);
    move(QPoint(0,0));
    setCacheMode(CacheBackground);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setWindowTitle("Codify");
}

#ifndef LOBBY_H
#define LOBBY_H

#include <QDialog>

namespace Ui {
class Lobby;
}

class Lobby : public QDialog
{
    Q_OBJECT

public:
    explicit Lobby(QWidget *parent = 0);
    ~Lobby();

private:
    Ui::Lobby *ui;
};

#endif // LOBBY_H

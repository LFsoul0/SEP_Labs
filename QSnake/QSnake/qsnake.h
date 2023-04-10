#ifndef QSNAKE_H
#define QSNAKE_H

#include <QWidget>
#include <QSound>

#include "mainmenu.h"
#include "gamescene.h"

class QSnake : public QWidget
{
    Q_OBJECT
public:
    explicit QSnake(QWidget *parent = nullptr);
    ~QSnake();

    void keyPressEvent(QKeyEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;

private:
    MainMenu *mainMenu;
    GameScene *gameSence;

    QSoundEffect *bgm;

    void manageConnections();

};
#endif // QSNAKE_H

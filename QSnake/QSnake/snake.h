#ifndef SNAKE_H
#define SNAKE_H

#include <QWidget>
#include <QLabel>
#include <QDebug>

class Snake : public QWidget
{
    Q_OBJECT
public:
    explicit Snake(QPoint spawnPoint, QPoint stepTowards, QWidget *parent = nullptr, int player_ = 1, bool isAI_ = false);
    Snake(QDataStream &sav, QWidget *parent = nullptr);
    ~Snake();

    void save(QDataStream &sav);

    int getPlayer();
    bool isAuto();
    int getLife();
    void addLife();
    int getLength();
    QPoint getHead();
    QPoint getTail();
    QPoint getDirecton();

    QPoint moveStep();
    bool turn(QPoint direct);
    void grow();
    QVector<QPoint>& reborn();
    QPoint bonus();

private:
    int player;
    bool isAI;
    int life;

    QVector<QLabel *> body;
    int len;

    QPoint step;
    QPoint neck;
    QPoint lastHead;
    QPoint lastNeck;
    QVector<QPoint> trail;

signals:

};

#endif // SNAKE_H

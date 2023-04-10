#include "snake.h"

Snake::Snake(QPoint spawnPoint, QPoint stepTowards, QWidget *parent, int player_, bool isAI_) : QWidget(parent)
{
    setFixedSize(parent->size());

    player = player_;
    isAI = isAI_;

    body.append(new QLabel(this));
    body.at(0)->setFixedSize(30, 30);
    body.at(0)->move(spawnPoint);
    body.at(0)->setStyleSheet(QString("border-image: url(:/art/sprite/snakeHeadRight%1.png)").arg(player));

    len = 1;
    life = 0;

    step = stepTowards;
    neck = QPoint(-30, 0);
    lastHead = spawnPoint;
    lastNeck = neck;

    turn(step);
    show();
}

Snake::Snake(QDataStream &sav, QWidget *parent) : QWidget(parent)
{
    setFixedSize(parent->size());

    sav >> player >> isAI >> life;
    sav >> step >> neck;

    sav >> len;
    QPoint tmp;
    for (int i = 0; i < len; i++) {
        body.append(new QLabel(this));
        body.at(i)->setFixedSize(30, 30);
        if (i % 2 == 0) {
            body.at(i)->setStyleSheet(QString("border-image: url(:/art/sprite/snakeBody%1.png)").arg(player));
        }
        else {
            body.at(i)->setStyleSheet(QString("border-image: url(:/art/sprite/snakeBody0.png)"));
        }
        sav >> tmp;
        body.at(i)->move(tmp);
    }
    turn(step);

    int tlen;
    sav >> tlen;
    for (int i = 0; i < tlen; i++) {
        sav >> tmp;
        trail.append(tmp);
    }

    show();
}

Snake::~Snake()
{

}

void Snake::save(QDataStream &sav)
{
    sav << player << isAI << life;
    sav << step << neck;
    sav << body.size();
    for (QLabel *n : body) {
        sav << n->pos();
    }
    sav << trail.size();
    for (QPoint p : trail) {
        sav << p;
    }
}


int Snake::getPlayer()
{
    return player;
}

bool Snake::isAuto()
{
    return isAI;
}

int Snake::getLife()
{
    return life;
}

void Snake::addLife()
{
    ++life;
}

int Snake::getLength()
{
    return len;
}

QPoint Snake::getHead()
{
    return body.at(0)->pos();
}

QPoint Snake::getTail()
{
    return body.at(body.size() - 1)->pos();
}

QPoint Snake::getDirecton()
{
    return step;
}

QPoint Snake::moveStep()
{
    lastHead = body[0]->pos();
    lastNeck = neck;

    QPoint tmp = body.at(0)->pos() + step;
    if (tmp.x() < 250)
        tmp.setX(tmp.x() + 1350);
    else if (tmp.x() >= 1600)
        tmp.setX(tmp.x() - 1350);
    else if (tmp.y() < 0)
        tmp.setY(tmp.y() + 900);
    else if (tmp.y() >= 900)
        tmp.setY(tmp.y() - 900);

    trail.clear();
    trail.append(body.at(len - 1)->pos());

    for (int i = len - 1; i > 0; i--) {
        body.at(i)->move(body.at(i - 1)->pos());
    }

    body.at(0)->move(tmp);

    neck = -step;

    return tmp;
}

bool Snake::turn(QPoint direct)
{
    if (direct != neck || len == 1) {
        step = direct;

        if (direct.x() > 0) {
            body.at(0)->setStyleSheet(QString("border-image: url(:/art/sprite/snakeHeadRight%1.png)").arg(player));
        }
        else if (direct.x() < 0) {
            body.at(0)->setStyleSheet(QString("border-image: url(:/art/sprite/snakeHeadLeft%1.png)").arg(player));
        }
        else if (direct.y() > 0) {
            body.at(0)->setStyleSheet(QString("border-image: url(:/art/sprite/snakeHeadDown%1.png)").arg(player));
        }
        else if (direct.y() < 0) {
            body.at(0)->setStyleSheet(QString("border-image: url(:/art/sprite/snakeHeadUp%1.png)").arg(player));
        }

        return true;
    }
    else {
        return false;
    }

}

void Snake::grow()
{
    body.append(new QLabel(this));
    body.at(len)->setFixedSize(body.at(0)->size());
    if (len % 2 == 0) {
        body.at(len)->setStyleSheet(QString("border-image: url(:/art/sprite/snakeBody%1.png)").arg(player));
    }
    else {
        body.at(len)->setStyleSheet(QString("border-image: url(:/art/sprite/snakeBody0.png)"));
    }
    body.at(len)->move(trail.at(0));
    body.at(len)->show();

    len++;
}

QVector<QPoint>& Snake::reborn()
{
    life--;

    trail.clear();

    body.at(0)->move(lastHead);
    for (int i = 1; i < len; i++) {
        trail.append(body.at(i)->pos());
        body.at(i)->move(lastHead);
    }

    body.at(0)->raise();

    neck = -lastNeck;
    turn(lastNeck);

    return trail;
}

QPoint Snake::bonus()
{
    for (int i = 0; i < 20; i++) {
        body.append(new QLabel(this));
        body.at(len)->setFixedSize(body.at(0)->size());
        if (len % 2 == 0) {
            body.at(len)->setStyleSheet(QString("border-image: url(:/art/sprite/snakeBody%1.png)").arg(player));
        }
        else {
            body.at(len)->setStyleSheet(QString("border-image: url(:/art/sprite/snakeBody0.png)"));
        }
        body.at(len)->move(trail.at(0));
        body.at(len)->show();

        len++;
    }

    return trail.at(0);
}

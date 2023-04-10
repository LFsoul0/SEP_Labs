#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>
#include <QSoundEffect>

struct BtnInitialList
{
    QWidget *parent;
    QSize size;
    QPoint pos;
    QPixmap iconNormal;
    QPixmap iconPressed;
    QString text;
    QFont font;
    QString style;

    bool stateChangeable = false;
    QPixmap iconFalseNormal;
    QPixmap iconFalsePressed;

    bool defaultState = true;
};

class PushButton : public QPushButton
{
    Q_OBJECT
public:
    static QSoundEffect *sound;

    explicit PushButton(QWidget *parent = nullptr);
    PushButton(BtnInitialList &bil);

    void setIconNormal(QPixmap newIcon);
    void setIconPressed(QPixmap newIcon);
    void setState(bool newState);
    bool getState();

    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;

private:
    bool state;

    QLabel *label;

    QPixmap iconNormal;
    QPixmap iconPressed;
    QPixmap iconFalseNormal;
    QPixmap iconFalsePressed;

    static QSoundEffect *setSound();

signals:
    void stateChange(bool newState);

};


#endif // PUSHBUTTON_H

#include "pushbutton.h"

QSoundEffect *PushButton::sound = PushButton::setSound();

PushButton::PushButton(QWidget *parent) : QPushButton(parent)
{

}

PushButton::PushButton(BtnInitialList &bil) : QPushButton(bil.parent)
{
    state = bil.defaultState;

    iconNormal = bil.iconNormal;
    iconPressed = bil.iconPressed;
    if (bil.stateChangeable) {
        iconFalseNormal = bil.iconFalseNormal;
        iconFalsePressed = bil.iconFalsePressed;
    }
    else {
        iconFalseNormal = bil.iconNormal;
        iconFalsePressed = bil.iconPressed;
    }

    setFixedSize(bil.size);
    move(bil.pos);
    setStyleSheet("QPushButton{ border: 0px; }");
    setIconSize(QSize(bil.size.width(), bil.iconNormal.height() / bil.size.width() * bil.iconNormal.width()));
    if (state) {
        setIcon(iconNormal);
    }
    else {
        setIcon(iconFalseNormal);
    }

    label = new QLabel(this);
    label->setFixedSize(bil.size);
    label->setText(bil.text);
    label->setFont(bil.font);
    label->setStyleSheet(bil.style);
    label->setAlignment(Qt::AlignCenter);
    label->setAttribute(Qt::WA_TransparentForMouseEvents);

    connect(this, &QPushButton::clicked, this, [=](){ setState(!state); });

}

QSoundEffect *PushButton::setSound()
{
    QSoundEffect *s = new QSoundEffect;
    s->setSource(QUrl::fromLocalFile(":/audio/sound/button.wav"));
    s->setLoopCount(1);
    s->setVolume(1);

    return s;
}

void PushButton::setIconNormal(QPixmap newIcon)
{
    iconNormal = newIcon;
}

void PushButton::setIconPressed(QPixmap newIcon)
{
    iconPressed = newIcon;
}

void PushButton::setState(bool newState)
{
    state = newState;
    emit stateChange(state);

    if (state) {
        setIcon(iconNormal);
    }
    else {
        setIcon(iconFalseNormal);
    }
}

void PushButton::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton) {
        if (state) {
            setIcon(iconPressed);
        }
        else {
            setIcon(iconFalsePressed);
        }
    }

    sound->play();

    return QPushButton::mousePressEvent(ev);
}

void PushButton::mouseReleaseEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton) {
        if (state) {
            setIcon(iconNormal);
        }
        else {
            setIcon(iconFalseNormal);
        }
    }

    return QPushButton::mouseReleaseEvent(ev);
}


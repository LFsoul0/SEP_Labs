#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QDebug>
#include <QString>
#include <QPainter>
#include <QPixmap>
#include <QLabel>
#include <QSlider>
#include <QToolButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QCheckBox>
#include <QLineEdit>
#include <QLayout>
#include <QFile>
#include <QDir>

#include "pushbutton.h"


enum Mode
{
    Challenge,
    Single,
    Multiplayer,
    Load,
    Edit
};


class MainMenu : public QWidget
{
    Q_OBJECT
public:
    explicit MainMenu(QWidget *parent = nullptr);

    void readMapFiles(bool withDefault = true);
    void readHighScores();

    void paintEvent(QPaintEvent *ev) override;
    void show();

private:
    ///---root menu set---///
    QWidget *root;
    //first menu
    QWidget *firstMenu;
    PushButton *conti;
    PushButton *start;
    PushButton *mapEdit;
    PushButton *set;
    PushButton *exit;
    //second menu
    QWidget *secondMenu;
    PushButton *challange;
    PushButton *single;
    PushButton *multi;
    PushButton *startRootBack;

    ///---select menu---///
    QWidget *selectMenu;
    QLabel *mapSelect;
    QScrollArea *mapWidget;
    QCheckBox *snakeAI_1;
    QCheckBox *snakeAI_2;
    PushButton *newMap;
    PushButton *selectBack;

    ///---input dialog box---///
    QWidget *inputDialog;
    QLabel *dialogBox;
    QLineEdit *lineEditor;
    PushButton *yesBtn;
    PushButton *noBtn;

    ///---settings menu---///
    QWidget *setMenu;
    PushButton *setBack;
    PushButton *scoreClear;
    PushButton *music;
    PushButton *sound;
    QSlider *musicVol;
    QSlider *soundVol;

    ///---high score list---///
    PushButton *scoreList;
    QLabel *listBase;
    QLabel *listTitle;
    QLabel *records[11];

    QDir mapDir;

    void creatRootMenu();
    void creatSelectMenu();
    void creatInputDialog();
    void creatSettingsMenu();
    void creatHighScoreList();
    void manageConnections();

    void singleStart(QString mapName);
    void multiStart(QString mapName);
    void editStart(QString mapName);

signals:
    void quit();
    void enterGame(Mode mode, QString mapName = "", bool ai1 = false, bool ai2 = false);
    void mapSelected(QString mapName);
    void musicMuted(bool muted);
    void soundMuted(bool muted);
    void musicVolChanged(float vol);
    void soundVolChanged(float vol);

};

#endif // MAINMENU_H

#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QRandomGenerator>
#include <QToolButton>
#include <QLineEdit>
#include <QFile>
#include <QDataStream>
#include <QDir>
#include <QSoundEffect>

#include "pushbutton.h"
#include "snake.h"

enum MapItem
{
    Snake1,
    Snake2,
    Grass,
    Wall,
    Apple,
    Donuts,
    BlueBerry,
    Turkey
};



class GameScene : public QWidget
{
    Q_OBJECT
public:
    QSoundEffect *gameSound;

    explicit GameScene(QWidget *parent = nullptr);

    void runChallenge();
    void runSingle(QString mapName, bool ai = false);
    void runMultiplayer(QString mapName, bool ai1 = false, bool ai2 = false);
    void load();

    void edit(QString mapName);

    void hide();

    void paintEvent(QPaintEvent *ev) override;
    void keyPressEvent(QKeyEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;

protected:
    struct Point
    {
        int x;
        int y;
        Point(int rx = 0, int ry = 0): x(rx), y(ry) { }
        Point operator+(Point other) { return Point((x + other.x + 45) % 45, (y + other.y + 30) % 30); }
        bool operator==(Point other) { return (x == other.x) && (y == other.y); }
        bool operator!=(Point other) { return (x != other.x) || (y != other.y); }
    };

private:
    bool inGame;
    bool endFlag[2];
    bool editFlag;
    QString mapPath;
    MapItem editItem;
    int rank;

    QTimer *timer;
    QTimer *bulletTime;
    QRandomGenerator randGen;

    MapItem grid[45][30];
    QLabel *linkBoard[45][30];
    QVector<QPoint> blank;
    Snake *s1;
    Snake *s2;

    ///---score board---///
    QWidget *scoreBoard;
    QLabel *score[2];
    QLabel *pauseHint;

    ///---edit board---///
    QWidget *editBoard;
    QLabel *editHint;
    PushButton *editBtns[8];
    PushButton *editSave;
    PushButton *editQuit;
    PushButton *editBack;

    ///---pause menu---///
    QWidget *pauseMenu;
    QLabel *pauseLabel;
    QLabel *helpTitle[7];
    QLabel *helpIcon[7];
    QLabel *helpText[7];
    PushButton *conti;
    PushButton *mapEdit;
    PushButton *save;
    PushButton *exit;

    ///---end menu---///
    QWidget *endMenu;
    QLabel *endBase;
    PushButton *endQuit;

    QWidget *resultTab;
    QLabel *result;
    QLabel *detail[2];

    QWidget *listTab;
    QLabel *listTitle;
    QLabel *inputHint;
    QLabel *endList[12];
    QLineEdit *nameInput;


    MapItem playerToMapItem(int player_num);

    void creatPauseMenu();
    void creatScoreBoard();
    void creatEditBoard();
    void creatEndMenu();

    QLabel *newWall(QPoint p);
    QLabel *newApple(QPoint p);
    QLabel *newDonuts(QPoint p);
    QLabel *newBlueberry(QPoint p);
    QLabel *newTurkey(QPoint p);

    void newFood();
    void gameUpdate();
    bool moveSnake(Snake *s);
    void getDemage(Snake *s);
    void gameEnd();

    void autoSnake(Snake *s);
    int distance(Point origin, Point &phase);
    Point findWay(bool map[45][30], bool mark[45][30], Point start, Point dest);

    void editSelect(MapItem item);
    void runtimeEdit();

    void gameSave();
    void mapSave();
    void highScoreSave();

signals:
    void quit();
    void highScoreUpdated();
    void gameWin();
    void gameLose();

};

#endif // GAMESCENE_H

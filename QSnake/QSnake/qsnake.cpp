#include "qsnake.h"

QSnake::QSnake(QWidget *parent) : QWidget(parent)
{
    ///---set window---///
    setWindowIcon(QIcon(":/art/icon/qsnake.png"));
    setWindowTitle("QSnake");
    setFixedSize(1600, 900);
    setFocusPolicy(Qt::StrongFocus);

    ///---creat scenes---///
    mainMenu = new MainMenu(this);
    gameSence = new GameScene(this);
    gameSence->hide();

    ///---background music---///
    bgm = new QSoundEffect;
    bgm->setSource(QUrl::fromLocalFile(":/audio/music/inMenu.wav"));
    bgm->setLoopCount(QSoundEffect::Infinite);
    bgm->setVolume(1);
    bgm->play();

    ///---manage connections---///
    manageConnections();

}

void QSnake::manageConnections()
{
    //quit from main menu
    connect(mainMenu, &MainMenu::quit, this, &QWidget::close);

    //quit from game menu
    connect(gameSence, &GameScene::quit, this, [=]()
    {
        gameSence->hide();
        gameSence->clearFocus();
        mainMenu->show();

        bgm->stop();
        bgm->setSource(QUrl::fromLocalFile(":/audio/music/inMenu.wav"));
        bgm->setLoopCount(QSoundEffect::Infinite);
        bgm->play();
    });

    //enter game sence
    connect(mainMenu, &MainMenu::enterGame, this, [=](Mode mode, QString mapName, bool ai1, bool ai2)
    {
        mainMenu->hide();
        mainMenu->clearFocus();
        gameSence->show();

        bgm->stop();
        if (mode == Edit) {
            bgm->setSource(QUrl::fromLocalFile(":/audio/music/inEdit.wav"));
        }
        else {
            bgm->setSource(QUrl::fromLocalFile(":/audio/music/inGame.wav"));
        }
        bgm->setLoopCount(QSoundEffect::Infinite);
        bgm->play();

        switch(mode) {
        case Mode::Challenge:
            gameSence->runChallenge();
            break;
        case Mode::Single:
            gameSence->runSingle(mapName, ai1);
            break;
        case Mode::Multiplayer:
            gameSence->runMultiplayer(mapName, ai1, ai2);
            break;
        case Mode::Load:
            gameSence->load();
            break;
        case Mode::Edit:
            gameSence->edit(mapName);
            break;
        }
    });

    //high scores update
    connect(gameSence, &GameScene::highScoreUpdated, mainMenu, &MainMenu::readHighScores);

    //game sound
    connect(gameSence, &GameScene::gameWin, this, [=]()
    {
        bgm->stop();
        bgm->setSource(QUrl::fromLocalFile(":/audio/music/win.wav"));
        bgm->setLoopCount(1);
        bgm->play();
    });

    connect(gameSence, &GameScene::gameLose, this, [=]()
    {
        bgm->stop();
        bgm->setSource(QUrl::fromLocalFile(":/audio/music/lose.wav"));
        bgm->setLoopCount(1);
        bgm->play();
    });

    //volume control
    connect(mainMenu, &MainMenu::musicMuted, bgm, &QSoundEffect::setMuted);

    connect(mainMenu, &MainMenu::soundMuted, this, [=](bool muted)
    {
        gameSence->gameSound->setMuted(muted);
        PushButton::sound->setMuted(muted);
    });

    connect(mainMenu, &MainMenu::musicVolChanged, bgm, &QSoundEffect::setVolume);

    connect(mainMenu, &MainMenu::soundVolChanged, this, [=](float vol)
    {
        gameSence->gameSound->setVolume(vol);
        PushButton::sound->setVolume(vol);
    });
}

QSnake::~QSnake()
{

}

void QSnake::keyPressEvent(QKeyEvent *ev)
{
    gameSence->keyPressEvent(ev);
}

void QSnake::mousePressEvent(QMouseEvent *ev)
{
    gameSence->mousePressEvent(ev);
}

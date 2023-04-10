#include "gamescene.h"

GameScene::GameScene(QWidget *parent) : QWidget(parent)
{
    ///---set fundamental properties---///
    setFixedSize(parent->size());
    setFocusPolicy(Qt::StrongFocus);

    ///---set timers---///
    timer = new QTimer(this);
    bulletTime = new QTimer(this);

    connect(timer, &QTimer::timeout, this, &GameScene::gameUpdate);
    connect(bulletTime, &QTimer::timeout, this, [=]()
    {
        timer->setInterval(200);
        bulletTime->stop();
    });

    ///---set random engine---///
    randGen = QRandomGenerator::securelySeeded();

    ///---set null pointers---///
    s1 = nullptr;
    s2 = nullptr;
    for (int i = 0; i < 45; i++) {
        for (int j = 0; j < 30; j++) {
            linkBoard[i][j] = nullptr;
        }
    }

    ///---set initial values---///
    inGame = false;
    endFlag[0] = false;
    endFlag[1] = false;
    editFlag = false;
    mapPath = "";
    editItem = Grass;

    ///---sub widgets---///
    creatPauseMenu();
    creatScoreBoard();
    creatEditBoard();
    creatEndMenu();

    ///---visiablity control---///
    pauseMenu->hide();
    scoreBoard->hide();
    editBoard->hide();

    ///---set sound effect---///
    gameSound = new QSoundEffect;
    gameSound->setLoopCount(1);
    gameSound->setVolume(1);

}

void GameScene::creatPauseMenu()
{
    /// Summary:
    /// pause menu contains map edit, continue, save and quit button
    /// included in a widget
    /// present once [ESC] is pressed while playing
    /// (total: 16 labels, 4 buttons)

    pauseMenu = new QWidget(this);

    //set bil
    BtnInitialList bil;
    bil.parent = pauseMenu;
    bil.size = QSize(100, 100);
    bil.pos.setY(400);
    bil.font.setFamily("Comic Sans MS");
    bil.font.setBold(true);
    bil.font.setPixelSize(30);
    bil.font.setLetterSpacing(QFont::AbsoluteSpacing, 2);
    bil.style = "color: white";

    //continue button
    bil.pos.setX(725);
    bil.iconNormal = QPixmap(":/art/ui/startNormal.png");
    bil.iconPressed = QPixmap(":/art/ui/startPressed.png");
    conti = new PushButton(bil);

    connect(conti, &QPushButton::clicked, this, [=]()
    {
        scoreBoard->show();
        pauseMenu->hide();
        timer->start();
    });

    //save button
    bil.pos.setX(875);
    bil.iconNormal = QPixmap(":/art/ui/saveNormal.png");
    bil.iconPressed = QPixmap(":/art/ui/savePressed.png");
    save = new PushButton(bil);

    connect(save, &QPushButton::clicked, this, &GameScene::gameSave);

    //exit button
    bil.pos.setX(1025);
    bil.iconNormal = QPixmap(":/art/ui/exitNormal.png");
    bil.iconPressed = QPixmap(":/art/ui/exitPressed.png");
    exit = new PushButton(bil);

    connect(exit, &QPushButton::clicked, this, &GameScene::quit);

    //map edit button
    bil.size = QSize(270, 85);
    bil.pos = QPoint(790, 550);
    bil.iconNormal = QPixmap(":/art/ui/menuBtnNormal.png");
    bil.iconPressed = QPixmap(":/art/ui/menuBtnPressed.png");
    bil.text = "Edit Map";
    mapEdit = new PushButton(bil);

    connect(mapEdit, &QPushButton::clicked, this, &GameScene::runtimeEdit);

    //pause label
    pauseLabel = new QLabel(pauseMenu);
    pauseLabel->setFixedSize(200, 100);
    pauseLabel->move(825, 250);
    bil.font.setPixelSize(60);
    pauseLabel->setFont(bil.font);
    pauseLabel->setStyleSheet("color: black;");
    pauseLabel->setText("Pause");
    pauseLabel->setAlignment(Qt::AlignCenter);

    //help title
    bil.font.setPixelSize(34);
    helpTitle[0] = new QLabel(pauseMenu);
    helpTitle[0]->setFixedSize(200, 50);
    helpTitle[0]->move(50, 0);
    helpTitle[0]->setFont(bil.font);
    helpTitle[0]->setStyleSheet("color: white;");
    helpTitle[0]->setText("Player1:");
    helpTitle[0]->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    helpTitle[1] = new QLabel(pauseMenu);
    helpTitle[1]->setFixedSize(200, 50);
    helpTitle[1]->move(50, 110);
    helpTitle[1]->setFont(bil.font);
    helpTitle[1]->setStyleSheet("color: white;");
    helpTitle[1]->setText("Player2:");
    helpTitle[1]->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    helpTitle[2] = new QLabel(pauseMenu);
    helpTitle[2]->setFixedSize(200, 50);
    helpTitle[2]->move(50, 220);
    helpTitle[2]->setFont(bil.font);
    helpTitle[2]->setStyleSheet("color: white;");
    helpTitle[2]->setText("Wall:");
    helpTitle[2]->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    helpTitle[3] = new QLabel(pauseMenu);
    helpTitle[3]->setFixedSize(200, 50);
    helpTitle[3]->move(50, 370);
    helpTitle[3]->setFont(bil.font);
    helpTitle[3]->setStyleSheet("color: white;");
    helpTitle[3]->setText("Apple:");
    helpTitle[3]->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    helpTitle[4] = new QLabel(pauseMenu);
    helpTitle[4]->setFixedSize(200, 50);
    helpTitle[4]->move(50, 520);
    helpTitle[4]->setFont(bil.font);
    helpTitle[4]->setStyleSheet("color: white;");
    helpTitle[4]->setText("Donut:");
    helpTitle[4]->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    helpTitle[5] = new QLabel(pauseMenu);
    helpTitle[5]->setFixedSize(200, 50);
    helpTitle[5]->move(50, 630);
    helpTitle[5]->setFont(bil.font);
    helpTitle[5]->setStyleSheet("color: white;");
    helpTitle[5]->setText("Berry:");
    helpTitle[5]->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    helpTitle[6] = new QLabel(pauseMenu);
    helpTitle[6]->setFixedSize(200, 50);
    helpTitle[6]->move(50, 780);
    helpTitle[6]->setFont(bil.font);
    helpTitle[6]->setStyleSheet("color: white;");
    helpTitle[6]->setText("Turkey:");
    helpTitle[6]->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    //help icon
    helpIcon[0] = new QLabel(pauseMenu);
    helpIcon[0]->setFixedSize(30, 30);
    helpIcon[0]->move(10, 10);
    helpIcon[0]->setStyleSheet(QString("border-image: url(:/art/sprite/snakeHeadRight1.png)"));

    helpIcon[1] = new QLabel(pauseMenu);
    helpIcon[1]->setFixedSize(30, 30);
    helpIcon[1]->move(10, 120);
    helpIcon[1]->setStyleSheet(QString("border-image: url(:/art/sprite/snakeHeadRight2.png)"));

    helpIcon[2] = new QLabel(pauseMenu);
    helpIcon[2]->setFixedSize(30, 30);
    helpIcon[2]->move(10, 230);
    helpIcon[2]->setStyleSheet(QString("border-image: url(:/art/sprite/wall.png)"));

    helpIcon[3] = new QLabel(pauseMenu);
    helpIcon[3]->setFixedSize(30, 30);
    helpIcon[3]->move(10, 380);
    helpIcon[3]->setStyleSheet(QString("border-image: url(:/art/sprite/apple.png)"));

    helpIcon[4] = new QLabel(pauseMenu);
    helpIcon[4]->setFixedSize(30, 30);
    helpIcon[4]->move(10, 530);
    helpIcon[4]->setStyleSheet(QString("border-image: url(:/art/sprite/donuts.png)"));

    helpIcon[5] = new QLabel(pauseMenu);
    helpIcon[5]->setFixedSize(30, 30);
    helpIcon[5]->move(10, 640);
    helpIcon[5]->setStyleSheet(QString("border-image: url(:/art/sprite/blueberry.png)"));

    helpIcon[6] = new QLabel(pauseMenu);
    helpIcon[6]->setFixedSize(30, 30);
    helpIcon[6]->move(10, 790);
    helpIcon[6]->setStyleSheet(QString("border-image: url(:/art/sprite/turkey.png)"));

    //help text
    bil.font.setPixelSize(26);
    bil.font.setItalic(true);
    helpText[0] = new QLabel(pauseMenu);
    helpText[0]->setFixedSize(225, 100);
    helpText[0]->move(15, 50);
    helpText[0]->setFont(bil.font);
    helpText[0]->setStyleSheet("color: white;");
    helpText[0]->setText("Ctrl-WASD");
    helpText[0]->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    helpText[1] = new QLabel(pauseMenu);
    helpText[1]->setFixedSize(225, 100);
    helpText[1]->move(15, 160);
    helpText[1]->setFont(bil.font);
    helpText[1]->setStyleSheet("color: white;");
    helpText[1]->setText("Ctrl-Arrow Key");
    helpText[1]->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    helpText[2] = new QLabel(pauseMenu);
    helpText[2]->setFixedSize(225, 100);
    helpText[2]->move(15, 270);
    helpText[2]->setFont(bil.font);
    helpText[2]->setStyleSheet("color: white;");
    helpText[2]->setText("Dead on collis-\n-ion.");
    helpText[2]->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    helpText[3] = new QLabel(pauseMenu);
    helpText[3]->setFixedSize(225, 100);
    helpText[3]->move(15, 420);
    helpText[3]->setFont(bil.font);
    helpText[3]->setStyleSheet("color: white;");
    helpText[3]->setText("Add 1 point &\nget new food.");
    helpText[3]->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    helpText[4] = new QLabel(pauseMenu);
    helpText[4]->setFixedSize(225, 100);
    helpText[4]->move(15, 570);
    helpText[4]->setFont(bil.font);
    helpText[4]->setStyleSheet("color: white;");
    helpText[4]->setText("Add 1 life.");
    helpText[4]->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    helpText[5] = new QLabel(pauseMenu);
    helpText[5]->setFixedSize(225, 100);
    helpText[5]->move(15, 680);
    helpText[5]->setFont(bil.font);
    helpText[5]->setStyleSheet("color: white;");
    helpText[5]->setText("Slow down the\ntime.");
    helpText[5]->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    helpText[6] = new QLabel(pauseMenu);
    helpText[6]->setFixedSize(225, 100);
    helpText[6]->move(15, 830);
    helpText[6]->setFont(bil.font);
    helpText[6]->setStyleSheet("color: white;");
    helpText[6]->setText("Add 20 points!");
    helpText[6]->setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

void GameScene::creatScoreBoard()
{
    /// Summary:
    /// score board contains score labels and pause hint
    /// included in a widget
    /// present while playing
    /// (total: 3 labels)

    scoreBoard = new QWidget(this);

    QFont font;
    font.setFamily("Comic Sans MS");
    font.setBold(true);
    font.setPixelSize(40);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 2);

    score[0] = new QLabel(scoreBoard);
    score[0]->setFixedSize(250, 300);
    score[0]->setFont(font);
    score[0]->setStyleSheet("color: #ADD8E6;");
    score[0]->setAlignment(Qt::AlignCenter);

    score[1] = new QLabel(scoreBoard);
    score[1]->setFixedSize(250, 300);
    score[1]->move(0, 300);
    score[1]->setFont(font);
    score[1]->setStyleSheet("color: #FFB6C1;");
    score[1]->setAlignment(Qt::AlignCenter);

    pauseHint = new QLabel(scoreBoard);
    pauseHint->setFixedSize(250, 100);
    pauseHint->move(0, 800);
    font.setPixelSize(26);
    pauseHint->setFont(font);
    pauseHint->setStyleSheet("color: white");
    pauseHint->setAlignment(Qt::AlignCenter);
    pauseHint->setText("[ESC] to Pause");
}

void GameScene::creatEditBoard()
{
    /// Summary:
    /// edit board contains a series of buttons for map items, and buttons for save and quit
    /// included in a widget
    /// present in edit mode
    /// (total: 1 label, 10 buttons)

    editBoard = new QWidget(this);

    //edit hint label
    QFont font;
    font.setFamily("Comic Sans MS");
    font.setBold(true);
    font.setPixelSize(34);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 2);

    editHint = new QLabel(editBoard);
    editHint->setFixedSize(250, 800);
    editHint->setFont(font);
    editHint->setStyleSheet("color: white");
    editHint->setAlignment(Qt::AlignHCenter);
    editHint->setText("\nSpawn Points\n\n\n\nEnvironment\n\n\n\nFood");

    //button initialization list
    BtnInitialList bil;
    bil.parent = editBoard;
    bil.size = QSize(100, 100);
    bil.font = font;
    bil.font.setPixelSize(30);
    bil.style = "color: white";
    bil.stateChangeable = true;
    bil.defaultState = false;

    //edit tool buttons
    bil.pos = QPoint(20, 100);
    bil.iconNormal = QPixmap(":/art/ui/snake1Selected.png");
    bil.iconPressed = QPixmap(":/art/ui/snake1Pressed.png");
    bil.iconFalseNormal = QPixmap(":/art/ui/snake1Normal.png");
    bil.iconFalsePressed = QPixmap(":/art/ui/snake1Pressed.png");
    editBtns[Snake1] = new PushButton(bil);
    connect(editBtns[Snake1], &QPushButton::clicked, this, [=](){ editSelect(Snake1); });

    bil.pos = QPoint(130, 100);
    bil.iconNormal = QPixmap(":/art/ui/snake2Selected.png");
    bil.iconPressed = QPixmap(":/art/ui/snake2Pressed.png");
    bil.iconFalseNormal = QPixmap(":/art/ui/snake2Normal.png");
    bil.iconFalsePressed = QPixmap(":/art/ui/snake2Pressed.png");
    editBtns[Snake2] = new PushButton(bil);
    connect(editBtns[Snake2], &QPushButton::clicked, this, [=](){ editSelect(Snake2); });

    bil.pos = QPoint(20, 290);
    bil.iconNormal = QPixmap(":/art/ui/grassSelected.png");
    bil.iconPressed = QPixmap(":/art/ui/grassPressed.png");
    bil.iconFalseNormal = QPixmap(":/art/ui/grassNormal.png");
    bil.iconFalsePressed = QPixmap(":/art/ui/grassPressed.png");
    editBtns[Grass] = new PushButton(bil);
    connect(editBtns[Grass], &QPushButton::clicked, this, [=](){ editSelect(Grass); });

    bil.pos = QPoint(130, 290);
    bil.iconNormal = QPixmap(":/art/ui/wallSelected.png");
    bil.iconPressed = QPixmap(":/art/ui/wallPressed.png");
    bil.iconFalseNormal = QPixmap(":/art/ui/wallNormal.png");
    bil.iconFalsePressed = QPixmap(":/art/ui/wallPressed.png");
    editBtns[Wall] = new PushButton(bil);
    connect(editBtns[Wall], &QPushButton::clicked, this, [=](){ editSelect(Wall); });

    bil.pos = QPoint(20, 480);
    bil.iconNormal = QPixmap(":/art/ui/appleSelected.png");
    bil.iconPressed = QPixmap(":/art/ui/applePressed.png");
    bil.iconFalseNormal = QPixmap(":/art/ui/appleNormal.png");
    bil.iconFalsePressed = QPixmap(":/art/ui/applePressed.png");
    editBtns[Apple] = new PushButton(bil);
    connect(editBtns[Apple], &QPushButton::clicked, this, [=](){ editSelect(Apple); });

    bil.pos = QPoint(130, 480);
    bil.iconNormal = QPixmap(":/art/ui/donutsSelected.png");
    bil.iconPressed = QPixmap(":/art/ui/donutsPressed.png");
    bil.iconFalseNormal = QPixmap(":/art/ui/donutsNormal.png");
    bil.iconFalsePressed = QPixmap(":/art/ui/donutsPressed.png");
    editBtns[Donuts] = new PushButton(bil);
    connect(editBtns[Donuts], &QPushButton::clicked, this, [=](){ editSelect(Donuts); });

    bil.pos = QPoint(20, 590);
    bil.iconNormal = QPixmap(":/art/ui/blueberrySelected.png");
    bil.iconPressed = QPixmap(":/art/ui/blueberryPressed.png");
    bil.iconFalseNormal = QPixmap(":/art/ui/blueberryNormal.png");
    bil.iconFalsePressed = QPixmap(":/art/ui/blueberryPressed.png");
    editBtns[BlueBerry] = new PushButton(bil);
    connect(editBtns[BlueBerry], &QPushButton::clicked, this, [=](){ editSelect(BlueBerry); });

    bil.pos = QPoint(130, 590);
    bil.iconNormal = QPixmap(":/art/ui/turkeySelected.png");
    bil.iconPressed = QPixmap(":/art/ui/turkeyPressed.png");
    bil.iconFalseNormal = QPixmap(":/art/ui/turkeyNormal.png");
    bil.iconFalsePressed = QPixmap(":/art/ui/turkeyPressed.png");
    editBtns[Turkey] = new PushButton(bil);
    connect(editBtns[Turkey], &QPushButton::clicked, this, [=](){ editSelect(Turkey); });

    bil.stateChangeable = false;
    bil.pos = QPoint(10, 780);
    bil.iconNormal = QPixmap(":/art/ui/saveNormal.png");
    bil.iconPressed = QPixmap(":/art/ui/savePressed.png");
    editSave = new PushButton(bil);
    connect(editSave, &QPushButton::clicked, this, &GameScene::mapSave);

    bil.pos = QPoint(140, 780);
    bil.iconNormal = QPixmap(":/art/ui/exitNormal.png");
    bil.iconPressed = QPixmap(":/art/ui/exitPressed.png");
    editQuit = new PushButton(bil);
    connect(editQuit, &QPushButton::clicked, this, &GameScene::quit);

    bil.pos = QPoint(75, 780);
    bil.iconNormal = QPixmap(":/art/ui/backNormal.png");
    bil.iconPressed = QPixmap(":/art/ui/backPressed.png");
    editBack = new PushButton(bil);
    connect(editBack, &QPushButton::clicked, this, [=]()
    {
        editFlag = false;
        editBoard->hide();
        editSave->show();
        editQuit->show();
        editBack->hide();
        editBtns[Snake1]->setEnabled(true);
        editBtns[Snake2]->setEnabled(true);

        bool appleExist = false;
        for (int i = 0; i < 45; i++) {
            for (int j = 0; j < 30; j++) {
                if (grid[i][j] == Apple) {
                    appleExist = true;
                    break;
                }
            }
        }
        if (!appleExist)
            newFood();

        scoreBoard->show();
        timer->start();
    });
    editBack->hide();
}

void GameScene::creatEndMenu()
{
    /// Summary:
    /// end menu provides hints when at end of a game
    /// in challenge mode, it provides a high score list and try to get player name
    /// in single player mode, it provides a score
    /// in multiplayer mode, it provides player scores and point out who is the winner
    /// contains high score list, a hint label and a quit button
    /// included in a widget
    /// (total: 18 labels, 1 button, 1 line editor)

    endMenu = new QWidget(this);

    ///---set font---///
    QFont font;
    font.setFamily("Comic Sans MS");
    font.setBold(true);
    font.setPixelSize(80);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 2);

    ///---creat base label---///
    endBase = new QLabel(endMenu);
    endBase->setFixedSize(1600, 900);
    endBase->move(0, 0);
    endBase->setPixmap(QPixmap(":/art/ui/base.png"));

    ///---creat quit button---///
    BtnInitialList bil;
    bil.parent = endMenu;
    bil.size = QSize(100, 100);
    bil.pos = QPoint(1100, 750);
    bil.iconNormal = QPixmap(":/art/ui/exitNormal.png");
    bil.iconPressed = QPixmap(":/art/ui/exitPressed.png");
    bil.text = "";
    endQuit = new PushButton(bil);
    connect(endQuit, &QPushButton::clicked, this, &GameScene::quit);

    ///---result tab---///
    resultTab = new QWidget(endMenu);

    result = new QLabel(resultTab);
    result->setFixedSize(1000, 150);
    result->move(300, 300);
    result->setFont(font);
    result->setText("P1 is the Winner!");
    result->setStyleSheet("color: white;");
    result->setAlignment(Qt::AlignCenter);

    detail[0] = new QLabel(resultTab);
    detail[0]->setFixedSize(450, 75);
    detail[0]->move(575, 450);
    font.setPixelSize(40);
    detail[0]->setFont(font);
    detail[0]->setText("Player1 Score: ");
    detail[0]->setStyleSheet("color: #ADD8E6");
    detail[0]->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    detail[1] = new QLabel(resultTab);
    detail[1]->setFixedSize(450, 75);
    detail[1]->move(575, 525);
    detail[1]->setFont(font);
    detail[1]->setText("Player2 Score: ");
    detail[1]->setStyleSheet("color: #FFB6C1");
    detail[1]->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    ///---high score list tab---///
    listTab = new QWidget(endMenu);

    listTitle = new QLabel(listTab);
    listTitle->setFixedSize(600, 100);
    listTitle->move(500, 20);
    font.setPixelSize(60);
    listTitle->setFont(font);
    listTitle->setText("High Scores");
    listTitle->setStyleSheet("color: white");
    listTitle->setAlignment(Qt::AlignCenter);

    inputHint = new QLabel(listTab);
    inputHint->setFixedSize(600, 100);
    inputHint->move(500, 670);
    font.setPixelSize(34);
    inputHint->setFont(font);
    inputHint->setText("Congratulations!\nPlease input your name: ");
    inputHint->setStyleSheet("color: white");
    inputHint->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    font.setPixelSize(30);
    for (int i = 0; i < 12; i++) {
        endList[i] = new QLabel(listTab);
        endList[i]->setFixedSize(500, 50);
        endList[i]->move(550, 120 + i * 45);
        endList[i]->setFont(font);
        endList[i]->setStyleSheet("color: white;");
        endList[i]->setText(QString("%1").arg(i));
        endList[i]->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    }

    endList[0]->setText("No.\tName\t\tScore");
    endList[1]->setStyleSheet("color: #FFD700");
    endList[2]->setStyleSheet("color: #C0C0C0");
    endList[3]->setStyleSheet("color: #8B4513");
    font.setItalic(true);
    endList[11]->setFont(font);
    endList[11]->setStyleSheet("color: #ADD8E6");

    nameInput = new QLineEdit(listTab);
    nameInput->setFixedSize(400, 50);
    nameInput->move(500, 770);
    nameInput->setFont(font);

    ///---set visiablity---///
    endMenu->hide();
    resultTab->hide();
    listTab->hide();
}

MapItem GameScene::playerToMapItem(int player_num)
{
    if (player_num == 1)
        return Snake1;
    if (player_num == 2)
        return Snake2;

    return Grass;
}


void GameScene::runChallenge()
{
    scoreBoard->show();
    mapEdit->setDisabled(true);

    endFlag[0] = false;
    endFlag[1] = false;
    editFlag = false;



    for (int i = 0; i < 45; i++) {
        for (int j = 0; j < 30; j++) {
            if (i == 10 && j == 10) {
                grid[i][j] = Snake1;
                s1 = new Snake(QPoint(i * 30 + 250, j * 30), QPoint(30, 0), this);
            }
            else {
                grid[i][j] = Grass;
                blank.push_back(QPoint(i * 30 + 250, j * 30));
            }
        }
    }

    newFood();

    score[0]->setText("Score\n0\nLife\n0");
    score[1]->setText("");

    timer->start(200);

    inGame = true;
}

void GameScene::runSingle(QString mapName, bool ai)
{
    scoreBoard->show();
    mapEdit->setEnabled(true);

    endFlag[0] = false;
    endFlag[1] = false;
    editFlag = false;
    bool fail = false;
    bool appleExist = false;

    fail = (mapName == "");
    if (!fail) {
        //open file
        QFile mapFile("./data/map/" + mapName);
        mapFile.open(QIODevice::ReadWrite);
        QDataStream mapData(&mapFile);

        //load map
        fail = mapData.atEnd();
        if (!fail)  {
            ///---part1: grid---///
            for (int y = 0; y < 30; y++) {
                for (int x = 0; x < 45; x++) {
                    mapData >> grid[x][y];
                    switch (grid[x][y]) {
                    case Snake1:
                        s1 = new Snake(QPoint(x * 30 + 250, y * 30), QPoint(30, 0), this, 1, ai);
                        break;

                    case Wall:
                        linkBoard[x][y] = newWall(QPoint(x * 30 + 250, y * 30));
                        break;

                    case Apple:
                        appleExist = true;
                        linkBoard[x][y] = newApple(QPoint(x * 30 + 250, y * 30));
                        break;

                    case Donuts:
                        linkBoard[x][y] = newDonuts(QPoint(x * 30 + 250, y * 30));
                        break;

                    case BlueBerry:
                        linkBoard[x][y] = newBlueberry(QPoint(x * 30 + 250, y * 30));
                        break;

                    case Turkey:
                        linkBoard[x][y] = newTurkey(QPoint(x * 30 + 250, y * 30));
                        break;

                    case Grass:
                    case Snake2:
                        blank.push_back(QPoint(x * 30 + 250, y * 30));
                        break;
                    }
                }
            }

            ///---part2: directions---///
            QPoint tmp;
            mapData >> tmp;
            s1->turn(tmp);
        }
    }

    if (fail) {
        for (int i = 0; i < 45; i++) {
            for (int j = 0; j < 30; j++) {
                if (i == 10 && j == 10) {
                    grid[i][j] = Snake1;
                    s1 = new Snake(QPoint(i * 30 + 250, j * 30), QPoint(30, 0), this, 1, ai);
                }
                else {
                    grid[i][j] = Grass;
                    blank.push_back(QPoint(i * 30 + 250, j * 30));
                }
            }
        }
    }

    if (!appleExist)
        newFood();

    score[0]->setText("Score\n0\nLife\n0");
    score[1]->setText("");

    timer->start(200);

    inGame = true;
}

void GameScene::runMultiplayer(QString mapName, bool ai1, bool ai2)
{
    scoreBoard->show();
    mapEdit->setEnabled(true);

    endFlag[0] = false;
    endFlag[1] = false;
    editFlag = false;
    bool fail = false;
    bool appleExist = false;

    fail = (mapName == "");
    if (!fail) {
        //open file
        QFile mapFile("./data/map/" + mapName);
        mapFile.open(QIODevice::ReadWrite);
        QDataStream mapData(&mapFile);

        //load map
        fail = mapData.atEnd();
        if (!fail)  {
            ///---part1: grid---///
            for (int y = 0; y < 30; y++) {
                for (int x = 0; x < 45; x++) {
                    mapData >> grid[x][y];
                    switch (grid[x][y]) {
                    case Snake1:
                        s1 = new Snake(QPoint(x * 30 + 250, y * 30), QPoint(30, 0), this, 1, ai1);
                        break;

                    case Snake2:
                        s2 = new Snake(QPoint(x * 30 + 250, y * 30), QPoint(-30, 0), this, 2, ai2);
                        break;

                    case Wall:
                        linkBoard[x][y] = newWall(QPoint(x * 30 + 250, y * 30));
                        break;

                    case Apple:
                        appleExist = true;
                        linkBoard[x][y] = newApple(QPoint(x * 30 + 250, y * 30));
                        break;

                    case Donuts:
                        linkBoard[x][y] = newDonuts(QPoint(x * 30 + 250, y * 30));
                        break;

                    case BlueBerry:
                        linkBoard[x][y] = newBlueberry(QPoint(x * 30 + 250, y * 30));
                        break;

                    case Turkey:
                        linkBoard[x][y] = newTurkey(QPoint(x * 30 + 250, y * 30));
                        break;

                    case Grass:
                        blank.push_back(QPoint(x * 30 + 250, y * 30));
                        break;
                    }
                }
            }

            ///---part2: directions---///
            QPoint tmp;
            mapData >> tmp;
            s1->turn(tmp);
            mapData >> tmp;
            s2->turn(tmp);
        }
    }

    if (fail) {
        for (int i = 0; i < 45; i++) {
            for (int j = 0; j < 30; j++) {
                if (i == 10 && j == 10) {
                    grid[i][j] = Snake1;
                    s1 = new Snake(QPoint(i * 30 + 250, j * 30), QPoint(30, 0), this, 1, ai1);
                }
                else if (i == 35 && j == 20) {
                    grid[i][j] = Snake2;
                    s2 = new Snake(QPoint(i * 30 + 250, j * 30), QPoint(-30, 0), this, 2, ai2);
                }
                else {
                    grid[i][j] = Grass;
                    blank.push_back(QPoint(i * 30 + 250, j * 30));
                }
            }
        }
    }

    if (!appleExist)
        newFood();

    score[0]->setText("Score\n0\nLife\n0");
    score[1]->setText("Score\n0\nLife\n0");

    timer->start(200);

    setFocus();
    inGame = true;
}

void GameScene::load()
{ 
    endFlag[0] = false;
    endFlag[1] = false;
    editFlag = false;
    scoreBoard->show();

    QFile saveFile("./data/save/save0.sav");
    saveFile.open(QIODevice::ReadWrite);
    QDataStream saveData(&saveFile);

    bool flag;

    ///---part1: editablity---///
    saveData >> flag;
    mapEdit->setEnabled(flag);


    ///---part2: map data---///
    for (int y = 0; y < 30; y++) {
        for (int x = 0; x < 45; x++) {
            saveData >> grid[x][y];
            switch (grid[x][y]) {
            case Grass:
                blank.push_back(QPoint(x * 30 + 250, y * 30));
                break;

            case Wall:
                linkBoard[x][y] = newWall(QPoint(x * 30 + 250, y * 30));
                break;

            case Apple:
                linkBoard[x][y] = newApple(QPoint(x * 30 + 250, y * 30));
                break;

            case Donuts:
                linkBoard[x][y] = newDonuts(QPoint(x * 30 + 250, y * 30));
                break;

            case BlueBerry:
                linkBoard[x][y] = newBlueberry(QPoint(x * 30 + 250, y * 30));
                break;

            case Turkey:
                linkBoard[x][y] = newTurkey(QPoint(x * 30 + 250, y * 30));
                break;

            default:
                break;
            }
        }
    }

    ///---part3: snake---///
    s1 = new Snake(saveData, this);
    score[0]->setText(QString("Score\n%1\nLife\n%2").arg(s1->getLength() - 1).arg(s1->getLife()));
    saveData >> flag;
    if (flag) {
        s2 = new Snake(saveData, this);
        score[1]->setText(QString("Score\n%1\nLife\n%2").arg(s2->getLength() - 1).arg(s2->getLife()));
    }
    else {
        score[1]->setText("");
    }

    timer->start(200);

    inGame = true;
}


void GameScene::gameUpdate()
{
    QPoint head1, head2;
    bool foodFlag;

    if (s2) {
        if (s1->isAuto())
            autoSnake(s1);
        if (s2->isAuto())
            autoSnake(s2);

        foodFlag = moveSnake(s1);
        head1 = s1->getHead();
        bool ftmp = moveSnake(s2);
        foodFlag |= ftmp;
        head2 = s2->getHead();

        if (grid[(head1.x() - 250) / 30][head1.y() / 30] != Grass || head1 == head2) {
            getDemage(s1);
        }
        else {
            grid[(head1.x() - 250) / 30][head1.y() / 30] = Snake1;
            blank.removeAll(head1);
        }

        if (grid[(head2.x() - 250) / 30][head2.y() / 30] != Grass || head1 == head2) {
            getDemage(s2);
        }
        else {
            grid[(head2.x() - 250) / 30][head2.y() / 30] = Snake2;
            blank.removeAll(head2);
        }

        score[0]->setText(QString("Score\n%1\nLife\n%2").arg(s1->getLength() - 1).arg(s1->getLife()));
        score[1]->setText(QString("Score\n%1\nLife\n%2").arg(s2->getLength() - 1).arg(s2->getLife()));
    }
    else {
        if (s1->isAuto())
            autoSnake(s1);

        foodFlag = moveSnake(s1);
        head1 = s1->getHead();

        if (grid[(head1.x() - 250) / 30][head1.y() / 30] != Grass) {
            getDemage(s1);
        }
        else {
            grid[(head1.x() - 250) / 30][head1.y() / 30] = Snake1;
            blank.removeAll(head1);
        }

        score[0]->setText(QString("Score\n%1\nLife\n%2").arg(s1->getLength() - 1).arg(s1->getLife()));
    }

    if (foodFlag)
        newFood();

    if (endFlag[0] || endFlag[1]) {
        gameEnd();
    }
}

void GameScene::autoSnake(Snake *s)
{
    bool map[45][30];
    Point target(0, 0);
    int foodDis = INT32_MAX;
    Point head((s->getHead().x() - 250) / 30, s->getHead().y() / 30);
    Point tail((s->getTail().x() - 250) / 30, s->getTail().y() / 30);

    //copy map
    int dis = INT32_MAX;
    Point tmp(0, 0);
    for (int i = 0; i < 45; i++) {
        for (int j = 0; j < 30; j++) {
            switch (grid[i][j]) {
            case Wall:
            case Snake1:
            case Snake2:
                map[i][j] = true;
                break;
            case Apple:
            case Donuts:
            case BlueBerry:
            case Turkey:
                tmp = Point(i, j);
                dis = distance(head, tmp);
                if (dis < foodDis) {
                    foodDis = dis;
                    target = tmp;
                }
            case Grass:
                map[i][j] = false;
            }
        }
    }


    //forcast next state
    map[head.x][head.y] = false;
    if (s->getPlayer() == 1) {
        if (s2) {
            tmp = Point((s2->getHead().x() - 250) / 30, s2->getHead().y() / 30);

            map[(tmp.x + 44) % 45][tmp.y] = true;
            map[(tmp.x + 1) % 45][tmp.y] = true;
            map[tmp.x][(tmp.y + 29) % 30] = true;
            map[tmp.x][(tmp.y + 1) % 30] = true;
        }
    }
    else {
        tmp = Point((s1->getHead().x() - 250) / 30, s1->getHead().y() / 30);

        map[(tmp.x + 44) % 45][tmp.y] = true;
        map[(tmp.x + 1) % 45][tmp.y] = true;
        map[tmp.x][(tmp.y + 29) % 30] = true;
        map[tmp.x][(tmp.y + 1) % 30] = true;
    }

    //find way to nearest food
    bool mark[45][30];
    while (true) {
        if (!map[(target.x + 45) % 45][(target.y + 30) % 30]) {
            for (int i = 0; i < 45; i++) {
                for (int j = 0; j < 30; j++) {
                    mark[i][j] = false;
                }
            }
            tmp = findWay(map, mark, head, target);
        }

        //if no way, then wander
        if (tmp == Point(0, 0) || map[(target.x + 45) % 45][(target.y + 30) % 30]) {
            QVector<QPoint> steps;
            map[head.x][head.y] = true;

            for (int i = 0; i < 45; i++) {
                for (int j = 0; j < 30; j++) {
                    mark[i][j] = false;
                }
            }
            if (findWay(map, mark, head + Point(-1, 0), tail) != Point(0, 0))
                steps.append(QPoint(-1, 0));

            for (int i = 0; i < 45; i++) {
                for (int j = 0; j < 30; j++) {
                    mark[i][j] = false;
                }
            }
            if (findWay(map, mark, head + Point(1, 0), tail) != Point(0, 0))
                steps.append(QPoint(1, 0));

            for (int i = 0; i < 45; i++) {
                for (int j = 0; j < 30; j++) {
                    mark[i][j] = false;
                }
            }
            if (findWay(map, mark, head + Point(0, -1), tail) != Point(0, 0))
                steps.append(QPoint(0, -1));

            for (int i = 0; i < 45; i++) {
                for (int j = 0; j < 30; j++) {
                    mark[i][j] = false;
                }
            }
            if (findWay(map, mark, head + Point(0, 1), tail) != Point(0, 0))
                steps.append(QPoint(0, 1));

            if (!steps.size()) {
                if (!map[(head.x + 44) % 45][head.y])
                    steps.append(QPoint(-1, 0));
                if (!map[(head.x + 1) % 45][head.y])
                    steps.append(QPoint(1, 0));
                if (!map[head.x][(head.y + 29) % 30])
                    steps.append(QPoint(0, -1));
                if (!map[head.x][(head.y + 1) % 30])
                    steps.append(QPoint(0, 1));

                if (!steps.size()) {
                    steps.append(QPoint(-1, 0));
                    steps.append(QPoint(1, 0));
                    steps.append(QPoint(0, -1));
                    steps.append(QPoint(0, 1));
                }
            }

            int ran = randGen.bounded(steps.size());
            if (!s->turn(steps[ran] * 30)) {
                steps.removeAt(ran);
                if (!steps.size()) {
                    steps.append(QPoint(-1, 0));
                    steps.append(QPoint(1, 0));
                    steps.append(QPoint(0, -1));
                    steps.append(QPoint(0, 1));
                }

                ran = randGen.bounded(steps.size());
                s->turn(steps[ran] * 30);
            }

            break;
        }

        //assume step, if still connected with tail, turn
        map[head.x][head.y] = true;
        for (int i = 0; i < 45; i++) {
            for (int j = 0; j < 30; j++) {
                mark[i][j] = false;
            }
        }
        if (findWay(map, mark, head + tmp, tail) != Point(0, 0))  {
            if (s->turn(QPoint(tmp.x * 30, tmp.y * 30)))
                break;
        }

        //if lost connection or fail to turn, step back and block this way
        map[head.x][head.y] = false;
        map[(head.x + tmp.x + 45) % 45][(head.y + tmp.y + 30) % 30] = true;

    }

}

int GameScene::distance(Point origin, Point &phase)
{
    int tmp, dis = abs(origin.x - phase.x) + abs(origin.y - phase.y);

    if (origin.x < phase.x) {
        tmp = abs(origin.x - phase.x + 45) + abs(origin.y - phase.y);
        if (tmp < dis) {
            dis = tmp;
            phase.x -= 45;
        }
    }
    else if (origin.x > phase.x) {
        tmp = abs(origin.x - phase.x - 45) + abs(origin.y - phase.y);
        if (tmp < dis) {
            dis = tmp;
            phase.x += 45;
        }
    }

    if (origin.y < phase.y) {
        tmp = abs(origin.x - phase.x) + abs(origin.y - phase.y + 30);
        if (tmp < dis) {
            dis = tmp;
            phase.y -= 30;
        }
    }
    else if (origin.y > phase.y) {
        tmp = abs(origin.x - phase.x) + abs(origin.y - phase.y - 30);
        if (tmp < dis) {
            dis = tmp;
            phase.y += 30;
        }
    }

    return dis;
}

GameScene::Point GameScene::findWay(bool map[45][30], bool mark[45][30], Point start, Point dest)
{
    dest.x = (dest.x + 45) % 45;
    dest.y = (dest.y + 30) % 30;

    if (start == dest)
        return Point(0, 1);
    if (map[start.x][start.y] || mark[start.x][start.y])
        return Point(0, 0);

    //step in
    mark[start.x][start.y] = true;

    //get nearest phase
    distance(start, dest);

    //a star
    Point step(0, 0);
    if (abs(start.x - dest.x) >= abs(start.y - dest.y)) {
        if (start.x <= dest.x) {
            if (findWay(map, mark, Point((start.x + 1) % 45, start.y), dest) != Point(0, 0))
                step = Point(1, 0);
            else if (start.y <= dest.y) {
                if (findWay(map, mark, Point(start.x, (start.y + 1) % 30), dest) != Point(0, 0))
                    step = Point(0, 1);
                else if (findWay(map, mark, Point(start.x, (start.y + 29) % 30), dest) != Point(0, 0))
                    step = Point(0, -1);
                else if (findWay(map, mark, Point((start.x + 44) % 45, start.y), dest) != Point(0, 0))
                    step = Point(-1, 0);
                else
                    step = Point(0, 0);
            }
            else {
                if (findWay(map, mark, Point(start.x, (start.y + 29) % 30), dest) != Point(0, 0))
                    step = Point(0, -1);
                else if (findWay(map, mark, Point(start.x, (start.y + 1) % 30), dest) != Point(0, 0))
                    step = Point(0, 1);
                else if (findWay(map, mark, Point((start.x + 44) % 45, start.y), dest) != Point(0, 0))
                    step = Point(-1, 0);
                else
                    step = Point(0, 0);
            }
        }
        else {
            if (findWay(map, mark, Point((start.x + 44) % 45, start.y), dest) != Point(0, 0))
                step = Point(-1, 0);
            else if (start.y <= dest.y) {
                if (findWay(map, mark, Point(start.x, (start.y + 1) % 30), dest) != Point(0, 0))
                    step = Point(0, 1);
                else if (findWay(map, mark, Point(start.x, (start.y + 29) % 30), dest) != Point(0, 0))
                    step = Point(0, -1);
                else if (findWay(map, mark, Point((start.x + 1) % 45, start.y), dest) != Point(0, 0))
                    step = Point(1, 0);
                else
                    step = Point(0, 0);
            }
            else {
                if (findWay(map, mark, Point(start.x, (start.y + 29) % 30), dest) != Point(0, 0))
                    step = Point(0, -1);
                else if (findWay(map, mark, Point(start.x, (start.y + 1) % 30), dest) != Point(0, 0))
                    step = Point(0, 1);
                else if (findWay(map, mark, Point((start.x + 1) % 45, start.y), dest) != Point(0, 0))
                    step = Point(1, 0);
                else
                    step = Point(0, 0);
            }
        }
    }
    else {
        if (start.y <= dest.y) {
            if (findWay(map, mark, Point(start.x, (start.y + 1) % 30), dest) != Point(0, 0))
                step = Point(0, 1);
            else if (start.x <= dest.x) {
                if (findWay(map, mark, Point((start.x + 1) % 45, start.y), dest) != Point(0, 0))
                    step = Point(1, 0);
                else if (findWay(map, mark, Point((start.x + 44) % 45, start.y), dest) != Point(0, 0))
                    step = Point(-1, 0);
                else if (findWay(map, mark, Point(start.x, (start.y + 29) % 30), dest) != Point(0, 0))
                    step = Point(0, -1);
                else
                    step = Point(0, 0);
            }
            else {
                if (findWay(map, mark, Point((start.x + 44) % 45, start.y), dest) != Point(0, 0))
                    step = Point(-1, 0);
                else if (findWay(map, mark, Point((start.x + 1) % 45, start.y), dest) != Point(0, 0))
                    step = Point(1, 0);
                else if (findWay(map, mark, Point(start.x, (start.y + 29) % 30), dest) != Point(0, 0))
                    step = Point(0, -1);
                else
                    step = Point(0, 0);
            }
        }
        else {
            if (findWay(map, mark, Point(start.x, (start.y + 29) % 30), dest) != Point(0, 0))
                step = Point(0, -1);
            else if (start.x <= dest.x) {
                if (findWay(map, mark, Point((start.x + 1) % 45, start.y), dest) != Point(0, 0))
                    step = Point(1, 0);
                else if (findWay(map, mark, Point((start.x + 44) % 45, start.y), dest) != Point(0, 0))
                    step = Point(-1, 0);
                else if (findWay(map, mark, Point(start.x, (start.y + 1) % 30), dest) != Point(0, 0))
                    step = Point(0, 1);
                else
                    step = Point(0, 0);
            }
            else {
                if (findWay(map, mark, Point((start.x + 44) % 45, start.y), dest) != Point(0, 0))
                    step = Point(-1, 0);
                else if (findWay(map, mark, Point((start.x + 1) % 45, start.y), dest) != Point(0, 0))
                    step = Point(1, 0);
                else if (findWay(map, mark, Point(start.x, (start.y + 1) % 30), dest) != Point(0, 0))
                    step = Point(0, 1);
                else
                    step = Point(0, 0);
            }
        }
    }

    return step;
}

bool GameScene::moveSnake(Snake *s)
{
    bool foodFlag = false;
    QPoint lastTail = s->getTail();

    QPoint dest = s->moveStep();
    switch (grid[(dest.x() - 250) / 30][dest.y() / 30]) {
    case Apple:
        grid[(dest.x() - 250) / 30][dest.y() / 30] = Grass;
        blank.push_back(dest);
        delete linkBoard[(dest.x() - 250) / 30][dest.y() / 30];
        linkBoard[(dest.x() - 250) / 30][dest.y() / 30] = nullptr;

        s->grow();
        foodFlag = true;

        gameSound->setSource(QUrl::fromLocalFile(":/audio/sound/apple.wav"));
        gameSound->play();

        break;

    case Donuts:
        grid[(dest.x() - 250) / 30][dest.y() / 30] = Grass;
        blank.push_back(dest);
        delete linkBoard[(dest.x() - 250) / 30][dest.y() / 30];
        linkBoard[(dest.x() - 250) / 30][dest.y() / 30] = nullptr;

        s->addLife();

        gameSound->setSource(QUrl::fromLocalFile(":/audio/sound/donuts.wav"));
        gameSound->play();

        break;

    case BlueBerry:
        grid[(dest.x() - 250) / 30][dest.y() / 30] = Grass;
        blank.push_back(dest);
        delete linkBoard[(dest.x() - 250) / 30][dest.y() / 30];
        linkBoard[(dest.x() - 250) / 30][dest.y() / 30] = nullptr;

        timer->setInterval(400);
        bulletTime->start(10000);

        gameSound->setSource(QUrl::fromLocalFile(":/audio/sound/blueberry.wav"));
        gameSound->play();

        break;

    case Turkey:
        grid[(dest.x() - 250) / 30][dest.y() / 30] = Grass;
        blank.push_back(dest);
        delete linkBoard[(dest.x() - 250) / 30][dest.y() / 30];
        linkBoard[(dest.x() - 250) / 30][dest.y() / 30] = nullptr;

        s->bonus();

        gameSound->setSource(QUrl::fromLocalFile(":/audio/sound/turkey.wav"));
        gameSound->play();

        break;      

    default:
        break;
    }

    if (s->getTail() != lastTail) {
        grid[(lastTail.x() - 250) / 30][lastTail.y() / 30] = Grass;
        blank.push_back(s->getTail());
    }

    return foodFlag;
}

void GameScene::getDemage(Snake *s)
{
    if (s->getLife()) {
        QVector<QPoint> &tmp = s->reborn();
        for (QPoint p : tmp) {
            grid[(p.x() - 250) / 30][p.y() / 30] = Grass;
            blank.push_back(p);
        }

        grid[(s->getHead().x() - 250) / 30][s->getHead().y() / 30] = playerToMapItem(s->getPlayer());
        blank.removeAll(s->getHead());

        gameSound->setSource(QUrl::fromLocalFile(":/audio/sound/reborn.wav"));
        gameSound->play();

    }
    else {
        endFlag[s->getPlayer() - 1] = true;
    }
}

void GameScene::newFood()
{
    if (!blank.size())
        return;

    int tmp = randGen.bounded(blank.size());

    linkBoard[(blank.at(tmp).x() - 250) / 30][blank.at(tmp).y() / 30] = newApple(blank.at(tmp));
    grid[(blank.at(tmp).x() - 250) / 30][blank.at(tmp).y() / 30] = Apple;
    blank.removeAt(tmp);

    tmp = randGen.bounded(100);
    if (tmp < 2 && blank.size()) {
        tmp = randGen.bounded(blank.size());

        linkBoard[(blank.at(tmp).x() - 250) / 30][blank.at(tmp).y() / 30] = newDonuts(blank.at(tmp));
        grid[(blank.at(tmp).x() - 250) / 30][blank.at(tmp).y() / 30] = Donuts;
        blank.removeAt(tmp);
    }

    tmp = randGen.bounded(100);
    if (tmp < 5 && blank.size()) {
        tmp = randGen.bounded(blank.size());

        linkBoard[(blank.at(tmp).x() - 250) / 30][blank.at(tmp).y() / 30] = newBlueberry(blank.at(tmp));
        grid[(blank.at(tmp).x() - 250) / 30][blank.at(tmp).y() / 30] = BlueBerry;
        blank.removeAt(tmp);
    }

    tmp = randGen.bounded(100);
    if (tmp < 10 && blank.size()) {
        tmp = randGen.bounded(blank.size());

        linkBoard[(blank.at(tmp).x() - 250) / 30][blank.at(tmp).y() / 30] = newTurkey(blank.at(tmp));
        grid[(blank.at(tmp).x() - 250) / 30][blank.at(tmp).y() / 30] = Turkey;
        blank.removeAt(tmp);
    }
}

QLabel *GameScene::newWall(QPoint p)
{
    QLabel *a;
    a = new QLabel(this);
    a->setFixedSize(30, 30);
    a->move(p);
    a->setStyleSheet(QString("border-image: url(:/art/sprite/wall.png)"));
    a->show();
    return a;
}

QLabel *GameScene::newApple(QPoint p)
{
    QLabel *a;
    a = new QLabel(this);
    a->setFixedSize(30, 30);
    a->move(p);
    a->setStyleSheet(QString("border-image: url(:/art/sprite/apple.png)"));
    a->show();
    return a;
}

QLabel *GameScene::newDonuts(QPoint p)
{
    QLabel *a;
    a = new QLabel(this);
    a->setFixedSize(30, 30);
    a->move(p);
    a->setStyleSheet(QString("border-image: url(:/art/sprite/donuts.png)"));
    a->show();
    return a;
}

QLabel *GameScene::newBlueberry(QPoint p)
{
    QLabel *a;
    a = new QLabel(this);
    a->setFixedSize(30, 30);
    a->move(p);
    a->setStyleSheet(QString("border-image: url(:/art/sprite/blueberry.png)"));
    a->show();
    return a;
}

QLabel *GameScene::newTurkey(QPoint p)
{
    QLabel *a;
    a = new QLabel(this);
    a->setFixedSize(30, 30);
    a->move(p);
    a->setStyleSheet(QString("border-image: url(:/art/sprite/turkey.png)"));
    a->show();
    return a;
}

void GameScene::gameEnd()
{
    timer->stop();
    scoreBoard->hide();

    if (!mapEdit->isEnabled()) {
        //find or creat dir
        QDir dir;
        if (!dir.exists("./data/save")) {
            if (!dir.exists("./data")) {
                dir.mkdir("./data");
            }
            dir.mkdir("./data/save");
        }

        //open file
        QFile scoreFile("./data/save/HighScores.sav");
        scoreFile.open(QIODevice::ReadWrite);
        QDataStream scoreData(&scoreFile);

        //read
        int scores[11] = { 0 }, n;
        QString names[11];
        for (n = 0; n < 10; n++) {
            if (scoreData.atEnd())
                break;
            scoreData >> names[n] >> scores[n];
        }

        //sort
        scores[n] = s1->getLength() - 1;
        while (n != 0) {
            if (scores[n] <= scores[n - 1])
                break;

            scores[n] = scores[n - 1];
            names[n] = names[n - 1];
            scores[--n] = s1->getLength() - 1;
        }
        names[n] = "You";

        //set label
        for (int i = 0; i < 10; i++) {
            endList[i + 1]->setText(QString("%1\t").arg(i + 1) + names[i].leftJustified(15, ' ').left(15) + QString("\t%1").arg(scores[i]));
        }

        if (n == 10) {
            endList[11]->setText("-\t" + names[n].leftJustified(15, ' ').left(15) + QString("\t%1").arg(scores[n]));
            inputHint->setText("You are expected to do even better.\nCome on!");

            resultTab->hide();
            listTab->show();
            nameInput->hide();

            emit gameLose();
        }
        else {
            QFont font;
            font.setFamily("Comic Sans MS");
            font.setBold(true);
            font.setItalic(true);
            font.setPixelSize(30);
            font.setLetterSpacing(QFont::AbsoluteSpacing, 2);

            endList[n + 1]->setFont(font);
            endList[11]->setText(QString("%1\t").arg(n + 1) + names[n].leftJustified(15, ' ').left(15) + QString("\t%1").arg(scores[n]));
            inputHint->setText("Congratulations!\nPlease input your name:");
            rank = n + 1;

            resultTab->hide();
            listTab->show();
            nameInput->show();
            nameInput->clear();
            nameInput->setFocus();
            connect(endQuit, &QPushButton::clicked, this, &GameScene::highScoreSave);

            emit gameWin();
        }
    }
    else {
        listTab->hide();
        resultTab->show();

        if (!s2) {
            result->setStyleSheet("color: white");
            result->setText(QString("Your Score: %1").arg(s1->getLength() - 1));
            detail[0]->setText("");
            detail[1]->setText("");
        }
        else {
            int score1 = s1->getLength() - 1;
            int score2 = s2->getLength() - 1;

            if (endFlag[1] && !endFlag[0])
                score1 += score2;
            else if (endFlag[0] && !endFlag[1])
                score2 += score1;

            if (score1 == score2) {
                result->setStyleSheet("color: white");
                result->setText("Draw");
            }
            else if (score1 < score2) {
                result->setStyleSheet("color: #FFB6C1");
                result->setText("P2 is the Winner!");
            }
            else {
                result->setStyleSheet("color: #ADD8E6");
                result->setText("P1 is the Winner!");
            }

            detail[0]->setText(QString("Player1 Score: %1").arg(score1));
            detail[1]->setText(QString("Player2 Score: %1").arg(score2));
        }
    }

    endMenu->raise();
    endQuit->raise();
    endMenu->show();

    emit gameWin();
}


void GameScene::gameSave()
{
    //find or creat dir
    QDir dir;
    if (!dir.exists("./data/save")) {
        if (!dir.exists("./data")) {
            dir.mkdir("./data");
        }
        dir.mkdir("./data/save");
    }

    //open file
    QFile saveFile("./data/save/save0.sav");
    saveFile.open(QIODevice::ReadWrite | QIODevice::Truncate);
    QDataStream saveData(&saveFile);

    ///---part1 : editablity---///
    saveData << mapEdit->isEnabled();

    ///---part2 : map data---///
    for (int y = 0; y < 30; y++) {
        for (int x = 0; x < 45; x++) {
            saveData << grid[x][y];
        }
    }

    ///---part3 : snake---///
    //snake 1
    s1->save(saveData);

    //snake 2
    if (s2) {
        saveData << true;
        s2->save(saveData);
    }
    else {
        saveData << false;
    }

}

void GameScene::mapSave()
{
    //open file
    QFile mapFile(mapPath);
    mapFile.open(QIODevice::ReadWrite | QIODevice::Truncate);
    QDataStream mapData(&mapFile);

    ///---part1: grid---///
    for (int y = 0; y < 30; y++) {
        for (int x = 0; x < 45; x++) {
            mapData << grid[x][y];
        }
    }

    ///---part2: directions---///
    mapData << s1->getDirecton();
    mapData << s2->getDirecton();
}

void GameScene::highScoreSave()
{
    ///---save high scores---///
    //find or creat dir
    QDir dir;
    if (!dir.exists("./data/save")) {
        if (!dir.exists("./data")) {
            dir.mkdir("./data");
        }
        dir.mkdir("./data/save");
    }

    //open file
    QFile scoreFile("./data/save/HighScores.sav");
    scoreFile.open(QIODevice::ReadWrite | QIODevice::Truncate);
    QDataStream scoreData(&scoreFile);

    //write
    QStringList tmp;
    for (int i = 1; i <= 10; i++) {
        tmp = endList[i]->text().split('\t');
        if (i == rank)
            tmp[1] = nameInput->text();
        scoreData << tmp[1] << tmp[2].toInt();
    }

    ///---reset label format---///
    QFont font;
    font.setFamily("Comic Sans MS");
    font.setBold(true);
    font.setPixelSize(30);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 2);
    endList[rank]->setFont(font);

    ///---disconnect---///
    disconnect(endQuit, &QPushButton::clicked, this, &GameScene::highScoreSave);

    scoreFile.close();
    emit highScoreUpdated();
}

void GameScene::edit(QString mapName)
{
    editBoard->show();
    editBoard->raise();
    editFlag = true;
    mapPath = "./data/map/" + mapName;
    editBtns[Grass]->click();

    //open file
    QFile mapFile(mapPath);
    mapFile.open(QIODevice::ReadWrite);
    QDataStream mapData(&mapFile);

    //creat default snake
    s1 = new Snake(QPoint(0, 0), QPoint(30, 0), this, 1);
    s1->move(550, 300);
    s2 = new Snake(QPoint(0, 0), QPoint(-30, 0), this, 2);
    s2->move(1300, 600);

    //load map
    if (mapData.atEnd()) {
        for (int i = 0; i < 45; i++) {
            for (int j = 0; j < 30; j++) {
                grid[i][j] = Grass;
            }
        }

        grid[10][10] = Snake1;
        grid[35][20] = Snake2;
    }
    else {
        ///---part1: grid---///
        for (int y = 0; y < 30; y++) {
            for (int x = 0; x < 45; x++) {
                mapData >> grid[x][y];
                switch (grid[x][y]) {
                case Snake1:
                    s1->move(x * 30 + 250, y * 30);
                    break;

                case Snake2:
                    s2->move(x * 30 + 250, y * 30);
                    break;

                case Wall:
                    linkBoard[x][y] = newWall(QPoint(x * 30 + 250, y * 30));
                    break;

                case Apple:
                    linkBoard[x][y] = newApple(QPoint(x * 30 + 250, y * 30));
                    break;

                case Donuts:
                    linkBoard[x][y] = newDonuts(QPoint(x * 30 + 250, y * 30));
                    break;

                case BlueBerry:
                    linkBoard[x][y] = newBlueberry(QPoint(x * 30 + 250, y * 30));
                    break;

                case Turkey:
                    linkBoard[x][y] = newTurkey(QPoint(x * 30 + 250, y * 30));
                    break;

                default:
                    break;
                }
            }
        }

        ///---part2: directions---///
        QPoint tmp;
        mapData >> tmp;
        s1->turn(tmp);
        mapData >> tmp;
        s2->turn(tmp);

    }

    inGame = true;
}

void GameScene::runtimeEdit()
{
    pauseMenu->hide();

    editFlag = true;
    editSave->hide();
    editQuit->hide();
    editBack->show();
    editBtns[Snake1]->setDisabled(true);
    editBtns[Snake2]->setDisabled(true);
    editBtns[Grass]->click();
    editBoard->show();
    editBoard->raise();
}


void GameScene::editSelect(MapItem item)
{
    editBtns[editItem]->setState(false);
    editItem = item;
    editBtns[editItem]->setState(true);
}

void GameScene::hide()
{
    inGame = false;

    if (s1) {
        delete s1;
        s1 = nullptr;
    }
    if (s2) {
        delete s2;
        s2 = nullptr;
    }
    blank.clear();
    for (int i = 0; i < 45; i++) {
        for (int j = 0; j < 30; j++) {
            if (linkBoard[i][j]) {
                delete linkBoard[i][j];
                linkBoard[i][j] = nullptr;
            }
        }
    }

    scoreBoard->hide();
    editBoard->hide();
    pauseMenu->hide();
    endMenu->hide();

    QWidget::hide();
}

void GameScene::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    QPixmap pix;

    //draw background
    pix.load(":/art/environment/grass.jpg");
    painter.drawPixmap(0, 0, pix);

    //draw info column
    pix.load(":/art/ui/base.png");
    painter.drawPixmap(0, 0, 250, 900, pix);


    return QWidget::paintEvent(ev);
}


void GameScene::keyPressEvent(QKeyEvent *ev)
{
    if (!inGame)
        return;
    if (endFlag[0] || endFlag[1] || editFlag)
        return;

    if (ev->key() == Qt::Key_Escape) {
        if (timer->isActive()) {
            timer->stop();
            scoreBoard->hide();
            pauseMenu->raise();
            pauseMenu->show();
        }
        else {
            pauseMenu->hide();
            scoreBoard->show();
            timer->start();
        }
    }
    if (s1 && !s1->isAuto()) {
        if (ev->key() == Qt::Key_W) {
            s1->turn(QPoint(0, -30));
        }
        else if (ev->key() == Qt::Key_A) {
            s1->turn(QPoint(-30, 0));
        }
        else if (ev->key() == Qt::Key_S) {
            s1->turn(QPoint(0, 30));
        }
        else if (ev->key() == Qt::Key_D) {
            s1->turn(QPoint(30, 0));
        }
    }
    if (s2 && !s2->isAuto()) {
        if (ev->key() == Qt::Key_Up) {
            s2->turn(QPoint(0, -30));
        }
        else if (ev->key() == Qt::Key_Left) {
            s2->turn(QPoint(-30, 0));
        }
        else if (ev->key() == Qt::Key_Down) {
            s2->turn(QPoint(0, 30));
        }
        else if (ev->key() == Qt::Key_Right) {
            s2->turn(QPoint(30, 0));
        }
    }
}

void GameScene::mousePressEvent(QMouseEvent *ev)
{
    if (!inGame)
        return;
    if (!editFlag || ev->button() != Qt::LeftButton || ev->x() < 250)
        return;

    int x = (ev->x() - 250) / 30;
    int y = ev->y() / 30;
    QPoint tmp;

    switch (grid[x][y]) {
    case Snake1:
        if (editItem == Snake1) {
            tmp.setX(-s1->getDirecton().y());
            tmp.setY(s1->getDirecton().x());
            s1->turn(tmp);
        }
        break;

    case Snake2:
        if (editItem == Snake2) {
            tmp.setX(-s2->getDirecton().y());
            tmp.setY(s2->getDirecton().x());
            s2->turn(tmp);
        }
        break;

    default:
        grid[x][y] = editItem;
        if (linkBoard[x][y]) {
            delete linkBoard[x][y];
            linkBoard[x][y] = nullptr;
        }
        switch (editItem) {
        case Snake1:
            tmp = s1->pos();
            grid[(tmp.x() - 250) / 30][tmp.y() / 30] = Grass;
            s1->move(x * 30 + 250, y * 30);
            break;

        case Snake2:
            tmp = s2->pos();
            grid[(tmp.x() - 250) / 30][tmp.y() / 30] = Grass;
            s2->move(x * 30 + 250, y * 30);
            break;

        case Grass:
            break;

        case Wall:
            linkBoard[x][y] = newWall(QPoint(x * 30 + 250, y * 30));
            break;

        case Apple:
            linkBoard[x][y] = newApple(QPoint(x * 30 + 250, y * 30));
            break;

        case Donuts:
            linkBoard[x][y] = newDonuts(QPoint(x * 30 + 250, y * 30));
            break;

        case BlueBerry:
            linkBoard[x][y] = newBlueberry(QPoint(x * 30 + 250, y * 30));
            break;

        case Turkey:
            linkBoard[x][y] = newTurkey(QPoint(x * 30 + 250, y * 30));
            break;
        }
    }
}

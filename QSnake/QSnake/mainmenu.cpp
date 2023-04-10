#include "mainmenu.h"

MainMenu::MainMenu(QWidget *parent) : QWidget(parent)
{
    ///---set window size---///
    setFixedSize(parent->size());

    ///---creat menus---///
    creatRootMenu();
    creatSelectMenu();
    creatInputDialog();
    creatSettingsMenu();
    creatHighScoreList();

    ///---manage connections---///
    manageConnections();

    ///---save check---///
    if (!mapDir.exists("./data/map")) {
        if (!mapDir.exists("./data")) {
            mapDir.mkdir("./data");
        }
        mapDir.mkdir("./data/map");
    }
    mapDir.setPath("./data/map");

    QFile saveFile("./data/save/save0.sav");
    conti->setEnabled(saveFile.open(QIODevice::ReadOnly | QIODevice::ExistingOnly));
}

void MainMenu::creatRootMenu()
{
    /// Summary:
    /// root menu set contains 2 menus which contain a set of button
    /// included in a widget
    /// present once the game start
    /// (total: 2 widget, 9 buttons)

    root = new QWidget(this);

    ///---creat first menu---///
    firstMenu = new QWidget(root);

    //creat button initialization list
    BtnInitialList bil;
    bil.parent = firstMenu;
    bil.size = QSize(270, 85);
    bil.pos.setX(width() / 2 - bil.size.width() / 2);
    int top = 320;
    int gap = 100;
    bil.iconNormal = QPixmap(":/art/ui/menuBtnNormal.png");
    bil.iconPressed = QPixmap(":/art/ui/menuBtnPressed.png");
    bil.font.setFamily("Comic Sans MS");
    bil.font.setBold(true);
    bil.font.setPixelSize(30);
    bil.font.setLetterSpacing(QFont::AbsoluteSpacing, 2);
    bil.style = "color: white";

    //creat continue button
    bil.text = "Continue";
    bil.pos.setY(top);
    conti = new PushButton(bil);

    //creat start button
    bil.text = "New Start";
    bil.pos.setY(top + gap);
    start = new PushButton(bil);

    //creat start button
    bil.text = "Map Edit";
    bil.pos.setY(top + gap * 2);
    mapEdit = new PushButton(bil);

    //set settings button
    bil.text = "Settings";
    bil.pos.setY(top + gap * 3);
    set = new PushButton(bil);

    //set exit button
    bil.text = "Quit";
    bil.pos.setY(top + gap * 4);
    exit = new PushButton(bil);


    ///---creat the secondary menu---///
    secondMenu = new QWidget(root);

    //update button initialization list
    bil.parent = secondMenu;
    top = 350;
    gap = 100;

    //creat challenge button
    bil.text = "Challenge";
    bil.pos.setY(top);
    challange = new PushButton(bil);

    //creat single player button
    bil.text = "Single Player";
    bil.pos.setY(top + gap);
    single = new PushButton(bil);

    //creat multiplayer button
    bil.text = "Multiplayer";
    bil.pos.setY(top + gap * 2);
    multi = new PushButton(bil);

    //creat back button
    bil.text = "Back";
    bil.pos.setY(top + gap * 3);
    startRootBack = new PushButton(bil);

    ///---visiable control---///
    firstMenu->show();
    secondMenu->hide();

}


void MainMenu::creatSelectMenu()
{
    /// Summary:
    /// select menu contains map select area, snake AI check boxs and back button
    /// included in a widget
    /// present before enter game in single or multiplayer mode
    /// (total: 2 button, 1 label, 1 scroll area, 2 check boxes)

    selectMenu = new QWidget(this);

    ///---buttons---///
    //button initialize list
    BtnInitialList bil;
    bil.parent = selectMenu;
    bil.size = QSize(270, 85);
    bil.font.setFamily("Comic Sans MS");
    bil.font.setBold(true);
    bil.font.setPixelSize(30);
    bil.font.setLetterSpacing(QFont::AbsoluteSpacing, 2);
    bil.style = "color: white";
    bil.iconNormal = QPixmap(":/art/ui/menuBtnNormal.png");
    bil.iconPressed = QPixmap(":/art/ui/menuBtnPressed.png");

    //back button
    bil.pos = QPoint(1250, 800);
    bil.text = "Back";
    selectBack = new PushButton(bil);

    //new map button
    bil.pos = QPoint(1250, 700);
    bil.text = "New Map";
    newMap = new PushButton(bil);

    ///---select area title---///
    mapSelect = new QLabel(selectMenu);
    mapSelect->setFixedSize(800, 100);
    mapSelect->move(400, 280);
    mapSelect->setText("Map Select");
    mapSelect->setFont(bil.font);
    mapSelect->setStyleSheet("color: white;");
    mapSelect->setAlignment(Qt::AlignCenter);

    ///---select area---///
    //scroll area style
    mapWidget = new QScrollArea(selectMenu);
    mapWidget->setFixedSize(800, 550);
    mapWidget->move(400, 280);
    mapWidget->setStyleSheet("QScrollArea\
                              {\
                                  border-top: 100px;\
                                  border-bottom: 50px;\
                                  border-left: 50px;\
                                  border-right: 50px;\
                                  border-image: url(:/art/ui/infoBox.png);\
                                  background-attachment: fixed;\
                              }");
    mapWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mapWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    mapWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical\
                                                   {\
                                                       border: 2px black;\
                                                       border-image: url(:/art/ui/scrollBox.png);\
                                                       width: 20px;\
                                                       margin: 20px 0 20px 0;\
                                                   }\
                                                   QScrollBar::handle:vertical\
                                                   {\
                                                       border-image: url(:/art/ui/scrollHandle.png);\
                                                       min-height: 20px;\
                                                   }\
                                                   QScrollBar::add-line:vertical\
                                                   {\
                                                       border-image: url(:/art/ui/scrollDown.png);\
                                                       height: 20px;\
                                                       subcontrol-position: bottom;\
                                                       subcontrol-origin: margin;\
                                                       border: 2px solid black;\
                                                   }\
                                                   QScrollBar::sub-line:vertical\
                                                   {\
                                                       border-image: url(:/art/ui/scrollUp.png);\
                                                       height: 20px;\
                                                       subcontrol-position: top;\
                                                       subcontrol-origin: margin;\
                                                       border: 2px solid black;\
                                                   }\
                                                   QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {\
                                                       background: none;\
                                                   }");
    readMapFiles();

    ///---check boxes---///
    //snake AI 1
    snakeAI_1 = new QCheckBox(selectMenu);
    snakeAI_1->setText("Snake AI 1");
    snakeAI_1->setFont(bil.font);
    snakeAI_1->setFixedSize(250, 75);
    snakeAI_1->move(1250, 350);
    snakeAI_1->setStyleSheet("QCheckBox\
                              {\
                                  color: black;\
                              }\
                              QCheckBox::indicator:unchecked\
                              {\
                                  image: url(:/art/ui/checkbox_blank.png);\
                              }\
                              QCheckBox::indicator:checked\
                              {\
                                  image: url(:/art/ui/checkbox_check.png);\
                              }");

    //snake AI 2
    snakeAI_2 = new QCheckBox(selectMenu);
    snakeAI_2->setText("Snake AI 2");
    snakeAI_2->setFont(bil.font);
    snakeAI_2->setFixedSize(250, 75);
    snakeAI_2->move(1250, 450);
    snakeAI_2->setStyleSheet("QCheckBox\
                              {\
                                  color: black;\
                              }\
                              QCheckBox::indicator:unchecked\
                              {\
                                  image: url(:/art/ui/checkbox_blank.png);\
                              }\
                              QCheckBox::indicator:checked\
                              {\
                                  image: url(:/art/ui/checkbox_check.png);\
                              }");

    ///---visiable control---///
    mapSelect->raise();
    snakeAI_1->hide();
    snakeAI_2->hide();
    newMap->hide();
    selectMenu->hide();
}

void MainMenu::readMapFiles(bool withDefault)
{
    //creat scroll area content (a layout in a widget)
    QWidget *subMapWidget = new QWidget;
    QVBoxLayout *mapLayout = new QVBoxLayout;

    //read map flies
    QStringList filter;
    filter << "*.smap";
    QStringList files = mapDir.entryList(filter, QDir::Files|QDir::Readable, QDir::Name);

    //set font
    QFont font;
    font.setFamily("Comic Sans MS");
    font.setBold(true);
    font.setPixelSize(30);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 2);

    //creat button list
    QToolButton *btn;
    QString s;

    if (withDefault) {
        btn = new QToolButton;
        btn->setAutoRaise(true);
        btn->setFont(font);
        btn->setStyleSheet("color: white");
        btn->setMinimumSize(640, 50);

        s = "";
        btn->setText("Default Map");

        connect(btn, &QToolButton::clicked, this, [=]()
        {
            emit mapSelected(s);
        });

        mapLayout->addWidget(btn);
    }

    for (int i = 0; i < files.size(); i++) {
        btn = new QToolButton;
        btn->setAutoRaise(true);
        btn->setFont(font);
        btn->setStyleSheet("color: white");
        btn->setMinimumSize(640, 50);

        s = files.at(i);
        btn->setText(s.left(s.length() - 5));

        connect(btn, &QToolButton::clicked, this, [=]()
        {
            emit mapSelected(s);
        });

        mapLayout->addWidget(btn);
    }

    //add content to the scroll area
    mapLayout->setAlignment(Qt::AlignTop);
    subMapWidget->setLayout(mapLayout);
    subMapWidget->setStyleSheet("QWidget { background-image: url(:/art/ui/base.png); }");
    subMapWidget->setMinimumSize(640, 400);
    mapWidget->setWidget(subMapWidget);
}

void MainMenu::creatInputDialog()
{
    /// Summary:
    /// input dialog contains line editor to get new map name and two buttons presenting "yes" or "no"
    /// included in a widget
    /// present when creat new map
    /// (total: 1 label, 1 line editor, 2 buttons)

    inputDialog = new QWidget(this);

    ///---background label---///
    QFont font;
    font.setFamily("Comic Sans MS");
    font.setBold(true);
    font.setPixelSize(30);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 2);

    dialogBox = new QLabel(inputDialog);
    dialogBox->setFixedSize(600, 320);
    dialogBox->move(width() / 2 - dialogBox->width() / 2, 400);
    dialogBox->setStyleSheet("border-image: url(:/art/ui/dialogBox.png); color: white;");
    dialogBox->setText("\n    Map Name:");
    dialogBox->setFont(font);
    dialogBox->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    ///---line editor---///
    lineEditor = new QLineEdit(inputDialog);
    lineEditor->setFixedSize(500, 70);
    lineEditor->move(width() / 2 - lineEditor->width() / 2, 500);
    lineEditor->setFont(font);

    ///---buttons---///
    BtnInitialList bil;
    bil.parent = inputDialog;
    bil.size = QSize(100, 100);
    bil.pos = QPoint(620, 585);
    bil.font = font;
    bil.style = "color: white";
    bil.iconNormal = QPixmap(":/art/ui/noNormal.png");
    bil.iconPressed = QPixmap(":/art/ui/noPressed.png");
    noBtn = new PushButton(bil);

    bil.pos.setX(880);
    bil.iconNormal = QPixmap(":/art/ui/yesNormal.png");
    bil.iconPressed = QPixmap(":/art/ui/yesPressed.png");
    yesBtn = new PushButton(bil);

    ///---visiable control---///
    inputDialog->hide();
}

void MainMenu::creatSettingsMenu()
{
    /// Summary:
    /// settings menu contains volume controller, score clear button and back button
    /// included in a widget
    /// present once settings button is pressed
    /// (total: 4 buttons, 2 slider)

    setMenu = new QWidget(this);


    ///---creat button initialization list---///
    BtnInitialList bil;
    bil.parent = setMenu;
    bil.size = QSize(270, 85);
    bil.pos.setX(width() / 2 - bil.size.width() / 2);
    bil.font.setFamily("Comic Sans MS");
    bil.font.setBold(true);
    bil.font.setPixelSize(30);
    bil.font.setLetterSpacing(QFont::AbsoluteSpacing, 2);
    bil.style = "color: white";
    bil.iconNormal = QPixmap(":/art/ui/menuBtnNormal.png");
    bil.iconPressed = QPixmap(":/art/ui/menuBtnPressed.png");


    ///---creat square buttons---///
    //creat back button
    bil.pos.setY(650);
    bil.text = "Back";
    setBack = new PushButton(bil);

    //creat score clear button
    bil.pos.setY(550);
    bil.text = "Clear Scores";
    scoreClear = new PushButton(bil);


    ///---creat circle buttons--///
    //reset initialization list
    bil.size = QSize(100, 100);
    bil.pos.setX(width() / 2 - bil.size.width() / 2 - 170);
    bil.text = "";
    bil.stateChangeable = true;

    //creat music button
    bil.pos.setY(285);
    bil.iconNormal = QPixmap(":/art/ui/musicOnNormal.png");
    bil.iconPressed = QPixmap(":/art/ui/musicOnPressed.png");
    bil.iconFalseNormal = QPixmap(":/art/ui/musicOffNormal.png");
    bil.iconFalsePressed = QPixmap(":/art/ui/musicOffPressed.png");
    music = new PushButton(bil);

    //creat sound button
    bil.pos.setY(405);
    bil.iconNormal = QPixmap(":/art/ui/soundOnNormal.png");
    bil.iconPressed = QPixmap(":/art/ui/soundOnPressed.png");
    bil.iconFalseNormal = QPixmap(":/art/ui/soundOffNormal.png");
    bil.iconFalsePressed = QPixmap(":/art/ui/soundOffPressed.png");
    bil.text = "";
    sound = new PushButton(bil);


    ///---creat sliders---///
    //set slider style sheet
    QString style = "QSlider::groove:horizontal \
                     { \
                         border-image: url(:/art/ui/sliderBox.png);\
                     } \
                     QSlider::handle:horizontal \
                     { \
                         width: 50px; \
                         height: 60px; \
                         margin-top: 2px; \
                         margin-left: 6px; \
                         margin-bottom: 2px; \
                         margin-right: 6px; \
                         border-image: url(:/art/ui/sliderPin.png); \
                     } \
                     QSlider::sub-page:horizontal \
                     { \
                         margin-top: 9px; \
                         margin-left: 10px; \
                         background-image: url(:/art/ui/sliderBar.png); \
                     }";

    //creat music slider
    musicVol = new QSlider(Qt::Horizontal, setMenu);
    musicVol->setFixedSize(340, 70);
    musicVol->move(width() / 2 - musicVol->width() / 2 + 50, 300);
    musicVol->setStyleSheet(style);
    musicVol->setValue(100);

    //creat sound slider
    soundVol = new QSlider(Qt::Horizontal, setMenu);
    soundVol->setFixedSize(340, 70);
    soundVol->move(width() / 2 - musicVol->width() / 2 + 50, 420);
    soundVol->setStyleSheet(style);
    soundVol->setValue(100);

    ///---visiable control---///
    setMenu->hide();

}

void MainMenu::creatHighScoreList()
{
    /// Summary:
    /// high score list contains a list of record labels and a button to control its presentation
    /// present once high score button is pressed
    /// (total: 1 button, 13 labels)

    ///---set font---///
    QFont font;
    font.setFamily("Comic Sans MS");
    font.setBold(true);
    font.setPixelSize(40);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 2);


    ///---creat list base---///
    listBase = new QLabel(this);
    listBase->setFixedSize(400, height());
    listBase->setPixmap(QPixmap(":/art/ui/base.png"));


    ///---creat score list button---///
    BtnInitialList bil;
    bil.parent = this;
    bil.size = QSize(100, 100);
    bil.pos = QPoint(0, 0);
    bil.iconNormal = QPixmap(":/art/ui/highScoreBtnNormal.png");
    bil.iconPressed = QPixmap(":/art/ui/highScoreBtnPressed.png");
    bil.text = "";
    scoreList = new PushButton(bil);


    ///---creat list title---///
    listTitle = new QLabel(listBase);
    listTitle->setFixedSize(listBase->width(), 100);
    listTitle->setText("High Scores ");
    listTitle->setFont(font);
    listTitle->setStyleSheet("color: white;");
    listTitle->setAlignment(Qt::AlignRight | Qt::AlignVCenter);


    ///---creat list of records---///
    font.setPixelSize(30);

    for (int i = 0; i <= 10; i++) {
        records[i] = new QLabel(listBase);
        records[i]->setFixedSize(listBase->width(), 60);
        records[i]->move(0, 100 + i * 70);
        records[i]->setFont(font);
        records[i]->setStyleSheet("color: white;");
        records[i]->setText(QString("  %1").arg(i));
        records[i]->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    }

    font.setItalic(true);
    records[0]->setFont(font);
    records[0]->setText("  No.\tName\tScore");

    records[1]->setStyleSheet("color: #FFD700;");
    records[2]->setStyleSheet("color: #C0C0C0;");
    records[3]->setStyleSheet("color: #8B4513");


    ///---visiable control---///
    listBase->hide();

}

void MainMenu::readHighScores()
{
    int scores[10] = { 0 };
    QString names[10];

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
    for (int i = 0; i < 10; i++) {
        if (!scoreData.atEnd()) {
            scoreData >> names[i] >> scores[i];
            records[i + 1]->setText(QString("  %1\t").arg(i + 1) + names[i].leftJustified(7, ' ').left(7) + QString("\t%1").arg(scores[i]));
        }
        else {
            records[i + 1]->setText(QString("  %1\t").arg(i + 1) + names[i].leftJustified(7, ' ').left(7) + QString("\t%1").arg(scores[i]));
        }
    }

}

void MainMenu::manageConnections()
{
    ///---connections for root menu---///
    //connect continue button
    connect(conti, &QPushButton::clicked, this, [=]()
    {
        emit enterGame(Mode::Load);
    });

    //connect start button
    connect(start, &QPushButton::clicked, this, [=]()
    {
        firstMenu->hide();
        secondMenu->show();
    });

    //connect map edit button
    connect(mapEdit, &QPushButton::clicked, this, [=]()
    {
        root->hide();
        readMapFiles(false);
        selectMenu->show();
        newMap->show();

        connect(this, &MainMenu::mapSelected, this, &MainMenu::editStart);
    });

    //connect settings button
    connect(set, &QPushButton::clicked, this, [=]()
    {
        root->hide();
        setMenu->show();
    });    

    //connect exit button
    connect(exit, &QPushButton::clicked, this, &MainMenu::quit);

    ///---connections for start menu---///
    //connect challenge button
    connect(challange, &QPushButton::clicked, this, [=]()
    {
        emit enterGame(Mode::Challenge);
        startRootBack->click();
    });

    //connect single player button
    connect(single, &QPushButton::clicked, this, [=]()
    {
        root->hide();
        readMapFiles();
        selectMenu->show();
        snakeAI_1->show();
        snakeAI_1->setText("Snake AI");

        connect(this, &MainMenu::mapSelected, this, &MainMenu::singleStart);
    });

    //connect multiplayer button
    connect(multi, &QPushButton::clicked, this, [=]()
    {
        root->hide();
        readMapFiles();
        selectMenu->show();
        snakeAI_1->show();
        snakeAI_1->setText("Snake AI 1");
        snakeAI_2->show();

        connect(this, &MainMenu::mapSelected, this, &MainMenu::multiStart);
    });

    //connect back button
    connect(startRootBack, &QPushButton::clicked, this, [=]()
    {
        secondMenu->hide();
        firstMenu->show();
    });

    ///---connections for select menu---///
    //connect new map button
    connect(newMap, &QPushButton::clicked, this, [=]()
    {
        inputDialog->show();
        lineEditor->clear();
        lineEditor->setFocus();
    });

    //connect back button
    connect(selectBack, &QPushButton::clicked, this, [=]()
    {
        selectMenu->hide();
        snakeAI_1->hide();
        snakeAI_2->hide();
        newMap->hide();
        root->show();

        disconnect(this, &MainMenu::mapSelected, this, &MainMenu::singleStart);
        disconnect(this, &MainMenu::mapSelected, this, &MainMenu::multiStart);
        disconnect(this, &MainMenu::mapSelected, this, &MainMenu::editStart);
    });

    ///---connections for input dialog---///
    //yes button
    connect(yesBtn, &QPushButton::clicked, this, [=]()
    {
        if (!lineEditor->text().length())
            return;
        QString mapName = lineEditor->text() + ".smap";
        QFile mapFile("./data/map/" + mapName);
        if (!mapFile.open(QIODevice::ReadWrite))
            return;
        if (!mapFile.atEnd())
            return;

        emit enterGame(Mode::Edit, lineEditor->text() + ".smap");
        noBtn->click();
        selectBack->click();
        startRootBack->click();
    });

    //no button
    connect(noBtn, &QPushButton::clicked, this, [=]()
    {
        inputDialog->hide();
    });

    ///---connections for settings menu---///
    //connect back button
    connect(setBack, &QPushButton::clicked, this, [=]()
    {
        setMenu->hide();
        root->show();
    });

    //connect score clear button
    connect(scoreClear, &QPushButton::clicked, this, [=]()
    {
        QFile::remove("./data/save/HighScores.sav");
        readHighScores();
    });

    //connect music button
    connect(music, &PushButton::stateChange, this, [=](bool state)
    {
        emit musicMuted(!state);
    });

    //connect sound button
    connect(sound, &PushButton::stateChange, this, [=](bool state)
    {
        emit soundMuted(!state);
    });

    //connect music slider
    connect(musicVol, &QSlider::valueChanged, this, [=](int value)
    {
        music->setState(true);
        emit musicVolChanged(float(value) / 100);
    });

    //connect sound slider
    connect(soundVol, &QSlider::valueChanged, this, [=](int value)
    {
        sound->setState(true);
        emit soundVolChanged(float(value) / 100);
    });


    ///---connections for high score list---///
    connect(scoreList, &QPushButton::clicked, this, [=]()
    {
        if (listBase->isVisible()) {
            listBase->hide();
        }
        else {
            readHighScores();
            listBase->show();
        }
    });

}


void MainMenu::singleStart(QString mapName)
{
    emit enterGame(Mode::Single, mapName, snakeAI_1->isChecked());
    selectBack->click();
    startRootBack->click();
}

void MainMenu::multiStart(QString mapName)
{
    emit enterGame(Mode::Multiplayer, mapName, snakeAI_1->isChecked(), snakeAI_2->isChecked());
    selectBack->click();
    startRootBack->click();
}

void MainMenu::editStart(QString mapName)
{
    emit enterGame(Mode::Edit, mapName);
    selectBack->click();
}


void MainMenu::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    QPixmap pix;

    //draw background
    pix.load(":/art/environment/grass.jpg");
    painter.drawPixmap(0, 0, pix);

    //draw title
    pix.load(":/art/icon/title.png");
    painter.drawPixmap(500, -70, 600, 350, pix);


    return QWidget::paintEvent(ev);
}

void MainMenu::show()
{
    QFile saveFile("./data/save/save0.sav");
    conti->setEnabled(saveFile.open(QIODevice::ReadOnly | QIODevice::ExistingOnly));

    readHighScores();

    QWidget::show();
}

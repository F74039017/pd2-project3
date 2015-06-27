#include "Mainwindow.h"
#include <QInputDialog>

bool Mainwindow::soundMute = false;  //-- true for test
bool Mainwindow::musicMute = false;

Mainwindow::Mainwindow(QWidget *parent, Qt::WindowFlags flags)
    :QMainWindow(parent, flags)
{
    setWindowTitle("Project3");

    /* add game window */
    game = new Game(this);
    setCentralWidget(game);

    /* create action */
        /* Mode */
    stepAct = new QAction("&Step", this);
    QObject::connect(stepAct, SIGNAL(triggered()), game, SLOT(setStepMode()));
    timeAct = new QAction("&Time", this);
    QObject::connect(timeAct, SIGNAL(triggered()), game, SLOT(setTimeMode()));
        /* Option */
    restartAct = new QAction("&Restart", this);
    QObject::connect(restartAct, SIGNAL(triggered()), game, SLOT(restart()));
    giveupAct = new QAction("&GiveUp", this);
    QObject::connect(giveupAct, SIGNAL(triggered()), game, SLOT(toIndexScene()));
        /* Mute */
    musicMuteAct = new QAction("&Music", this);
    QObject::connect(musicMuteAct, SIGNAL(triggered()), game, SLOT(muteMusic()));
    soundMuteAct = new QAction("&Sound", this);
    QObject::connect(soundMuteAct, SIGNAL(triggered()), this, SLOT(muteSound()));
        /* Info */
    mapper = new QSignalMapper(this);
    lastrecordAct = new QAction("&lastrecordAct", this);
    rule_enAct = new QAction("&rule_enAct", this);
    rule_zhAct = new QAction("&rule_zhAct", this);
    QObject::connect(lastrecordAct, SIGNAL(triggered()), mapper, SLOT(map()));
    QObject::connect(rule_enAct, SIGNAL(triggered()), mapper, SLOT(map()));
    QObject::connect(rule_zhAct,SIGNAL(triggered()), mapper, SLOT(map()));
    mapper->setMapping(lastrecordAct, 1);
    mapper->setMapping(rule_enAct, 2);
    mapper->setMapping(rule_zhAct, 3);
    QObject::connect(mapper, SIGNAL(mapped(int)), this, SLOT(showRule(int)));


    /* create menu */
        /* Mode */
    modeMenu = menuBar()->addMenu("&Mode");
    modeMenu->addAction(stepAct);
    modeMenu->addAction(timeAct);
        /* Option */
    optionMenu = menuBar()->addMenu("&Option");
    optionMenu->addAction(restartAct);
    optionMenu->addAction(giveupAct);
    optionMenu->setDisabled(true);
    QObject::connect(game, SIGNAL(toGame()), this, SLOT(menuEnable()));
    QObject::connect(game, SIGNAL(toIndex()), this, SLOT(menuDisable()));
        /* Mute */
    muteMenu = menuBar()->addMenu("&Mute");
    muteMenu->addAction(musicMuteAct);
    muteMenu->addAction(soundMuteAct);
        /* Info */
    infoMenu = menuBar()->addMenu("&Info");
    infoMenu->addAction(lastrecordAct);
    infoMenu->addAction(rule_enAct);
    infoMenu->addAction(rule_zhAct);

    /* rule dialog */
    ruleDia = new QMessageBox();
    ruleDia->setModal(false);   // unblock

    setFixedSize(sizeHint());   // disable to resize from drag border

    setWindowIcon(QIcon(":/images/images/star.png"));
}

Mainwindow::~Mainwindow()
{
    emit quit(Game::getlastStarNum(), Game::getlastScore());
    delete optionMenu;
    delete modeMenu;
    delete muteMenu;
    delete infoMenu;
    delete restartAct;
    delete giveupAct;
    delete stepAct;
    delete timeAct;
    delete soundMuteAct;
    delete musicMuteAct;
    delete lastrecordAct;
    delete rule_enAct;
    delete rule_zhAct;
    delete ruleDia;
    delete mapper;
    delete game;
}

void Mainwindow::startGame()
{
    game->init();   // start game
}

void Mainwindow::askUserName()  // input dialog
{
    bool ok;
    QInputDialog dia;
    dia.setModal(false);
    QString text = dia.getText(this, "Your name?", "User name:", QLineEdit::Normal, "Anonymous", &ok);
    if(!ok)
        game->setUserName("Anonymous");
    else
        game->setUserName(text);
}

void Mainwindow::closeEvent(QCloseEvent *)
{
    emit quit(Game::getlastStarNum(), Game::getlastScore());
}

void Mainwindow::menuEnable()
{
    optionMenu->setEnabled(true);
}

void Mainwindow::menuDisable()
{
    optionMenu->setDisabled(true);
}

void Mainwindow::muteSound()
{
    soundMute = !soundMute;
}

void Mainwindow::showRule(int x)
{
    if(x==1)
    {
        ruleDia->setText("Last Record");
        int score = game->getlastScore();
        int star = game->getlastStarNum();
        if(star||score)
            ruleDia->setInformativeText(QString("User: %1 \nScore: %2 \nStar: %3").arg(game->getuserName(), QString::number(score), QString::number(star)));
        else
            ruleDia->setInformativeText("No last record");
    }
    else if(x==2)
    {
        ruleDia->setText("Rule");
        ruleDia->setInformativeText("Step Mode:\nGet scores in 10 steps\n\nTime Mode:\nGet scores in 50 seconds\n\nStar:\n0~500\t  1 Star\n500~1000\t  2 Stars\n1000~1500\t  3 Stars");
    }
    else if(x==3)
    {
        ruleDia->setText("規則");
        ruleDia->setInformativeText("步數模式:\n在10步之內取得分數\n\n限時模式:\n在50秒內取得分數\n\n星星數:\n0~500\t  1 星\n500~1000\t  2 星\n1000~1500\t  3 星");
    }
    ruleDia->show();
}


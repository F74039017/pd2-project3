#include "Game.h"
#define dbName "user.db"
#define tbName "rank"
#include "Mainwindow.h"

int Game::lastScore = 0;
int Game::lastStarNum = 0;

Game::Game(QWidget * parent)
    :QGraphicsView(parent)
{
    /* construct gameScene */
    gameScene = new GameScene(this);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setMinimumSize(1000, 650);
    QObject::connect(gameScene, SIGNAL(pressBack()), this, SLOT(toIndexScene()));

    /* construct indexScene */
    indexScene = new IndexScene(this);
    setScene(indexScene);
    QObject::connect(indexScene, SIGNAL(pressStart()), this, SLOT(toGameScene()));

    /* BGM music */
    bgm = new QMediaPlayer();
    playList = new QMediaPlaylist();
    playList->addMedia(QUrl("qrc:/sounds/sounds/BGM.wav"));
    playList->setPlaybackMode(QMediaPlaylist::Loop);
    bgm->setPlaylist(playList);
    if(!Mainwindow::musicMute)
        bgm->play();

    /* Create database and user table */
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    if(!db.open())
    {
        qDebug() << "DataBase " << dbName <<" can't be opened";
        exit(EXIT_FAILURE);
    }
    else
    {
        QSqlQuery qry;
        qry.exec(QString("CREATE TABLE %1 (id INT PRIMARY KEY, name TEXT, score INT)").arg(tbName));
    }

    /* get last record from gamescene */
    QObject::connect(gameScene, SIGNAL(quit(int,int)), this, SLOT(setLastRecord(int,int)));
}

Game::~Game()
{
    delete gameScene;
    delete indexScene;
    delete bgm;
    delete playList;
}

void Game::init()
{
    gameScene->init();  // propagate
}

void Game::setUserName(QString name)
{
    userName = name;
}

QString Game::getuserName()
{
    return userName;
}

int Game::getlastScore()
{
    return lastScore;
}

int Game::getlastStarNum()
{
    return lastStarNum;
}

void Game::setLastRecord(int star, int score)
{
//    qDebug() << "Game Class get last record from gamescene: " << star << " " << score;    // demo
    lastStarNum = star;
    lastScore = score;
}

void Game::restart()
{
    init();
}

void Game::toGameScene()
{
    gameScene->init();
    setScene(gameScene);
    emit toGame();
}

void Game::toIndexScene()
{
    if(gameScene->getisAnimation())
        return;
    indexScene->resetIcon();
    setScene(indexScene);
    emit toIndex();
}

void Game::muteMusic()
{
    Mainwindow::musicMute = !Mainwindow::musicMute;
    if(Mainwindow::musicMute)
        bgm->pause();
    else
        bgm->play();
}

void Game::setStepMode()
{
    gameScene->setMode(GameScene::STEP);
    gameScene->init();
}

void Game::setTimeMode()
{
    gameScene->setMode(GameScene::TIME);
    gameScene->init();
}

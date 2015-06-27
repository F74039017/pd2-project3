#ifndef MAINWINDOW
#define MAINWINDOW

#include <QMainWindow>
#include <QMenuBar>
#include "Game.h"
#include <QMessageBox>
#include <QSignalMapper>

class Mainwindow: public QMainWindow
{
    Q_OBJECT

public:
    Mainwindow();
    ~Mainwindow();
    void startGame();
    void askUserName();
    static bool soundMute;
    static bool musicMute;

signals:
    void quit(int star, int score);

public slots:
    void menuEnable();
    void menuDisable();
    void muteSound();
    void showRule(int x);

private:
    QMenu *optionMenu;
    QMenu *modeMenu;
    QMenu *muteMenu;
    QMenu *infoMenu;
    QAction *restartAct;
    QAction *giveupAct;
    QAction *stepAct;
    QAction *timeAct;
    QAction *soundMuteAct;
    QAction *musicMuteAct;
    QAction *lastrecordAct;
    QAction *rule_enAct;
    QAction *rule_zhAct;
    QMessageBox *ruleDia;
    QSignalMapper *mapper;
    Game *game;
};


#endif // MAINWINDOW


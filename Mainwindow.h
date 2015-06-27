#ifndef MAINWINDOW
#define MAINWINDOW

#include <QMainWindow>
#include <QMenuBar>
#include "Game.h"

class Mainwindow: public QMainWindow
{
    Q_OBJECT

public:
    Mainwindow();
    void startGame();
    void askUserName();
    static bool soundMute;
    static bool musicMute;

public slots:
    void menuEnable();
    void menuDisable();
    void muteSound();

private:
    QMenu *optionMenu;
    QMenu *modeMenu;
    QMenu *muteMenu;
    QAction *restartAct;
    QAction *giveupAct;
    QAction *stepAct;
    QAction *timeAct;
    QAction *soundMuteAct;
    QAction *musicMuteAct;
    Game *game;
};


#endif // MAINWINDOW


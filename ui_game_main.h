/********************************************************************************
** Form generated from reading UI file 'game_main.ui'
**
** Created: Fri Oct 5 16:55:41 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAME_MAIN_H
#define UI_GAME_MAIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>
#include <QWidget>

QT_BEGIN_NAMESPACE

class Ui_Game_main
{
public:
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Game_main)
    {
        if (Game_main->objectName().isEmpty())
            Game_main->setObjectName(QString::fromUtf8("Game_main"));
        Game_main->resize(653, 494);
        centralWidget = new QWidget(Game_main);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        Game_main->setCentralWidget(centralWidget);
        centralWidget->setAttribute(Qt::WA_TransparentForMouseEvents);
        this->centralWidget->setMouseTracking(true);

        statusBar = new QStatusBar(Game_main);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        Game_main->setStatusBar(statusBar);

        retranslateUi(Game_main);

        QMetaObject::connectSlotsByName(Game_main);
    } // setupUi

    void retranslateUi(QMainWindow *Game_main)
    {
        Game_main->setWindowTitle(QApplication::translate("Game_main", "Game_main", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Game_main: public Ui_Game_main {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAME_MAIN_H

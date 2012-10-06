#include <QtGui/QApplication>
#include "game_main.h"
#include <iostream>
#include <QtCore/qstring.h>
#include <QtCore/qlist.h>
#include <QTextCodec>
#include "tds.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Game_main *w = new Game_main();
    QApplication::addLibraryPath("./plugins");
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
    w->show();

    return a.exec();
}

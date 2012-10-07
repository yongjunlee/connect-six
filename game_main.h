#ifndef GAME_MAIN_H
#define GAME_MAIN_H

#include <QMainWindow>
#include <QMainWindow>
#include <QThread>
#include <qmenu.h>
#include <qmenubar.h>
#include <qaction.h>
#include <qpainter.h>
#include <qwidget.h>
#include <qdialog.h>
#include <qimage.h>
#include <qtimer.h>
#include <QLabel>
#include <qlayout.h>
#include <algorithm>
#include <vector>
#include <string>
#include "checkifwin.h"
#include "playchess.h"
#include "tds.h"
#include <QTimer>
#include <stack>

#define WIDTH 26 //棋盘格子宽度
#define SCALE 19 //棋盘格子个数
#define START_X 420 //棋盘起始坐标x
#define START_Y 142  //棋盘起始坐标y
#define FIXED_X 1058  //主窗口长x
#define FIXED_Y 700  //主装口宽y
#define POS_X 200   //主窗口位置x
#define POS_Y 40   //主窗口位置y
#define MAP_LEN 528 //背景图片长度
#define MAP_WID 533 //背景图片宽度
#define WHITE_LABEL_X  150//白方名字的x坐标 如：“computer”
#define WHITE_LABEL_Y 335
#define BLACK_LABEL_X 150
#define BLACK_LABEL_Y 540
#define WHITE_LABEL_LENGTH
#define WHITE_LABEL_HEIGTH
using namespace std;

namespace Ui {
    class Game_main;
}

class Game_main : public QMainWindow
{
    Q_OBJECT

public:
    explicit Game_main(QWidget *parent = 0);
    ~Game_main();

    //定义函数
    void initData();//开始初始化数据
    void chooseMode();//选择游戏开始模式
    void playWhite(int x,int y);//白棋子着棋
    void playWhite();
    void playBlack(int x,int y);//黑棋子着棋
    //void playBlack();
    void whiteWin();//白棋子赢了的操作
    void blackWin();//黑棋子赢了的操作
    void gameOver();//游戏结束
    void drawChess(int POS[SCALE][SCALE]);//根据pos[][]绘画棋子
    void ifWin();//判断输赢
    void whoWin();
    void whoIsNext(int p[SCALE][SCALE]);
    void drawTrack(int x,int y);
    void diffs(int p[SCALE][SCALE],int p_tmp[SCALE][SCALE]);

    //定义数据
    bool is_start;//游戏是否开始
    bool is_over;//游戏是否结束
    bool iswhite_play;//是否白棋子着棋
    bool black_play;//是否黑棋子着棋
    bool white_win;//是否白色棋子赢了
    bool black_win;//是否黑色棋子赢了
    int who_win; //谁赢了，1白色，2黑色

    int test_count;
    int count_white;//白色棋子的个数
    int count_black;//黑色棋子的个数
    int playMode; //选择模式 取值为0表示无人模式，取值为一表示人机模式，2表示双人对战模式
    int track_x;
    int track_y;

    QString white_name;//白方名称 如：“computer”
    QString black_name;//黑方名称 如：”you“

    //棋子位置和坐标
    int pos[SCALE][SCALE]; //取值为1表示白色棋子，取值为2表示为黑色棋子，为0则无棋子
    int pos_tmp[SCALE][SCALE];
    int tmp_x,tmp_y;
    typedef struct node
    {
        int x;
        int y;
        int value;
    }Node;
    stack<Node> nodes;
    Node tmpNode;

private:
    Ui::Game_main *ui;
    //UI构建
    QPushButton *start_btn;
    QPushButton *about_btn;
    QPushButton *quit_btn;
    QPushButton *backBtn;

    QLabel *black_view;
    QLabel *white_view;

    QLabel *black_text;
    QLabel *white_text;

    QPainter *painter;
    QDialog * setPlayMode;
    QMouseEvent *mouseEvent;

    checkIfWin *ck;//判断输赢
    playChess *pc;
    tds *tmd;
    QTimer *qtimer;

protected:
    void changeEvent(QEvent *e);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *);
    void timerEvent(QTimerEvent *);
    void mouseMoveEvent(QMouseEvent * event);


    public slots:
        void start();
        void about();
        void quit();
        void setSingleMode();
        void setDoubleMode();
        void setNoneMode();
        void goBack();

     signals:


};

#endif // GAME_MAIN_H

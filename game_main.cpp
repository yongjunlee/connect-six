#include "game_main.h"
#include "ui_game_main.h"
#include <QtGui/QtGui>
#include <iostream>
#include "checkifwin.h"
#include "playchess.h"
#include "iomanip"
#include "fstream"
#include "tds.h"
#include <iostream>
#include <cstdio>
using namespace std;

Game_main::Game_main(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Game_main)
{
    ui->setupUi(this);

    //初始化UI
    start_btn = new QPushButton("Start",this);
    about_btn = new QPushButton("About",this);
    quit_btn = new QPushButton("Exit",this);
    backBtn = new QPushButton("back ",this);

    start_btn->setGeometry (80,100,80,60 );
    about_btn->setGeometry (280,100,80,60);
    quit_btn->setGeometry (180,100,80,60 );
    backBtn->setGeometry(0,0,80,60);

    setGeometry(POS_X,POS_Y,FIXED_X,FIXED_Y);
    setFixedSize(FIXED_X,FIXED_Y);
    setMouseTracking(true);
    //初始化数据
    initData();
    startTimer(1);
    //信号与槽
    connect(start_btn,SIGNAL(clicked()),this,SLOT(start()));
    connect(quit_btn,SIGNAL(clicked()),this,SLOT(quit()));
    connect(about_btn,SIGNAL(clicked()),this,SLOT(about()));
    connect(backBtn,SIGNAL(clicked()),this,SLOT(goBack()));
    connect(backBtn,SIGNAL(clicked()),this,SLOT(repaint()));
//    connect(tmd,SIGNAL(cook()),this,SLOT(repaint()));

}

Game_main::~Game_main()
{
    delete ui;
    delete start_btn;
    delete about_btn;
    delete quit_btn;

    delete painter;
    delete setPlayMode;
}

void Game_main::paintEvent(QPaintEvent *event)
{
    //绘画背景
    painter = new QPainter(this);
    QRectF target(START_X-29,START_Y-33,MAP_LEN,MAP_WID);
    QRectF source(0.0, 0.0, MAP_LEN,MAP_WID);
    QImage image_white(":/image/white.png");
    QImage image_black(":/image/black.png");
    QImage image_map(":/image/map.png");
    QImage image_bgr(":/image/bgr.png");
    QImage image_user_white(":/image/user.png");
    QImage image_user_black(":/image/computer.png");
    target.setRect(0,0,FIXED_X,FIXED_Y);
    source.setRect(0,0,FIXED_X,FIXED_Y);
    painter->drawImage(target, image_bgr, source);
    target.setRect(START_X-29,START_Y-33,FIXED_X,FIXED_Y);
    painter->drawImage(target,image_map,source);
    target.setRect(150,200,300,200);
    painter->drawImage(target,image_user_black,source);
    target.setRect(150,400,300,200);
    painter->drawImage(target,image_user_white,source);
    QFont font("System", 16, QFont::Bold);
    painter->setFont(font);
    painter->drawText(WHITE_LABEL_X,WHITE_LABEL_Y,"白棋");
    painter->drawText(BLACK_LABEL_X,BLACK_LABEL_Y,"黑棋");
    painter->setFont(QFont("System",12));
//    painter->drawText(300,90,QString("开局采集程序Demo,黑棋个数达到三个，程序重新开始！结果保存在result.txt中"));

    if(track_x >=0 &&track_x<=18&&track_y<=18&& track_y >= 0)
    {
        drawTrack(track_x,track_y);
        cout<<"track_x:"<<track_x<<endl;
    }
    whoIsNext(pos);
    //下棋子
    drawChess(pos);//画棋子
    if(!is_over)
      {
        who_win = pc->whoWin(pos);
      }
    switch(who_win)
      {
      case 1:
        white_win =true;break;
      case 2:
        black_win = true;break;
      case 0:
        black_win = false;
        white_win = false;
        break;
      }

    if(!is_over)whoWin();

}
void Game_main::initData()
{
    is_start = false;
    count_white = 0;
    count_black = 0;
    is_over = false;
    test_count = 0;
    playMode = 2;
    white_win = false;
    black_win = false;
    track_x = -1;
    track_y = -1;
    tmp_x = -1;
    tmp_y = -1;
    who_win =0;
    for(int i = 0; i<SCALE;i++)
    {
        for(int j = 0; j<SCALE;j++)
        {
            pos[i][j]=0;
        }
    }
}


/*
  *点击开始执行此函数
  *
  */
void Game_main::start()
{

    initData();
    is_start =true;
    start_btn->setText("Restart");

    chooseMode();
}

/*
  *点击“关于”执行此函数
  *弹出对话框
  */
void Game_main::about()
{
    QDialog *Dblack = new QDialog();
    QVBoxLayout *vlayout = new QVBoxLayout;
    Dblack->setFixedSize(300,280);
    QLabel *label = new QLabel("Copy Right @ UESTC-CCSE wytk2008.net");
    QAbstractButton *bExit = new QPushButton("back");
    vlayout->addWidget(label);
    vlayout->addWidget(bExit);
    Dblack->setLayout(vlayout);
    Dblack->show();
    Dblack->connect(bExit,SIGNAL(clicked()),Dblack,SLOT(close()));
}

/*
  *点击退出按钮，退出程序
  *
  */
void Game_main::quit()
{
    setPlayMode->close();
    close();
}

void Game_main::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/*
  *鼠标点击事件响应
  *
  */
void Game_main::mousePressEvent(QMouseEvent *event)
{
    if(!is_over)
    {
        if(event->x() >=START_X-WIDTH && event->x() <= START_X+(SCALE-1)*WIDTH+WIDTH
                && event->y() >= START_Y-WIDTH && event->y() <=START_Y+WIDTH*(SCALE-1)+WIDTH)
        {
            if(playMode == 0)
            {

            }
            else if(playMode == 1)//默认用户为黑棋，电脑为白棋
            {
                if(count_white + count_black ==0 || count_black < count_white || (count_black ==count_white && count_black%2==0))
                {
                    playBlack(event->x(),event->y());
                    repaint();
                      if(count_white + count_black ==0 || count_black < count_white || (count_black ==count_white && count_black%2==0))
                      {
                      }
                    else
                      {
                        if(pc->whoWin(pos) == 0)
                        {
                        pc->putChesses(pos,&count_white,&count_black);
                        repaint();
                        }

                      }
                }

            }
            else if(playMode == 2)
            {
                if(count_white + count_black ==0 || count_black < count_white || (count_black ==count_white && count_black%2==0))
                {

                    playBlack(event->x(),event->y());
                    repaint();
                }
                else
                {
                    playWhite(event->x(),event->y());
                    repaint();

                    }
                }
            }
         }
    }
 void Game_main::mouseMoveEvent(QMouseEvent * event)
 {
     int x=-1;
     int y=-1;
     int event_x = event->x();
     int event_y = event->y();
     if(event_x >=START_X-WIDTH && event_x <= START_X+(SCALE-1)*WIDTH+WIDTH
             && event_y >= START_Y-WIDTH && event_y <=START_Y+WIDTH*(SCALE-1)+WIDTH)
     {
         if((event_x-START_X)%WIDTH >WIDTH/2)
         {
             x=(event_x-START_X)/WIDTH+1;
         }
         else x=(event_x-START_X)/WIDTH;
         if((event_y-START_Y)%WIDTH >WIDTH/2)
         {
             y=(event_y-START_Y)/WIDTH+1;
         }
         else y=(event_y-START_Y)/WIDTH;
     }
     track_x = x;
     track_y = y;
     repaint();
 }

/*
  *根据二维数组绘画棋子
  *
  */
void Game_main::drawChess(int POS[SCALE][SCALE])
{
    painter = new QPainter(this);
    QRectF target(START_X-29,START_Y-33,MAP_LEN,MAP_WID);
    QRectF source(0.0, 0.0, MAP_LEN,MAP_WID);
    QImage image_white(":/image/white.png");
    QImage image_black(":/image/black.png");

    //绘画棋子
    if(is_start)
    {
        for(int i = 0; i<SCALE; i++)
        {
            for(int j = 0; j<SCALE; j++)
            {
                switch(POS[i][j])
                {
                case 0:

                    break;
                case 1:
                    painter->setPen(Qt::black);
                    painter->setBrush(Qt::black);
                    target.setRect(START_X+WIDTH*i-WIDTH/2,START_Y+WIDTH*j-WIDTH/2,WIDTH,WIDTH);
                    source.setRect(0,0,WIDTH,WIDTH);
                    painter->drawImage(target, image_white, source);
                    break;
                case 2:
                    painter->setPen(Qt::red);
                    painter->setBrush(Qt::red);
                    target.setRect(START_X+WIDTH*i-WIDTH/2,START_Y+WIDTH*j-WIDTH/2,WIDTH,WIDTH);
                    source.setRect(0,0,WIDTH,WIDTH);
                    painter->drawImage(target, image_black, source);

                    break;
                default:
                    break;
                }

            }
        }
    }//绘画棋子结束
}

//白棋着子
void Game_main::playBlack(int event_x,int event_y)
{
    int x,y;
    if(playMode == 1 || playMode == 2)
    {
        if(event_x >=START_X-WIDTH && event_x <= START_X+(SCALE-1)*WIDTH+WIDTH
                && event_y >= START_Y-WIDTH && event_y <=START_Y+WIDTH*(SCALE-1)+WIDTH)
        {
            if((event_x-START_X)%WIDTH >WIDTH/2)
            {
                x=(event_x-START_X)/WIDTH+1;
            }
            else x=(event_x-START_X)/WIDTH;
            if((event_y-START_Y)%WIDTH >WIDTH/2)
            {
                y=(event_y-START_Y)/WIDTH+1;
            }
            else y=(event_y-START_Y)/WIDTH;
        }
        if(pos[x][y] ==0)
        {
            pos[x][y] = 2;
            tmp_x = x; tmp_y = y;
            count_black++;
            repaint();
        }

    }
    if(playMode == 0)
    {
        //playchess->playWhite();-------------------------添加算法

    }
    black_win = ck->ifWin(pos,x,y);

}

//黑棋着子
void Game_main::playWhite(int event_x,int event_y)
{
    int x,y;
    if(playMode == 1)
    {
        //playchess----------------------------------------添加算法
        pc->putChesses(pos,&count_white,&count_black);
        repaint();
    }
    if(playMode == 2)
    {
        if(event_x >=START_X-WIDTH && event_x <= START_X+(SCALE-1)*WIDTH+WIDTH
                && event_y >= START_Y-WIDTH && event_y <=START_Y+WIDTH*(SCALE-1)+WIDTH)
        {
            if((event_x-START_X)%WIDTH >WIDTH/2)
            {
                x=(event_x-START_X)/WIDTH+1;
            }
            else x=(event_x-START_X)/WIDTH;
            if((event_y-START_Y)%WIDTH >WIDTH/2)
            {
                y=(event_y-START_Y)/WIDTH+1;
            }
            else y=(event_y-START_Y)/WIDTH;
        }
        if(pos[x][y] ==0)
        {
            pos[x][y] = 1;
            tmp_x = x;tmp_y = y;
            count_white++;
            repaint();
        }

    }
    white_win = ck->ifWin(pos,x,y);
}

/*
  *选择游戏模式
  *机机模式，人机模式，人人模式
  */
void Game_main::chooseMode()
{
    setPlayMode = new QDialog();
    QHBoxLayout *dlgLayout = new QHBoxLayout;
    QPushButton *SingleBtn = new QPushButton(tr("人机对战"));
    QPushButton *DoubleBtn = new QPushButton(tr("双人模式"));
    QPushButton *NoneBtn = new QPushButton(tr("机机对战"));
    QPushButton *ExitBtn = new QPushButton(tr("退出"));
    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(60);
    btnLayout->addWidget(SingleBtn);
    btnLayout->addWidget(DoubleBtn);
    btnLayout->addWidget(NoneBtn);
    btnLayout->addWidget(ExitBtn);

    QVBoxLayout *pngLayout = new QVBoxLayout;
    dlgLayout->setMargin(40);
    dlgLayout->addLayout(pngLayout);
    dlgLayout->setMargin(40);
    dlgLayout->addLayout(btnLayout);
    setPlayMode->setLayout(dlgLayout);
    setPlayMode->show();
    setPlayMode->connect(SingleBtn,SIGNAL(clicked()),this,SLOT(setSingleMode()));
    setPlayMode->connect(DoubleBtn,SIGNAL(clicked()),this,SLOT(setDoubleMode()));
    setPlayMode->connect(NoneBtn,SIGNAL(clicked()),this,SLOT(setNoneMode()));
    setPlayMode->connect(ExitBtn,SIGNAL(clicked()),this,SLOT(quit()));

}

//三种模式的情况，机机对战，人机对战，人人对战
void Game_main::setSingleMode()
{
    setPlayMode->close();
    playMode = 1;
}
void Game_main::setDoubleMode()
{
    setPlayMode->close();
    playMode = 2;
}
void Game_main::setNoneMode()
{
    setPlayMode->close();
    playMode = 0;
    while(!white_win && !black_win)
    {
        if(count_white + count_black ==0 || count_white < count_black || (count_white ==count_black && count_white%2==0))
        {
            //white_play()------------------添加算法
             pc->putChesses(pos,&count_white,&count_black);
            repaint();
        }
        else
        {
            //black_play()------------------添加算法
             pc->putChesses(pos,&count_white,&count_black);
            repaint();
        }
    }
}

void Game_main::whoWin()//直接判断white_win,black_win弹出窗口
{
    if(white_win)
    {
        QDialog *Dblack = new QDialog();
        QVBoxLayout *vlayout = new QVBoxLayout;
        Dblack->setFixedSize(150,140);
        QLabel *label = new QLabel("白棋胜利！");
        QAbstractButton *bExit = new QPushButton("再来一局");

        bExit->setText("再来一局");

        vlayout->addWidget(label);
        vlayout->addWidget(bExit);
        Dblack->setLayout(vlayout);
        Dblack->show();
        Dblack->connect(bExit,SIGNAL(clicked()),Dblack,SLOT(close()));
        Dblack->connect(bExit,SIGNAL(clicked()),this,SLOT(start()));

        is_over = true;


    }
    if(black_win)
    {
        QDialog *Dblack = new QDialog();
        QVBoxLayout *vlayout = new QVBoxLayout;
        Dblack->setFixedSize(150,140);
        QLabel *label = new QLabel("黑棋胜利！");
        QAbstractButton *bExit = new QPushButton("再来一局");

        bExit->setText("再来一局");

        vlayout->addWidget(label);
        vlayout->addWidget(bExit);
        Dblack->setLayout(vlayout);
        Dblack->show();
        Dblack->connect(bExit,SIGNAL(clicked()),Dblack,SLOT(close()));
        Dblack->connect(bExit,SIGNAL(clicked()),this,SLOT(start()));

        is_over =true;
    }
}
void Game_main::whoIsNext(int p[SCALE][SCALE])
{
  QRectF target(START_X-29,START_Y-33,MAP_LEN,MAP_WID);
  QRectF source(0.0, 0.0, MAP_LEN,MAP_WID);
  QImage image_red(":/image/red.png");
  QImage image_green(":/image/green.png");
  QImage image_user_white(":/image/user.png");
  QImage image_user_black(":/image/computer.png");

  painter->setFont(QFont("system",16));
  painter->setPen(Qt::black);
  painter->setBrush(Qt::black);
  source.setRect(0,0,FIXED_X,FIXED_Y);
  painter->setOpacity(1);
    switch(pc->WhoisNext(pos))
      {
        case 1:
            if(rand()%2)painter->setOpacity(0.0);
             else painter->setOpacity(1);
            target.setRect(280,200,300,200);
            painter->drawText(target,"playing");

            painter->setOpacity(1);
            target.setRect(280,400,300,200);
            painter->drawText(target,"thinking");


        break;
    case 2:
        if(rand()%2)painter->setOpacity(0.0);
        else painter->setOpacity(1);
            target.setRect(280,400,300,200);
            painter->drawText(target,"playing");
            painter->setOpacity(1);
            target.setRect(280,200,300,200);
            painter->drawText(target,"thinking");

        break;


      }
}

void Game_main::timerEvent(QTimerEvent *)
{
    repaint();
}

void Game_main::drawTrack(int x, int y)
{

    int i = x;
    int j = y;
    painter = new QPainter(this);
    QRectF target(START_X-29,START_Y-33,MAP_LEN,MAP_WID);

    painter->setPen(Qt::green);
    painter->setBrush(Qt::white);
    painter->setOpacity(0.5);

    target.setRect(START_X+WIDTH*i-WIDTH/2,START_Y+WIDTH*j-WIDTH/2,WIDTH,WIDTH);
    painter->drawRect(target);
    cout<<"x:"<<x<<"y:"<<y<<endl;
}

void Game_main::goBack()
{
    if(tmp_x >= 0 && tmp_y >=0)
    {
        switch(pos[tmp_x][tmp_y])
        {
        case 1:
            count_white--;
            break;
        case 2:
            count_black--;
            break;
        }
         pos[tmp_x][tmp_y] = 0;
    }
    cout<<"tmpx:"<<tmp_x<<" tmpy"<<tmp_y<<endl;
    repaint();
}

#include "playchess.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
using namespace std;

playChess::playChess()
{
    //yongjunlee add
    ckif = new checkIfWin();
    //end
}
/*
  *输入一个二维数组，输出二维数组
  *或者直接传址运算
  */

bool playChess::cmp(node a,node b)
{
    return a.score>b.score;
}

int playChess::WhoisNext(int p[19][19])//判定下一步该谁下字
{
    int i,j;
    int cc[3]={0,0,0};//Count the curent number of each color
    for(i = 0; i < SCALE; i++)
            for(j = 0; j < SCALE; j++)
                cc[p[i][j]]++;
    //printf("%d %d\n",cc[1],cc[2]);
    if (cc[1]<cc[2])
        return 1;
    else if (cc[1]==cc[2])
    {
        if (cc[1]%2==1)
            return 1;
        else
            return 2;
    }
        return 2;

}

int playChess::EvaluateChessStatus(int p[19][19],int MyColor)
{
    int Enemy = 3 - MyColor;//定义对手棋子颜色
    double ScoreOfRoad[13] = {0,17,78,141,788,2040,1.0,-17,-78,-288,-3000,-8600,-1.0};
    int i,j,k,tot = 0,res = 0;
    int NumberOfMyRoad[7]= {0,0,0,0,0,0,0},NumberOfEnemyRoad[7]= {0,0,0,0,0,0,0};
    int Road[13];
    //Line 对0~18行进行遍历
    for(i = 0; i < SCALE; i++)
        for(j=0; j < SCALE-6+1; j++)
        {
            int T_My=0,T_Enemy=0;
            for(k=j; k<j+6; k++)
            {
                if (p[i][k]==MyColor)
                    T_My++;
                else if (p[i][k]==Enemy)
                    T_Enemy++;
            }
            //路上必须没有其他颜色的棋子，才算有效路，则记录++
            if (T_Enemy==0)
                NumberOfMyRoad[T_My]++;
            else if (T_My==0)
                NumberOfEnemyRoad[T_Enemy]++;
        }
    //Column 对0~18列进行遍历
    for(j = 0; j < SCALE; j++)
        for(i=0; i < SCALE-6+1; i++)
        {
            int T_My=0,T_Enemy=0;
            for(k=i; k<i+6; k++)
            {
                if (p[k][j]==MyColor)
                    T_My++;
                else if (p[k][j]==Enemy)
                    T_Enemy++;
            }
            if (T_Enemy==0)
                NumberOfMyRoad[T_My]++;
            else if (T_My==0)
                NumberOfEnemyRoad[T_Enemy]++;

        }
    //Left-X 对"/"型路进行遍历
    for(i = 5; i < SCALE; i++)
        for(j = 0; j < SCALE-6+1; j++)
        {
            int T_My=0,T_Enemy=0;
            for(k=0; k<6; k++)
            {
                if (p[i-k][j+k]==MyColor)
                    T_My++;
                else if (p[i-k][j+k]==Enemy)
                    T_Enemy++;
            }
            if (T_Enemy==0)
                NumberOfMyRoad[T_My]++;
            else if (T_My==0)
                NumberOfEnemyRoad[T_Enemy]++;

        }
    //Right-X 对"\"路进行遍历
    for(i = 0; i < SCALE-6+1; i++)
        for(j = 0; j < SCALE-6+1; j++)
        {
            int T_My=0,T_Enemy=0;
            for(k=0; k<6; k++)
            {
                if (p[i+k][j+k]==MyColor)
                    T_My++;
                else if (p[i+k][j+k]==Enemy)
                    T_Enemy++;
            }
            if (T_Enemy==0)
                NumberOfMyRoad[T_My]++;
            else if (T_My==0)
                NumberOfEnemyRoad[T_Enemy]++;

        }
    for(i=1; i<=6; i++)
    {
        Road[i] = NumberOfMyRoad[i];
        Road[i+6] = NumberOfEnemyRoad[i];
    }
    double score=0;
    for(i=1;i<13;i++)
    {
        score += Road[i]*ScoreOfRoad[i];
    }
    return score;
}

int playChess::Legal(int i1,int j1,int i2,int j2)
{
    int id[4];
    id[0] = i1;id[1] = j1;id[2] = i2;id[3] = j2;
    for (int x=0;x<4;x++)
    {
        if (id[x]<0 || id[x]>=SCALE)
            return 0;
    }
    return 1;
}


playChess::node playChess::makenode(int i1,int j1,int i2,int j2,int score)
{
    node pt;
    pt.x1 = i1;
    pt.y1 = j1;
    pt.x2 = i2;
    pt.y2 = j2;
    pt.score = score;
    return pt;
}

 int playChess::putChesses(int p[SCALE][SCALE],int *count_white,int *count_black)
{
    int i,j,MyColor;
    int cc[3]={0,0,0};//Count the curent number of each color
    for(i = 0; i < SCALE; i++)
        for(j = 0; j < SCALE; j++)
            cc[p[i][j]]++;
    srand(unsigned(time(0)));
    if (cc[1]==0 && cc[2]==0)
    {
        int x = 7+int(5*rand()/(RAND_MAX+1.0));
        int y = 7+int(5*rand()/(RAND_MAX+1.0));
        p[x][y] = 2;
        ( *count_black) += 1;
        return 0;
    }
    vector<node> Queue;
    MyColor = WhoisNext(p);
    Queue.clear();
    if (cc[1]==0 && cc[2]==1)
    {
        int x,y;
        i = 0;j = 0;
        for(x = 0; x < SCALE; x++)
            for(y = 0; y < SCALE; y++)
                if (p[x][y] == 2)
                {
                    i = x;
                    j = y;
                }
        if (Legal(i-1,j,i,j+1))
            Queue.push_back(makenode(i-1,j,i,j+1,4*rand()/(RAND_MAX+1.0)));
        if (Legal(i-1,j,i,j-1))
            Queue.push_back(makenode(i-1,j,i,j-1,4*rand()/(RAND_MAX+1.0)));
        if (Legal(i+1,j,i,j-1))
            Queue.push_back(makenode(i+1,j,i,j-1,4*rand()/(RAND_MAX+1.0)));
        if (Legal(i+1,j,i,j+1))
            Queue.push_back(makenode(i+1,j,i,j+1,4*rand()/(RAND_MAX+1.0)));
    }
    else
    {
        int i1,i2,j1,j2;
        for(i1 = 0; i1 < SCALE; i1++)
        for(j1 = 0; j1 < SCALE; j1++)
            for(i2 = 0;i2 < SCALE;i2++)
            for(j2 = 0;j2 < SCALE;j2++)
                if (abs(i1-i2)+abs(j1-j2)!=0 && p[i1][j1] == 0 && p[i2][j2] == 0)
                {
                    p[i1][j1] = MyColor;
                    p[i2][j2] = MyColor;
                    Queue.push_back(makenode(i1,j1,i2,j2,EvaluateChessStatus(p,MyColor)));
                    p[i1][j1] = 0;
                    p[i2][j2] = 0;
                }
    }
    //std::sort(Queue.begin(),Queue.end(),cmp);
    for(int it=0;it<Queue.size();it++)
    {
        if (Queue[it].score>Queue[0].score)
        {
            Queue[0] = Queue[it];
        }
    }
    cout << Queue[0].score << endl;
    i = Queue[0].x1;
    j = Queue[0].y1;
    p[i][j] = MyColor;
    switch(MyColor)
      {
      case 1:
      ( *count_white)++;break;
      case 2:
      ( *count_black)++;break;
      }

    i = Queue[0].x2;
    j = Queue[0].y2;
    p[i][j] = MyColor;
    switch(MyColor)
      {
      case 1:
      ( *count_white)++;break;
      case 2:
      ( *count_black)++;break;
      }

    return 0;
}

int playChess:: whoWin(int p[SCALE][SCALE])
 {
     int MyColor = 1;
     int Enemy = 3 - MyColor;//定义对手棋子颜色
     int i,j,k,tot = 0,res = 0;
     int NumberOfMyRoad[7]= {0,0,0,0,0,0,0},NumberOfEnemyRoad[7]= {0,0,0,0,0,0,0};
     //Line 对0~18行进行遍历
     for(i = 0; i < SCALE; i++)
         for(j=0; j < SCALE-6+1; j++)
         {
             int T_My=0,T_Enemy=0;
             for(k=j; k<j+6; k++)
             {
                 if (p[i][k]==MyColor)
                     T_My++;
                 else if (p[i][k]==Enemy)
                     T_Enemy++;
             }
             //路上必须没有其他颜色的棋子，才算有效路，则记录++
             if (T_Enemy==0)
                 NumberOfMyRoad[T_My]++;
             else if (T_My==0)
                 NumberOfEnemyRoad[T_Enemy]++;
         }
     //Column 对0~18列进行遍历
     for(j = 0; j < SCALE; j++)
         for(i=0; i < SCALE-6+1; i++)
         {
             int T_My=0,T_Enemy=0;
             for(k=i; k<i+6; k++)
             {
                 if (p[k][j]==MyColor)
                     T_My++;
                 else if (p[k][j]==Enemy)
                     T_Enemy++;
             }
             if (T_Enemy==0)
                 NumberOfMyRoad[T_My]++;
             else if (T_My==0)
                 NumberOfEnemyRoad[T_Enemy]++;

         }
     //Left-X 对"/"型路进行遍历
     for(i = 5; i < SCALE; i++)
         for(j = 0; j < SCALE-6+1; j++)
         {
             int T_My=0,T_Enemy=0;
             for(k=0; k<6; k++)
             {
                 if (p[i-k][j+k]==MyColor)
                     T_My++;
                 else if (p[i-k][j+k]==Enemy)
                     T_Enemy++;
             }
             if (T_Enemy==0)
                 NumberOfMyRoad[T_My]++;
             else if (T_My==0)
                 NumberOfEnemyRoad[T_Enemy]++;

         }
     //Right-X 对"\"路进行遍历
     for(i = 0; i < SCALE-6+1; i++)
         for(j = 0; j < SCALE-6+1; j++)
         {
             int T_My=0,T_Enemy=0;
             for(k=0; k<6; k++)
             {
                 if (p[i+k][j+k]==MyColor)
                     T_My++;

                else if (p[i+k][j+k]==Enemy)
                     T_Enemy++;
             }
             if (T_Enemy==0)
                 NumberOfMyRoad[T_My]++;
             else if (T_My==0)
                 NumberOfEnemyRoad[T_Enemy]++;

         }
     if (NumberOfMyRoad[6]>0)
         return MyColor;
     else if (NumberOfEnemyRoad[6]>0)
         return Enemy;
     else
         return 0;
 }

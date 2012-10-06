#include "tds.h"
#include <iostream>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <vector>
using namespace std;

tds::tds()
{
    int temp_t[13] = {0,52.7963,92.4191,144.409,788.793,2040.64,30000,14.0648,-70.9723,-287.043,-3000
                  -8600,-50000};
    for(int i=0;i<13;i++)
    {
        theta[i] = temp_t[i];
    }
    int temp_tw[13] = {0,17,78,141,788,2040,30000,-17,-78,-288,-3000,-8600,-50000};
    for(int i=0;i<13;i++)
    {
        theta_white[i] = temp_tw[i];
    }


}

int tds::Initialize()
{
    for(int i=0;i<19;i++)
        for(int j=0;j<19;j++)
            p[i][j] = 0;
    for (int i=0;i<13;i++)
        fai[i] = 0;
    return 0;
}
int tds::Legal(int i1,int j1,int i2,int j2)
{
    int id[4];
    id[0] = i1;id[1] = j1;
    id[2] = i2;id[3] = j2;
    for (int x=0;x<4;x++)
        if (id[x]>=SCALE || id[x]<0)
            return 0;
    if (p[i1][j1]!=0 || p[i2][j2]!=0)
        return 0;
    return 1;
}
int tds::Copy(int a[13],int b[13])
{
    for (int i=0;i<13;i++)
        a[i] = b[i];
    return 0;
}
int tds::WhotoPlay()
{
    int i,j;
    int cc[3]={0,0,0};//Count the curent number of each color
    for(i = 0; i < SCALE; i++)
            for(j = 0; j < SCALE; j++)
                cc[p[i][j]]++;
    if (cc[1]<cc[2])
        return 1;
    else
        return 2;
}
double tds::GetFeatures(int MyColor)
{
    //Return Eval,Change Features
    int Enemy = 3 - MyColor;//定义对手棋子颜色
    int i,j,k;
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
    for(i=1; i<=6; i++)
    {
        fai[i] = NumberOfMyRoad[i];
        fai[i+6] = NumberOfEnemyRoad[i];
    }
    double score=0,Norm = 0;
    for(i=0;i<13;i++)
        Norm += fai[i]*fai[i];
    for(i=1;i<13;i++)
    {
        if (MyColor==1)
            score += fai[i]*theta_white[i]/Norm;
        else
            score += fai[i]*theta[i]/Norm;
    }
    double ans = 1.0/(1.0+exp(-score));
    //cout << ans << endl;
    return ans;
}
int tds::Terminal()
{
    if (fai[6]>0 || fai[12]>0)
        return 1;
    else
        return 0;
}
tds::node tds::makenode(int i1,int j1,int i2,int j2,double score)
{
    node pt;
    pt.x1 = i1;
    pt.y1 = j1;
    pt.x2 = i2;
    pt.y2 = j2;
    pt.score = score;
    return pt;
}
int tds::Greedy(double eps)
{
    srand(unsigned(time(0)));
    int i,j,MyColor;
    int cc[3]={0,0,0};//Count the curent number of each color
    for(i = 0; i < SCALE; i++)
        for(j = 0; j < SCALE; j++)
            cc[p[i][j]]++;

    if (cc[1]==0 && cc[2]==0)
    {
        int x = 7+int(5*rand()/(RAND_MAX+1.0));
        int y = 7+int(5*rand()/(RAND_MAX+1.0));
        p[x][y] = 2;
        return 2;
    }
    vector<node> Queue;
    MyColor = WhotoPlay();
    Queue.clear();
    bool Flag = false;
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
        Flag = true;
    }
    else
    {
        if (cc[1]+cc[2]<300 && rand()/(RAND_MAX+1.0)<0.1)
        {
            int i1=-1,j1=-1,i2=-1,j2=-1;
            while(!Legal(i1,j1,i2,j2))
            {
                i1 = int(rand()/(RAND_MAX+1.0)*19);
                j1 = int(rand()/(RAND_MAX+1.0)*19);
                i2 = int(rand()/(RAND_MAX+1.0)*19);
                j2 = int(rand()/(RAND_MAX+1.0)*19);
            }
            p[i1][j1] = MyColor;
            p[i2][j2] = MyColor;
            return 0;
        }
        int first,second,i1,i2,j1,j2;
        for(first = 0; first < SCALE*SCALE; first++)
            for(second = first+1;second < SCALE*SCALE;second++)
            {
                i1 = first/19;
                j1 = first%19;
                i2 = second/19;
                j2 = second%19;
                if (p[i1][j1] == 0 && p[i2][j2] == 0)
                {
                    Flag = true;
                    p[i1][j1] = MyColor;
                    p[i2][j2] = MyColor;
                    Queue.push_back(makenode(i1,j1,i2,j2,GetFeatures(MyColor)));
                    p[i1][j1] = 0;
                    p[i2][j2] = 0;
                }
            }
    }
    if (Flag == false)
        return -1;

    for(int it=0;it<Queue.size();it++)
        if (Queue[it].score>Queue[0].score)
            Queue[0] = Queue[it];
    //cout << Queue[0].score << endl;
    i = Queue[0].x1;
    j = Queue[0].y1;
    p[i][j] = MyColor;

    i = Queue[0].x2;
    j = Queue[0].y2;
    p[i][j] = MyColor;
    return MyColor;
}
int tds::SelfPlay()
{
    int t = 0;
    int F[300][13];
    double V[300];
    Copy(F[0],fai);
    while (Terminal()!=1)
    {
        double eps = 0.1,alfa = 0.1;
        int state = Greedy(eps);
        if (state==-1)
            break;
        t += 1;
        V[t] = GetFeatures(state);
        if (state==1)
            cout << t << ":" <<V[t] << endl;
        Copy(F[t],fai);
        if (t>2 && state==1)
        {
            double delta = V[t] - V[t-2];
            double Norm = 0;
            for(int i=0;i<13;i++)
                Norm += F[t-2][i]*F[t-2][i];
            for(int i=0;i<13;i++)
            {
                theta[i] += alfa*delta*F[t-2][i]/Norm;
                //cout << theta[i] << " ";
            }
            //cout << endl;
        }
        if (t%10==0)
        for(int x=0;x<SCALE;x++)
        {
            for(int y=0;y<SCALE;y++)
                cout << p[x][y] << " ";
            cout << endl;
        }
    }
}
int tds::TD_Learning(int n)
{
    int i=0;
//    while(i<n)
    {
        Initialize();
        SelfPlay();

        for(int j=0;j<13;j++)
            cout << theta[j] << ",";
        cout << endl;
        for(int x=0;x<SCALE;x++)
        {
            for(int y=0;y<SCALE;y++)
                cout << p[x][y] << " ";
            cout << endl;

        }


//        i++;
    }
}
void tds::run()
{
    TD_Learning(3);
}

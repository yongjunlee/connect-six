#ifndef TDS_H
#define TDS_H
#include <QThread>
#define SCALE 19

class tds:public QThread
{

public:
    tds();
    struct node
    {
        int x1,y1,x2,y2;
        double score;
    };
    int p[19][19];
    int fai[13];
    double theta[13];
    double theta_white[13];
    int Initialize();
    int Legal(int i1,int j1,int i2,int j2);
    int Copy(int a[13],int b[13]);
    int WhotoPlay();
    double GetFeatures(int MyColor);
    int Terminal();
    node makenode(int i1,int j1,int i2,int j2,double score);
    int Greedy(double eps);
    int SelfPlay();
    int TD_Learning(int n);

    void run();

};

#endif // TD0_H

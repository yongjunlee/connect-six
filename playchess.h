#ifndef PLAYCHESS_H
#define PLAYCHESS_H
#define SCALE 19
#include <vector>
#include "checkifwin.h"
class playChess
{
public:
    playChess();
    struct node
    {
        int x1,y1,x2,y2,score;
    };
    //判断输赢--yongjunlee add
    checkIfWin *ckif;
    //end

    int EvaluateChessStatus(int p[19][19],int MyColor);
    int whoWin(int p[SCALE][SCALE]);
    bool cmp(node a,node b);
    int WhoisNext(int p[19][19]);
    int Legal(int i1,int j1,int i2,int j2);
    node makenode(int i1,int j1,int i2,int j2,int score);
    int putChesses(int p[19][19],int *count_white,int *count_black);//此是主函数，输入矩阵，算法直接在其上添加2子，下白还是黑程序会自己判断。
};

#endif // PLAYCHESS_H

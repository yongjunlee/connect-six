#include "checkifwin.h"
#define SCALE 19
checkIfWin::checkIfWin()
{

}
int checkIfWin::checkWinner(int p[SCALE][SCALE],int x,int y,int lol[2])
{
    int knumber = 0;
    while(x>=0&&x<SCALE&&y>=0&&y<=SCALE)
    {
        if(p[x][y]==p[(x+lol[0])][y+lol[1]])
        {

            return knumber=1+ checkWinner(p,x+lol[0],y+lol[1],lol);
        }
        else return knumber;

    }

}

bool checkIfWin::ifWin(int p[SCALE][SCALE],int x,int y)
{
    int lol[8][2] = {{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}};
    int count[8];
    count[0]=0;
    for(int i=0; i<=7; i++)
    {
        count[i] = checkWinner(p,x,y,lol[i]);
    }
    if((count[0]+count[4]>=5)||(count[1]+count[5]>=5)||(count[2]+count[6]>=5)||(count[3]+count[7]>=5))
        return true;
    else
        return false;
}


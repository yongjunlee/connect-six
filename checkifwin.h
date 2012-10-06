#ifndef CHECKIFWIN_H
#define CHECKIFWIN_H
#define SCALE 19
class checkIfWin
{

public:
    checkIfWin();
    static int checkWinner(int p[SCALE][SCALE],int x,int y,int lol[2]);
    static bool ifWin(int p[SCALE][SCALE],int x,int y);

};

#endif // CHECKIFWIN_H

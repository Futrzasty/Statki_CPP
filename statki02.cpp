#include <stdlib.h>
#include <iostream>
#include <conio.h>

#include "statki02.h"

using namespace std;

#define WINDOWS 1

void clrscr() {
#ifdef WINDOWS
  system("cls");
#endif
#ifdef LINUX
  system("clear");
#endif
}

static int pole[10][10] = {0}, max_x = 10, max_y = 10, okrety[5][5][5], CH,
    ilosc_mozliwosci[5] = {0, 1, 2, 2, 2},
    maszty[11] = {0, 4, 3, 3, 2, 2, 2, 1, 1, 1, 1},
    X, Y, oldX, oldY, typ, oldtyp, is_ok,
    statek[11] = {0};
int *pole_human;

static int check_otocz(int x, int y)
{
  //if (pole[x][y] != 0) return 0;
  for (int i=-1; i<=1; i++)
    {
    if (x+i<0 || x+i>=max_x) continue;
    for (int j=-1; j<=1; j++)
      {
	if (y+j<0 || y+j>=max_y) continue;
	if (pole[x+i][y+j] != 0) return 0;
      }
    }
  return 1;
}

static void wypisz()
{
cout << "          Rozmiesc okrety\n";
cout << endl;
for (int i=0; i<=max_y-1 ;i++)
  {
  for (int j=0; j<=max_x-1 ;j++)
    {
    if (pole[j][i] < 0)
       cout << " *";
    else
       cout << " " << pole[j][i];
    }
  cout << "\n";
  }
  cout << "\n\nSterowanie:\nKlawisze kursorow - przesuwanie\nSpacja - obrot\nEnter - zawtierdza statek\n";
}

static void usunmin()
{
for (int i=0; i<=max_y-1 ;i++)
  {
  for (int j=0; j<=max_x-1 ;j++)
    {
    if (pole[j][i] < 0) pole[j][i] = 0;
    }
  cout << "\n";
  }
}

static void zatw()
{
for (int i=0; i<=max_y-1 ;i++)
  {
  for (int j=0; j<=max_x-1 ;j++)
    {
    pole[j][i] = abs(pole[j][i]);
    }
  cout << "\n";
  }
}

static void rozmiesc()
{
okrety[1][1][1] = 0;

okrety[2][1][1] = 0;
okrety[2][1][2] = 100;

okrety[2][2][1] = 0;
okrety[2][2][2] = 1;

okrety[3][1][1] = 0;
okrety[3][1][2] = 100;
okrety[3][1][3] = 200;

okrety[3][2][1] = 0;
okrety[3][2][2] = 1;
okrety[3][2][3] = 2;

okrety[4][1][1] = 0;
okrety[4][1][2] = 100;
okrety[4][1][3] = 200;
okrety[4][1][4] = 300;

okrety[4][2][1] = 0;
okrety[4][2][2] = 1;
okrety[4][2][3] = 2;
okrety[4][2][4] = 3;
  srand(0);
  
  for (int i=1; i<=10; i++)
  {
    CH = 0;
    typ = (rand() % ilosc_mozliwosci[maszty[i]]) + 1;
    do {
      //X = 0; Y = 0;
      X = rand()%max_x;
      Y = rand()%max_y;

      is_ok = 1;
      for (int j=1; j<=maszty[i]; j++)
      {
        statek[j] = okrety[maszty[i]][typ][j];
        if (pole[X][Y] != 0) is_ok = 0;
        if (X+statek[j]/100 >= max_x || X+statek[j]/100 < 0 || Y+statek[j]%100 >= max_y || Y+statek[j]%100 < 0) is_ok = 0;
        if (pole[X+statek[j]/100][Y+statek[j]%100] != 0) is_ok = 0;
        if (!check_otocz(X+statek[j]/100, Y+statek[j]%100)) is_ok = 0;
      }
    } while (!is_ok);


    while (CH != 13) {
      do {

        is_ok = 1;
        for (int j=1; j<=maszty[i]; j++)
          {
            statek[j] = okrety[maszty[i]][typ][j];
            if (pole[X][Y] != 0) is_ok = 0;
            if (X+statek[j]/100 >= max_x || X+statek[j]/100 < 0 || Y+statek[j]%100 >= max_y || Y+statek[j]%100 < 0) is_ok = 0;
            if (pole[X+statek[j]/100][Y+statek[j]%100] != 0) is_ok = 0;
            if (!check_otocz(X+statek[j]/100, Y+statek[j]%100)) is_ok = 0;
            if (!is_ok && CH != ' ') {Y = oldY; X = oldX; is_ok = 1;}
            if (!is_ok && CH == ' ')
            {
              typ = oldtyp; is_ok = 0;
            }
          }
      } while (!is_ok);

      for (int j=1; j<=maszty[i]; j++)
        {
        pole[X+statek[j]/100][Y+statek[j]%100]= -i;
        }
      clrscr();
      wypisz();
      int OK = 0;
      do {
        CH = getch();
        if (CH == 0) CH=getch();

        if (CH == 72) {oldY = Y; oldX = X; Y--; usunmin(); OK = 1;}    //   UP
        if (CH == 80) {oldY = Y; oldX = X; Y++; usunmin(); OK = 1;}    //   DOWN
        if (CH == 75) {oldX = X; oldY = Y; X--; usunmin(); OK = 1;}    //   LEFT
        if (CH == 77) {oldX = X; oldY = Y; X++; usunmin(); OK = 1;}    //   RIGHT
        if (CH == 32) {oldtyp = typ; typ++; if (typ> ilosc_mozliwosci[maszty[i]]) typ = 1; usunmin(); OK = 1;}
        if (CH == 13) {zatw(); OK = 1;}
      } while (!OK);
    }
  }
}

void losuj_human()
{
  rozmiesc();
  //cout << "Wylosowalem_human";
  pole_human = &pole[0][0];
}

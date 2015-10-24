#include <stdlib.h>

#include "statki01.h"


static int pole[10][10] = {0}, max_x = 10, max_y = 10;
int *pole_AI;
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

static void rozmiesc()
{
int okrety[5][5][5];

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

int ilosc_mozliwosci[5] = {0, 1, 2, 2, 2};
int maszty[11] = {0, 4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
int X, Y, typ, is_ok;
int statek[11] = {0};

srand(0);
for (int i=1; i<=10; i++)
  {
  typ = (rand() % ilosc_mozliwosci[maszty[i]]) + 1;
  do {

    do {
      X = rand()%max_x;
      Y = rand()%max_y;
      } while (pole[X][Y] != 0);

    is_ok = 1;
    for (int j=1; j<=maszty[i]; j++)
      {
      statek[j] = okrety[maszty[i]][typ][j];
      if (X+statek[j]/100 >= max_x || X+statek[j]/100 < 0 || Y+statek[j]%100 >= max_y || Y+statek[j]%100 < 0) is_ok = 0;
      if (pole[X+statek[j]/100][Y+statek[j]%100] != 0) is_ok = 0;
      if (!check_otocz(X+statek[j]/100, Y+statek[j]%100)) is_ok = 0;

      }
     } while (!is_ok);

  for (int j=1; j<=maszty[i]; j++)
    {
    pole[X+statek[j]/100][Y+statek[j]%100]= i;
    }

  }

//for (int i = 0; i <= max_y - 1; i++)
//  {
//  for (int j=0; j <= max_x - 1; j++)
//    {
//    cout << " " << pole[j][i];
//    }
//  cout << "\n";
//  }

}

void losuj_AI()
{
  rozmiesc();
  //cout << "Wylosowalem_AI";
  pole_AI = &pole[0][0];
}

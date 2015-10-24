#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "windows.h"

#include "statki01.h"
#include "statki02.h"

#define MAX_X 10
#define MAX_Y 10
#define IL_OKRETOW 10

using namespace std;

#define WINDOWS 1

void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int typ_gracza[2] = {0, 1}, //0-Human, 1-AI
    akwen[2][MAX_X][MAX_Y]={0}, gracz,
    AIakwen[MAX_X][MAX_Y]={0}, AIclever, AIlinear, AIlist[10], AIlistPTR = 0,
    maszty_up[2][IL_OKRETOW]={ { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1},
                             { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1} },
    winner;
//    maszty_up[2][IL_OKRETOW]={ {1,1,1,1,2,2,2,3,3,4},
//                               {1,1,1,1,2,2,2,3,3,4} };
time_t t;

// Deklaracje
void AI_postsink();
int check_winner();
void show_stats(int g);

// Funkcje

void clean_neighbour(int g, int i, int j)
{
   for (int k=0; k < 8 ; k++)
     {
        switch (k) {
            case 0:
              if (akwen[g==1?0:1][i][j+1] == 0 && j+1 < MAX_Y) { akwen[g==1?0:1][i][j+1] = 100; }
              break;
            case 1:
              if (akwen[g==1?0:1][i][j-1] == 0 && j-1 >= 0) { akwen[g==1?0:1][i][j-1] = 100; }
              break;
            case 2:
              if (akwen[g==1?0:1][i+1][j] == 0 && i+1 < MAX_X) { akwen[g==1?0:1][i+1][j] = 100; }
              break;
            case 3:
              if (akwen[g==1?0:1][i-1][j] == 0 && i-1 >= 0) { akwen[g==1?0:1][i-1][j] = 100; }
              break;

            case 4:
              if (akwen[g==1?0:1][i+1][j+1] == 0 && i+1 < MAX_X && j+1 < MAX_Y) { akwen[g==1?0:1][i+1][j+1] = 100; }
              break;
            case 5:
              if (akwen[g==1?0:1][i+1][j-1] == 0 && i+1 < MAX_X && j-1 >= 0) { akwen[g==1?0:1][i+1][j-1] = 100; }
              break;
            case 6:
              if (akwen[g==1?0:1][i-1][j+1] == 0 && i-1 >= 0 && j+1 < MAX_Y) { akwen[g==1?0:1][i-1][j+1] = 100; }
              break;
            case 7:
              if (akwen[g==1?0:1][i-1][j-1] == 0 && i-1 >= 0 && j-1 >= 0 ) { akwen[g==1?0:1][i-1][j-1] = 100; }
              break;
        };
     }
}

int strzal(int g, int polexy)
{
  int x, y, pole;
  x = (polexy / 100) - 1;
  y = (polexy % 100) - 1;

  pole = akwen[g==1?0:1][x][y];

  //cout << "pole: " << pole;      //OUT

  if(pole == 0)
  {
    akwen[g==1?0:1][x][y]= 100;
    return 0;   //pudlo
  }

  if(pole > 0 && pole < 100)
  {
    akwen[g==1?0:1][x][y] += 100;
    maszty_up[g==1?0:1][pole-1]--;
    if (maszty_up[g==1?0:1][pole-1] > 0)
      return 1; //trafiony
    else
    {
      for(int w=0;w<MAX_Y;w++)
      {
	for(int e=0;e<MAX_X;e++)
        {
	  if(akwen[g==1?0:1][e][w] == pole+100)
          {
            akwen[g==1?0:1][e][w]+=100;
            clean_neighbour(g, e, w);
          }
	}
      }
      return 2; //zatopiony
    }
  }
return 1000;
}

int check_valid (int x, int y)
{
   //cout << "\nSprawdziles pole: " << x << "," << y << endl;
   int pole;
   x--;y--;
   if (x<0 || y<0 || x>MAX_X-1 || y>MAX_Y-1 || AIakwen[x][y]!=0 )
     return 0;
   else return 1;
}

int AI_shot()
{
  int x, y, proba1, proba, cele[100], celeptr = 0, los;
  do {
  if (AIclever)
  {
    for (int i=0; i < AIlistPTR; i++)
    {
       x = AIlist[i] / 100;
       y = AIlist[i] % 100;
       for (int j=0; j < 4 ; j++)
       {
          switch (j) {
              case 0:
                if (check_valid(x, y+1)) { cele[celeptr] = x*100 + (y+1); celeptr++; }
                break;
              case 1:
                if (check_valid(x, y-1)) { cele[celeptr] = x*100 + (y-1); celeptr++; }
                break;
              case 2:
                if (check_valid(x+1, y)) { cele[celeptr] = (x+1)*100 + y; celeptr++; }
                break;
              case 3:
                if (check_valid(x-1, y)) { cele[celeptr] = (x-1)*100 + y; celeptr++; }
                break;
          };
       }
    }

    los = rand() % celeptr;
    x = cele[los] / 100;
    y = cele[los] % 100;

    proba = check_valid(x, y);
    //cout << proba << "\nUdajac, ze mysle, ";
  }
  else
  {
    if (AIlinear)
    {
       int vert=0, hor=0, n_min=10000, n_max=0;
       if (AIlist[0] % 100 == AIlist[1] % 100) hor = 1; else vert = 1;

       if (vert)
       {
          x = AIlist[0] / 100;
          for (int i=0; i < AIlistPTR; i++)
          {
             if (AIlist[i] % 100 < n_min) n_min = AIlist[i] % 100;
          }
          for (int i=0; i < AIlistPTR; i++)
          {
             if (AIlist[i] % 100 > n_max) n_max = AIlist[i] % 100;
          }
          if (check_valid(x,n_min - 1))
          {
             y = n_min - 1;
          }
          else
          {
             if (check_valid(x,n_max + 1))
             {
               y = n_max + 1;
             }
          }
       //cout << n_min << " " << n_max << " " << x << " " << y << " " << "vert\n";
       }

       if (hor)
       {
          y = AIlist[0] % 100;
          for (int i=0; i < AIlistPTR; i++)
          {
             if (AIlist[i] / 100 < n_min) n_min = AIlist[i] / 100;
          }
          for (int i=0; i < AIlistPTR; i++)
          {
             if (AIlist[i] / 100 > n_max) n_max = AIlist[i] / 100;
          }
          if (check_valid(n_min - 1, y))
          {
             x = n_min - 1;
          }
          else
          {
             if (check_valid(n_max + 1,y))
             {
               x = n_max + 1;
             }
          }
       //cout << n_min << " " << n_max << " " << x << " " << y << " " << "hor\n";
       }
    }
    else
    {
      x = rand() % MAX_X + 1;
      y = rand() % MAX_Y + 1;
      proba = check_valid(x, y);
      //cout << proba << "\nNie wiem co robic, ";
    }
  }
  } while (!proba);
    cout << "\nStrzelam: (" << x << "," << y << ")\n";
  return (x*100 +y);
}

void AI_think(int polexy, int output)
{
  int x, y;
  x = polexy / 100;
  y = polexy % 100;

  AIakwen[x-1][y-1] = output+1;

  if (output == 1)
  {
    AIclever = 1;
    AIlist[AIlistPTR] = polexy;
    AIlistPTR++;
    if (AIlistPTR > 1) { AIclever = 0; AIlinear = 1; }
  }
  if (output == 2)
  {
     AIclever = 0;
     AIlinear = 0;
     AIlistPTR = 0;
     AI_postsink();
  }
}

void AI_postsink()
{
   //cout << "\nSprzatam!!!\n";

   for (int j = 0; j < MAX_Y; j++)
   {
       for (int i = 0; i < MAX_X; i++)
       {
          if (AIakwen[i][j] > 1)
          {
             for (int k=0; k < 8 ; k++)
             {
                switch (k) {
                    case 0:
                      if (AIakwen[i][j+1] == 0 && j+1 < MAX_Y) { AIakwen[i][j+1] = 1; }
                      break;
                    case 1:
                      if (AIakwen[i][j-1] == 0 && j-1 >= 0) { AIakwen[i][j-1] = 1; }
                      break;
                    case 2:
                      if (AIakwen[i+1][j] == 0 && i+1 < MAX_X) { AIakwen[i+1][j] = 1; }
                      break;
                    case 3:
                      if (AIakwen[i-1][j] == 0 && i-1 >= 0) { AIakwen[i-1][j] = 1; }
                      break;

                    case 4:
                      if (AIakwen[i+1][j+1] == 0 && i+1 < MAX_X && j+1 < MAX_Y) { AIakwen[i+1][j+1] = 1; }
                      break;
                    case 5:
                      if (AIakwen[i+1][j-1] == 0 && i+1 < MAX_X && j-1 >= 0) { AIakwen[i+1][j-1] = 1; }
                      break;
                    case 6:
                      if (AIakwen[i-1][j+1] == 0 && i-1 >= 0 && j+1 < MAX_Y) { AIakwen[i-1][j+1] = 1; }
                      break;
                    case 7:
                      if (AIakwen[i-1][j-1] == 0 && i-1 >= 0 && j-1 >= 0 ) { AIakwen[i-1][j-1] = 1; }
                      break;
                }; // switch
             }     // k
          }        // if
       }           // i --> x
   }               // j --> y
}

void pejntuj()
{
  clrscr();
  cout << "\n       Gracz nr: 1                   Gracz nr: 2" << endl;
  cout << "\n   a b c d e f g h i j           a b c d e f g h i j" << endl;
  for(int i=0;i<MAX_Y;i++)
	 {
	 if(i!=9) cout << " " << i+1; else cout << i+1;
	 for(int j=0;j<MAX_X;j++)
		{
		if(akwen[0][j][i]>=0 && akwen[0][j][i]<100) cout << " .";
		if(akwen[0][j][i]>=101 && akwen[0][j][i]<200) cout << " x";
		if(akwen[0][j][i]>=201 && akwen[0][j][i]<300) cout << " z";
		if(akwen[0][j][i]==100) cout << " o";
		}
	 cout << "        ";
	 if(i!=9) cout << " " << i+1; else cout << i+1;
	 for(int j=0;j<MAX_X;j++)
		{
		if(akwen[1][j][i]>=0 && akwen[1][j][i]<100) cout << " .";
		if(akwen[1][j][i]>=101 && akwen[1][j][i]<200) cout << " x";
		if(akwen[1][j][i]>=201 && akwen[1][j][i]<300) cout << " z";
		if(akwen[1][j][i]==100) cout << " o";
		}

	 //cout << "   ";
	 //for(int j=0;j<10;j++)
	 //	{
	 //	cout << akwen[1][j][i]<< " ";
	 //	}

	 cout << endl;
	 }
  cout << endl;
  show_stats(0);
  cout << endl;
  show_stats(1);
}

void show_stats(int g)
{
  int pustych=0, m3=0, m2=0, m1=0;
  for (int i=0; i<MAX_Y; i++)
  {
  for (int j=0; j<MAX_X; j++)
    {
       if (akwen[g][j][i] < 100) pustych++;
    }
  }
  cout << "U gracza " << g+1 << " pozostalo: " << pustych * 100 /(MAX_X*MAX_Y)<< "% powierzchni.\n";
  if (maszty_up[g][0] != 0) cout << "1 - ****    ";
  for (int i=1; i<=2; i++) if (maszty_up[g][i] != 0) m3++;
  for (int i=3; i<=5; i++) if (maszty_up[g][i] != 0) m2++;
  for (int i=6; i<=9; i++) if (maszty_up[g][i] != 0) m1++;
  if (m3 != 0) cout << m3 << " - ***    ";
  if (m2 != 0) cout << m2 << " - **    ";
  if (m1 != 0) cout << m1 << " - *";
  cout << endl;
}

void kolejka (int g)
{
  int x, y, output, AIfire;
  char cel[10], X, Y;
  do {
    if (typ_gracza[g] == 0)
    {
      //cout << "\nGraczu " << g+1 << " - ognia!!!\n";
      do {
        gotoxy(1,22);
        cout << "Podaj wspolrzedne celu (np. e5):                                  ";
        gotoxy(34,22);
        cin >> cel;
        X = cel[0];
        if (X >= 97 && X <=106)
          x = X - 96;
        else
          if (X >= 65 && X <=74)
            x = X - 64;
          else
            x = 0;
        if (cel[2] == 0)
          y = cel[1] - 48;
        else
          y = (cel[1]-48)*10 + (cel[2]-48);
      } while ((x <= 0 || x > MAX_X) || (y <= 0 || y > MAX_Y));
      output = strzal(g, 100*x+y);
      //cout << endl << output << endl;    //OUT
    }
    else
    {
      AIfire = AI_shot();
      output = strzal(g, AIfire);
      AI_think(AIfire, output);
    }
  pejntuj();
  if (check_winner()) break;
  } while (output);
}

void wprowadz()
  {
  FILE *plik;
  plik=fopen("ship1.txt","r");
  for(int i=0;i<10;i++)
	 {
	 for(int j=0;j<10;j++)
		{
		fscanf(plik,"%d",&akwen[0][j][i]);
		}
	 }
  fclose(plik);
  plik=fopen("ship2.txt","r");
  for(int i=0;i<10;i++)
	 {
	 for(int j=0;j<10;j++)
		{
		fscanf(plik,"%d",&akwen[1][j][i]);
		}
	 }
  fclose(plik);
  }

void wprowadz2()
{
  losuj_human();
  losuj_AI();
  for(int i=0;i<10;i++)
	 {
	 for(int j=0;j<10;j++)
		{
                    akwen[0][i][j] = *(pole_human++);
                    akwen[1][i][j] = *(pole_AI++);
		}
	 }

}

int check_winner()
{
   int suma[2];
   suma[0] = 0;
   suma[1] = 0;
   for (int i=0; i < IL_OKRETOW; i++)
   {
      suma[0] += maszty_up[0][i];
      suma[1] += maszty_up[1][i];
   }
   if (suma[0] == 0)  return 2;
   else if (suma[1] == 0) return 1;
   else return 0;
}

int main()
{
//clrscr();
srand((unsigned) time(&t));
wprowadz2();

clrscr();
pejntuj();
for(gracz = 0;; gracz++)
  {
  kolejka(gracz % 2);
  winner = check_winner();
  if (winner) break;
  }
  if (winner == 1)
     cout << "Wygrales, gratulacje!!! :-)";
  else
     cout << "Niestety, przegrales... :-(";
  getch();
}

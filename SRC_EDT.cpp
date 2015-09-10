#include<graphics.h>
#include<dos.h>
#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include<iostream.h>
#include<conio.h>
#include<fstream.h>
#include<iomanip.h>

// Cursor controls L/R/U/D

#define K_UP
#define K_DO
#define K_LE
#define K_RI

// Tool controls Wall / Cash / OneWay / Goal / Ghost

#define K_WA
#define K_CA
#define K_OW
#define K_GO
#define K_GH

//using namespace std;

// Function declarations

void init_menu();	// Editor menu ffs

void edit_loop();

void render_maze(); // Draw maze on screen

void render_blockxy(int,int,int,int);

void save(char*);		// Save new maze to file

void load(char*);		// Load a new maze from file

int get_key();          // Get last key pressed

void setp(char,int*,int*);   // Set player position

char MAZE[20][15]; 	// scaled 32x to 640*480 VGA resolution, men!

// Player globals

int px, py,lk,k,pts;

char cItem = 'W'; //item to be drawn is set to wall by default

char savname[256];

/*

d	 - Wall		(W) RED 1
d	 - One Way  (O) WHT 2
d	 - Money	(M) GRN 3
d	 - Eraser   (X) BLK 4
d	 - GHOST    (B) MGT 5
d	 - Goal	    (G) YLW 7
d	 - Player	(P) BLU 6

*/

void init_menu()
{
	cout << "\nPlease enter the file name to save the new maze to\n\n";
	//TODO RANDOMIZER
	//TODO PROPER HELP SYSTEM
	//TODO LANGUAGE CLEANUP
	cin >> savname;
	getchar();
}

int main()
{
	clrscr();
	cout << "\t\t WELCOME TO MAZETRIC'S LEVEL EDITOR\n    THIS PROGRAM ALLOWS YOU TO MAKE MAZES THAT CAN BE PLAYED USING MAZETRIC.\n\n";
    cout << "\nControls: \n\n";
    cout << "Use the arrow keys to move across the grid. On the horizontal number pad, press: \n\n";
    cout << setw(8)<< "1 for a wall"; cout<<"\t\t"<<setw(8) <<"2 for a one-way tile \n";
    cout << setw(8)<< "3 for money"; cout<<"\t\t"<<setw(8) <<"4 for blank spaces \n";
    cout << setw(8)<< "5 for ghosts"; cout<<"\t\t"<<setw(8) <<"6 for the player \n"; cout<<setw(8)<<"7 for the goal\n\n";
    cout << "\n\nPress 'S' to save the maze and Esc to exit\n\n";

	init_menu();

	int driv = DETECT;
	int gm = 0;
	initgraph(&driv,&gm,"C:\\TC\\BGI");

	memset((void*)MAZE,'X',20*15*sizeof(char));

    px = py = 0;

    edit_loop();

	getchar();
	closegraph();
    return 1;
}

void edit_loop()
{
	int draw = 1;
	while(1)
	{
		if(kbhit() && px<20 && py<15)
		{
		   lk = k;
		   k = get_key();

		   switch(k)
           {
			   case 2: cItem = 'W';break;
			   case 3: cItem = 'O';break;
			   case 4: cItem = 'M';break;
			   case 5: cItem = 'X';break;
			   case 6: cItem = 'B';break;
			   case 7: cItem = 'P';break;
			   case 8: cItem = 'G';break;
			   case 31: save(savname);break;
		   }

		if(k == 1){closegraph();exit(0);}

		else if(k == 80 ){ if(draw == 0){MAZE[px][py]=0;} MAZE[px][py+1] = cItem;py++;}
		else if(k == 72 ){ if(draw == 0){MAZE[px][py]=0;} MAZE[px][py-1] = cItem;py--;}
		else if(k == 75 ){ if(draw == 0){MAZE[px][py]=0;} MAZE[px-1][py] = cItem;px--;}
		else if(k == 77 ){ if(draw == 0){MAZE[px][py]=0;} MAZE[px+1][py] = cItem;px++;}

		render_maze();

		}
	}
}

int get_key()
{
   union REGS i,o;
   i.h.ah = 0;
   int86(22,&i,&o);
   return ( o.h.ah );
}

void render_maze()
{
	cleardevice();
	char coords[16];
	sprintf(coords,"X=%d;Y=%d;K=%d\n",px,py,k);
	outtextxy(0,0,coords);
	for(int i = 0;i<20;i++)
	{	for(int j = 0;j<15;j++)
		{	//TODO ADD OTHER ELEMS
			if(MAZE[i][j] == 'W') render_blockxy(i*32,j*32,RED,XHATCH_FILL);
			if(MAZE[i][j] == 'O') render_blockxy(i*32,j*32,WHITE,LINE_FILL);
			if(MAZE[i][j] == 'M') render_blockxy(i*32,j*32,GREEN,CLOSE_DOT_FILL);

			if(MAZE[i][j] == 'X') {setcolor(WHITE);rectangle(i*32,j*32,i*32 + 32,j*32 + 32);} // This is actually pretty cool!
			if(MAZE[i][j] == 'B') render_blockxy(i*32,j*32,MAGENTA,INTERLEAVE_FILL);

			if(MAZE[i][j] == 'P') render_blockxy(i*32,j*32,BLUE,SOLID_FILL);
			if(MAZE[i][j] == 'G') render_blockxy(i*32,j*32,YELLOW,SOLID_FILL);
		}
	}
	outtextxy(px*32 + 16,py*32 + 16,"C"); // Show position of cursor!
}

void render_blockxy(int x,int y,int c,int f)
{
	setcolor(c);
	setfillstyle(f,c);
	rectangle(x,y,x+32,y+32);
	floodfill(x+1,y+1,c);
}

void setp(char c,int *x,int *y) //Determines player position from loaded file.
{
	for(int i = 0;i<20;i++)
	{
		for(int j = 0;j<15;j++)
		{
			if(MAZE[i][j] == c)
			{
				*x = i;
				*y = j;
			}
		}
	}
}

void load(char *fname)  // Loads maze from disk.
{
	ifstream lvl;
	char pathf[256];
	sprintf(pathf,"%s",fname);
	lvl.open(pathf,ios::binary|ios::in);
	for(int x = 0;x<20;x++)
	{
		for(int y = 0;y<15;y++)
		{
			MAZE[x][y] = lvl.get();
		}
	}
	lvl.close();
}

void save(char *fname)	// Saves maze to disk.
{
	ofstream f1;
	char pathf[256];
	sprintf(pathf,"%s",fname);
	f1.open(pathf,ios::binary|ios::out);
	for(int x = 0;x<20;x++)
	{
		for(int y = 0;y<15;y++)
		{
			f1.put(MAZE[x][y]);
		}
	}
	f1.close();
	outtextxy(0,0,"Saved maze!");
}

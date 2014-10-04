#include<graphics.h>
#include<dos.h>
#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include<conio.h>
#include<fstream.h>
#include<process.h>
#include<string.h>

// Player globals

int px, py,lk,k,pts = 0;

// Function declarations

void init_menu();	// Play, Edit, Save, Load selection
void mvt(int k);
void game_loop();	// Play the game!
void main();

void render_maze(); // Draw maze on the screen
void render_blockxy(int,int,int,int); // Draw basic block.


void load(char*);		// Load a new maze from file




int get_key();          // Get last key pressed
void setp(int*,int*);   // Set player position

void death(char * msg)
{
	cleardevice();
	outtextxy(640/4,480/2,msg);
	outtextxy(640/4,480/2 + 16,"Press enter to play again.\n");
	cin.get();cin.get();cin.get();
	closegraph();
	main();
}

void win()
{
	cleardevice();
	outtextxy(640/4,480/2,"You cleared this level. Congratulations.");
	char sstr[64];
	sprintf(sstr,"Your final score was %d\nPress enter to play again\n",pts);
	outtextxy(640/4,480/2 + 16,sstr);
	cin.get();cin.get();cin.get();
	closegraph();
	main();
}

char MAZE[20][15]; 	// scaled 32x to 640*480 VGA

void main()
{
	int driv = DETECT;
	int gm = 0;
	initgraph(&driv,&gm,"C:\\TC\\BGI");

	// ^^ Initialize graphics system.

	memset((void*)MAZE,'X',20*15*sizeof(char));

	cout << "\t\t\t\t MAZETRIC \n\n";
	cout << "Enter the name of the file you wish to play! (Or enter 'exit' to finish) : ";
	char path[512];
	cin >> path;
	if(strcmp(path,"Exit")==0 || strcmp(path,"exit")==0 || strcmp(path,"EXIT")==0)
	{
		cout<<"\n\nThank you for playing! Press any key to exit."<<endl;
		getch();
		closegraph();
	}
	else
	{
		load(path);

		setp(&px,&py);

		game_loop();

		getchar();
		closegraph();
	}
}

void game_loop()
{
	while(1)
	{
		if(kbhit())	// Update player position in maze.
		{
		   lk = k;
		   k = get_key();
		   if(k == 1){closegraph();exit(0);}
		   mvt(k); // Input handling and collision detection and win/lose situee.
		   render_maze();

		}

		// ->  ai_ghost(); // Update ghost position in maze

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

	for(int i = 0;i<20;i++)
	{
	    for(int j = 0;j<15;j++)
		{
			if(MAZE[i][j] == 'W') render_blockxy(i*32,j*32,RED,XHATCH_FILL);

			if(MAZE[i][j] == 'O') render_blockxy(i*32,j*32,WHITE,LINE_FILL);
			if(MAZE[i][j] == 'H') {setcolor(WHITE);circle(i*32+16,j*32+16,10); }

			if(MAZE[i][j] == 'M') render_blockxy(i*32,j*32,GREEN,CLOSE_DOT_FILL);

			if(MAZE[i][j] == 'B') render_blockxy(i*32,j*32,MAGENTA,INTERLEAVE_FILL);
			if(MAZE[i][j] == 'P') render_blockxy(i*32,j*32,BLUE,SOLID_FILL);
			if(MAZE[i][j] == 'G') render_blockxy(i*32,j*32,YELLOW,SOLID_FILL);
		}
	}
}


int h_x = 900;int h_y=900;

void mvt(int k)   // --  Movement and Collisions handler w/ kill-win screens
{
	MAZE[h_x][h_y] = 'H';

	int mx = 0; int my = 0;

	if(k == 80 &&  MAZE[px][py+1] != 'W'  )	 {	my=1;}//MAZE[px][py]=0; MAZE[px][py+1] = 'P';	}

	else if(k == 72 && MAZE[px][py-1] != 'W'){	my=-1;}//MAZE[px][py]=0; MAZE[px][py-1] = 'P';	}

	else if(k == 75 && MAZE[px-1][py] != 'W'){	mx=-1;}//MAZE[px][py]=0; MAZE[px-1][py] = 'P';	}

	else if(k == 77 && MAZE[px+1][py] != 'W'){	mx=1;}//MAZE[px][py]=0; MAZE[px+1][py] = 'P';	}

	MAZE[px][py] = 'X';


	if( MAZE[px+mx][py+my] == 'M') pts++;
	if( MAZE[px+mx][py+my] == 'O') {	h_x = px+mx;h_y=py+my;		}
	if( MAZE[px+mx][py+my] == 'B') death("You died after a ghost ate you.");  //
	if( MAZE[px+mx][py+my] == 'H') death("You died after falling into a hole."); //
	if( MAZE[px+mx][py+my] == 'G') win();

	px = px + mx;
	py = py + my;

	MAZE[px][py] = 'P';

}

void render_blockxy(int x,int y,int c,int f)
{
	setcolor(c);
	setfillstyle(f,c);
	rectangle(x,y,x+32,y+32);
	floodfill(x+1,y+1,c);
}

void setp(int *x,int *y)
{
	for(int i = 0;i<20;i++)
	{
		for(int j = 0;j<15;j++)
		{
			if(MAZE[i][j] == 'P')
			{
				*x = i;
				*y = j;
			}
		}
	}
}

void load(char *fname)
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


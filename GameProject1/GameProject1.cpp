// Ken Johnson - Dodgers! Game Project
// Last Date Modified: 11 May 2014

#include "stdafx.h"
#include <GL/glut.h>
#include <GL/GL.H>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <string.h>
#include <sstream>

const int WIDTH = 720;
const int HEIGHT = 480;




static bool menu = true;
static bool over = false;
static bool side = false;
static char diff1[] = "Easy";
static char diff2[] = "Medium";
static char diff3[] = "Hard";
static char ex[] = "Exit";
static char gOver[] = "Game Over :(";
static char gTime[10];
static char time1[] = "You survived for ";
static char time2[] = " seconds!";
static char title[9] = "Dodgers!";
static char title2[] = "by Ken Johnson";
static char title3[] = "Vehicle:";
static char title4[] = "Difficulty: ";
static char start[] = "Play!";
static float clockTime = 0;
static float clock2 = 0;
static float rClock;
static float colorR = .5;
static float colorG = 0;
static float colorB = 0;
static float t_deltaX = 0;
static float t_deltaY = 0;
static int difficulty = 1; // 1 - Easy, 2 - Medium, 3 - Hard
static int spin = 0;
static int steps = 0;
static int scale = 1;
static int vehicle = 1; // 1 - Red, 2 - Green, 3 - Purple

//AI car struct
//Holds variables and functions for updating a computer controlled Car
struct AI_car
{
bool appear;
bool first;
float deltaY;
float holdX;
float holdx_two;
float bR_CornerX;
float bR_CornerY;
float diffY;
float front[2];
float back[2];
float left[2];
float right[2];
int ai_scale;
int steps_AI;

void draw_AI(float x, float y);
void draw_train();
void draw(float x, float y, float dY);

};

AI_car artInt[7];

void drawCar(float r, float g, float b);
void car(float r, float g, float b); 
void car_AI(float x, float y);
void display();
void init();
void handleKey(unsigned char key, int x, int y);
void handleMouse(int button, int state, int x, int y);
void gameOver();
void translate();
void launch(int value);
void checkHits();
void mainMenu();
void drawMenu();
void overMenu();

//Creates the user's car
//Void return, three float inputs to control color of car
void car(float r, float g, float b)
{
	//Body
	glBegin(GL_POLYGON);
		glColor3f(r, g, b);
			glVertex2f(0, 2);
			glVertex2f(0, -12);
			glVertex2f(6, -12);
			glVertex2f(6, 2);
	glEnd();

	//Hood
	glBegin(GL_POLYGON);
		glColor3f(r+.20,g+.20,b+.20);
			glVertex2f(.5, -8);
			glVertex2f(.5, -3.5);
			glVertex2f(5.5, -3.5);
			glVertex2f(5.5, -8);
	glEnd();

	
	//Tire Color (controlled by handleKey)
	switch(steps)
	{
	case 0: glColor3f(0,0,0); break;

	case 1: glColor3f(.153, .153, .153); break;

	case 2: glColor3f(.255,.255,.255); break;

	default: steps = 0; glColor3f(.333,.333,.333); break;
	}

	//Tires
	glBegin(GL_POLYGON);
			glVertex2f(7.5, 1);
			glVertex2f(7.5, -3);
			glVertex2f(6, -3);
			glVertex2f(6, 1);
	glEnd();
	glBegin(GL_POLYGON);
			glVertex2f(7.5, -11);
			glVertex2f(7.5, -7);
			glVertex2f(6, -7);
			glVertex2f(6, -11);
	glEnd();
	glBegin(GL_POLYGON);
			glVertex2f(-1.5, 1);
			glVertex2f(-1.5, -3);
			glVertex2f(0, -3);
			glVertex2f(0, 1);
	glEnd();
	glBegin(GL_POLYGON);
			glVertex2f(-1.5, -11);
			glVertex2f(-1.5, -7);
			glVertex2f(0, -7);
			glVertex2f(0, -11);
	glEnd();


}


//Creates a GLMatrix to draw, translate, scale, and rotate the user's car
//Void return, three float inputs to control color of car
void drawCar(float r, float g, float b)
{
	glPushMatrix();
		glTranslatef(t_deltaX, t_deltaY, 0);
		glRotatef(spin, 0, 0, 1);
		glScalef(scale,scale,1);
		car(r,g,b);
	glPopMatrix();
}

//Makes the call to create an AI controlled car on the screen
//Void return, three floating points inputted
//One for speed, the others for position
void AI_car::draw(float x, float y, float dY)
{
	glPushMatrix();
		glTranslatef(0, dY, 0);
		glRotatef(0,0,0,1);
		glScalef(ai_scale,ai_scale,1);
		draw_AI(x, y);
		holdX = x;
		front[0] = x/2;
		front[1] = y;
		back[0] = x/2;
		back[1] = y+12;
		right[0] = x+6;
		right[1] = y+6;
		left[0] = x;
		left[1] = y+6;
	glPopMatrix();
}

//Same as Car(), but modified to draw for AI instead of player
//Void return, two floating point values for x and y positions
void AI_car::draw_AI(float x, float y)
{
		
		
		//Body
		glBegin(GL_POLYGON);
			glColor3f(.0, 0, .5);
				glVertex2f(x, y);
				glVertex2f(x, y+12);
				glVertex2f(x+6, y+12);
				glVertex2f(x+6, y);
		glEnd();
	
		//Hood
		glBegin(GL_POLYGON);
			glColor3f(.23, .53, .73);
				glVertex2f(x+.5, y+8);
				glVertex2f(x+.5, y+3.5);
				glVertex2f(x+5.5, y+3.5);
				glVertex2f(x+5.5, y+8);
		glEnd();
	
	
		//Tire Color (controlled by handleKey)
		switch(steps_AI)
		{
		case 0: steps_AI++; glColor3f(0,0,0); break;
	
		case 1: steps_AI++; glColor3f(.153, .153, .153); break;
	
		case 2: steps_AI++; glColor3f(.255,.255,.255); break;
	
		default: steps_AI = 0; glColor3f(.333,.333,.333); break;
		}
		
		//Tires
		glBegin(GL_POLYGON);
				glVertex2f(x+7.5, y-1);
				glVertex2f(x+7.5, y+3);
				glVertex2f(x+6, y+3);
				glVertex2f(x+6, y-1);
		glEnd();
		glBegin(GL_POLYGON);
				glVertex2f(x+7.5, y+11);
				glVertex2f(x+7.5, y+7);
				glVertex2f(x+6, y+7);
				glVertex2f(x+6, y+11);
		glEnd();
		glBegin(GL_POLYGON);
				glVertex2f(x-1.5, y-1);
				glVertex2f(x-1.5, y+3);
				glVertex2f(x, y+3);
				glVertex2f(x, y-1);
		glEnd();
		glBegin(GL_POLYGON);
				glVertex2f(x-1.5, y+11);
				glVertex2f(x-1.5, y+7);
				glVertex2f(x, y+7);
				glVertex2f(x, y+11);
		glEnd();
		

}

//Acts as AI for Computer controlled cars, runs on timer to make sure it continues to run
//Void return, one int value required for input to use as glTimerFunc
void launch(int value)
{
	int c;
	for (c = 0; c < 4; c++)		
	{
		switch(c)
		{	
		case 0:
		case 2:
			{
				int p = rand()%180;
				int q = rand()%180;
				bool flip = false;

				if (p > 90)
					p = (-p)+90;
				if (q > 90)
					q= (-q)+90;

				if (!artInt[c].appear)
					artInt[c].appear = true;

				if (artInt[c].first)
				{
					switch(difficulty)
					{
					case 1:
						{
							artInt[c].deltaY = rand()%3+1;
							break;
						}
					case 2:
						{
							artInt[c].deltaY = rand()%7+2;
							break;
						}
					case 3:
						{
							artInt[c].deltaY = rand()%10+2;
							break;
						}
					}
					
					if (!flip)
					{
						artInt[c].draw(p, 114, artInt[c].deltaY);
						flip = true;
					}
					else
					{
						artInt[c].draw(q, 114, artInt[c].deltaY);
						flip = false;
					}	
					artInt[c].first = false;
				}
				else
				{
					switch(difficulty)
					{
					case 1:
						{
							artInt[c].deltaY -= rand()%3+1;
							break;
						}
					case 2:
						{
							artInt[c].deltaY -= rand()%7+1;
							break;
						}
					case 3:
						{
							artInt[c].deltaY -= rand()%10+1;
							break;
						}
					}
					artInt[c].draw(artInt[c].holdX, 114, artInt[c].deltaY);
				}

				artInt[c].front[1] -= artInt[c].deltaY;
				
				
				if (artInt[c].deltaY <= -220)
				{
					artInt[c].appear = false;
					artInt[c].first = true;
				}
				break;
			}
		case 1:
		case 3:
			{
				int p = rand()%180;
				int q = rand()%180;
				bool flip = false;

				if (p > 90)
					p = (-p)+90;
				if (q > 90)
					q= (-q)+90;

				if (!artInt[c].appear)
					artInt[c].appear = true;

				if (artInt[c].first)
				{
					switch(difficulty)
					{
					case 1:
						{
							artInt[c].deltaY = rand()%3+1;
							break;
						}
					case 2:
						{
							artInt[c].deltaY = rand()%7+2;
							break;
						}
					case 3:
						{
							artInt[c].deltaY = rand()%10+2;
							break;
						}
					}
					
					if (!flip)
					{
						artInt[c].draw(p, -114, -artInt[c].deltaY);
						flip = true;
					}
					else
					{
						artInt[c].draw(q, -114, -artInt[c].deltaY);
						flip = false;
					}	
					artInt[c].first = false;
				}
				else
				{
					switch(difficulty)
					{
					case 1:
						{
							artInt[c].deltaY -= rand()%3+1;
							break;
						}
					case 2:
						{
							artInt[c].deltaY -= rand()%7+1;
							break;
						}
					case 3:
						{
							artInt[c].deltaY -= rand()%10+1;
							break;
						}
					}
					artInt[c].draw(artInt[c].holdX, -114, -artInt[c].deltaY);
				}
				
				if (artInt[c].deltaY <= -220)
				{
					artInt[c].appear = false;
					artInt[c].first = true;
					artInt[c].holdX = 0;
					artInt[c].deltaY = 0;
				}
				break;
			}

		}
	}
	
	checkHits();
}

//Does all collision checking based on the players deltaX and deltaY global values
//Void return, no input
void checkHits()
{
	int k;

	for (k = 0; k < 4; k++)
	{
		
			switch(k)
			{
			case 0:
			case 2:
				{
					if(artInt[k].holdX == t_deltaX || artInt[k].holdX <= t_deltaX +3 && artInt[k].holdX > t_deltaX-3)
					{
						if (artInt[k].deltaY+100 < t_deltaY)
						{	
							menu = true;
							over = true;
						}
					}

					break;
				}
			case 1:
			case 3:
				{
					
					if(artInt[k].holdX == t_deltaX || artInt[k].holdX <= t_deltaX +3 && artInt[k].holdX > t_deltaX-3)
					{
						if (artInt[k].deltaY+60 < t_deltaY)
						{
							menu = true;
							over = true;
						}
					}
					break;
				}
			}
		
	}
	
	}

//Initializes OpenGL to get objects ready for screen
//Void return, no input
void init()
{
	int r;

	glMatrixMode(GL_PROJECTION);	//Tells OGL how to use matrices
	glLoadIdentity();				//Loads the matrix identity
	gluOrtho2D(-100, 100, -100, 100);	//Sets the viewpoint for the scene using an orthographic 2D view
	srand(time(NULL));				//Creates a random number generator based on time (crappy, but it works)

	for (r = 0; r < 7; r++)
	{
		artInt[r].deltaY = 0;
		artInt[r].steps_AI = 0;
		artInt[r].appear = true;
		artInt[r].first = true;
		artInt[r].ai_scale = 1;
	}
}

//Makes calls to OpenGL to tell which screen to pull up (Either menu, game, or game_over).
//Void return, no input
void display()
{
	if (!menu && !over)
	{
		glClearColor(1,1,1,0);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BITS);
		launch(0);
		drawCar(colorR, colorG, colorB);
		glFlush();
		clockTime = (glutGet(GLUT_ELAPSED_TIME)/1000.0);
	}
	else if (menu && !over)
	{
		glClearColor(.304,0.204,0.104,0);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BITS);
		mainMenu();
		glFlush();
		clock2 = (glutGet(GLUT_ELAPSED_TIME)/1000.0);
	}
	else
	{
		glClearColor(.304,0.204,0.104,0);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BITS);
		overMenu();
		glFlush();
	}

	glutSwapBuffers();
	
}


//Just the main program call to start everything up, including OpenGL
//Void return, one int and one char (pointer) value inputted
void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(WIDTH/2, HEIGHT/2);

	glutCreateWindow("DODGERS! by Ken Johnson");
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(handleKey);
	glutMouseFunc(handleMouse);
	glutTimerFunc(2500, launch, 0);
	init();
	glutMainLoop();
}

//Controls the user's keyboard, and reacts accordingly
//Void return, 3 int values inputted
void handleKey(unsigned char key, int x, int y)
{

	switch(key)
	{
	case 27:
		{
			exit(0);
			break;
		}
	case 119: //w
		{ 
				t_deltaY += 3; 
				spin = 0;
				steps++;
			break;
		}
	case 115: //s
		{
				t_deltaY -=3;
				spin = 180; 
				steps++;
			break;
		}
	case 97: //a
		{
				t_deltaX -=3;
				spin = 90; 
				steps++;
			break;
		}
	case 100: //d
		{ 
				t_deltaX += 3; 
				spin = 270; 
				steps++;
			break;
		}
	
	}

}

//Kills the program
//Nothing in or out
void gameOver()
{
	exit(0);
}

//Makes the calls to draw the Main Menu on the screen
//Nothing in or out
void mainMenu()
{
	glColor3f(1, .275, .275); 
	glPushMatrix();
		glTranslatef(-50, 50, 0);
		glScalef(.2,.2,0);
		for(char *p = title; *p; p++)
		{
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
		}
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-50, 25, 0);
		glScalef(.1, .1, 0);
		for(char *p = title2; *p; p++)
		{
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
		}
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-50, 0, 0);
		glScalef(.1, .1, 0);
		for(char *p = title3; *p; p++)
		{
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
		}
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-50, -25, 0);
		glScalef(.1, .1, 0);
		for(char *p = title4; *p; p++)
		{
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
		}
	glPopMatrix();

	switch(vehicle)
	{
	case 1:
		{
			colorR = .5;
			colorG = 0;
			colorB = 0;
			break;
		}
	case 2:
		{
			colorR = 0;
			colorG = .5;
			colorB = 0;
			break;
		}
	case 3:
		{
			colorR = .376;
			colorG = .169;
			colorB = .376;
			break;
		}
	}

	t_deltaX = 30;
	t_deltaY = 10;


	drawCar(colorR, colorG, colorB);

	switch(difficulty)
	{
	case 2:
		{
			glColor3f(1, .275, .275);
			glPushMatrix();
				glTranslatef(25, -25, 0);
				glScalef(.1, .1, 0);
				for(char *p = diff2; *p; p++)
				{
					glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
				}
			glPopMatrix();
			break;
		}
	case 3:
		{
			glColor3f(1, .275, .275);
			glPushMatrix();
				glTranslatef(25, -25, 0);
				glScalef(.1, .1, 0);
				for(char *p = diff3; *p; p++)
				{
					glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
				}
			glPopMatrix();
			break;
		}
	default:
		{
			glColor3f(1, .275, .275);
			glPushMatrix();
				glTranslatef(25, -25, 0);
				glScalef(.1, .1, 0);
				for(char *p = diff1; *p; p++)
				{
					glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
				}
			glPopMatrix();
			break;
		}
	}

	glColor3f(1, .275, .275);
	glPushMatrix();
		glTranslatef(-10, -75, 0);
		glScalef(.1, .1, 0);
		for(char *p = start; *p; p++)
		{
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
		}
	glPopMatrix();


}

//Handles and reacts to the user's mouse
//Void return, four ints inputted
void handleMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		if (x >= 175 && x < 380)
		{
			if (y <= 310 && y > 260)
			{
				difficulty++;
				if(difficulty > 3)
					difficulty = 1;

				mainMenu();
			}
		}

		if (x >= 169 && x < 344)
		{
			if (y <= 250 && y > 204)
			{
				vehicle++;
				if(vehicle > 3)
					vehicle = 1;

				mainMenu();
			}
		}

		if (x >= 320 && x < 423 && menu)
		{
			if (y <= 430 && y > 385)
			{
				t_deltaX = 0;
				t_deltaY = 0;
				menu = false;
			}
		}

		if (x >=307 && x <384 && over)
		{
			if (y >= 390 && y < 425)
				gameOver();
		}
	
	}
}

//Creates and displays the game over menu
//Nothing in or out
void overMenu()
{

	

	glColor3f(1, .275, .275); 
	glPushMatrix();
		glTranslatef(-85, 75, 0);
		glScalef(.2,.2,0);
		for(char *p = gOver; *p; p++)
		{
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
		}
	glPopMatrix();

	glColor3f(1, .275, .275); 
	glPushMatrix();
		glTranslatef(-60, 25, 0);
		glScalef(.1,.1,0);
		for(char *p = time1; *p; p++)
		{
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
		}
	glPopMatrix();

	rClock = clockTime - clock2;
	_snprintf_s(gTime, 10, "%.3f", rClock);

	glColor3f(1, .275, .275); 
	glPushMatrix();
		glTranslatef(-65, 0, 0);
		glScalef(.1,.1,0);
		for(char *p = gTime; *p; p++)
		{
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
		}
	glPopMatrix();

	glColor3f(1, .275, .275); 
	glPushMatrix();
		glTranslatef(-20, 0, 0);
		glScalef(.1,.1,0);
		for(char *p = time2; *p; p++)
		{
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
		}
	glPopMatrix();

	glColor3f(1, .275, .275); 
	glPushMatrix();
		glTranslatef(-15, -75, 0);
		glScalef(.1,.1,0);
		for(char *p = ex; *p; p++)
		{
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
		}
	glPopMatrix();
}


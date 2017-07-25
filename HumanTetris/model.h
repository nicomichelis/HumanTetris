#pragma once

#include "vertex.h"
#include <ctime>
#define nP 33


class MyModel {
public:
	/* Stage
	 * 0: startscreen
	 * 1: controls
	 * 2: game
	 * 3: quit
	 */
	int stage;
	// OpenGL
	HDC			hDC;
	HGLRC		hRC;
	HWND		hWnd;
	HINSTANCE	hInstance;
	bool		keys[256];
	bool		active;
	bool		cursor;
	bool		captured;
	int			cx, cy;
	int			Wheight, Wwidth;
	int			frames;
	double		frameTime;
	double		fps;
	clock_t		Tstamp, Tstart;
	double		Full_elapsed;
	bool		StartScreen;
	bool		Perso;
	GLuint		texture[50];
	GLuint		base;
	GLdouble	fovy;
	GLfloat		RotX_a;
	GLfloat		RotY_a;


private:
	//butt
	double buttonWidth;
	double buttonHeight;
	double cursorWidth;
	double cursorHeight;
	int nbuttons;
	int buttCount;
	boolean selectButt[2];

	// Floor
	double floorLargh;
	double floorProf;
	double floorAltezza;

	// Wall
	double wallLargh;
	double wallProf;
	double wallAltezza;
	double wallPosition;
	Vertex wallPositionPoint;

	// Player
	Vertex PlayerPosition;
	double PlayerRotation;
	double PlayerHeadSize;
	double PlayerThickness;
	double PlayerBodyHeight;
	Vertex a, b, c, d;
	Vertex CheckPoints[33];
	
	boolean checkIn=true;

	Vertex Body[4];
	Vertex HoleBody[20];

	// Difficulty
	double size;
	double rotation;
	double diff;
	int score;

	// LIMITI
	double limitesuperiore;
	double limiteinferiore;
	double limitedestro;
	double limitesinistro;
	double randomX;
	double randomY;
	double randomR;


	// Hole
	Vertex holePosition;
	double holeRotation;
	Vertex ha, hb, hc, hd;
	Vertex HolePoints[nP];

	// TEST
	double Rotx = 0.0;
	double Roty = 0.0;


public:
	
	MyModel();
	void DrawFloor();
	void DrawWall();
	void DrawPlayerOnWall(Vertex position, double rotation, double size);
	void DrawPlayer();
	void DrawStartscreen();
	void DrawCommands();
	void DrawGame();
	void SetWallPosition(double x) {
		this->wallPosition = x;
	}
	double GetWallPosition() {
		return wallPosition;
	}
	Vertex GetPlayerPosition() {
		return PlayerPosition;
	}
	void SetPlayerPosition(Vertex x) {
		this->PlayerPosition = x;
	}
	double GetPlayerRotation() {
		return this->PlayerRotation;
	}
	double GetwallLargh() { 
		return wallLargh; 
	}
	double GetwallAltezza() { 
		return wallAltezza; 
	}
	void SetPlayerRotation(double x) {
		this->PlayerRotation = x;
	}
	double GetPlayerBodyHeight() { 
		return PlayerBodyHeight; 
	}
	double GetPlayerHeadSize() { 
		return PlayerHeadSize; 
	}
	void Randomize();

	//butt
	int getNbuttons() { 
		return this->nbuttons; 
	}
	int getCounterButtons() { 
		return this->buttCount; 
	}
	void SetCounterButtons(int n) {
		this->buttCount = n;
	}
	void setScene(int n) {
		for (int i = 0; i < nbuttons; i++) {
			selectButt[i] = 0;
		}
		selectButt[n] = 1;
	}
	
	//reset level
	void SetLevel(void);

	// GL
	bool InitGL(void);
	bool LoadGLTextures(void);
	void ReSizeGLScene(int width, int height);
	bool DrawGLScene(void);
	void SetProjection();
	void BuildFont(void);
	void KillFont(void);
	void glPrint(const char *fmt, ...);
	~MyModel() {
		this->KillFont();
	}

	//collision
	void Collision();
	void isInside(Vertex x);
	void Lose(int a);
	boolean lost();
	
	boolean CheckPoint();
	boolean included(Vertex HoleBody[], Vertex v, int j);
};

extern class MyModel Data;



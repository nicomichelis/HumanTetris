#pragma once

#include "vertex.h"
#include <ctime>


class MyModel {
public:
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
	GLuint		texture[10];
	GLdouble	fovy;
	GLfloat		RotX_a;
	GLfloat		RotY_a;

private:
	//butt
	float buttonWidth;
	float buttonHeight;
	float cursorWidth;
	float cursorHeight;
	int nbuttons;
	int buttCount;

	// Floor
	float floorLargh;
	float floorProf;
	float floorAltezza;

	// Wall
	float wallLargh;
	float wallProf;
	float wallAltezza;
	float wallPosition;
	Vertex wallPositionPoint;

	// Player
	Vertex PlayerPosition;
	float PlayerRotation;
	float PlayerHeadSize;
	float PlayerThickness;
	float PlayerBodyHeight;

	// Difficulty
	float size;
	float rotation;
	float diff;

	// LIMITI
	float limitesuperiore;
	float limiteinferiore;
	float limitedestro;
	float limitesinistro;
	float randomX;
	float randomY;
	float randomR;

	// TEST
	float Rotx = 0.0;
	float Roty = 0.0;
public:
	MyModel();
	void DrawFloor();
	void DrawWall();
	void DrawPlayerOnWall(Vertex position, float rotation, float size);
	void DrawPlayer();
	void SetWallPosition(float x);
	float GetWallPosition();
	Vertex GetPlayerPosition();
	void SetPlayerPosition(Vertex x);
	float GetPlayerRotation();
	float GetwallLargh() { return wallLargh; }
	float GetwallAltezza() { return wallAltezza; }
	void SetPlayerRotation(float x);
	float GetPlayerBodyHeight() { return PlayerBodyHeight; }
	float GetPlayerHeadSize() { return PlayerHeadSize; }
	void Randomize();
	int getNbuttons() { return nbuttons; }
	int getCounterButtons() { return buttCount; }
	void SetCounterButtons(int n);

	// GL
	bool InitGL(void);
	bool LoadGLTextures(void);
	void ReSizeGLScene(int width, int height);
	bool DrawGLScene(void);
	void SetProjection();

};

extern class MyModel Data;
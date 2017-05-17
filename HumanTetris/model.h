#pragma once

#include "vertex.h"
#include <ctime>


class MyModel {
private:
	// Floor
	float floorLargh;
	float floorProf;
	float floorAltezza;
	Vertex fa, fb, fc, fd, fe, ff, fg, fh;
	Rect floorFront, floorBack, floorSideA, floorSideB, floorTop, floorBottom;

	// Wall
	float wallLargh;
	float wallProf;
	float wallAltezza;
	float wallPosition;
	Vertex wallPositionPoint;
	Vertex wa, wb, wc, wd, we, wf, wg, wh;
	Rect wallFront, wallBack, wallSideA, wallSideB, wallTop, wallBottom;

	// Player
	Vertex PlayerPosition;
	float PlayerRotation;
	float PlayerHeadSize;
	float PlayerThickness;
	float PlayerBodyHeight;

	// Difficulty
	float size;
	float rotation;

	Vertex ba, bb, bc, bd, be, bf, bg, bh;


	// LIMITI
	float limitesuperiore;
	float limiteinferiore;
	float limitedestro;
	float limitesinistro;
	float randomX;
	float randomY;
	


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

};
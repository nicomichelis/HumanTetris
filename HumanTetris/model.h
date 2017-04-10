#pragma once

#include "vertex.h"

class MyModel {
private:
	// Floor
	float floorLargh;
	float floorProf;
	float floorAltezza;
	Vertex fa, fb, fc, fd, fe, ff, fg, fh;
	
	// Wall
	float wallLargh;
	float wallProf;
	float wallAltezza;
	float wallPosition;
	Vertex wallPositionPoint;
	Vertex wa, wb, wc, wd, we, wf, wg, wh;

public:
	MyModel();
	void DrawFloor();
	void DrawWall();
	void SetWallPosition(float x);
	float GetWallPosition();
};
#include "model.h"
#include "vertex.h"
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h> 

#pragma comment( lib, "opengl32.lib" )			
#pragma comment( lib, "glu32.lib" )					
#pragma comment( lib, "winmm.lib" )					

MyModel::MyModel() {
	// Init Floor
	floorLargh = 5.0;
	floorProf = 20.0;
	floorAltezza = 1.0;
	

	// Wall
	wallLargh = 5.0;
	wallProf = 1.0;
	wallAltezza = 7.0;
	wallPosition = -10.0;

	// Player
	Vertex initPlayer(0.0, 5.0, 10.0);
	initPlayer.SetColor(1.0, 0.5, 0.5);
	PlayerPosition = initPlayer;
	PlayerRotation = 0.0;
	PlayerHeadSize = 0.1;
	
}

void MyModel::DrawFloor(){
	fa.SetP(-(floorLargh / 2), 0.0, (floorProf / 2));
	fa.SetColor(0.0, 0.0, 1.0);
	fb.SetP((floorLargh / 2), 0.0, (floorProf / 2));
	fb.SetColor(0.0, 0.0, 1.0);
	fc.SetP((floorLargh / 2), 0.0, -(floorProf / 2));
	fc.SetColor(0.0, 0.0, 1.0);
	fd.SetP(-(floorLargh / 2), 0.0, -(floorProf / 2));
	fd.SetColor(0.0, 0.0, 1.0);
	fe.SetP(-(floorLargh / 2), -floorAltezza, (floorProf / 2));
	fe.SetColor(0.0, 0.0, 1.0);
	ff.SetP((floorLargh / 2), -floorAltezza, (floorProf / 2));
	ff.SetColor(0.0, 0.0, 1.0);
	fg.SetP((floorLargh / 2), -floorAltezza, -(floorProf / 2));
	fg.SetColor(0.0, 0.0, 1.0);
	fh.SetP(-(floorLargh / 2), -floorAltezza, -(floorProf / 2));
	fh.SetColor(0.0, 0.0, 1.0);
	floorTop.SetP(fa, fb, fc, fd);
	floorBottom.SetP(fh, fg, ff, fe);
	floorFront.SetP(fe, ff, fb, fa);
	floorBack.SetP(fg, fh, fd, fc);
	floorSideA.SetP(fh, fe, fa, fd);
	floorSideB.SetP(ff, fg, fc, fb);
	floorFront.Draw();
	floorBack.Draw();
	floorTop.Draw();
	floorBottom.Draw();
	floorSideA.Draw();
	floorSideB.Draw();
}

void MyModel::DrawWall(){
	
	wallPositionPoint.SetP(0.0, 0.0, wallPosition);
	wallPositionPoint.SetColor(1.0, 0, 0);

	wa.SetP(-(wallLargh / 2), 0.0, wallPosition + wallProf);
	wa.SetColor(0.0, 1.0, 0.0);
	wb.SetP(+(wallLargh / 2), 0.0, wallPosition + wallProf);
	wb.SetColor(0.0, 1.0, 0.0);
	wc.SetP(+(wallLargh / 2), wallAltezza, wallPosition + wallProf);
	wc.SetColor(0.0, 1.0, 0.0);
	wd.SetP(-(wallLargh / 2), wallAltezza, wallPosition + wallProf);
	wd.SetColor(0.0, 1.0, 0.0);
	we.SetP(-(wallLargh / 2), 0.0, wallPosition);
	we.SetColor(0.0, 1.0, 0.0);
	wf.SetP(+(wallLargh / 2), 0.0, wallPosition);
	wf.SetColor(0.0, 1.0, 0.0);
	wg.SetP(+(wallLargh / 2), wallAltezza, wallPosition);
	wg.SetColor(0.0, 1.0, 0.0);
	wh.SetP(-(wallLargh / 2), wallAltezza, wallPosition);
	wh.SetColor(0.0, 1.0, 0.0);
	wallFront.SetP(wa, wb, wc, wd);
	wallBack.SetP(wf, we, wh, wg);
	wallTop.SetP(wd, wc, wg, wh);
	wallSideA.SetP(wb, wf, wg, wc);
	wallSideB.SetP(we, wa, wd, wh);
	wallBottom.SetP(we, wf, wb, wa);
	wallFront.Draw();
	wallBack.Draw();
	wallTop.Draw();
	wallSideA.Draw();
	wallSideB.Draw();
	wallBottom.Draw();
}

void MyModel::DrawPlayer(){
	// Head
	Cylinder Head(PlayerPosition, PlayerHeadSize, 0.1); //0.1 spessore
	Head.Draw();
}

void MyModel::SetWallPosition(float x){
	this->wallPosition = x;
}

float MyModel::GetWallPosition(){
	return wallPosition;
}

Vertex MyModel::GetPlayerPosition(){
	return PlayerPosition;
}

void MyModel::SetPlayerPosition(Vertex x) {
	this->PlayerPosition = x;
}

float MyModel::GetPlayerRotation(){
	return this->PlayerRotation;
}

void MyModel::SetPlayerRotation(float x) {
	this->PlayerRotation = x;
}

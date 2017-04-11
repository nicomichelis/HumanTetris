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
	PlayerRotation = 3.14/4;
	PlayerHeadSize = 0.2;
	PlayerThickness = 0.1;
	PlayerBodyHeight = 1.0;
}

void MyModel::DrawFloor() {
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

void MyModel::DrawWall() {

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

void MyModel::DrawPlayer() {
	// Tutto sbagliato perchè non tiene conto dell'angolo di rotazione!
	// Head
	/*
	Vertex HeadPosition = PlayerPosition;
	HeadPosition.y += 0.7;
	Cylinder Head(HeadPosition, PlayerHeadSize, PlayerThickness);
	Head.Draw();
	*/
	PlayerPosition.Draw();
	// Body
	ba = bb = bc = bd = be = bf = bg = bh = PlayerPosition;
	ba.x = (PlayerPosition.x)*cosf(PlayerRotation) - (PlayerThickness / 2);
	ba.y = (PlayerPosition.y)*sinf(PlayerRotation) - (PlayerBodyHeight / 2);
	ba.z = PlayerPosition.z - PlayerThickness / 2;

	bb.x = (PlayerPosition.x)*cosf(PlayerRotation) + (PlayerThickness / 2);
	bb.y = (PlayerPosition.y)*sinf(PlayerRotation) - (PlayerBodyHeight / 2);
	bb.z = PlayerPosition.z - PlayerThickness / 2;
	
	bc.x = (PlayerPosition.x)*cosf(PlayerRotation) + (PlayerThickness / 2);
	bc.y = (PlayerPosition.y)*sinf(PlayerRotation) + (PlayerBodyHeight / 2);
	bc.z = PlayerPosition.z - PlayerThickness / 2;

	bd.x = (PlayerPosition.x)*cosf(PlayerRotation) - (PlayerThickness / 2);
	bd.y = (PlayerPosition.y)*sinf(PlayerRotation) + (PlayerBodyHeight / 2);
	bd.z = PlayerPosition.z - PlayerThickness / 2;
	
	Rect BodyFront(ba, bb, bc, bd);
	BodyFront.Draw();

	/*
	ba = bb = bc = bd = be = bf = bg = bh = PlayerPosition;
	bb.x = bg.x = bf.x = bc.x = (PlayerPosition.x + PlayerThickness / 2); // larghezza corpo
	ba.x = be.x = bd.x = bh.x = (PlayerPosition.x - PlayerThickness / 2);
	bd.y = bc.y = bg.y = bh.y = (PlayerPosition.y + PlayerBodyHeight / 2);	// Altezza corpo
	ba.y = bb.y = bf.y = be.y = (PlayerPosition.y - PlayerBodyHeight / 2);
	ba.z = bb.z = bc.z = bd.z = PlayerPosition.z - PlayerThickness / 2; //Spessore corpo
	be.z = bf.z = bg.z = bh.z = PlayerPosition.z + PlayerThickness / 2;

	ba.x = PlayerPosition.x +ba.x*cosf(PlayerRotation) + ba.y*sinf(PlayerRotation);
	ba.y = PlayerPosition.y -ba.x*sinf(PlayerRotation) + ba.y*sinf(PlayerRotation);
	ba.Draw();
	*/
	/*
	Rect FrontBody(ba, bb, bc, bd);
	FrontBody.Draw();
	Rect BackBody(bf, be, bh, bg);
	BackBody.Draw();
	Rect SideA(bb, bf, bg, bc);
	SideA.Draw();
	Rect SideB(be, ba, bd, bh);
	SideB.Draw();
	Rect Top(bd, bc, bg, bh);
	Top.Draw();
	Rect Bottom(be, bf, bb, ba);
	Bottom.Draw();
	
	// Arms (45 degrees)
	for (int i = 0; i < 4; i++) {
		Vertex armPos = PlayerPosition;
		float theta = 2.0f * 3.1415926f * float(i) / float(4) + 3.1415926f / float(4); // 45°*i
		float distArmBody = PlayerBodyHeight; // Distanza dal corpo al braccio (da cambiare)
		float x = distArmBody * cosf(theta) * sin(3.1415926f / float(4));	//calculate the x component 
		float y = distArmBody * sinf(theta) * cos(3.1415926f / float(4));	//calculate the y component
		armPos.SetP(armPos.x+x, armPos.y+y, armPos.z);
		armPos.Draw();

		float radius = PlayerBodyHeight;
		float theta = 2.0f * 3.1415926f * float(i) / float(4) + 3.1415926f / float(4);	// 45° 
		float x = radius * cosf(theta) * sin(3.1415926f / float(4)) + PlayerPosition.x;	//calculate the x component 
		float y = radius * sinf(theta) * cos(3.1415926f / float(4)) + PlayerPosition.y;	//calculate the y component
		ba = bb = bc = bd = be = bf = bg = bh = PlayerPosition;
		bb.x = bg.x = bf.x = bc.x = x + PlayerThickness / 2; // larghezza corpo
		ba.x = be.x = bd.x = bh.x = x - PlayerThickness / 2;
		bd.y = bc.y = bg.y = bh.y = y + PlayerBodyHeight / 2;	// Altezza corpo
		ba.y = bb.y = bf.y = be.y = y - PlayerBodyHeight / 2;
		ba.z = bb.z = bc.z = bd.z = PlayerPosition.z - PlayerThickness / 2; //Spessore corpo
		be.z = bf.z = bg.z = bh.z = PlayerPosition.z + PlayerThickness / 2;
	}
*/
}

void MyModel::SetWallPosition(float x) {
	this->wallPosition = x;
}

float MyModel::GetWallPosition() {
	return wallPosition;
}

Vertex MyModel::GetPlayerPosition() {
	return PlayerPosition;
}

void MyModel::SetPlayerPosition(Vertex x) {
	this->PlayerPosition = x;
}

float MyModel::GetPlayerRotation() {
	return this->PlayerRotation;
}

void MyModel::SetPlayerRotation(float x) {
	this->PlayerRotation = x;
}

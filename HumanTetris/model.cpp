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
	
	Vertex HeadPosition = PlayerPosition;
	HeadPosition.y += (PlayerBodyHeight / 2 + PlayerHeadSize)*sin(3.14 / 2 + PlayerRotation);
	HeadPosition.x += (PlayerBodyHeight / 2 + PlayerHeadSize)*cos(3.14 / 2 + PlayerRotation);
	HeadPosition.Draw();
	Cylinder Head(HeadPosition, PlayerHeadSize, PlayerThickness);
	Head.Draw();
	
	 PlayerPosition.Draw();
	// Body
	float diag = sqrt(pow((PlayerBodyHeight / 2),2.0) + pow((PlayerThickness / 2),2.0));
	ba = bb = bc = bd = be = bf = bg = bh = PlayerPosition;
	float angle = asin((PlayerBodyHeight / 2) / diag) ;

	ba.x = PlayerPosition.x + ((diag)*cos(3.14 + angle + PlayerRotation));
	ba.y = PlayerPosition.y + ((diag)*sin(3.14 + angle + PlayerRotation));
	ba.z = PlayerPosition.z - PlayerThickness / 2;


	bb.x = PlayerPosition.x + ((diag)*cos(6.28 - angle + PlayerRotation));
	bb.y = PlayerPosition.y + ((diag)*sin(-angle + 6.28 + PlayerRotation));
	bb.z = PlayerPosition.z - PlayerThickness / 2;

	
	bc.x = PlayerPosition.x + ((diag)*cos(angle + PlayerRotation));
	bc.y = PlayerPosition.y + ((diag)*sin(angle + PlayerRotation));
	bc.z = PlayerPosition.z - PlayerThickness / 2;
	
	bd.x = PlayerPosition.x + (diag)*cos(3.14 - angle + PlayerRotation);
	bd.y = PlayerPosition.y + (diag)*sin(3.14 - angle + PlayerRotation);
	bd.z = PlayerPosition.z - PlayerThickness / 2;
	
	Rect corpo_front(ba, bb, bc, bd);
	corpo_front.Draw();
	
	be = ba;
	be.z = PlayerPosition.z + PlayerThickness / 2;
	bf = bb;
	bf.z = PlayerPosition.z + PlayerThickness / 2;
	bg = bc;
	bg.z = PlayerPosition.z + PlayerThickness / 2;
	bh = bd;
	bh.z = PlayerPosition.z + PlayerThickness / 2;
	
	
	Rect corpo_back(bf, be, bh, bg);
	corpo_back.Draw();

	Rect corpo_right(bb, bf, bg, bc);
	corpo_right.Draw();

	Rect corpo_left(be, ba, bd, bh);
	corpo_left.Draw();

	Rect corpo_top(bd, bc, bg, bh);
	corpo_top.Draw();

	Rect corpo_bottom(be, bf, bb, ba);
	corpo_bottom.Draw();
	
	
	// Arms (45 degrees)
	float PlayerArmHeight = PlayerBodyHeight / 5 * 3;
	float ArmDist = 2 * diag ;
	float angle2;
	
	float ArmDiag = diag;
	
	ArmDist = sqrt(pow((PlayerArmHeight / 2), 2.0)+ pow((ArmDiag), 2.0)-2*ArmDiag*PlayerArmHeight*cos(3.14/6*5));
	float alpha = 3.14 / 4;
	float ArmThickness = PlayerThickness / 2;
	angle2 = asin( sin(90) / ArmDiag * ArmThickness / 2);
	float angle3;
	angle3 =acos( cos(angle2)*ArmDiag / PlayerArmHeight);
	ArmDist = sqrt(pow((PlayerArmHeight / 2), 2.0) + pow((ArmDiag), 2.0) - 2 * ArmDiag*PlayerArmHeight*cos(3.14 -angle2 - angle3));
	float beta = 3.14 / 6;
	ArmDiag =  sqrt(pow((PlayerArmHeight / 2), 2.0) + pow((ArmThickness / 2), 2.0));
	for (int i = 0; i < 4; i++) {
		//ArmDist = ArmDiag*4/3;
		alpha += i*3.14/2;
		Vertex ArmPos, xa, xb, xc, xd, xe, xf, xg, xh;
		xa.SetColor(1.0, 0.5, 0.3);
		xb.SetColor(1.0, 0.5, 0.3);
		xc.SetColor(1.0, 0.5, 0.3);
		xd.SetColor(1.0, 0.5, 0.3);
		xe.SetColor(1.0, 0.5, 0.3);
		xf.SetColor(1.0, 0.5, 0.3);
		xg.SetColor(1.0, 0.5, 0.3);
		xh.SetColor(1.0, 0.5, 0.3);
		xa.x = PlayerPosition.x + ArmDist*cos(alpha + PlayerRotation) + ((ArmDiag)*cos(3.14 + angle + PlayerRotation - alpha));
		xa.y = PlayerPosition.y + ArmDist*sin(alpha + PlayerRotation) + ((ArmDiag)*sin(3.14 + angle + PlayerRotation - alpha));
		xa.z = PlayerPosition.z + ArmThickness / 2;
		
		xb.x = PlayerPosition.x + ArmDist*cos(alpha + PlayerRotation) + ((ArmDiag)*cos(6.28 - angle + PlayerRotation - alpha));
		xb.y = PlayerPosition.y + ArmDist*sin(alpha + PlayerRotation) + ((ArmDiag)*sin(-angle + 6.28 + PlayerRotation - alpha));
		xb.z = PlayerPosition.z + ArmThickness / 2;
		
		xc.x = PlayerPosition.x + ArmDist*cos(alpha + PlayerRotation) + ((ArmDiag)*cos(angle + PlayerRotation - alpha));
		xc.y = PlayerPosition.y + ArmDist*sin(alpha + PlayerRotation) + ((ArmDiag)*sin(angle + PlayerRotation - alpha));
		xc.z = PlayerPosition.z + ArmThickness / 2;
		
		xd.x = PlayerPosition.x + ArmDist*cos(alpha + PlayerRotation) + (ArmDiag)*cos(3.14 - angle + PlayerRotation - alpha);
		xd.y = PlayerPosition.y + ArmDist*sin(alpha + PlayerRotation) + (ArmDiag)*sin(3.14 - angle + PlayerRotation - alpha);
		xd.z = PlayerPosition.z + ArmThickness / 2;
		

		Rect rightArm_front(xa, xb, xc, xd);
		rightArm_front.Draw();
		xe = xa;
		xf = xb;
		xg = xc;
		xh = xd;
		xe.z = xf.z = xg.z = xh.z = PlayerPosition.z - ArmThickness / 2;
		Rect rightArm_back(xe, xf, xh, xg);
		rightArm_back.Draw();

		Rect rightArm_a(xb, xf, xg, xc);
		rightArm_a.Draw();

		Rect rightArm_b(xe, xa, xd, xh);
		rightArm_b.Draw();
	}
	/*
	float ArmDist = 2 * diag;
	float ArmDiag = diag;
	float alpha = 3.14 / 4;
	float beta = 3.14 / 6; 

	Vertex ArmPos;
	ArmPos.SetColor(1.0, 0.5, 0.3);
	ArmPos.x = PlayerPosition.x + ArmDist*cos(alpha + PlayerRotation) + ArmDiag*cos(beta + PlayerRotation);
	ArmPos.y = PlayerPosition.y + ArmDist*sin(alpha + PlayerRotation) + ArmDiag*sin(beta + PlayerRotation);
	ArmPos.z = PlayerPosition.z + PlayerThickness / 2;
	ArmPos.Draw();
	ArmPos.x = PlayerPosition.x + ArmDist*cos(alpha + PlayerRotation) + ArmDiag*cos(3.14+beta + PlayerRotation);
	ArmPos.y = PlayerPosition.y + ArmDist*sin(alpha + PlayerRotation) + ArmDiag*sin(3.14+beta + PlayerRotation);
	ArmPos.Draw();
	ArmPos.x = PlayerPosition.x + ArmDist*cos(alpha + PlayerRotation) + ArmDiag*cos(3.14 - beta + PlayerRotation);
	ArmPos.y = PlayerPosition.y + ArmDist*sin(alpha + PlayerRotation) + ArmDiag*sin(3.14 - beta + PlayerRotation);
	ArmPos.Draw();
	ArmPos.x = PlayerPosition.x + ArmDist*cos(alpha + PlayerRotation) + ArmDiag*cos(- beta + PlayerRotation);
	ArmPos.y = PlayerPosition.y + ArmDist*sin(alpha + PlayerRotation) + ArmDiag*sin(- beta + PlayerRotation);
	ArmPos.Draw();*/


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

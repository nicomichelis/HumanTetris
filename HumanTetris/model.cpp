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
	/*
	Vertex HeadPosition = PlayerPosition;
	HeadPosition.y += (PlayerBodyHeight / 2 + PlayerHeadSize)*sin(3.14 / 2 + PlayerRotation);
	HeadPosition.x += (PlayerBodyHeight / 2 + PlayerHeadSize)*cos(3.14 / 2 + PlayerRotation);
	Cylinder Head(HeadPosition, PlayerHeadSize, PlayerThickness);
	Head.Draw();*/
	
	//PlayerPosition.Draw();
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
	
	Rect corpo(ba, bb, bc, bd);
	//corpo.Draw();
	
	be = ba;
	be.z = PlayerPosition.z + PlayerThickness / 2;
	bf = bb;
	bf.z = PlayerPosition.z + PlayerThickness / 2;
	bg = bc;
	bg.z = PlayerPosition.z + PlayerThickness / 2;
	bh = bd;
	bh.z = PlayerPosition.z + PlayerThickness / 2;
	
	/*
	Rect corpo_back(bf, be, bh, bg);
	corpo_back.Draw();

	Rect corpo_right(bb, bf, bg, bc);
	corpo_right.Draw();

	Rect corpo_left(be, ba, bd, bh);
	corpo_left.Draw();*/
	
	
	// Arms (45 degrees)
	float height = 2 * diag;
	float angle2;
	for (int j=0; j<4; j++) {
		angle2 = 3.14 * j / 2 + 3.14/4;
		
		Vertex ArmPos;
		ArmPos.SetColor(0.5, 0.5, 0.5);
		ArmPos.x = PlayerPosition.x + height*cos(angle2 + PlayerRotation);
		ArmPos.y = PlayerPosition.y + height*sin(angle2 + PlayerRotation);
		ArmPos.z = PlayerPosition.z;
		ArmPos.Draw();

		bc.x = PlayerPosition.x + height*cos(angle2 + PlayerRotation) + diag*cos(angle + PlayerRotation);
		bc.y = PlayerPosition.y + height*sin(angle2 + PlayerRotation) + diag*sin(angle + PlayerRotation);
		bc.SetColor(0.1, 0.1, 0.6);
		bc.Draw();

		bd.x = PlayerPosition.x + height*cos(angle2 + PlayerRotation) + diag*cos(angle + PlayerRotation);
		bd.y = PlayerPosition.y + height*sin(angle2 + PlayerRotation) - diag*sin(angle + PlayerRotation);
		bd.Draw();
		/*
		ba.x = ArmPos.x + height / 2 * cos(angle2);
		ba.y = ArmPos.y + height / 2 * sin(angle2);
		ba.SetColor(0.1, 0.1, 0.6);
		ba.Draw();

		bb.x = ArmPos.x + height / 2 * cos(angle2) + PlayerThickness*cos(angle2);
		bb.y = ArmPos.y + height / 2 * sin(angle2) - PlayerThickness*sin(angle2);
		bb.Draw();
		Rect leftArm(bb, ba, bc, bd);
		leftArm.Draw();
		*/
	}
	/*
		
		Vertex armPos = PlayerPosition;
		
		armPos.x = PlayerPosition.x - PlayerBodyHeight * 3 / 8 * cos(3.14 / 4) - PlayerThickness / 2;
		armPos.y = PlayerPosition.y + PlayerBodyHeight * 3 / 8 - PlayerBodyHeight * 3 / 8 * sin(3.14 / 4);
		armPos.Draw();


		float diag2 = sqrt(pow((PlayerBodyHeight *3/8), 2.0) + pow((PlayerThickness / 8*3), 2.0));
		
		float angle2 =  asin((PlayerBodyHeight *3/8) / diag2);
		
		bd.z = PlayerPosition.z - PlayerThickness / 4*3;
		ba.z = PlayerPosition.z - PlayerThickness / 4 * 3;
		bb.z = PlayerPosition.z - PlayerThickness / 4 * 3;
		bc.z = PlayerPosition.z - PlayerThickness / 4 * 3;

		bc = bc;
		bc.x = armPos.x + PlayerBodyHeight / 4 * 3 * cos(angle2);
		bc.y = armPos.y + PlayerThickness / 8*3 * sin(angle2);
		
		bc.SetColor(1.0, 1.0, 1.0);
		bd.SetColor(1.0, 1.0, 1.0);
		//bc.Draw();
		//bd.Draw();

		ba.x = bd.x - PlayerBodyHeight * 3 / 4 * cos(3.14 / 4);
		ba.y = bd.y - PlayerBodyHeight * 3 / 4 * cos(3.14 / 4);
		//ba.Draw();

		bb.x = bc.x - PlayerBodyHeight * 3 / 4 * cos(3.14 / 4);
		bb.y = bc.y - PlayerBodyHeight * 3 / 4 * cos(3.14 / 4);
		//bb.Draw();

		Rect leftArm(ba, bb, bc, bd);
		//leftArm.Draw();

		/*ba.x = armPos.x + ((diag2)*cos(3.14 + angle2 + PlayerRotation))*cos(3.14 / 4);
		ba.y = armPos.y + ((diag2)*sin(3.14 + angle2 + PlayerRotation))*sin(3.14 / 4);
		ba.z = armPos.z - PlayerThickness / 2;


		bb.x = armPos.x + ((diag2)*cos(6.28 - angle2 + PlayerRotation))*cos(3.14 / 4);
		bb.y = armPos.y + ((diag2)*sin(-angle2 + 6.28 + PlayerRotation))*sin(3.14 / 4);
		bb.z = armPos.z + PlayerThickness / 2;


		bc.x = armPos.x + ((diag2)*cos(angle2 + PlayerRotation))*cos(3.14 / 4);
		bc.y = armPos.y + ((diag2)*sin(angle2 + PlayerRotation))*sin(3.14 / 4);
		bc.z = armPos.z - PlayerThickness / 2;

		bd.x = armPos.x + (diag2)*cos(3.14 - angle2 + PlayerRotation)*cos(3.14 / 4);
		bd.y = armPos.y + (diag2)*sin(3.14 - angle2 + PlayerRotation)*sin(3.14 / 4);
		bd.z = armPos.z - PlayerThickness / 2;
		ba.SetColor(1.0, 1.0, 1.0);
		bb.SetColor(0.0, 1.0, 1.0);
		bc.SetColor(1.0, 0.0, 1.0);
		bd.SetColor(1.0, 1.0, 0.0);
		ba.Draw();
		bb.Draw();
		bc.Draw();
		bd.Draw();
		Rect leftArm(ba, bb, bc, bd);
		leftArm.Draw();*/
		/*
		be = ba;
		be.z = PlayerPosition.z + PlayerThickness / 2;
		bf = bb;
		bf.z = PlayerPosition.z + PlayerThickness / 2;
		bg = bc;
		bg.z = PlayerPosition.z + PlayerThickness / 2;
		bh = bd;
		bh.z = PlayerPosition.z + PlayerThickness / 2;

		be.Draw();
		bf.Draw();
		bg.Draw();
		bh.Draw();*/
		//Rect corpo_back(bf, be, bh, bg);
		//corpo_back.Draw();

		//Rect corpo_right(bb, bf, bg, bc);
		//corpo_right.Draw();

		//Rect corpo_left(be, ba, bd, bh);
		//corpo_left.Draw();



		/*
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
		be.z = bf.z = bg.z = bh.z = PlayerPosition.z + PlayerThickness / 2;*/
	

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

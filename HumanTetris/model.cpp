#include "model.h"
#include "vertex.h"
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h> 
#include "SOIL.h"
#include <stdio.h>
#include <string>
#include <math.h>

#pragma comment( lib, "opengl32.lib" )			
#pragma comment( lib, "glu32.lib" )					
#pragma comment( lib, "winmm.lib" )					

#pragma warning(disable:4996)

MyModel::MyModel(): hDC(NULL), hRC(NULL), hWnd(NULL), active(true), frames(0), fps(0), cursor(true), captured(false), StartScreen(true), Perso(false), fovy(45.0), RotX_a(0), RotY_a(0) {
	// stage init
	stage = 0;

	//butt
	buttonWidth = 4.5;
	buttonHeight = 1.5;
	cursorWidth = 1.2;
	cursorHeight = 0.7;
	nbuttons = 2;
	buttCount = 0;
	selectButt[0] = 0;
	selectButt[1] = 0;
	selectButt[2] = 0;

	// Init timing
	this->Tstart = this->Tstamp = clock();
	this->Full_elapsed = 0;
	this->frameTime = 0;

	// Init Floor
	floorLargh = 8.0;
	floorProf = 20.0;
	floorAltezza = 1.0;

	// Wall
	wallLargh = floorLargh;
	wallProf = 1.0;
	wallAltezza = 7.0;
	wallPosition = -10.0;

	// Player
	Vertex initPlayer(0.0, 1.0, 10.0);
	initPlayer.SetColor(1.0, 0.5, 0.5);
	PlayerPosition = initPlayer;
	PlayerRotation = 0.0;
	PlayerHeadSize = 0.2;
	PlayerThickness = 0.25;
	PlayerBodyHeight = 0.7;

	// Difficulty
	size = 4; // da 4 si scende a max 2
	diff = 0.01;
	score = 0;

	// Init limits
	srand((unsigned)time(NULL));
	limitesuperiore = wallAltezza - (size*PlayerBodyHeight / 2 + size*PlayerHeadSize * 2);
	limiteinferiore = (size*PlayerBodyHeight / 2 + size*PlayerHeadSize * 2);
	limitedestro = wallLargh / 2 - (size*PlayerBodyHeight / 2 + size*PlayerHeadSize * 2);
	limitesinistro = -wallLargh / 2 + (size*PlayerBodyHeight / 2 + size*PlayerHeadSize * 2);
	Randomize();
}

void MyModel::DrawFloor() {
	Vertex fa, fb, fc, fd, fe, ff, fg, fh;
	Rect floorFront, floorBack, floorSideA, floorSideB, floorTop, floorBottom;
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
	glBindTexture(GL_TEXTURE_2D, texture[8]);
	floorTop.DrawTextures();
	glBindTexture(GL_TEXTURE_2D, texture[21]);
	floorBack.DrawTextures();
	floorFront.DrawTextures();
	floorBottom.DrawTextures();
	floorSideA.DrawTextures();
	floorSideB.DrawTextures();

}

void MyModel::DrawWall() {
	Vertex wa, wb, wc, wd, we, wf, wg, wh;
	Rect wallFront, wallBack, wallSideA, wallSideB, wallTop, wallBottom;
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
	wallFront.DrawTextures();
	wallBack.DrawTextures();
	wallTop.DrawTextures();
	wallSideA.DrawTextures();
	wallSideB.DrawTextures();
	wallBottom.DrawTextures();

	// Limiti
	Vertex hole = wallPositionPoint;
	hole.y = randomY;
	hole.x = randomX;
	hole.z += wallProf / 2;
	DrawPlayerOnWall(hole, randomR, size);

}

void MyModel::DrawPlayerOnWall(Vertex position, double rotation, double size) {
	holePosition = position;
	holeRotation = rotation;
	double spessore = wallProf + 0.01;
	// Head
	position.r = 0.0;
	position.g = 0.0;
	position.b = 0.0;
	Vertex HeadPosition = position;
	HeadPosition.y += (PlayerBodyHeight / 2 + size*PlayerHeadSize)*sin(PI / 2 + rotation);
	HeadPosition.x += (PlayerBodyHeight / 2 + size*PlayerHeadSize)*cos(PI / 2 + rotation);
	Cylinder Head(HeadPosition, size*PlayerHeadSize/1.2, spessore);

	HolePoints[30].SetP(HeadPosition.x - size*PlayerHeadSize/0.6,HeadPosition.y,HeadPosition.z);
	HolePoints[31].SetP(HeadPosition.x + size*PlayerHeadSize / 0.6, HeadPosition.y, HeadPosition.z);
	HolePoints[32].SetP(HeadPosition.x, +size*PlayerHeadSize / 0.6, HeadPosition.z);
	
	
	Head.Draw();
	
	// Body
	Vertex ba, bb, bc, bd, be, bf, bg, bh;
	ba = bb = bc = bd = be = bf = bg = bh = position;

	ba.x = position.x - (size*PlayerThickness / 2)*cos(rotation) + (PlayerBodyHeight * (size / 2) / 2)* sin(rotation);
	bb.x = position.x + (size*PlayerThickness / 2)*cos(rotation) + (PlayerBodyHeight * (size / 2) / 2)* sin(rotation);
	bc.x = position.x + (size*PlayerThickness / 2)*cos(rotation) - (PlayerBodyHeight * (size / 2) / 2)* sin(rotation);
	bd.x = position.x - (size*PlayerThickness / 2)*cos(rotation) - (PlayerBodyHeight * (size / 2) / 2)* sin(rotation);

	ba.y = position.y - (PlayerBodyHeight * (size / 2) / 2)*cos(rotation) - (size*PlayerThickness / 2)* sin(rotation);
	bb.y = position.y - (PlayerBodyHeight * (size / 2) / 2)*cos(rotation) + (size*PlayerThickness / 2)* sin(rotation);
	bc.y = position.y + (PlayerBodyHeight * (size / 2) / 2)*cos(rotation) + (size*PlayerThickness / 2)* sin(rotation);
	bd.y = position.y + (PlayerBodyHeight * (size / 2) / 2)*cos(rotation) - (size*PlayerThickness / 2)* sin(rotation);

	ba.z = position.z + spessore / 2;
	bb.z = position.z + spessore / 2;
	bc.z = position.z + spessore / 2;
	bd.z = position.z + spessore / 2;

	Rect corpo_front(ba, bb, bc, bd);
	corpo_front.Draw();
	
	be = ba;
	bf = bb;
	bg = bc;
	bh = bd;

	be.z=bf.z=bg.z=bh.z = position.z - spessore / 2;

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
	glBindTexture(GL_TEXTURE_2D, texture[20]);
	corpo_front.DrawTextures();
	corpo_back.DrawTextures();
	corpo_right.DrawTextures();
	corpo_left.DrawTextures();
	corpo_top.DrawTextures();
	corpo_bottom.DrawTextures();

	// Arms
	double diag = sqrt(pow((PlayerBodyHeight / 2), 2.0) + pow((size*PlayerThickness / 2), 2.0));
	double angle = asin((PlayerBodyHeight / 2) / diag);
	double PlayerArmHeight = size*PlayerBodyHeight / 5 * 3;
	double ArmDist;
	double angle2;
	double angle3;
	double ArmThickness = size*PlayerThickness / 1.5;
	double ArmDiag;
	double alpha = PI / 3;

	ArmDiag = sqrt(pow((PlayerArmHeight / 2), 2.0) + pow((ArmThickness / 2), 2.0));
	angle2 = asin(sin(90) / diag * size*PlayerThickness / 2) - alpha;
	angle3 = acos(cos(angle2)*diag / PlayerArmHeight / 2);

	ArmDist = sqrt(pow((PlayerArmHeight / 2), 2.0) + pow(diag, 2.0) - 2 * diag*PlayerArmHeight / 2 * cos(PI - angle2 - angle3));
	double beta = PI / 6;
	double angle_arm = asin((PlayerArmHeight / 2) / ArmDiag);
	Vertex ArmPos, xa, xb, xc, xd, xe, xf, xg, xh;
	xa = xb = xc = xd = xe = xf = xg = xh = ba;
	for (int i = 0; i < 4; i++) {
		xa.x = position.x + (ArmDist - ArmThickness)*cos(alpha + rotation) + ((ArmDiag)*cos(PI + angle_arm + rotation - alpha));
		xa.y = position.y + (ArmDist - ArmThickness)*sin(alpha + rotation) + ((ArmDiag)*sin(PI + angle_arm + rotation - alpha));
		xa.z = position.z + spessore / 2;

		xb.x = position.x + (ArmDist - ArmThickness)*cos(alpha + rotation) + ((ArmDiag)*cos(2 * PI - angle_arm + rotation - alpha));
		xb.y = position.y + (ArmDist - ArmThickness)*sin(alpha + rotation) + ((ArmDiag)*sin(2 * PI - angle_arm + +rotation - alpha));
		xb.z = position.z + spessore / 2;

		xc.x = position.x + (ArmDist - ArmThickness)*cos(alpha + rotation) + ((ArmDiag)*cos(angle_arm + rotation - alpha));
		xc.y = position.y + (ArmDist - ArmThickness)*sin(alpha + rotation) + ((ArmDiag)*sin(angle_arm + rotation - alpha));
		xc.z = position.z + spessore / 2;

		xd.x = position.x + (ArmDist - ArmThickness)*cos(alpha + rotation) + (ArmDiag)*cos(PI - angle_arm + rotation - alpha);
		xd.y = position.y + (ArmDist - ArmThickness)*sin(alpha + rotation) + (ArmDiag)*sin(PI - angle_arm + rotation - alpha);
		xd.z = position.z + spessore / 2;

		Rect arm_front(xa, xb, xc, xd);
		arm_front.Draw();

		xe = xa;
		xf = xb;
		xg = xc;
		xh = xd;


		HolePoints[6 + (6 * i)] = xa;
		HolePoints[7 + (6 * i)] = xb;
		HolePoints[8 + (6 * i)] = xc;
		HolePoints[9 + (6 * i)] = xc;
		HolePoints[10 + (6 * i)] = xd;
		HolePoints[11 + (6 * i)] = xa;

		HoleBody[4 + (4 * i)] = xa;
		HoleBody[5 + (4 * i)] = xb;
		HoleBody[6 + (4 * i)] = xc;
		HoleBody[7 + (4 * i)] = xd;



		xe.z = xf.z = xg.z = xh.z = position.z - spessore / 2;

		Rect arm_back(xh, xg, xf, xe);
		arm_back.Draw();

		Rect arm_left(xb, xf, xg, xc);
		arm_left.Draw();

		Rect arm_right(xe, xa, xd, xh);
		arm_right.Draw();

		Rect arm_estremo_a(xg, xh, xd, xc);
		arm_estremo_a.Draw();

		Rect arm_estremo_b(xe, xf, xb, xa);
		arm_estremo_b.Draw();

		arm_front.DrawTextures();
		arm_back.DrawTextures();
		arm_right.DrawTextures();
		arm_left.DrawTextures();
		arm_estremo_a.DrawTextures();
		arm_estremo_b.DrawTextures();

		if (i == 0)
			alpha = PI / 6 * 4;
		if (i == 1)
			alpha = PI / 3 * 4;
		if (i == 2)
			alpha = PI / 6 * 10;
	}
	
}

void MyModel::DrawPlayer() {
	Vertex ba, bb, bc, bd, be, bf, bg, bh;
	// Head
	Vertex HeadPosition = PlayerPosition;
	HeadPosition.y += (PlayerBodyHeight / 2 + PlayerHeadSize)*sin(PI / 2 + PlayerRotation);
	HeadPosition.x += (PlayerBodyHeight / 2 + PlayerHeadSize)*cos(PI / 2 + PlayerRotation);
	Cylinder Head(HeadPosition, PlayerHeadSize, PlayerThickness);
		
	//Head.Draw();
	// PROVE TEXTURE TESTA
	glBindTexture(GL_TEXTURE_2D, texture[23]);
	Head.DrawTexture();
	glDisable(GL_TEXTURE);


	// Body
	double diag = sqrt(pow((PlayerBodyHeight / 2),2.0) + pow((PlayerThickness / 2),2.0));
	ba = bb = bc = bd = be = bf = bg = bh = PlayerPosition;
	double angle = asin((PlayerBodyHeight / 2) / diag) ;

	ba.x = PlayerPosition.x + ((diag)*cos(PI + angle + PlayerRotation));
	ba.y = PlayerPosition.y + ((diag)*sin(PI + angle + PlayerRotation));
	ba.z = PlayerPosition.z + PlayerThickness / 2;

	bb.x = PlayerPosition.x + ((diag)*cos(2*PI - angle + PlayerRotation));
	bb.y = PlayerPosition.y + ((diag)*sin(-angle + 2*PI + PlayerRotation));
	bb.z = PlayerPosition.z + PlayerThickness / 2;
	
	bc.x = PlayerPosition.x + ((diag)*cos(angle + PlayerRotation));
	bc.y = PlayerPosition.y + ((diag)*sin(angle + PlayerRotation));
	bc.z = PlayerPosition.z + PlayerThickness / 2;
	
	bd.x = PlayerPosition.x + (diag)*cos(PI - angle + PlayerRotation);
	bd.y = PlayerPosition.y + (diag)*sin(PI - angle + PlayerRotation);
	bd.z = PlayerPosition.z + PlayerThickness / 2;
	
	Rect corpo_front(ba, bb, bc, bd);
	corpo_front.Draw();
	
	be = ba;
	bf = bb;
	bg = bc;
	bh = bd;
	be.z = bf.z = bg.z = bh.z = PlayerPosition.z - PlayerThickness / 2;

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
	a = ba;
	b = bb;
	c = bc;
	d = bd;
	Body[0] = ba;
	Body[1] = bb;
	Body[2] = bc;
	Body[3] = bd;
		
	// Arms
	double PlayerArmHeight = PlayerBodyHeight / 4*3;
	double ArmDist;
	double angle2;
	double angle3;
	double ArmThickness = PlayerThickness / 2;
	double ArmDiag;
	double alpha = PI / 3;

	ArmDiag = sqrt(pow((PlayerArmHeight / 2), 2.0) + pow((ArmThickness / 2), 2.0));
	angle2 = asin( sin(90) / diag * PlayerThickness / 2) - alpha;
	angle3 =acos( cos(angle2)*diag / PlayerArmHeight/2);
	
	ArmDist = sqrt(pow((PlayerArmHeight / 2), 2.0) + pow(diag, 2.0) - 2 * diag*PlayerArmHeight/2*cos(PI -angle2 - angle3));
	double beta = PI / 6;
	double angle_arm = asin((PlayerArmHeight / 2) / ArmDiag);
	Vertex ArmPos, xa, xb, xc, xd, xe, xf, xg, xh;
	xa = xb = xc = xd = xe = xf = xg = xh = ba;
	for (int i = 0; i < 4; i++) {
		xa.x = PlayerPosition.x + (ArmDist - ArmThickness)*cos(alpha + PlayerRotation) + 
			((ArmDiag)*cos(PI+ angle_arm + PlayerRotation - alpha));
		xa.y = PlayerPosition.y + (ArmDist - ArmThickness)*sin(alpha + PlayerRotation) + 
			((ArmDiag)*sin(PI + angle_arm + PlayerRotation - alpha));
		xa.z = PlayerPosition.z + ArmThickness / 2;
		
		xb.x = PlayerPosition.x + (ArmDist - ArmThickness)*cos(alpha + PlayerRotation) + ((ArmDiag)*cos(2*PI - angle_arm + PlayerRotation - alpha));
		xb.y = PlayerPosition.y + (ArmDist - ArmThickness)*sin(alpha + PlayerRotation) + ((ArmDiag)*sin(2*PI - angle_arm + + PlayerRotation - alpha));
		xb.z = PlayerPosition.z + ArmThickness / 2;
		
		xc.x = PlayerPosition.x + (ArmDist - ArmThickness)*cos(alpha + PlayerRotation) + ((ArmDiag)*cos(angle_arm + PlayerRotation - alpha));
		xc.y = PlayerPosition.y + (ArmDist - ArmThickness)*sin(alpha + PlayerRotation) + ((ArmDiag)*sin(angle_arm + PlayerRotation - alpha));
		xc.z = PlayerPosition.z + ArmThickness / 2;
		
		xd.x = PlayerPosition.x + (ArmDist - ArmThickness)*cos(alpha + PlayerRotation) + (ArmDiag)*cos(PI - angle_arm + PlayerRotation - alpha);
		xd.y = PlayerPosition.y + (ArmDist - ArmThickness)*sin(alpha + PlayerRotation) + (ArmDiag)*sin(PI - angle_arm + PlayerRotation - alpha);
		xd.z = PlayerPosition.z + ArmThickness / 2;

		Rect arm_front(xa, xb, xc, xd);
		arm_front.Draw();
		
		xe = xa;
		xf = xb;
		xg = xc;
		xh = xd;

		xe.z = xf.z = xg.z = xh.z = PlayerPosition.z - ArmThickness / 2;

		Rect arm_back(xh, xg, xf, xe);
		arm_back.Draw();

		Rect arm_left(xb, xf, xg, xc);
		arm_left.Draw();

		Rect arm_right(xe, xa, xd, xh);
		arm_right.Draw();

		Rect arm_estremo_a(xg, xh, xd, xc);
		arm_estremo_a.Draw();

		Rect arm_estremo_b(xe, xf, xb, xa);
		arm_estremo_b.Draw();

		if (i == 0)
			alpha = PI/6*4;
		if (i == 1)
			alpha =PI / 3*4;
		if (i == 2)
			alpha = PI/6*10;
	}
}

void MyModel::DrawStartscreen() {
	// Start Screen
	Vertex a, b, c, d, e, f, g, h, i, l, m, n;
	glRotatef(0.0, 1.0, 0.0, 0.0);
	glRotatef(0.0, 0.0, 1.0, 0.0);
	// Command button
	a.SetP(-(buttonWidth / 2), buttonHeight / 2, 0.0);
	b.SetP((+buttonWidth / 2), buttonHeight / 2, 0.0);
	c.SetP((-buttonWidth / 2), -(buttonHeight / 2), 0.0);
	d.SetP((+buttonWidth / 2), -(buttonHeight / 2), 0.0);
	Rect commands(c, d, b, a);
	commands.Draw();
	if (buttCount != 1)
		glBindTexture(GL_TEXTURE_2D, texture[4]);
	else
		glBindTexture(GL_TEXTURE_2D, texture[5]);
	commands.DrawTextures();
	// Start button
	e.SetP(-buttonWidth / 2, buttonHeight * 2, 0.0);
	f.SetP(buttonWidth / 2, buttonHeight * 2, 0.0);
	g.SetP(-buttonWidth / 2, buttonHeight, 0.0);
	h.SetP(buttonWidth / 2, buttonHeight, 0.0);
	Rect start(g, h, f, e);
	if (buttCount != 0)
		glBindTexture(GL_TEXTURE_2D, texture[2]);
	else
		glBindTexture(GL_TEXTURE_2D, texture[3]);
	start.DrawTextures();
	// Quit button
	i.SetP(-buttonWidth / 2, -buttonHeight * 2, 0.0);
	l.SetP(buttonWidth / 2, -buttonHeight * 2, 0.0);
	m.SetP(-buttonWidth / 2, -buttonHeight, 0.0);
	n.SetP(buttonWidth / 2, -buttonHeight, 0.0);
	Rect quit(i, l, n, m);
	quit.Draw();
	if (buttCount != 2)
		glBindTexture(GL_TEXTURE_2D, texture[6]);
	else
		glBindTexture(GL_TEXTURE_2D, texture[7]);
	quit.DrawTextures();

	// logo
	Vertex  l1, l2, l3, l4;
	l1.SetColor(0.0, 0.0, 0.0);
	l2.SetColor(0.0, 0.0, 0.0);
	l3.SetColor(0.0, 0.0, 0.0);
	l4.SetColor(0.0, 0.0, 0.0);
	l1.SetP(-9.0, 4.5, 0.0);
	l2.SetP(9.0, 4.5, 0.0);
	l3.SetP(9.0, 6.5, 0.0);
	l4.SetP(-9.0, 6.5, 0.0);

	// Prossime due righe utili a mantenere la trasparenza delle immagini
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Rect log(l1, l2, l3, l4);
	glBindTexture(GL_TEXTURE_2D, texture[19]);
	log.DrawTextures();

	// Cursor
	Vertex cursorP;
	Vertex ca, cb, cc, cd;
	cursorP.x = e.x - cursorWidth / 2;
	cursorP.z = e.z;
	cursorP.y = e.y - cursorHeight / 2;

	switch (buttCount) {
	case 1:
		cursorP.y = a.y - cursorHeight / 2;
		break;
	case 2:
		cursorP.y = m.y - cursorHeight / 2;
		break;
	}

	ca.SetP(cursorP.x - cursorWidth / 2, cursorP.y - cursorHeight / 2, 0.0);
	cb.SetP(cursorP.x + cursorWidth / 2, cursorP.y - cursorHeight / 2, 0.0);
	cc.SetP(cursorP.x + cursorWidth / 2, cursorP.y + cursorHeight / 2, 0.0);
	cd.SetP(cursorP.x - cursorWidth / 2, cursorP.y + cursorHeight / 2, 0.0);
	Rect cursor(ca, cb, cc, cd);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	cursor.DrawTextures();
}

void MyModel::DrawCommands() {
	// Commands
	glRotatef(0.0, 1.0, 0.0, 0.0);
	glRotatef(0.0, 0.0, 1.0, 0.0);

	double butt = 1.0;
	Vertex ka, kb, kc, kd;
	double marginx, marginy;
	static const std::string command[10] = { "Move Up","Move Down","Move left","Move right","Rotate counterclockwise","Rotate clockwise","Mute music", "Back to Menu", "Move view", "Zoom" };

	marginx = 10.0;
	marginy = 7.0;
	ka.SetColor(0.0, 0.0, 0.0);
	kb.SetColor(0.0, 0.0, 0.0);
	kc.SetColor(0.0, 0.0, 0.0);
	kd.SetColor(0.0, 0.0, 0.0);


	kd.SetP(-marginx, marginy, 0.0);
	ka.SetP(kd.x, kd.y - butt, kd.z);
	kc.SetP(kd.x + butt, kd.y, kd.z);
	kb.SetP(kd.x + butt, kd.y - butt, kd.z);

	Rect com(ka, kb, kc, kd);
	com.Draw();
	glBindTexture(GL_TEXTURE_2D, texture[9]);
	com.DrawTextures();
	glColor3f(0.1f, 0.9f, 0.1f);
	glRasterPos3f(kc.x + butt, kc.y - butt / 2, 0.0f);
	this->glPrint("%s", command[0].c_str());
	int d = 0;

	for (int g = 0; g < 9; g++) {
		d++;
		if (g == 5) {
			kd.SetP(0.0 + 2.0, marginy, 0.0);
			ka.SetP(kd.x, kd.y - butt, kd.z);
			kc.SetP(kd.x + butt, kd.y, kd.z);
			kb.SetP(kd.x + butt, kd.y - butt, kd.z);
		}
		else {
			ka.y -= butt * 2;
			kb.y -= butt * 2;
			kc.y -= butt * 2;
			kd.y -= butt * 2;
		}
		Rect com(ka, kb, kc, kd);
		com.Draw();
		glBindTexture(GL_TEXTURE_2D, texture[10 + g]);
		com.DrawTextures();

		glColor3f(0.1f, 0.9f, 0.1f);
		glRasterPos3f(kc.x + butt, kc.y - butt / 2, 0.0f);
		this->glPrint("%s", command[d].c_str());

	}


	kd.SetP(0.0 + 2.0, marginy, 0.0);
	ka.SetP(kd.x, kd.y - butt, kd.z);
	kc.SetP(kd.x + butt, kd.y, kd.z);
	kb.SetP(kd.x + butt, kd.y - butt, kd.z);

	Rect com2(ka, kb, kc, kd);
	com2.Draw();
	glBindTexture(GL_TEXTURE_2D, texture[15]);
	com2.DrawTextures();
}

void MyModel::DrawGame() {
	// POW
	glTranslatef(0.0, -2.0, 0.0);
	glRotatef(RotX_a, 1.0, 0.0, 0.0);
	glRotatef(RotY_a, 0.0, 1.0, 0.0);

	// Controllo posizione corretta
	// Roba da disegnare
	if (!Perso) {
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		this->DrawWall();
		if (wallPosition < 10.0) {
			wallPosition += diff;
		}
		else {
			wallPosition = -10.0;
			diff += 0.001;
			if (size > 2)
				size -= 0.1;
			Randomize();
			score++;
		}
		// Floor
		this->DrawFloor();
		// Player
		this->DrawPlayer();
		double limit = fabs(size*PlayerThickness - PlayerThickness);
		double dist = fabs(PlayerPosition.x - holePosition.x);

		glDisable(GL_TEXTURE);
		// Collisioni
		if (wallPosition >= 10) {
			if (CheckPoint()) {
				Perso = true;
			}
		}
		this->SetProjection();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glPushMatrix();
		glTranslatef(0.0, -2.0, -22.0);
		glColor3f(1.0f, 0.0f, 0.0f);
		glRasterPos3f(2.0, -5.0, 5.0);
		this->glPrint("Score: %d", score);
		Perso = false;
	}
	else {
		// Cosa fare quando perso
		wallPosition = -20.0;
		Lose(score);
		/*
		if (die->isPlaying()) die->reset();
		else die->play();
		*/
	}
}

void MyModel::Randomize() {
	double rands = (rand() % 100)*0.01;
	randomX = limitesinistro + rands*(limitedestro - limitesinistro);
	rands = (rand() % 100) *0.01;
	randomY = limiteinferiore + rands*(limitesuperiore - limiteinferiore);
	randomR = (rand() % 100)*0.01 * 2 * PI;
}

bool MyModel::InitGL(void) {
	if (!this->LoadGLTextures()) {
		return false;
	}
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	this->BuildFont();
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
	return true;
}

bool MyModel::LoadGLTextures(void) {
	texture[0] = SOIL_load_OGL_texture("../Data/green.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texture[0] == 0) return false;
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	texture[1] = SOIL_load_OGL_texture("../Data/cursor.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texture[1] == 0) return false;
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	texture[2] = SOIL_load_OGL_texture("../Data/start1.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texture[2] == 0) return false;
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	texture[3] = SOIL_load_OGL_texture("../Data/start2.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texture[3] == 0) return false;
	glBindTexture(GL_TEXTURE_2D, texture[3]);

	texture[4] = SOIL_load_OGL_texture("../Data/comm1.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texture[4] == 0) return false;
	glBindTexture(GL_TEXTURE_2D, texture[4]);

	texture[5] = SOIL_load_OGL_texture("../Data/comm2.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texture[5] == 0) return false;
	glBindTexture(GL_TEXTURE_2D, texture[5]);

	texture[6] = SOIL_load_OGL_texture("../Data/quit1.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texture[6] == 0) return false;
	glBindTexture(GL_TEXTURE_2D, texture[6]);

	texture[7] = SOIL_load_OGL_texture("../Data/quit2.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texture[7] == 0) return false;
	glBindTexture(GL_TEXTURE_2D, texture[7]);


	texture[8] = SOIL_load_OGL_texture("../Data/black1.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texture[8] == 0) return false;
	glBindTexture(GL_TEXTURE_2D, texture[8]);

	texture[9] = SOIL_load_OGL_texture("../Data/up.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texture[9] == 0) return false;
	glBindTexture(GL_TEXTURE_2D, texture[9]);

	texture[10] = SOIL_load_OGL_texture("../Data/down.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texture[10] == 0) return false;
	glBindTexture(GL_TEXTURE_2D, texture[10]);

	texture[11] = SOIL_load_OGL_texture("../Data/left.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texture[11] == 0) return false;
	glBindTexture(GL_TEXTURE_2D, texture[11]);

	texture[12] = SOIL_load_OGL_texture("../Data/right.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texture[12] == 0) return false;
	glBindTexture(GL_TEXTURE_2D, texture[12]);

	texture[13] = SOIL_load_OGL_texture("../Data/A.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texture[13] == 0) return false;
	glBindTexture(GL_TEXTURE_2D, texture[13]);

	texture[14] = SOIL_load_OGL_texture("../Data/S.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texture[14] == 0) return false;
	glBindTexture(GL_TEXTURE_2D, texture[14]);

	texture[15] = SOIL_load_OGL_texture("../Data/M.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texture[15] == 0) return false;
	glBindTexture(GL_TEXTURE_2D, texture[15]);

	texture[16] = SOIL_load_OGL_texture("../Data/esc.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texture[16] == 0) return false;
	glBindTexture(GL_TEXTURE_2D, texture[16]);

	texture[17] = SOIL_load_OGL_texture("../Data/scroll.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texture[17] == 0) return false;
	glBindTexture(GL_TEXTURE_2D, texture[17]);

	texture[18] = SOIL_load_OGL_texture("../Data/click.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texture[18] == 0) return false;
	glBindTexture(GL_TEXTURE_2D, texture[18]);

	texture[19] = SOIL_load_OGL_texture("../Data/logo2.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texture[19] == 0) return false;
	glBindTexture(GL_TEXTURE_2D, texture[19]);

	texture[20] = SOIL_load_OGL_texture("../Data/black.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texture[20] == 0) return false;
	glBindTexture(GL_TEXTURE_2D, texture[20]);

	texture[21] = SOIL_load_OGL_texture("../Data/green.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texture[21] == 0) return false;
	glBindTexture(GL_TEXTURE_2D, texture[21]);

	texture[22] = SOIL_load_OGL_texture("../Data/lose.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texture[22] == 0) return false;
	glBindTexture(GL_TEXTURE_2D, texture[22]);

	texture[23] = SOIL_load_OGL_texture("../Data/testa.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texture[23] == 0) return false;
	glBindTexture(GL_TEXTURE_2D, texture[23]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return true;
}

void MyModel::ReSizeGLScene(int width, int height) {
	if (height == 0) height = 1;
	if (width == 0) width = 1;
	this->Wwidth = width;
	this->Wheight = height;
	glViewport(0, 0, width, height);
	this->Wwidth = width;
	this->Wheight = height;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool MyModel::DrawGLScene(void) {
	this->SetProjection();
	// timing
	clock_t t = clock();
	double elapsed = double(t - Tstamp) / (double)CLOCKS_PER_SEC;
	int ms_elapsed = (int)(t - Tstamp);
	this->Full_elapsed = double(t - Tstamp) / (double)CLOCKS_PER_SEC;
	this->frameTime += double(t - Tstamp) / (double)CLOCKS_PER_SEC;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glDisable(GL_LIGHTING);
	//POW
	glPushMatrix();
	glTranslatef(0.0, -1.0, -20.0);
	
	if (this->StartScreen) {
		this->DrawStartscreen();
	}
	else {
		if (selectButt[0] == 1) {
			this->DrawGame();
		}
		if (selectButt[1] == 1) {
			this->DrawCommands();
		}
		if (selectButt[2] == 1) {
			//quit
			PostQuitMessage(0);
			return 0;
		}
	}
	return true;
}

void MyModel::SetProjection() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
	gluPerspective(fovy, (GLfloat)Wwidth / (GLfloat)Wheight, 0.1, 200.0);
}

void MyModel::BuildFont(void) {
	HFONT font;
	HFONT oldfont;
	base = glGenLists(96);
	font = CreateFont(-20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, "Courier New");
	oldfont = (HFONT)SelectObject(hDC, font);
	wglUseFontBitmaps(hDC, 32, 96, base);
	SelectObject(hDC, oldfont);
	DeleteObject(font);
}

void MyModel::KillFont(void) {
	glDeleteLists(base, 96);
}

void MyModel::SetLevel(void) {
	size = 4; // da 4 si scende a max 2
	diff = 0.01;
	score = 0;
}

void MyModel::Lose(int score) {
	Perso = true;
	glRotatef(0.0, 1.0, 0.0, 0.0);
	glRotatef(0.0, 0.0, 1.0, 0.0);
	Vertex a, b, c, d;
	double l=5.0;
	
	a.SetP(0.0 - wallLargh/2, 0.0 , 10.5);
	b.SetP(0.0 + wallLargh / 2, 0.0 , 10.5);
	c.SetP(0.0 + wallLargh / 2, 0.0 + wallAltezza , 10.5);
	d.SetP(0.0 - wallLargh / 2, 0.0 + wallAltezza , 10.5);
	
	Rect lost(a,b,c,d);
	lost.Draw();
	glBindTexture(GL_TEXTURE_2D, texture[22]);
	lost.DrawTextures();
}

void MyModel::glPrint(const char * fmt, ...) {
	char text[256];
	va_list ap;
	if (fmt == NULL)
		return;
	va_start(ap, fmt);
	vsprintf (text, fmt, ap);
	va_end(ap);
	glPushAttrib(GL_LIST_BIT);
	glListBase(base - 32);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}

boolean MyModel::lost() {
	if (Perso == true)
		return true;
	else
		return false;
}

boolean MyModel::CheckPoint() {
	boolean temp = true;
	for each (Vertex v in Body) {
		temp = true;
		for (int j = 0; j<20; j = j + 4) {
			if (!included(HoleBody, v, j)) {
				temp = false;
			}
			else {
				temp = true;
				break;
			}
		}
		if (!temp)
			return false;
	}
	return true;
}

boolean MyModel::included(Vertex HoleBody[], Vertex v, int j) {
	boolean t1, t2, t3, t4;
	float x0 = v.x;
	float y0 = v.y;
	float x1 = HoleBody[0+j].x;
	float y1 = HoleBody[0+j].y;
	float x2 = HoleBody[1+j].x;
	float y2 = HoleBody[1+j].y;
	float a = abs((0.5)*(x1*y2 - y1*x2 - x0*y2 + y0*x2 + x0*y1 - y0*x1));

	if ((0.5)*(x1*y2 - y1*x2 - x0*y2 + y0*x2 + x0*y1 - y0*x1) > 0)
		t1 = true;
	else
		t1 = false;

	x1 = HoleBody[1+j].x;
	y1 = HoleBody[1+j].y;
	x2 = HoleBody[2 + j].x;
	y2 = HoleBody[2 + j].y;
	a += abs((0.5)*(x1*y2 - y1*x2 - x0*y2 + y0*x2 + x0*y1 - y0*x1));
	if ((0.5)*(x1*y2 - y1*x2 - x0*y2 + y0*x2 + x0*y1 - y0*x1) > 0)
		t2 = true;
	else
		t2 = false;
	x1 = HoleBody[2 + j].x;
	y1 = HoleBody[2 + j].y;
	x2 = HoleBody[3 + j].x;
	y2 = HoleBody[3 + j].y;
	a += abs((0.5)*(x1*y2 - y1*x2 - x0*y2 + y0*x2 + x0*y1 - y0*x1));
	if ((0.5)*(x1*y2 - y1*x2 - x0*y2 + y0*x2 + x0*y1 - y0*x1) > 0)
		t3 = true;
	else
		t3 = false;
	x1 = HoleBody[3 + j].x;
	y1 = HoleBody[3 + j].y;
	x2 = HoleBody[0 + j].x;
	y2 = HoleBody[0 + j].y;
	a += abs((0.5)*(x1*y2 - y1*x2 - x0*y2 + y0*x2 + x0*y1 - y0*x1));
	if ((0.5)*(x1*y2 - y1*x2 - x0*y2 + y0*x2 + x0*y1 - y0*x1) > 0)
		t4 = true;
	else
		t4 = false;
	if (a <= size*PlayerThickness*PlayerBodyHeight) {
		MessageBox(NULL, "True.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return true;
	}
	else {
		MessageBox(NULL, "False.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
}
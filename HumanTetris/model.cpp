#include "model.h"
#include "vertex.h"
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h> 
#include "SOIL.h"

#pragma comment( lib, "opengl32.lib" )			
#pragma comment( lib, "glu32.lib" )					
#pragma comment( lib, "winmm.lib" )					

MyModel::MyModel(): hDC(NULL), hRC(NULL), hWnd(NULL), active(true), frames(0), fps(0), cursor(true), captured(false), StartScreen(true), Perso(false), fovy(45.0), RotX_a(0), RotY_a(0) {
	
	//butt
	buttonWidth = 4.5;
	buttonHeight = 1.5;
	cursorWidth = 0.5;
	
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
	floorFront.Draw();
	floorBack.Draw();
	floorTop.Draw();
	floorBottom.Draw();
	floorSideA.Draw();
	floorSideB.Draw();
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

void MyModel::DrawPlayerOnWall(Vertex position, float rotation, float size) {
	float spessore = wallProf + 0.01;
	// Head
	position.r = 0.0;
	position.g = 0.0;
	position.b = 0.0;
	Vertex HeadPosition = position;
	HeadPosition.y += (PlayerBodyHeight / 2 + size*PlayerHeadSize)*sin(PI / 2 + rotation);
	HeadPosition.x += (PlayerBodyHeight / 2 + size*PlayerHeadSize)*cos(PI / 2 + rotation);
	Cylinder Head(HeadPosition, size*PlayerHeadSize/1.2, spessore);
	Head.Draw();
	// Body
	float diag = sqrt(pow((PlayerBodyHeight / 2), 2.0) + pow((size*PlayerThickness / 2), 2.0));
	float angle = asin((PlayerBodyHeight / 2) / diag);
	Vertex ba, bb, bc, bd, be, bf, bg, bh;
	ba = bb = bc = bd = be = bf = bg = bh = position;
	ba.x = position.x + ((diag)*cos(PI + angle + rotation));
	ba.y = position.y + ((diag)*sin(PI + angle + rotation));
	ba.z = position.z + spessore / 2;

	bb.x = position.x + ((diag)*cos(2 * PI - angle + rotation));
	bb.y = position.y + ((diag)*sin(-angle + 2 * PI + rotation));
	bb.z = position.z + spessore / 2;

	bc.x = position.x + ((diag)*cos(angle + rotation));
	bc.y = position.y + ((diag)*sin(angle + rotation));
	bc.z = position.z + spessore / 2;

	bd.x = position.x + (diag)*cos(PI - angle + rotation);
	bd.y = position.y + (diag)*sin(PI - angle + rotation);
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
	// Arms
	float PlayerArmHeight = size*PlayerBodyHeight / 5 * 3;
	float ArmDist;
	float angle2;
	float angle3;
	float ArmThickness = size*PlayerThickness / 1.5;
	float ArmDiag;
	float alpha = PI / 3;

	ArmDiag = sqrt(pow((PlayerArmHeight / 2), 2.0) + pow((ArmThickness / 2), 2.0));
	angle2 = asin(sin(90) / diag * size*PlayerThickness / 2) - alpha;
	angle3 = acos(cos(angle2)*diag / PlayerArmHeight / 2);

	ArmDist = sqrt(pow((PlayerArmHeight / 2), 2.0) + pow(diag, 2.0) - 2 * diag*PlayerArmHeight / 2 * cos(PI - angle2 - angle3));
	float beta = PI / 6;
	float angle_arm = asin((PlayerArmHeight / 2) / ArmDiag);
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
	Head.Draw();
	// Body
	float diag = sqrt(pow((PlayerBodyHeight / 2),2.0) + pow((PlayerThickness / 2),2.0));
	ba = bb = bc = bd = be = bf = bg = bh = PlayerPosition;
	float angle = asin((PlayerBodyHeight / 2) / diag) ;

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
	
	// Arms
	float PlayerArmHeight = PlayerBodyHeight / 4*3;
	float ArmDist;
	float angle2;
	float angle3;
	float ArmThickness = PlayerThickness / 2;
	float ArmDiag;
	float alpha = PI / 3;

	ArmDiag = sqrt(pow((PlayerArmHeight / 2), 2.0) + pow((ArmThickness / 2), 2.0));
	angle2 = asin( sin(90) / diag * PlayerThickness / 2) - alpha;
	angle3 =acos( cos(angle2)*diag / PlayerArmHeight/2);
	
	ArmDist = sqrt(pow((PlayerArmHeight / 2), 2.0) + pow(diag, 2.0) - 2 * diag*PlayerArmHeight/2*cos(PI -angle2 - angle3));
	float beta = PI / 6;
	float angle_arm = asin((PlayerArmHeight / 2) / ArmDiag);
	Vertex ArmPos, xa, xb, xc, xd, xe, xf, xg, xh;
	xa = xb = xc = xd = xe = xf = xg = xh = ba;
	for (int i = 0; i < 4; i++) {
		xa.x = PlayerPosition.x + (ArmDist - ArmThickness)*cos(alpha + PlayerRotation) + ((ArmDiag)*cos(PI+ angle_arm + PlayerRotation - alpha));
		xa.y = PlayerPosition.y + (ArmDist - ArmThickness)*sin(alpha + PlayerRotation) + ((ArmDiag)*sin(PI + angle_arm + PlayerRotation - alpha));
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

void MyModel::Randomize() {
	float rands = (rand() % 100)*0.01;
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
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
	return true;
}

bool MyModel::LoadGLTextures(void) {
	texture[0] = SOIL_load_OGL_texture("../Data/image0.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texture[0] == 0) return false;
	glBindTexture(GL_TEXTURE_2D, texture[0]);
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
	RECT R;
	GetClientRect(hWnd, &R);
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
	// Start Screen
	Vertex a, b, c, d,e,f,g,h,i,l,m,n;
	Vertex cursorP;
	Vertex ca, cb, cc, cd;
	if (this->StartScreen) {
		

		glRotatef(0.0, 1.0, 0.0, 0.0);
		glRotatef(0.0, 0.0, 1.0, 0.0);

		a.SetP(-(buttonWidth / 2),  buttonHeight / 2, 0.0);
		a.SetColor(0.0, 0.0, 1.0);
		b.SetP(( + buttonWidth / 2),  buttonHeight / 2, 0.0);
		b.SetColor(0.0, 0.0, 1.0);
		c.SetP(( -buttonWidth / 2),  -(buttonHeight / 2), 0.0);
		c.SetColor(1.0, 0.0, 1.0);
		d.SetP(( + buttonWidth / 2),  -(buttonHeight / 2), 0.0);
		d.SetColor(0.0, 1.0, 1.0);
		
		e.SetP(-buttonWidth / 2,  buttonHeight * 2, 0.0);
		e.SetColor(0.0, 0.0, 1.0);
		f.SetP(buttonWidth / 2, buttonHeight * 2, 0.0);
		f.SetColor(0.0, 0.0, 1.0);
		g.SetP( -buttonWidth / 2, buttonHeight , 0.0);
		g.SetColor(1.0, 0.0, 1.0);
		h.SetP( buttonWidth / 2, buttonHeight, 0.0);
		h.SetColor(0.0, 1.0, 1.0);

		i.SetP(-buttonWidth / 2, -buttonHeight * 2, 0.0);
		i.SetColor(0.0, 0.0, 1.0);
		l.SetP(buttonWidth / 2, -buttonHeight * 2, 0.0);
		l.SetColor(0.0, 0.0, 1.0);
		m.SetP(-buttonWidth / 2, -buttonHeight, 0.0);
		m.SetColor(1.0, 0.0, 1.0);
		n.SetP(buttonWidth / 2, -buttonHeight, 0.0);
		n.SetColor(0.0, 1.0, 1.0);

		Rect start(g, h, f, e);
		start.Draw();
		Rect commands(c,d,b,a);
		commands.Draw();
		Rect quit(n, m, i, l);
		quit.Draw();

		cursorP.x = a.x - cursorWidth/2;
		cursorP.z = a.z;
		cursorP.y = a.y - buttonHeight / 2;

		ca.SetP(-(buttonWidth / 2), buttonHeight / 2, 0.0);
		ca.SetColor(0.0, 0.0, 1.0);
		cb.SetP((+buttonWidth / 2), buttonHeight / 2, 0.0);
		cb.SetColor(0.0, 0.0, 1.0);
		cc.SetP((-buttonWidth / 2), -(buttonHeight / 2), 0.0);
		cc.SetColor(1.0, 0.0, 1.0);
		cd.SetP((+buttonWidth / 2), -(buttonHeight / 2), 0.0);
		cd.SetColor(0.0, 1.0, 1.0);

		Rect cursor(cc, cd, cb, ca);
		cursor.Draw();

		if (WM_KEYDOWN) {
			Data.keys[WM_KEYDOWN] = FALSE;
			//spostare puntatore
		}
		
	}
	else {
		
		// POW
	
		glRotatef(RotX_a, 1.0, 0.0, 0.0);//<---
		glRotatef(RotY_a, 0.0, 1.0, 0.0);
		// Roba da disegnare
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		this->DrawWall();
		if (wallPosition < 10.0) {
			wallPosition += diff;
		}
		else {
			wallPosition = -10.0;
			diff += 0.001;
			size -= 0.001;
			Randomize();
		}
		// Floor
		this->DrawFloor();
		// Player
		this->DrawPlayer();


		glDisable(GL_TEXTURE);
	}
	return true;
}

void MyModel::SetProjection() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
	gluPerspective(fovy, (GLfloat)Wwidth / (GLfloat)Wheight, 0.1, 200.0);
}
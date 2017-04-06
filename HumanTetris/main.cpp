//	Cosi` vede la rotella del mouse ...
//	needed to operate the mouse wheel..
#if(_WIN32_WINNT < 0x400)
#define _WIN32_WINNT	0x0400
#endif


//  INCLUDE
#include <windows.h>      // all windows..
#include <gl\gl.h>        // OPENGL
#include <gl\glu.h>       // OPENGL
#include <stdio.h>        // io standard
#include <stdlib.h>       // librerie standard
#include "vertex.h"
#include "model.h"
#include <iostream>

// resources (menu, icons...)

//  LIBRERIE OPENGL e multimendia
//	OpenGL libraries
#pragma comment( lib, "opengl32.lib" )				// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )						// Search For GLu32.lib While Linking
#pragma comment( lib, "winmm.lib" )						// Search For WinMM Library While Linking

typedef struct CommonData {
	HINSTANCE Shinstance;
	HWND      MainW;
	HDC dc;
	HGLRC hRC;        // Rendering context
	bool lCaptured;
	int lxs, lys;     // posizione iniziale / initial position
	int ldx, ldy;     // delta
	GLfloat	RotX_a;   // angolo di rotazione in x / x angle
	GLfloat	RotY_a;   // angolo di rotazione in y / y angle

	GLdouble fovy;    // angolo di vista in y (gradi, init a 45.0)
					  // view angle in y (degrees)

	CommonData() : lCaptured(false), RotX_a(0), RotY_a(0),
		fovy(45.0)
	{}
} CoDa;

static CoDa CS;

GLvoid InitGL();
GLvoid SetProjection(GLsizei iWidth, GLsizei iHeight);
GLvoid DrawGLScene(GLvoid);

//  callback
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_CREATE:	// here we must make the window suitable for OpenGL rendering
	{
		CS.dc = GetDC(hwnd);
		PIXELFORMATDESCRIPTOR pfd;
		ZeroMemory(&pfd, sizeof(pfd));
		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 24;
		pfd.cDepthBits = 16;
		pfd.iLayerType = PFD_MAIN_PLANE;
		int iFormat = ChoosePixelFormat(CS.dc, &pfd);
		SetPixelFormat(CS.dc, iFormat, &pfd);  // Set pixel format
		CS.hRC = wglCreateContext(CS.dc);  // Set rendering context
		wglMakeCurrent(CS.dc, CS.hRC);
		InitGL();
		return 0;
	}

	case WM_LBUTTONDOWN:
		if (!CS.lCaptured) {
			CS.lCaptured = true;
			CS.lxs = LOWORD(lParam);
			CS.lys = HIWORD(lParam);
			SetCapture(hwnd);
			InvalidateRect(hwnd, NULL, FALSE);
		}
		break;

	case WM_MOUSEMOVE:
		if (CS.lCaptured) {
			CS.ldx = LOWORD(lParam) - CS.lxs;
			CS.ldy = HIWORD(lParam) - CS.lys;

			CS.lxs = LOWORD(lParam);
			CS.lys = HIWORD(lParam);
			CS.RotX_a += (GLfloat)(CS.ldy * 0.4);
			CS.RotY_a += (GLfloat)(CS.ldx * 0.4);
			InvalidateRect(hwnd, NULL, FALSE);
		}
		break;

	case WM_LBUTTONUP:
		if (CS.lCaptured) { CS.lCaptured = false; ReleaseCapture(); }
		InvalidateRect(hwnd, NULL, FALSE);
		break;

	case WM_CLOSE:
		wglMakeCurrent(NULL, NULL);
		ReleaseDC(CS.MainW, CS.dc);
		wglDeleteContext(CS.hRC);
		break;

	case WM_MOUSEWHEEL: {
		int t, z;
		z = (short)HIWORD(wParam);
		t = (z / WHEEL_DELTA);
		//	Mouse wheel "activation"
		CS.fovy += t * 0.5;
		InvalidateRect(hwnd, NULL, FALSE);
		return 0;
	}
						break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hwnd, &ps);
		wglMakeCurrent(hdc, CS.hRC);
		RECT R;
		GetClientRect(CS.MainW, &R);
		if (R.right > 0 && R.bottom > 0) SetProjection(R.right, R.bottom);
		DrawGLScene(); glFlush(); SwapBuffers(hdc);
		EndPaint(hwnd, &ps);
	}
	return 0;

	case WM_DESTROY:
		PostQuitMessage(0); break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

bool RegistraClasse(HINSTANCE hInstance)
{
	WNDCLASSEX wc;
	static bool registrata = false;
	if (registrata) return true;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	wc.lpfnWndProc = (WNDPROC)MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_3DSHADOW + 1);
	wc.lpszMenuName = "MAIN";
	wc.lpszClassName = "MainWindow";
	wc.hIconSm = LoadIcon(hInstance, IDI_WINLOGO);

	if (!RegisterClassEx(&wc)) return false;
	registrata = true;
	return true;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	MSG Msg;
	if (!RegistraClasse(hInstance)) exit(1);
	CS.Shinstance = hInstance;

	CS.MainW = CreateWindowEx(WS_EX_LEFT, "MainWindow", "HumanTetris",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		100, 100, 900, 600, NULL, NULL, hInstance, NULL);

	ShowWindow(CS.MainW, iCmdShow);
	UpdateWindow(CS.MainW);

	while (GetMessage(&Msg, NULL, 0, 0)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}


GLvoid SetProjection(GLsizei iWidth, GLsizei iHeight)
{
	if (iHeight == 0) iHeight = 1;
	glViewport(0, 0, iWidth, iHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 0.0,
		0.0, 0.0, -1.0,
		0.0, 1.0, 0.0);
	gluPerspective(CS.fovy, (GLfloat)iWidth / (GLfloat)iHeight, 0.1, 200.0);
}

/* Set the rendering options for OpenGL */
GLvoid InitGL()
{
	// COSE
	glCullFace(GL_BACK);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	//glShadeModel(GL_FLAT);


	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	glPointSize(2); // !!
}

//	The cube dimensions are a 2 x 2 x 2, defined around the origin
//	then rotated around the x and the y axes
//	then the center is transalated in (0,0,-5)

GLvoid DrawGLScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(0.0, -1.0, -20.0);
	glRotatef(CS.RotX_a, 1.0, 0.0, 0.0);
	glRotatef(CS.RotY_a, 0.0, 1.0, 0.0);

	// BASE
	float baseLargh = 5.0;
	float baseProf = 20.0;
	float baseAltezza = 1.0;

	Vertex ba(-(baseLargh / 2), 0.0, (baseProf / 2));
	ba.SetColor(0.0, 0.0, 1.0);
	Vertex bb((baseLargh / 2), 0.0, (baseProf / 2));
	bb.SetColor(0.0, 0.0, 1.0);
	Vertex bc((baseLargh / 2), 0.0, -(baseProf / 2));
	bc.SetColor(0.0, 0.0, 1.0);
	Vertex bd(-(baseLargh / 2), 0.0, -(baseProf / 2));
	bd.SetColor(0.0, 0.0, 1.0);
	Rect baseTop(ba, bb, bc, bd);
	baseTop.Draw();

	Vertex be(-(baseLargh / 2), -baseAltezza, (baseProf / 2));
	be.SetColor(0.0, 1.0, 1.0);
	Vertex bf((baseLargh / 2), -baseAltezza, (baseProf / 2));
	bf.SetColor(0.0, 1.0, 1.0);
	Vertex bg((baseLargh / 2), -baseAltezza, -(baseProf / 2));
	bg.SetColor(0.0, 1.0, 1.0);
	Vertex bh(-(baseLargh / 2), -baseAltezza, -(baseProf / 2));
	bh.SetColor(0.0, 1.0, 1.0);
	Rect baseBottom(bh, bg, bf, be);
	baseBottom.Draw();

	Rect latoA(be, bf, bb, ba);
	latoA.Draw();
	Rect latoB(bf, bg, bc, bb);
	latoB.Draw();
	Rect latoC(bg, bh, bd, bc);
	latoC.Draw();
	Rect latoD(bh, be, ba, bd);
	latoD.Draw();

	// MURO
	float muroLargh = 5.0;
	float muroProf = 2.0;
	float muroAltezza = 7.0;
	float posizioneMuro = -10.0; // da non lasciare qua quando si vuole muovere
	Vertex posMuro(0.0, 0.0, posizioneMuro);
	posMuro.SetColor(1.0, 0, 0);

	Vertex ma(-(muroLargh / 2), 0.0, posizioneMuro + muroProf);
	ma.SetColor(0.0, 1.0, 0.0);
	Vertex mb(+(muroLargh / 2), 0.0, posizioneMuro + muroProf);
	mb.SetColor(0.0, 1.0, 0.0);
	Vertex mc(+(muroLargh / 2), muroAltezza, posizioneMuro + muroProf);
	mc.SetColor(0.0, 1.0, 0.0);
	Vertex md(-(baseLargh / 2), muroAltezza, posizioneMuro + muroProf);
	md.SetColor(0.0, 1.0, 0.0);
	Rect muroFronte(ma, mb, mc, md);
	muroFronte.Draw();

	Vertex me(-(muroLargh / 2), 0.0, posizioneMuro);
	me.SetColor(0.0, 1.0, 0.0);
	Vertex mf(+(muroLargh / 2), 0.0, posizioneMuro);
	mf.SetColor(0.0, 1.0, 0.0);
	Vertex mg(+(muroLargh / 2), muroAltezza, posizioneMuro);
	mg.SetColor(0.0, 1.0, 0.0);
	Vertex mh(-(baseLargh / 2), muroAltezza, posizioneMuro);
	mh.SetColor(0.0, 1.0, 0.0);
	Rect muroRetro(mf, me, mh, mg);
	muroRetro.Draw();

	Rect muroTop(md, mc, mg, mh);
	muroTop.Draw();
	Rect muroLatoA(mb, mf, mg, mc);
	muroLatoA.Draw();
	Rect muroLatoB(me, ma, md, mh);
	muroLatoB.Draw();

	glPopMatrix();
}

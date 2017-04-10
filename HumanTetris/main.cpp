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
	float movement = 0.1; // Di quanto si muove l'omino ogni volta che premo un tasto

	CommonData() : lCaptured(false), RotX_a(0), RotY_a(0),
		fovy(45.0)
	{}
	MyModel modello;
	bool keys[256];
} CoDa;

static CoDa CS;

GLvoid InitGL();
GLvoid SetProjection(GLsizei iWidth, GLsizei iHeight);
GLvoid DrawGLScene(GLvoid);

//  callback
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_CREATE:
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
	case WM_KEYDOWN:
		CS.keys[wParam] = TRUE;
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_KEYUP:
		CS.keys[wParam] = FALSE;
		InvalidateRect(hwnd, NULL, FALSE);
		break;

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
		if (R.right > 0 && R.bottom > 0)
			SetProjection(R.right, R.bottom);
		DrawGLScene();
		glFlush();
		SwapBuffers(hdc);
		EndPaint(hwnd, &ps);
	}
	return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	
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
	BOOL done = FALSE;
	if (!RegistraClasse(hInstance)) exit(1);
	CS.Shinstance = hInstance;

	CS.MainW = CreateWindowEx(WS_EX_LEFT, "MainWindow", "HumanTetris",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		100, 100, 900, 600, NULL, NULL, hInstance, NULL);

	ShowWindow(CS.MainW, iCmdShow);
	UpdateWindow(CS.MainW);
	while (!done) {
		if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) {
			if (Msg.message == WM_QUIT) {
				// When x to close the window is pressed
				done = TRUE;
			}
			else {
				TranslateMessage(&Msg);
				DispatchMessage(&Msg);
			}
		}
		else {
			if (CS.keys[VK_UP]) {
				CS.keys[VK_UP] = FALSE;
				Vertex temp = CS.modello.GetPlayerPosition();
				temp.y += CS.movement;
				CS.modello.SetPlayerPosition(temp);
			}
			if (CS.keys[VK_DOWN]) {
				CS.keys[VK_DOWN] = FALSE;
				Vertex temp = CS.modello.GetPlayerPosition();
				temp.y -= CS.movement;
				CS.modello.SetPlayerPosition(temp);
			}
			if (CS.keys[VK_LEFT]) {
				CS.keys[VK_LEFT] = FALSE;
				Vertex temp = CS.modello.GetPlayerPosition();
				temp.x -= CS.movement;
				CS.modello.SetPlayerPosition(temp);
			}
			if (CS.keys[VK_RIGHT]) {
				CS.keys[VK_RIGHT] = FALSE;
				Vertex temp = CS.modello.GetPlayerPosition();
				temp.x += CS.movement;
				CS.modello.SetPlayerPosition(temp);
			}

		}
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

GLvoid InitGL()
{
	glCullFace(GL_BACK);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	glPointSize(2); // !!
}

GLvoid DrawGLScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(0.0, -1.0, -20.0); // POW
	glRotatef(CS.RotX_a, 1.0, 0.0, 0.0);
	glRotatef(CS.RotY_a, 0.0, 1.0, 0.0);
	// Floor
	CS.modello.DrawFloor();
	// Wall
	CS.modello.DrawWall();
	// Player
	CS.modello.DrawPlayer();
	glPopMatrix();
}

////////////////////////////////////////////////////////////////////////
//	From http://nehe.gamedev.net/ "Legacy Tutorials"
//
//	You can find several tutorials with help (in english),
//	please explore them...
////////////////////////////////////////////////////////////////////////
//
//  Esercitazione sulle OPENGL n. 1
//
//  1)  capire il codice (l'help si attiva con tasto F1 e comprende
//      le funzioni OpenGL
//  2)  attivare la rotella del mouse
//  3)  cambiare qualcosa: ad esempio i colori del cubo o dei punti
//	4)  cambiare il fron e back clipping planes per "tagliare" il cubo
//  5)  cambiare qualcosa d'altro: ad esempio trasformare il
//      cubo in piramide tronca (o, se siete abili, in cilindro)
//
////////////////////////////////////////////////////////////////////////
//	OPENGL tutorial n. 1
//
//	1) understand the code, and use the F1 key for the help (the microsoft
//     visual studio has the help for OpenGL, but NOT for DirectX !
//	2) activate the mouse wheel
//	3) change something, for example the cube or the points colors
//	4) change the front and back clipping planes to "crop" the cube
//	5) change something other: for example transform the cube in a pyramid
//	   or (more difficult) in a cylinder.
///////////////////////////////////////////////////////////////////////

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

//  Funzioni usate prima della definizione
//	Two routines declared before they are defined
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

/*	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case CM_FILE_EXIT:	//  FILE -> exit
			PostMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		}
		break;*/

	case WM_LBUTTONDOWN:
		if (!CS.lCaptured) {
			CS.lCaptured = true;
			CS.lxs = LOWORD(lParam); CS.lys = HIWORD(lParam);
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

	case WM_MOUSEWHEEL:
	{	int t, z;
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
		wglMakeCurrent(hdc, CS.hRC);  // inutile / not necessary in this program
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


//  Registra il tipo di classe per la finestra principale (l'unica)
//  compatibile con il disegno in OPENGL
// Before creating a window you need to define it
// This function return true if all is ok, or false in case of errors.
// Some style is for OpenGL (see also the WM_CREATE message)
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
	wc.hIcon = LoadIcon(hInstance,  IDI_WINLOGO);
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

	CS.MainW = CreateWindowEx(WS_EX_LEFT, "MainWindow", "OpenGL 1 Tutorial",
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
	//	Try to change the front clipping plane, for example:
	//gluPerspective(CS.fovy,(GLfloat)iWidth/(GLfloat)iHeight,4,200.0);
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
	glTranslatef(0.0, 0.0, -5.0);
	glRotatef(CS.RotX_a, 1.0, 0.0, 0.0);
	glRotatef(CS.RotY_a, 0.0, 1.0, 0.0);
	//	QUADS: every 4 vertex OpenGL draw a 4 edge polygon.
	glBegin(GL_QUADS);
	// Front Face
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-1.0, -1.0, 1.0);	// Bottom Left
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(1.0, -1.0, 1.0);	// Bottom Right
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.5, 1.0, 0.5);	// Top Right
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-0.5, 1.0, 0.5);	// Top Left
								// Back Face
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-1.0, -1.0, -1.0);	// Bottom Right
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-0.5, 1.0, -0.5);	// Top Right
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.5, 1.0, -0.5);	// Top Left
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(1.0, -1.0, -1.0);	// Bottom Left
									// Top Face
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(-0.5, 1.0, -0.5);	// Top Left
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(-0.5, 1.0, 0.5);	// Bottom Left
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.5, 1.0, 0.5);	// Bottom Right
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.5, 1.0, -0.5);	// Top Right
								// Bottom Face
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(-1.0, -1.0, -1.0);	// Top Right
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(1.0, -1.0, -1.0);	// Top Left
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(1.0, -1.0, 1.0);	// Bottom Left
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(-1.0, -1.0, 1.0);	// Bottom Right
									// Right face
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(1.0, -1.0, -1.0);	// Bottom Right
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.5, 1.0, -0.5);	// Top Right
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.5, 1.0, 0.5);	// Top Left
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(1.0, -1.0, 1.0);	// Bottom Left
								// Left Face
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(-1.0, -1.0, -1.0);	// Bottom Left
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(-1.0, -1.0, 1.0);	// Bottom Right
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(-0.5, 1.0, 0.5);	// Top Right
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(-0.5, 1.0, -0.5);	// Top Left
	glEnd();
	//  prova punti gialli
	glBegin(GL_POINTS);     //  AAAA
	glColor3f(1.0, 1.0, 0.0);
	glVertex3d(-1.2, 1.2, -1.2);
	glVertex3d(1.2, 1.2, 1.2);
	glVertex3d(1.2, -1.2, 1.2);
	glEnd();          //  AAAA

	glPopMatrix();
}

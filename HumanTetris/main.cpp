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
#include "audiere.h"
#include <iostream>
#include "SOIL.h"

using namespace audiere;

#pragma comment( lib, "opengl32.lib" )					// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )						// Search For GLu32.lib While Linking
#pragma comment( lib, "winmm.lib" )						// Search For WinMM Library While Linking

class MyModel Data;

typedef struct CommonData {
	bool lCaptured;
	int lxs, lys;     // posizione iniziale / initial position
	int ldx, ldy;     // delta
	float movement = 0.1; // Di quanto si muove l'omino ogni volta che premo un tasto
	float rotation = 0.1;

	CommonData() : lCaptured(false){
	}
} CoDa;
static CoDa CS;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

GLvoid KillGLWindow(GLvoid) {

}

BOOL CreateGLWindow(char* title, int width, int height, int bits) {
	GLuint		PixelFormat;
	WNDCLASS	wc;
	DWORD		dwExStyle;
	DWORD		dwStyle;
	RECT		WindowRect;
	WindowRect.left = (long)0;
	WindowRect.right = (long)width;
	WindowRect.top = (long)0;
	WindowRect.bottom = (long)height;
	Data.hInstance = GetModuleHandle(NULL);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;	
	wc.cbWndExtra = 0;
	wc.hInstance = Data.hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "HumanTetris";
	if (!RegisterClass(&wc)) {
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	dwStyle = WS_OVERLAPPEDWINDOW;
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);
	if (!(Data.hWnd = CreateWindowEx(dwExStyle,							// Extended Style For The Window
		"HumanTetris",							// Class Name
		title,								// Window Title
		dwStyle |							// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		WindowRect.right - WindowRect.left,	// Calculate Window Width
		WindowRect.bottom - WindowRect.top,	// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		Data.hInstance,							// Instance
		NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}
	static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	if (!(Data.hDC = GetDC(Data.hWnd))) {
		KillGLWindow();
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	if (!(PixelFormat = ChoosePixelFormat(Data.hDC, &pfd))) {
		KillGLWindow();				
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	if (!SetPixelFormat(Data.hDC, PixelFormat, &pfd)) {
		KillGLWindow();
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!(Data.hRC = wglCreateContext(Data.hDC))) {
		KillGLWindow();
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!wglMakeCurrent(Data.hDC, Data.hRC)) {
		KillGLWindow();	
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	ShowWindow(Data.hWnd, SW_SHOW);
	SetForegroundWindow(Data.hWnd);
	SetFocus(Data.hWnd);
	Data.ReSizeGLScene(width, height);
	if (!Data.InitGL())	{
		KillGLWindow();
		MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_ACTIVATE:
		if (!HIWORD(wParam)) {
			Data.active = TRUE;
		}
		else {
			Data.active = FALSE;
		}
		return 0;

		// MOUSE
	case WM_LBUTTONUP:
		if (CS.lCaptured) {
			CS.lCaptured = false;
			ReleaseCapture();
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_LBUTTONDOWN:
		if (!CS.lCaptured) {
			CS.lCaptured = true;
			CS.lxs = LOWORD(lParam);
			CS.lys = HIWORD(lParam);
			SetCapture(hWnd);
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;

	case WM_MOUSEMOVE:
		if (CS.lCaptured) {
			CS.ldx = LOWORD(lParam) - CS.lxs;
			CS.ldy = HIWORD(lParam) - CS.lys;
			CS.lxs = LOWORD(lParam);
			CS.lys = HIWORD(lParam);
			Data.RotX_a += (GLfloat)(CS.ldy * 0.4);
			Data.RotY_a += (GLfloat)(CS.ldx * 0.4);
			InvalidateRect(hWnd, NULL, FALSE);
			
		}
		break;
	case WM_SYSCOMMAND:
		switch (wParam) {
		case SC_SCREENSAVE:
		case SC_MONITORPOWER:
			return 0;
		}
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
		Data.keys[wParam] = TRUE;
		return 0;
	case WM_KEYUP:
		Data.keys[wParam] = FALSE;
		return 0;

	case WM_SIZE:
		Data.ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));
		return 0;
	case WM_MOUSEWHEEL:
		int t, z;
		z = (short)HIWORD(wParam);
		t = (z / WHEEL_DELTA);
		Data.fovy += t * 0.5;
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	MSG msg;
	BOOL done = FALSE;
	if (!CreateGLWindow("Human Tetris", 640, 480, 16)) {
		return 0;
	}
	//  AUDIO - start
	AudioDevicePtr device(OpenDevice());
	if (!device) {
		return 0;
	}
	OutputStreamPtr stream(OpenSound(device, "../Data/space.wav", true));
	if (!stream) {
		return 0;
	}
	stream->setRepeat(true);
	stream->setVolume(0.5f);
	stream->play();
	OutputStreamPtr limit(OpenSound(device, "../Data/limit.wav", false));
	OutputStreamPtr stupid(OpenSound(device, "../Data/stupid.wav", false));

	while (!done) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				done = TRUE;
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else {
			if ((Data.active && !Data.DrawGLScene() )) {
				done = TRUE;
			}
			else {
				SwapBuffers(Data.hDC);
			}
			if (Data.keys[VK_UP]) {
				Data.keys[VK_UP] = FALSE;

				if (Data.StartScreen == TRUE) {
					int a, b;
					a = Data.getCounterButtons();
					b = Data.getNbuttons();
					if (a == 0)
						Data.SetCounterButtons(b);
					else
					{
						a--;
						Data.SetCounterButtons(a);
					}
				}
				else {

					Vertex temp = Data.GetPlayerPosition();
					if (temp.y <= (Data.GetwallAltezza() - (Data.GetPlayerBodyHeight() / 2 + Data.GetPlayerHeadSize() * 2)))
					{
						temp.y += CS.movement;
						Data.SetPlayerPosition(temp);
					}
					else {
						//sonoro limite
						if (limit->isPlaying()) limit->reset();
						else limit->play();
					}
				}
			}
			if (Data.keys[VK_DOWN]) {
				Data.keys[VK_DOWN] = FALSE;
				
				if (Data.StartScreen ==TRUE) {
					int a, b;
					a = Data.getCounterButtons();
					b = Data.getNbuttons();
					
						if (a==b)
						{

							Data.SetCounterButtons(0);
						}
						else
						{
							a++;
							Data.SetCounterButtons(a);
						}
				}
				else {
					Vertex temp = Data.GetPlayerPosition();

					if (temp.y >= (+(Data.GetPlayerBodyHeight() / 2 + Data.GetPlayerHeadSize() * 2)))
					{
						temp.y -= CS.movement;
						Data.SetPlayerPosition(temp);
					}
					else {
						//sonoro limite
						if (limit->isPlaying()) limit->reset();
						else limit->play();
					}
				}
			}
			if (Data.keys[VK_LEFT]) {
				Data.keys[VK_LEFT] = FALSE;
				Vertex temp = Data.GetPlayerPosition();
				if (temp.x >= (-Data.GetwallLargh() / 2 + (Data.GetPlayerBodyHeight() / 2 + Data.GetPlayerHeadSize() * 2)))
				{
					temp.x -= CS.movement;
					Data.SetPlayerPosition(temp);
				}
				else {
					//sonoro limite
					if (limit->isPlaying()) limit->reset();
					else limit->play();
				}
			}
			if (Data.keys[VK_RIGHT]) {
				Data.keys[VK_RIGHT] = FALSE;
				Vertex temp = Data.GetPlayerPosition();
				if (temp.x <= (Data.GetwallLargh() / 2 - (Data.GetPlayerBodyHeight() / 2 + Data.GetPlayerHeadSize() * 2)))
				{
					temp.x += CS.movement;
					Data.SetPlayerPosition(temp);
				}
				else {

					//sonoro limite
					if (limit->isPlaying()) limit->reset();
					else limit->play();
				}
			}
			if (Data.keys[VK_RETURN]) {
				if (Data.StartScreen == TRUE) {
					int a;
					a = Data.getCounterButtons();
					
					Data.StartScreen = FALSE;
					//a seconda di cosa si preme
					Data.setScene(a);
				
				}
			}
			
			if (Data.keys[VK_ESCAPE]) {
				//torna alla schermata iniziale
				
				Data.StartScreen = TRUE;
				
			}
			

			if (Data.keys['A']) {
				Data.keys['A'] = FALSE;
				float temp = Data.GetPlayerRotation();
				temp += CS.rotation;
				Data.SetPlayerRotation(temp);
			}
			if (Data.keys['S']) {
				Data.keys['S'] = FALSE;
				float temp = Data.GetPlayerRotation();
				temp -= CS.rotation;
				Data.SetPlayerRotation(temp);
			}

			if (Data.keys['M']) {
				Data.keys['M'] = FALSE;
				if (stream->isPlaying()) {
					stream->stop();				
				}
				else {
					stream->play();
					}
				
			}


		}
	}
	//KillGLWindow();
	return msg.wParam;
}


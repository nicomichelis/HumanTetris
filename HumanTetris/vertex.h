#pragma once
#include <Windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <math.h>

class Vertex {
public:
	float x, y, z;				// 3D coordinates
	float Nx, Ny, Nz;			// Normal to the surface (if present)
	float r, g, b;				// Color (0.0 - 1.0)

	Vertex() {
		x = y = z = Nx = Ny = Nz = r = g = b = 0.0;
	}
	Vertex(float x, float y, float z) : x(x), y(y), z(z) {
		Nx = Ny = Nz = r = g = b = 0.0;
	}
	Vertex(float x, float y, float z, float Nx, float Ny, float Nz) : x(x), y(y), z(z), Nx(Nx), Ny(Ny), Nz(Nz) {
		r = g = b = 0.0;
	}

	void SetP(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	void SetN(float x, float y, float z) {
		Nx = x;
		Ny = y;
		Nz = z;
	}
	inline void SetColor(float rr, float gg, float bb) {
		r = rr;
		g = gg;
		b = bb;
	}
	inline void Draw() {
		glBegin(GL_POINTS);
		glColor3f(r, g, b);
		glVertex3f(x, y, z);
		glEnd();
	}
};
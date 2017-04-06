#pragma once
#include <Windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

class Vertex {
public:
	float x, y, z;				// 3D coordinates
	float Nx, Ny, Nz;			// Normal to the surface (if present)
	float u, v;					// Texture coordinates
	float r, g, b;				// Color (0.0 - 1.0)

	Vertex() {}
	Vertex(float x, float y, float z) : x(x), y(y), z(z) {}
	Vertex(float x, float y, float z, float u, float v) : x(x), y(y), z(z), u(u), v(v) {}
	inline void SetP(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	inline void SetN(float x, float y, float z) {
		Nx = x;
		Ny = y;
		Nz = z;
	}
	inline void SetTexture(float uu, float vv) {
		u = uu;
		v = vv;
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

class Triangle {
public:
	Vertex a, b, c;

	Triangle() {}
	Triangle(Vertex x, Vertex y, Vertex z) {
		a = x;
		b = y;
		c = z;
	}
	inline void SetP(Vertex a, Vertex b, Vertex c) {
		this->a = a;
		this->b = b;
		this->c = c;
	}
	void Draw()
	{
		glBegin(GL_TRIANGLES);
		glColor3f(a.r, a.g, a.b);
		glVertex3f(a.x, a.y, a.z);
		glColor3f(b.r, b.g, b.b);
		glVertex3f(b.x, b.y, b.z);
		glColor3f(c.r, c.g, c.b);
		glVertex3f(c.x, c.y, c.z);
		glEnd();
	}
};

class Rect {
public:
	Vertex a, b, c, d;

	Rect() { }
	Rect(Vertex x, Vertex y, Vertex z, Vertex w) {
		a = x;
		b = y;
		c = z;
		d = w;
	}
	inline void SetP(Vertex a, Vertex b, Vertex c, Vertex d) {
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;
	}
	void Draw()
	{
		glBegin(GL_QUADS);
		glColor3f(a.r, a.g, a.b);
		glVertex3f(a.x, a.y, a.z);
		glColor3f(b.r, b.g, b.b);
		glVertex3f(b.x, b.y, b.z);
		glColor3f(c.r, c.g, c.b);
		glVertex3f(c.x, c.y, c.z);
		glColor3f(d.r, d.g, d.b);
		glVertex3f(d.x, d.y, d.z);
		glEnd();
	}
};
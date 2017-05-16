#pragma once
#include <Windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <math.h>

#define PI 3.14159265359

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
	void TraslateZ(float value) {
		// Traslate the triangle along the z axis of selected value
		a.z += value;
		b.z += value;
		c.z += value;
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
		glBegin(GL_TRIANGLES);
		glColor3f(a.r, a.g, a.b);
		glVertex3f(a.x, a.y, a.z);
		glColor3f(b.r, b.g, b.b);
		glVertex3f(b.x, b.y, b.z);
		glColor3f(c.r, c.g, c.b);
		glVertex3f(c.x, c.y, c.z);
		glColor3f(c.r, c.g, c.b);
		glVertex3f(c.x, c.y, c.z);
		glColor3f(d.r, d.g, d.b);
		glVertex3f(d.x, d.y, d.z);
		glColor3f(a.r, a.g, a.b);
		glVertex3f(a.x, a.y, a.z);
		glEnd();
	}
};

class Cylinder {
public:
	Vertex center;
	float radius;
	float width;
	Cylinder() {
	}
	Cylinder(Vertex c, float r, float w) {
		this->center = c;
		this->radius = r;
		this->width = w;
	}
	void Draw() {
		int res = 1000;
		for (int i = 0; i < res; i++) {
			// Current point
			float theta = 2.0f * PI * float(i) / float(res);
			float x = radius * cos(theta) + center.x;
			float y = radius * sin(theta) + center.y;
			Vertex current = center;
			current.SetP(x, y, center.z);
			// Next point
			theta = 2.0f * PI * float(i+1) / float(res);
			x = radius * cos(theta) + center.x;
			y = radius * sin(theta) + center.y;
			Vertex next = center;
			next.SetP(x, y, center.z);
			// Front
			Triangle front(center, current, next);
			front.TraslateZ(width / 2);
			front.Draw();
			// Back
			Triangle back(current, center, next);
			back.TraslateZ(-width / 2);
			back.Draw();
			// Sides
			Vertex nextfront = next;
			nextfront.z += width / 2;
			Vertex currentfront = current;
			currentfront.z += width / 2;
			Vertex nextback = next;
			nextback.z -= width / 2;
			Vertex currentback = current;
			currentback.z -= width / 2;
			Rect side(currentfront, currentback, nextback, nextfront);
			side.Draw();
		}
	}
};
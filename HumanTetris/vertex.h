#pragma once
#include <Windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <math.h>

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
		/*glBegin(GL_QUADS);
		glColor3f(a.r, a.g, a.b);
		glVertex3f(a.x, a.y, a.z);
		glColor3f(b.r, b.g, b.b);
		glVertex3f(b.x, b.y, b.z);
		glColor3f(c.r, c.g, c.b);
		glVertex3f(c.x, c.y, c.z);
		glColor3f(d.r, d.g, d.b);
		glVertex3f(d.x, d.y, d.z);
		glEnd();
		*/

		glBegin(GL_TRIANGLES);
		glColor3f(a.r, a.g, a.b);
		glVertex3f(a.x, a.y, a.z);
		glColor3f(b.r, b.g, b.b);
		glVertex3f(b.x, b.y, b.z);
		glColor3f(c.r, c.g, c.b);
		glVertex3f(c.x, c.y, c.z);
		glEnd();

		glBegin(GL_TRIANGLES);
		glColor3f(c.r, c.g, c.b);
		glVertex3f(c.x, c.y, c.z);
		glColor3f(d.r, d.g, d.b);
		glVertex3f(d.x, d.y, d.z);
		glColor3f(a.r, a.g, a.b);
		glVertex3f(a.x, a.y, a.z);
		glEnd();
	}
};
/*
class Circle {
public:
	Vertex center;
	float radius;
	Circle() {
		Vertex temp(0, 0, 0);
		temp.SetColor(0.0, 0.0, 0.0);
		center = temp;
		radius = 0.0;
	}
	Circle(Vertex c) {
		center = c;
		radius = 0.0;
	}
	Circle(Vertex c, float r) {
		center = c;
		radius = r;
	}
	void Draw()
	{
		glBegin(GL_TRIANGLES);
		int res = 1000;
		Vertex last;
		for (int i = 0; i < res + 1; i++) {
			float theta = 2.0f * 3.1415926f * float(i) / float(res);	//get the current angle 
			float x = radius * cosf(theta) + center.x;	//calculate the x component 
			float y = radius * sinf(theta) + center.y;	//calculate the y component 
			Vertex current(x, y, center.z);
			current.SetColor(center.r, center.b, center.g);
			if (i != 0) {
				
				Triangle tri(center, last, current);
				tri.Draw();
			}
			last.SetP(x, y, center.z);
			last.SetColor(center.r, center.b, center.g);
		}
		glEnd();
	}
};
*/

class Cylinder {
public:
	Vertex center;
	float radius;
	float width;
	Cylinder() {
	}
	Cylinder(Vertex c, float r, float w) {
		center = c;
		radius = r;
		width = w;
	}
	void Draw() {
		glBegin(GL_TRIANGLES);
		int res = 1000;
		Vertex last;
		
		for (int i = 0; i < res ; i++) {
			float theta = 2.0f * 3.1415926f * float(i) / float(res);	//get the current angle 
			float x = radius * cosf(theta) + center.x;	//calculate the x component 
			float y = radius * sinf(theta) + center.y;	//calculate the y component 
			Vertex current(x, y, center.z + width/2);
			current.SetColor(center.r, center.b, center.g);
			if (i != 0) {
				
				Triangle tri(center, last, current);
				tri.Draw();
				Vertex a = center;
				a.z -= width;
				Vertex b = last;
				b.z -= width;
				Vertex c = current;
				c.z -= width;
				tri.SetP(b, a, c);
				tri.Draw();
				tri.SetP(last, b, current);
				tri.Draw();
				tri.SetP(b, c, current);
				tri.Draw();
				//current.Draw();
				//last.Draw();
				//center.Draw();
				//a.Draw();
				//b.Draw();
				//c.Draw();
			}
			last = current;
		}
		glEnd();
	}
};
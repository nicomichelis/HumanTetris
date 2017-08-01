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

	void DrawTextures() {
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex3f(a.x, a.y, a.z);
		glTexCoord2f(1, 0);
		glVertex3f(b.x, b.y, b.z);
		glTexCoord2f(1, 1);
		glVertex3f(c.x, c.y, c.z);
		glTexCoord2f(0, 1);
		glVertex3f(d.x, d.y, d.z);

		
		glEnd();
		glDisable(GL_TEXTURE_2D);
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
		int res = 25;
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
			glBegin(GL_TRIANGLES);
			glColor3f(center.r, center.g, center.b);
			glVertex3f(center.x, center.y, center.z+width/2);
			glColor3f(current.r, current.g, current.b);
			glVertex3f(current.x, current.y, current.z + width/2);
			glColor3f(next.r, next.g, next.b);
			glVertex3f(next.x, next.y, next.z+width/2);
			
			glColor3f(current.r, current.g, current.b);
			glVertex3f(current.x, current.y, current.z - width / 2);
			glColor3f(center.r, center.g, center.b);
			glVertex3f(center.x, center.y, center.z - width / 2);
			glColor3f(next.r, next.g, next.b);
			glVertex3f(next.x, next.y, next.z - width / 2);
			
			glColor3f(current.r, current.g, current.b);
			glVertex3f(current.x, current.y, current.z + width / 2);
			glColor3f(current.r, current.g, current.b);
			glVertex3f(current.x, current.y, current.z - width / 2);
			glColor3f(next.r, next.g, next.b);
			glVertex3f(next.x, next.y, next.z - width / 2);
			
			glColor3f(current.r, current.g, current.b);
			glVertex3f(current.x, current.y, current.z + width / 2);
			glColor3f(next.r, next.g, next.b);
			glVertex3f(next.x, next.y, next.z - width / 2);
			glColor3f(next.r, next.g, next.b);
			glVertex3f(next.x, next.y, next.z + width / 2);
			glEnd();
		}
	}
	void DrawTexture() {
		int res = 25;
		for (int i = 0; i < res; i++) {
			// Current point
			float theta = 2.0f * PI * float(i) / float(res);
			float x = radius * cos(theta) + center.x;
			float y = radius * sin(theta) + center.y;
			Vertex current = center;
			current.SetP(x, y, center.z);
			// Next point
			theta = 2.0f * PI * float(i + 1) / float(res);
			x = radius * cos(theta) + center.x;
			y = radius * sin(theta) + center.y;
			Vertex next = center;
			next.SetP(x, y, center.z);
			glEnable(GL_TEXTURE_2D);
			glBegin(GL_TRIANGLES);
			
			glTexCoord2f(0.5, 0.5);
			glVertex3f(center.x, center.y, center.z + width / 2);
			glTexCoord2f(((current.x-center.x)/radius)/2+0.5, ((current.y-center.y)/radius)/2+0.5);
			glVertex3f(current.x, current.y, current.z + width / 2);
			glTexCoord2f(((next.x - center.x) / radius)/2+0.5, ((next.y - center.y) / radius)/2+0.5);
			glVertex3f(next.x, next.y, next.z + width / 2);

			glTexCoord2f(((current.x - center.x) / radius)/2+0.5, ((current.y - center.y) / radius)/2+0.5);
			glVertex3f(current.x, current.y, current.z - width / 2);
			glTexCoord2f(0.5, 0.5);
			glVertex3f(center.x, center.y, center.z - width / 2);
			glTexCoord2f(((next.x - center.x) / radius)/2+0.5, ((next.y - center.y) / radius)/2+0.5);
			glVertex3f(next.x, next.y, next.z - width / 2);
			
			glEnd();
			glDisable(GL_TEXTURE_2D);
			glBegin(GL_TRIANGLES);
			// I lati non hanno texture
			glColor3f(current.r, current.g, current.b);
			glVertex3f(current.x, current.y, current.z + width / 2);
			glColor3f(current.r, current.g, current.b);
			glVertex3f(current.x, current.y, current.z - width / 2);
			glColor3f(next.r, next.g, next.b);
			glVertex3f(next.x, next.y, next.z - width / 2);

			glColor3f(current.r, current.g, current.b);
			glVertex3f(current.x, current.y, current.z + width / 2);
			glColor3f(next.r, next.g, next.b);
			glVertex3f(next.x, next.y, next.z - width / 2);
			glColor3f(next.r, next.g, next.b);
			glVertex3f(next.x, next.y, next.z + width / 2);
			glEnd();
			
		}
	}
	
};
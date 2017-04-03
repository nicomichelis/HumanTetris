#pragma once

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
};
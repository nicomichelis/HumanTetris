#include "model.h"
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h> 

#pragma comment( lib, "opengl32.lib" )			
#pragma comment( lib, "glu32.lib" )					
#pragma comment( lib, "winmm.lib" )					

void MyModel::DrawTriangle(Vertex a, Vertex b, Vertex c)
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

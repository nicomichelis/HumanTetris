#include "model.h"
#include "vertex.h"
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h> 

#pragma comment( lib, "opengl32.lib" )			
#pragma comment( lib, "glu32.lib" )					
#pragma comment( lib, "winmm.lib" )					


void MyModel::DrawFloor(){
	floorFront.Draw();
	floorBack.Draw();
	floorTop.Draw();
	floorBottom.Draw();
	floorSideA.Draw();
	floorSideB.Draw();
}

void MyModel::DrawWall(){
	wallFront.Draw();
	wallBack.Draw();
	wallTop.Draw();
	wallSideA.Draw();
	wallSideB.Draw();
	wallBottom.Draw();
}

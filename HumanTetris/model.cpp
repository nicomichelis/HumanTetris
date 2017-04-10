#include "model.h"
#include "vertex.h"
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h> 

#pragma comment( lib, "opengl32.lib" )			
#pragma comment( lib, "glu32.lib" )					
#pragma comment( lib, "winmm.lib" )					

MyModel::MyModel() {
	// Init Floor
	floorLargh = 5.0;
	floorProf = 20.0;
	floorAltezza = 1.0;

	// Wall
	wallLargh = 5.0;
	wallProf = 1.0;
	wallAltezza = 7.0;
	wallPosition = -10.0;
}

void MyModel::DrawFloor(){
	fa.SetP(-(floorLargh / 2), 0.0, (floorProf / 2));
	fa.SetColor(0.0, 0.0, 1.0);
	fb.SetP((floorLargh / 2), 0.0, (floorProf / 2));
	fb.SetColor(0.0, 0.0, 1.0);
	fc.SetP((floorLargh / 2), 0.0, -(floorProf / 2));
	fc.SetColor(0.0, 0.0, 1.0);
	fd.SetP(-(floorLargh / 2), 0.0, -(floorProf / 2));
	fd.SetColor(0.0, 0.0, 1.0);
	fe.SetP(-(floorLargh / 2), -floorAltezza, (floorProf / 2));
	fe.SetColor(0.0, 0.0, 1.0);
	ff.SetP((floorLargh / 2), -floorAltezza, (floorProf / 2));
	ff.SetColor(0.0, 0.0, 1.0);
	fg.SetP((floorLargh / 2), -floorAltezza, -(floorProf / 2));
	fg.SetColor(0.0, 0.0, 1.0);
	fh.SetP(-(floorLargh / 2), -floorAltezza, -(floorProf / 2));
	fh.SetColor(0.0, 0.0, 1.0);

	// Drawing


}

void MyModel::DrawWall(){
	
	wallPositionPoint.SetP(0.0, 0.0, wallPosition);
	wallPositionPoint.SetColor(1.0, 0, 0);

	wa.SetP(-(wallLargh / 2), 0.0, wallPosition + wallProf);
	wa.SetColor(0.0, 1.0, 0.0);
	wb.SetP(+(wallLargh / 2), 0.0, wallPosition + wallProf);
	wb.SetColor(0.0, 1.0, 0.0);
	wc.SetP(+(wallLargh / 2), wallAltezza, wallPosition + wallProf);
	wc.SetColor(0.0, 1.0, 0.0);
	wd.SetP(-(wallLargh / 2), wallAltezza, wallPosition + wallProf);
	wd.SetColor(0.0, 1.0, 0.0);
	we.SetP(-(wallLargh / 2), 0.0, wallPosition);
	we.SetColor(0.0, 1.0, 0.0);
	wf.SetP(+(wallLargh / 2), 0.0, wallPosition);
	wf.SetColor(0.0, 1.0, 0.0);
	wg.SetP(+(wallLargh / 2), wallAltezza, wallPosition);
	wg.SetColor(0.0, 1.0, 0.0);
	wh.SetP(-(wallLargh / 2), wallAltezza, wallPosition);
	wh.SetColor(0.0, 1.0, 0.0);
}


void MyModel::SetWallPosition(float x){
	this->wallPosition = x;
}

float MyModel::GetWallPosition(){
	return wallPosition;
}

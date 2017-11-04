#include "stdafx.h"
#include "CSHAPE.h"



/*	
	Class Cline Method
*/
void CLine::Draw(HDC hdc) {
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}

CShape*  CLine::Create() { return new CLine; }

void  CLine::SetData(int a, int b, int c, int d) {
	x1 = a;
	y1 = b;
	x2 = c;
	y2 = d;
}



/*
	Class CRectangle Method
*/
void CRectangle::Draw(HDC hdc) {
	Rectangle(hdc, x1, y1, x2, y2);
}

CShape* CRectangle::Create() { return new CRectangle; }

void CRectangle::SetData(int a, int b, int c, int d) {
	x1 = a;
	y1 = b;
	x2 = c;
	y2 = d;
}

/*
	Class CEclipse Method
*/
void  CEclipse::Draw(HDC hdc) {
	Ellipse(hdc, x1, y1, x2, y2);
}

CShape* CEclipse::Create() { return new CEclipse; }

void CEclipse::SetData(int a, int b, int c, int d) {
	x1 = a;
	y1 = b;
	x2 = c;
	y2 = d;
}


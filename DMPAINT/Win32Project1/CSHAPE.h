#pragma once

class CShape {
public:
	virtual void Draw(HDC hdc) = 0;
	virtual CShape* Create() = 0;
	virtual void SetData(int a, int b, int c, int d) = 0;
};

class CLine : public CShape {
public:
	int x1;
	int y1;
	int x2;
	int y2;
public:
	void Draw(HDC hdc);
	CShape* Create();

	void SetData(int a, int b, int c, int d);
};

class CRectangle : public CShape {
public:
	int x1;
	int y1;
	int x2;
	int y2;
public:
	void Draw(HDC hdc);

	CShape* Create();

	void SetData(int a, int b, int c, int d);
};

class CEclipse : public CShape {
public:
	int x1;
	int y1;
	int x2;
	int y2;
public:
	void Draw(HDC hdc);

	CShape* Create();

	void SetData(int a, int b, int c, int d);
};


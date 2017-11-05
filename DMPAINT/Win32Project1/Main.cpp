// Win32Project1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Main.h"
#include <iostream>
#include <vector>
using namespace std;
#include <windowsX.h>
#include "CSHAPE.h"
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

void CheckItemMenu(HMENU hMenu, int idCheck);

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}


//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WIN32PROJECT1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = NULL;

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, L"This Paint", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

enum Shape
{
	LINE,
	RECTANGLE,
	ECLIPSE
};

vector<CShape*> shapes;


bool isDrawing = FALSE;
int currentX;
int currentY;
int lastX;
int lastY;
int key = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int x;
	int y;
	HMENU hMenu = GetMenu(hWnd);

	switch (message)
	{
	case WM_CREATE:
		CheckMenuItem(hMenu, ID_DRAW_LINE, MF_CHECKED);
	break;
	case WM_LBUTTONDOWN:
		x = GET_X_LPARAM(lParam);
		y = GET_Y_LPARAM(lParam);

		if (!isDrawing)
		{
			isDrawing = TRUE;
			currentX = x;
			currentY = y;
		}

		break;
	case WM_MOUSEMOVE:

		x = GET_X_LPARAM(lParam);
		y = GET_Y_LPARAM(lParam);

		if (isDrawing) {
			lastX = x;
			lastY = y;
			WCHAR buffer[200];
			wsprintf(buffer, L"%d, %dpx", x, y);
			SetWindowText(hWnd, buffer);
			
			InvalidateRect(hWnd, NULL, TRUE);

		}

		break;

	case WM_LBUTTONUP:

		x = GET_X_LPARAM(lParam);
		y = GET_Y_LPARAM(lParam);
		if (key == LINE) {
			CLine* line = new CLine;
			line->SetData(currentX, currentY, x, y);
			shapes.push_back(line);
		}
		if (key == RECTANGLE) {
			CRectangle* rec = new CRectangle;

			rec->SetData(currentX, currentY, lastX, lastY);
			shapes.push_back(rec);
		}
		if (key == ECLIPSE) {
			CEclipse* Ell = new CEclipse;
			Ell->SetData(currentX, currentY, lastX, lastY);
			shapes.push_back(Ell);
		}

		isDrawing = FALSE;
		InvalidateRect(hWnd, NULL, FALSE);

		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{

		case ID_DRAW_LINE:
		{
			CheckItemMenu(hMenu, ID_DRAW_LINE);
			key = LINE;
			break;
		}
		case ID_DRAW_RECTANGLE:
		{
			CheckItemMenu(hMenu, ID_DRAW_RECTANGLE);

			key = RECTANGLE;
			break;
		}
		case ID_DRAW_ELLIPSE:
		{
			CheckItemMenu(hMenu, ID_DRAW_ELLIPSE);

			key = ECLIPSE;
			break;
		}
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);

		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		RECT rw;
		GetClientRect(hWnd, &rw);
		int win_width = rw.right - rw.left;
		int win_height = -rw.top + rw.bottom;

		CEclipse* Eclipse;

		CLine* line;
		CRectangle* Rectangle;
		// Create an off-screen DC for double-buffering
		HDC hdcMem = CreateCompatibleDC(hdc);
		HBITMAP hbmMem = CreateCompatibleBitmap(hdc, win_width, win_height);

		HANDLE hOld = SelectObject(hdcMem, hbmMem);
		FillRect(hdcMem, &rw, NULL);

		for (int i = 0; i < shapes.size(); i++)
		{
			shapes[i]->Draw(hdc);
		}

		if (isDrawing)
		{
			if (key != 0 && GetAsyncKeyState(VK_SHIFT) < 0)
			{
				int dx = (lastX - currentX);
				int dy = (lastY - currentY);
				if (dx < dy)
					lastY = dx + currentY;
				else
					lastX = dy + currentX;
			}
			int index = shapes.size();
			switch (key)
			{
			case 0:
				
				line = new CLine();
				line->SetData(currentX, currentY, lastX, lastY);
				line->Draw(hdcMem);
			//	MoveToEx(hdcMem, currentX, currentY, NULL);
			//	LineTo(hdcMem, lastX, lastY);
				break;
			case 1:
				Rectangle = new CRectangle();
				Rectangle->SetData(currentX, currentY, lastX, lastY);
				Rectangle->Draw(hdcMem);
			//	Rectangle(hdcMem, currentX, currentY, lastX, lastY);
				break;
			case 2:
				Eclipse = new CEclipse();
				Eclipse->SetData(currentX, currentY, lastX, lastY);
				Eclipse->Draw(hdcMem);
			//	Ellipse(hdcMem, currentX, currentY, lastX, lastY);
				break;
			}
			ReleaseDC(hWnd, hdcMem);

			BitBlt(hdc, 0, 0, win_width, win_height, hdcMem, 0, 0, SRCAND);
			// Free-up the off-screen DC
			SelectObject(hdcMem, hOld);
			DeleteObject(hbmMem);
			DeleteDC(hdcMem);

		}
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
// check item of menu
void CheckItemMenu(HMENU hMenu, int idCheck)
{
	CheckMenuItem(hMenu, ID_DRAW_ELLIPSE, MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_DRAW_LINE, MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_DRAW_RECTANGLE, MF_UNCHECKED);
	CheckMenuItem(hMenu, idCheck, MF_CHECKED);
}
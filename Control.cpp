#include "stdafx.h"
#include "Control.h"


Control::Control(HWND hWnd, HINSTANCE hInst) {
	hMainWindow = hWnd;
	hInstance = hInst;

	POINT p;
	p.x = 0;
	p.y = 20;
	listLeft = new ListViewControl(hMainWindow, hInstance, p, 500, 600, 1);

	LPWSTR items[] = { L"Text1", L"1Text", L"T1ext" };
	listLeft->AddItem(items);
	items[0] = L"Text2";
	listLeft->AddItem(items);
	listLeft->Refresh();

	p.x = 501;
	p.y = 20;
	listRight = new ListViewControl(hMainWindow, hInstance, p, 500, 600, 2);
	items[0] = L"Text1";
	listRight->AddItem(items);
	items[0] = L"Text2";
	listRight->AddItem(items);
	listRight->Refresh();

}


Control::~Control() {

}

#pragma once
#include "ListViewControl.h"

class Control
{
public:
	Control(HWND hWnd, HINSTANCE hInst);
	~Control();

private:
	HWND hMainWindow;
	HINSTANCE hInstance;
	ListViewControl *listLeft;
	ListViewControl *listRight;
};


#include "stdafx.h"
#include "LabelControl.h"

LabelControl::LabelControl(HWND hWnd, HINSTANCE hInst,POINT p, int w, int h, int id) {
	hLabel = CreateWindow(L"static", L"path",
						  WS_CHILD | WS_VISIBLE | WS_TABSTOP,
						  p.x, p.y, w, h,
						  hWnd, (HMENU)id, hInst, NULL);
}

LabelControl::~LabelControl() {
	DestroyWindow(hLabel);
}

void LabelControl::SetText(LPWSTR text) {
	SetWindowText(hLabel, text);
}

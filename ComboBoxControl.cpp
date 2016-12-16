#include "stdafx.h"
#include "ComboBoxControl.h"

ComboBoxControl::ComboBoxControl(HWND hWnd, HINSTANCE hInst, POINT p, int w, int h, int id) {
	hComboBox = CreateWindow(L"ComboBox", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL
							 | CBS_DROPDOWN | CBS_SORT | CBS_DROPDOWNLIST, p.x, p.y, w, h, hWnd,
							 (HMENU)id, hInst, NULL);
}

ComboBoxControl::~ComboBoxControl() {
	DestroyWindow(hComboBox);
}

void ComboBoxControl::AddItem(LPWSTR item) {
	SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)item);
}

void ComboBoxControl::SelectItem(int num) {
	SendMessage(hComboBox, CB_SETCURSEL, num, 0);
}

LPWSTR ComboBoxControl::GetSelected()
{
	LPWSTR result = new TCHAR[MAX_STR_BLOCKREASON];

	int sel = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);
	SendMessage(hComboBox, CB_GETLBTEXT, sel, (LPARAM)result);

	return result;
}

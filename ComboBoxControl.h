#pragma once
class ComboBoxControl
{
public:
	ComboBoxControl(HWND hWnd, HINSTANCE hInst, POINT p, int w, int h, int id);
	~ComboBoxControl();
	void AddItem(LPWSTR item);
	void SelectItem(int num);
	LPWSTR GetSelected();

private:
	HWND hComboBox;
};


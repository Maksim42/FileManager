#pragma once
class LabelControl
{
public:
	LabelControl(HWND hWnd, HINSTANCE hInst, POINT p, int w, int h, int id);
	~LabelControl();
	void SetText(LPWSTR text);

private:
	HWND hLabel;
};


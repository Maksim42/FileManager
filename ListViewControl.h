#pragma once
#include <vector>

class ListViewControl
{
public:
	ListViewControl(HWND hWnd, HINSTANCE hInst, POINT top, int w, int h, int id);
	~ListViewControl();
	void AddItem(LPWSTR items[]);
	void Refresh();
	int GetSelectedItems(LPWSTR items[], int length);
	void Clear();

private:
	HWND hList;
	int itemCount;
	int columnCount;
	std::vector<std::vector<LPWSTR> > listItems;

	void InitColumn(LPWSTR items[], int length);
	void ClearData();
	void ClearView();
};


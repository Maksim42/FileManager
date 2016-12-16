#pragma once
#include <vector>

class ListViewControl
{
public:
	HWND hList;

	ListViewControl(HWND hWnd, HINSTANCE hInst, POINT top, int w, int h, int id);
	~ListViewControl();
	void AddItem(LPWSTR items[]);
	void Refresh();
	int GetSelectedItems(LPWSTR items[], int length);
	void Clear();
	void StartEdit();
	void StopEdit();

private:
	int itemCount;
	int columnCount;
	std::vector<std::vector<LPWSTR> > listItems;
	std::vector<int> iconNumber;
	HWND hEdit;

	void InitColumn(LPWSTR items[], int length);
	void ClearData();
	void ClearView();
};


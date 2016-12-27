#pragma once
#include <vector>

class ListViewControl
{
public:
	HWND hList;
	LPWSTR path;
	int columnCount;
	int sortingPatern;

	ListViewControl(HWND hWnd, HINSTANCE hInst, POINT top, int w, int h, int id);
	~ListViewControl();
	void AddItem(LPWSTR items[]);
	void Refresh();
	std::vector<LPWSTR> GetSelectedItems();
	void Clear();
	void GetItemText(int itemNum, LPWSTR text);
	void Sorting(int columnIndex);
	void StartEdit();
	void StopEdit();

private:
	int itemCount;
	std::vector< std::vector<LPWSTR> > listItems;
	std::vector<int> iconNumber;
	//HWND hEdit;

	void InitColumn(LPWSTR items[], int length);
	void ClearData();
	void ClearView();
	void ColumnSorting();
	void Swap(int from, int to);
};


#include "stdafx.h"
#include "ListViewControl.h"


ListViewControl::ListViewControl(HWND hWnd, HINSTANCE hInst, POINT position, int w, int h, int id) {
	itemCount = 0;

	hList = CreateWindow(WC_LISTVIEW, NULL,
		LVS_REPORT | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | LVS_AUTOARRANGE,
		(position.x), (position.y), w, h, hWnd,
		(HMENU) id, hInst, NULL);

	ListView_SetExtendedListViewStyle(hList, LVS_EX_FULLROWSELECT);

	LPWSTR columns[] = { L"Name", L"Type", L"?" };
	InitColumn(columns, 3);
}


ListViewControl::~ListViewControl() {
	DestroyWindow(hList);
}

void ListViewControl::InitColumn(LPWSTR items[], int count) {
	columnCount = count;

	for (int i = 0; i < count; i++) {
		listItems.push_back(std::vector<LPWSTR>());
	}
	

	LVCOLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.cx = 75;
	lvc.fmt = LVCFMT_LEFT;

	for (int i = 0; i < count; i++) {
		lvc.iSubItem = i;

		int length = wcslen(items[i]) + 1;
		lvc.pszText = new TCHAR[length];
		wcscpy_s(lvc.pszText, length, items[i]);

		ListView_InsertColumn(hList, i + 1, &lvc);
	}
}

void ListViewControl::AddItem(LPWSTR items[]) {
	int length = wcslen(items[0]) + 1;
	listItems[0].push_back(new TCHAR[length]);
	wcscpy_s(listItems[0][itemCount], length, items[0]);
	
	for (int i = 1; i < columnCount; i++) {
		int length = wcslen(items[i]) + 1;
		listItems[i].push_back (new TCHAR[length]);
		wcscpy_s(listItems[i][itemCount], length, items[i]);
	}

	itemCount++;
}

void ListViewControl::Refresh() {
	this->ClearView();

	LVITEM lvI;
	lvI.mask = LVIF_TEXT;//| LVIF_IMAGE | LVIF_STATE;
	lvI.stateMask = 0;
	lvI.state = 0;

	for (int j = 0; j < itemCount; j++) {
		lvI.iItem = j;

		lvI.iSubItem = 0;
		//lvI.iImage = 0;
		lvI.pszText = listItems[0][j];
		ListView_InsertItem(hList, &lvI);

		for (int i = 1; i < columnCount; i++) {
			lvI.iSubItem = i;
			//lvI.iImage = 0;
			lvI.pszText = listItems[i][j];

			ListView_SetItem(hList, &lvI);
		}
	}
}

void ListViewControl::ClearData() {
	for (int i = 0; i < columnCount; i++) {
		for (int j = 0; j < itemCount; j++) {
			delete[] listItems[i][j];
		}

		listItems[i].clear();
	}

	itemCount = 0;
}

void ListViewControl::ClearView() {
	for (int i = 0; i <= itemCount; i++) {
		ListView_DeleteItem(hList, 0);
	}
}

int ListViewControl::GetSelectedItems(LPWSTR items[], int length) {
	int resultLength = 0;

	int position = ListView_GetNextItem(hList, -1, LVNI_SELECTED);
	while ((position != -1) && (resultLength < length)) {
		LPWSTR bufer = new TCHAR[MAX_STR_BLOCKREASON];
		ListView_GetItemText(hList, position, 0, bufer, MAX_STR_BLOCKREASON);
		
		wcscpy_s(items[resultLength], MAX_STR_BLOCKREASON, bufer);
		delete[] bufer;
		resultLength++;

		position = ListView_GetNextItem(hList, position, LVNI_SELECTED);
	}

	return resultLength;
}

void ListViewControl::Clear() {
	this->ClearView();
	this->ClearData();
}

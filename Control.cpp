#include "stdafx.h"
#include "Control.h"


Control::Control(HWND hWnd, HINSTANCE hInst) {
	hMainWindow = hWnd;
	hInstance = hInst;
	focus = false;

	InitializeComponent();
}


Control::~Control() {
	delete listLeft;
	delete labelRight;
	delete labelLeft;
	//delete labelRight;
	delete comboLeft;
	delete comboRight;
}

void Control::InitializeComponent() {
	fileSystem = new FileSystem();

	int w = 441;
	int h = 515;
	POINT p;

	p.x = 0;
	p.y = 25;
	listLeft = new ListViewControl(hMainWindow, hInstance, p, w, h, LeftListView);
	p.x = w + 2;
	listRight = new ListViewControl(hMainWindow, hInstance, p, w, h, RightListViev);

	p.x = 3;
	p.y = 0;
	labelLeft = new LabelControl(hMainWindow, hInstance, p, w - 100, 20, LeftLabel);
	p.x = w + 5;
	labelRight = new LabelControl(hMainWindow, hInstance, p, w - 100, 20, RightLabel);
	
	p.x = w - 100 + 3;
	comboLeft = new ComboBoxControl(hMainWindow, hInstance, p, 100, 100, LeftComboBox);
	p.x = w + w - 100 + 3;
	comboRight = new ComboBoxControl(hMainWindow, hInstance, p, 100, 100, RightComboBox);

	DWORD drives = GetLogicalDrives();
	for (int i = 0; i<26; i++)
	{
		if (drives & (1 << i))
		{
			TCHAR driveName[] = { TEXT('A') + i, TEXT(':'), TEXT('\\'), TEXT('\0') };
			comboLeft->AddItem(driveName);
			comboRight->AddItem(driveName);
		}
	}
	comboLeft->SelectItem(0);
	comboRight->SelectItem(0);

	fileSystem->FindFile(listLeft, listLeft->path);
	fileSystem->FindFile(listRight, listRight->path);

	labelLeft->SetText(listLeft->path);
	labelRight->SetText(listRight->path);
}

void Control::BeginEditHandler(LPNMHDR lpnmhdr) {
	switch (lpnmhdr->idFrom)
	{
	case LeftListView:
		listLeft->StartEdit();
		break;
	case RightListViev:
		listRight->StartEdit();
		break;
	}
}

void Control::EndEditHandler(LPNMHDR lpnmhdr) {
	switch (lpnmhdr->idFrom)
	{
	case LeftListView:
		listLeft->StopEdit();
		break;
	case RightListViev:
		listRight->StopEdit();
		break;
	}
}

void Control::ListColumnClick(LPNMHDR lpnmhdr, LPNMLISTVIEW pnmLV) {
	switch (lpnmhdr->idFrom)
	{
	case LeftListView:
		listLeft->Sorting(pnmLV->iSubItem);
		listLeft->Refresh();
		break;
	case RightListViev:
		listRight->Sorting(pnmLV->iSubItem);
		listRight->Refresh();
		break;
	}
}

void Control::LeftComboBoxSelect() {
	LPWSTR newPath = comboLeft->GetSelected();

	LPWSTR oldPath = new TCHAR[MAX_PATH];
	wcscpy_s(oldPath, MAX_PATH, listLeft->path);

	bool success = fileSystem->FindFile(listLeft, newPath);
	if (success) {
		labelLeft->SetText(newPath);
	} else {
		MessageBox(0, L"Error", L"BreakComander", MB_OK | MB_ICONWARNING);
		fileSystem->FindFile(listLeft, oldPath);
	}

	delete[] oldPath;
}

void Control::RightComboBoxSelect() {
	LPWSTR newPath = comboRight->GetSelected();

	LPWSTR oldPath = new TCHAR[MAX_PATH];
	wcscpy_s(oldPath, MAX_PATH, listRight->path);

	bool success = fileSystem->FindFile(listRight, newPath);
	if (success) {
		labelRight->SetText(newPath);
	} else {
		MessageBox(0, L"Error", L"BreakComander", MB_OK | MB_ICONWARNING);
		fileSystem->FindFile(listRight, oldPath);
	}

	delete[] oldPath;
}

void Control::SetListFocus(LPNMHDR lpnmHdr) {
	switch (lpnmHdr->idFrom)
	{
	case LeftListView:
		focus = false;
		break;
	case RightListViev:
		focus = true;
		break;
	}
}

void Control::OpenItem(LPNMHDR lpnmHdr, LPNMLISTVIEW pnmLV) {
	ListViewControl *list;
	LabelControl *label;
	if (lpnmHdr->idFrom == LeftListView) {
		list = listLeft;
		label = labelLeft;
	} else {
		list = listRight;
		label = labelRight;
	}

	fileSystem->Open(list, pnmLV, label);
}

void Control::CopyFiles() {
	if (focus) {
		fileSystem->CopySelectedFiles(listRight, listLeft);
		fileSystem->FindFile(listLeft, listLeft->path);
	} else {
		fileSystem->CopySelectedFiles(listLeft, listRight);
		fileSystem->FindFile(listRight, listRight->path);
	}
}

void Control::DeleteFiles() {
	if (focus) {
		fileSystem->DeleteSelectedFiles(listRight);
		fileSystem->FindFile(listRight, listRight->path);
		fileSystem->FindFile(listLeft, listLeft->path);
	} else {
		fileSystem->DeleteSelectedFiles(listLeft);
		fileSystem->FindFile(listLeft, listLeft->path);
		fileSystem->FindFile(listRight, listRight->path);
	}
}



#include "stdafx.h"
#include "Control.h"


Control::Control(HWND hWnd, HINSTANCE hInst) {
	hMainWindow = hWnd;
	hInstance = hInst;
}


Control::~Control() {
	delete listLeft;
	delete labelRight;
	delete labelLeft;
	delete labelRight;
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

	labelLeft->SetText(fileSystem->path1);
	labelRight->SetText(fileSystem->path2);

	fileSystem->FindFile(listLeft, fileSystem->path1, 1);
	fileSystem->FindFile(listRight, fileSystem->path2, 2);
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

void Control::LeftComboBoxSelect(UINT message, WPARAM wParam) {
	LPWSTR newPath = comboLeft->GetSelected();
	labelLeft->SetText(newPath);

	LPWSTR oldPath = new TCHAR[MAX_PATH];
	wcscpy_s(oldPath, MAX_PATH, fileSystem->path1);

	bool success = fileSystem->FindFile(listLeft, newPath, 1);
	if (!success) {
		MessageBox(0, L"Error", L"BreakComander", MB_OK | MB_ICONWARNING);
		fileSystem->FindFile(listLeft, oldPath, 1);
		labelLeft->SetText(oldPath);
	}
	delete[] oldPath;
}

void Control::RightComboBoxSelect(UINT message, WPARAM wParam) {
	LPWSTR newPath = comboRight->GetSelected();
	labelRight->SetText(newPath);

	LPWSTR oldPath = new TCHAR[MAX_PATH];
	wcscpy_s(oldPath, MAX_PATH, fileSystem->path2);

	bool success = fileSystem->FindFile(listRight, newPath, 2);
	if (!success) {
		MessageBox(0, L"Error", L"BreakComander", MB_OK | MB_ICONWARNING);
		fileSystem->FindFile(listRight, oldPath, 2);
		labelRight->SetText(oldPath);
	}
	delete[] oldPath;

}



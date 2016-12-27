#pragma once
#include "ListViewControl.h"
#include "LabelControl.h"
#include "ComboBoxControl.h"
#include "FileSystem.h"

#define LeftListView 101
#define RightListViev 102
#define LeftLabel 201
#define RightLabel 202
#define LeftComboBox 301
#define RightComboBox 302

class Control
{
public:
	Control(HWND hWnd, HINSTANCE hInst);
	~Control();
	void InitializeComponent();
	void BeginEditHandler(LPNMHDR lpnmhdr);
	void EndEditHandler(LPNMHDR lpnmhdr);
	void ListColumnClick(LPNMHDR lpnmhdr, LPNMLISTVIEW pnmLV);
	void RightComboBoxSelect();
	void LeftComboBoxSelect();
	void SetListFocus(LPNMHDR lpnmHdr);
	void OpenItem(LPNMHDR lpnmHdr, LPNMLISTVIEW pnmLV);
	void CopyFiles();
	void DeleteFiles();

private:
	FileSystem *fileSystem;
	HWND hMainWindow;
	HINSTANCE hInstance;
	ListViewControl *listLeft;
	ListViewControl *listRight;
	LabelControl *labelLeft;
	LabelControl *labelRight;
	ComboBoxControl *comboLeft;
	ComboBoxControl *comboRight;
	bool focus;
};


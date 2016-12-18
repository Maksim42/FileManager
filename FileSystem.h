#pragma once
#include "Shellapi.h"
#include "ListViewControl.h"
#include "LabelControl.h"

class FileSystem
{
public:
	FileSystem();
	~FileSystem();
	bool FindFile(ListViewControl * list, LPWSTR dir);
	bool Open(ListViewControl *list, LPNMLISTVIEW pnmLV, LabelControl *label);
	bool FileCheck(LPWSTR name);
	bool OpenFile(LPWSTR name);
	BOOL InitListViewImageLists(ListViewControl * listView, int size, LPWSTR rootDir);
};


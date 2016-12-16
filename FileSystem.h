#pragma once
#include "Shellapi.h"
#include "ListViewControl.h"

class FileSystem
{
public:
	FileSystem();
	~FileSystem();
	bool FindFile(ListViewControl *list, LPWSTR rootDir, int pathNum);
	BOOL InitListViewImageLists(ListViewControl * listView, int size, LPWSTR rootDir);

	LPWSTR path1;
	LPWSTR path2;
};


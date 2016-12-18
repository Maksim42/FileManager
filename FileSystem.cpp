#include "stdafx.h"
#include "FileSystem.h"

FileSystem::FileSystem() {
	/*path1 = new TCHAR[MAX_PATH];
	path2 = new TCHAR[MAX_PATH];
	wcscpy_s(path1, MAX_PATH, L"C:\\");
	wcscpy_s(path2, MAX_PATH, L"C:\\");*/
}


FileSystem::~FileSystem() {
}

bool FileSystem::FindFile(ListViewControl *list, LPWSTR dir)
{
	list->Clear();
	int i = 0;

	LPWSTR rootDir = new TCHAR[MAX_PATH];
	wcscpy_s(rootDir, MAX_PATH, dir);
	wcscat_s(rootDir, MAX_PATH, L"*");

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile(rootDir, &FindFileData);
	delete rootDir;

	if (hFind == INVALID_HANDLE_VALUE) {
		return false;
	}
	else
	{
		do
		{
			wcscpy_s(list->path, MAX_PATH, dir);
			
			LPWSTR *item = new LPWSTR[list->columnCount];

			LPWSTR fullPath = new TCHAR[MAX_PATH];
			wcscpy_s(fullPath, MAX_PATH, dir);
			wcscat_s(fullPath, MAX_PATH, FindFileData.cFileName);

			if ((FILE_ATTRIBUTE_DIRECTORY & GetFileAttributes(fullPath))
				 != FILE_ATTRIBUTE_DIRECTORY) {
				item[1] = L"File"; 
			} else {
				item[1] = L"Directory";
			}

			item[0] = FindFileData.cFileName;
			item[2] = L"None";
			list->AddItem(item);

			delete[] item;

			i++;
		} while (FindNextFile(hFind, &FindFileData) != 0);

		FindClose(hFind);
		//InitListViewImageLists(list, i, rootDir);
	}

	list->Refresh();

	return true;
}

bool FileSystem::Open(ListViewControl *list, LPNMLISTVIEW pnmLV, LabelControl *label) {
	LPWSTR itemName = new TCHAR[MAX_PATH];
	list->GetItemText(pnmLV->iItem, itemName);

	LPWSTR fullPath = new TCHAR[MAX_PATH];
	wcscpy_s(fullPath, MAX_PATH, list->path);
	wcscat_s(fullPath, MAX_PATH, itemName);

	if (FileCheck(fullPath)) {
		//MessageBox(0, L"File", L"BreakComander", MB_OK | MB_ICONWARNING);
		OpenFile(fullPath);
	} else {
		//MessageBox(0, L"Dirictory", L"BreakComander", MB_OK | MB_ICONWARNING);
		wcscat_s(fullPath, MAX_PATH, L"\\");
		OpenDir(list, label, itemName, fullPath);
	}

	delete[] itemName;
	delete[] fullPath;

	return true;
}

bool FileSystem::FileCheck(LPWSTR name)
{
	if ((FILE_ATTRIBUTE_DIRECTORY & GetFileAttributes(name))
		!= FILE_ATTRIBUTE_DIRECTORY) {
		return true;
	} else {
		return false;
	}
}

bool FileSystem::OpenFile(LPWSTR name)
{
	HINSTANCE result = ShellExecute(0, L"open", name, NULL, NULL, SW_SHOWNORMAL);
	return true;
}

bool FileSystem::OpenDir(ListViewControl *list, LabelControl *label, LPWSTR name, LPWSTR path) {
	bool result = true;
	
	if (wcscmp(name, L".") == 0) {
		//FindFile(list, list->path);
		return result;
	}

	if (wcscmp(name, L"..") == 0) {
		MessageBox(0, L"Parent Folber", L"BreakComander", MB_OK | MB_ICONWARNING);
		return result;
	}

	LPWSTR oldPath = new TCHAR[MAX_PATH];
	wcscpy_s(oldPath, MAX_PATH, list->path);

	bool success = FindFile(list, path);
	if (success) {
		label->SetText(path);
	} else {
		MessageBox(0, L"Error", L"BreakComander", MB_OK | MB_ICONWARNING);
		FindFile(list, oldPath);
		result = false;
	}

	delete[] oldPath;
	return result;
}

bool ParentDir(LPWSTR path, LPWSTR newPath) {
	for (int i = wcslen(path) - 1; i > 0; i--) {
		// child dir cut
	}
}

BOOL FileSystem::InitListViewImageLists(ListViewControl *listView, int size, LPWSTR rootDir)
{
	//HIMAGELIST hSmall;
	//SHFILEINFO lp;
	//hSmall = ImageList_Create(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON),
	//										   ILC_MASK | ILC_COLOR32, size + 2, 1);

	//WIN32_FIND_DATA FindFileData;
	//HANDLE hFind;

	//hFind = FindFirstFile(rootDir, &FindFileData);
	//if (hFind == INVALID_HANDLE_VALUE) {
	//	//MessageBox(0, L"Ошибка", L"Не найден", MB_OK | MB_ICONWARNING);
	//} else {
	//	do {
	//		LPWSTR buf1 = new TCHAR[MAX_PATH];
	//		if (wcscmp(FindFileData.cFileName, L".") == 0) {
	//			wcscpy_s(buf1, MAX_PATH,rootDir);
	//			wcscat_s(buf1, MAX_PATH, FindFileData.cFileName);
	//			SHGetFileInfo(L"", FILE_ATTRIBUTE_DEVICE, &lp, sizeof(lp), SHGFI_ICONLOCATION | SHGFI_ICON | SHGFI_SMALLICON);

	//			ImageList_AddIcon(hSmall, lp.hIcon);
	//			DestroyIcon(lp.hIcon);
	//		}

	//		if (wcscmp(FindFileData.cFileName, L"..") == 0) {
	//			wcscpy_s(buf1, MAX_PATH, rootDir);
	//			wcscat_s(buf1, MAX_PATH, FindFileData.cFileName);
	//			SHGetFileInfo(L"", FILE_ATTRIBUTE_DIRECTORY, &lp, sizeof(lp), SHGFI_ICONLOCATION | SHGFI_ICON | SHGFI_SMALLICON);

	//			ImageList_AddIcon(hSmall, lp.hIcon);
	//			DestroyIcon(lp.hIcon);
	//		}

	//		wcscpy_s(buf1, MAX_PATH, rootDir);
	//		buf1[wcslen(buf1) - 1] = 0;

	//		wcscat_s(buf1, MAX_PATH, FindFileData.cFileName);
	//		DWORD num = GetFileAttributes(buf1);
	//		SHGetFileInfo(buf1, num, &lp, sizeof(lp), SHGFI_ICONLOCATION | SHGFI_ICON | SHGFI_SMALLICON);

	//		ImageList_AddIcon(hSmall, lp.hIcon);
	//		DestroyIcon(lp.hIcon);
	//		delete[] buf1;
	//	} while (FindNextFile(hFind, &FindFileData) != 0);

	//	FindClose(hFind);
	//}

	//ListView_SetImageList(listView->hList, hSmall, LVSIL_SMALL);

	return TRUE;
}

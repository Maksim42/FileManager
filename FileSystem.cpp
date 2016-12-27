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
	delete[] rootDir;

	if (hFind == INVALID_HANDLE_VALUE) {
		return false;
	}
	else
	{
		wcscpy_s(list->path, MAX_PATH, dir);

		do
		{
			if (wcscmp(FindFileData.cFileName, L".") == 0) {
				continue;
			}

			LPWSTR *item = new LPWSTR[list->columnCount];

			LPWSTR fullPath = new TCHAR[MAX_PATH];
			wcscpy_s(fullPath, MAX_PATH, dir);
			wcscat_s(fullPath, MAX_PATH, FindFileData.cFileName);

			bool file = false;
			if ((FILE_ATTRIBUTE_DIRECTORY & GetFileAttributes(fullPath))
				 != FILE_ATTRIBUTE_DIRECTORY) {
				item[1] = L"File"; 
				item[2] = new TCHAR[MAX_PATH];

				int size = (FindFileData.nFileSizeHigh * (MAXWORD + 1)) + FindFileData.nFileSizeLow;
				SizeConverting(size, item[2]);
				//swprintf(item[2], MAX_PATH, L"%db", size);

				file = true;
			} else {
				item[1] = L"Directory";
				item[2] = L"";
			}

			item[0] = FindFileData.cFileName;
			list->AddItem(item);

			if (file) {
				delete[] item[2];
			}
			delete[] item;

			i++;
		} while (FindNextFile(hFind, &FindFileData) != 0);

		FindClose(hFind);
		//InitListViewImageLists(list, i, rootDir);
	}

	list->sortingPatern = 0;
	list->Sorting(0);
	list->Refresh();

	return true;
}

void FileSystem::SizeConverting(int size, LPWSTR sizeString) {
	if (size - (size % 1000000000)) {
		swprintf(sizeString, MAX_PATH, L"%d Gb", size / 1000000000);
		return;
	}

	if (size - (size % 1000000)) {
		swprintf(sizeString, MAX_PATH, L"%d Mb", size / 1000000);
		return;
	}

	if (size - (size % 1000)) {
		swprintf(sizeString, MAX_PATH, L"%d Kb", size / 1000);
		return;
	}

	swprintf(sizeString, MAX_PATH, L"%d b", size);
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
	bool result = true;

	if ((FILE_ATTRIBUTE_DIRECTORY & GetFileAttributes(name))
		!= FILE_ATTRIBUTE_DIRECTORY) {
		result =  true;
	} else {
		result =  false;
	}

	return result;
}

bool FileSystem::OpenFile(LPWSTR name)
{
	HINSTANCE result = ShellExecute(0, L"open", name, NULL, NULL, SW_SHOWNORMAL);
	return true;
}

bool FileSystem::OpenDir(ListViewControl *list, LabelControl *label, LPWSTR name, LPWSTR path) {
	bool result = true;
	
	if (wcscmp(name, L".") == 0) {
		return result;
	}

	if (wcscmp(name, L"..") == 0) {
		//MessageBox(0, L"Parent Folber", L"BreakComander", MB_OK | MB_ICONWARNING);
		ParentDir(path);
		FindFile(list, path);
		label->SetText(path);
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

bool FileSystem::ParentDir(LPWSTR path) {
	int sleshCount = 0;

	for (int i = wcslen(path) - 1; i > 0; i--) {
		if (path[i] == '\\') {
			sleshCount++;
		}
		if (sleshCount == 3) {
			path[i + 1] = '\0';
			break;
		}
	}

	if (sleshCount != 3) {
		return false;
	}

	return true;
}

bool FileSystem::CopySelectedFiles(ListViewControl * from, ListViewControl * destination)
{
	LPWSTR fromPath = new TCHAR[MAX_PATH];
	LPWSTR destinationPath = new TCHAR[MAX_PATH];
	std::vector<LPWSTR> selectedList = from->GetSelectedItems();
	
	for (int i = 0; i < selectedList.size(); i++) {
		wcscpy_s(fromPath, MAX_PATH, from->path);
		wcscat_s(fromPath, MAX_PATH, selectedList[i]);
		wcscpy_s(destinationPath, MAX_PATH, destination->path);
		wcscat_s(destinationPath, MAX_PATH, selectedList[i]);
		
		CopyFile(fromPath, destinationPath, false);

		delete[] selectedList[i];
	}

	delete[] fromPath;
	delete[] destinationPath;

	return true;
}

bool FileSystem::DeleteSelectedFiles(ListViewControl * from) {
	LPWSTR fromPath = new TCHAR[MAX_PATH];
	std::vector<LPWSTR> selectedList = from->GetSelectedItems();

	for (int i = 0; i < selectedList.size(); i++) {
		wcscpy_s(fromPath, MAX_PATH, from->path);
		wcscat_s(fromPath, MAX_PATH, selectedList[i]);

		DeleteFile(fromPath);

		delete[] selectedList[i];
	}

	return true;
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

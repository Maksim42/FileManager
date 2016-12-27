// FileManager.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "FileManager.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
Control *control;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FILEMANAGER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FILEMANAGER));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FILEMANAGER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_FILEMANAGER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, (WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME)
			   & ~WS_MAXIMIZEBOX, CW_USEDEFAULT, 0, 900, 600,
			   nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);

            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;

			case LeftComboBox:
				switch (HIWORD(wParam)) {
				case CBN_SELENDOK:
					control->LeftComboBoxSelect();
					//MessageBox(hWnd, L"ComboBox", L"Click left", MB_OK | MB_ICONWARNING);
					break;
				}
				break;

			case RightComboBox:
				switch (HIWORD(wParam)) {
				case CBN_SELENDOK:
					control->RightComboBoxSelect();
					//MessageBox(hWnd, L"ComboBox", L"Click right", MB_OK | MB_ICONWARNING);
					break;
				}
				break;

			case ID_COPY:
				//MessageBox(hWnd, L"Copy Hotkey", L"BreakComander", MB_OK | MB_ICONWARNING);
				control->CopyFiles();
				break;

			case ID_DELETE:
				control->DeleteFiles();
				break;

            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

	case WM_NOTIFY: {
		LPNMHDR lpnmHdr = (LPNMHDR)lParam;
		LPNMLISTVIEW pnmLV = (LPNMLISTVIEW)lParam;

		switch (lpnmHdr->code) {
		case LVN_COLUMNCLICK:
			//MessageBox(hWnd, L"Column Click", L"Click left", MB_OK | MB_ICONWARNING);
			//pnmLV->iSubItem
			control->ListColumnClick(lpnmHdr, pnmLV);
			break;

		case LVN_BEGINLABELEDIT:
			//MessageBox(hWnd, L"Begin edit", L"Click left", MB_OK | MB_ICONWARNING);
			//control->BeginEditHandler(lpnmHdr);
			break;

		case LVN_ENDLABELEDIT:
			//MessageBox(hWnd, L"End edit", L"Click left", MB_OK | MB_ICONWARNING);
			//control->EndEditHandler(lpnmHdr);
			break;

		case NM_SETFOCUS:
			//MessageBox(hWnd, L"Focused", L"Click left", MB_OK | MB_ICONWARNING);
			control->SetListFocus(lpnmHdr);
			break;

		case NM_CLICK:
			break;

		case NM_DBLCLK:
			//MessageBox(hWnd, L"DBclick", L"Click left", MB_OK | MB_ICONWARNING);
			control->OpenItem(lpnmHdr, pnmLV);
			break;
		}
	}
	break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;

	case WM_CREATE: {
		control = new Control(hWnd, hInst);
	}
	break;

    case WM_DESTROY:
		delete control;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

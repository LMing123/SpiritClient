// AdminLauncher.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "AdminLauncher.h"

#include "shellapi.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

bool RunAdminProcess(CString sFilePath, CString sCmdLine);
CString GetFilePath();  //��õ�ǰexe·��
CString GetFolderPath();  //��õ�ǰ�ļ���·��

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_ADMINLAUNCHER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ADMINLAUNCHER));

	CString cmdLine( lpCmdLine );
	//CString path = GetFolderPath() + L"SpiritClinet.exe";
	if ( cmdLine.Find( L"write" ) != -1 )
	{
		RunAdminProcess( L"RegWorker.exe" , L"write" );
	}
	else if ( cmdLine.Find( L"delete" ) != -1 )
	{
		RunAdminProcess( L"RegWorker.exe" , L"delete" );
	}
	else
	{
		MessageBox( NULL, L"�봫�ݲ�����", L"����", MB_OK );
	}

	PostQuitMessage(0);

	// ����Ϣѭ��:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

bool RunAdminProcess(CString sFilePath, CString sCmdLine)
{
	OSVERSIONINFOEX OSVerInfo;
	OSVerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if( !GetVersionEx( (OSVERSIONINFO *)&OSVerInfo ) )
	{
			OSVerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			GetVersionEx((OSVERSIONINFO *)&OSVerInfo);
	}

	TCHAR atszVerb[16];
	if(OSVerInfo.dwMajorVersion >= 6) // Vista ����
	{
			_tcscpy_s(atszVerb,_T("runas"));
	}
	else
	{
			_tcscpy_s(atszVerb,_T(""));
	}

	// Launch itself as administrator.
	SHELLEXECUTEINFO sei;
	memset( &sei, 0, sizeof(SHELLEXECUTEINFO) );
	sei.cbSize = sizeof(SHELLEXECUTEINFO); 
	sei.lpVerb = atszVerb;
	//sei.lpFile = szPath;
	sei.lpFile = (LPCTSTR)sFilePath;
	sei.lpParameters = (LPCTSTR)sCmdLine;
	//sei.hwnd = hWnd;
	sei.nShow = SW_SHOWNORMAL;
		
	if ( !ShellExecuteEx(&sei) )
	{
		DWORD dwStatus = GetLastError();
		if (dwStatus == ERROR_CANCELLED)
		{
			// The user refused to allow privileges elevation.
			return false;
		}
		else
			if (dwStatus == ERROR_FILE_NOT_FOUND)
			{
				// The file defined by lpFile was not found and
				// an error message popped up.
				return FALSE;
			}
				
		return false;
	}    
		
	return true;
}

//��õ�ǰ�ļ���·��
CString GetFilePath()
{
	const int pathLen = 10240;
	TCHAR filePath[pathLen];
	GetModuleFileName( NULL, filePath, pathLen );

	CString path( filePath );

	return path;
}

//��õ�ǰ�ļ���
CString GetFolderPath()
{
	CString path = GetFilePath();
	for (int i=path.GetLength()-1; i>=0; i--)
	{
		if ( path[i] == '\\' )
		{
			path = path.Left(i+1);
			break;
		}
	}

	return path;
}

//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ADMINLAUNCHER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 300, 200, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   //ShowWindow(hWnd, nCmdShow);
   //UpdateWindow(hWnd);

   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��:
		switch (wmId)
		{
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: �ڴ���������ͼ����...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

#ifndef UNICODE
#define UNICODE
#endif // !UNICODE

#include <Windows.h>
#include "resource.h"
#include "MapiMuleI.h"
#include <MAPI.h>
#include <MapiX.h>
#include <MAPIUtil.h>
#include <atlbase.h>


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT InitSession(HWND hwnd);
bool LogoffProvider(HWND hwnd);
MAPIINIT_0 mpInit;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	//Register the window class
	const wchar_t CLASS_NAME[] = L"MAPI Mule I Class";

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);

	RegisterClass(&wc);

	// Create the window
	HWND hwnd = CreateWindowEx(
		0,							//Optional window styles
		CLASS_NAME,					//Window class
		L"MAPI Mule I",				//Window text
		WS_OVERLAPPEDWINDOW,		//Window style

		//Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,						//Parent window
		NULL,						//Parent window
		hInstance,					//Instance handle
		NULL						//Parent window
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	//Run the message loop
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		InitSession(hwnd);
		return 0;

	case WM_DESTROY:
	{
		LogoffProvider(hwnd);
		PostQuitMessage(0);
		return 0;
	}

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		EndPaint(hwnd, &ps);
		return 0;
	}

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_FILE_EXIT:
		{
			LogoffProvider(hwnd);
			PostQuitMessage(0);
			return 0;
		}
		case ID_HELP_ABOUT:
		{
			MessageBox(hwnd, L"Mapi Mule Ver 1", L"Mapi Mule", MB_OK);
		}
		default:
			return 0;
		}

		return 0;
	}

	return 0;

	}
	// If none of the above messages are passed to the window proc,
	// DefWindowProc will handle it
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


//MAPI initialization and logon funcitonality goes here....
HRESULT InitSession(HWND hwnd)
{
	HRESULT hr;
	mpInit = { 0, MAPI_MULTITHREAD_NOTIFICATIONS };

	CORg(MAPIInitialize(&mpInit));
	
	{  // context for smart pointer -- compiler error if this isn't present	
		CComPtr<IMAPISession> spSession;
		CORg(MAPILogonEx(NULL, NULL, NULL, MAPI_UNICODE | MAPI_LOGON_UI, &spSession));
	}  // end of smart pointer context
Error:
	return hr;
}

bool LogoffProvider(HWND hwnd)
{
	MAPIUninitialize();
	
	return true;
}

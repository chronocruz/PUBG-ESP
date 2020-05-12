#include<iostream>
#include<windows.h>
#include"MemoryManager.h"
#include "DirectXM.h"
#include <tchar.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <Psapi.h>
#include<stdio.h>
#include <math.h>
#include<thread>
#include <cmath>
#include"ESPWork.h"


#pragma comment(lib,"Psapi.lib")
using namespace std;

//const TCHAR *szCaption = _T("Bluestacks");
const TCHAR* szCaption = _T("LDPlayer");
//const TCHAR *szCaption = _T("SmartGaGa");
//const TCHAR *szCaption = _T("Gameloop【Turbo AOW Engine】");


HANDLE hProcess;

int main()
{

	bool Play = false;

	DWORD processIid = FindProcessId(L"LdVBoxHeadless.exe");

	//auto processId = FindProcessId(L"HD-Player.exe");
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processIid);
	pMM = new MemoryManager(hProcess);
	handle(hProcess);
	if (hProcess != NULL)
	{
		cout << "Process: " << hProcess << endl;
		cout << "PID: " << processIid << endl;
		cout << "Searching for ViewMatrix.........." << endl;
		Play = cheak_ViewMatrix();


	}


	/*int data;
	ReadProcessMemory(hProcess, (LPCVOID)(0x7BFB6DD8 + 0x1B5), &data , sizeof(int), NULL);
	if(data == 2)
	cout <<hex<<data << endl;*/




	while (Play)
	{
		pDxm = new DirectXM();
		pDxm->SetupWindow(szCaption);

		MSG msg;
		while (1)
		{

			ZeroMemory(&msg, sizeof(MSG));
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			HWND hWnd = NULL;
			hWnd = FindWindow(0, szCaption);
			if (!hWnd)
			{
				DestroyWindow(pDxm->over_hWnd);
				delete pDxm;
				break;
			}

			hWnd = ::GetWindow(hWnd, GW_CHILD);

			RECT rc;
			ZeroMemory(&rc, sizeof(RECT));

			GetWindowRect(hWnd, &rc);

			pDxm->s_width = rc.right - rc.left;
			pDxm->s_height = rc.bottom - rc.top;

			MoveWindow(pDxm->over_hWnd, rc.left, rc.top, pDxm->s_width, pDxm->s_height, true);

			HWND hwnd2 = GetForegroundWindow();
			HWND hwnd3 = GetWindow(hwnd2, GW_HWNDPREV);



			SetWindowPos(pDxm->over_hWnd, hwnd3, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			UpdateWindow(pDxm->over_hWnd);
			pDxm->render();
			Sleep(10);
		}
	}

	cout << "Exit Game!" << endl;
	cin.get();
	return 0;
}
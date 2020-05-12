#include "DirectXM.h"
#include "ESPWork.h"
#include <time.h>
#include<iostream>
#include"MemoryManager.h"


using namespace std;



#define DARKORANGE  D3DCOLOR_ARGB(255,255,127,000)
#define TextWhite   D3DCOLOR_ARGB(255,255,255,255) 
#define TextRed     D3DCOLOR_ARGB(255,255,0,0) 
#define TextGreen   D3DCOLOR_ARGB(255,0,255,0)
#define TextBlue    D3DCOLOR_ARGB(255,0,0,255)
#define TextBlack   D3DCOLOR_ARGB(255,0,0,0)
#define TextPurple  D3DCOLOR_ARGB(255,125,0,255)
#define TextGrey    D3DCOLOR_ARGB(255,128,128,128)
#define TextYellow  D3DCOLOR_ARGB(255,255,255,0)
#define TextOrange  D3DCOLOR_ARGB(255,255,140,0)
#define TextCyan    D3DCOLOR_ARGB(255,0,139,200)
#define TextPink    D3DCOLOR_ARGB(255,255,192,203)

int enNum = 0;
MemoryManager *pMM = NULL;

DirectXM::DirectXM()
{
	game_hWnd = NULL;
	g_pIndexBuffer = NULL;
	g_pVertexBuffer = NULL;
}

void DirectXM::DrawLine(float x, float y, float center, D3DCOLOR color)
{
	D3DXVECTOR2 points[2];
	points[0] = D3DXVECTOR2(s_width / 2, 0);
	points[1] = D3DXVECTOR2(x, y);
	pD3DLine->SetWidth(1);
	pD3DLine->Draw(points, 2, color);
}

DirectXM::~DirectXM()
{
	SAFE_RELEASE(pD3D);
	SAFE_RELEASE(g_pIndexBuffer)
	SAFE_RELEASE(g_pVertexBuffer)
	SAFE_RELEASE(pFont)
	SAFE_RELEASE(pFontMenu)
	SAFE_RELEASE(pDevice)
	SAFE_RELEASE(pD3DLine)

}

int enNumper()
{
	enNum = GetpNum();
	if (enNum < 2)
	{
		return	enNum = 0;
	}
	else
		return enNum - 2;
}

void PrintString()
{

	pDxm->DrawString(500 - 40, 40, TextRed, pDxm->pFont, "Chronocruz ESP", 1);
	pDxm->DrawString(250, 200, TextYellow, pDxm->pFont, "Enemies Near:[%d]", enNumper());

}

void DirectXM::FillRGB(int x, int y, int w, int h, D3DCOLOR color)
{
	D3DRECT rec = { x, y, x + w, y + h };
	pDevice->Clear(1, &rec, D3DCLEAR_TARGET, color, 0, 0);
}

void DirectXM::render()
{
	    

		// clear the window alpha
		pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);//D3DCLEAR_ZBUFFER
		pDevice->BeginScene();    // begins the 3D scene
		
		PrintString();

		print();
							
		pDevice->EndScene();    // ends the 3D scene
		pDevice->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen
	
}

void DirectXM::DrawString(int x, int y, DWORD D3DColor, LPD3DXFONT pUseFont, const char *fmt, ...)
{
	//OutputDebugStringA("DrawString\n");
	char szText[1024] = { 0 };
	va_list va_alist;
	RECT rFontPos = { x, y, x + 120, y + 16 };
	va_start(va_alist, fmt);
	vsprintf_s(szText, fmt, va_alist);
	va_end(va_alist);
	pUseFont->DrawTextA(NULL, szText, -1, &rFontPos, DT_NOCLIP, D3DColor);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	

	switch (uMsg)
	{
	case WM_PAINT:
	{
		
		DirectXM* dm = (DirectXM*)GetWindowLong(hWnd, GWLP_USERDATA);
		dm->render();
		ValidateRect(hWnd, NULL); 
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void DirectXM::SetupWindow(LPCTSTR szWndName)
{	
	RECT rc;
	while (!game_hWnd)
	{
		Sleep(500);
		game_hWnd = ::FindWindow(0, szWndName);

	}
	if (game_hWnd != NULL)
	{
		game_hWnd = ::GetWindow(game_hWnd, GW_CHILD);

		Sleep(1000);
		GetWindowRect(game_hWnd, &rc);
		s_width = rc.right - rc.left;
		s_height = rc.bottom - rc.top;
	}
	else
	{
	}
	WNDCLASSEXA wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(0);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)RGB(0, 0, 0);
	srand((unsigned)time(NULL));	
	const auto pClassN = "Chrono_ESP";	
	wc.lpszClassName = pClassN;
	RegisterClassExA(&wc);	
	over_hWnd = CreateWindowExA(WS_EX_LAYERED | WS_EX_TRANSPARENT, pClassN, "BOTWIN", WS_POPUP, rc.left, rc.top, s_width, s_height, NULL, NULL, wc.hInstance, NULL);
	SetWindowLong(over_hWnd, GWLP_USERDATA, (LONG)this);
	InitD3D();
	SetLayeredWindowAttributes(over_hWnd, RGB(0, 0, 0), 0, ULW_COLORKEY);
	SetLayeredWindowAttributes(over_hWnd, 0, 255, LWA_ALPHA);
	MARGINS  margin = { -1, -1, -1, -1 }; 										 
	DwmExtendFrameIntoClientArea(over_hWnd, &margin);
	ShowWindow(over_hWnd, SW_SHOW);
	UpdateWindow(over_hWnd);
}

void DirectXM::InitD3D()
{
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);    // create the Direct3D interface
	if (pD3D == NULL)
	{
		
		cout << "Direct3DCreate9 failed" << endl;
		return;
	}
	D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information

	D3DCAPS9 caps; int vp = 0;
	if (FAILED(pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
	{
	
		cout << "GetDeviceCaps failed" << endl;
		return;
	}
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;   
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	ZeroMemory(&d3dpp, sizeof(d3dpp));    // clear out the struct for use
	d3dpp.Windowed = TRUE;    // program windowed, not fullscreen
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
	d3dpp.hDeviceWindow = over_hWnd;    // set the window to be used by Direct3D
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;     // set the back buffer format to 32-bit
	d3dpp.BackBufferWidth = s_width;    // set the width of the buffer
	d3dpp.BackBufferHeight = s_height;    // set the height of the buffer

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	

	// create a device class using this information and the info from the d3dpp stuct
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, over_hWnd, vp, &d3dpp, &pDevice)))
	{
		cout << "CreateDevice failed" << endl;
		return;
	}

	
	D3DXCreateFontA(pDevice, 13, 0, FW_SEMIBOLD, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "simsun", &pFont);
	D3DXCreateFontA(pDevice, 15, 0, FW_NORMAL, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "simsun", &pFontMenu);
	D3DXCreateLine(pDevice, &pD3DLine);



}

void DirectXM::DrawBox(float x, float y, float width, float height, D3DCOLOR color )
{
	D3DXVECTOR2 points[5];
	points[0] = D3DXVECTOR2(x, y);
	points[1] = D3DXVECTOR2(x + width, y);
	points[2] = D3DXVECTOR2(x + width, y + height);
	points[3] = D3DXVECTOR2(x, y + height);
	points[4] = D3DXVECTOR2(x, y);
	pD3DLine->SetWidth(2);
	pD3DLine->Draw(points, 5, color);
}


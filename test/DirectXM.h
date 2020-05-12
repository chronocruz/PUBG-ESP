#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <Dwmapi.h> 

#pragma comment(lib,"dwmapi.lib")
#pragma comment(lib,"d3d9.lib") 
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib, "winmm.lib")

#define M_PI	3.14159265358979323846264338327950288419716939937510

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }      


class DirectXM
{
public:
	DirectXM();
	~DirectXM();


	HWND game_hWnd;
	HWND over_hWnd;
	int s_width;
	int s_height;
	void SetupWindow(LPCTSTR szWndName);

	LPDIRECT3D9 pD3D;
	LPDIRECT3DDEVICE9 pDevice;
	LPD3DXFONT pFont;
	LPD3DXFONT pFontMenu;
	ID3DXLine* pD3DLine;
	void InitD3D();
	void render();
	void DrawString(int x, int y, DWORD D3DColor, LPD3DXFONT pUseFont, const char *fmt, ...);
	void DrawBox(float x, float y, float width, float height, D3DCOLOR color);
	void DrawLine(float x, float y, float center, D3DCOLOR color);
	void FillRGB(int x, int y, int w, int h, D3DCOLOR color);

	LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer;   
	LPDIRECT3DINDEXBUFFER9  g_pIndexBuffer;    
	
	
};


struct CUSTOMVERTEX
{
	FLOAT x, y, z;
	DWORD color;
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)  //FVF灵活顶点格式#pragma once

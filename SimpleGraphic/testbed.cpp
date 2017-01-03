#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include "RenderManager.h"

const TCHAR* WINDOW_NAME = _T("jj");
const TCHAR* WINDOW_CAPTION = _T("SimpleGraphics - by jj");
UINT WINDOW_POS_X = 100;
UINT WINDOW_POS_Y = 100;
UINT WINDOW_WIDTH = 497;
UINT WINDOW_HEIGHT = 500;

HWND g_hwnd = NULL;

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){
	switch (msg){
	case WM_PAINT:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wp){
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	}
	return DefWindowProc(hwnd, msg, wp, lp);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR lpCmdLine, int nCmdLine){
	WNDCLASSEX myWnd = {
		sizeof(WNDCLASSEX),
		CS_HREDRAW | CS_VREDRAW,
		WinProc,
		0, 0,
		hInst,
		NULL,
		NULL,
		NULL,
		NULL,
		WINDOW_NAME,
		NULL
	};
	if (FAILED(RegisterClassEx(&myWnd))){
		return E_FAIL;
	};

	g_hwnd = CreateWindowEx(
		NULL,
		WINDOW_NAME,
		WINDOW_CAPTION,
		WS_OVERLAPPEDWINDOW,
		WINDOW_POS_X, WINDOW_POS_Y,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		NULL, NULL, hInst, NULL);
	if (g_hwnd == NULL){
		return E_FAIL;
	}

	if (FAILED(ShowWindow(g_hwnd, SW_SHOWDEFAULT))){
		return E_FAIL;
	}

	// 初始化管理的类
	if (!RenderManager::Init(WINDOW_WIDTH, WINDOW_HEIGHT)){
		return E_FAIL;
	}

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (true){
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			if (msg.message == WM_QUIT){
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else{
			RenderManager::GetInstance()->Clear();
			RenderManager::GetInstance()->Render();
			RenderManager::GetInstance()->Present();
		}
	}

	RenderManager::Uninit();
	UnregisterClass(WINDOW_NAME, myWnd.hInstance);

	return S_OK;
}
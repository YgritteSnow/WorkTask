#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include "RenderManager.h"
#include "Scene.h"
#include "Light.h"
#include "Texture.h"

const TCHAR* WINDOW_NAME = _T("jj");
const TCHAR* WINDOW_CAPTION = _T("SimpleGraphics - by jj");
UINT WINDOW_POS_X = 100;
UINT WINDOW_POS_Y = 100;
UINT WINDOW_WIDTH = 500;
UINT WINDOW_HEIGHT = 500;
TimeType MAX_FRAME_RATE = 0.0001;
HWND g_hwnd = NULL;

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){
	switch (msg){
	case WM_PAINT:
		SceneManager::GetInstance()->Render();
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
		CS_CLASSDC,
		//CS_HREDRAW | CS_VREDRAW,
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
		WS_VISIBLE | WS_POPUP,
		WINDOW_POS_X, WINDOW_POS_Y,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		NULL, NULL, hInst, NULL);
	if (g_hwnd == NULL){
		return E_FAIL;
	}

	if (FAILED(ShowWindow(g_hwnd, SW_SHOWDEFAULT))){
		return E_FAIL;
	}

	auto initResult = S_OK;
	// 初始化管理的类
	if (!RenderManager::Init(WINDOW_WIDTH, WINDOW_HEIGHT)){
		initResult = E_FAIL;
	}
	if (!SceneManager::Init()){
		initResult = E_FAIL;
	}
	if (!CameraManager::Init()){
		initResult = E_FAIL;
	}
	if (!TimeManager::Init(MAX_FRAME_RATE)){
		initResult = E_FAIL;
	}
	if (!LightManager::Init()){
		initResult = E_FAIL;
	}
	if (!TextureManager::Init()){
		initResult = E_FAIL;
	}

	if (initResult == S_OK){
		// 设置一些测试数据
		Model<DummyVertex>* dummyModel = new Model<DummyVertex>;
		//dummyModel->DummyBall(1, 28, 10, NormColor4(1,1,1,1));
		dummyModel->DummyQuad(5, 5);

		Scene* dummyScene = new Scene;
		dummyScene->AddModel(dummyModel);

		SceneManager::GetInstance()->AddScene(dummyScene);

		RenderManager::GetInstance()->SetRenderState(StateMask_DrawMode, StateMaskValue_Fill);
		//RenderManager::GetInstance()->SetRenderState(StateMask_DrawMode, StateMaskValue_Wareframe);
		//RenderManager::GetInstance()->SetRenderState(StateMask_Light, StateMaskValue_LightDisable);
		RenderManager::GetInstance()->SetRenderState(StateMask_Light, StateMaskValue_LightEnable);
		RenderManager::GetInstance()->SetRenderState(StateMask_CalNormal, StateMaskValue_NotCalNormal);

		// 主循环
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
				if (TimeManager::GetInstance()->Tick()){
					CameraManager::GetInstance()->Update(TimeManager::GetInstance()->GetFrameInterval());
					SceneManager::GetInstance()->Update(TimeManager::GetInstance()->GetFrameInterval());
					SceneManager::GetInstance()->Render();

					dummyModel->RotateXYZ(0, 0, 0.01);
				}
			}
		}
	}

	CameraManager::UnInit();
	SceneManager::UnInit();
	RenderManager::UnInit();
	TimeManager::UnInit();
	LightManager::UnInit();
	UnregisterClass(WINDOW_NAME, myWnd.hInstance);

	return S_OK;
}
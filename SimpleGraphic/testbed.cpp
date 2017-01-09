#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include "RenderManager.h"
#include "Scene.h"
#include "Light.h"
#include "Texture.h"
#include "InputEvent.h"

const TCHAR* WINDOW_NAME = _T("jj");
const TCHAR* WINDOW_CAPTION = _T("SimpleGraphics - by jj");
UINT WINDOW_POS_X = 100;
UINT WINDOW_POS_Y = 100;
UINT WINDOW_WIDTH = 400;
UINT WINDOW_HEIGHT = 300;
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
		default:
			InputEventHandlerManager::GetInstance()->HandleKeyEvent(wp, true);
		}
		break;
	case WM_LBUTTONDOWN:
		InputEventHandlerManager::GetInstance()->HandleMouseEvent(true);
		break;
	case WM_LBUTTONUP:
		InputEventHandlerManager::GetInstance()->HandleMouseEvent(false);
		break;
	case WM_MOUSEMOVE:
		InputEventHandlerManager::GetInstance()->HandleMouseMoveEvent(LOWORD(lp), HIWORD(lp));
		break;
	}
	return DefWindowProc(hwnd, msg, wp, lp);
}

LRESULT InitManagers();
void UnInitManagers();

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
	
	if (InitManagers() == S_OK){
		// 设置相机
		CameraManager::GetInstance()->CurrentCamera()->SetProjMat(1.4f, (float)WINDOW_WIDTH / WINDOW_HEIGHT, 1.f, 1000.f);
		CameraManager::GetInstance()->CurrentCamera()->SetViewMat(WorldPos(0, 0, 5), WorldPos(0, 0, 0), WorldPos(0, 1, 0));

		// 模型（距离5）
		Model<DummyVertex>* dummyModel_mid = new Model<DummyVertex>;
		dummyModel_mid->DummyBall(1, 10, 20, NormColor4(1, 1, 1, 1), WorldPos(0, 0, 5));

		// 模型（距离6）
		//Model<DummyVertex>* dummyModel_far = new Model<DummyVertex>;
		//dummyModel_far->DummyBall(1.7, 10, 20, NormColor4(1, 1, 1, 1), WorldPos(1, 1, 6));

		// 模型（距离2）
		Model<DummyVertex>* dummyModel_near = new Model<DummyVertex>;
		dummyModel_near->DummyBall(0.7, 10, 20, NormColor4(1, 1, 1, 1), WorldPos(-0.3, -0.3, 4));

		// 设置场景，添加模型
		Scene* dummyScene = new Scene;
		//dummyScene->AddModel(dummyModel_far);
		dummyScene->AddModel(dummyModel_near);
		dummyScene->AddModel(dummyModel_mid);

		SceneManager::GetInstance()->AddScene(dummyScene);

		// 设置材质
		MaterialManager::GetInstance()->SetMaterial(Material(
			NormColor4(1, 1, 1, 1) * 0.1,
			NormColor4(1, 1, 1, 1) * 0.3,
			NormColor4(0.7, 0.7, 0.7, 50),
			NormColor4(1, 1, 1, 1) * 0.1
		));

		// 设置贴图
		TextureManager::GetInstance()->SetTexture("tex_alpha_color.tga");

		// 设置光照
		LightManager::GetInstance()->AddLight(new AmbientLight(NormColor4(1, 1, 1, 1)));
		LightManager::GetInstance()->AddLight(new DirectLight(NormColor4(0, 1, 0, 1) * 2, WorldPos(1, 0, 0)));
		LightManager::GetInstance()->AddLight(new DirectLight(NormColor4(0, 0, 1, 1) * 2, WorldPos(-1, 0, 0)));
		LightManager::GetInstance()->AddLight(new DirectLight(NormColor4(1, 0, 0, 1) * 2, WorldPos(0, 1, 0)));

		// 设置渲染状态
		RenderManager::GetInstance()->SetRenderState(StateMask_DrawMode, StateMaskValue_Fill);
		RenderManager::GetInstance()->SetRenderState(StateMask_Light, StateMaskValue_LightEnable);
		RenderManager::GetInstance()->SetRenderState(StateMask_CalNormal, StateMaskValue_NotCalNormal);
		RenderManager::GetInstance()->SetRenderState(StateMask_BackCull, StateMaskValue_BackCullR);
		RenderManager::GetInstance()->SetRenderState(StateMask_DepthBuffer, StateMaskValue_UseDepth);
		RenderManager::GetInstance()->SetRenderState(StateMask_Alpha, StateMaskValue_UseAlpha);

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
				}
			}
		}
	}

	UnregisterClass(WINDOW_NAME, myWnd.hInstance);

	return S_OK;
}

LRESULT InitManagers() {
	if (!InputEventHandlerManager::Init()) { return E_FAIL; }
	if (!RenderManager::Init(WINDOW_WIDTH, WINDOW_HEIGHT)) {return E_FAIL;}
	if (!SceneManager::Init()) {return E_FAIL;}
	if (!CameraManager::Init()) {return E_FAIL;}
	if (!TimeManager::Init(MAX_FRAME_RATE)) {return E_FAIL;}
	if (!LightManager::Init()) {return E_FAIL;}
	if (!TextureManager::Init()) {return E_FAIL;}
	if (!MaterialManager::Init()) {return E_FAIL;}
	return S_OK;
}
void UnInitManagers() {
	CameraManager::UnInit();
	SceneManager::UnInit();
	RenderManager::UnInit();
	TimeManager::UnInit();
	LightManager::UnInit();
	TextureManager::UnInit();
	MaterialManager::UnInit();
	InputEventHandlerManager::UnInit();
}

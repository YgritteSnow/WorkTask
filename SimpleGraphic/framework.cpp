#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include "RenderManager.h"
#include "Scene.h"
#include "Light.h"
#include "TexManager.h"
#include "InputEvent.h"
#include "AnimatorManager.h"
#include "AniResManager.h"

#include "StructMeta.h"
#include "TestModel.h"
#include "TestSkinnedModel.h"
#include "LightingShader.h"
#include "DebugGUI.h"

const TCHAR* WINDOW_NAME = _T("jj");
const TCHAR* WINDOW_CAPTION = _T("SimpleGraphics - by jj");
unsigned int WINDOW_POS_X = 100;
unsigned int WINDOW_POS_Y = 100;
unsigned int WINDOW_WIDTH = 500;
unsigned int WINDOW_HEIGHT = 500;
TimeType MAX_FRAME_RATE = 120.f;
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

	// 这里注册所有的 Vertex 类型
	REFLECTION_FILTER_FUNC();
	
	if (InitManagers() == S_OK){
		// 设置相机
		CameraManager::GetInstance()->CurrentCamera()->SetProjMat(1.f, (float)WINDOW_WIDTH / WINDOW_HEIGHT, 1.f, 100.f);
		CameraManager::GetInstance()->CurrentCamera()->SetViewMat(WorldPos(0, 0, 5), WorldPos(0, 0, 0), WorldPos(0, 1, 0));

		// 设置材质
		MaterialManager::GetInstance()->SetMaterial(Material(
			NormColor4(1, 1, 1, 1) * 1,
			NormColor4(1, 1, 1, 1) * 1,
			NormColor4(0.7, 0.7, 0.7, 30),
			NormColor4(1, 1, 1, 1) * 0
		));

		// 设置光照
		LightManager::GetInstance()->AddLight(new AmbientLight(NormColor4(1, 1, 1, 1)));
		//LightManager::GetInstance()->AddLight(new DirectLight(NormColor4(0, 1, 0, 1), WorldPos(1, 0, 0)));
		//LightManager::GetInstance()->AddLight(new DirectLight(NormColor4(0, 0, 1, 1), WorldPos(-1, 0, 0)));
		//LightManager::GetInstance()->AddLight(new DirectLight(NormColor4(1, 0, 0, 1), WorldPos(0, 1, 0)));

		// 设置场景，添加模型
		Scene* dummyScene = new Scene;
		
		//// 模型（球（远））
		//TestModel* dummyModel_far = new TestModel;
		//dummyModel_far->DummyBall(0.7, 10, 20, NormColor4(1, 1, 1, 1), WorldPos(0, 0, 4));
		//dummyScene->AddModel(dummyModel_far);
		
		//// 模型（球（近））
		//TestModel* dummyModel_near = new TestModel;
		//dummyModel_near->DummyBall(0.4, 10, 20, NormColor4(1, 1, 1, 1), WorldPos(0.4, 0, 2));
		//dummyScene->AddModel(dummyModel_near);

		//// 模型（水平地面）
		//TestModel* dummyModel_ground = new TestModel;
		//dummyModel_ground->DummyGround(2, 2, NormColor4(1, 1, 1, 1), WorldPos(0, -0.5, 3));
		//dummyScene->AddModel(dummyModel_ground);

		// 蒙皮模型（蛇）
		TestSkinnedModel* dummyModel_snake = new TestSkinnedModel;
		dummyModel_snake->DummySnake(3, 0.3, 1.1f, 10, 5);
		dummyScene->AddModel(dummyModel_snake);

		// 蒙皮模型（骨骼）
		TestSkinnedModel* dummyModel_bone = new TestSkinnedModel;
		dummyModel_bone->DummyBones(3);
		dummyScene->AddModel(dummyModel_bone);
		
		//// 模型（一个四方面片）
		//TestModel* dummyModel_quad = new TestModel;
		//dummyModel_quad->DummyQuad(1,1,5);
		//dummyScene->AddModel(dummyModel_quad);

		SceneManager::GetInstance()->AddScene(dummyScene);

		// 设置渲染状态
		RenderManager::GetInstance()->SetRenderState(StateMask_DrawMode, StateMaskValue_Wareframe);
		RenderManager::GetInstance()->SetRenderState(StateMask_Cull, StateMaskValue_UseCull);
		RenderManager::GetInstance()->SetRenderState(StateMask_BackCull, StateMaskValue_BackCull);
		RenderManager::GetInstance()->SetRenderState(StateMask_DepthBuffer, StateMaskValue_UseDepth);
		RenderManager::GetInstance()->SetRenderState(StateMask_Alpha, StateMaskValue_NoAlpha);

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
				if (TimeManager::GetInstance()->FrameTick()) {
					DebugManager::GetInstance()->SetFPS(1.0 / TimeManager::GetInstance()->GetFrameInterval());

					CameraManager::GetInstance()->Update(TimeManager::GetInstance()->GetFrameInterval());
					SceneManager::GetInstance()->Update(TimeManager::GetInstance()->GetFrameInterval());

					RenderManager::GetInstance()->Clear();
					DebugManager::GetInstance()->OnRenderBegin();

					SceneManager::GetInstance()->Render();

					RenderManager::GetInstance()->Present();

					DebugManager::GetInstance()->Render();
				}
				if (TimeManager::GetInstance()->AnimateTick()) {
					AnimatorManager::GetInstance()->Update(TimeManager::GetInstance()->GetAniInterval());
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
	if (!ShaderManager::Init()) { return E_FAIL; }
	if (!DebugManager::Init()) { return E_FAIL; }
	if (!AnimatorManager::Init()) { return E_FAIL; }
	if (!AniResManager::Init()) { return E_FAIL; }
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
	ShaderManager::UnInit();
	DebugManager::UnInit();
	AnimatorManager::UnInit();
	AniResManager::UnInit();
}

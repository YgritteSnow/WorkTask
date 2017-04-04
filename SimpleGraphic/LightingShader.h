#pragma once
#include "JMath.h"
#include "Color.h"
#include "ShaderStruct.h"

#include "TestVertex.h"
#include "StructMeta.h"
#include "RenderManager.h"

class TestVSShader : public VertexShader
{
public:
	DECLARE_VERTEXSHADER_START(TestVertex, pVin, TestVertex_v2p, pVout)
	{
		pVout->pos = pVin->pos;
		pVout->color = pVin->color;
	}
	DECLARE_VERTEXSHADER_END
};

class TestPSShader : public PixelShader
{
public:
	DECLARE_PIXELSHADER_START(TestVertex_v2p, pVout, TestPixel, pPout)
	{
		pPout->pos = pVout->pos;

		// 更新深度缓存
		if (RenderManager::GetInstance()->CheckCurState(StateMask_Alpha, StateMaskValue_NoAlpha)
			&& RenderManager::GetInstance()->CheckCurState(StateMask_DepthBuffer, StateMaskValue_UseDepth)) {
			auto& depth_buffer = RenderManager::GetInstance()->GetDepthBuffer();
			ScreenCoord coord_x = static_cast<ScreenCoord>(pVout->pos._x);
			ScreenCoord coord_y = static_cast<ScreenCoord>(pVout->pos._y);
			if (pVout->pos._z < depth_buffer->GetPixel_coordPos(coord_x, coord_y)) {
				*depth_buffer->pPixelAt(coord_x, coord_y) = pVout->pos._z;
			}
		}
		// 根据贴图更新颜色
		if (TextureManager::GetInstance()->GetTexture()) {
			pPout->color *= TextureManager::GetInstance()->GetTexture()->GetPixel_normedPos_smart(pVout->uv._x, pVout->uv._y);
		}
	}
	DECLARE_PIXELSHADER_END

	DECLARE_PIXELSHADER_TEST_START(TestVertex_v2p, pix)
	{
		// 排除近平面以前、远平面以后、相机背面
		if (pix->pos._z < 0 || pix->pos._z > 1) {
			return false;
		}
		// 排除屏幕以外
		ScreenCoord coord_x = static_cast<ScreenCoord>(pix->pos._x);
		ScreenCoord coord_y = static_cast<ScreenCoord>(pix->pos._y);
		if (coord_x < 0 || coord_x >= RenderManager::GetInstance()->GetWidth() || coord_y < 0 || coord_y >= RenderManager::GetInstance()->GetHeight()) {
			return false;
		}
		// 进行深度测试
		if (RenderManager::GetInstance()->CheckCurState(StateMask_Alpha, StateMaskValue_NoAlpha)
			&& RenderManager::GetInstance()->CheckCurState(StateMask_DepthBuffer, StateMaskValue_UseDepth)) {
			auto& depth_buffer = RenderManager::GetInstance()->GetDepthBuffer();
			if (pix->pos._z >= depth_buffer->GetPixel_coordPos(coord_x, coord_y)) {
				return false;
			}
		}
		return true;
	}
	DECLARE_PIXELSHADER_TEST_END
};

//class TestVSShader : public VertexShader
//{
//	DECLARE_VERTEXSHADER(TestVertex, pVin, TestVertex_v2p, pVout);
//};
//
//class TestPSShader : public PixelShader
//{
//	DECLARE_PIXELSHADER(TestVertex_v2p, pVout, TestPixel, pPout);
//};
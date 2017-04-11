#include "LightingShader.h"

#include "Color.h"
#include "RenderManager.h"
#include "CameraManager.h"
#include "TexBuffer.h"


DECLARE_VERTEXSHADER_START(TestVSShader, TestVertex, pVin, TestVertex_v2p, pVout)
{
	// uv照抄
	pVout->uv = pVin->uv;

	HomoPos tmp = modelMat.PreMulVec(pVin->pos.ToVec4Pos());
	WorldPos worldPos = tmp.ToVec3Homo();

	// 转换位置到世界空间
	HomoPos tmp2 = CameraManager::GetInstance()->CurrentCamera()->GetViewProjMat().PreMulVec(tmp);
	const auto& tmp22 = tmp2.ToVec3Homo();
	WorldPos screen_pos = CameraManager::GetInstance()->CurrentCamera()->TransToScreenPos(tmp22);
	pVout->pos = screen_pos;
	// 转换法线到世界空间
	pVout->normal = pVin->normal;
	pVout->normal.Normalise();
	auto tt = modelMat.PreMulVec(pVout->normal.ToVec4Dir());
	pVout->normal = modelMat.PreMulVec(pVout->normal.ToVec4Dir()).ToVec3();
	// 颜色使用光照
	pVout->color = LightManager::GetInstance()->Process(
		pVin->color,
		pVout->normal,
		worldPos);
}
DECLARE_VERTEXSHADER_END

DECLARE_PIXELSHADER_START(TestPSShader, TestVertex_v2p, pVout, TestPixel, pPout)
{
	pPout->pos = pVout->pos;
	pPout->color = pVout->color;

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

DECLARE_PIXELSHADER_TEST_START(TestPSShader, TestVertex_v2p, pix)
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
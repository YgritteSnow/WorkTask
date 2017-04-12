#include "LightingShader.h"

#include "Color.h"
#include "RenderManager.h"
#include "CameraManager.h"
#include "TexBuffer.h"


DECLARE_VERTEXSHADER_START(TestVSShader, TestVertex, pVin, TestVertex_v2p, pVout)
{
	// �ճ�
	pVout->uv = pVin->uv;
	pVout->color = pVin->color;

	// ת��λ�ã����� - �ӽ�&ͶӰ - ��Ļ
	HomoPos worldPos = modelMat.PreMulVec(pVin->pos.ToVec4Pos());
	HomoPos viewPos = CameraManager::GetInstance()->CurrentCamera()->GetViewProjMat().PreMulVec(worldPos);
	pVout->z = viewPos._w;

	WorldPos screenPos = CameraManager::GetInstance()->CurrentCamera()->TransToScreenPos(viewPos.ToVec3Homo());
	pVout->pos = screenPos;

	// ת�����ߵ�����ռ�
	pVout->normal = pVin->normal;
	pVout->normal = modelMat.PreMulVec(pVout->normal.ToVec4Dir()).ToVec3();
	// ת�����ߵ�����ռ�
	pVout->biNormal = pVin->biNormal;
	pVout->biNormal = modelMat.PreMulVec(pVout->biNormal.ToVec4Dir()).ToVec3();
	// ���㸱����
	pVout->taNormal = pVout->biNormal.CrossProduct(pVout->normal);

	// ��¼��������
	pVout->worldPos = worldPos.ToVec3Homo();

}
DECLARE_VERTEXSHADER_END

DECLARE_PIXELSHADER_START(TestPSShader, TestVertex_v2p, pVout, TestPixel, pPout)
{
	pPout->pos = pVout->pos;
	pVout->normal.Normalise();
	pVout->biNormal.Normalise();
	pVout->taNormal.Normalise();

	// ���ݷ�����ͼ΢������
	if (m_cur_normalTexture) {
		auto normal_color = m_cur_normalTexture->GetPixel_normedPos_smart(pVout->uv._x, pVout->uv._y);
		normal_color = (normal_color - 0.5) * 2;
		WorldPos normal_offset;
		normal_offset._x = pVout->biNormal._x * normal_color._x + pVout->taNormal._x * normal_color._y + pVout->normal._x * normal_color._z;
		normal_offset._y = pVout->biNormal._y * normal_color._x + pVout->taNormal._y * normal_color._y + pVout->normal._y * normal_color._z;
		normal_offset._z = pVout->biNormal._z * normal_color._x + pVout->taNormal._z * normal_color._y + pVout->normal._z * normal_color._z;
		normal_offset.Normalise();
		pVout->normal._x = normal_offset._x;
		pVout->normal._y = normal_offset._y;
		pVout->normal._z = normal_offset._z;
		pVout->normal.Normalise();
	}

	// ��ɫʹ�ù���
	pPout->color = LightManager::GetInstance()->Process(
		pVout->color,
		pVout->normal,
		pVout->worldPos);

	// ������Ȼ���
	if (RenderManager::GetInstance()->CheckCurState(StateMask_Alpha, StateMaskValue_NoAlpha)
		&& RenderManager::GetInstance()->CheckCurState(StateMask_DepthBuffer, StateMaskValue_UseDepth)) {
		auto& depth_buffer = RenderManager::GetInstance()->GetDepthBuffer();
		ScreenCoord coord_x = static_cast<ScreenCoord>(pVout->pos._x);
		ScreenCoord coord_y = static_cast<ScreenCoord>(pVout->pos._y);
		if (pVout->pos._z < depth_buffer->GetPixel_coordPos(coord_x, coord_y)) {
			*depth_buffer->pPixelAt(coord_x, coord_y) = pVout->pos._z;
		}
	}
	// ������ͼ������ɫ
	if (m_cur_texture) {
		pPout->color *= m_cur_texture->GetPixel_normedPos_smart(pVout->uv._x, pVout->uv._y);
	}
}
DECLARE_PIXELSHADER_END

DECLARE_PIXELSHADER_TEST_START(TestPSShader, TestVertex_v2p, pix)
{
	// �ų���ƽ����ǰ��Զƽ���Ժ��������
	if (pix->pos._z < 0 || pix->pos._z > 1) {
		return false;
	}
	// �ų���Ļ����
	ScreenCoord coord_x = static_cast<ScreenCoord>(pix->pos._x);
	ScreenCoord coord_y = static_cast<ScreenCoord>(pix->pos._y);
	if (coord_x < 0 || coord_x >= RenderManager::GetInstance()->GetWidth() || coord_y < 0 || coord_y >= RenderManager::GetInstance()->GetHeight()) {
		return false;
	}
	// ������Ȳ���
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
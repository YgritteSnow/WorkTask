#pragma once

#include "Model.h"
#include "Light.h"
#include "Material.h"
#include "TestSkinnedVertex.h"
#include "TestSkinnedMesh.h"

class TestSkinnedModel : public Model
{
public:
	void DummySnake(float radius, float length, float slice_count, float step_count, float radius_count) {
		m_vsMain = new TestVSShader();
		m_psMain = new TestPSShader();
		m_worldMat.SetTranslate(0, 0, 5);
		m_mesh = new TestSnakeMesh(radius, length, slice_count, step_count, radius_count);
		m_mesh->Load();
		m_cur_texture = TextureManager::GetInstance()->LoadTexture_norm("tex_alpha_color.tga");
		//m_cur_normalTexture = TextureManager::GetInstance()->LoadTexture_norm("tex_normal.tga");

		static_cast<TestVSShader*>(m_vsMain)->SetModelMat(m_worldMat);
		static_cast<TestPSShader*>(m_psMain)->SetDiffuseTexture(m_cur_texture);
		static_cast<TestPSShader*>(m_psMain)->SetNormalTexture(m_cur_normalTexture);
		static_cast<TestPSShader*>(m_psMain)->SetMaterial(MaterialManager::GetInstance()->CurMaterial());
		int light_count = LightManager::GetInstance()->Count();
		for (int i = 0; i < light_count; ++i) {
			static_cast<TestPSShader*>(m_psMain)->AddLight(LightManager::GetInstance()->GetLight(i));
		}
	}
public:
	void BeforeRender() {
		Model::BeforeRender();
		static_cast<TestVSShader*>(m_vsMain)->SetModelMat(m_worldMat);
	}
private:
	TexBuffer<NormColor4>* m_cur_texture;
	TexBuffer<NormColor4>* m_cur_normalTexture;
};
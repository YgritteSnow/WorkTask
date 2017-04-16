#pragma once

#include "SkinnedModel.h"
#include "Light.h"
#include "Material.h"
#include "TestSkinnedVertex.h"
#include "TestSkinnedMesh.h"
#include "LightingSkinnedShader.h"

class TestSkinnedModel : public SkinnedModel
{
public:
	void DummySnake(float radius, float length, float slice_count, float step_count, float radius_count) {
		m_vsMain = new TestSkinnedVSShader();
		m_psMain = new TestSkinnedPSShader();
		m_worldMat.SetTranslate(0, 0, 5);
		m_mesh = new TestSnakeMesh(radius, length, slice_count, step_count, radius_count);
		m_mesh->Load();
		m_cur_texture = TextureManager::GetInstance()->LoadTexture_norm("tex_alpha_color.tga");
		//m_cur_normalTexture = TextureManager::GetInstance()->LoadTexture_norm("tex_normal.tga");

		static_cast<TestSkinnedVSShader*>(m_vsMain)->SetModelMat(m_worldMat);
		static_cast<TestSkinnedPSShader*>(m_psMain)->SetDiffuseTexture(m_cur_texture);
		static_cast<TestSkinnedPSShader*>(m_psMain)->SetNormalTexture(m_cur_normalTexture);
		static_cast<TestSkinnedPSShader*>(m_psMain)->SetMaterial(MaterialManager::GetInstance()->CurMaterial());
		int light_count = LightManager::GetInstance()->Count();
		for (int i = 0; i < light_count; ++i) {
			static_cast<TestSkinnedPSShader*>(m_psMain)->AddLight(LightManager::GetInstance()->GetLight(i));
		}
	}

	void DummyBones(int bone_count) {
		m_vsMain = new TestSkinnedVSShader();
		m_psMain = new TestSkinnedPSShader();
		m_worldMat.SetTranslate(0, 0, 5);
		m_mesh = new TestSkeletonMesh(bone_count);
		m_mesh->Load();

		static_cast<TestSkinnedVSShader*>(m_vsMain)->SetModelMat(m_worldMat);
		static_cast<TestSkinnedPSShader*>(m_psMain)->SetMaterial(MaterialManager::GetInstance()->CurMaterial());
		int light_count = LightManager::GetInstance()->Count();
		for (int i = 0; i < light_count; ++i) {
			static_cast<TestSkinnedPSShader*>(m_psMain)->AddLight(LightManager::GetInstance()->GetLight(i));
		}
	}
public:
	void BeforeRender() {
		SkinnedModel::BeforeRender();
		static_cast<TestSkinnedVSShader*>(m_vsMain)->SetModelMat(m_worldMat);
	}
private:
	TexBuffer<NormColor4>* m_cur_texture;
	TexBuffer<NormColor4>* m_cur_normalTexture;
};
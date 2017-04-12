#pragma once

#include "TestVertex.h"
#include "Model.h"
#include "TestMesh.h"

class TestModel : public Model
{
public:
	void DummyData() {
		m_vsMain = new TestVSShader();
		m_psMain = new TestPSShader();
		m_worldMat.SetTranslate(0, 0, 5);
		m_mesh = new TestCubeMesh();
		m_mesh->Load();
		m_cur_texture = TextureManager::GetInstance()->LoadTexture_norm("tex_alpha_color.tga");
		//m_cur_normalTexture = TextureManager::GetInstance()->LoadTexture_norm("tex_normal.tga");
	}
	void DummyQuad(float width, float height, float dist) {
		m_vsMain = new TestVSShader();
		m_psMain = new TestPSShader();
		m_worldMat.SetTranslate(0, 0, dist);
		m_mesh = new TestQuadMesh(width, height);
		m_mesh->Load();
		m_cur_texture = TextureManager::GetInstance()->LoadTexture_norm("tex.tga");
		//m_cur_normalTexture = TextureManager::GetInstance()->LoadTexture_norm("tex_normal.tga");
	}
	void DummyBall(float radius, int h_count, int r_count, NormColor4 color, WorldPos pos) {
		m_vsMain = new TestVSShader();
		m_psMain = new TestPSShader();
		m_worldMat.SetTranslate(pos);
		m_mesh = new TestBallMesh(radius, h_count, r_count, color);
		m_mesh->Load();
		m_cur_texture = TextureManager::GetInstance()->LoadTexture_norm("tex.tga");
		m_cur_normalTexture = TextureManager::GetInstance()->LoadTexture_norm("tex_normal3.tga");
	}
	void DummyGround(float width, float height, NormColor4 color, WorldPos pos) {
		m_vsMain = new TestVSShader();
		m_psMain = new TestPSShader();
		m_worldMat.SetTranslate(pos);
		m_mesh = new TestGroundMesh(width, height, color);
		m_mesh->Load();
		m_cur_texture = TextureManager::GetInstance()->LoadTexture_norm("tex_normal.tga");
		m_cur_normalTexture = TextureManager::GetInstance()->LoadTexture_norm("tex_normal3.tga");
	}
public:
	void BeforeRender() {
		Model::BeforeRender();
		static_cast<TestVSShader*>(m_vsMain)->SetModelMat(m_worldMat);
		static_cast<TestPSShader*>(m_psMain)->SetTexture(m_cur_texture);
		static_cast<TestPSShader*>(m_psMain)->SetNormalTexture(m_cur_normalTexture);
	}
private:
	TexBuffer<NormColor4>* m_cur_texture;
	TexBuffer<NormColor4>* m_cur_normalTexture;
};
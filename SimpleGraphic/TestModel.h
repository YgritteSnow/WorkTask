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
	}
	void DummyQuad(float width, float height, float dist) {
		m_vsMain = new TestVSShader();
		m_psMain = new TestPSShader();
		m_worldMat.SetTranslate(0, 0, dist);
		m_mesh = new TestQuadMesh(width, height);
		m_mesh->Load();
	}
	void DummyBall(float radius, int h_count, int r_count, NormColor4 color, WorldPos pos) {
		m_vsMain = new TestVSShader();
		m_psMain = new TestPSShader();
		m_worldMat.SetTranslate(pos);
		m_mesh = new TestBallMesh(radius, h_count, r_count, color);
		m_mesh->Load();
	}
	void DummyGround(float width, float height, NormColor4 color, WorldPos pos) {
		m_vsMain = new TestVSShader();
		m_psMain = new TestPSShader();
		m_worldMat.SetTranslate(pos);
		m_mesh = new TestGroundMesh(width, height, color);
		m_mesh->Load();
	}
public:
	void BeforeRender() {
		Model::BeforeRender();
		static_cast<TestVSShader*>(m_vsMain)->SetModelMat(m_worldMat);
	}
};
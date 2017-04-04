#pragma once

#include "TestVertex.h"
#include "Model.h"
#include "TestMesh.h"

class TestModel : public Model
{
public:
	void DummyData() {
		m_worldMat.SetTranslate(0, 0, 5);
		m_mesh = new TestCubeMesh();
		m_mesh->Load();
		m_vsID = 0; m_psID = 0;
	}
	void DummyQuad() {
		m_worldMat.SetTranslate(0, 0, 0);
		m_mesh = new TestQuadMesh();
		m_mesh->Load();
		m_vsID = 0; m_psID = 0;
	}
	void DummyBall(float radius, int h_count, int r_count, NormColor4 color, WorldPos pos) {
		m_worldMat.SetTranslate(pos);
		m_mesh = new TestBallMesh(radius, h_count, r_count, color);
		m_mesh->Load();
		m_vsID = 0; m_psID = 0;
	}
	void DummyGround(float width, float height, NormColor4 color, WorldPos pos) {
		m_worldMat.SetTranslate(pos);
		m_mesh = new TestGroundMesh(width, height, color);
		m_mesh->Load();
		m_vsID = 0; m_psID = 0;
	}
};
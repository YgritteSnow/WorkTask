#pragma once
#include "StructMeta.h"
#include "Mesh.h"
#include "VertexUtilities.h"

class TestCubeMesh : public Mesh
{
private:
	virtual void OnLoad() override {
		Dummy();
		ProcessVertex_calNormal(
			static_cast<TestVertex*>(static_cast<void*>(m_vertexBuffer->m_vec_vertex)),
			static_cast<TestVertex*>(static_cast<void*>(m_vertexBuffer->m_vec_vertex)),
			m_vertexBuffer->m_length,
			m_indexBuffer->m_vec_indice,
			m_indexBuffer->m_length);
		ProcessVertex_calBiNormal(
			static_cast<TestVertex*>(static_cast<void*>(m_vertexBuffer->m_vec_vertex)),
			static_cast<TestVertex*>(static_cast<void*>(m_vertexBuffer->m_vec_vertex)),
			m_vertexBuffer->m_length,
			m_indexBuffer->m_vec_indice,
			m_indexBuffer->m_length);
	}
	void Dummy() {
		m_vertexBuffer = new VertexBuffer(GetID(TestVertex), 4);
		m_indexBuffer = new IndexBuffer(12);

		m_vertexBuffer->SetVertex(0, TestVertex(-1, -1, -1, 1, 0, 0, 1));
		m_vertexBuffer->SetVertex(1, TestVertex(-1,  1, -1, 1, 1, 0, 1));
		m_vertexBuffer->SetVertex(2, TestVertex(-1,  1,  1, 0, 0, 1, 1));
		m_vertexBuffer->SetVertex(3, TestVertex( 1,  1,  1, 0, 1, 1, 1));

		m_indexBuffer->m_vec_indice[0] = 0;
		m_indexBuffer->m_vec_indice[1] = 1;
		m_indexBuffer->m_vec_indice[2] = 2;
		m_indexBuffer->m_vec_indice[3] = 2;
		m_indexBuffer->m_vec_indice[4] = 1;
		m_indexBuffer->m_vec_indice[5] = 3;
		m_indexBuffer->m_vec_indice[6] = 3;
		m_indexBuffer->m_vec_indice[7] = 1;
		m_indexBuffer->m_vec_indice[8] = 0;
		m_indexBuffer->m_vec_indice[9] = 3;
		m_indexBuffer->m_vec_indice[10] = 0;
		m_indexBuffer->m_vec_indice[11] = 2;
	}
};

class TestBallMesh : public Mesh
{
public:
	TestBallMesh(float radius, int h_count, int r_count, NormColor4 color)
		: m_radius(radius)
		, m_h_count(h_count)
		, m_r_count(r_count)
		, m_color(color)
	{}
private:
	virtual void OnLoad() override {
		Dummy();
		ProcessVertex_calNormal(
			static_cast<TestVertex*>(static_cast<void*>(m_vertexBuffer->m_vec_vertex)),
			static_cast<TestVertex*>(static_cast<void*>(m_vertexBuffer->m_vec_vertex)),
			m_vertexBuffer->m_length,
			m_indexBuffer->m_vec_indice,
			m_indexBuffer->m_length);
		ProcessVertex_calBiNormal(
			static_cast<TestVertex*>(static_cast<void*>(m_vertexBuffer->m_vec_vertex)),
			static_cast<TestVertex*>(static_cast<void*>(m_vertexBuffer->m_vec_vertex)),
			m_vertexBuffer->m_length,
			m_indexBuffer->m_vec_indice,
			m_indexBuffer->m_length);
	}
	void Dummy() {
		std::vector<TestVertex> t_vertex;
		std::vector<DWORD> t_index;

		// 横切为 m_h_count 层圆片，每个圆片分为 m_r_count 个扇形
		// 至少应该用 2 层圆片，4 个扇形
		if (m_h_count < 2 || m_r_count < 4) {
			m_h_count = 2; m_r_count = 4;
		}

		// --------------- 计算顶点 ------------
		// 圆片的下底高度
		float lo_y = -m_radius;
		// 高度步进
		float h_step = m_radius * 2 / m_h_count;

		// 第一层只有一个点
		WorldPos point = WorldPos(0, lo_y, 0);
		t_vertex.push_back(TestVertex(point, m_color, point));
		// 从第二层开始进行每周 m_r_count 个点的 push
		for (int i = 1; i < m_h_count; ++i) {
			lo_y += h_step;
			float i_radius = std::sqrt(m_radius*m_radius - lo_y*lo_y);
			for (int j = 0; j < m_r_count; ++j) {
				float ang = j * 2 * JMath::PI / m_r_count;
				point._x = std::cos(ang) * i_radius;
				point._y = lo_y;
				point._z = std::sin(ang) * i_radius;
				t_vertex.push_back(TestVertex(point, m_color, point, UVPos((std::cos(ang) + 1) / 2, lo_y / m_radius / 2 + 0.5)));
			}
		}
		// 最后一层只有一个点
		lo_y += h_step;
		point = WorldPos(0, lo_y, 0);
		t_vertex.push_back(TestVertex(point, m_color, point));

		// --------------- 计算索引 ------------
		// 第一层，和圆片的 m_r_count 个点连接
		int idx_start = 1;
		for (int j = 0; j < m_r_count - 1; ++j) {
			t_index.push_back(0);
			t_index.push_back(idx_start + j);
			t_index.push_back(idx_start + j + 1);
		}
		t_index.push_back(0);
		t_index.push_back(idx_start + m_r_count - 1);
		t_index.push_back(idx_start);

		// 圆片部分
		for (int i = 1; i < m_h_count - 1; ++i) {
			int idx_start_lo = 1 + m_r_count * (i - 1);
			int idx_start_hi = 1 + m_r_count * i;
			int j;
			for (j = 0; j < m_r_count - 1; ++j) {
				t_index.push_back(idx_start_lo + j);
				t_index.push_back(idx_start_hi + j);
				t_index.push_back(idx_start_hi + j + 1);

				t_index.push_back(idx_start_lo + j);
				t_index.push_back(idx_start_hi + j + 1);
				t_index.push_back(idx_start_lo + j + 1);
			}
			t_index.push_back(idx_start_lo + j);
			t_index.push_back(idx_start_hi + j);
			t_index.push_back(idx_start_hi);

			t_index.push_back(idx_start_lo + j);
			t_index.push_back(idx_start_hi);
			t_index.push_back(idx_start_lo);
		}

		// 最后一层，和圆片的 m_r_count 个点连接
		idx_start = 1 + (m_h_count - 2)*m_r_count;
		int idx_last = 1 + (m_h_count - 1)*m_r_count;
		for (int j = 0; j < m_r_count - 1; ++j) {
			t_index.push_back(idx_start + j);
			t_index.push_back(idx_last);
			t_index.push_back(idx_start + j + 1);
		}
		t_index.push_back(idx_start + m_r_count - 1);
		t_index.push_back(idx_last);
		t_index.push_back(idx_start);

		// 这里转换一下上边的数据
		m_vertexBuffer = new VertexBuffer(GetID(TestVertex), t_vertex.size());
		m_indexBuffer = new IndexBuffer(t_index.size());
		for (int i = 0; i < t_vertex.size(); ++i) {
			m_vertexBuffer->SetVertex(i, t_vertex[i]);
		}
		for (int i = 0; i < t_index.size(); ++i) {
			m_indexBuffer->m_vec_indice[i] = t_index[i];
		}
	}
private:
	float m_radius;
	int m_h_count;
	int m_r_count;
	NormColor4 m_color;
};


class TestQuadMesh : public Mesh
{
public:
	TestQuadMesh(float w, float h)
		: m_width(w), m_height(h)
	{}
private:
	void OnLoad() override {
		Dummy();
		ProcessVertex_calNormal(
			static_cast<TestVertex*>(static_cast<void*>(m_vertexBuffer->m_vec_vertex)),
			static_cast<TestVertex*>(static_cast<void*>(m_vertexBuffer->m_vec_vertex)),
			m_vertexBuffer->m_length,
			m_indexBuffer->m_vec_indice,
			m_indexBuffer->m_length);
		ProcessVertex_calBiNormal(
			static_cast<TestVertex*>(static_cast<void*>(m_vertexBuffer->m_vec_vertex)),
			static_cast<TestVertex*>(static_cast<void*>(m_vertexBuffer->m_vec_vertex)),
			m_vertexBuffer->m_length,
			m_indexBuffer->m_vec_indice,
			m_indexBuffer->m_length);
	}
	void Dummy() {
		std::vector<TestVertex> t_vertex;
		std::vector<DWORD> t_index;

		t_vertex.push_back(TestVertex(WorldPos(-m_width, -m_height, 0), NormColor4(1, 0, 0, 1), WorldPos(0, 0, -1), UVPos(0, 0)));
		t_vertex.push_back(TestVertex(WorldPos(-m_width, m_height, 0), NormColor4(0, 1, 0, 1), WorldPos(0, 0, -1), UVPos(0, 1)));
		t_vertex.push_back(TestVertex(WorldPos(m_width, m_height, 0), NormColor4(0, 0, 1, 1), WorldPos(0, 0, -1), UVPos(1, 1)));
		t_vertex.push_back(TestVertex(WorldPos(m_width, -m_height, 0), NormColor4(1, 1, 1, 1), WorldPos(0, 0, -1), UVPos(1, 0)));

		t_index.push_back(0);
		t_index.push_back(2);
		t_index.push_back(1);
		t_index.push_back(0);
		t_index.push_back(3);
		t_index.push_back(2);

		// 这里转换一下上边的数据
		m_vertexBuffer = new VertexBuffer(GetID(TestVertex), t_vertex.size());
		m_indexBuffer = new IndexBuffer(t_index.size());
		for (int i = 0; i < t_vertex.size(); ++i) {
			m_vertexBuffer->SetVertex(i, t_vertex[i]);
		}
		for (int i = 0; i < t_index.size(); ++i) {
			m_indexBuffer->m_vec_indice[i] = t_index[i];
		}
	}
private:
	float m_width;
	float m_height;
};

class TestGroundMesh : public Mesh
{
public:
	TestGroundMesh(float width, float height, NormColor4 color)
		: m_width(width)
		, m_height(height)
		, m_color(color)
	{}

	void OnLoad() override {
		Dummy();
		ProcessVertex_calNormal(
			static_cast<TestVertex*>(static_cast<void*>(m_vertexBuffer->m_vec_vertex)),
			static_cast<TestVertex*>(static_cast<void*>(m_vertexBuffer->m_vec_vertex)),
			m_vertexBuffer->m_length,
			m_indexBuffer->m_vec_indice,
			m_indexBuffer->m_length);
		ProcessVertex_calBiNormal(
			static_cast<TestVertex*>(static_cast<void*>(m_vertexBuffer->m_vec_vertex)),
			static_cast<TestVertex*>(static_cast<void*>(m_vertexBuffer->m_vec_vertex)),
			m_vertexBuffer->m_length,
			m_indexBuffer->m_vec_indice,
			m_indexBuffer->m_length);
	}
	void Dummy() {
		float width = m_width;
		float height = m_height;
		NormColor4 c = m_color;

		std::vector<TestVertex> t_vertex;
		std::vector<DWORD> t_index;

		WorldPos center;
		t_vertex.push_back(TestVertex(WorldPos(center._x - width, center._y, center._z - height), c, WorldPos(0, 1, 0), UVPos(0, 0)));
		t_vertex.push_back(TestVertex(WorldPos(center._x + width, center._y, center._z - height), c, WorldPos(0, 1, 0), UVPos(1, 0)));
		t_vertex.push_back(TestVertex(WorldPos(center._x + width, center._y, center._z + height), c, WorldPos(0, 1, 0), UVPos(1, 1)));
		t_vertex.push_back(TestVertex(WorldPos(center._x - width, center._y, center._z + height), c, WorldPos(0, 1, 0), UVPos(0, 1)));

		t_index.push_back(0);
		t_index.push_back(1);
		t_index.push_back(2);
		t_index.push_back(0);
		t_index.push_back(2);
		t_index.push_back(3);

		// 这里转换一下上边的数据
		m_vertexBuffer = new VertexBuffer(GetID(TestVertex), t_vertex.size());
		m_indexBuffer = new IndexBuffer(t_index.size());
		for (int i = 0; i < t_vertex.size(); ++i) {
			m_vertexBuffer->SetVertex(i, t_vertex[i]);
		}
		for (int i = 0; i < t_index.size(); ++i) {
			m_indexBuffer->m_vec_indice[i] = t_index[i];
		}
	}
private:
	float m_width;
	float m_height;
	NormColor4 m_color;
};
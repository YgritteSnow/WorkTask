#ifndef __MODEL_H__
#define __MODEL_H__

#include <Windows.h>
#include <vector>
#include <cmath>

#include "JMath.h"
#include "Color.h"
#include "Material.h"

/* *********************************************
* Model
* *********************************************/
struct DummyVertex{
	DummyVertex() :pos(), color(), normal(0, 0, 1){}
	DummyVertex(float x, float y, float z)
		:pos(x, y, z)
		, color()
		, normal(0, 0, 1)
	{}
	DummyVertex(float x, float y, float z, float r, float g, float b, float a)
		:pos(x, y, z)
		, color(r, g, b, a)
		, normal(0, 0, 1)
	{}
	DummyVertex(float x, float y, float z, const NormColor4& c)
		:pos(x, y, z)
		, color(c)
		, normal(0, 0, 1)
	{}
	DummyVertex(const WorldPos& p, const NormColor4& c, const WorldPos& n)
		:pos(p)
		, color(c)
		, normal(n)
	{}
	DummyVertex(const WorldPos& p, const NormColor4& c, const WorldPos& n, const UVPos& u)
		:pos(p)
		, color(c)
		, normal(n)
		, uv(u)
	{}
	WorldPos pos;
	NormColor4 color;
	WorldPos normal;
	UVPos uv;
};

template <typename VertexStruct>
class Model{
public:
	void DummyData() {
		m_worldMat.SetTranslate(0, 0, 5);

		m_vec_vertex.push_back(VertexStruct(-1, -1, -1, 1, 0, 0, 1));
		m_vec_vertex.push_back(VertexStruct(-1, 1, -1, 1, 1, 0, 1));
		m_vec_vertex.push_back(VertexStruct(-1, 1, 1, 0, 0, 1, 1));
		m_vec_vertex.push_back(VertexStruct(1, 1, 1, 0, 1, 1, 1));

		m_vec_indice.push_back(0);
		m_vec_indice.push_back(1);
		m_vec_indice.push_back(2);
		m_vec_indice.push_back(2);
		m_vec_indice.push_back(1);
		m_vec_indice.push_back(3);
		m_vec_indice.push_back(3);
		m_vec_indice.push_back(1);
		m_vec_indice.push_back(0);
		m_vec_indice.push_back(3);
		m_vec_indice.push_back(0);
		m_vec_indice.push_back(2);
	}
	void DummyBall(float radius, int h_count, int r_count, NormColor4 color){
		// ����Ϊ h_count ��ԲƬ��ÿ��ԲƬ��Ϊ r_count ������
		// ����Ӧ���� 2 ��ԲƬ��4 ������
		if (h_count < 2 || r_count < 4){
			DummyData();
			return;
		}

		m_worldMat.SetTranslate(0, 0, 5);
		m_vec_vertex.clear();
		m_vec_indice.clear();

		// --------------- ���㶥�� ------------
		// ԲƬ���µ׸߶�
		float lo_y = -radius;
		// �߶Ȳ���
		float h_step = radius * 2 / h_count;

		// ��һ��ֻ��һ����
		WorldPos point = WorldPos(0, lo_y, 0);
		m_vec_vertex.push_back(VertexStruct(point, color, point));
		// �ӵڶ��㿪ʼ����ÿ�� r_count ����� push
		for (int i = 1; i < h_count; ++i){
			lo_y += h_step;
			float i_radius = std::sqrt(radius*radius - lo_y*lo_y);
			for (int j = 0; j < r_count; ++j){
				float ang = j * 2 * JMath::PI / r_count;
				point._x = std::cos(ang) * i_radius;
				point._y = lo_y;
				point._z = std::sin(ang) * i_radius;
				//m_vec_vertex.push_back(VertexStruct(point, NormColor4((std::cos(ang)+1)/2, (std::sin(ang)+1)/2, 1, 1), point));
				m_vec_vertex.push_back(VertexStruct(point, color, point, UVPos((std::cos(ang) + 1) / 2, lo_y/radius/2 + 0.5)));
			}
		}
		// ���һ��ֻ��һ����
		lo_y += h_step;
		point = WorldPos(0, lo_y, 0);
		m_vec_vertex.push_back(VertexStruct(point, color, point));

		// --------------- �������� ------------
		// ��һ�㣬��ԲƬ�� r_count ��������
		int idx_start = 1;
		for (int j = 0; j < r_count-1; ++j){
			m_vec_indice.push_back(0);
			m_vec_indice.push_back(idx_start + j);
			m_vec_indice.push_back(idx_start + j + 1);
		}
		m_vec_indice.push_back(0);
		m_vec_indice.push_back(idx_start + r_count - 1);
		m_vec_indice.push_back(idx_start);

		// ԲƬ����
		for (int i = 1; i < h_count - 1; ++i){
			int idx_start_lo = 1 + r_count * (i - 1);
			int idx_start_hi = 1 + r_count * i;
			int j;
			for (j = 0; j < r_count - 1; ++j){
				m_vec_indice.push_back(idx_start_lo + j);
				m_vec_indice.push_back(idx_start_hi + j);
				m_vec_indice.push_back(idx_start_hi + j + 1);

				m_vec_indice.push_back(idx_start_lo + j);
				m_vec_indice.push_back(idx_start_hi + j + 1);
				m_vec_indice.push_back(idx_start_lo + j + 1);
			}
			m_vec_indice.push_back(idx_start_lo + j);
			m_vec_indice.push_back(idx_start_hi + j);
			m_vec_indice.push_back(idx_start_hi);

			m_vec_indice.push_back(idx_start_lo + j);
			m_vec_indice.push_back(idx_start_hi);
			m_vec_indice.push_back(idx_start_lo);
		}

		// ���һ�㣬��ԲƬ�� r_count ��������
		idx_start = 1 + (h_count - 2)*r_count;
		int idx_last = 1 + (h_count-1)*r_count;
		for (int j = 0; j < r_count - 1; ++j){
			m_vec_indice.push_back(idx_start + j);
			m_vec_indice.push_back(idx_start + j + 1);
			m_vec_indice.push_back(idx_last);
		}
		m_vec_indice.push_back(idx_start + r_count - 1);
		m_vec_indice.push_back(idx_start);
		m_vec_indice.push_back(idx_last);
	}
	void DummyQuad(float width, float height) {
		m_worldMat.SetTranslate(0, 0, 5);

		m_vec_vertex.push_back(VertexStruct(WorldPos(-1, -1, 0), NormColor4(1, 0,0, 1), WorldPos(0, 0, -1), UVPos(0, 0)));
		m_vec_vertex.push_back(VertexStruct(WorldPos(-1, 1, 0), NormColor4(0, 1, 0, 1), WorldPos(0, 0, -1), UVPos(0, 1)));
		m_vec_vertex.push_back(VertexStruct(WorldPos(1, 1, 0), NormColor4(0, 0, 1, 1), WorldPos(0, 0, -1), UVPos(1, 1)));
		m_vec_vertex.push_back(VertexStruct(WorldPos(1, -1, 0), NormColor4(1, 1, 1, 1), WorldPos(0, 0, -1), UVPos(1, 0)));

		m_vec_indice.push_back(0);
		m_vec_indice.push_back(1);
		m_vec_indice.push_back(2);
		m_vec_indice.push_back(0);
		m_vec_indice.push_back(2);
		m_vec_indice.push_back(3);
	}
	VertexStruct* GetVertexes(){ return m_vec_vertex.size() > 0 ? &(m_vec_vertex[0]) : nullptr; }
	DWORD* GetIndices(){ return m_vec_indice.size() > 0 ? &(m_vec_indice[0]) : nullptr; }
	std::size_t GetVertexCount(){ return m_vec_vertex.size(); }
	std::size_t GetIndiceCount(){ return m_vec_indice.size(); }
	JMath::Mat44 GetModelMat(){ return m_worldMat; }

	void RotateXYZ(float x, float y, float z){
		m_worldMat = m_worldMat.RotateXYZ(x, y, z);
	}
	void Translate(float x, float y, float z){
		m_worldMat = m_worldMat.Translate(x, y, z);
	}
private:
	// ģ�͵��������
	JMath::Mat44 m_worldMat;
	// ģ�͵Ķ��������
	std::vector<VertexStruct> m_vec_vertex;
	// ģ�͵�����������
	std::vector<DWORD> m_vec_indice;
};

#endif
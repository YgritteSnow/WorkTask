#pragma once
#include "StructMeta.h"
#include "SkinnedMesh.h"
#include "VertexUtilities.h"
#include "TestSkinnedVertex.h"
#include "Animator.h"

class TestSkeletonMesh : public SkinnedMesh
{
public:
	TestSkeletonMesh(int bone_count)
		: m_bones_count(bone_count)
	{}

private:
	virtual void OnLoad() override {
		if (m_bones_count < 1)return;
		m_vertexBuffer = new VertexBuffer(GetID(TestSkinnedVertex), m_bones_count);
		m_indexBuffer = new IndexBuffer((m_bones_count-1) * 3);

		for (int i = 0; i < m_bones_count; ++i) {
			m_vertexBuffer->SetVertex(i, TestSkinnedVertex(0, i * 0.2f, 0, 0, 0, VertexBone(i, 1.0f)));
			if (i < m_bones_count - 1) {
				m_indexBuffer->m_vec_indice[i * 3] = i;
				m_indexBuffer->m_vec_indice[i * 3 + 1] = i;
				m_indexBuffer->m_vec_indice[i * 3 + 2] = i + 1;
			}
		}

		m_animator = new Animator(0, 0);
		m_animator->Start();
	}
private:
	int m_bones_count;
};

class TestSnakeMesh : public SkinnedMesh
{
public:
	TestSnakeMesh(float radius, float length, float slice_count, float step_count, float radius_count)
		: m_radius(radius)
		, m_length(length)
		, m_sliceCount(slice_count)
		, m_stepCount(step_count)
		, m_radiusCount(radius_count)
	{}
private:
	virtual void OnLoad() override {
		Dummy();
		ProcessVertex_calNormal(
			static_cast<TestSkinnedVertex*>(static_cast<void*>(m_vertexBuffer->m_vec_vertex)),
			static_cast<TestSkinnedVertex*>(static_cast<void*>(m_vertexBuffer->m_vec_vertex)),
			m_vertexBuffer->m_length,
			m_indexBuffer->m_vec_indice,
			m_indexBuffer->m_length);
		ProcessVertex_calBiNormal(
			static_cast<TestSkinnedVertex*>(static_cast<void*>(m_vertexBuffer->m_vec_vertex)),
			static_cast<TestSkinnedVertex*>(static_cast<void*>(m_vertexBuffer->m_vec_vertex)),
			m_vertexBuffer->m_length,
			m_indexBuffer->m_vec_indice,
			m_indexBuffer->m_length);
	}
	void Dummy() {
		std::vector<TestSkinnedVertex> tmp_vertex;
		std::vector<DWORD> tmp_indice;

		// 计算顶点
		float radius_step = JMath::PI_M2 / m_radiusCount;
		float length_step = m_length / m_stepCount;
		float cur_height = 0;
		// 每一截
		for (int slice_i = 0; slice_i < m_sliceCount; ++slice_i) {
			float bottom_height = slice_i * m_length;
			float top_height = bottom_height + m_length;
			// 每一层高度
			for (cur_height = bottom_height; cur_height < top_height - length_step/2; cur_height += length_step) {
				// 每一个角度
				for (float radius_i = 0; radius_i < JMath::PI_M2 - radius_step/2; radius_i += radius_step) {
					tmp_vertex.push_back(TestSkinnedVertex(m_radius * std::cos(radius_i), cur_height, m_radius * std::sin(radius_i)
						, radius_i/JMath::PI_M2, cur_height / m_length / m_sliceCount));
				}
			}
		}
		// 补上最后一层
		for (float radius_i = 0; radius_i < JMath::PI_M2 - radius_step/2; radius_i += radius_step) {
			tmp_vertex.push_back(TestSkinnedVertex(m_radius * std::cos(radius_i), cur_height, m_radius * std::sin(radius_i)
				, radius_i / JMath::PI_M2, cur_height / m_length / m_sliceCount));
		}

		// 计算索引
		int row_count = m_radiusCount;
		int height_count = m_sliceCount * m_stepCount;
		int aaa = 0;
		for(int height_i = 0; height_i < height_count; ++height_i){
			int row_i = height_i * row_count;
			int row_dest = row_i + row_count;
			for (; row_i < row_dest - 1; ++row_i) {
				tmp_indice.push_back(row_i);
				tmp_indice.push_back(row_i + 1);
				tmp_indice.push_back(row_i + row_count);

				tmp_indice.push_back(row_i + 1);
				tmp_indice.push_back(row_i + 1 + row_count);
				tmp_indice.push_back(row_i + row_count);
				aaa += 6;
			}
			tmp_indice.push_back(row_i);
			tmp_indice.push_back(row_i + 1 - row_count);
			tmp_indice.push_back(row_i + row_count);

			tmp_indice.push_back(row_i + 1 - row_count);
			tmp_indice.push_back(row_i + 1);
			tmp_indice.push_back(row_i + row_count);
			aaa += 6;
		}

		// 转换一下上边的数据
		m_vertexBuffer = new VertexBuffer(GetID(TestSkinnedVertex), tmp_vertex.size());
		m_indexBuffer = new IndexBuffer(tmp_indice.size());
		for (int i = 0; i < tmp_vertex.size(); ++i) {
			m_vertexBuffer->SetVertex(i, tmp_vertex[i]);
		}
		aaa = 0;
		for (int i = 0; i < tmp_indice.size(); ++i) {
			aaa += 1;
			m_indexBuffer->m_vec_indice[i] = tmp_indice[i];
		}

	}
private:
	float m_radius;		// 半径
	float m_length;		// 每截长度
	int m_sliceCount;		// 几截
	int m_stepCount;	// 每一截分成几份
	int m_radiusCount;	// 弧度分成几份
};
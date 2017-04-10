#pragma once

#include <Windows.h>
#include <vector>
#include <cmath>

#include "JMath.h"
#include "Color.h"
#include "ABuffer.h"
#include "Material.h"
#include "VertexBuffer.h"

/* *********************************************
* Mesh
* *********************************************/

class Mesh {
public:
	Mesh() {
		m_vertexBuffer = nullptr;
		m_indexBuffer = nullptr;
	}

	// 这里的参数以后应该至少提供资源的handle
	virtual void Load() { OnLoad(); }

	VertexBuffer* GetVertexBuffer() { return m_vertexBuffer; }
	IndexBuffer* GetIndexBuffer() { return m_indexBuffer; }

protected:
	virtual void OnLoad() = 0;
	VertexBuffer* m_vertexBuffer;// 顶点缓存
	IndexBuffer* m_indexBuffer;// 索引缓存
};
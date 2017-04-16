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
	Mesh();
	~Mesh();

	// todo 这里的参数以后应该至少提供资源的handle
	virtual void Load();
	virtual void BeforeRender() {}
	VertexBuffer* GetVertexBuffer();
	IndexBuffer* GetIndexBuffer();

protected:
	virtual void OnLoad() = 0;
	VertexBuffer* m_vertexBuffer;	// 顶点缓存
	IndexBuffer* m_indexBuffer;		// 索引缓存
};
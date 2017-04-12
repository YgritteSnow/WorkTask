#pragma once

#include <tchar.h>

extern unsigned int WINDOW_WIDTH;
extern unsigned int WINDOW_HEIGHT;

class DebugManager
{
private:
	DebugManager();

public:
	~DebugManager() {}
	static bool Init();
	static void UnInit();
	static DebugManager* GetInstance() { return m_instance; }

	void Render();

	void AddValidTriangleCount(unsigned int c);
	void AddTriangleCount(unsigned int c);
	void AddVertexCount(unsigned int c);
	
	void SetLightCount(unsigned int c);

	void OnRenderBegin();

	void SetFPS(float c);
private:
	static DebugManager* m_instance;

private:
	unsigned int m_lineStart;
	unsigned int m_lineHeight;

	unsigned int m_triangleCount;
	unsigned int m_triangleCount_valid;
	unsigned int m_vertexCount;
	float m_fps;
	unsigned int m_lightCount;
};
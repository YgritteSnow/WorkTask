#include "DebugGUI.h"

#include <tchar.h>
#include "BitMap.h"

DebugManager* DebugManager::m_instance = nullptr;

bool DebugManager::Init() {
	m_instance = new DebugManager;
	if (!m_instance) {
		return false;
	}
	return true;
}

void DebugManager::UnInit() {
	if (m_instance) {
		delete m_instance;
		m_instance = nullptr;
	}
}

DebugManager::DebugManager()
	: m_fps(1.f)
	, m_triangleCount(0)
	, m_triangleCount_valid(0)
	, m_vertexCount(0)
	, m_lineHeight(14)
	, m_lineStart(10)
{}

void DebugManager::Render() {
	TCHAR tmp[128];
	char tmp_char[128];
	ScreenCoord height = 0;

	wsprintf(tmp, _T("Primitive Count:  %d/%d"), m_triangleCount_valid, m_triangleCount);
	BitMap::DisplayText(tmp, ShortColor4(100, 100, 100, 100), ScreenPos(m_lineStart, height, 0));
	height += m_lineHeight;

	wsprintf(tmp, _T("Vertex Count:  %d"), m_vertexCount);
	BitMap::DisplayText(tmp, ShortColor4(100, 100, 100, 100), ScreenPos(m_lineStart, height, 0));
	height += m_lineHeight;

#ifdef _UNICODE
	sprintf_s(tmp_char, "FPS:  %.2f", m_fps);
	mbstowcs_s(nullptr, tmp, tmp_char, strlen(tmp_char));
#else
	sprintf_s(tmp, "FPS:  %.2f", m_fps);
#endif
	BitMap::DisplayText(tmp, ShortColor4(100, 100, 100, 100), ScreenPos(m_lineStart, height, 0));
	height += m_lineHeight;
}

void DebugManager::OnRenderBegin() {
	m_triangleCount_valid = 0;
	m_triangleCount = 0;
	m_vertexCount = 0;
}
void DebugManager::AddValidTriangleCount(unsigned int c) {
	m_triangleCount_valid += c;
}
void DebugManager::AddTriangleCount(unsigned int c) {
	m_triangleCount += c;
}
void DebugManager::AddVertexCount(unsigned int c) {
	m_vertexCount += c;
}
void DebugManager::SetFPS(float c) {
	m_fps = c * 0.1f + m_fps * 0.9f;
}
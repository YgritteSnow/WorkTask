#include "ShaderManager.h"

ShaderManager* ShaderManager::m_instance = nullptr;

ShaderManager::ShaderManager()
	: m_psMain(nullptr)
	, m_vsMain(nullptr)
{}
ShaderManager::~ShaderManager() {
	if (m_psMain) {
		delete m_psMain;
		m_psMain = nullptr;
	}
	if (m_vsMain) {
		delete m_vsMain;
		m_vsMain = nullptr;
	}
}

bool ShaderManager::Init() {
	if (!m_instance) {
		m_instance = new ShaderManager();
	}
	if (!m_instance) {
		return false;
	}
	return true;
}

void ShaderManager::UnInit() {
	if (m_instance) {
		delete m_instance;
		m_instance = nullptr;
	}
}

ShaderManager* ShaderManager::GetInstance() {
	return m_instance;
}
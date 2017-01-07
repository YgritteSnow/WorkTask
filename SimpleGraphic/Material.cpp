#include "Material.h"

MaterialManager* MaterialManager::m_instance = nullptr;

bool MaterialManager::Init() {
	m_instance = new MaterialManager;
	if (m_instance == nullptr) {
		return false;
	}
	return true;
}

void MaterialManager::UnInit() {
	if (m_instance) {
		delete m_instance;
		m_instance = nullptr;
	}
}

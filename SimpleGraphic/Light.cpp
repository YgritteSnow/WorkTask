#include "Light.h"

/* *********************************************
* Light
* *********************************************/
NormColor4 AmbientLight::ProcessColor(NormColor4 origin_color, WorldPos normal, WorldPos pos){
	return origin_color * this->m_light_color;
}

NormColor4 DirectLight::ProcessColor(NormColor4 origin_color, WorldPos normal, WorldPos pos){
	auto tt = normal.DotProduct(m_direct);
	tt = saturate(tt);
	return saturate(normal.DotProduct(m_direct)) * origin_color * this->m_light_color;
}

/* *********************************************
* LightManager
* *********************************************/
LightManager* LightManager::m_instance = nullptr;

bool LightManager::Init(){
	m_instance = new LightManager;
	if (m_instance == nullptr){
		return false;
	}

	return true;
}

void LightManager::UnInit(){
	delete m_instance;
	m_instance = nullptr;
}
NormColor4 LightManager::Process(NormColor4 origin_color, WorldPos normal, WorldPos pos){
	NormColor4 res_color;
	for (auto it = m_vec_light.begin(); it != m_vec_light.end(); ++it){
		res_color += (*it)->ProcessColor(origin_color, normal, pos);
	}
	return res_color;
}
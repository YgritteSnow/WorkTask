#include "Light.h"
#include "Material.h"
#include "Camera.h"
#include <cmath>

/* *********************************************
* Light
* *********************************************/
NormColor4 AmbientLight::ProcessColor(WorldPos normal, WorldPos pos){
	// ambient
	MaterialManager* aaa = MaterialManager::GetInstance();
	NormColor4 bbb = aaa->GetAmbient();
	return (bbb * this->m_light_color);
}

NormColor4 DirectLight::ProcessColor(WorldPos normal, WorldPos pos){
	// diffuse
	auto ln = normal.DotProduct(m_direct);
	ln = saturate(ln);
	NormColor4 diffuse = MaterialManager::GetInstance()->GetDiffuse() * ln;

	// specular
	auto viewDir = pos - CameraManager::GetInstance()->CurrentCamera()->GetCameraPos();
	viewDir.Normalise();
	auto midDir = viewDir + this->m_direct;
	midDir.Normalise();
	auto lv = midDir.DotProduct(normal);
	lv = saturate(lv);
	lv = std::pow(lv, MaterialManager::GetInstance()->GetSpecular()._w);
	NormColor4 specular = MaterialManager::GetInstance()->GetSpecular() * lv;

	return ((diffuse + specular) * this->m_light_color);
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
		res_color += (*it)->ProcessColor(normal, pos);
	}
	res_color *= origin_color;
	return res_color;
}
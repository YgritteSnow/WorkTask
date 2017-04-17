#include "Light.h"
#include "Material.h"
#include "CameraManager.h"
#include <cmath>

/* *********************************************
* AmbientLight
* *********************************************/
NormColor4 AmbientLight::ProcessDummy(WorldPos normal, WorldPos pos){
	return MaterialManager::GetInstance()->GetAmbient() * this->m_light_color;
}
NormColor4 AmbientLight::ProcessAmbient(const Material* material, WorldPos normal, WorldPos pos) {
	return material->ambient * this->m_light_color;
}
NormColor4 AmbientLight::ProcessDiffuse(const Material* material, WorldPos normal, WorldPos pos) {
	return NormColor4();
}
NormColor4 AmbientLight::ProcessSpecular(const Material* material, WorldPos normal, WorldPos pos) {
	return NormColor4();
}

/* *********************************************
* DirectLight
* *********************************************/
NormColor4 DirectLight::ProcessDummy(WorldPos normal, WorldPos pos){
	NormColor4 diffuse = ProcessDiffuse(MaterialManager::GetInstance()->CurMaterial(), normal, pos);
	NormColor4 specular = ProcessSpecular(MaterialManager::GetInstance()->CurMaterial(), normal, pos);
	return (diffuse + specular);
}
NormColor4 DirectLight::ProcessAmbient(const Material* material, WorldPos normal, WorldPos pos) {
	return NormColor4();
}
NormColor4 DirectLight::ProcessDiffuse(const Material* material, WorldPos normal, WorldPos pos) {
	auto ln = normal.DotProduct(m_direct);
	ln = saturate(ln);
	return material->diffuse * ln * this->m_light_color;
}
NormColor4 DirectLight::ProcessSpecular(const Material* material, WorldPos normal, WorldPos pos) {
	auto viewDir = CameraManager::GetInstance()->CurrentCamera()->GetCameraPos() - pos;
	viewDir.Normalise();
	auto midDir = viewDir + this->m_direct;
	midDir.Normalise();
	auto lv = midDir.DotProduct(normal);
	lv = saturate(lv);
	lv = pow(lv, material->specular._w);
	return material->specular * lv * this->m_light_color;
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
NormColor4 LightManager::ProcessDummy(NormColor4 origin_color, WorldPos normal, WorldPos pos){
	NormColor4 res_color;
	for (auto it = m_vec_light.begin(); it != m_vec_light.end(); ++it){
		res_color += (*it)->ProcessDummy(normal, pos);
	}
	res_color += MaterialManager::GetInstance()->GetEmissive();
	res_color._w = 1;
	res_color *= origin_color;
	return res_color;
}
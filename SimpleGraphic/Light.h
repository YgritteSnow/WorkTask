#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <vector>
#include "JMath.h"
#include "Color.h"
#include "DebugGUI.h"

/* *********************************************
* Light
* *********************************************/
class Light{
public:
	Light(NormColor4 color) :m_light_color(color){};
public:
	virtual NormColor4 ProcessColor(WorldPos normal, WorldPos pos) = 0;
protected:
	NormColor4 m_light_color;
};

class AmbientLight : public Light{
public:
	AmbientLight(NormColor4 color):Light(color){}
public:
	virtual NormColor4 ProcessColor(WorldPos normal, WorldPos pos);
};

class DirectLight : public Light{
public:
	DirectLight(NormColor4 color, WorldPos dir) :Light(color), m_direct(dir){
		m_direct.Normalise();
	};
public:
	virtual NormColor4 ProcessColor(WorldPos normal, WorldPos pos);
protected:
	WorldPos m_direct;
};
class PointLight : public Light{
};
class SpotLight : public Light{
};

/* *********************************************
* LightManager
* *********************************************/
class LightManager{
private:
	LightManager(){}
	~LightManager(){
		for (auto it = m_vec_light.begin(); it != m_vec_light.end(); ++it){
			delete (*it);
			*it = nullptr;
		}
		m_vec_light.clear();
	}

public:
	static bool Init();
	static void UnInit();
	static LightManager* GetInstance(){return m_instance;};

	void AddLight(Light* l){
		m_vec_light.push_back(l);
		DebugManager::GetInstance()->SetLightCount(m_vec_light.size());
	}
public:
	NormColor4 Process(NormColor4 origin_color, WorldPos normal, WorldPos pos);
private:
	static LightManager* m_instance;
	std::vector<Light*> m_vec_light;
};
#endif
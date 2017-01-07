#ifndef __JMATERIAL_H__
#define __JMATERIAL_H__

#include "Color.h"

struct Material {
	Material()
		: ambient(NormColor4(1,1,1,1))
		, diffuse(NormColor4(1,1,1,1))
		, specular(NormColor4(1,1,1,1))
	{};
	Material(NormColor4 a, NormColor4 d, NormColor4 s)
		: ambient(a)
		, diffuse(d)
		, specular(s)
	{}

	NormColor4 ambient;
	NormColor4 diffuse;
	NormColor4 specular;
};

class MaterialManager {
public:
	MaterialManager() :m_cur_material() {}
	static bool Init();
	static void UnInit();
	static MaterialManager* GetInstance() { 
		return m_instance; 
	}

public:
	void SetMaterial(const Material& m) {
		m_cur_material = m; 
	}
	void ClearMaterial() { m_cur_material = Material(); }

	NormColor4 GetAmbient() { 
		return m_cur_material.ambient; 
	}
	NormColor4 GetDiffuse() { return m_cur_material.diffuse; }
	NormColor4 GetSpecular() { return m_cur_material.specular; }

private:
	static MaterialManager* m_instance;
	Material m_cur_material;
};

#endif
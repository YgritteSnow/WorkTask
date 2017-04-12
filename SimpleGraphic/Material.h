#ifndef __JMATERIAL_H__
#define __JMATERIAL_H__

#include "Color.h"

struct Material {
	Material()
		: ambient()
		, diffuse()
		, specular()
		, emissive()
	{};
	Material(const NormColor4& a, const NormColor4& d, const NormColor4& s, const NormColor4& e)
		: ambient(a)
		, diffuse(d)
		, specular(s)
		, emissive(e)
	{}

	NormColor4 ambient;
	NormColor4 diffuse;
	NormColor4 specular;
	NormColor4 emissive;
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
	void SetMaterial(const Material& m) { m_cur_material = m; }
	void ClearMaterial() { m_cur_material = Material(); }
	Material* CurMaterial() { return &m_cur_material; }

	NormColor4 GetAmbient() { return m_cur_material.ambient; }
	NormColor4 GetDiffuse() { return m_cur_material.diffuse; }
	NormColor4 GetSpecular() { return m_cur_material.specular; }
	NormColor4 GetEmissive() { return m_cur_material.emissive; }

private:
	static MaterialManager* m_instance;
	Material m_cur_material;
};

#endif
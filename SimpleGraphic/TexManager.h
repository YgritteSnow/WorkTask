#pragma once
#include "TexBuffer.h"

class TextureManager {
public:
	~TextureManager();

	static bool Init();
	static void UnInit();
	static TextureManager* GetInstance() { return m_instance; }

public:
	// ��ǰ��ͼ ��todo ֱ��ʹ���ⲿָ�룬���ܱ��Ұָ�룩
	void SetTexture(std::string texname) { m_cur_texture = TextureManager::GetInstance()->LoadTexture_norm(texname); }
	TexBuffer<NormColor4>* GetTexture() { return m_cur_texture; }

public:
	TexBuffer<ShortColor4>* LoadTexture_short(std::string texname);
	TexBuffer<ShortColor4>* GetTexture_short(std::string texname);
	TexBuffer<NormColor4>* LoadTexture_norm(std::string texname);
	TexBuffer<NormColor4>* GetTexture_norm(std::string texname);
	TexBuffer<NormColor4>* ConvertShortToNrom(TexBuffer<ShortColor4>* pTex_short);

private:
	static TextureManager* m_instance;
	std::map<std::string, TexBuffer<ShortColor4>*> m_map_tex_short;
	std::map<std::string, TexBuffer<NormColor4>*> m_map_tex_norm;
	// ��ͼ����
	TexBuffer<NormColor4>* m_cur_texture;
};
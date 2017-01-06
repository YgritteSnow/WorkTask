#include "Texture.h"
#include "BitMap.h"

TextureManager* TextureManager::m_instance = nullptr;

TextureManager::~TextureManager(){
	for (auto it = m_map_tex_short.begin(); it != m_map_tex_short.end(); ++it){
		delete it->second;
		it->second = nullptr;
	}
	m_map_tex_short.clear();
}

bool TextureManager::Init(){
	m_instance = new TextureManager();
	if (!m_instance){
		return false;
	}
	return true;
}

void TextureManager::UnInit(){
	if (m_instance){
		delete m_instance;
		m_instance = nullptr;
	}
}

ImgBuffer<NormColor4>* TextureManager::ConvertShortToNrom(ImgBuffer<ShortColor4>* pTex_short){
	ImgBuffer<NormColor4>* pTex_norm = new ImgBuffer<NormColor4>(pTex_short->width, pTex_short->height);
	if (!pTex_norm){
		return nullptr;
	}
	for (int y = 0; y < pTex_short->height; ++y){
		for (int x = 0; x < pTex_short->width; ++x){
			*pTex_norm->pPixelAt(x, y) = static_cast<NormColor4>(*pTex_short->pPixelAt(x, y));
		}
	}
	return pTex_norm;
}

ImgBuffer<ShortColor4>* TextureManager::LoadTexture_short(std::string texname){
	auto pTex = GetTexture_short(texname);
	if (pTex == nullptr){
		auto newTex_short = BitMap::Load(texname.c_str());
		if (!newTex_short){
			return nullptr;
		}
		else{
			auto newTex_norm = ConvertShortToNrom(newTex_short);
			if (!newTex_norm){
				delete newTex_short;
				return nullptr;
			}
			else{
				m_map_tex_short[texname] = newTex_short;
				m_map_tex_norm[texname] = newTex_norm;
				return newTex_short;
			}
		}
	}
	else{
		return pTex;
	}
}

ImgBuffer<ShortColor4>* TextureManager::GetTexture_short(std::string texname){
	auto it = m_map_tex_short.find(texname);
	if (it == m_map_tex_short.end()){
		return nullptr;
	}
	else{
		return it->second;
	}
}

ImgBuffer<NormColor4>* TextureManager::LoadTexture_norm(std::string texname){
	auto pTex = GetTexture_norm(texname);
	if (pTex == nullptr){
		auto pTex_short = LoadTexture_short(texname);
		if (!pTex_short){
			return nullptr;
		}

		return GetTexture_norm(texname);
	}
	else{
		return pTex;
	}
}
ImgBuffer<NormColor4>* TextureManager::GetTexture_norm(std::string texname){
	auto it = m_map_tex_norm.find(texname);
	if (it == m_map_tex_norm.end()){
		return nullptr;
	}
	else{
		return it->second;
	}
}
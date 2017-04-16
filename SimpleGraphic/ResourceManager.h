#pragma once

template <typename ResourceType, typename ResourceID>
class ResourceManager
{
private:
	ResourceManager();
public:
	~ResourceManager();

	static bool Init();
	static void UnInit();
	static ResourceManager* GetInstance() { return m_instance; }

	ResourceManager::~ResourceManager() {
		for (auto it = m_map_resource.begin(); it != m_map_resource.end(); ++it) {
			delete it->second;
			it->second = nullptr;
		}
		m_map_resource.clear();
	}

	bool ResourceManager::Init() {
		m_instance = new ResourceManager();
		if (!m_instance) {
			return false;
		}
		return true;
	}
public:
	void SetCurResourceByID(ResourceID res_id);
	void SetCurResourceByPtr(ResourceType* res_ptr);

	ResourceType* GetResourceByID(ResourceID res_id) {
		auto it = m_map_tex_short.find(res_id);
		if (it == m_map_tex_short.end()) {
			return nullptr;
		}
		else {
			return it->second;
		}
	}
	ResourceType* LoadResourceByID(ResourceID res_id) {
		auto pTex = GetTexture_short(res_id);
		if (pTex == nullptr) {
			auto newTex_short = BitMap::Load(res_id.c_str());
			if (!newTex_short) {
				return nullptr;
			}
			else {
				auto newTex_norm = ConvertShortToNrom(newTex_short);
				if (!newTex_norm) {
					delete newTex_short;
					return nullptr;
				}
				else {
					m_map_tex_short[res_id] = newTex_short;
					m_map_tex_norm[res_id] = newTex_norm;
					return newTex_short;
				}
			}
		}
		else {
			return pTex;
		}
	}
	bool ReleaseResourceByID(ResourceID res_id);
private:
	static ResourceManager* m_instance = nullptr;
	std::map<ResourceID, ResourceType*> m_map_resource;
	ResourceType* m_cur_resource;
};

//ResourceManager* ResourceManager::m_instance = nullptr;
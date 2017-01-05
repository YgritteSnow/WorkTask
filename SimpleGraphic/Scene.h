#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>
#include <Windows.h>

#include "JMath.h"
#include "Color.h"
#include "Camera.h"
#include "Model.h"

/* *********************************************
* Scene & SceneManager
* *********************************************/
class Scene{
public:
	void Render();
	void AddModel(Model<DummyVertex>* model){ m_vec_model.push_back(model); }
	void DelModel(Model<DummyVertex>* model){ m_vec_model.erase(std::find(m_vec_model.begin(), m_vec_model.end(), model)); }
public:
	void Update(TimeType delta_time);
private:
	std::vector< Model<DummyVertex>* > m_vec_model;
	Camera* m_vec_camera;
};

class SceneManager{
public:
	~SceneManager(){
		for (auto it = m_vec_scenes.begin(); it != m_vec_scenes.end(); ++it){
			delete (*it);
			*it = nullptr;
		}
		m_vec_scenes.clear();
	}
	static bool Init();
	static void UnInit();
	static SceneManager* GetInstance(){ return m_instance; }

public:
	void AddScene(Scene* scene){ m_vec_scenes.push_back(scene); }
	void DelScene(Scene* scene){ m_vec_scenes.erase(std::find(m_vec_scenes.begin(), m_vec_scenes.end(), scene)); }
	void Render();

public:
	void Update(TimeType delta_time);

private:
	static SceneManager* m_instance;
	std::vector<Scene*> m_vec_scenes; // ֻ��Ⱦ��ǰ��idx=0���ĳ���
};

#endif
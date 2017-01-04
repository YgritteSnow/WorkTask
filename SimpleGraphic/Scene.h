#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>
#include <Windows.h>

#include "JMath.h"
#include <Windows.h>

/* *********************************************
* Camera & CameraManager
* *********************************************/
extern UINT WINDOW_WIDTH;
extern UINT WINDOW_HEIGHT;

class Camera{
public:
	Camera() :m_viewMat(), m_projMat(){}
	void DummyData(){
		this->SetViewMat(WorldPos(0, 0, 10), WorldPos(0, 0, 0), WorldPos(0, 1, 0));
		this->SetProjMat(0.4f, 1.f, 1.f, 1000.f);
	}
	void SetViewMat(WorldPos lookat, WorldPos cameraPos, WorldPos upDirect);
	void SetProjMat(float fov, float aspect, float nearPlane, float farPlane);
	JMath::Mat44 GetViewMat() const { return m_viewMat; };
	JMath::Mat44 GetProjMat() const { return m_projMat; };
	JMath::Mat44 GetViewProjMat() const { return m_viewMat.PreMulMat(m_projMat); };
	WorldPos GetViewPortPos(WorldPos pos) const { return WorldPos((pos._x + 0.5f) * WINDOW_WIDTH, (pos._y + 0.5f) * WINDOW_HEIGHT, pos._z); }
private:
	// 相机矩阵
	JMath::Mat44 m_viewMat;
	// 投影矩阵
	JMath::Mat44 m_projMat;
	// 视口矩阵
	ScreenCoord m_screen_width, m_screen_height;
};

class CameraManager{
private:
	CameraManager() :m_curCamera(nullptr){}
public:
	static bool Init();
	static void UnInit();
	static CameraManager* GetInstance(){ return m_instance; }

public:
	bool InitCamera(){
		if (!m_curCamera){
			m_curCamera = new Camera();
		}
		if (!m_curCamera){
			return false;
		}
		m_curCamera->DummyData();
		return true;
	}
	Camera* CurrentCamera(){
		if (!m_instance){
			return nullptr;
		}
		return m_instance->m_curCamera;
	}
public:
	static CameraManager* m_instance;
	Camera* m_curCamera;
};

/* *********************************************
* Model
* *********************************************/
template <typename VertexStruct>
class Model{
public:
	void DummyData(){
		m_worldMat.SetTranslate(0, 0, 5);

		m_vec_vertex.push_back(VertexStruct(-1, -1, -1));
		m_vec_vertex.push_back(VertexStruct(-1, 1, -1));
		m_vec_vertex.push_back(VertexStruct(-1, 1, 1));
		m_vec_vertex.push_back(VertexStruct(1, 1, 1));

		m_vec_indice.push_back(0);
		m_vec_indice.push_back(1);
		m_vec_indice.push_back(2);
		m_vec_indice.push_back(2);
		m_vec_indice.push_back(1);
		m_vec_indice.push_back(3);
		m_vec_indice.push_back(3);
		m_vec_indice.push_back(1);
		m_vec_indice.push_back(0);
		m_vec_indice.push_back(3);
		m_vec_indice.push_back(0);
		m_vec_indice.push_back(2);
	}
	VertexStruct* GetVertexes(){ return m_vec_vertex.size() > 0 ? &(m_vec_vertex[0]) : nullptr; }
	DWORD* GetIndices(){ return m_vec_indice.size() > 0 ? &(m_vec_indice[0]) : nullptr; }
	std::size_t GetVertexCount(){ return m_vec_vertex.size(); }
	std::size_t GetIndiceCount(){ return m_vec_indice.size(); }
	JMath::Mat44 GetModelMat(){ return m_worldMat; }
private:
	// 模型的世界矩阵
	JMath::Mat44 m_worldMat;
	// 模型的顶点的数据
	std::vector<VertexStruct> m_vec_vertex;
	// 模型的索引的数据
	std::vector<DWORD> m_vec_indice;
};

/* *********************************************
* Scene & SceneManager
* *********************************************/
struct DummyVertex{
	DummyVertex():pos(), color(255, 255, 255, 255), normal(0, 0, 1){}
	DummyVertex(float x, float y, float z) :pos(x, y, z), color(255, 255, 255, 255), normal(0, 0, 1){}
	WorldPos pos;
	Color4 color;
	WorldPos normal;
};
class Scene{
public:
	void Render();
	void AddModel(Model<DummyVertex>* model){ m_vec_model.push_back(model); }
	void DelModel(Model<DummyVertex>* model){ m_vec_model.erase(std::find(m_vec_model.begin(), m_vec_model.end(), model)); }
private:
private:
	std::vector< Model<DummyVertex>* > m_vec_model;
	Camera* m_vec_camera;
};

class SceneManager{
public:
	static bool Init();
	static void UnInit();
	static SceneManager* GetInstance(){ return m_instance; }

public:
	void AddScene(Scene* scene){ m_vec_scenes.push_back(scene); }
	void DelScene(Scene* scene){ m_vec_scenes.erase(std::find(m_vec_scenes.begin(), m_vec_scenes.end(), scene)); }
	void Render();

private:
	static SceneManager* m_instance;
	std::vector<Scene*> m_vec_scenes; // 只渲染当前（idx=0）的场景
};

#endif
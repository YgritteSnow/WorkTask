#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>

class Camera{
private:
	// 相机矩阵
	JMath::Mat44 ViewMat;
	// 投影矩阵
	JMath::Mat44 ProjMat;
};

class Model{
private:
	// 模型的世界矩阵
	JMath::Mat44 WorldMat;
};

class Scene{
public:

private:
	std::vector<Model*> m_vec_model;
	std::vector<Camera*> m_vec_camera;
};

#endif
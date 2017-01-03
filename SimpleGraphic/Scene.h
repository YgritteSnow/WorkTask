#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>

class Camera{
private:
	// �������
	JMath::Mat44 ViewMat;
	// ͶӰ����
	JMath::Mat44 ProjMat;
};

class Model{
private:
	// ģ�͵��������
	JMath::Mat44 WorldMat;
};

class Scene{
public:

private:
	std::vector<Model*> m_vec_model;
	std::vector<Camera*> m_vec_camera;
};

#endif
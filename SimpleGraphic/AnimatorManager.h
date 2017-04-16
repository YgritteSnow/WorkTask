#pragma once

#include <vector>
#include "Animator.h"
#include "JTime.h"

class AnimatorManager
{
private:
	AnimatorManager();
public:
	~AnimatorManager();
	static bool Init();
	static void UnInit();
	static AnimatorManager* GetInstance();
public:
	void Update(TimeType t);
	void RegisterAnimator(Animator* ani);
	void UnRegisterAnimator(Animator* ani);
private:
	static AnimatorManager* m_instance;
	std::vector<Animator*> m_vec_animator;
};
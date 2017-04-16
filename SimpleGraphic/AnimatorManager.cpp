#include "AnimatorManager.h"
#include <algorithm>

AnimatorManager* AnimatorManager::m_instance = nullptr;

AnimatorManager::AnimatorManager() {}
AnimatorManager::~AnimatorManager() {
	m_vec_animator.clear();
}

bool AnimatorManager::Init() {
	m_instance = new AnimatorManager;
	if (!m_instance) {
		return false;
	}
	return true;
}

void AnimatorManager::UnInit() {
	delete m_instance;
	m_instance = nullptr;
}

AnimatorManager* AnimatorManager::GetInstance() {
	return m_instance;
}

void AnimatorManager::RegisterAnimator(Animator* ani) {
	m_vec_animator.push_back(ani);
}

void AnimatorManager::UnRegisterAnimator(Animator* ani) {
	m_vec_animator.erase(std::remove_if(m_vec_animator.begin(), m_vec_animator.end(), [&ani](const auto& v) {
		return v == ani;
	}));
}

void AnimatorManager::Update(TimeType t) {
	std::for_each(m_vec_animator.begin(), m_vec_animator.end(), [&t](const auto& v) {
		v->Update(t);
	});
}
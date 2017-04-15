#include "Animator.h"

/************************************************************************/
/* 动画资源（一份）
/************************************************************************/
AnimateData::AnimateData()
	: m_frames(nullptr)
	, m_bones_count(0)
	, m_frames_count(0)
{}

AnimateData::~AnimateData() {
	delete[] m_frames;
	m_frames = nullptr;
}

/************************************************************************/
/* 动画实例（多份）
/************************************************************************/
Animator::Animator()
	: m_animate(nullptr)
	, m_skeletonCalculator(nullptr)
{}

Animator::~Animator() {
	delete m_animate;
	m_animate = nullptr;
	delete m_skeletonCalculator;
	m_skeletonCalculator = nullptr;
}
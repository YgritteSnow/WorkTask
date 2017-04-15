#include "Animator.h"

/************************************************************************/
/* ������Դ��һ�ݣ�
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
/* ����ʵ������ݣ�
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
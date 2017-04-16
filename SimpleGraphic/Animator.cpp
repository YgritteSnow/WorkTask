#include "Animator.h"
#include "AniResManager.h"
#include "AnimatorManager.h"

#include "DebugGUI.h"

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

AnimateData::FrameInfo::FrameInfo() 
	:joints(nullptr), time(0.f) 
{}

AnimateData::FrameInfo::~FrameInfo() { 
	delete[] joints; 
	joints = nullptr; 
}

/************************************************************************/
/* 动画实例（多份）
/************************************************************************/
Animator::Animator(SkeSourceID ske_id, AniSourceID ani_id)
	: m_ske_id(ske_id)
	, m_ani_id(ani_id)
	, m_animate(nullptr)
	, m_skeletonCalculator(nullptr)
	, m_cur_time(0)
	, m_isDirty(true)
	, m_isActive(false)
	, m_duation_scale(1.f)
{
	m_animate = AniResManager::GetInstance()->GetAnimation(m_ani_id);
	m_skeleton = AniResManager::GetInstance()->GetSkeleton(m_ske_id);
	m_skeletonCalculator = new SkeletonCalculator(m_skeleton);
	m_cur_joints = new JointData[m_skeleton->m_bones_count];
	RefreshJoints();
	m_skeletonCalculator->ChangeSkeleton(m_cur_joints);
}

Animator::~Animator() {
	delete m_skeletonCalculator;
	m_skeletonCalculator = nullptr;
	delete[] m_cur_joints;
	m_cur_joints = nullptr;
	Stop();
}

void Animator::Update(TimeType t){
	m_cur_time += t * m_duation_scale;
	RefreshJoints();
	m_skeletonCalculator->ChangeSkeleton(m_cur_joints);
	DebugManager::GetInstance()->SetAnimateTime(m_cur_time);
}

void Animator::RefreshJoints() {
	while (m_cur_time < 0)m_cur_time += m_animate->m_duation;
	while (m_cur_time > m_animate->m_duation)m_cur_time -= m_animate->m_duation;

	int frame_idx = FindFrameIndex(m_cur_time);
	auto& frame_left = m_animate->m_frames[frame_idx];
	auto& frame_right = m_animate->m_frames[(frame_idx + 1) % m_animate->m_frames_count];
	float lerp_para = (m_cur_time - frame_left.time) / (frame_right.time - frame_left.time);
	for (int i = 0; i < m_skeleton->m_bones_count; ++i) {
		auto& cur = m_cur_joints[i];
		auto& l = frame_left.joints[i];
		auto& r = frame_right.joints[i];
		cur.quaternion = l.quaternion.Slerp(r.quaternion, lerp_para);
		cur.scale = lerp(l.scale, r.scale, lerp_para);
		cur.translation = lerp(l.translation, r.translation, lerp_para);
	}

	m_isDirty = true;
}

int Animator::FindFrameIndex(float t) {
	// todo 以后可以把这个优化下，可能可以用时间桶的方式来快速索引
	for (int i = 0; i < m_animate->m_frames_count; ++i) {
		if (m_animate->m_frames[i].time > t) {
			return i;
		}
	}
	return 0;
}

void Animator::Start() {
	if (!m_isActive) {
		m_isActive = true;
		AnimatorManager::GetInstance()->RegisterAnimator(this);
	}
}

void Animator::Stop() {
	if (m_isActive) {
		m_isActive = false;
		AnimatorManager::GetInstance()->UnRegisterAnimator(this);
	}
}

WorldPos Animator::CalculateVertexPos(int bone_idx, const WorldPos& old_pos) {
	return m_skeletonCalculator->CalculateVertexPos(bone_idx, old_pos);
}
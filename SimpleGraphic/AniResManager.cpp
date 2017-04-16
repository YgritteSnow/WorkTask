#include "AniResManager.h"

#include <algorithm>

const int DEBUG_BONE_COUNT = 3;

AniResManager* AniResManager::m_instance = nullptr;

AniResManager::AniResManager() {}

AniResManager::~AniResManager() {
	std::for_each(m_map_skeleton.begin(), m_map_skeleton.end(), [](auto& v) {
		delete v.second;
	});
	m_map_skeleton.clear();

	std::for_each(m_map_animator.begin(), m_map_animator.end(), [](auto& v) {
		delete v.second;
	});
	m_map_animator.clear();
}

bool AniResManager::Init() {
	if (m_instance) {
		return false;
	}
	m_instance = new AniResManager();
	if (!m_instance) {
		return false;
	}
	return true;
}

void AniResManager::UnInit() {
	delete m_instance;
	m_instance = nullptr;
}

AniResManager* AniResManager::GetInstance() {
	return m_instance;
}
/************************************************************************/
/* ¼ÓÔØ¹Ç÷À
/************************************************************************/
const SkeletonData* AniResManager::GetSkeleton(SkeSourceID src_id) {
	auto ani = GetSkeletonInner(src_id);
	if (ani == nullptr) {
		return LoadSkeleton(src_id);
	}
	else {
		return ani;
	}
}

const SkeletonData* AniResManager::GetSkeletonInner(SkeSourceID src_id) {
	auto tmp = m_map_skeleton.find(src_id);
	if (tmp == m_map_skeleton.end()) {
		return nullptr;
	}
	else {
		return tmp->second;
	}
}

bool AniResManager::ReleaseSkeletonById(SkeID ani_id) {
	for (auto it = m_map_skeleton.begin(); it != m_map_skeleton.end(); ++it) {
		if (it->first == ani_id) {
			m_map_skeleton.erase(it);
			return true;
		}
	}
	return false;
}

bool AniResManager::ReleaseSkeletonByPtr(SkeletonData* ani_ptr) {
	for (auto it = m_map_skeleton.begin(); it != m_map_skeleton.end(); ++it) {
		if (it->second == ani_ptr) {
			m_map_skeleton.erase(it);
			return true;
		}
	}
	return false;
}

const SkeletonData* AniResManager::LoadSkeleton(AniSourceID src_id) {
	SkeletonData* res = new SkeletonData(DEBUG_BONE_COUNT);
	for (int i = 0; i < DEBUG_BONE_COUNT; ++i) {
		if (i == 0) {
			res->m_joints_local[i] = JointData(0
				, Quaternion(0, 0, 0, 1)
				, WorldPos(0, 0, 0)
				, 1.f);
		}
		else {
			res->m_joints_local[i] = JointData(i - 1
				, Quaternion(0, 0, 0, 1)
				, WorldPos(0, 1.1f, 0)
				, 1.f);
		}
	}
	return res;
}

/************************************************************************/
/* ¼ÓÔØ¶¯»­                                                                     */
/************************************************************************/
const AnimateData* AniResManager::GetAnimation(AniSourceID src_id) {
	auto ani = GetAnimationInner(src_id);
	if (ani == nullptr) {
		return LoadAnimation(src_id);
	}
	else {
		return ani;
	}
}

const AnimateData* AniResManager::GetAnimationInner(AniSourceID src_id) {
	auto tmp = m_map_animator.find(src_id);
	if (tmp == m_map_animator.end()) {
		return nullptr;
	}
	else {
		return tmp->second;
	}
}

bool AniResManager::ReleaseAnimationById(AniID ani_id) {
	for (auto it = m_map_animator.begin(); it != m_map_animator.end(); ++it) {
		if (it->first == ani_id) {
			m_map_animator.erase(it);
			return true;
		}
	}
	return false;
}

bool AniResManager::ReleaseAnimationByPtr(AnimateData* ani_ptr) {
	for (auto it = m_map_animator.begin(); it != m_map_animator.end(); ++it) {
		if (it->second == ani_ptr) {
			m_map_animator.erase(it);
			return true;
		}
	}
	return false;
}

const AnimateData* AniResManager::LoadAnimation(AniSourceID src_id) {
	AnimateData* res = new AnimateData;
	res->m_bones_count = DEBUG_BONE_COUNT;
	res->m_frames_count = 2;
	res->m_duation = 10.f;
	res->m_frames = new AnimateData::FrameInfo[res->m_frames_count];
	AnimateData::FrameInfo* fr;

		fr = &res->m_frames[0];
		fr->time = 0.f;
		fr->joints = new JointData[res->m_bones_count];
		for (int ib = 0; ib < DEBUG_BONE_COUNT; ++ib) {
			if (ib == 0) {
				fr->joints[ib] = JointData(0
					, Quaternion(0, 0, 0, 1)
					, WorldPos(0, 0, 0)
					, 1.f);
			}
			else {
				fr->joints[ib] = JointData(ib - 1
					, Quaternion(0, 0, 0, 1)
					, WorldPos(0, 0, 0)
					, 1.f);
			}
		}

		fr = &res->m_frames[1];
		fr->time = 1.f;
		fr->joints = new JointData[res->m_bones_count];
		for (int ib = 0; ib < DEBUG_BONE_COUNT; ++ib) {
			if (ib == 0) {
				fr->joints[ib] = JointData(0
					, Quaternion(0, 0, std::cos(JMath::PI / 6), std::sin(JMath::PI / 6))
					, WorldPos(0, 0, 0)
					, 1.f);
			}
			else {
				fr->joints[ib] = JointData(ib - 1
					, Quaternion(0, 0, std::cos(JMath::PI / 6), std::sin(JMath::PI / 6))
					, WorldPos(0, 0, 0)
					, 1.f);
			}
		}

	return res;
}
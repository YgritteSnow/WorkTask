#pragma once

#include <map>
#include "Animator.h"

/**
 * ����������
 * ע�Ტ�������ж����ĸ���
**/

typedef int AniID;
typedef int SkeID;

class AniResManager
{
private:
	AniResManager();
	~AniResManager();
public:
	static bool Init();
	static void UnInit();
	static AniResManager* GetInstance();

public:
	// ���ع���
	const SkeletonData* AniResManager::GetSkeleton(SkeSourceID src_id);
	const SkeletonData* AniResManager::GetSkeletonInner(SkeSourceID src_id);
	const SkeletonData* LoadSkeleton(SkeSourceID src_id);
	bool ReleaseSkeletonById(SkeID ani_id);
	bool ReleaseSkeletonByPtr(SkeletonData* ani_ptr);

	// ���ض���
	const AnimateData* AniResManager::GetAnimation(AniSourceID src_id);
	const AnimateData* AniResManager::GetAnimationInner(AniSourceID src_id);
	const AnimateData* LoadAnimation(AniSourceID src_id);
	bool ReleaseAnimationById(AniID ani_id);
	bool ReleaseAnimationByPtr(AnimateData* ani_ptr);

private:
	std::map<AniID, AnimateData*> m_map_animator;
	std::map<AniID, SkeletonData*> m_map_skeleton;
private:
	static AniResManager* m_instance;
};
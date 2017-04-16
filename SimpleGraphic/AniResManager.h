#pragma once

#include <map>
#include "Animator.h"

/**
 * 动画控制类
 * 注册并控制所有动画的更新
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
	// 加载骨骼
	const SkeletonData* AniResManager::GetSkeleton(SkeSourceID src_id);
	const SkeletonData* AniResManager::GetSkeletonInner(SkeSourceID src_id);
	const SkeletonData* LoadSkeleton(SkeSourceID src_id);
	bool ReleaseSkeletonById(SkeID ani_id);
	bool ReleaseSkeletonByPtr(SkeletonData* ani_ptr);

	// 加载动画
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
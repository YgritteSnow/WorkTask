#pragma once
/**
 * 动画
 * 描述骨骼在多帧中的每节点的变换信息
**/

#include "JMath.h"
#include "Skeleton.h"
#include "JTime.h"

/************************************************************************/
/* 资源handle的类型
/************************************************************************/
typedef int AniSourceID;
typedef int SkeSourceID;

/************************************************************************/
/* 动画资源（一份）
/************************************************************************/
struct AnimateData
{
	AnimateData();
	~AnimateData();

	struct FrameInfo
	{
		FrameInfo();
		~FrameInfo();
		JointData* joints;
		float time;
	};
	FrameInfo* m_frames;	// 动画帧
	int m_frames_count;		// 关键帧的个数
	int m_bones_count;		// 骨骼个数
	float m_duation;		// 动画总时间
};

/************************************************************************/
/* 动画实例（多份）
/************************************************************************/
class Animator
{
public:
	Animator(SkeSourceID ske_id, AniSourceID sni_id);
	~Animator();
public:
	void Update(TimeType t);
	WorldPos CalculateVertexPos(int bone_idx, const WorldPos& old_pos); // 转发自骨骼计算器
	
	bool IsDirty() { return m_isDirty; }
	void Cleaned() { m_isDirty = false; }

	void Start();
	void Stop();
private:
	int FindFrameIndex(float t);
	void RefreshJoints();
private:
	SkeSourceID m_ske_id;
	AniSourceID m_ani_id;

	TimeType m_cur_time;		// 当前播放到的时间
	float m_duation_scale;		// 时间缩放

	JointData* m_cur_joints;	// 当前的骨骼点插值结果

	const SkeletonData* m_skeleton;		// 骨骼信息
	const AnimateData* m_animate;			// 动画信息
	SkeletonCalculator* m_skeletonCalculator; // 骨骼处理器

	bool m_isDirty;	// 是否进行过动画更新
	bool m_isActive; // 是否在播放
};
#pragma once

#include "JMath.h"
#include "Bone.h"

class Skeleton
{
private:
	int m_bones_count;
	Bone* m_bones;
	JMath::Mat44* m_localMat;
	JMath::Mat44* m_worldMat;
};
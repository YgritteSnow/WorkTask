#pragma once

#include "JMath.h"
#include "VertexBone.h"

class Skeleton
{
private:
	int m_bones_count;
	VertexBone* m_bones;
	JMath::Mat44* m_localMat;
	JMath::Mat44* m_worldMat;
};
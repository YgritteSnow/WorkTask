#include "SkinnedMesh.h"

SkinnedMesh::SkinnedMesh()
	: m_skeleton(nullptr)
	, m_animator(nullptr)
{}

SkinnedMesh::~SkinnedMesh() {
	delete m_skeleton;
	m_skeleton = nullptr;
	delete m_animator;
	m_animator = nullptr;
}
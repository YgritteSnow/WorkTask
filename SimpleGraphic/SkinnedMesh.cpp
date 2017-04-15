#include "SkinnedMesh.h"

SkinnedMesh::SkinnedMesh()
	: m_animator(nullptr)
{}

SkinnedMesh::~SkinnedMesh() {
	delete m_animator;
	m_animator = nullptr;
}
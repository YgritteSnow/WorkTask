#pragma once

#include "Model.h"

class SkinnedModel : public Model
{
public:
	void BeforeRender() override {
		Model::BeforeRender();
		m_mesh->BeforeRender();
	}
};
#pragma once
#include "ShaderStruct.h"
#include "VertexBuffer.h"
#include "TexBuffer.h"
#include "VertexUtilities.h"

#define TRIANGLE_COUNT 3

class ShaderManager
{
private:
	ShaderManager();
	~ShaderManager();
public:
	static bool Init();
	static void UnInit();
	static ShaderManager* GetInstance();

	static void ProcessVertex(VertexBuffer* vin, const VertexBuffer* vout) {
		for (int i = 0; i < vin->m_length; ++i) {
			m_instance->m_vsMain->Run(vin->GetVertex(i), vout->GetVertex(i));
		}
	}

	static STRUCT_ID ProcessPixel(byte* pin, byte* pout) {
		m_instance->m_psMain->Run(pin, pout);
		return m_instance->m_psMain->GetOutId();
	}

	static bool TestPixel(byte* v) {
		return m_instance->m_psMain->Test(v);
	}

	void SetShader(VertexShader* vs, PixelShader* ps) {
		m_vsMain = vs;
		m_psMain = ps;
	}

	const VertexShader* GetVertexShader() { return m_vsMain; }
	const PixelShader* GetPixelShader() { return m_psMain; }
private:
	static ShaderManager* m_instance;

	VertexShader* m_vsMain;
	PixelShader* m_psMain;
};
#pragma once
#include "VertexBuffer.h"
#include "StructReflection.h"
#include "VertexUtilities.h"

typedef int ShaderID;

class VertexShader
{
public:
	virtual void Run(byte* vin, byte* vout) = 0;
	virtual STRUCT_ID GetOutId() const = 0;
};

#define DECLARE_VERTEXSHADER(vinClass, vinIns, voutClass, voutIns) \
public: \
	STRUCT_ID GetOutId() const override; \
	void Run(byte* _vinIns, byte* _voutIns) override;

#define DECLARE_VERTEXSHADER_START(ShaderClass, vinClass, vinIns, voutClass, voutIns) \
	STRUCT_ID ShaderClass::GetOutId() const { return GetID(voutClass); } \
	void ShaderClass::Run(byte* _vinIns, byte* _voutIns) \
	{ \
		vinClass* vinIns = static_cast<vinClass*>(static_cast<void*>(_vinIns)); \
		voutClass* voutIns = static_cast<voutClass*>(static_cast<void*>(_voutIns)); \

#define DECLARE_VERTEXSHADER_END \
	}

class PixelShader
{
public:
	virtual void Run(byte* vin, byte* pout) = 0;
	virtual bool Test(byte* vin) { return true; }
	virtual STRUCT_ID GetOutId() const = 0;
};


#define DECLARE_PIXELSHADER(vinClass, vinIns, voutClass, voutIns) \
	STRUCT_ID GetOutId() const override; \
	void Run(byte* _vinIns, byte* _voutIns) override; \
	bool Test(byte* _vinIns) override;

#define DECLARE_PIXELSHADER_START(ShaderClass, vinClass, vinIns, voutClass, voutIns) \
	STRUCT_ID ShaderClass::GetOutId() const { return GetID(voutClass); } \
	void ShaderClass::Run(byte* _vinIns, byte* _voutIns) \
	{ \
		vinClass* vinIns = static_cast<vinClass*>(static_cast<void*>(_vinIns)); \
		voutClass* voutIns = static_cast<voutClass*>(static_cast<void*>(_voutIns)); \

#define DECLARE_PIXELSHADER_END \
	}

#define DECLARE_PIXELSHADER_TEST_START(ShaderClass, vinClass, vinIns) \
	bool ShaderClass::Test(byte* _vinIns) \
	{ \
		vinClass* vinIns = static_cast<vinClass*>(static_cast<void*>(_vinIns)); \

#define DECLARE_PIXELSHADER_TEST_END \
	}

class GeometryShader
{
public:
	virtual void Run(VertexBuffer* pVbuffer, DWORD* pIndex, MaskType* pMask) = 0;
};

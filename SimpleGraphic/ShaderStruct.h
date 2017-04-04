#pragma once
#include "VertexBuffer.h"
#include "StructReflection.h"
#include "VertexUtilities.h"

typedef int ShaderID;

class VertexShader
{
public:
	virtual void Run(const byte* vin, byte* vout) = 0;
	virtual STRUCT_ID GetOutId() const = 0;
};

#define DECLARE_VERTEXSHADER(vinClass, vinIns, voutClass, voutIns) \
public: \
	STRUCT_ID GetOutId() const override; \
	void Run(const byte* _vinIns, byte* _voutIns) override;

#define DECLARE_VERTEXSHADER_START(vinClass, vinIns, voutClass, voutIns) \
	STRUCT_ID GetOutId() const override { return GetID(voutClass); } \
	void Run(const byte* _vinIns, byte* _voutIns) override \
	{ \
		const vinClass* vinIns = static_cast<const vinClass*>(static_cast<const void*>(_vinIns)); \
		voutClass* voutIns = static_cast<voutClass*>(static_cast<void*>(_voutIns)); \

#define DECLARE_VERTEXSHADER_END \
	}

class PixelShader
{
public:
	virtual void Run(const byte* vin, byte* pout) = 0;
	virtual bool Test(const byte* vin) { return true; }
	virtual STRUCT_ID GetOutId() const = 0;
};


#define DECLARE_PIXELSHADER(vinClass, vinIns, voutClass, voutIns) \
	STRUCT_ID GetOutId() const override; \
	void Run(const byte* _vinIns, byte* _voutIns) override; \
	bool Test(const byte* _vinIns) override;

#define DECLARE_PIXELSHADER_START(vinClass, vinIns, voutClass, voutIns) \
	STRUCT_ID GetOutId() const override { return GetID(voutClass); } \
	void Run(const byte* _vinIns, byte* _voutIns) override \
	{ \
		const vinClass* vinIns = static_cast<const vinClass*>(static_cast<const void*>(_vinIns)); \
		voutClass* voutIns = static_cast<voutClass*>(static_cast<void*>(_voutIns)); \

#define DECLARE_PIXELSHADER_END \
	}

#define DECLARE_PIXELSHADER_TEST_START(vinClass, vinIns) \
	bool Test(const byte* _vinIns) override \
	{ \
		const vinClass* vinIns = static_cast<const vinClass*>(static_cast<const void*>(_vinIns)); \

#define DECLARE_PIXELSHADER_TEST_END \
	}

class GeometryShader
{
public:
	virtual void Run(const VertexBuffer* pVbuffer, DWORD* pIndex, MaskType* pMask) = 0;
};

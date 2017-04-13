#pragma once
#include <map>
#include <vector>

typedef unsigned long STRIDE_TYPE;
typedef unsigned long STRUCT_ID;
#ifndef byte
typedef unsigned char byte;
#endif

#define MAX_ATTRI_SIZE 16
#define MAX_CID 16

enum AttriType {
	POSITION,
	NORMAL,
	UV,
	COLOR,
	FLOATPARA,
	VERTEXBONE,
};

struct _StructReflect
{
	_StructReflect()
		: m_size(0)
		, m_attriType_pos_size(0)
		, m_attriType_normal_size(0)
		, m_attriType_uv_size(0)
		, m_attriType_color_size(0)
		, m_attriType_float_size(0)
		, m_attriType_vertexbone_size(0)
		, m_attriType_all_size(0)
	{}
	template <typename T, typename C>
	_StructReflect* AddAttri(const C T::* pField, AttriType attriType) {
		STRIDE_TYPE offset = ((STRIDE_TYPE)&(((const T*)1024)->*pField)) - 1024;
		push_back(offset, attriType);
		m_size = sizeof(T);
		return this;
	}

	void push_back(STRIDE_TYPE offset, AttriType attrType) {
		m_attriType_all[m_attriType_all_size++] = offset;
		if (attrType == POSITION)m_attriType_pos[m_attriType_pos_size++] = offset;
		else if (attrType == NORMAL)m_attriType_normal[m_attriType_normal_size++] = offset;
		else if (attrType == UV)m_attriType_uv[m_attriType_uv_size++] = offset;
		else if (attrType == COLOR)m_attriType_color[m_attriType_color_size++] = offset;
		else if (attrType == FLOATPARA)m_attriType_float[m_attriType_float_size++] = offset;
		else if (attrType == VERTEXBONE)m_attriType_vertexbone[m_attriType_vertexbone_size++] = offset;
	}

	template <AttriType attriType>
	STRIDE_TYPE get_attri(int index){}
	template <>
	STRIDE_TYPE get_attri<POSITION>(int index) { return m_attriType_pos[index]; }
	template <>
	STRIDE_TYPE get_attri<NORMAL>(int index) { return m_attriType_normal[index]; }
	template <>
	STRIDE_TYPE get_attri<UV>(int index) { return m_attriType_uv[index]; }
	template <>
	STRIDE_TYPE get_attri<COLOR>(int index) { return m_attriType_color[index]; }
	template <>
	STRIDE_TYPE get_attri<FLOATPARA>(int index) { return m_attriType_float[index]; }
	template <>
	STRIDE_TYPE get_attri<VERTEXBONE>(int index) { return m_attriType_vertexbone[index]; }

	STRIDE_TYPE m_attriType_pos[MAX_ATTRI_SIZE];
	STRIDE_TYPE m_attriType_normal[MAX_ATTRI_SIZE];
	STRIDE_TYPE m_attriType_uv[MAX_ATTRI_SIZE];
	STRIDE_TYPE m_attriType_color[MAX_ATTRI_SIZE];
	STRIDE_TYPE m_attriType_float[MAX_ATTRI_SIZE];
	STRIDE_TYPE m_attriType_vertexbone[MAX_ATTRI_SIZE];
	STRIDE_TYPE m_attriType_all[MAX_ATTRI_SIZE];
	
	int m_attriType_pos_size;
	int m_attriType_normal_size;
	int m_attriType_uv_size;
	int m_attriType_color_size;
	int m_attriType_float_size;
	int m_attriType_vertexbone_size;
	int m_attriType_all_size;

	STRIDE_TYPE m_size;
};

struct StructReflectManager
{
	template <AttriType attriType>
	static int GetOffset(STRUCT_ID vid, int index) { return _id2ref[vid].get_attri<attriType>(index); }
	static const auto& GetOffsetAll(STRUCT_ID vid) { return _id2ref[vid].m_attriType_all; }
	static const auto& GetOffsetSize(STRUCT_ID vid) { return _id2ref[vid].m_size; }

	static _StructReflect _id2ref[MAX_CID];
	//static std::map<STRUCT_ID, _StructReflect > _id2ref;
};

#define DECLARE_REFLECTION_BEGIN() \
	void REFLECTION_FILTER_FUNC(void){

#define DECLARE_REFLECTION_CLASS(cls) \
		StructReflectManager::_id2ref[GetID(cls)] = _StructReflect();  \
		auto* t_ref##cls = &(StructReflectManager::_id2ref[GetID(cls)]); \
		(t_ref##cls)

#define DECLARE_REFECTION_END() \
	}

struct StructWrapper
{
	StructWrapper(STRUCT_ID vid, byte* value)
		:vid(vid)
		, value(value)
	{}
	STRUCT_ID vid;
	byte* value;
};
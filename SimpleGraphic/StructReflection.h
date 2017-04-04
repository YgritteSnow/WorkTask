#pragma once
#include <map>
#include <vector>

typedef unsigned long STRIDE_TYPE;
typedef unsigned long STRUCT_ID;
#ifndef byte
typedef unsigned char byte;
#endif

enum AttriType {
	POSITION,
	NORMAL,
	UV,
	COLOR,
};

struct _StructReflect
{
	_StructReflect() :m_size(0) {}
	template <typename T, typename C>
	_StructReflect* AddAttri(const C T::* pField, AttriType attriType) {
		STRIDE_TYPE offset = ((STRIDE_TYPE)&(((const T*)1024)->*pField)) - 1024;
		push_back(offset, attriType);
		m_size = sizeof(T);
		return this;
	}

	void push_back(STRIDE_TYPE offset, AttriType attrType) {
		m_attriType_all.push_back(offset);
		if (attrType == POSITION)m_attriType_pos.push_back(offset);
		else if (attrType == NORMAL)m_attriType_normal.push_back(offset);
		else if (attrType == UV)m_attriType_uv.push_back(offset);
		else if (attrType == COLOR)m_attriType_color.push_back(offset);
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

	std::vector<STRIDE_TYPE> m_attriType_pos;
	std::vector<STRIDE_TYPE> m_attriType_normal;
	std::vector<STRIDE_TYPE> m_attriType_uv;
	std::vector<STRIDE_TYPE> m_attriType_color;
	std::vector<STRIDE_TYPE> m_attriType_all;

	STRIDE_TYPE m_size;
};

struct StructReflectManager
{
	template <AttriType attriType>
	static int GetOffset(STRUCT_ID vid, int index) { return _id2ref[vid].get_attri<attriType>(index); }
	static const auto& GetOffsetAll(STRUCT_ID vid) { return _id2ref[vid].m_attriType_all; }
	static const auto& GetOffsetSize(STRUCT_ID vid) { return _id2ref[vid].m_size; }


	static std::map<STRUCT_ID, _StructReflect > _id2ref;
};

#define DECLARE_REFLECTION_BEGIN() \
	void REFLECTION_FILTER_FUNC(void){

#define DECLARE_REFLECTION_CLASS(cls) \
		StructReflectManager::_id2ref[GetID(cls)] = _StructReflect();  \
		auto* t_ref##cls = &(StructReflectManager::_id2ref[GetID(cls)]); \
		(t_ref##cls)

#define DECLARE_REFECTION_END() \
	}
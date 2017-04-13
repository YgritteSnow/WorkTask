#pragma once

#include "StructReflection.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/
template <typename T>
struct REFLECTION_TEMPLATE { enum { TYPEID = 0 }; };

#define DECLARE_ID(cls, cid) \
	class cls; \
	template <> \
	struct REFLECTION_TEMPLATE<cls> \
	{ \
		enum{ TYPEID = cid }; \
	};

/************************************************************************/
/*                                                                      */
/************************************************************************/
DECLARE_ID(TestVertex, 1)
DECLARE_ID(TestVertex_v2p, 2)
DECLARE_ID(TestPixel, 3)
DECLARE_ID(TestSkinnedVertex, 4)
DECLARE_ID(TestSkinnedVertex_v2p, 5)

//template <typename TT, typename T>
//constexpr STRUCT_ID GetID(TT T) {
//	return REFLECTION_TEMPLATE<T>::TYPEID
//}
#define GetID(cls) REFLECTION_TEMPLATE<cls>::TYPEID

void REFLECTION_FILTER_FUNC(void);
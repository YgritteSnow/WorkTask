#include "StructMeta.h"
#include "TestSkinnedVertex.h"
#include "TestVertex.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/
DECLARE_REFLECTION_BEGIN()

DECLARE_REFLECTION_CLASS(TestVertex)
->AddAttri(&TestVertex::pos, POSITION)
->AddAttri(&TestVertex::color, COLOR)
->AddAttri(&TestVertex::normal, NORMAL)
->AddAttri(&TestVertex::uv, COLOR)
->AddAttri(&TestVertex::biNormal, POSITION);

DECLARE_REFLECTION_CLASS(TestVertex_v2p)
->AddAttri(&TestVertex_v2p::pos, POSITION)
->AddAttri(&TestVertex_v2p::z, FLOATPARA)
->AddAttri(&TestVertex_v2p::color, COLOR)
->AddAttri(&TestVertex_v2p::normal, NORMAL)
->AddAttri(&TestVertex_v2p::worldPos, POSITION)
->AddAttri(&TestVertex_v2p::uv, COLOR)
->AddAttri(&TestVertex_v2p::biNormal, POSITION)
->AddAttri(&TestVertex_v2p::taNormal, POSITION);

DECLARE_REFLECTION_CLASS(TestPixel)
->AddAttri(&TestPixel::pos, POSITION)
->AddAttri(&TestPixel::color, COLOR);

DECLARE_REFLECTION_CLASS(TestSkinnedVertex)
->AddAttri(&TestSkinnedVertex::pos, POSITION)
->AddAttri(&TestSkinnedVertex::color, COLOR)
->AddAttri(&TestSkinnedVertex::normal, NORMAL)
->AddAttri(&TestSkinnedVertex::uv, COLOR)
->AddAttri(&TestSkinnedVertex::biNormal, POSITION)
->AddAttri(&TestSkinnedVertex::vertexBone, VERTEXBONE);

DECLARE_REFLECTION_CLASS(TestSkinnedVertex_v2p)
->AddAttri(&TestSkinnedVertex_v2p::pos, POSITION)
->AddAttri(&TestSkinnedVertex_v2p::z, FLOATPARA)
->AddAttri(&TestSkinnedVertex_v2p::color, COLOR)
->AddAttri(&TestSkinnedVertex_v2p::normal, NORMAL)
->AddAttri(&TestSkinnedVertex_v2p::worldPos, POSITION)
->AddAttri(&TestSkinnedVertex_v2p::uv, COLOR)
->AddAttri(&TestSkinnedVertex_v2p::biNormal, POSITION)
->AddAttri(&TestSkinnedVertex_v2p::taNormal, POSITION);

DECLARE_REFECTION_END()
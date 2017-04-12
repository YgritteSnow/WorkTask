#include "StructMeta.h"

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

DECLARE_REFECTION_END()
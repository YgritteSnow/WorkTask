#include "VertexUtilities.h"
#pragma once

#include "JMath.h"

bool CheckState(StateMaskType curState, StateMaskType state, StateMaskType value) {
	return ((curState & state) == (StateMaskAll & value));
}

// 三角形裁剪的 mask
const MaskType NO_CULLED = 0xff;
const MaskType BACK_CULLED = 0xff ^ 0x01;
const MaskType SCREEN_CULLED = 0xff ^ 0x02;

/* *********************************************
* 对三角形进行处理
* *********************************************/
MaskType ProcessTriangle_backCull(const WorldPos& v0, const WorldPos& v1, const WorldPos& v2, MaskType curState) {
	auto backpara = (v1 - v0).CrossProduct(v2 - v1).DotProduct(WorldPos(0, 0, -1)) < 0;
	if ((CheckState(curState, StateMask_BackCull, StateMaskValue_BackCull) && backpara < 0)
		|| (CheckState(curState, StateMask_BackCull, StateMaskValue_BackCullR) && backpara > 0)
		) {
		return BACK_CULLED;
	}
	else {
		return NO_CULLED;
	}
}
MaskType ProcessTriangle_screenCull(const WorldPos& v0, const WorldPos& v1, const WorldPos& v2) {
	if ((v0._x < 0 || v0._y < 0 || v0._x >= WINDOW_WIDTH || v0._y >= WINDOW_HEIGHT)
		&& (v1._x < 0 || v1._y < 0 || v1._x >= WINDOW_WIDTH || v1._y >= WINDOW_HEIGHT)
		&& (v2._x < 0 || v2._y < 0 || v2._x >= WINDOW_WIDTH || v2._y >= WINDOW_HEIGHT)) {
		return SCREEN_CULLED;
	}
	else {
		return NO_CULLED;
	}
}
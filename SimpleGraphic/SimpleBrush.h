#ifndef __SIMPLE_BRUSH_H__
#define __SIMPLE_BRUSH_H__

#include "JMath.h"
class SimpleBrush{
public:
	// »­µã
	// »­Ö±Ïß
	static void DrawLine_screen(float x0, float y0, float x1, float y1);
	static void DrawLine_screen(JMath::Vec2 v1, JMath::Vec2 v2);
}

#endif
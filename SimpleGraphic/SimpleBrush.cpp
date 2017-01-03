#include "SimpleBrush.h"

// »­Ö±Ïß
void SimpleBrush::DrawLine_screen(float x0, float y0, float x1, float y1){

}
void SimpleBrush::DrawLine_screen(JMath::Vec2 v1, JMath::Vec2 v2){
	SimpleBrush::DrawLine_screen(v1._x, v1._y, v2._x, v2._y);
}
void SimpleBrush::_DrawLine_screen_bresenham(float x0, float y0, float x1, float y1){

}
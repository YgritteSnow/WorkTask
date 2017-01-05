#include "Color.h"

NormColor4::NormColor4(const ShortColor4& c){
	_x = static_cast<float>(c._x / 255.f);
	_y = static_cast<float>(c._y / 255.f);
	_z = static_cast<float>(c._z / 255.f);
	_w = static_cast<float>(c._w / 255.f);
}

ShortColor4::ShortColor4(const NormColor4& cc){
	NormColor4 c = cc.ToSaturate();
	_x = static_cast<unsigned char>(c._x * 255.f + 0.5);
	_y = static_cast<unsigned char>(c._y * 255.f + 0.5);
	_z = static_cast<unsigned char>(c._z * 255.f + 0.5);
	_w = static_cast<unsigned char>(c._w * 255.f + 0.5);
}
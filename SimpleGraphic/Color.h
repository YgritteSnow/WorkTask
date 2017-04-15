#ifndef __COLOR_H__
#define __COLOR_H__

#include "JMath.h"
#include "StructReflection.h"

class NormColor4;
class ShortColor4;

class NormColor4 : public FloatColor{
public:
	NormColor4() :FloatColor(){}
	NormColor4(const FloatColor& c) :FloatColor(c){}
	explicit NormColor4(float r, float g, float b, float a) :FloatColor(r, g, b, a){}
	explicit NormColor4(const ShortColor4& c);
	NormColor4& operator=(const StructWrapper& s) {
		const auto& color = *static_cast<const NormColor4*>(static_cast<const void*>(s.value + StructReflectManager::GetOffset<COLOR>(s.vid, 0)));
		return this->operator=(color);
	}
};

class ShortColor4 : public Int8Color{
public:
	ShortColor4() :Int8Color(){}
	ShortColor4(const Int8Color& c) :Int8Color(c){}
	explicit ShortColor4(unsigned char r, unsigned char g, unsigned char b, unsigned char a):Int8Color(r, g, b, a) {}
	explicit ShortColor4(const NormColor4& c);
};

inline
NormColor4 BlendColor(const NormColor4& c_src, const NormColor4& c_dst) {
	return c_src * (1 - c_dst._w) + c_dst * c_dst._w;
}

#endif
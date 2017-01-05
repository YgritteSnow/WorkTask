#ifndef __COLOR_H__
#define __COLOR_H__

#include "JMath.h"

class NormColor4;
class ShortColor4;

class NormColor4 : public JMath::Vec4<float>{
public:
	NormColor4() :JMath::Vec4<float>(){}
	NormColor4(const JMath::Vec4<float>& c) :JMath::Vec4<float>(c){}
	explicit NormColor4(float r, float g, float b, float a) :JMath::Vec4<float>(r, g, b, a){}
	explicit NormColor4(const ShortColor4& c);
};

class ShortColor4 : public JMath::Vec4<unsigned char>{
public:
	ShortColor4() :JMath::Vec4<unsigned char>(){}
	ShortColor4(const JMath::Vec4<unsigned char>& c) :JMath::Vec4<unsigned char>(c){}
	explicit ShortColor4(unsigned char r, unsigned char g, unsigned char b, unsigned char a):JMath::Vec4<unsigned char>(r, g, b, a) {}
	explicit ShortColor4(const NormColor4& c);
};

typedef NormColor4 LongColor4;

#endif
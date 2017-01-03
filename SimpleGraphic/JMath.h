#ifndef __JMath_H__
#define __JMath_H__

namespace JMath{

	/* *********************************************
	* Vector3
	* *********************************************/
	class Vec3{
	public:
		Vec3() :_x(0), _y(0), _z(0){};
		Vec3(float x, float y, float z) :_x(x), _y(y), _z(z){}
	public:
		// 加 / 减 / 点积 / 叉积
		Vec3 Add(const Vec3& v) const;
		Vec3 Minus(const Vec3& v) const;
		float DotProduct(const Vec3& v) const;
		Vec3 CrossProduct(const Vec3& v) const;
	public:
		float _x, _y, _z;
	};
	// 加 / 减 / 点积 / 叉积
	Vec3 Add(const Vec3& v1, const Vec3& v2);
	Vec3 Minus(const Vec3& v1, const Vec3& v2);
	float DotProduct(const Vec3& v1, const Vec3& v2);
	Vec3 CrossProduct(const Vec3& v1, const Vec3& v2);

	/* *********************************************
	* Vector2
	* *********************************************/
	class Vec2{
	public:
		Vec2() :_x(0), _y(0){};
		Vec2(float x, float y) : _x(x), _y(y){};
	public:
		float _x, _y;
	};

	/* *********************************************
	* Matrix 4x4
	* *********************************************/
	class Mat44{
	private:
		float _m00, _m01, _m02, _m03;
		float _m10, _m11, _m12, _m13;
		float _m20, _m21, _m22, _m23;
		float _m30, _m31, _m32, _m33;
	};

	/* *********************************************
	* Triangle
	* *********************************************/
	class Triangle{
	public:
		Vec3 a;
		Vec3 b;
		Vec3 c;
	};

}
#endif
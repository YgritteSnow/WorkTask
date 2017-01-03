#ifndef __JMath_H__
#define __JMath_H__

namespace JMath {
	/* *********************************************
	* Vector3
	* *********************************************/
	template<typename DataType>
	class Vec3 {
	public:
		Vec3() :_x(0), _y(0), _z(0) {};
		Vec3(DataType x, DataType y, DataType z) :_x(x), _y(y), _z(z) {}
	public:
		// 加 / 减 / 点积 / 叉积
		Vec3 Add(const Vec3& v) const {
			return Vec3(
				this->_x + v._x,
				this->_y + v._y,
				this->_z + v._z);
		}
		Vec3 Minus(const Vec3& v) const {
			return Vec3(
				this->_x - v._x,
				this->_y - v._y,
				this->_z - v._z);
		}
		DataType DotProduct(const Vec3& v) const {
			return (
				this->_x * v._x +
				this->_y * v._y +
				this->_z * v._z);
		}
		Vec3 CrossProduct(const Vec3& v) const {
			return Vec3(
				this->_y * v._z - this->_z * v._y,
				this->_z * v._x - this->_x * v._z,
				this->_x * v._y - this->_y * v._x
			);
		}
		// 加 / 减 / 点积 / 叉积
		friend Vec3 Add(const Vec3& v1, const Vec3& v2) { return v1.Add(v2); }
		friend Vec3 Minus(const Vec3& v1, const Vec3& v2) { return v1.Minus(v2); }
		friend DataType DotProduct(const Vec3& v1, const Vec3& v2) { return v1.DotProduct(v2); }
		friend Vec3 CrossProduct(const Vec3& v1, const Vec3& v2) { return v1.CrossProduct(v2); }
	public:
		float _x, _y, _z;
	};

	/* *********************************************
	* Vector2
	* *********************************************/
	template< typename DataType>
	class Vec2 {
	public:
		Vec2() :_x(0), _y(0) {};
		Vec2(DataType x, DataType y) : _x(x), _y(y) {};
	public:
		DataType _x, _y;
	};
	/* *********************************************
	* Vector4
	* *********************************************/
	template< typename DataType>
	class Vec4 {
	public:
		Vec4() :_x(0), _y(0), _z(0), _w(0) {};
		Vec4(DataType x, DataType y, DataType z, DataType w) : _x(x), _y(y), _z(z), _w(w) {};
	public:
		DataType _x, _y, _z, _w;
	};

	/* *********************************************
	* Matrix 4x4
	* *********************************************/
	class Mat44 {
	private:
		float _m00, _m01, _m02, _m03;
		float _m10, _m11, _m12, _m13;
		float _m20, _m21, _m22, _m23;
		float _m30, _m31, _m32, _m33;
	};
}

typedef JMath::Vec4<char> Color4;
typedef JMath::Vec2<float> ScreenPos;
typedef JMath::Vec3<float> WorldPos;
typedef JMath::Vec4<float> HomoPos;
typedef unsigned ScreenCoord;

namespace JMath{
	/* *********************************************
	* Triangle
	* *********************************************/
	class Triangle{
	public:
		WorldPos a;
		WorldPos b;
		WorldPos c;
	};

}
#endif
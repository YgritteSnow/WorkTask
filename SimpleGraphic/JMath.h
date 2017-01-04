#ifndef __JMath_H__
#define __JMath_H__

#include <memory.h>
#include <cmath>

namespace JMath {
	/* *********************************************
	* common
	* *********************************************/
	extern const float EPSL;
	bool f_equal(float l, float r);

	/* *********************************************
	* Vector3
	* *********************************************/
	template <typename DataType>
	class Vec4;

	template<typename DataType>
	class Vec3 {
	public:
		Vec3() :_x(0), _y(0), _z(0) {};
		Vec3(DataType x, DataType y, DataType z) :_x(x), _y(y), _z(z) {}
	public:
		// todo 可以用SIMD和asm优化
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

		// 标量运算
		Vec3 Add(float f) const {
			return Vec3(
				this->_x + f,
				this->_y + f,
				this->_z + f);
		}
		Vec3 Minus(float f) const {
			return this->Add(-f);
		}
		Vec3 Mul(float f) const {
			return Vec3(
				this->_x * f,
				this->_y * f,
				this->_z * f);
		}
		Vec3 Div(float f) const {
			return Mul(1.f / f);
		}
		// 归一化
		void Normalise(){
			float length = this->DotProduct(*this);
			if (f_equal(length, 0)){
				return;
			}
			*this = this->Div(std::sqrt(length));
		}
		// 归零
		void Zero(){
			this->_x = 0;
			this->_y = 0;
			this->_z = 0;
		}

		// 转换为齐次坐标
		Vec4<DataType> ToVec4Pos(){ return Vec4<DataType>(_x, _y, _z, 1); }
		Vec4<DataType> ToVec4Dir(){ return Vec4<DataType>(_x, _y, _z, 0); }
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
		Vec4 Mul(const Vec4& other) const {
			return Vec4(_x*other._x, _y*other._y, _z*other._z, _w*other._w);
		}
		Vec4 operator+(const Vec4& other) const {
			return Vec4(_x + other._x, _y + other._y, _z + other._z, _w + other._w);
		}
		Vec4 operator*(float f) const {
			return Vec4(_x*f, _y*f, _z*f, _w*f);
		}
	public:
		Vec3<DataType> ToVec3(){ return Vec3<DataType>(_x / _w, _y / _w, _z / _w); }
	public:
		DataType _x, _y, _z, _w;
	};
}

typedef JMath::Vec4<unsigned char> Color4;
typedef JMath::Vec2<float> ScreenPos;
typedef JMath::Vec3<float> WorldPos;
typedef JMath::Vec4<float> HomoPos;
typedef int ScreenCoord;

namespace JMath{
	/* *********************************************
	* Matrix 4x4
	* *********************************************/
	class Mat44 {
	public:
		Mat44(){memset(_m, 0, sizeof(float)* 4 * 4); };
		Mat44(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33
			){
			_m[0][0] = m00; _m[0][1] = m01; _m[0][2] = m02; _m[0][3] = m03;
			_m[1][0] = m10; _m[1][1] = m11; _m[1][2] = m12; _m[1][3] = m13;
			_m[2][0] = m20; _m[2][1] = m21; _m[2][2] = m22; _m[2][3] = m23;
			_m[3][0] = m30; _m[3][1] = m31; _m[3][2] = m32; _m[3][3] = m33;
		}
		Mat44(
			HomoPos v1, HomoPos v2, HomoPos v3, HomoPos v4
			){
			_m[0][0] = v1._x; _m[0][1] = v1._y; _m[0][2] = v1._z; _m[0][3] = v1._w;
			_m[1][0] = v2._x; _m[1][1] = v2._y; _m[1][2] = v2._z; _m[1][3] = v2._w;
			_m[2][0] = v3._x; _m[2][1] = v3._y; _m[2][2] = v3._z; _m[2][3] = v3._w;
			_m[3][0] = v4._x; _m[3][1] = v4._y; _m[3][2] = v4._z; _m[3][3] = v4._w;
		}
		void SetZero();
		void SetIdentity();
		void SetTranslate(float x, float y, float z);
		void SetTranslate(WorldPos pos);
		void SetViewMat(WorldPos lookat, WorldPos cameraPos, WorldPos upDirect);
		void SetProjMat(float fov, float aspect, float nearPlane, float farPlane);
		void SetRow(int row, float m0, float m1, float m2, float m3);
		HomoPos GetRow(int row) const;
		HomoPos GetCol(int col) const;

		HomoPos PostMulVec(HomoPos pos) const;
		HomoPos PreMulVec(HomoPos pos) const;
		Mat44 PostMulMat(const Mat44& other) const;
		Mat44 PreMulMat(const Mat44& other) const;
	private:
		float _m[4][4];
	};
}

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

	/* *********************************************
	* Line
	* *********************************************/
	class Line{
	public:
		WorldPos a;
		WorldPos b;
	};
}

#endif
#ifndef __JMath_H__
#define __JMath_H__

#include <memory.h>
#include <cmath>

template <typename DataType>
DataType saturate(DataType d){
	return d < 0 ? 0 : d;
}

namespace JMath {
	/* *********************************************
	* common
	* *********************************************/
	extern const float EPSL;
	bool f_equal(float l, float r);
	extern const float PI;

	/* *********************************************
	* Vector3
	* *********************************************/
	template <typename DataType>
	class Vec4;
	template <typename DataType>
	class Vec2;

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
		Vec3 Mul(const Vec3& v)const {
			return Vec3(
				this->_x * v._x,
				this->_y * v._y,
				this->_z * v._z);
		}
		Vec3 Div(const Vec3& v)const {
			return Vec3(
				this->_x / v._x,
				this->_y / v._y,
				this->_z / v._z);
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
		friend Vec3 Mul(const Vec3& v1, const Vec3& v2) { return v1.Mul(v2); }
		friend Vec3 Div(const Vec3& v1, const Vec3& v2) { return v1.Div(v2); }
		friend DataType DotProduct(const Vec3& v1, const Vec3& v2) { return v1.DotProduct(v2); }
		friend Vec3 CrossProduct(const Vec3& v1, const Vec3& v2) { return v1.CrossProduct(v2); }

		friend Vec3 operator+(const Vec3& v1, const Vec3& v2) { return v1.Add(v2); }
		friend Vec3 operator-(const Vec3& v1, const Vec3& v2) { return v1.Minus(v2); }
		friend Vec3 operator*(const Vec3& v1, const Vec3& v2) { return v1.Mul(v2); }
		friend Vec3 operator/(const Vec3& v1, const Vec3& v2) { return v1.Div(v2); }
		Vec3 operator+=(const Vec3& v) { *this = *this + v; return *this; }
		Vec3 operator-=(const Vec3& v) { *this = *this - v; return *this; }

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
		Vec2<DataType> ToVec2(){ return Vec2<DataType>(_x, _y); }
	public:
		DataType _x, _y, _z;
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
		Vec2 operator- (const Vec2& other) const{
			return Vec2(_x - other._x, _y - other._y);
		}
		Vec2 operator*(const Vec2& other) const{
			return Vec2(_x * other._x, _y * other._y);
		}
		Vec2 operator+(const Vec2& other) const{
			return Vec2(_x + other._x, _y + other._y);
		}
		Vec2 operator*(float f) const{
			return Vec2(_x * f, _y * f);
		}
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
		template <typename OtherDataType>
		explicit Vec4(const Vec4<OtherDataType>& v) :_x(v._x), _y(v._y), _z(v._z), _w(v._w){}
	public:
		Vec4 Mul(const Vec4& other) const {
			return Vec4(_x*other._x, _y*other._y, _z*other._z, _w*other._w);
		}
		Vec4 Pow(float f) const {
			return Vec4(pow(_x, f),
				pow(_y, f),
				pow(_z, f),
				pow(_w, f)
			);
		}
		Vec4 ReversePow(float f) const {
			return Vec4(pow(f, _x),
				pow(f, _y),
				pow(f, _z),
				pow(f, _w)
			);
		}
		Vec4 operator+=(const Vec4& v){
			*this = *this + v;
			return *this;
		}
		Vec4 operator*=(const Vec4& v) {
			*this = *this * v;
			return *this;
		}
		Vec4 operator/ (float f) const {
			return Vec4(this->_x / f, this->_y / f, this->_z / f, this->_w / f);
		}
		friend Vec4 operator+(const Vec4& lhm, const Vec4& rhm) {
			return Vec4(lhm._x + rhm._x, lhm._y + rhm._y, lhm._z + rhm._z, lhm._w + rhm._w);
		}
		friend Vec4 operator+(const Vec4& lhm, float rhm) {
			return Vec4(rhm + lhm._x, rhm + lhm._y, rhm + lhm._z, rhm + lhm._w);
		}
		friend Vec4 operator-(const Vec4& lhm, const Vec4& rhm) {
			return Vec4(lhm._x - rhm._x, lhm._y - rhm._y, lhm._z - rhm._z, lhm._w - rhm._w);
		}
		friend Vec4 operator-(const Vec4& lhm, float rhm) {
			return lhm + -rhm;
		}
		friend Vec4 operator*(const Vec4& lhm, const Vec4& rhm) {
			return Vec4(lhm._x*rhm._x, lhm._y*rhm._y, lhm._z*rhm._z, lhm._w*rhm._w);
		}
		friend Vec4 operator*(const Vec4& lhm, float f) {
			return Vec4(lhm._x*f, lhm._y*f, lhm._z*f, lhm._w*f);
		}
		friend Vec4 operator*(DataType f, const Vec4& lhm) {
			return Vec4(lhm._x*f, lhm._y*f, lhm._z*f, lhm._w*f);
		}
	public:
		Vec3<DataType> ToVec3Homo(){ return Vec3<DataType>(_x / _w, _y / _w, _z / _w); }
		Vec3<DataType> ToVec3(){ return Vec3<DataType>(_x, _y, _z); }
		Vec4 ToSaturate() const {
			return Vec4(
				(_x > 1) ? 1 : ((_x < 0) ? 0 : _x),
				(_y > 1) ? 1 : ((_y < 0) ? 0 : _y),
				(_z > 1) ? 1 : ((_z < 0) ? 0 : _z),
				(_w > 1) ? 1 : ((_w < 0) ? 0 : _w));
		}
		Vec4 Zero() {
			this->_x = this->_y = this->_z = this->_w = 0;
			return *this;
		}
	public:
		DataType _x, _y, _z, _w;
	};
}

typedef JMath::Vec3<float> ScreenPos;
typedef JMath::Vec3<float> WorldPos;
typedef JMath::Vec4<float> HomoPos;
typedef JMath::Vec2<float> UVPos;
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
		static Mat44 GenByRotateXYZ(float x, float y, float z);
		static Mat44 GenByRotateZ(float y);
		static Mat44 GenByRotateY(float y);
		static Mat44 GenByRotateX(float y);
		static Mat44 GenByTranslate(float x, float y, float z);

		void SetZero();
		void SetIdentity();
		void SetTranslate(float x, float y, float z);
		void SetTranslate(WorldPos pos);
		void SetViewMat(WorldPos lookat, WorldPos cameraPos, WorldPos upDirect);
		void SetProjMat(float fov, float aspect, float nearPlane, float farPlane);
		void SetRow(int row, float m0, float m1, float m2, float m3);
		HomoPos GetRow(int row) const;
		HomoPos GetCol(int col) const;
		WorldPos Mat44::GetTranslate() const;

		HomoPos PostMulVec(HomoPos pos) const;
		HomoPos PreMulVec(HomoPos pos) const;
		Mat44 PostMulMat(const Mat44& other) const;
		Mat44 PreMulMat(const Mat44& other) const;
		Mat44 RotateXYZ(float y, float p, float r) const;
		Mat44 Translate(float x, float y, float z) const;
		float Determinant() const;
		Mat44 Inverse() const;
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
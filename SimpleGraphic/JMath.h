#ifndef __JMath_H__
#define __JMath_H__

#include <memory.h>
#include <cmath>

template <typename DataType>
DataType saturate(DataType d){
	return d < 0 ? 0 : d;
}

template <typename DataType>
DataType lerp(DataType l, DataType r, float lerp_para) {
	return l*(1-lerp_para) + r*lerp_para;
}

namespace JMath {
	/* *********************************************
	* common
	* *********************************************/
	extern const float EPSL;
	bool f_equal(float l, float r);
	extern const float PI;
	extern const float PI_M2;

	/* *********************************************
	* Vector3
	* *********************************************/
	template <typename DataType>
	class _Vec4;
	template <typename DataType>
	class _Vec2;

	template<typename DataType>
	class _Vec3 {
	public:
		_Vec3() :_x(0), _y(0), _z(0) {};
		_Vec3(DataType x, DataType y, DataType z) :_x(x), _y(y), _z(z) {}
	public:
		// todo 可以用SIMD和asm优化
		// 加 / 减 / 点积 / 叉积
		_Vec3 Add(const _Vec3& v) const {
			return _Vec3(
				this->_x + v._x,
				this->_y + v._y,
				this->_z + v._z);
		}
		_Vec3 Minus(const _Vec3& v) const {
			return _Vec3(
				this->_x - v._x,
				this->_y - v._y,
				this->_z - v._z);
		}
		_Vec3 Mul(const _Vec3& v)const {
			return _Vec3(
				this->_x * v._x,
				this->_y * v._y,
				this->_z * v._z);
		}
		_Vec3 Div(const _Vec3& v)const {
			return _Vec3(
				this->_x / v._x,
				this->_y / v._y,
				this->_z / v._z);
		}
		DataType DotProduct(const _Vec3& v) const {
			return (
				this->_x * v._x +
				this->_y * v._y +
				this->_z * v._z);
		}
		_Vec3 CrossProduct(const _Vec3& v) const {
			return _Vec3(
				this->_y * v._z - this->_z * v._y,
				this->_z * v._x - this->_x * v._z,
				this->_x * v._y - this->_y * v._x
				);
		}
		// 加 / 减 / 点积 / 叉积
		friend _Vec3 Add(const _Vec3& v1, const _Vec3& v2) { return v1.Add(v2); }
		friend _Vec3 Minus(const _Vec3& v1, const _Vec3& v2) { return v1.Minus(v2); }
		friend _Vec3 Mul(const _Vec3& v1, const _Vec3& v2) { return v1.Mul(v2); }
		friend _Vec3 Mul(const _Vec3& v1, DataType v2) { return v1.Mul(v2); }
		friend _Vec3 Div(const _Vec3& v1, const _Vec3& v2) { return v1.Div(v2); }
		friend DataType DotProduct(const _Vec3& v1, const _Vec3& v2) { return v1.DotProduct(v2); }
		friend _Vec3 CrossProduct(const _Vec3& v1, const _Vec3& v2) { return v1.CrossProduct(v2); }

		friend _Vec3 operator+(const _Vec3& v1, const _Vec3& v2) { return v1.Add(v2); }
		friend _Vec3 operator-(const _Vec3& v1, const _Vec3& v2) { return v1.Minus(v2); }
		friend _Vec3 operator*(const _Vec3& v1, const _Vec3& v2) { return v1.Mul(v2); }
		friend _Vec3 operator*(const _Vec3& v1, DataType v2) { return v1.Mul(v2); }
		friend _Vec3 operator*(DataType v2, const _Vec3& v1) { return v1.Mul(v2); }
		friend _Vec3 operator/(const _Vec3& v1, const _Vec3& v2) { return v1.Div(v2); }
		friend _Vec3 operator/(DataType v2, const _Vec3& v1) { return v1.Div(v2); }
		
		_Vec3 operator+=(const _Vec3& v) { *this = *this + v; return *this; }
		_Vec3 operator-=(const _Vec3& v) { *this = *this - v; return *this; }
		_Vec3 operator-() { return _Vec3(-_x, -_y, -_z); }

		// 标量运算
		_Vec3 Add(DataType f) const {
			return _Vec3(
				this->_x + f,
				this->_y + f,
				this->_z + f);
		}
		_Vec3 Minus(DataType f) const {
			return this->Add(-f);
		}
		_Vec3 Mul(DataType f) const {
			return _Vec3(
				this->_x * f,
				this->_y * f,
				this->_z * f);
		}
		_Vec3 Div(DataType f) const {
			return Mul(1 / f);
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
		_Vec4<DataType> ToVec4Pos() const { return _Vec4<DataType>(_x, _y, _z, 1); }
		_Vec4<DataType> ToVec4Dir() const { return _Vec4<DataType>(_x, _y, _z, 0); }
		_Vec2<DataType> ToVec2() const { return _Vec2<DataType>(_x, _y); }
	public:
		DataType _x, _y, _z;
	};

	/* *********************************************
	* Vector2
	* *********************************************/
	template< typename DataType>
	class _Vec2 {
	public:
		_Vec2() :_x(0), _y(0) {};
		_Vec2(DataType x, DataType y) : _x(x), _y(y) {};
	public:
		_Vec2 operator- (const _Vec2& other) const{
			return _Vec2(_x - other._x, _y - other._y);
		}
		_Vec2 operator*(const _Vec2& other) const{
			return _Vec2(_x * other._x, _y * other._y);
		}
		_Vec2 operator+(const _Vec2& other) const{
			return _Vec2(_x + other._x, _y + other._y);
		}
		_Vec2 operator*(float f) const{
			return _Vec2(_x * f, _y * f);
		}
	public:
		DataType _x, _y;
	};
	/* *********************************************
	* Vector4
	* *********************************************/
	template< typename DataType>
	class _Vec4 {
	public:
		_Vec4() :_x(0), _y(0), _z(0), _w(0) {};
		_Vec4(DataType x, DataType y, DataType z, DataType w) : _x(x), _y(y), _z(z), _w(w) {};
		template <typename OtherDataType>
		explicit _Vec4(const _Vec4<OtherDataType>& v) :_x(v._x), _y(v._y), _z(v._z), _w(v._w){}
	public:
		_Vec4 Mul(const _Vec4& other) const {
			return _Vec4(_x*other._x, _y*other._y, _z*other._z, _w*other._w);
		}
		_Vec4 Pow(float f) const {
			return _Vec4(pow(_x, f),
				pow(_y, f),
				pow(_z, f),
				pow(_w, f)
			);
		}
		_Vec4 ReversePow(float f) const {
			return _Vec4(pow(f, _x),
				pow(f, _y),
				pow(f, _z),
				pow(f, _w)
			);
		}
		_Vec4 operator+=(const _Vec4& v){
			*this = *this + v;
			return *this;
		}
		_Vec4 operator*=(const _Vec4& v) {
			*this = *this * v;
			return *this;
		}
		_Vec4 operator/ (float f) const {
			return _Vec4(this->_x / f, this->_y / f, this->_z / f, this->_w / f);
		}
		friend _Vec4 operator+(const _Vec4& lhm, const _Vec4& rhm) {
			return _Vec4(lhm._x + rhm._x, lhm._y + rhm._y, lhm._z + rhm._z, lhm._w + rhm._w);
		}
		friend _Vec4 operator+(const _Vec4& lhm, float rhm) {
			return _Vec4(rhm + lhm._x, rhm + lhm._y, rhm + lhm._z, rhm + lhm._w);
		}
		friend _Vec4 operator-(const _Vec4& lhm, const _Vec4& rhm) {
			return _Vec4(lhm._x - rhm._x, lhm._y - rhm._y, lhm._z - rhm._z, lhm._w - rhm._w);
		}
		friend _Vec4 operator-(const _Vec4& lhm, float rhm) {
			return lhm + -rhm;
		}
		friend _Vec4 operator*(const _Vec4& lhm, const _Vec4& rhm) {
			return _Vec4(lhm._x*rhm._x, lhm._y*rhm._y, lhm._z*rhm._z, lhm._w*rhm._w);
		}
		friend _Vec4 operator*(const _Vec4& lhm, float f) {
			return _Vec4(lhm._x*f, lhm._y*f, lhm._z*f, lhm._w*f);
		}
		friend _Vec4 operator*(DataType f, const _Vec4& lhm) {
			return _Vec4(lhm._x*f, lhm._y*f, lhm._z*f, lhm._w*f);
		}
	public:
		_Vec3<DataType> ToVec3Homo(){ return _Vec3<DataType>(_x / _w, _y / _w, _z / _w); }
		_Vec3<DataType> ToVec3(){ return _Vec3<DataType>(_x, _y, _z); }
		_Vec4 ToSaturate() const {
			return _Vec4(
				(_x > 1) ? 1 : ((_x < 0) ? 0 : _x),
				(_y > 1) ? 1 : ((_y < 0) ? 0 : _y),
				(_z > 1) ? 1 : ((_z < 0) ? 0 : _z),
				(_w > 1) ? 1 : ((_w < 0) ? 0 : _w));
		}
		_Vec4 Zero() {
			this->_x = this->_y = this->_z = this->_w = 0;
			return *this;
		}
	public:
		DataType _x, _y, _z, _w;
	};
}

/************************************************************************/
/* Type wrapper                                                         */
/************************************************************************/
typedef JMath::_Vec3<float> ScreenPos;
typedef JMath::_Vec3<float> WorldPos;
typedef JMath::_Vec4<float> HomoPos;
typedef JMath::_Vec2<float> UVPos;
typedef JMath::_Vec4<float> FloatColor;
typedef JMath::_Vec4<unsigned char> Int8Color;
typedef int ScreenCoord;

namespace JMath{
	/* *********************************************
	* Matrix 4x4
	* *********************************************/
	class _Mat44 {
	public:
		_Mat44(){memset(_m, 0, sizeof(float)* 4 * 4); };
		_Mat44(
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
		_Mat44(
			HomoPos v1, HomoPos v2, HomoPos v3, HomoPos v4
			){
			_m[0][0] = v1._x; _m[0][1] = v1._y; _m[0][2] = v1._z; _m[0][3] = v1._w;
			_m[1][0] = v2._x; _m[1][1] = v2._y; _m[1][2] = v2._z; _m[1][3] = v2._w;
			_m[2][0] = v3._x; _m[2][1] = v3._y; _m[2][2] = v3._z; _m[2][3] = v3._w;
			_m[3][0] = v4._x; _m[3][1] = v4._y; _m[3][2] = v4._z; _m[3][3] = v4._w;
		}
		static _Mat44 GenByRotateXYZ(float x, float y, float z);
		static _Mat44 GenByRotateZ(float y);
		static _Mat44 GenByRotateY(float y);
		static _Mat44 GenByRotateX(float y);
		static _Mat44 GenByTranslate(float x, float y, float z);

		void SetZero();
		void SetIdentity();
		void SetTranslate(float x, float y, float z);
		void SetTranslate(WorldPos pos);
		void SetViewMat(WorldPos lookat, WorldPos cameraPos, WorldPos upDirect);
		void SetProjMat(float fov, float aspect, float nearPlane, float farPlane);
		void SetRow(int row, float m0, float m1, float m2, float m3);
		HomoPos GetRow(int row) const;
		void SetCol(int col, float m0, float m1, float m2, float m3);
		HomoPos GetCol(int col) const;
		WorldPos _Mat44::GetTranslate() const;

		HomoPos PostMulVec(HomoPos pos) const;
		HomoPos PreMulVec(HomoPos pos) const;
		_Mat44 PostMulMat(const _Mat44& other) const;
		_Mat44 PreMulMat(const _Mat44& other) const;
		_Mat44 RotateXYZ(float y, float p, float r) const;
		_Mat44 Translate(float x, float y, float z) const;
		float Determinant() const;
		_Mat44 Inverse() const;
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

namespace JMath {
	/************************************************************************/
	/* Quaternion                                                           */
	/************************************************************************/
	template< typename DataType>
	class _Quaternion {
	public:
		_Quaternion() :_x(0), _y(0), _z(0), _w(1) {}
		_Quaternion(DataType x, DataType y, DataType z, DataType w)
			: _x(x), _y(y), _z(z), _w(w) {}
		explicit _Quaternion(const WorldPos& dir)
			: _x(dir._x), _y(dir._y), _z(dir._z), _w(0) {}
	public:
		// 赋值函数
		void SetByDirAndAng(DataType x, DataType y, DataType z, DataType ang) {
			_x = std::cos(ang) * x;
			_y = std::cos(ang) * y;
			_z = std::cos(ang) * z;
			_w = std::sin(ang);
		}

		// 计算相关
		DataType LengthSquare() const { return _x*_x + _y*_y + _z*_z + _w*_w; }
		DataType Length() const { return std::sqrt(LengthSquare()); }
		_Quaternion Scale(float s) const { return _Quaternion(_x*s, _y*s, _z*s, _w*s); }
		_Quaternion Conjugate() const { return _Quaternion(-_x, -_y, -_z, _w); }
		_Quaternion Inverse() const { return Conjugate() / LengthSquare(); }
		DataType DotProduct(const _Quaternion& other) const { return _x*other._x + _y*other._y + _z*other._z + _w*other._w; }

		// 旋转所使用的格拉斯曼积
		// Grassmann product:
		// if:
		//	p = (u, s) = (a, b, c, s)
		//	q = (v, w) = (x, y, z, w)
		// then:
		//	p * q	= ( (uw + sv + u x v), (u.v + sw) )
		//			= (	aw + sx + (bz - cy)
		//				bw + sy + (cx - az)
		//				cw + sz + (ay - bx)
		//				(ax + by + cz) + sw )
		//			= (	aw + bz - cy + sx
		//				-az+ bw + cx + sy
		//				ay - bx + cw + sz
		//				ax + by + cz + sw )
		_Quaternion operator*(DataType other) {
			return _Quaternion(_x * other, _y * other, _z * other, _w * other);
		}
		friend _Quaternion operator*(DataType lhm, const _Quaternion& rhm) { return rhm.operator*(lhm); }
		friend _Quaternion operator*(const _Quaternion& lhm, const _Quaternion& rhm) {
			return _Quaternion(
				 lhm._x * rhm._w + lhm._y * rhm._z - lhm._z * rhm._y + lhm._w * rhm._x,
				-lhm._x * rhm._z + lhm._y * rhm._w + lhm._z * rhm._x + lhm._w * rhm._y,
				 lhm._x * rhm._y - lhm._y * rhm._x + lhm._z * rhm._w + lhm._w * rhm._z,
				-lhm._x * rhm._x - lhm._y * rhm._y - lhm._z * rhm._z + lhm._w * rhm._w);
		}

		// 旋转方向
		WorldPos RotateVec(const WorldPos& other) const {
			auto tmp = this->Conjugate() * _Quaternion(other) * *this;
			return WorldPos(tmp._x, tmp._y, tmp._z);
		}

		// 转换为矩阵
		// R = (1-2yy-2zz,	2xy+2zw,	2xy-2yw,
		//		2xy-2zw,	1-2xx-2zz,	2yz+2xw,
		//		2xz+2yw,	2yz-2xw,	1-2xx-2yy)
		_Mat44 toMatrix44(WorldPos trans, WorldPos scale) {
			return _Mat44(	scale._x * (1 - 2*_y*_y - 2*_z*_z	),	scale._y * (2*_x*_y + 2*_z*_w		),	scale._z * (2*_x*_y - 2*_y*_w		),	trans._x,
							scale._x * (2*_x*_y - 2*_z*_w		),	scale._y * (1 - 2*_x*_x - 2*_z*_z	),	scale._z * (2*_y*_z + 2*_x*_w		),	trans._y,
							scale._x * (2*_x*_z + 2*_y*_w		),	scale._y * (2*_y*_z - 2*_x*_w		),	scale._z * (1 - 2*_x*_x - 2*_y*_y	),	trans._z,
							0									,	0									,	0									,	1
			);
		}

		// 插值
		_Quaternion Slerp(const _Quaternion& other, float lerp_para) {
			float cos_theta = this->DotProduct(other);
			if (f_equal(cos_theta, 1.f)) {
				return _Quaternion(
					(1 - lerp_para) * _x + lerp_para * other._x,
					(1 - lerp_para) * _y + lerp_para * other._y,
					(1 - lerp_para) * _z + lerp_para * other._z,
					(1 - lerp_para) * _w + lerp_para * other._w
				);
			}
			if (f_equal(cos_theta, -1.f)) {
				// todo 这种异常情况需要完善
				return *this;
			}

			float theta = std::acos(cos_theta);
			float sin_theta = std::sin(theta);
			float sin_theta_r = std::sin(lerp_para * theta);
			float sin_theta_l = std::sin((1 - lerp_para)*theta);
			return this->Scale(sin_theta_l / sin_theta).Add(other.Scale(sin_theta_r / sin_theta));
		}
	private:
		_Quaternion Add(const _Quaternion& other) {
			return _Quaternion(_x + other._x, _y + other._y, _z + other._z, _w + other._w);
		}
	public:
		DataType _x, _y, _z, _w;
	};
};

/************************************************************************/
/* Type wrapper                                                         */
/************************************************************************/
typedef JMath::_Mat44 Matrix44;
typedef JMath::_Quaternion<float> Quaternion;

#endif
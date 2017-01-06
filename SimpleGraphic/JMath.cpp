#include "JMath.h"

#include <cmath>
#include <memory.h>

namespace JMath{
	/* *********************************************
	* common
	* *********************************************/
	const float EPSL = 0.0001f; // todo 
	bool f_equal(float l, float r) { return std::fabs(l - r) < EPSL; }
	extern const float PI = 3.14159265359;

	/* *********************************************
	* Matrix
	* *********************************************/
	void Mat44::SetZero(){
		memset(&_m, 0, sizeof(float)* 4 * 4);
	}

	void Mat44::SetIdentity(){
		this->SetZero();
		_m[0][0] = 1.f;
		_m[1][1] = 1.f;
		_m[2][2] = 1.f;
		_m[3][3] = 1.f;
	}

	void Mat44::SetTranslate(float x, float y, float z){
		this->SetIdentity();
		SetRow(3, x, y, z, 1);
	}

	void Mat44::SetTranslate(WorldPos pos){
		this->SetTranslate(pos._x, pos._y, pos._z);
	}

	void Mat44::SetViewMat(WorldPos lookat, WorldPos cameraPos, WorldPos upDirect){
		// 以 lookat 的方向为准，upDirect 不一定是摄像机的正上方
		lookat.Normalise();
		auto left = lookat.CrossProduct(upDirect);
		left.Normalise();
		auto up = left.CrossProduct(lookat);
		SetRow(0, left._x, left._y, left._z, 0);
		SetRow(1, up._x, up._y, up._z, 0);
		SetRow(2, lookat._x, lookat._y, lookat._z, 0);
		SetRow(3, 0, 0, 0, 1);
	}

	void Mat44::SetProjMat(float fov, float aspect, float nearPlane, float farPlane){
		this->SetIdentity();
		_m[2][2] = 1;
		_m[3][2] = 1 / nearPlane;
		_m[2][3] = -1;
		_m[3][3] = 0;
	}

	void Mat44::SetRow(int row, float m0, float m1, float m2, float m3){
		_m[row][0] = m0;
		_m[row][1] = m1;
		_m[row][2] = m2;
		_m[row][3] = m3;
	}

	HomoPos Mat44::GetRow(int row) const {
		return HomoPos(
			_m[row][0], _m[row][1], _m[row][2], _m[row][3]
			);
	}
	HomoPos Mat44::GetCol(int col) const {
		return HomoPos(
			_m[0][col], _m[1][col], _m[2][col], _m[3][col]
			);
	}

	HomoPos Mat44::PostMulVec(HomoPos pos) const {
		return HomoPos(
			_m[0][0] * pos._x + _m[0][1] * pos._y + _m[0][2] * pos._z + _m[0][3] * pos._w,
			_m[1][0] * pos._x + _m[1][1] * pos._y + _m[1][2] * pos._z + _m[1][3] * pos._w,
			_m[2][0] * pos._x + _m[2][1] * pos._y + _m[2][2] * pos._z + _m[2][3] * pos._w,
			_m[3][0] * pos._x + _m[3][1] * pos._y + _m[3][2] * pos._z + _m[3][3] * pos._w
			);
	}
	HomoPos Mat44::PreMulVec(HomoPos pos) const {
		return HomoPos(
			_m[0][0] * pos._x + _m[1][0] * pos._y + _m[2][0] * pos._z + _m[3][0] * pos._w,
			_m[0][1] * pos._x + _m[1][1] * pos._y + _m[2][1] * pos._z + _m[3][1] * pos._w,
			_m[0][2] * pos._x + _m[1][2] * pos._y + _m[2][2] * pos._z + _m[3][2] * pos._w,
			_m[0][3] * pos._x + _m[1][3] * pos._y + _m[2][3] * pos._z + _m[3][3] * pos._w
			);
	}
	Mat44 Mat44::PreMulMat(const Mat44& other) const {
		return Mat44(
			other.GetRow(0) * _m[0][0] + other.GetRow(1) * _m[0][1] + other.GetRow(2) * _m[0][2] + other.GetRow(3) * _m[0][3],
			other.GetRow(0) * _m[1][0] + other.GetRow(1) * _m[1][1] + other.GetRow(2) * _m[1][2] + other.GetRow(3) * _m[1][3],
			other.GetRow(0) * _m[2][0] + other.GetRow(1) * _m[2][1] + other.GetRow(2) * _m[2][2] + other.GetRow(3) * _m[2][3],
			other.GetRow(0) * _m[3][0] + other.GetRow(1) * _m[3][1] + other.GetRow(2) * _m[3][2] + other.GetRow(3) * _m[3][3]
			);
	}
	Mat44 Mat44::PostMulMat(const Mat44& other) const {
		return other.PreMulMat(*this);
	}
	Mat44 Mat44::GenByRotateXYZ(float x, float y, float z){
		return GenByRotateZ(z).PreMulMat(GenByRotateY(y)).PreMulMat(GenByRotateX(x));
	}
	Mat44 Mat44::GenByRotateZ(float y){
		Mat44 tmp;
		tmp.SetIdentity();
		tmp._m[0][0] = std::cos(y);
		tmp._m[0][1] = std::sin(y);
		tmp._m[1][1] = std::cos(y);
		tmp._m[1][0] = -std::sin(y);
		return tmp;
	}
	Mat44 Mat44::GenByRotateY(float y){
		Mat44 tmp;
		tmp.SetIdentity();
		tmp._m[1][1] = std::cos(y);
		tmp._m[1][2] = std::sin(y);
		tmp._m[2][2] = std::cos(y);
		tmp._m[2][1] = -std::sin(y);
		return tmp;
	}
	Mat44 Mat44::GenByRotateX(float y){
		Mat44 tmp;
		tmp.SetIdentity();
		tmp._m[2][2] = std::cos(y);
		tmp._m[2][0] = std::sin(y);
		tmp._m[0][0] = std::cos(y);
		tmp._m[0][2] = -std::sin(y);
		return tmp;
	}
	Mat44 Mat44::GenByTranslate(float x, float y, float z){
		Mat44 tmp;
		tmp.SetIdentity();
		tmp._m[3][0] = x;
		tmp._m[3][1] = y;
		tmp._m[3][2] = z;
		return tmp;
	}
	Mat44 Mat44::RotateXYZ(float x, float y, float z) const {
		Mat44 tmp = GenByRotateXYZ(x, y, z);
		return this->PreMulMat(tmp);
	}
	Mat44 Mat44::Translate(float x, float y, float z) const {
		Mat44 tmp = GenByTranslate(x, y, z);
		return this->PreMulMat(tmp);
	}
}
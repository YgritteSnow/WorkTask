#include "JMath.h"

#include <cmath>
#include <memory.h>

const int MAX_N = 4;
bool GetMatrixInverse(const float src[MAX_N][MAX_N], int n, float des[MAX_N][MAX_N]);
float getDeterminant(const float arcs[MAX_N][MAX_N], int n);

namespace JMath{
	/* *********************************************
	* common
	* *********************************************/
	const float EPSL = 0.0001f; // todo 
	bool f_equal(float l, float r) { return std::fabs(l - r) < EPSL; }
	extern const float PI = 3.14159265359;
	extern const float PI_M2 = PI * 2;

	/* *********************************************
	* Matrix
	* *********************************************/
	void _Mat44::SetZero(){
		memset(&_m, 0, sizeof(float)* 4 * 4);
	}

	void _Mat44::SetIdentity(){
		this->SetZero();
		_m[0][0] = 1.f;
		_m[1][1] = 1.f;
		_m[2][2] = 1.f;
		_m[3][3] = 1.f;
	}

	void _Mat44::SetTranslate(float x, float y, float z){
		this->SetIdentity();
		SetCol(3, x, y, z, 1);
	}

	void _Mat44::SetTranslate(WorldPos pos){
		this->SetTranslate(pos._x, pos._y, pos._z);
	}

	void _Mat44::SetViewMat(WorldPos lookat, WorldPos cameraPos, WorldPos upDirect){
		// 以 lookat 的方向为准，upDirect 不一定是摄像机的正上方
		lookat.Normalise();
		auto x_dir = upDirect.CrossProduct(lookat);
		x_dir.Normalise();
		auto y_dir = lookat.CrossProduct(x_dir);
		SetCol(0, x_dir._x, x_dir._y, x_dir._z, 0);
		SetCol(1, y_dir._x, y_dir._y, y_dir._z, 0);
		SetCol(2, lookat._x, lookat._y, lookat._z, 0);
		SetCol(3, cameraPos._x, cameraPos._y, cameraPos._z, 1);
	}

	void _Mat44::SetProjMat(float fov, float aspect, float nearPlane, float farPlane){
		this->SetIdentity();
		_m[0][0] = 1 / fov;
		_m[1][1] = aspect / fov;
		//_m[2][2] = 1 / nearPlane;
		//_m[2][3] = 1 / nearPlane;
		//_m[3][2] = -1;
		_m[2][2] = -nearPlane / (farPlane - nearPlane);
		_m[2][3] = -_m[2][2] * farPlane;
		_m[3][2] = 1;
		_m[3][3] = 0;
	}

	void _Mat44::SetRow(int row, float m0, float m1, float m2, float m3){
		_m[row][0] = m0;
		_m[row][1] = m1;
		_m[row][2] = m2;
		_m[row][3] = m3;
	}

	HomoPos _Mat44::GetRow(int row) const {
		return HomoPos(
			_m[row][0], _m[row][1], _m[row][2], _m[row][3]
			);
	}

	void _Mat44::SetCol(int row, float m0, float m1, float m2, float m3) {
		_m[0][row] = m0;
		_m[1][row] = m1;
		_m[2][row] = m2;
		_m[3][row] = m3;
	}

	HomoPos _Mat44::GetCol(int col) const {
		return HomoPos(
			_m[0][col], _m[1][col], _m[2][col], _m[3][col]
			);
	}

	HomoPos _Mat44::PreMulVec(HomoPos pos) const {
		return HomoPos(
			_m[0][0] * pos._x + _m[0][1] * pos._y + _m[0][2] * pos._z + _m[0][3] * pos._w,
			_m[1][0] * pos._x + _m[1][1] * pos._y + _m[1][2] * pos._z + _m[1][3] * pos._w,
			_m[2][0] * pos._x + _m[2][1] * pos._y + _m[2][2] * pos._z + _m[2][3] * pos._w,
			_m[3][0] * pos._x + _m[3][1] * pos._y + _m[3][2] * pos._z + _m[3][3] * pos._w
			);
	}
	HomoPos _Mat44::PostMulVec(HomoPos pos) const {
		return HomoPos(
			_m[0][0] * pos._x + _m[1][0] * pos._y + _m[2][0] * pos._z + _m[3][0] * pos._w,
			_m[0][1] * pos._x + _m[1][1] * pos._y + _m[2][1] * pos._z + _m[3][1] * pos._w,
			_m[0][2] * pos._x + _m[1][2] * pos._y + _m[2][2] * pos._z + _m[3][2] * pos._w,
			_m[0][3] * pos._x + _m[1][3] * pos._y + _m[2][3] * pos._z + _m[3][3] * pos._w
			);
	}
	_Mat44 _Mat44::PreMulMat(const _Mat44& other) const {
		return _Mat44(
			other.GetRow(0) * _m[0][0] + other.GetRow(1) * _m[0][1] + other.GetRow(2) * _m[0][2] + other.GetRow(3) * _m[0][3],
			other.GetRow(0) * _m[1][0] + other.GetRow(1) * _m[1][1] + other.GetRow(2) * _m[1][2] + other.GetRow(3) * _m[1][3],
			other.GetRow(0) * _m[2][0] + other.GetRow(1) * _m[2][1] + other.GetRow(2) * _m[2][2] + other.GetRow(3) * _m[2][3],
			other.GetRow(0) * _m[3][0] + other.GetRow(1) * _m[3][1] + other.GetRow(2) * _m[3][2] + other.GetRow(3) * _m[3][3]
			);
	}
	_Mat44 _Mat44::PostMulMat(const _Mat44& other) const {
		return other.PreMulMat(*this);
	}
	_Mat44 _Mat44::GenByRotateXYZ(float x, float y, float z){
		return GenByRotateZ(z).PreMulMat(GenByRotateY(y)).PreMulMat(GenByRotateX(x));
	}
	_Mat44 _Mat44::GenByRotateZ(float y){
		_Mat44 tmp;
		tmp.SetIdentity();
		tmp._m[0][0] = std::cos(y);
		tmp._m[1][1] = std::cos(y);
		tmp._m[0][1] = -std::sin(y);
		tmp._m[1][0] = std::sin(y);
		return tmp;
	}
	_Mat44 _Mat44::GenByRotateY(float y){
		_Mat44 tmp;
		tmp.SetIdentity();
		tmp._m[1][1] = std::cos(y);
		tmp._m[2][2] = std::cos(y);
		tmp._m[1][2] = -std::sin(y);
		tmp._m[2][1] = std::sin(y);
		return tmp;
	}
	_Mat44 _Mat44::GenByRotateX(float y){
		_Mat44 tmp;
		tmp.SetIdentity();
		tmp._m[2][2] = std::cos(y);
		tmp._m[0][0] = std::cos(y);
		tmp._m[0][2] = std::sin(y);
		tmp._m[2][0] = -std::sin(y);
		return tmp;
	}
	_Mat44 _Mat44::GenByTranslate(float x, float y, float z){
		_Mat44 tmp;
		tmp.SetIdentity();
		tmp.SetCol(3, x, y, z, 1);
		return tmp;
	}
	_Mat44 _Mat44::RotateXYZ(float x, float y, float z) const {
		_Mat44 tmp = GenByRotateXYZ(x, y, z);
		return this->PreMulMat(tmp);
	}
	_Mat44 _Mat44::Translate(float x, float y, float z) const {
		_Mat44 tmp = GenByTranslate(x, y, z);
		return this->PreMulMat(tmp);
	}

	WorldPos _Mat44::GetTranslate() const {
		return WorldPos(_m[0][3], _m[1][3], _m[2][3]); 
	}

	float _Mat44::Determinant() const {
		return getDeterminant(_m, 4);
	}

	_Mat44 _Mat44::Inverse() const {
		_Mat44 res;
		GetMatrixInverse(_m, 4, res._m);
		return res;
	}
}

/* *********************************************
* 矩阵计算的函数
* *********************************************/
void  getComplementMinor(const float arcs[MAX_N][MAX_N], int n, float ans[MAX_N][MAX_N]);

//得到给定矩阵src的逆矩阵保存到des中。
bool GetMatrixInverse(const float src[MAX_N][MAX_N], int n, float des[MAX_N][MAX_N])
{
	float flag = getDeterminant(src, n);
	float t[MAX_N][MAX_N];
	if (flag == 0)
	{
		return false;
	}
	else
	{
		getComplementMinor(src, n, t);
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				des[i][j] = t[i][j] / flag;
			}

		}
	}


	return true;

}

float getDeterminant(const float arcs[MAX_N][MAX_N], int n)
{
	if (n == 1)
	{
		return arcs[0][0];
	}
	float ans = 0;
	float temp[MAX_N][MAX_N] = { 0.0 };
	int i, j, k;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n - 1; j++)
		{
			for (k = 0; k < n - 1; k++)
			{
				temp[j][k] = arcs[j + 1][(k >= i) ? k + 1 : k];

			}
		}
		float t = getDeterminant(temp, n - 1);
		if (i % 2 == 0)
		{
			ans += arcs[0][i] * t;
		}
		else
		{
			ans -= arcs[0][i] * t;
		}
	}
	return ans;
}

//计算每一行每一列的每个元素所对应的余子式，组成A*
void  getComplementMinor(const float arcs[MAX_N][MAX_N], int n, float ans[MAX_N][MAX_N])
{
	if (n == 1)
	{
		ans[0][0] = 1;
		return;
	}
	int i, j, k, t;
	float temp[MAX_N][MAX_N];
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			for (k = 0; k < n - 1; k++)
			{
				for (t = 0; t < n - 1; t++)
				{
					temp[k][t] = arcs[k >= i ? k + 1 : k][t >= j ? t + 1 : t];
				}
			}

			ans[j][i] = getDeterminant(temp, n - 1);
			if ((i + j) % 2 == 1)
			{
				ans[j][i] = -ans[j][i];
			}
		}
	}
}
#include "JMath.h"

namespace JMath{
	/* *********************************************
	* Vector
	* *********************************************/
#define _THIS_OP_4(cls_name, param_name, op) cls_name(\
	this->_x op param_name._x, \
	this->_y op param_name._y, \
	this->_z op param_name._z)

	// 加 / 减 / 点积 / 叉积
	Vec3 Vec3::Add(const Vec3& v) const{
		return _THIS_OP_4(Vec3, v, +);
	}
	Vec3 Vec3::Minus(const Vec3& v) const{
		return _THIS_OP_4(Vec3, v, -);
	}

	float Vec3::DotProduct(const Vec3& v) const{
		return (
			this->_x * v._x +
			this->_y * v._y +
			this->_z * v._z);
	}
	Vec3 Vec3::CrossProduct(const Vec3& v) const{
		return Vec3(
			this->_y * v._z - this->_z * v._y,
			this->_z * v._x - this->_x * v._z,
			this->_x * v._y - this->_y * v._x
			);
	}
	// 加 / 减 / 点积 / 叉积
	Vec3 Add(const Vec3& v1, const Vec3& v2){return v1.Add(v2);}
	Vec3 Minus(const Vec3& v1, const Vec3& v2){return v1.Minus(v2);}
	float DotProduct(const Vec3& v1, const Vec3& v2){ return v1.DotProduct(v2); }
	Vec3 CrossProduct(const Vec3& v1, const Vec3& v2){ return v1.CrossProduct(v2); }

	/* *********************************************
	* Matrix
	* *********************************************/
}
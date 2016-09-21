#include "QuaternionHelper.h"
#include <math.h>

Quaternion QuaternionHelper::Multi(Quaternion q1, Quaternion q2)
{
	Quaternion ret;
	ret.t = q1.t * q2.t - Inner(q1.v, q2.v);
	ret.v = Plus( Plus(Multi(q1.t, q2.v), Multi(q2.t, q1.v)), Outer(q1.v, q2.v));
	return ret;
}

float QuaternionHelper::Inner(VECTOR q1, VECTOR q2)
{
	float ret;
	ret = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
	return ret;
}

VECTOR QuaternionHelper::Outer(VECTOR v1, VECTOR v2)
{
	VECTOR ret;
	ret.x = v1.y * v2.z - v1.z * v2.y;
	ret.y = v1.z * v2.x - v1.x * v2.z;
	ret.z = v1.x * v2.y - v1.y * v2.x;
	return ret;
}

VECTOR QuaternionHelper::Plus(VECTOR v1, VECTOR v2)
{
	VECTOR ret;
	ret.x = v1.x + v2.x;
	ret.y = v1.y + v2.y;
	ret.z = v1.z + v2.z;
	return ret;
}

VECTOR QuaternionHelper::Minus(VECTOR v1, VECTOR v2)
{
	VECTOR ret;
	ret.x = v1.x - v2.x;
	ret.y = v1.y - v2.y;
	ret.z = v1.z - v2.z;
	return ret;
}

VECTOR QuaternionHelper::Multi(float f, VECTOR v)
{
	VECTOR ret;
	ret.x = v.x * f;
	ret.y = v.y * f;
	ret.z = v.z * f;
	return ret;
}

Quaternion QuaternionHelper::Rotation(float t, VECTOR v)
{
	Quaternion ret;
	ret.t = (float)cos(t/2.0);
	ret.v.x = (float)(v.x * sin(t/2.0));
	ret.v.y = (float)(v.y * sin(t/2.0));
	ret.v.z = (float)(v.z * sin(t/2.0));
	return ret;
}

Quaternion QuaternionHelper::Normalization(VECTOR v)
{
	Quaternion ret;
	ret.t = 0.0f;
	float vl;
	vl = sqrt(pow(v.x,2) + pow(v.y,2) + pow(v.z,2));
	ret.v.x = v.x / vl;
	ret.v.y = v.y / vl;
	ret.v.z = v.z / vl;
	return ret;
}
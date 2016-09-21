#pragma once

#include "DxLib.h"

struct Quaternion{
	float t;
	VECTOR v;
};

//	モデル回転のクォータニアオン補助関数
//	DXライブラリとかにも補助関数はあるらしい？
class QuaternionHelper{
private:
public:
	static Quaternion Multi(Quaternion q1, Quaternion q2);	//	クォータニオン同士の掛け算
	static float Inner(VECTOR q1, VECTOR q2);				//	内積
	static VECTOR Outer(VECTOR v1, VECTOR v2);				//	外積
	static VECTOR Plus(VECTOR v1, VECTOR v2);				//	足し算
	static VECTOR Minus(VECTOR v1, VECTOR v2);				//	引き算
	static VECTOR Multi(float f, VECTOR v);					//	掛け算
	static Quaternion Rotation(float t, VECTOR v);			//	回転
	static Quaternion Normalization(VECTOR v);				//	正規化（あってるか不安）
};
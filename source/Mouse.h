#pragma once

#include "DxLib.h"

// マウス用クラス
// 常に位置、ドラッグ時間、離したタイミングを監視している
class Mouse{
private:
	static int left;			// 左ボタン時間
	static int right;			// 右ボタン時間
	static int wheel;			// ホイール回転量
	static bool lefttouch;		// 左ボタンを離した瞬間
	static bool righttouch;		// 右ボタンを離した瞬間
public:
	Mouse();
	void Update();				// 毎フレーム呼び出される
	static void Reset();		// 記録をリセットする
	void Draw();				// 描画
	static bool Touch(int n);	// 指定したボタンが押された瞬間かを返す
	static bool Out(int n);		// 指定したボタンが離された瞬間かを返す
	static int Hold(int n);		// 指定したボタンの押している時間を返す
	static int Wheel();			// ホイール回転量を返す
	static VECTOR GetPos2D();	// スクリーン位置をVECTORで返す
	static VECTOR GetPos3D();	// 3D座標をVECTORで返す
};
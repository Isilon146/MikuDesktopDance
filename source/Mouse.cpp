#include "Mouse.h"
#include "Camera.h"
#include <math.h>

int Mouse::left;		// 左ボタン時間
int Mouse::right;		// 右ボタン時間
int Mouse::wheel;		// ホイール回転量
bool Mouse::lefttouch;	// 左ボタンを離した瞬間
bool Mouse::righttouch;	// 右ボタンを離した瞬間

Mouse::Mouse()
{
	left = 0;
	right = 0;
	wheel = 0;
	lefttouch = false;
	righttouch = false;
}

void Mouse::Update()
{
	//前にクリックされていればtouchをtrueにする
	//その後今回クリックされていない場合はそのままtrueとなり
	//離した瞬間を1フレームのみ保存する
	if (left != 0)
		lefttouch = true;
	else
		lefttouch = false;

	if (right != 0)
		righttouch = true;
	else
		righttouch = false;
	
	//マウスのクリック状態を取得
	if ( GetActiveFlag() && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		//押されていた場合時間増加
		left++;
		//離した瞬間ではないのでtouchはfalse
		lefttouch = false;
	}
	else{
		//離しているので時間をs0にする
		left = 0;
	}
	
	if ( GetActiveFlag() && (GetMouseInput() & MOUSE_INPUT_RIGHT) != 0)
	{
		right++;
		righttouch = false;
	}
	else{
		right = 0;
	}

	//ホイール回転量取得
	wheel = GetMouseWheelRotVol();
}

void Mouse::Reset()
{
	left = 0;
	right = 0;
	wheel = 0;
	lefttouch = false;
	righttouch = false;
}

void Mouse::Draw()
{
}

bool Mouse::Touch(int n)
{
	//引数が0なら左、それ以外なら右クリックが押された瞬間かを返す
	bool ret = false;
	if (n == 0) ret = left == 1;
	else ret = right == 1;
	return ret;
}

bool Mouse::Out(int n)
{
	//引数が0なら左、それ以外なら右クリックが離した瞬間かを返す
	bool ret = false;
	if (n == 0) ret = lefttouch;
	else ret = righttouch;
	return ret;
}

int Mouse::Hold(int n)
{
	//引数が0なら左、それ以外なら右クリックが押された時間を返す
	int ret = 0;
	if (n == 0) ret = left;
	else ret = right;
	return ret;
}

int Mouse::Wheel()
{
	return wheel;
}

VECTOR Mouse::GetPos2D()
{
	//2D上のマウス座標をVECTOR構造体で返す
	int mx, my;
	GetMousePoint(&mx, &my);
	VECTOR mpos;
	mpos.x = (float)mx;
	mpos.y = (float)my;
	mpos.z = 0.0f;
	return mpos;
}

VECTOR Mouse::GetPos3D()
{
	//3D上のマウス座標をVECTOR構造体で返す
	int mx, my;
	GetMousePoint(&mx, &my);
	return ConvScreenPosToWorldPos( VGet( (float)mx, (float)my, 0.0f ) ) ;
}
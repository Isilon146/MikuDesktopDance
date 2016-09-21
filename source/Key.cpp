#include "DxLib.h"
#include "Key.h"
#include <math.h>

int Key::key[256];
bool Key::lasttouch[256];

Key::Key()
{
	// 初期化
	for ( int i = 0; i < 256; i++ )
	{
		key[i] = 0;
		lasttouch[i] = false;
	}
}

void Key::Update()
{
	// 前回押されていたキーは離した瞬間にする
	for ( int i = 0; i < 256; i++ )
	{
		if ( key[i] != 0 )
		{
			lasttouch[i] = true;
		}
	}

	// キーの状況をtmpkeyに
	char tmpkey[256];
	GetHitKeyStateAll(tmpkey);

	for ( int i = 0; i < 256; i++ )
	{
		// ウィンドウがアクティブでキーが押されている場合
		if ( GetActiveFlag() && tmpkey[i] != 0 )
		{
			// 押されているキーのカウントを増やし、lasttouchをfalseにする（離していない）
			key[i]++;
			lasttouch[i] = false;
		}
		else
		{
			// 押されていない場合、カウントを0に
			// 前回押されていたキーは離した瞬間のまま
			key[i] = 0;
		}
	}
}

void Key::Reset()
{
	// 初期化
	for ( int i = 0; i < 256; i++ )
	{
		key[i] = 0;
		lasttouch[i] = false;
	}
}

void Key::Draw()
{
}

bool Key::Touch( int n )
{
	// カウントが1なら押した瞬間
	return key[n] == 1;
}

bool Key::Out( int n )
{
	// 離した瞬間の変数を利用
	return lasttouch[n];
}

int Key::Hold( int n )
{
	// カウントをそのまま返す
	return key[n];
}
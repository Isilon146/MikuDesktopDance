#pragma once

// キーの入力関連をまとめるクラス
// DXライブラリのキー取得機能だけでは押している時間が取れない
// キーの使用制限をする時にまとめて処理できる

class Key{
private:
	static int key[256];				// キーの押している時間記録
	static bool lasttouch[256];			// キーを離した瞬間か
public:
	Key();
	void Update();							// 毎フレームの更新
	static void Reset();							// カウントのリセット
	void Draw();							// 描画
	static bool Touch(int n);				// 押した瞬間かどうか
	static bool Out(int n);					// 離した瞬間かどうか
	static int Hold(int n);					// どれぐらい押しているか
};
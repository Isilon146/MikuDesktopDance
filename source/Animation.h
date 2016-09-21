#pragma once

#include "DxLib.h"

// アニメーションリスト
struct AnimBox{
	char name[64];
	float time;
};

//	モデルのアニメーションクラス
class Animation{
private:
	AnimBox list[64];
	int amount;			// アニメーションの数
	int index;			// 現在再生しているアニメーション番号
	int index_old;		// 前回再生していたアニメーション番号
	int Handle[64];		// アタッチハンドル、ブレンド率の操作に使用
	float Time;			// アニメーションの再生時間
	LONGLONG PrevTime;	// 前回の現実の時間
	LONGLONG NowTime;	// 今回の現実の時間
	float blending;		// ブレンド率
public:
	void Initialize(int modelhandle);	// ソフト起動時に呼ばれる
	void Finalize(int modelhandle);		// ソフト終了時に呼ばれる
	void Update(int modelhandle);		// 毎フレームの更新
	void Draw();						// 描画処理
	void Play(int num, int modelhandle);// 指定した番号のアニメーションを再生する
	void PlayNext();					// 次の番号のアニメーションを再生する
	void PlayPrev();					// 前の番号のアニメーションを再生する
	void TimeReset();					// 再生時間を初期化する
	int GetAmount();					// アニメーションの総数を返す
};
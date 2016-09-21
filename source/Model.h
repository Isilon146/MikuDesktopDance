#pragma once

#include "DxLib.h"
#include "Animation.h"

//	モデル管理クラス
class Model{
private:
    char FileName[256];		// モデルファイルのパス
    int ModelHandle;		// モデルのハンドル
	int FrameNum;			// モデルフレーム数
	int MeshNum;			// モデルメッシュ数
	bool drawframe;			// フレームの描画オンオフ
	bool drawwireframe;		// ワイヤーフレームの描画オンオフ
	// 当たり判定構造体
	struct{
		char name[64];		// 当たり判定名称
		int number;			// 動作番号
		VECTOR point;		// 位置（3D座標）
		int size;			// 大きさ（ディスプレイサイズ）
	}hit[256];
	VECTOR ScreenHitPoint[256];		// スクリーン上当たり判定位置（Z座標は使わない）
	VECTOR ScreenFramePoint[256];	// スクリーン上フレーム位置
	Animation animation;			// アニメーション
	void LoadModel();				// モデルの読み込み
	void UpdateFP();				// ScreenHitPoint,ScreenFramePointの更新
	void DrawPolyModel();
	void DrawFrameModel();
	void DrawNamingFrameModel();
	void DrawFrameCrossModel();
public:
	void Initialize();		// ソフト開始時に呼ばれる
	void Finalize();		// ソフト終了時に呼ばれる
	void Update();			// 毎フレームの更新
	void Draw();			// 描画処理
	int GetHandle();		// モデルハンドルの取得
	int LoadNewModel(const char* fileFilter);		// 新しいモデルを読みこむ
	int GetAnimAmount();	// アニメーションの数を取得
	void Play(int num);					// 指定した番号のアニメーションを再生する
};
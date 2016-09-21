#pragma once

#include "DxLib.h"

//	カメラ管理クラス
//	（モデル表示位置の管理も行う）
class Camera{
private:
	static VECTOR Pos;				// カメラ座標
	static VECTOR Target;			// カメラ注視点
	static VECTOR Move;				// カメラの移動距離
	static float vnear,vfar;		// 手前・奥クリップ距離
	static float ortho;				// 表示範囲
	float distance;					// カメラと対象との距離
	bool moving;					// カメラ移動
	int moving_x, moving_y;			// 前回のマウス位置（移動用）
	bool rotate;					// モデル回転（カメラの位置変更）
	bool rotate_xp, rotate_xm;		// 回転のx軸プラス方向・マイナス方向
	bool rotate_yp, rotate_ym;		// 回転のy軸プラス方向・マイナス方向
	int rotate_count;				// 回転のカウント
	int rotate_count_max;			// 回転の終了カウント
	int rotate_amount;				// 回転量
	bool viewdatas;					// カメラ情報表示の可否
	void Rotate_Start(bool &dir);		// 回転開始 引数にrotate_xp等で方向指定
	void Rotate_Update();				// 回転の更新
	void SetCameraPosition();			// 各変数によるカメラ位置更新
public:
	void Initialize();					// ソフト起動時に呼ばれる
	void Init_Pos();					// ループ開始直前に呼ばれる（なぜかInitialize時とループ時でウィンドウサイズ取得値が変わるため）
	void Finalize();					// ソフト終了時に呼ばれる
	void Update();						// 毎フレームの更新
	void Draw();						// 描画処理
	static VECTOR CameraPos();			// カメラの座標を得る
	void ResetPos();					// カメラの座標を初期化
	void ResetMove();					// カメラの移動だけ初期化
	bool ChengeViewDatas();				// カメラ情報表示の切り替え
	void ChengeFocus(int num);					// カメラフォーカス設定（操作パネル用）
};
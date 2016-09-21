#pragma once

//	BGM管理クラス
class BGM{
private:
    static char FileName[256];	// BGMのファイルまでのパス
	static int Handle;			// BGMのハンドル
	long CalcDB( int sound_val );		// 音量調整用の関数
public:
	BGM();						// コンストラクタ
	int Load();					// 音楽ファイルの読み込み
	void Start(int vol);		// 音楽の再生
	void Stop();				// 音楽の停止
	void Volume(int vol);		// 音量の変更
};
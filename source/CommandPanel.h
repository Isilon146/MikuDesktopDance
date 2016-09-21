#pragma once

#include <windows.h>
#include "BGM.h"

enum COMMAND_MESSAGE{
	CME_NONE,
	CME_ANIMATION,
	CME_FOCUS
};

//	操作パネルクラス
class CommandPanel{
private:
	MSG msg;				//	スレッドからのメッセージ情報
	char szTitle[64];		//	ウィンドウタイトル
	char szWindowClass[64];	//	ウィンドウクラス
	HINSTANCE GethInstance;	//	インスタンス
	int GetnCmdShow;		//	ウィンドウ表示状態

	static BGM bgm;			//	BGM管理

	static int light;		//	モデルに当たる光の量
	static int volume;		//	音量
	static int anim;		//	アニメーション番号
	static int focus;		//	フォーカス量
	static char bgmname[256];	//	BGMのパス

	static int animation_amount[8];	//	アニメーションの量保存（配列なのは複数モデル対応のため）

	static COMMAND_MESSAGE message;	//	メッセージ
	static int param;		//	パラメータ保存

	ATOM MyRegisterClass( HINSTANCE hInstance );	// クラス登録
	BOOL InitInstance( HINSTANCE, int );			// インスタンス作成
	static LRESULT CALLBACK CommandPanel::WndProc(HWND hWnd, UINT uint_message, WPARAM wParam, LPARAM lParam);	//	ウィンドウプロシージャ
public:
	void Initialize( HINSTANCE hInstance, int nCmdShow );	//	クラス初期化
	bool Update();									//	毎フレーム呼ばれる
	void Open();									//	操作パネルを開く時に呼ばれる
	void AnimResist(int person_number, int amount);	//	アニメーションを登録する（第2：モデル番号、第2：モーション数
	COMMAND_MESSAGE Message();						//	メッセージを送る（メイン関数でやって欲しい事：主に他クラスの操作）
	int GetParam();									//	パラメータを送る（メッセージに加え数値を必要とする場合に使用）
};
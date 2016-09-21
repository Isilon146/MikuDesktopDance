#include "DxLib.h"
#include "Mouse.h"
#include "Camera.h"
#include "Model.h"
#include "End.h"
#include "Key.h"
#include "BGM.h"
#include "CommandPanel.h"
#include "colorname.h"

//メニュー用
HMENU hMenu;
WNDPROC dxWndProc;

Model model;
Camera camera;
BGM bgm;
CommandPanel commandpanel;

bool framedraw;	// ウィンドウ外枠描画
bool controck;	// 操作ロック
int light;		// ライト値

#define IDR_RMENU		1100 // メニュー
#define IDM_NEWMODEL	1102 // モデル新規読み込み
#define IDM_TOP			1120 // 常に手前/通常切り替え
#define IDM_FRAMEDRAW	1121 // ウィンドウフレーム描画
#define IDM_ROCK		1122 // 動作ロック
#define IDM_MDROTRESET	1200 // モデル回転リセット
#define IDM_CAMERARESET	1201 // カメラ位置リセット
#define IDM_CAMERAINFO	1202 // カメラ情報表示
#define IDM_CTPANEL		1300 // 操作パネル表示
#define IDM_CLOSE		1990 // 閉じる
#define IDM_QUIT		1991 // 終了

void CMenu();					// ポップアップメニュー作成
LRESULT CALLBACK MyProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);	// ポップアップメニューの動作
void Finalize();				// ソフト終了処理（状態のセーブ）

// メイン関数
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
						LPSTR lpCmdLine, int nCmdShow )
{
	int SoftImageHandle;
	int MSAAScreenHandle;
	int ScreenHandle;
	
	SetOutApplicationLogValidFlag(FALSE);

	int ScreenW, ScreenH, ScreenCol;

	framedraw = false;
	controck = false;

	//二重起動可能
	SetDoubleStartValidFlag( true );
	//メインウィンドウを描画しない（起動途中は描画しない）
	SetWindowVisibleFlag( false );
	//非アクティブでも動作
	SetAlwaysRunFlag( true );
	//ウィンドウモードで起動
	ChangeWindowMode( true );
	//ウィンドウサイズ設定
	GetDefaultState( &ScreenW, &ScreenH, &ScreenCol ) ;
	//ここを設定ファイルで変更可能にする
	{
		int datafilep = FileRead_open( "default.ini" );
		int inputi[256];
		char inputc[256];
		int i = 0;
		while( ( FileRead_getc(datafilep)) != '\n' );
		while( ( FileRead_getc(datafilep)) != '\n' );
		while( ( inputi[0] = FileRead_getc(datafilep) ) != '=' );
		while( ( inputi[i] = inputc[i] = FileRead_getc(datafilep) ) != '\n' )
			i++;
		inputc[i - 1] = '\0';
		ScreenW = atoi(inputc);
		i = 0;
		while( ( inputi[0] = FileRead_getc(datafilep) ) != '=' );
		while( ( inputi[i] = inputc[i] = FileRead_getc(datafilep) ) != '\n' )
			i++;
		inputc[i - 1] = '\0';
		ScreenH = atoi(inputc);

		light = 0;
		for( i = 0; i < 5; i++ )
			while( ( FileRead_getc(datafilep) ) != '\n' );
		//光量取得
		i = 0;
		while( ( inputi[0] = FileRead_getc(datafilep) ) != '=');
		while( ( inputi[i] = inputc[i] = FileRead_getc(datafilep) ) != '\n' )
			i++;
		inputc[i - 1] = '\0';
		light = atoi(inputc);

		FileRead_close( datafilep );
	}
	SetWindowSize( ScreenW, ScreenH );
	SetGraphMode( ScreenW, ScreenH, 32 ); 

	// ウインドウの透過色モードＯＮ
	SetUseBackBufferTransColorFlag( true );
	SetUseUpdateLayerdWindowFlag( true );

	SetWindowStyleMode(2);

	// ＤＸライブラリの初期化
	if( DxLib_Init() < 0 ) return -1;
	
	// アンチエイリアス付きスクリーン作成
	SetCreateDrawValidGraphMultiSample( 8, 4 );
	MSAAScreenHandle = MakeScreen( ScreenW, ScreenH, true );;
	SetCreateDrawValidGraphMultiSample( 0, 0 );
	// アンチエイリアス無しスクリーン作成
	ScreenHandle = MakeScreen( ScreenW, ScreenH, true );
	// 画面取り込み用のソフトウエア用画像を作成
	SoftImageHandle = MakeARGB8ColorSoftImage( ScreenW, ScreenH ); 

	Mouse mouse;
	Key key;

	commandpanel.Initialize( hInstance, nCmdShow );
	model.Initialize();
	camera.Initialize();

	commandpanel.AnimResist( 0, model.GetAnimAmount() );
	
	CMenu();
	HWND hMainWnd;
    hMainWnd = GetMainWindowHandle();
    //メインウィンドウのウィンドウプロシージャのアドレスを格納
    dxWndProc = (WNDPROC)GetWindowLong( hMainWnd, GWL_WNDPROC );
    //自作ウィンドウプロシージャのアドレスを設定
    SetWindowLong( hMainWnd, GWL_WNDPROC, (LONG)MyProc );
	
	SetLightAmbColor( GetColorF( light * 0.01f, light * 0.01f, light * 0.01f, 0.0f ) ) ;
	
	//ここからメインウィンドウを描画する（起動途中は描画しない）
	SetWindowVisibleFlag( true );
	
	// 透過色を設定（紫）
	SetTransColor( 255, 0, 255 ) ;

	camera.ResetMove();

	camera.Init_Pos();

	while( ProcessMessage() == 0 && !End::Get() )
	{
		// 描画先をアンチエイリアス付きスクリーンにする
		SetDrawScreen( MSAAScreenHandle );

		// 画面をクリア
		ClearDrawScreen();

		//各種機能更新
		if ( !controck )
		{
			mouse.Update();
			key.Update();
		}

		if ( ( key.Hold(KEY_INPUT_LSHIFT) || key.Hold(KEY_INPUT_RSHIFT) ) && ( key.Hold(KEY_INPUT_LCONTROL) || key.Hold(KEY_INPUT_RCONTROL) ) && key.Hold(KEY_INPUT_F12) )
		{
			camera.ResetMove();
		}

		commandpanel.Update();
		switch ( commandpanel.Message() )
		{
		case CME_NONE:
			break;
		case CME_ANIMATION:
			model.Play( commandpanel.GetParam() );
			break;
		case CME_FOCUS:
			camera.ChengeFocus( commandpanel.GetParam() );
			break;
		default:
			break;
		}

		model.Update();
		camera.Update();

		//各種機能描画
		if ( framedraw )
		{
			DrawBox( 0, 0, ScreenW, ScreenH, dblue, false );
			DrawBox( 1, 1, ScreenW - 1, ScreenH - 1, dblue, false );
			DrawBox( 2, 2, 62, 17, wblue, true );
			DrawBox( ScreenW - 62, ScreenH - 17, ScreenW - 2, ScreenH - 2, wblue, true );
		}
		model.Draw();
		camera.Draw();

		// 描画先をアンチエイリアス無しスクリーンにする
		SetDrawScreen( ScreenHandle );

		// アルファ値も含めてそのまま描画するブレンドモードにセットして描画
		SetDrawBlendMode( DX_BLENDMODE_SRCCOLOR, 255 );
		DrawGraph( 0, 0, MSAAScreenHandle, true );
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );
		 
		// 描画先の画像をソフトイメージに取得する
		GetDrawScreenSoftImage( 0, 0, ScreenW, ScreenH, SoftImageHandle );

		// 取り込んだソフトイメージを使用して透過ウインドウの状態を更新する
		UpdateLayerdWindowForPremultipliedAlphaSoftImage( SoftImageHandle );
	}
	
	model.Finalize();
	camera.Finalize();
	Finalize();

	DxLib_End() ;				// ＤＸライブラリ使用の終了処理

	return 0 ;				// ソフトの終了 
}

// ポップアップメニュー作成
void CMenu()
{
	int id = 0;
	hMenu = CreatePopupMenu();
	HMENU hAnimMenu = CreatePopupMenu();
	HMENU hLightMenu = CreatePopupMenu();
	HMENU hBGMVolumeMenu = CreatePopupMenu();
	MENUITEMINFO mii;

	ZeroMemory( &mii, sizeof(MENUITEMINFO) );	// 初期化
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fType = MFT_SEPARATOR;
	 
	// モデル新規読み込み
	mii.fMask = MIIM_ID | MIIM_STRING;
	mii.wID = IDM_NEWMODEL;
	mii.dwTypeData = TEXT( "モデル新規読み込み" );
    
	// メニュー項目追加
	InsertMenuItem( hMenu, id++, TRUE, &mii );

	// セパレーター
	mii.fMask = MIIM_FTYPE;

	InsertMenuItem( hMenu, id++, TRUE, &mii );

	// 常に手前/通常切り替え
	mii.fMask = MIIM_ID | MIIM_STRING;
	mii.wID = IDM_TOP;
	mii.dwTypeData = TEXT( "常に手前/通常切り替え" );
     
	InsertMenuItem( hMenu, id++, TRUE, &mii );
	
	// ウィンドウフレーム描画
	mii.fMask = MIIM_ID | MIIM_STRING;
	mii.wID = IDM_FRAMEDRAW;
	mii.dwTypeData = TEXT( "ウィンドウフレーム描画" );
     
	InsertMenuItem( hMenu, id++, TRUE, &mii );
	
	// 動作ロック
	mii.fMask = MIIM_ID | MIIM_STRING;
	mii.wID = IDM_ROCK;
	mii.dwTypeData = TEXT( "動作ロック" );
     
	InsertMenuItem( hMenu, id++, TRUE, &mii );

	// セパレーター
	mii.fMask = MIIM_FTYPE;
	
	InsertMenuItem( hMenu, id++, TRUE, &mii );
	 
	// モデル回転リセット
	mii.fMask = MIIM_ID | MIIM_STRING;
	mii.wID = IDM_MDROTRESET;
	mii.dwTypeData = TEXT( "モデル回転リセット" );
     
	InsertMenuItem( hMenu, id++, TRUE, &mii );
	 
	// カメラ位置リセット
	mii.fMask = MIIM_ID | MIIM_STRING;
	mii.wID = IDM_CAMERARESET;
	mii.dwTypeData = TEXT( "カメラ位置リセット" );
     
	InsertMenuItem( hMenu, id++, TRUE, &mii );
	 
	// カメラ情報表示
	mii.fMask = MIIM_ID | MIIM_STRING;
	mii.wID = IDM_CAMERAINFO;
	mii.dwTypeData = TEXT( "カメラ情報表示" );
     
	InsertMenuItem( hMenu, id++, TRUE, &mii );

	// 操作パネル表示
	mii.fMask = MIIM_ID | MIIM_STRING;
	mii.wID = IDM_CTPANEL;
	mii.dwTypeData = TEXT( "操作パネル表示" );
     
	InsertMenuItem( hMenu, id++, TRUE, &mii );
	 
	// 閉じる
	mii.fMask = MIIM_ID | MIIM_STRING;
	mii.wID = IDM_CLOSE;
	mii.dwTypeData = TEXT( "閉じる" );
     
	InsertMenuItem( hMenu, id++, TRUE, &mii );

	// セパレーター
	mii.fMask = MIIM_FTYPE;
	
	InsertMenuItem( hMenu, id++, TRUE, &mii );

	// 終了
	mii.fMask = MIIM_ID | MIIM_STRING;
	mii.wID = IDM_QUIT;
	mii.dwTypeData = TEXT( "終了" );
     
	InsertMenuItem( hMenu, id++, TRUE, &mii );
}

// ポップアップメニューの動作
LRESULT CALLBACK MyProc( HWND hWnd, UINT msg, WPARAM wp, LPARAM lp )
{
	POINT pt;
	static bool top = false;

    switch (msg) {
	case WM_CREATE:
       break;
	case WM_RBUTTONDOWN:
		// マウスカーソル座標取得
		pt.x = LOWORD( lp );
		pt.y = HIWORD( lp );
		// スクリーン座標に変換
		ClientToScreen( hWnd, &pt );
		// カーソル位置にメニューを出す
		TrackPopupMenu( hMenu, 0, pt.x, pt.y, 0, hWnd, NULL );
       break;
	case WM_COMMAND:
		{
			int lw = LOWORD( wp );
			switch ( lw ) {
			case IDM_NEWMODEL:
				{
					int ret = model.LoadNewModel( NULL );
					if ( ret )
					{
						CMenu();
					}
					controck = false;
				}
				break;
			case IDM_MDROTRESET:
				camera.ResetPos();
				break;
			case IDM_CAMERARESET:
				camera.ResetMove();
				break;
			case IDM_TOP:
				top = !top;
				if ( top )
				{
					SetWindowPos( GetMainWindowHandle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
					CheckMenuItem( hMenu, IDM_TOP, MFS_CHECKED );
				}
				else
				{
					SetWindowPos( GetMainWindowHandle(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
					CheckMenuItem( hMenu, IDM_TOP, MFS_UNCHECKED );
				}
				break;
			case IDM_FRAMEDRAW:
				framedraw = !framedraw;
				if ( framedraw )
					CheckMenuItem( hMenu, IDM_FRAMEDRAW, MFS_CHECKED );
				else
					CheckMenuItem( hMenu, IDM_FRAMEDRAW, MFS_UNCHECKED );
				break;
			case IDM_ROCK:
				controck = !controck;
				if ( controck )
				{
					Key::Reset();
					Mouse::Reset();
					CheckMenuItem( hMenu, IDM_ROCK, MFS_CHECKED );
				}
				else
				{
					CheckMenuItem( hMenu, IDM_ROCK, MFS_UNCHECKED );
				}
				break;
			case IDM_CAMERAINFO:
				if ( camera.ChengeViewDatas() )
					CheckMenuItem( hMenu, IDM_CAMERAINFO, MFS_CHECKED );
				else
					CheckMenuItem( hMenu, IDM_CAMERAINFO, MFS_UNCHECKED );
				break;
			case IDM_CTPANEL:
				commandpanel.Open();
				break;
			case IDM_CLOSE:
				break;
			case IDM_QUIT:
				End::SubEnd();
				break;
			default:
				break;
			}
		}
		break;
    case WM_LBUTTONDOWN:
        break;
	case WM_DESTROY : /* 終了処理 */
		DestroyMenu( hMenu );
		PostQuitMessage( 0 );
		break;
    }
    return CallWindowProc( dxWndProc, hWnd, msg, wp, lp );
}

void Finalize()
{
	TCHAR change_input[256];
	TCHAR path[MAX_PATH];
	AppLogAdd( "in camera finalize\n" );
	GetModuleFileName( NULL, path, MAX_PATH );
	TCHAR* ptmp = _tcsrchr( path, _T('\\') ); // \の最後の出現位置を取得
	if ( ptmp != NULL )
	{   //ファイル名を削除
		ptmp = _tcsinc( ptmp );   //一文字進める
		*ptmp = _T('\0');
	}
	else
	{
		//エラー：\が無い
		AppLogAdd( "error no save file\n" );
	}
	strcat_s( path, "default.ini" );

	wsprintf( change_input, "%d", light );
	WritePrivateProfileString( "default", "light", change_input, path );
}
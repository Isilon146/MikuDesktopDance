#include "Camera.h"
#include "Key.h"
#include "Mouse.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include "colorname.h"
#include "QuaternionHelper.h"

VECTOR Camera::Pos;
VECTOR Camera::Target;
VECTOR Camera::Move;
float Camera::vnear,Camera::vfar;
float Camera::ortho;

void Camera::Initialize()
{
	// ウィンドウサイズ取得
	int WindowX,WindowY;
	GetWindowSize( &WindowX , &WindowY ) ;

	// クリップ距離設定
	vnear = 0.1f;
	vfar = 600.0f;
    SetCameraNearFar( vnear, vfar );
	
	// 初期移動量設定（ウィンドウ中央）
	Move.x = ( WindowX - 10 ) / 2.0f;
	Move.y = ( WindowY - 5 ) / 1.0f;
	Move.z = 0;
	// カメラ位置設定
	Pos.x = 0;
	Pos.y = 0;
	Pos.z = -50;
	// 注視点設定
	Target.x = 0;
	Target.y = 0;
	Target.z = 0;
	// 設定した値を適用
	SetCameraPosition();

	// 正射影カメラをセットアップ
	ortho = 26.0f;
	SetupCamera_Ortho( ortho ) ;

	// 設定ファイルからカメラ情報を取得する
	int datafilep = FileRead_open( "default.ini" );
	int inputi[256];
	char inputc[256];
	int i = 0;
	for( i = 0; i < 4; i++ )
		while( ( FileRead_getc(datafilep) ) != '\n' );
	//拡大率（表示範囲）取得
	i = 0;
	while( ( inputi[0] = FileRead_getc(datafilep) ) != '=' );
	while( ( inputi[i] = inputc[i] = FileRead_getc(datafilep) ) != '\n' )
		i++;
	inputc[i - 1] = '\0';
	ortho = (float)atoi( inputc );
	//移動量取得
	i = 0;
	while( ( inputi[0] = FileRead_getc(datafilep) ) != '=' );
	while( ( inputi[i] = inputc[i] = FileRead_getc(datafilep) ) != '\n' )
		i++;
	inputc[i - 1] = '\0';
	Move.x = (float)atoi( inputc );
	i = 0;
	while( ( inputi[0] = FileRead_getc(datafilep) ) != '=' );
	while( ( inputi[i] = inputc[i] = FileRead_getc(datafilep) ) != '\n' )
		i++;
	inputc[i - 1] = '\0';
	Move.y = (float)atoi( inputc );
	// 拡大率（表示範囲）の適用
	SetupCamera_Ortho( ortho ) ;

	FileRead_close( datafilep );
	
	// 距離の設定
	distance = 10.0f;
	
	// モデル移動系の初期化
	moving = false;
	moving_x = 0;
	moving_y = 0;

	// 回転系の初期化
	rotate = false;
	rotate_xp = false;
	rotate_xm = false;
	rotate_yp = false;
	rotate_ym = false;
	rotate_count = 0;
	rotate_count_max = 1;
	rotate_amount = 1;

	// カメラ情報を表示しない（デフォルト）
	viewdatas = false;
}

void Camera::Init_Pos()
{
	// 設定ファイルからウィンドウ位置を取得する
	int datafilep = FileRead_open( "default.ini" );
	int inputi[256];
	char inputc[256];
	int i = 0;
	for( i = 0; i < 7; i++ )
		while( ( FileRead_getc(datafilep) ) != '\n' );
	// ウィンドウ位置
	int winpos_x, winpos_y;
	i = 0;
	while( ( inputi[0] = FileRead_getc(datafilep) ) != '=' );
	while( ( inputi[i] = inputc[i] = FileRead_getc(datafilep) ) != '\n' )
		i++;
	inputc[i - 1] = '\0';
	winpos_x = atoi( inputc );
	i = 0;
	while( ( inputi[0] = FileRead_getc(datafilep) ) != '=' );
	while( ( inputi[i] = inputc[i] = FileRead_getc(datafilep) ) != '\n' )
		i++;
	inputc[i - 1] = '\0';
	winpos_y = atoi( inputc );
	// ウィンドウ位置を適用する
	SetWindowPosition( winpos_x, winpos_y );
	FileRead_close( datafilep );
}

void Camera::Finalize()
{
	// カメラの情報を設定ファイルに記録する
	TCHAR change_input[256];	// 書き込む文字列を保存する変数
	// デフォルトパスを指定
	TCHAR path[MAX_PATH];
	// 実行ファイルのパスをpathに
	GetModuleFileName( NULL, path, MAX_PATH );
	// \の最後の出現位置を取得
	TCHAR* ptmp = _tcsrchr( path, _T( '\\' ) );
	if ( ptmp != NULL )
	{
		ptmp = _tcsinc( ptmp );   // 一文字進める
		*ptmp = _T( '\0' );	// NULLを挿入
	}
	else
	{
		// エラー処理
	}
	// 設定ファイルのパス
	strcat_s( path, "default.ini" );
	
	// 拡大率（表示範囲）
	wsprintf( change_input, "%d", (int)ortho );
	WritePrivateProfileString( "default", "modelfocus", change_input, path );
	// カメラ位置
	wsprintf( change_input, "%d", (int)Pos.x );
	WritePrivateProfileString( "default", "modelpadding_x", change_input, path );
	wsprintf( change_input, "%d", (int)Pos.y );
	WritePrivateProfileString( "default", "modelpadding_y", change_input, path );
	// ウィンドウ位置
	int winpos_x, winpos_y;
	GetWindowPosition( &winpos_x, &winpos_y );
	wsprintf( change_input, "%d", winpos_x );
	WritePrivateProfileString( "default", "winpos_x", change_input, path );
	wsprintf( change_input, "%d", winpos_y );
	WritePrivateProfileString( "default", "winpos_y", change_input, path );

}

void Camera::Update()
{
	// 回転の更新
	Rotate_Update();
	// 矢印キーで回転
	if ( Key::Hold(KEY_INPUT_LEFT) ) Rotate_Start( rotate_ym );
	if ( Key::Hold(KEY_INPUT_RIGHT) ) Rotate_Start( rotate_yp );
	if ( Key::Hold(KEY_INPUT_DOWN) ) Rotate_Start( rotate_xm );
	if ( Key::Hold(KEY_INPUT_UP) ) Rotate_Start( rotate_xp );

	//マウス左クリックがされていたらウィンドウ移動処理
	if ( moving )
	{
		// ウィンドウサイズ取得
		int WindowX,WindowY;
		GetWindowSize( &WindowX , &WindowY ) ;
		int ScreenX = GetSystemMetrics( SM_CXVIRTUALSCREEN );
		int ScreenY = GetSystemMetrics( SM_CYVIRTUALSCREEN );
		// 新しいマウス位置取得
		int newmoving_x, newmoving_y;
		GetMousePoint( &newmoving_x, &newmoving_y );
		// 前回との位置の差
		int moving_x_dif, moving_y_dif;
		moving_x_dif = newmoving_x - moving_x;
		moving_y_dif = newmoving_y - moving_y;
		// ウィンドウの位置取得
		int windowpos_x, windowpos_y;
		GetWindowPosition( &windowpos_x, &windowpos_y );
		// 新しいウィンドウ位置生成
		int newwindow_x, newwindow_y;
		newwindow_x = windowpos_x + moving_x_dif;
		newwindow_y = windowpos_y + moving_y_dif;
		// 左端判定
		if ( newwindow_x < 0 ) {
			// ウィンドウが端に到達した場合はカメラを移動する
			newwindow_x = 0;
			Move.x += moving_x_dif;
			moving_x = newmoving_x;
		}
		else if ( Move.x < WindowX / 2 )
		{
			// ウィンドウが端から離れる場合、先にカメラを移動する
			newwindow_x = 0;
			Move.x += moving_x_dif;
			if ( Move.x > WindowX / 2 ) 
				Move.x = WindowX / 2.0f;
			moving_x = newmoving_x;
		}
		// 右端判定
		if ( newwindow_x > ScreenX - WindowX ) {
			// ウィンドウが端に到達した場合はカメラを移動する
			newwindow_x = ScreenX - WindowX;
			Move.x += moving_x_dif;
			moving_x = newmoving_x;
		}
		else if ( Move.x > WindowX / 2 )
		{
			// ウィンドウが端から離れる場合、先にカメラを移動する
			newwindow_x = ScreenX - WindowX;
			Move.x += moving_x_dif;
			if ( Move.x < WindowX / 2 ) 
				Move.x = (float)(WindowX / 2);
			moving_x = newmoving_x;
		}
		// 上端判定
		if ( newwindow_y < -26 ) {
			// ウィンドウが端に到達した場合はカメラを移動する
			newwindow_y = -26;
			Move.y += moving_y_dif;
			moving_y = newmoving_y;
		}
		else if ( Move.y < WindowY )
		{
			// ウィンドウが端から離れる場合、先にカメラを移動する
			newwindow_y = -26;
			Move.y += moving_y_dif;
			if ( Move.y > WindowY ) 
				Move.y = (float)WindowY;
			moving_y = newmoving_y;
		}
		// 下端判定
		if ( newwindow_y > ScreenY - WindowY ) {
			// ウィンドウが端に到達した場合はカメラを移動する
			newwindow_y = ScreenY - WindowY;
			Move.y += moving_y_dif;
			moving_y = newmoving_y;
		}
		else if ( Move.y > WindowY )
		{
			// ウィンドウが端から離れる場合、先にカメラを移動する
			newwindow_y = ScreenY - WindowY;
			Move.y += moving_y_dif;
			if ( Move.y < WindowY ) 
				Move.y = (float)WindowY;
			moving_y = newmoving_y;
		}
		// ウィンドウの位置を変更する
		SetWindowPosition( newwindow_x, newwindow_y );
	}
	
	// マウスをクリックしたらウィンドウ移動状態に移行
	if ( Mouse::Touch( 0 ) ){
		moving = true;
		GetMousePoint( &moving_x, &moving_y );
	}
	// マウスを離したり、ウィンドウが非アクティブになったら移動状態を無くす
	if ( Mouse::Out( 0 ) || !GetActiveFlag() )
		moving = false;

	//カメラの位置の更新
	SetCameraPosition();
}

void Camera::Draw()
{
	// カメラ情報の描画
	// 表示しない場合は返す
	if ( !viewdatas ) return;
	// フォントサイズ指定
	SetFontSize( 15 );
	// 文字の太さ指定
	SetFontThickness( 3 );
	// フォントを枠（エッジ）付きに
	ChangeFontType( DX_FONTTYPE_EDGE ) ;
	//文字描画
	DrawFormatString( 15, 5, white, "camera(%.1f,%.1f,%.1f)", Pos.x, Pos.y, Pos.z );
	DrawFormatString( 15, 20, white, "target(%.1f,%.1f,%.1f)", Target.x, Target.y, Target.z );
	DrawFormatString( 15, 35, white, "move(%.1f,%.1f,%.1f)", Move.x, Move.y, Move.z );
	DrawFormatString( 15, 50, white, "%.1f", ortho );
	
	VECTOR m3dpos = Mouse::GetPos3D();
	DrawFormatString( 15, 65, white, "mouse(%.1f,%.1f,%.1f)", m3dpos.x, m3dpos.y, m3dpos.z );
}

void Camera::Rotate_Start(bool &dir)
{
	// 既に回転している場合は何もしない
	if ( rotate ) return;
	// 指定した方向の回転を開始する
	rotate = dir = true;
	// カウント初期化
	rotate_count = 0;
	// カウント最大値設定
	rotate_count_max = 10;
	// もしCTRLキーを押していたら、最大値を20にする（回転にかかる時間が倍になる）
	if ( Key::Hold(KEY_INPUT_LCONTROL) > 0 || Key::Hold(KEY_INPUT_RCONTROL) > 0 ) rotate_count_max = 20;
	// 回転量を設定（1で10度回転）
	rotate_amount = 1;
	// もしShiftキーを押していたら、回転量を3にする（3倍の30度回転）
	if ( Key::Hold(KEY_INPUT_LSHIFT) > 0 || Key::Hold(KEY_INPUT_RSHIFT) > 0 ) rotate_amount = 3;
}

void Camera::Rotate_Update()
{
	// もし回転していないなら何もしない
	if ( !rotate ) return;
	Quaternion Qua;
	Quaternion RotQua;
	Quaternion CQua;
	VECTOR Vx;
	// Y軸で回転していたらY軸プラス向きにベクトルを設定
	if ( rotate_ym || rotate_yp )
		Vx.y = 1.0f;
	else
		Vx.y = 0.0f;
	// X軸で回転していたらX軸プラス向きにベクトルを設定
	if ( rotate_xm || rotate_xp )
		Vx.x = 1.0f;
	else
		Vx.x = 0.0f;
	// Z軸回転は無し
	Vx.z = 0.0f;
	// 正規化したものをQuaに
	Qua = QuaternionHelper::Normalization( Vx );
	// 回転量を取得
	float axis = rotate_amount * 1.0f;
	// もしマイナス方向なら回転を逆にする
	if ( rotate_ym || rotate_xp )
		axis = rotate_amount * -1.0f;
	// 回転用のクォータニオンをRotQuaに
	RotQua = QuaternionHelper::Rotation( (float)( axis * M_PI / 180.0 ), Qua.v );
	// 逆方向のクォータニオンをCQuaに
	CQua = RotQua;
	CQua.v.x *= -1;
	CQua.v.y *= -1;
	CQua.v.z *= -1;
	// 現在位置を設定
	Quaternion NowPos;
	NowPos.t = 0;
	NowPos.v = Pos;
	// 次の位置をNextPosに
	Quaternion NextPos = QuaternionHelper::Multi( QuaternionHelper::Multi( CQua, NowPos ), RotQua );
	// ベクトル部分をPosに
	Pos = NextPos.v;
	// カメラの更新
	SetCameraPosition();
	//カウント増加
	rotate_count++;
	if ( rotate_count == rotate_count_max )
	{
		//指定値になったら終了処理
		rotate = false;
		rotate_xp = false;
		rotate_xm = false;
		rotate_yp = false;
		rotate_ym = false;
		rotate_count = 0;
	}
}

void Camera::SetCameraPosition()
{
	// カメラの設定を元に情報を更新する
	// 拡大率適用
	SetupCamera_Ortho( ortho ) ;
	// クリップ距離適用
    SetCameraNearFar( vnear, vfar );
	// カメラ位置と注視点の適用
	SetCameraPositionAndTarget_UpVecY( Pos, Target );
	// カメラの移動量適用
	SetCameraScreenCenter( Move.x, Move.y );
}

VECTOR Camera::CameraPos()
{
	// カメラの位置を取得
	return VGet( Pos.x, Pos.y, Pos.z );
}

void Camera::ResetPos()
{
	// カメラの設定を初期化する
	rotate = false;
	rotate_xp = false;
	rotate_xm = false;
	rotate_yp = false;
	rotate_ym = false;
	Pos.x = 0;
	Pos.y = 0;
	Pos.z = -50;
	Target.x = 0;
	Target.y = 0;
	Target.z = 0;
	// カメラの情報を更新
	SetCameraPosition();
}

void Camera::ResetMove()
{
	// ウィンドウサイズ取得
	int WindowX,WindowY;
	GetWindowSize( &WindowX , &WindowY ) ;
	// スクリーンサイズ取得
	int ScreenX,ScreenY,ScreenCol;
	GetDefaultState( &ScreenX , &ScreenY , &ScreenCol ) ;
	// スクリーン中央にウィンドウを設定
	SetWindowPosition( ScreenX / 2 - WindowX / 2, ScreenY / 2 - WindowY / 2 );
	// カメラの移動量をウィンドウサイズの中央に
	Move.x = WindowX / 2.0f;
	Move.y = WindowY / 1.0f;
	Move.z = 0;
	// カメラの情報を更新
	SetCameraPosition();
}

bool Camera::ChengeViewDatas()
{
	// データの表示切替
	viewdatas = !viewdatas;
	return viewdatas;
}

void Camera::ChengeFocus( int num )
{
	ortho = (float)num;
	if ( ortho > 128.0f ) ortho = 128.0f;
	if ( ortho < 2.0f ) ortho = 2.0f;
	SetupCamera_Ortho( ortho ) ;
}
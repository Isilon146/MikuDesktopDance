#include "BGM.h"
#include "DxLib.h"
#include <math.h>

char BGM::FileName[256];		// BGMのファイルまでのパス
int BGM::Handle;				// BGMのハンドル

BGM::BGM()
{
	//ハンドル無し
	Handle = 0;
	//ファイル名無し
	strcpy_s(FileName, "");
}

int BGM::Load()
{
	// 既に音楽ファイルをロードしていたら停止しメモリから削除
	if ( CheckSoundMem(Handle) == 1 )
	{
		Stop();
		DeleteSoundMem(Handle);
	}

	// 読み込む前にデフォルトパスを選択
    OPENFILENAME ofn;
	TCHAR path[MAX_PATH];
	// 実行ファイルのパスをpathに
	GetModuleFileName(NULL, path, MAX_PATH);
	// \の最後の出現位置を取得
	TCHAR* ptmp = _tcsrchr(path, _T('\\'));
	if (ptmp != NULL)
	{
		ptmp = _tcsinc(ptmp);   // 一文字進める
		*ptmp = _T('\0');	// NULLを挿入
	}
	else
	{
		// エラー処理
	}

	// ofnの初期化と設定
    ZeroMemory( &ofn,sizeof( ofn ) );
    ofn.lStructSize=sizeof( ofn );
    ofn.hwndOwner=GetMainWindowHandle();
    ofn.lpstrFilter=NULL;
    ofn.nFilterIndex=0;
	ofn.lpstrInitialDir = path;		// 最初に開くフォルダ
    ofn.lpstrFile=FileName;
    ofn.nMaxFile=sizeof( FileName );
    ofn.Flags=OFN_CREATEPROMPT | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
    //if(FileName == NULL) return 0;
	// wav、mp3、oggに対応
	ofn.lpstrFilter="音楽ファイル(*.wav;*.mp3;*.ogg)\0*.*wav;*.*mp3;*.*ogg\0";
    FileName[0]=0;
	int ret = GetOpenFileName( &ofn );
	// 取得したパスから音楽をメモリに読み込む
	Handle = LoadSoundMem(FileName);
    return ret;
}

void BGM::Start(int vol)
{
	// 既に再生していた場合は停止する
	if ( CheckSoundMem(Handle) == 1 ) Stop();
	// 音量を適用する
	Volume( vol );
	// ループで再生開始
	PlaySoundMem( Handle, DX_PLAYTYPE_LOOP );
}

void BGM::Stop()
{
	// 音楽を停止
	StopSoundMem( Handle );
}

long BGM::CalcDB( int sound_val )
{
	// パーセンテージを調整用の数値に変換
	int par;
	par = 100 * sound_val / 255;
	return ( par == 0 ) ? 0 : ( (int)( log10f( (float)par ) * 5000.0f ) );
}

void BGM::Volume(int vol)
{
	// 音量を適用する
	SetVolumeSoundMem( CalcDB( vol ), Handle );
}
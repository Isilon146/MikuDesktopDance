#include "Animation.h"

void Animation::Initialize( int modelhandle )
{
	// ソフト起動時に呼ばれる
	// 読みこまれたモデルのアニメーションの数を得る
	// モデル名+数字のvmdファイルを000,001,002...と見つからなくなるまで読み込んでいる。
	amount = MV1GetAnimNum(modelhandle);

	// 文字列初期化
	for( int i = 0; i < 64; i++ )
		strcpy_s( list[i].name, "" );

	// 各アニメーションごとにループ
	for( int i = 0; i < amount; i++ )
	{
		// アタッチする
		Handle[i] = MV1AttachAnim( modelhandle, i, -1, FALSE );
		// アニメーション名を登録する
		char istr[4];
		sprintf_s( istr, "%d", i + 1 );
		strcpy_s( list[i].name, "アニメーション" );
		strcat_s( list[i].name, istr );
		// アニメーションの時間を登録する
		list[i].time = MV1GetAttachAnimTotalTime( modelhandle, Handle[i] );
		// アタッチのブレンドを0にする
		// ブレンド率は0.0f～1.0fまで、0.0fで完全無効、1.0fで完全適用。
		// アタッチしてある複数のアニメーションに対しブレンドを行える。
		MV1SetAttachAnimBlendRate( modelhandle, Handle[i], 0.0f );
	}
	// 一番最初のアニメーションのブレンドを1にする
	MV1SetAttachAnimBlendRate( modelhandle, Handle[0], 1.0f );

	// 一番最初のアニメーションを再生している事を記録
    index = 0;
    index_old = 0;

    // 再生時間の初期化
	Time = 0.0f;

	// 現在の時間を記録
	PrevTime = GetNowHiPerformanceCount();
	NowTime = PrevTime;

	// 現在のフェードを0にする
	blending = 0.0f;
}

void Animation::Finalize( int modelhandle )
{
	// ソフト終了時に読み込まれる
	// モデルをメモリから削除すればいいので、デタッチしなくても問題はない
	for(int i = 0; i < amount; i++)
		MV1DetachAnim( modelhandle, Handle[i] );
}

void Animation::Update(int modelhandle)
{
	// 毎フレームの更新
	// 前回の時間を記録
	PrevTime = NowTime;
	// 今回の時間を記録
	NowTime = GetNowHiPerformanceCount();
	
	if ( blending == 0.0f )
	{
		// 非フェード時はアニメ―ションの時間を進める
		Time += ( NowTime - PrevTime ) / 1000000.0f * 30.0f;
	}
	else
	{
		// フェード処理
		blending -= 0.02f;
		if ( blending < 0.0f) blending = 0.0f;//誤差用
		// 前回のアニメーションのブレンド率を下げていく
		MV1SetAttachAnimBlendRate( modelhandle, Handle[index_old], blending );
		// 今回のアニメーションのブレンド率を上げていく
		MV1SetAttachAnimBlendRate( modelhandle, Handle[index], 1.0f - blending );
	}

	// アニメーションが最後まで行ったら時間を0にする（ループ処理）
	// ループが必要無いアニメーションの場合はこの処理をさせない必要がある
	if( Time >= list[index].time ){
		Time = 0.0f ;
	}
	
	// アニメーションの時間を現在のアニメーションに適用する
	MV1SetAttachAnimTime( modelhandle, Handle[index], Time );

	/**
		フェード中は前のフェードし始めたタイミングの前のモデルと、
		次のモデルの最初のポーズでフェードする。
		手は側面で体はまっすぐ立っている事を想定している。
	**/
}

void Animation::Draw()
{
	// 描画処理
}

void Animation::Play( int num, int modelhandle )
{
	if ( blending != 0.0f )
	{
		// 前回のアニメーションのブレンド率を0にする
		MV1SetAttachAnimBlendRate( modelhandle, Handle[index_old], 0.0f );
		// 今回のアニメーションのブレンド率を1にする
		MV1SetAttachAnimBlendRate( modelhandle, Handle[index], 1.0f );
	}
	// 指定した番号のアニメーションを再生する
	// フェード開始
	blending = 1.0f;
	// 再生時間を0に
	Time = 0.0f ;
	//前回の番号をindex_oldに、指定した番号をindexに
    index_old = index;
    index = num;
	if ( index == index_old ) blending = 0.0f;
}

void Animation::PlayNext()
{
	// 次の番号のアニメーションを再生する
	if ( index == amount - 1 ) return;
	blending = 1.0f;
	Time = 0.0f ;
    index_old = index;
    index++;
	if ( index == index_old ) blending = 0.0f;
}

void Animation::PlayPrev()
{
	// 前の番号のアニメーションを再生する
	if ( index == 0 ) return;
	blending = 1.0f;
	Time = 0.0f ;
    index_old = index;
    index--;
	if ( index == index_old ) blending = 0.0f;
}

void Animation::TimeReset()
{
	// 再生時間を初期化する
	Time = 0.0f ;
}

int Animation::GetAmount()
{
	return amount;
}
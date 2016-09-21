#include "Model.h"
#include "Mouse.h"
#include "Key.h"
#include "Math.h"

void Model::Initialize()
{
	// ファイルを開く
	int datafilep = FileRead_open( "default.ini" );
	// 読み込みに使う変数（1文字ずつ読み込み指定位置までを変数に）
	int inputi[64];
	char inputc[64];
	// 最初の=まで飛ばす
	while( ( inputi[0] = FileRead_getc(datafilep) ) != '=' );
	int i = 0;
	// =の次から改行までを代入
	while( ( inputi[i] = inputc[i] = FileRead_getc(datafilep) ) != '\n' )
		i++;
	// NULL追加
	inputc[i - 1] = '\0';
	// ファイルを閉じる
	FileRead_close(datafilep);
	
	// 読み込んだ文字列をFilenameにコピー
	strcpy_s( FileName, inputc );
	// モデルの読み込み
	LoadModel();

	// デフォルトの値設定
	drawframe = false;
	drawwireframe = false;

	// 当たり判定初期化（-1で未使用）
	for( int i = 0; i < 256; i++ )
		hit[i].number = -1;

	// 移行当たり判定の読み込み
	/*
	// 当たり判定のファイルを開く
	datafilep = FileRead_open("media/model/data.txt");
	// 読み込みの終了判定
	bool end = false;
	// 行数、項目数
	int n = 0, num = 0;
	// 配列の記録位置（「(」が当たり判定開始のため、その文字を読みこんだ場合に上昇）
	int hitindex = -1;

	// 読み込み
	while (!end){
		// 代入先変更しない（デフォルト）
		next = false;
		// 指定位置まで文字読み込み
		for (int i = 0; i < 64; i++){
			// 読み込んだ一文字をinputcとiinputiに代入
			inputc[i] = inputi[i] = FileRead_getc(datafilep);
			// もし「=」だった場合
			if (inputi[i] == '=')
			{
				// 一旦読み込みを中止して文字の再読み込み（num=0 → num=1）
				inputc[i] = '\0';
				break;
			}
			// もし「,」だった場合
			if (inputi[i] == ',')
			{
				// 読み込みを中止して値の記録（num=2 ～ num=7）
				inputc[i] = '\0';
				break;
			}
			// もし「(」だった場合
			if (inputi[i] == '(')
			{
				// 読み込みを中止して当たり判定の数を増やし文字の再読み込み（num=1 → num=2）
				inputc[i] = '\0';
				hitindex++;
				break;
			}
			// もし「)」だった場合
			if (inputi[i] == ')')
			{
				// 読み込みを中止して値の記録（num=7 → num=8）
				inputc[i] = '\0';
				break;
			}
			// もし改行だった場合
			if (inputi[i] == '\n')
			{
				// 一旦読み込みを中止して文字の再読み込み（num=8）
				inputc[i] = '\0';
				break;
			}
			// もしファイルの終了だった場合
			if (inputi[i] == EOF)
			{
				// 読み込みを終了する
				end = true;
				break;
			}
		}

		switch (num)
		{
		case 2:
			strcpy_s(hit[hitindex].name,inputc);
			break;
		case 3:
			hit[hitindex].number = atoi(inputc);
			break;
		case 4:
			hit[hitindex].point.x = (float)atof(inputc);
			break;
		case 5:
			hit[hitindex].point.y = (float)atof(inputc);
			break;
		case 6:
			hit[hitindex].point.z = (float)atof(inputc);
			break;
		case 7:
			hit[hitindex].size = atoi(inputc);
			break;
		default:
			break;
		}

		num++;
	}
	*/
	// ファイルを閉じる
	FileRead_close(datafilep);

	// アニメーションのInitializeを実行
	animation.Initialize(ModelHandle);
}

void Model::Finalize()
{
	// モデルをメモリから削除
	MV1DeleteModel(ModelHandle);
	// アニメーションのFinalizeを実行
	animation.Finalize(ModelHandle);
}

void Model::Update()
{
	//UpdateFP();
	// アニメーションの更新
	animation.Update(ModelHandle);
}

void Model::Draw()
{
	// モデルをアルファ画像として表示させる
	MV1SetUseDrawMulAlphaColor( ModelHandle, true );
	// モデルの描画
	MV1DrawModel( ModelHandle );

	// 以下デバッグ用
	//DrawFrameModel();
	//DrawNamingFrameModel();
	//DrawFrameCrossModel();;
	//DrawPolyModel();

    /*for( int i = 0 ; i < MeshNum && i < 256; i ++ )
	{
		VECTOR point = MV1GetMeshMinPosition( ModelHandle, i );
		VECTOR screenpoint1 = ConvWorldPosToScreenPos( point ) ;
		VECTOR screenpoint2 = ConvWorldPosToScreenPos( point ) ;
		//screenpoint.x = ( screenpoint1.x + screenpoint2.x ) / 2;
		//screenpoint.y = ( screenpoint1.y + screenpoint2.y ) / 2 - 50.0f;
		screenpoint[i].x = screenpoint1.x;
		screenpoint[i].y = screenpoint1.y - 50.0f;
		float length_i = sqrt( pow( screenpoint[i].x - mpos.x, 2 ) + pow(screenpoint[i].y - mpos.y,2 ) );
		if ( length > length_i ) {
			length = length_i;
			memi = i;
		}
	}
	if ( length < 100.0f )
		MV1DrawMesh( ModelHandle, memi );*/
	//MV1DrawMesh( ModelHandle, 4 );

	/*int MeshNum;
	MeshNum = MV1GetMeshNum( ModelHandle );
	int memi = -1;
    for( int i = 0 ; i < MeshNum; i ++ )
	{
		VECTOR point = MV1GetMeshMaxPosition( ModelHandle, i );
		float length_i = sqrt( pow( point.x - mpos.x, 2 ) + pow( point.y - mpos.y, 2 ) + pow( point.z - mpos.z, 2 ) );
		if ( length > length_i ) {
			length = length_i;
			memi = i;
		}
	}*/

	// アニメーションの描画（何も無し、アニメーション情報等追加したい場合に）
	animation.Draw();
}

int Model::GetHandle()
{
	return ModelHandle;
}

int Model::LoadNewModel(const char* fileFilter)
{
	// ファイル選択ダイアログボックスの構造体
    OPENFILENAME ofn;
	// ファイルパス
	TCHAR path[MAX_PATH];
	// 実行ファイルまでのファイルパスを取得
	GetModuleFileName( NULL, path, MAX_PATH );
	// \の最後の出現位置を取得（実行ファイル名の直前）
	TCHAR* ptmp = _tcsrchr( path, _T('\\') );
	if ( ptmp != NULL )
	{
		// \がある場合
		// NULLを置きファイル名部分を消す
		ptmp = _tcsinc( ptmp );
		*ptmp = _T( '\0' );
	}
	else
	{
		//エラー
	}

	// 初期化
    ZeroMemory( &ofn,sizeof(ofn) );
    ofn.lStructSize=sizeof(ofn);
	// メインウィンドウをオーナーにする
    ofn.hwndOwner=GetMainWindowHandle();
	// フィルタの指定（基本NULLなのでここで指定）
    ofn.lpstrFilter=fileFilter;
    if( fileFilter==NULL ) ofn.lpstrFilter="モデルデータ(*.pmd;*.pmx)\0*.*pmd;*.*pmx\0";
    ofn.nFilterIndex=0;
	// 現在のパス（実行ファイル位置）を表示
	ofn.lpstrInitialDir = path;
    ofn.lpstrFile=FileName;
    ofn.nMaxFile=sizeof(FileName);
	// フラグの設定
    ofn.Flags=OFN_CREATEPROMPT | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
    //if ( FileName==NULL ) return 0;
    FileName[0]=0;
	// ret=0の時エラー
	int ret = GetOpenFileName( &ofn );
	if ( ret != 0 )
	{
		// モデルをメモリから削除
		MV1DeleteModel(ModelHandle);
		// アニメーションのFinalizeを実行
		animation.Finalize(ModelHandle);
		// 新しいファイルパスのモデルを読み込み
		LoadModel();
		// アニメーションを再読み込み
		animation.Finalize( ModelHandle );
		animation.Initialize( ModelHandle );
	}
    return ret;
}

void Model::LoadModel()
{
	//メインウィンドウを描画しない（モデル読み込み中は描画しない）
	SetWindowVisibleFlag( false );

	// モデルの読み込み
    ModelHandle = MV1LoadModel( FileName ) ;
	// 位置とサイズの指定
	MV1SetPosition(ModelHandle, VGet( 0.0f, 0.0f, 0.0f ));
	MV1SetScale(ModelHandle, VGet( 1.0f, 1.0f, 1.0f ));

	int MaterialNum, i;
	float DotWidth ;

	// マテリアルの数を取得
	MaterialNum = MV1GetMaterialNum( ModelHandle ) ;
	for( i = 0 ; i < MaterialNum ; i ++ )
	{
		// マテリアルの元々の輪郭線の太さを取得
		DotWidth = MV1GetMaterialOutLineDotWidth( ModelHandle, i ) ;

		// マテリアルの輪郭線の太さを0にする
		//MV1SetMaterialOutLineDotWidth( ModelHandle, i, DotWidth / 50.0f ) ;
		MV1SetMaterialOutLineWidth( ModelHandle, i, 0.0f ) ;
		// アルファブレンドで描画
		MV1SetMaterialDrawBlendMode( ModelHandle, i, DX_BLENDMODE_ALPHA) ;
	}
	
	// メッシュとフレームの数を読み込み（デバッグ用？）
	MeshNum = MV1GetMeshNum(ModelHandle);
	FrameNum = MV1GetFrameNum(ModelHandle);
	//メインウィンドウを描画する（モデル読み込み中は描画しない）
	SetWindowVisibleFlag( true );
}

void Model::UpdateFP()
{
	// フレーム数分のループ（制限256）
    for(int i = 0 ; i < FrameNum && i < 256; i ++ )
	{
		// i番目のフレームの座標を取得
		VECTOR point = MV1GetFramePosition( ModelHandle, i );
		// スクリーン上の位置を取得
		ScreenFramePoint[i] = ConvWorldPosToScreenPos( point ) ;
	}

	// 制限値までループ
    for( int i = 0 ; i < 256; i ++ )
	{
		// 当たり判定が無ければループ終了
		if ( hit[i].number == -1 )
		{
			i = 256;
			continue;
		}
		else
		{
			// 当たり判定の座標を得る
			// 当たり判定に関連付けされたフレーム座標を得る
			VECTOR point = MV1GetFramePosition( ModelHandle, hit[i].number );
			// フレーム座標に当たり判定位置を加算
			point.x += hit[i].point.x;
			point.y += hit[i].point.y;
			point.z += hit[i].point.z;
			// 当たり判定座標をスクリーン上の位置に変換し取得
			ScreenHitPoint[i] = ConvWorldPosToScreenPos( point ) ;
		}
	}
}

void Model::DrawPolyModel()
{
	// 一番近いフレームとの距離
	float length = 100.0f;
	// 参照用メッシュの更新
	for(int i = 0; i < FrameNum; i ++)
		MV1RefreshReferenceMesh( ModelHandle, i, true );
	
	// 参照用ポリゴンデータ構造体
    MV1_REF_POLYGONLIST RefPoly;
    // 参照用メッシュ情報の取得
	RefPoly = MV1GetReferenceMesh( ModelHandle, 0, TRUE );
	
	// ポリゴン番号記録（初期値-1 記録無し）
	int memi = -1;
	// 頂点記録
	int memj;
	// ポリゴンの数だけループ（全てだと処理が重いので注意）
    for( int i = 0 ; i < RefPoly.PolygonNum; i += 50 )
	{
		//ポリゴンの頂点分ループ（3だが重いので1つだけ）
		for(int j = 0 ; j < 1; j ++ )
		{
			// 頂点分ループの回数によってインデックス設定
			int index1, index2;
			switch (j)
			{
			case 0:
				index1 = 0;
				index2 = 1;
				break;
			case 1:
				index1 = 1;
				index2 = 2;
				break;
			default:
				index1 = 2;
				index2 = 0;
				break;
			}

			// ポリゴンとインデックスの値から2点の座標（線）を取得
			VECTOR line1, line2;
			line1 = RefPoly.Vertexs[ RefPoly.Polygons[ i ].VIndex[ index1 ] ].Position;
			line2 = RefPoly.Vertexs[ RefPoly.Polygons[ i ].VIndex[ index2 ] ].Position;

			// 今回の距離
			float length_i;
			// マウスの3D座標上の位置取得
			VECTOR mpos = Mouse::GetPos3D();
			// 取得した2点の座標を線分とし、線分とマウス座標の最短距離を記録
			length_i = Segment_Point_MinLength( line1, line2, mpos ) ;

			// 今回が一番近い場合
			if (length > length_i) {
				// 距離を更新し今回のポリゴン番号と頂点位置を記録
				length = length_i;
				memi = i;
				memj = j;
			}
			
			// 3D上にポリゴンの線を描画
			DrawLine3D(
					RefPoly.Vertexs[ RefPoly.Polygons[ i ].VIndex[ index1 ] ].Position,
					RefPoly.Vertexs[ RefPoly.Polygons[ i ].VIndex[ index2 ] ].Position,
					GetColor( 255,255,0 ) ) ;
		}
	}
	
	// もし一番近い座標が記録されていれば
	if (memi != -1)
	{
		// 頂点によってインデックス設定
		int index1, index2;
		switch (memj)
		{
		case 0:
			index1 = 0;
			index2 = 1;
			break;
		case 1:
			index1 = 1;
			index2 = 2;
			break;
		default:
			index1 = 2;
			index2 = 0;
			break;
		}
		
		// 該当する線を赤で描画
		DrawLine3D(
				RefPoly.Vertexs[ RefPoly.Polygons[ memi ].VIndex[ index1 ] ].Position,
				RefPoly.Vertexs[ RefPoly.Polygons[ memi ].VIndex[ index2 ] ].Position,
				GetColor( 255,0,0 ) ) ;
	}
}

void Model::DrawFrameModel()
{
	// 一番近いフレームとの距離
	float length = 100.0f;
	// フレーム番号記録（初期値-1 記録無し）
	int memi = -1;
	// フレームの数だけループ（全てだと処理が重いので注意）
    for(int i = 0 ; i < FrameNum && i < 256; i ++ )
	{
		// マウスのスクリーン位置取得
		VECTOR mpos = Mouse::GetPos2D();
		// i番目のフレームのスクリーン位置とマウス位置の距離を代入
		float length_i = sqrt(pow(ScreenFramePoint[i].x - mpos.x,2) + pow(ScreenFramePoint[i].y - mpos.y,2));
		
		// 今回が一番近い場合
		if (length > length_i) {
			// 距離を更新し今回のフレーム番号を記録
			length = length_i;
			memi = i;
		}
	}

	// もし一番近い座標が記録されていれば
	// そのフレームのみ描画する
	if (length < 100.0f)
		MV1DrawFrame(ModelHandle, memi);
	
	// フレームの数だけループ（全てだと処理が重いので注意）
    for(int i = 0 ; i < FrameNum && i < 256; i ++ )
	{
		// 基本の色（黄）
		int color = GetColor(255,255,0);
		// サイズ指定
		int size = 3;
		// もし一番近いフレーム番号なら赤に変更
		if (i == memi) color = GetColor(255,0,0);
		// 制限値までループ
		for(int j = 0 ; j < 256; j ++ )
		{
			// 当たり判定が無ければループ終了
			if (hit[j].number == -1)
			{
				j = 256;
				continue;
			}
			else
			{
				// もし当たり判定に関連するフレーム番号であれば
				// サイズを変更する
				if (hit[j].number == i) size = 5;
			}
		}
		// フレーム位置を円で描画
		DrawCircle((int)ScreenFramePoint[i].x, (int)ScreenFramePoint[i].y, size, color, true);
	}

	// もし一番近い座標が記録されていれば
	// 関連する情報を画面左上に描画
	if (memi != -1)
	{
		DrawFormatString(15, 180, GetColor(255, 255, 255), "%f", length);
		DrawFormatString(15, 195, GetColor(255, 255, 255), "%d", memi);
		DrawFormatString(15, 210, GetColor(255, 255, 255), "%s", MV1GetFrameName(ModelHandle, memi));
	}
}

void Model::DrawNamingFrameModel()
{
	// 一番近い当たり判定との距離
	float length = 100.0f;
	// 当たり判定番号記録（初期値-1 記録無し）
	int memi = -1;
	// 当たり判定の数だけループ（全てだと処理が重いので注意）
    for(int i = 0 ; i < 256; i ++ )
	{
		// マウスのスクリーン位置取得
		VECTOR mpos = Mouse::GetPos2D();
		// i番目の当たり判定のスクリーン位置とマウス位置の距離を代入
		float length_i = sqrt(pow(ScreenHitPoint[i].x - mpos.x,2) + pow(ScreenHitPoint[i].y - mpos.y,2));
		// 今回が一番近い場合
		if (length > length_i) {
			// 距離を更新し今回の当たり判定番号を記録
			length = length_i;
			memi = i;
		}
	}
	
	// 制限値までループ
    for(int i = 0 ; i < 256; i ++ )
	{
		// 基本の色（青）
		int color = GetColor(0,0,255);
		// もし当たり判定内であれば緑にする
		if (hit[i].size > length) color = GetColor(0,255,0);
		// 当たり判定位置を円で描画
		DrawCircle((int)ScreenHitPoint[i].x, (int)ScreenHitPoint[i].y, hit[i].size, color, true);
	}

	// もし一番近い座標が記録されていれば
	// 関連する情報を画面左上に描画
	if (memi != -1)
	{
		DrawFormatString(15, 360, GetColor(255, 255, 255), "%f", length);
		DrawFormatString(15, 375, GetColor(255, 255, 255), "%d", memi);
		DrawFormatString(15, 390, GetColor(255, 255, 255), "%s", hit[memi].name);
	}
}

void Model::DrawFrameCrossModel()
{
	// 一番近い当たり判定との距離
	float length = 100.0f;
	// 当たり判定番号記録（初期値-1 記録無し）
	int memi = -1;
	// 当たり判定の数だけループ（全てだと処理が重いので注意）
    for(int i = 0 ; i < FrameNum - 1 && i < 255; i ++ )
	{
		// マウスのスクリーン位置取得
		VECTOR mpos = Mouse::GetPos2D();
		// i番目と次のフレーム2点のスクリーン位置を線分とし、線分とマウス座標の最短距離を記録
		float length_i = Segment_Point_MinLength(ScreenFramePoint[i], ScreenFramePoint[i + 1], mpos);
		//float length_i = Segment_Point_MinLength(point1, point2, mpos);
		if (length > length_i) {
			length = length_i;
			memi = i;
		}
	}
	
	// フレーム-1の数だけループ（全てだと処理が重いので注意）
    for(int i = 0 ; i < FrameNum - 1 && i < 255; i ++ )
	{
		// 基本の色（黄）
		int color = GetColor(255,255,0);
		// もし一番近いフレーム番号なら赤に変更
		if (i == memi) color = GetColor(255,0,0);
		// i番目とi+1番目のフレーム間を線で描画
		DrawLine((int)ScreenFramePoint[i].x, (int)ScreenFramePoint[i].y, (int)ScreenFramePoint[i + 1].x, (int)ScreenFramePoint[i + 1].y, color);
	}

	// もし一番近い座標が記録されていれば
	// 関連する情報を画面左上に描画
	if (memi != -1)
	{
		DrawFormatString(15, 270, GetColor(255, 255, 255), "%f", length);
		DrawFormatString(15, 285, GetColor(255, 255, 255), "%d", memi);
		//if (strcmp(meshname[memi],""))
		//	DrawFormatString(15, 210, GetColor(255, 255, 255), "%s", meshname[memi]);
	}
}

int Model::GetAnimAmount()
{
	return animation.GetAmount();
}

void Model::Play(int num)
{
	animation.Play( num, ModelHandle );
}
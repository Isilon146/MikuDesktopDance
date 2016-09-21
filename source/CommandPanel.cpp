#include "CommandPanel.h"
#include "DxLib.h"

/*
	ウィンドウプロシージャを情的にしなければいけなかったため
	必要な変数だけ同じように
*/
BGM CommandPanel::bgm;
int CommandPanel::light;
int CommandPanel::volume;
int CommandPanel::anim;
int CommandPanel::focus;
int CommandPanel::animation_amount[8];
COMMAND_MESSAGE CommandPanel::message;
int CommandPanel::param;

int on;

int padding_x = 5;
int padding_y = 5;

#define BUTTON_ANIM_PREV 0
#define BUTTON_ANIM_NEXT 1
#define BUTTON_ANIM_START 2
#define BUTTON_LIGHT_DOWN 3
#define BUTTON_LIGHT_UP 4
#define BUTTON_VOLUME_DOWN 5
#define BUTTON_VOLUME_UP 6
#define BUTTON_BGM_LOAD 7
#define BUTTON_BGM_START 8
#define BUTTON_BGM_STOP 9
#define BUTTON_BGMANIM_START 10
#define BUTTON_FOCUS_DOWN 11
#define BUTTON_FOCUS_UP 12

void CommandPanel::Initialize( HINSTANCE hInstance, int nCmdShow )
{
	strcpy_s(szTitle, "MMDU操作パネル");
	strcpy_s(szWindowClass, "panelclass1");

	MyRegisterClass( hInstance );
	GethInstance = hInstance;
	GetnCmdShow = nCmdShow;
	on = false;

	light = 0;
	volume = 50;

	// 設定ファイルからカメラ情報を取得する
	int datafilep = FileRead_open("default.ini");
	int inputi[256];
	char inputc[256];
	int i = 0;
	for( i = 0; i < 4; i++ )
		while((FileRead_getc(datafilep)) != '\n');
	//拡大率（表示範囲）取得
	i = 0;
	while((inputi[0] = FileRead_getc(datafilep)) != '=');
	while((inputi[i] = inputc[i] = FileRead_getc(datafilep)) != '\n')
		i++;
	inputc[i - 1] = '\0';
	focus = atoi(inputc);

	//ファイルを閉じる
	FileRead_close(datafilep);

	//メッセージを何も無しの状態にする
	message = CME_NONE;
	param = 0;

	//アニメーション量を0に設定
	for( int i = 0; i < 8; i++ )
		animation_amount[i] = 0;
}

bool CommandPanel::Update()
{
	TranslateMessage( &msg );
	DispatchMessage( &msg );
	bool ret = (bool)IsWindow(FindWindow(szWindowClass, szTitle));
	return ret;
}

void CommandPanel::Open()
{
	if ( on ) return;
	if( !InitInstance( GethInstance, GetnCmdShow ) ) 
	{
		//エラー処理
		MessageBox(NULL , TEXT("command panel can not open") ,
			TEXT("メッセージボックス") ,
			MB_OK | MB_ICONINFORMATION);
		return;
	}
	on = true;
}


//----------------------------------------------
//　ウィンドウクラスの登録
//----------------------------------------------
ATOM CommandPanel::MyRegisterClass( HINSTANCE hInstance )
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof( WNDCLASSEX ); 

	wcex.style		= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc		= ( WNDPROC )WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon		= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= ( HBRUSH )(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= NULL;

	return RegisterClassEx( &wcex );
}

//----------------------------------------------
//　ウィンドウの作成と表示
//----------------------------------------------
BOOL CommandPanel::InitInstance( HINSTANCE hInstance, int nCmdShow )
{
	HWND hWnd;

	hWnd = CreateWindow( szWindowClass, szTitle, WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, CW_USEDEFAULT, 0, 
			  400, 430, NULL, NULL, hInstance, NULL );

	if( !hWnd ) 
	{
		return FALSE;
	}
	
	CreateWindow(
		TEXT("BUTTON"), TEXT("←A"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
		padding_x, padding_y + 40, 40, 20,
		hWnd, (HMENU)BUTTON_ANIM_PREV, hInstance, NULL
	);
	
	CreateWindow(
		TEXT("BUTTON"), TEXT("A→"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
		padding_x + 40, padding_y + 40, 40, 20,
		hWnd, (HMENU)BUTTON_ANIM_NEXT, hInstance, NULL
	);
	
	CreateWindow(
		TEXT("BUTTON"), TEXT("ASTART"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
		padding_x, padding_y + 65, 80, 20,
		hWnd, (HMENU)BUTTON_ANIM_START, hInstance, NULL
	);
	
	CreateWindow(
		TEXT("BUTTON"), TEXT("←L"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
		padding_x, padding_y + 140, 40, 20,
		hWnd, (HMENU)BUTTON_LIGHT_DOWN, hInstance, NULL
	);
	
	CreateWindow(
		TEXT("BUTTON"), TEXT("L→"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
		padding_x + 40, padding_y + 140, 40, 20,
		hWnd, (HMENU)BUTTON_LIGHT_UP, hInstance, NULL
	);
	
	CreateWindow(
		TEXT("BUTTON"), TEXT("←V"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
		padding_x, padding_y + 200, 40, 20,
		hWnd, (HMENU)BUTTON_VOLUME_DOWN, hInstance, NULL
	);
	
	CreateWindow(
		TEXT("BUTTON"), TEXT("V→"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
		padding_x + 40, padding_y + 200, 40, 20,
		hWnd, (HMENU)BUTTON_VOLUME_UP, hInstance, NULL
	);
	
	CreateWindow(
		TEXT("BUTTON"), TEXT("BGMLoad"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
		padding_x + 90, padding_y + 200, 80, 20,
		hWnd, (HMENU)BUTTON_BGM_LOAD, hInstance, NULL
	);
	CreateWindow(
		TEXT("BUTTON"), TEXT("BGMStart"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
		padding_x + 90, padding_y + 220, 80, 20,
		hWnd, (HMENU)BUTTON_BGM_START, hInstance, NULL
	);
	
	CreateWindow(
		TEXT("BUTTON"), TEXT("BGMStop"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
		padding_x + 90, padding_y + 240, 80, 20,
		hWnd, (HMENU)BUTTON_BGM_STOP, hInstance, NULL
	);
	
	CreateWindow(
		TEXT("BUTTON"), TEXT("BGMANIMStart"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
		padding_x + 90, padding_y + 265, 120, 20,
		hWnd, (HMENU)BUTTON_BGMANIM_START, hInstance, NULL
	);
	
	CreateWindow(
		TEXT("BUTTON"), TEXT("←F"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
		padding_x, padding_y + 320, 40, 20,
		hWnd, (HMENU)BUTTON_FOCUS_DOWN, hInstance, NULL
	);
	
	CreateWindow(
		TEXT("BUTTON"), TEXT("F→"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
		padding_x + 40, padding_y + 320, 40, 20,
		hWnd, (HMENU)BUTTON_FOCUS_UP, hInstance, NULL
	);

	ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );

	return TRUE;
}

//----------------------------------------------
//　ウィンドウプロシージャ
//----------------------------------------------
LRESULT CALLBACK CommandPanel::WndProc(HWND hWnd, UINT uint_message, WPARAM wParam, LPARAM lParam)
{
	switch( uint_message ) 
	{
		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
			case BUTTON_ANIM_PREV:
				anim --;
				if ( anim < 0 ) anim = 0;
				break;
			case BUTTON_ANIM_NEXT:
				anim ++;
				if ( anim >= animation_amount[0] ) anim = animation_amount[0] - 1;
				break;
			case BUTTON_ANIM_START:
				message = CME_ANIMATION;
				param = anim;
				break;
			case BUTTON_LIGHT_DOWN:
				light -= 20;
				if ( light < 0 ) light = 0;
				SetLightAmbColor( GetColorF( light * 0.01f, light * 0.01f, light * 0.01f, 0.0f ) ) ;
				break;
			case BUTTON_LIGHT_UP:
				light += 20;
				if ( light > 100 ) light = 100;
				SetLightAmbColor( GetColorF( light * 0.01f, light * 0.01f, light * 0.01f, 0.0f ) ) ;
				break;
			case BUTTON_VOLUME_DOWN:
				volume -= 10;
				if ( volume < 0 ) volume = 0;
				bgm.Volume( volume );
				break;
			case BUTTON_VOLUME_UP:
				volume += 10;
				if ( volume > 100 ) volume = 100;
				bgm.Volume( volume );
				break;
			case BUTTON_BGM_LOAD:
				bgm.Load();
				break;
			case BUTTON_BGM_START:
				bgm.Start( volume );
				break;
			case BUTTON_BGM_STOP:
				bgm.Stop();
				break;
			case BUTTON_BGMANIM_START:
				message = CME_ANIMATION;
				param = anim;
				bgm.Start( volume );
				break;
			case BUTTON_FOCUS_DOWN:
				focus -= 2;
				if ( focus < 2 ) focus = 2;
				message = CME_FOCUS;
				param = focus;
				break;
			case BUTTON_FOCUS_UP:
				focus += 2;
				if ( focus > 128 ) focus = 128;
				message = CME_FOCUS;
				param = focus;
				break;
			}
			InvalidateRect( hWnd , NULL , TRUE );
			break;
		case WM_CREATE:
			break;
		case WM_LBUTTONDOWN:
			break;
		case WM_RBUTTONDOWN:
			break;
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint( hWnd, &ps );
				char word[256];
				FillRect( hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1) );
				SetTextColor( hdc, RGB( 0, 0, 0 ) );
				
				strcpy_s( word, "アニメーション" );
				TextOut( hdc, padding_x, padding_y, word, strlen(word) );
				strcpy_s( word, "番号" );
				TextOut( hdc, padding_x + 10, padding_y + 20, word, strlen(word) );
				sprintf_s( word, "%d", anim );
				TextOut( hdc, padding_x + 60, padding_y + 20, word, strlen(word) );

				strcpy_s( word, "ライト調整" );
				TextOut( hdc, padding_x, padding_y + 120, word, strlen(word) );
				
				strcpy_s( word, "BGM" );
				TextOut( hdc, padding_x, padding_y + 180, word, strlen(word) );
				sprintf_s( word, "%d", volume );
				strcat_s( word, " %" );
				TextOut( hdc, padding_x + 10, padding_y + 240, word, strlen(word) );
				
				strcpy_s( word, "フォーカス（距離）" );
				TextOut( hdc, padding_x, padding_y + 300, word, strlen(word) );
				sprintf_s( word, "%d", focus );
				TextOut( hdc, padding_x + 10, padding_y + 360, word, strlen(word) );

				EndPaint( hWnd, &ps );
			}
			break;
		case WM_DESTROY:
			on = false;
            DestroyWindow( hWnd );
			break;
		default:
			break;
	}

	return DefWindowProc( hWnd, uint_message, wParam, lParam );
}

void CommandPanel::AnimResist(int person_number, int amount)
{
	animation_amount[person_number] = amount;
}

COMMAND_MESSAGE CommandPanel::Message()
{
	COMMAND_MESSAGE div = message;
	message = CME_NONE;
	return div;
}

int CommandPanel::GetParam()
{
	return param;
}
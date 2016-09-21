#include "DxLib.h"

#ifndef COLOR_DEF
#define COLOR_DEF

/**
色取得ヘッダー
各色をその場で取得せず、色名で指定可能にする
色名を使ってるので、他の変数と被らないか確認必須
**/

/*白黒*/
#define black GetColor(0,0,0)//黒
#define gray GetColor(128,128,128)//灰
#define white GetColor(255,255,255)//白
/*基本色*/
#define red GetColor(255,0,0)//赤
#define green GetColor(0,128,0)//緑
#define blue GetColor(0,0,255)//青
#define yellow GetColor(255,255,0)//黄

//ピンク
#define pink GetColor(255,128,255)
//オレンジ
#define orange GetColor(255,128,0)
//黄緑
#define yellowgreen GetColor(0,255,0)
#define ygreen GetColor(0,255,0)
#define gyellow GetColor(0,255,0)
//水色
#define skyblue GetColor(0,255,255)
#define wblue GetColor(0,255,255)
//紫
#define purple GetColor(128,0,255)

/*変化色*/
#define bpink GetColor(255,0,255)//ピンク　濃いめ
#define rpink GetColor(255,0,128)//ピンク　赤
#define bpurple GetColor(70,0,150)//紫　濃いめ
#define gblue GetColor(0,255,130)
#define bgreen GetColor(0,255,130)//青緑
#define dblue GetColor(0,0,128)//紺
#define cream GetColor(255,220,160)//肌色、クリーム色
#define wyellow GetColor(255,255,128)//白金色
#define wwblue GetColor(200,200,255)//白青
#define wwred GetColor(255,200,200)//白赤

#define sysgray GetColor(208,208,208)
#define sysgray_act GetColor(208,224,224)
#define sysgray_act_border GetColor(208,240,240)


#endif //COLOR_DEF
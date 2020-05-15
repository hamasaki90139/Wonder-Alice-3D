//=============================================================================
//
// タイマー処理 [timer.h]
// Author : 濱崎 小春
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_TIMER		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_FRAME_TIMER	"data/TEXTURE/tokei.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_TOKEI		"data/TEXTURE/tokei_hari.png"	// 読み込むテクスチャファイル名
#define	TIMER_SIZE_X		(35.0f)							// タイマーの数字の幅
#define	TIMER_SIZE_Y		(50.0f)							// タイマーの数字の高さ
#define	TIMER_INTERVAL_X	(0.0f)							// タイマーの数字の表示間隔
#define TIMER_MAX			(60)

#define	NUM_PLACE			(3)			// タイマーの桁数

#define	TIMER_POS_X			(SCREEN_WIDTH / 2 - ((TIMER_SIZE_X * NUM_PLACE + TIMER_INTERVAL_X * (NUM_PLACE - 1)) / 2) )	// タイマーの表示基準位置Ｘ座標
#define	TIMER_POS_Y			(30.0f)																						// タイマーの表示基準位置Ｙ座標

#define	HARI_POS_X			(399.0f)	// タイマーの表示基準位置Ｘ座標
#define	HARI_POS_Y			(70.0f)																						// タイマーの表示基準位置Ｙ座標
#define	HARI_SIZE_X			(60.0f)							// タイマーの数字の幅
#define	HARI_SIZE_Y			(60)							// タイマーの数字の高さ

#define TIMER_TEXTURE_PATTERN_DIVIDE_X	(1)
#define TIMER_TEXTURE_PATTERN_DIVIDE_Y	(1)
#define TIMER_TIME_ANIMATION				(40)	// アニメーションの切り替わるカウント
#define TIMER_ANIM_PATTERN_NUM			(TIMER_TEXTURE_PATTERN_DIVIDE_X*TIMER_TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct	// ケーキ構造体
{
	LPDIRECT3DTEXTURE9		g_pD3DTextureTimer;	// テクスチャへのポリゴン
	bool					use;						// true:使用  false:未使用
	D3DXVECTOR3				pos;						// ポリゴンの移動量
	D3DXVECTOR3				rot;						// ポリゴンの回転量
	VERTEX_2D				vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク
	float					Radius;						// オブジェクトの半径
	float					BaseAngle;					// オブジェクトの角度
} TIMER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTimer(void);
void UninitTimer(void);
void UpdateTimer(void);
void DrawTimer(void);

void EnableTimer(bool bEnable);
void ResetTimer(int nTime = 999);
//CAKE *GetCake(void);

#endif

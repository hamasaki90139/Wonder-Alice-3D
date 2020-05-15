//=============================================================================
//
// ライフ処理 [life.h]
// Author : 濱崎 小春
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_LIFE		"data/TEXTURE/life001.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_FRAME_LIFE	"data/TEXTURE/cloud001.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_PLAYER_UI	"data/TEXTURE/★alice_001.png"		// 読み込むテクスチャファイル名

#define	LIFE_SIZE_X			(35.0f)								// ライフの数字の幅
#define	LIFE_SIZE_Y			(45.0f)								// ライフの数字の高さ
#define	LIFE_INTERVAL_X		(0.0f)								// ライフの数字の表示間隔

#define	NUM_PLACE			(5)									// ライフの桁数
#define	LIFE_POS_X			(300.0f)							// ライフの表示基準位置Ｘ座標
#define	LIFE_POS_Y			(470.0f)							// ライフの表示基準位置Ｙ座標

#define	PLAYER_UI_SIZE_X	(550.0f)							// ライフの数字の幅
#define	PLAYER_UI_SIZE_Y	(550.0f)							// ライフの数字の高さ
#define	PLAYER_UI_POS_X		(-120.0f)							// ライフの表示基準位置Ｘ座標
#define	PLAYER_UI_POS_Y		(300.0f)							// ライフの表示基準位置Ｙ座標

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitLife(void);
void UninitLife(void);
void UpdateLife(void);
void DrawLife(void);

void ChangeLife(int value);

#endif

//=============================================================================
//
// チュートリアル処理 [tutorial.h]
// Author : 濱崎 小春
//
//=============================================================================
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_TUTORIAL01		("data/TEXTURE/tutorial001.png")		// 読み込むテクスチャファイル名
#define	TEXTURE_TUTORIAL02		("data/TEXTURE/tutorial002.png")		// 読み込むテクスチャファイル名
#define	TUTORIAL_POS_X			(0)										// タイトルの表示位置
#define	TUTORIAL_POS_Y			(0)										// タイトルの表示位置
#define	TUTORIAL_SIZE_X			(800)									// タイトルの幅
#define	TUTORIAL_SIZE_Y			(600)									// タイトルの高さ

// 画面遷移定数
typedef enum
{
	SOUSA,
	GAMEN,
}TUTORIAL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTutorial(int type);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);
TUTORIAL GetTutorial(void);


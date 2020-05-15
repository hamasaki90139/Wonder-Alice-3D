//=============================================================================
//
// リザルト画面処理 [result.h]
// Author : 濱崎 小春
//
//=============================================================================
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RESULT01				("data/TEXTURE/★titlebg_003.png")			// 読み込むテクスチャファイル名
#define	TEXTURE_RESULT02				("data/TEXTURE/★queen_001.png")				// 読み込むテクスチャファイル名
#define	TEXTURE_RESULT03				("data/TEXTURE/★overalice_001.png")			// 読み込むテクスチャファイル名
#define	TEXTURE_RESULT_GAMECLEAR		("data/TEXTURE/★gameclear_001.png")			// 読み込むテクスチャファイル名
#define	TEXTURE_RESULT_GAMEOVER			("data/TEXTURE/★gameover_001.png")			// 読み込むテクスチャファイル名
#define	RESULT01_POS_X			(0)							// リザルトの表示位置
#define	RESULT01_POS_Y			(0)							// リザルトの表示位置
#define	RESULT01_SIZE_X			(800)						// リザルトの幅
#define	RESULT01_SIZE_Y			(600)						// リザルトの高さ
#define	RESULT02_POS_X			(400)						// リザルトの表示位置
#define	RESULT02_POS_Y			(180)						// リザルトの表示位置
#define	RESULT02_SIZE_X			(400)						// リザルトの幅
#define	RESULT02_SIZE_Y			(400)						// リザルトの高さ
#define	RESULT03_POS_X			(400)						// リザルトの表示位置
#define	RESULT03_POS_Y			(180)						// リザルトの表示位置
#define	RESULT03_SIZE_X			(400)						// リザルトの幅
#define	RESULT03_SIZE_Y			(400)						// リザルトの高さ

#define	RESULTLOGO_POS_X		(80)						// リザルトロゴの表示位置
#define	RESULTLOGO_POS_Y		(100)						// リザルトロゴの表示位置
#define	RESULTLOGO_SIZE_X		(480)						// リザルトロゴの幅
#define	RESULTLOGO_SIZE_Y		(80)						// リザルトロゴの高さ

#define RESULTSCORE_SIZE_W		(64)						// テクスチャサイズ
#define RESULTSCORE_SIZE_H		(128)						// 同上
#define	RESULTSCORE_POS_X		(410)															// スコアの幅
#define	RESULTSCORE_POS_Y		((SCREEN_HEIGHT/2)-(RESULTSCORE_SIZE_H/2))						// スコアの高さ
#define RESULT_MENU_MAX			(2)							//リザルト内メニュー

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitResult(int type);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);


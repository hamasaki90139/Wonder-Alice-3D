//=============================================================================
//
// ポーズ画面処理 [pose.h]
// Author : 濱崎小春
//
//=============================================================================
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_POSE_TITLE		("data/TEXTURE/★pose_001.png")		// 読み込むテクスチャファイル名
#define	TEXTURE_POSE_RETURN		("data/TEXTURE/★pose_002.png")		// 読み込むテクスチャファイル名
#define	TEXTURE_POSE_BG			("data/TEXTURE/★titlebg_002.png")		// 読み込むテクスチャファイル名

#define	RETRY_POS_X			(160)								// リトライの表示位置
#define	RETRY_POS_Y			(200)								// リトライの表示位置
#define	RETRYMENU_SIZE_X	(500)							// タイトルの幅
#define	RETRYMENU_SIZE_Y	(120)							// タイトルの高さ
#define	RETURN_POS_X		(160)							// タイトルの表示位置
#define RETURN_POS_Y		(350)							// タイトルの表示位置
//#define	TITLE02_SIZE_X			(550)							// タイトルの幅
//#define	TITLE02_SIZE_Y			(550)							// タイトルの高さ
//
//#define	TITLELOGO_POS_X			(15)							// タイトルロゴの表示位置
//#define	TITLELOGO_POS_Y			(80)							// タイトルロゴの表示位置
//#define	TITLELOGO_SIZE_X		(400)							// タイトルロゴの幅
//#define	TITLELOGO_SIZE_Y		(80)							// タイトルロゴの高さ
//#define	TITLE_NEWGAME_POS_X		(190)							// タイトルロゴの表示位置
//#define	TITLE_NEWGAME_POS_Y		(250)							// タイトルロゴの表示位置
//#define	TITLE_EXIT_POS_X		(190)							// タイトルロゴの表示位置
//#define	TITLE_EXIT_POS_Y		(350)							// タイトルロゴの表示位置
//#define	TITLE_MENU_SIZE_X		(250)							// タイトルロゴの幅
//#define	TITLE_MENU_SIZE_Y		(60)							// タイトルロゴの高さ
//
//#define MENU_MAX				(2)								//メニュー数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPose(int type);
void UninitPose(void);
void UpdatePose(void);
void DrawPose(void);
bool GetPose(void);

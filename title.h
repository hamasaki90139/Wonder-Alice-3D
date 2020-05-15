//=============================================================================
//
// タイトル画面処理 [title.h]
// Author : 濱崎 小春
//
//=============================================================================
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_TITLE01			("data/TEXTURE/title.png")				// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE02			("data/TEXTURE/★titlealice.png")		// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE_LOGO		("data/TEXTURE/★title_001.png")		// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE_NEWGAME	("data/TEXTURE/★newgame_001.png")		// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE_EXIT		("data/TEXTURE/★exit_001.png")			// 読み込むテクスチャファイル名

#define	TITLE01_POS_X			(0)								// タイトルの表示位置
#define	TITLE01_POS_Y			(0)								// タイトルの表示位置
#define	TITLE01_SIZE_X			(800)							// タイトルの幅
#define	TITLE01_SIZE_Y			(600)							// タイトルの高さ
#define	TITLE02_POS_X			(80)							// タイトルの表示位置
#define	TITLE02_POS_Y			(-30)							// タイトルの表示位置
#define	TITLE02_SIZE_X			(700)							// タイトルの幅
#define	TITLE02_SIZE_Y			(700)							// タイトルの高さ

#define	TITLELOGO_POS_X			(15)							// タイトルロゴの表示位置
#define	TITLELOGO_POS_Y			(80)							// タイトルロゴの表示位置
#define	TITLELOGO_SIZE_X		(400)							// タイトルロゴの幅
#define	TITLELOGO_SIZE_Y		(80)							// タイトルロゴの高さ
#define	TITLE_NEWGAME_POS_X		(190)							// タイトルロゴの表示位置
#define	TITLE_NEWGAME_POS_Y		(250)							// タイトルロゴの表示位置
#define	TITLE_EXIT_POS_X		(190)							// タイトルロゴの表示位置
#define	TITLE_EXIT_POS_Y		(350)							// タイトルロゴの表示位置
#define	TITLE_MENU_SIZE_X		(250)							// タイトルロゴの幅
#define	TITLE_MENU_SIZE_Y		(60)							// タイトルロゴの高さ

#define	TITLE_SKY_POS_X			(0)							// タイトルの表示位置
#define	TITLE_SKY_POS_Y			(0)							// タイトルの表示位置
#define	TITLE_SKY_SIZE_X			(2560)							// タイトルの幅
#define	TITLE_SKY_SIZE_Y			(480)							// タイトルの高さ

#define MENU_MAX				(2)								//メニュー数
#define NUM_BG					(2)		//背景の数

// 画面遷移定数
typedef enum
{
	NEWGAME,
	EXIT,
}TITLEMENU;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTitle(int type);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);


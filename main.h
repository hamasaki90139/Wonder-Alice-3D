//=============================================================================
//
// メイン処理 [main.h]
// Author : 濱崎 小春
//
//=============================================================================
#pragma once

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "windows.h"
#include "d3dx9.h"

#define DIRECTINPUT_VERSION (0x0800)	// 警告対策
#include "dinput.h"
#include "xaudio2.h"					// XAudio2関連のヘッダー

//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#if 1	// [ここを"0"にした場合、"構成プロパティ" -> "リンカ" -> "入力" -> "追加の依存ファイル"に対象ライブラリを設定する]
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")
#endif

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// ３Ｄポリゴン頂点フォーマット( 頂点座標[3D] / 法線 / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#define SCREEN_WIDTH	(800)				// ウインドウの幅
#define SCREEN_HEIGHT	(600)				// ウインドウの高さ
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// ウインドウの中心Ｙ座標

#define	NUM_VERTEX		(4)		// 頂点数
#define	NUM_POLYGON		(2)		// ポリゴン数

#define LR					(2)				//左右
#define R					(0)				//右
#define L					(1)				//左

// 上記２Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;		// 頂点座標
	float		rhw;		// テクスチャのパースペクティブコレクト用
	D3DCOLOR	diffuse;	// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標
} VERTEX_2D;

// 上記３Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;		// 頂点座標
	D3DXVECTOR3 nor;		// 法線ベクトル
	D3DCOLOR	diffuse;	// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標
} VERTEX_3D;

//パーツの初期値セット用構造体定義
typedef struct
{
	D3DXVECTOR3	pos;
	D3DXVECTOR3	rot;
	D3DXVECTOR3	scl;

} PLAYER_SET;

//パーツの初期値セット用構造体定義
typedef struct
{
	D3DXVECTOR3	pos;
	D3DXVECTOR3	rot;
	D3DXVECTOR3	scl;

} ENEMY_SET;

enum E_PARTS //パーツ列挙
{
	BODY,
	HEAD,
	ARM_R,
	ARM_L,
	LEG_R,
	LEG_L,
	PARTS_MAX
};

// 画面遷移定数
typedef enum
{
	STOP,
	WALK,
	JUMP,
	ATTACK,
	HIT,
	DEAD,
}STATUS;

// 画面遷移定数
enum E_SCENE
{
	SCENE_TITLE,
	SCENE_TUTORIAL,
	SCENE_GAME,
	SCENE_RETRY,
	SCENE_RESULT,
	SCENE_MAX
};

//*************************************
// モードの種類
//*************************************
typedef enum
{
	MODE_TITLE = 0,			// タイトル画面
	MODE_TUTORIAL,
	MODE_GAME,				// ゲーム画面
	MODE_RESULT,			// リザルト画面
	MODE_MAX
} MODE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);
void SetScene(int Scene);
//void InitGame(void);
void SetMode(MODE mode);
MODE GetMode(void);


//=============================================================================
//
// スコア処理 [score.h]
// Author : 濱崎 小春
//
//=============================================================================
#pragma once

// マクロ定義
#define SCORE_TEXTURE		("data/TEXTURE/★score_001.png")	// サンプル用画像
#define SCORE_SIZE_W		(64)								// テクスチャサイズ
#define SCORE_SIZE_H		(128)								// 同上
#define SCORE_POS_X			(700)								// ポリゴンの初期位置X
#define SCORE_POS_Y			(450)								// 同上
#define SCORE_MAX			(99999)								// スコアの最大値
#define SCORE_MIN			(0)									// スコアの最小値
#define SCORE_DIGIT			(4)									// 桁数
#define SCORE_MAG_MIN		(1)									// スコア初期倍率

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct	// スコア構造体
{
	int								score;			// スコア
	int								score_mag;		//スコア倍率の初期化
	D3DXVECTOR3						posScore;		// ポリゴンの移動量

}SCORE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitScore(int type);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);

void AddScore(int add, int mag);
SCORE *GetScore(void);


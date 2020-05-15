//=============================================================================
//
// エフェクト処理 [effect.h]
// Author : 濱崎 小春
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_EFFECT01		"data/TEXTURE/effect000.jpg"	// 読み込むテクスチャファイル名
#define	TEXTURE_EFFECT02		"data/TEXTURE/effect001.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_EFFECT03		"data/TEXTURE/effect002.png"	// 読み込むテクスチャファイル名

#define	EFFECT_SIZE_X			(300.0f)						// ビルボードの幅
#define	EFFECT_SIZE_Y			(300.0f)						// ビルボードの高さ
#define	VALUE_MOVE_BULLET		(2.0f)							// 移動速度

#define	MAX_EFFECT				(4096)							// エフェクト最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 回転
	D3DXVECTOR3 scale;		// スケール
	D3DXVECTOR3 move;		// 移動量
	D3DXCOLOR col;			// 色
	float fSizeX;			// 幅
	float fSizeY;			// 高さ
	int nTimer;				// タイマー
	float nDecAlpha;		// 減衰値
	bool bUse;				// 使用しているかどうか
} EFFECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);

int SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nTimer);
EFFECT *GetEffect(void);
int GetEffectNo(int no);
#endif

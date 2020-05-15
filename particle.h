//=============================================================================
//
// パーティクル処理 [particle.h]
// Author : 濱崎 小春
//
//=============================================================================
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_PARTICLE			"data/TEXTURE/effect000.jpg"	// 読み込むテクスチャファイル名
#define	PARTICLE_SIZE_X				(50.0f)							// ビルボードの幅
#define	PARTICLE_SIZE_Y				(50.0f)							// ビルボードの高さ
#define	VALUE_MOVE_PARTICLE			(20.0f)							// 移動速度
#define	MAX_PARTICLE				(512)							// ビルボード最大数

#define	DISP_SHADOW													// 影の表示
//#undef DISP_SHADOW

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
	int nIdxShadow;			// 影ID
	float nLife;				// 寿命
	bool bUse;				// 使用しているかどうか
	int cnt;
} PARTICLE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);

int SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nLife);
PARTICLE *GetParticle(int enemyNo, int particleNo);
D3DXVECTOR3 GetParticlePos(int enemyNo, int particleNo);
D3DXVECTOR3 GetParticleMove(int enemyNo, int particleNo);
int GetTime(int enemyNo);
float GetSize(int enemyNo);
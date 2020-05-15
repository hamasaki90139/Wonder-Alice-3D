//=============================================================================
//
// メッシュ空の処理 [meshsky.h]
// Author : 濱崎 小春
//
//=============================================================================
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_SKY	"data/TEXTURE/sky000.png"		// 読み込むテクスチャファイル名
#define	VALUE_MOVE_SKY		(5.0f)							// 移動速度
#define	VALUE_ROTATE_SKY	(D3DX_PI * 0.001f)				// 回転速度

#define	SKY_HEIGHT_RATE		(2.0f)		// 空ドームの高さ係数

#define	TEX_COUNT_LOOP		(2)			// テクスチャの繰り返し回数

#define	MAX_MESH_SKY		(2)			// 空の総数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMeshSky(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
	int nNumBlockX, int nNumBlockZ,
	float fRadius, float fRotY, int nTexType = -1, bool bReverse = false);
void UninitMeshSky(void);
void UpdateMeshSky(void);
void DrawMeshSky(void);


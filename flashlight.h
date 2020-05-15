//=============================================================================
//
// 懐中電灯処理 [flashlight.cpp]
// Author : GP11B292 29 濱崎 小春
//
//=============================================================================
#pragma once
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_FLASHLIGHT		(1)					// 懐中電灯の数
#define	VALUE_MOVE			(2.0f)				// 移動量
#define	POS_X_F_LIGHT		(0.0f)				// ライトの初期位置(X座標)
#define	POS_Y_F_LIGHT		(20.0f)				// ライトの初期位置(Y座標)
#define	POS_Z_F_LIGHT		(-200.0f)			// ライトの初期位置(Z座標)
#define	ROT_X_F_LIGHT		(0.1f)				// ライトの初期回転量(X座標)
#define	ROT_Y_F_LIGHT		(0.0f)				// ライトの初期回転量(Y座標)
#define	ROT_Z_F_LIGHT		(0.0f)			// ライトの初期回転量(Z座標)
#define	SCL_F_LIGHT			(0.5f)				// ライトの初期スケール

#define LIGHT_HIT_SITA		(12.3f)				// ライトの衝突判定用の角度
#define LIGHT_HIT_R1		(500)				// ライトの衝突判定用の半径(始点～終点）
#define LIGHT_HIT_R2		(100)				// ライトの衝突判定用の半径(始点～プレイヤーの位置）

struct FLASHLIGHT
{
	D3DXVECTOR3			pos;				// モデルの位置
	D3DXVECTOR3			rot;				// モデルの向き(回転)
	D3DXVECTOR3			scl;				// モデルの大きさ(スケール)

	float				spd;				// 移動スピード

	LPDIRECT3DTEXTURE9	pD3DTexture;		// テクスチャへのポインタ
	LPD3DXMESH			pD3DXMesh;			// メッシュ情報へのポインタ(xファイル表示関数用）
	LPD3DXBUFFER		pD3DXBuffMat;		// マテリアル情報へのポインタ(xファイル表示関数用）
	DWORD				nNumMat;			// マテリアル情報の数(xファイル表示関数用）

	D3DXMATRIX			mtxWorld;			// ワールドマトリックス(行列)※ワールド変換用
	CAMERA				*parent;			// 親子判定用

	D3DXMATRIX			mtxRoll;			// フラッシュライトの回転行列※当たり判定用
	D3DXVECTOR3			hitpos;		//衝突判定用フラッシュライトat

};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitFlashLight(int type);
void UninitFlashLight(void);
void UpdateFlashLight(void);
void DrawFlashLight(void);
void FlashLightHitPos(void);
FLASHLIGHT *GetFLashLight(void);

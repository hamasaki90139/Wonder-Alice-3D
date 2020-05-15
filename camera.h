//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 濱崎 小春
//
//=============================================================================
#pragma once

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "d3dx9.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TILT_Z_CAM		(100.0f)
#define	TILT_Y_CAM		(10.0f)

#define	POS_X_CAM		(0.0f)			// カメラの初期位置(X座標)
#define	POS_Y_CAM		(100.0f)			// カメラの初期位置(Y座標)
#define	POS_Z_CAM		(-500.0f)		// カメラの初期位置(Z座標)

struct CAMERA
{
	D3DXVECTOR3			pos;			// カメラの視点(位置)
	D3DXVECTOR3			at;				// カメラの注視点
	D3DXVECTOR3			up;				// カメラの上方向ベクトル
	D3DXVECTOR3			rot;			// カメラの回転
	float				len;			// カメラの視点と注視点の距離

	D3DXVECTOR3			*moveatpattern;
	float				moveat_time;
	float				atdt;
	int					atsize;

	D3DXVECTOR3			*movepospattern;
	float				movepos_time;
	float				posdt;
	int					possize;

	float				dt;


	D3DXMATRIX			mtxRoll;
	D3DXMATRIX			mtxView;		// ビューマトリックス
	D3DXMATRIX			mtxProjection;	// プロジェクションマトリックス
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera();
CAMERA *GetCamera();
D3DXMATRIX GetMtxView(void);

//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : 濱崎 小春
//
//=============================================================================
#include "main.h"
#include "debugproc.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "collision.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// ビュー平面の視野角
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比	
#define	VIEW_NEAR_Z		(10.0f)											// ビュー平面のNearZ値
#define	VIEW_FAR_Z		(20000.0f)										// ビュー平面のFarZ値

#define	VALUE_MOVE_CAMERA	(2.0f)										// カメラの移動量
#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.01f)							// カメラの回転量

//*****************************************************************************
// グローバル変数
//*****************************************************************************
CAMERA			g_Camera;				// カメラデータ

int				g_LockEneNom;
bool			g_CameraLock;
bool			g_Cameramove;
D3DXMATRIX		g_mtxView;					// ビューマトリックス

D3DXVECTOR3 move_Cameratblpos[] = {
	D3DXVECTOR3(0.0f, 40.0f, -150.0f),
	D3DXVECTOR3(0.0f, 23.0f, -75.0f),
	D3DXVECTOR3(5.0f, 6.0f, 0.0f),
	D3DXVECTOR3(0.0f, 23.0f, 80.0f),
	D3DXVECTOR3(0.0f, 40.0f, 160.0f),
	D3DXVECTOR3(20.0f, 50.0f, 120.0f),
	D3DXVECTOR3(50.0f, 60.0f, 80.0f),
	D3DXVECTOR3(70.0f, 70.0f, 40.0f),
	D3DXVECTOR3(80.0f, 80.0f, 0.0f),
	D3DXVECTOR3(70.0f, 70.0f, -37.0f),
	D3DXVECTOR3(50.0f, 60.0f, -74.0f),
	D3DXVECTOR3(20.0f, 50.0f, -111.0f),
	D3DXVECTOR3(0.0f, 40.0f, -150.0f),
};

D3DXVECTOR3 move_Cameratblat[] = {
	D3DXVECTOR3(0.0f, 40.0f, 0.0f),
	D3DXVECTOR3(0.0f, 30.0f, 0.0f),
	D3DXVECTOR3(0.0f, 20.0f, 0.0f),
	D3DXVECTOR3(0.0f, 30.0f, 0.0f),
	D3DXVECTOR3(0.0f, 40.0f, 0.0f),
	D3DXVECTOR3(0.0f, 42.0f, 0.0f),
	D3DXVECTOR3(0.0f, 44.0f, 0.0f),
	D3DXVECTOR3(0.0f, 46.0f, 0.0f),
	D3DXVECTOR3(0.0f, 48.0f, 0.0f),
	D3DXVECTOR3(0.0f, 46.0f, 0.0f),
	D3DXVECTOR3(0.0f, 44.0f, 0.0f),
	D3DXVECTOR3(0.0f, 42.0f, 0.0f),
	D3DXVECTOR3(0.0f, 40.0f, 0.0f),
};

//=============================================================================
// 初期化処理
//=============================================================================
void InitCamera(void)
{
	g_Cameramove = false;

	g_LockEneNom = NULL;
	g_CameraLock = false;
	g_Camera.pos = D3DXVECTOR3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);
	g_Camera.at = D3DXVECTOR3(0.0f, TILT_Y_CAM, TILT_Z_CAM);
	g_Camera.up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_Camera.dt = 0;

	g_Camera.moveatpattern = move_Cameratblat;
	g_Camera.atsize = sizeof(move_Cameratblat) / sizeof(D3DXVECTOR3);
	g_Camera.atdt = 0.05f;
	g_Camera.moveat_time = 0;

	g_Camera.movepospattern = move_Cameratblpos;
	g_Camera.possize = sizeof(move_Cameratblpos) / sizeof(D3DXVECTOR3);
	g_Camera.posdt = 0.05f;
	g_Camera.movepos_time = 0;

	// 視点と注視点の距離を計算
	float vx, vz;
	vx = g_Camera.pos.x - g_Camera.at.x;
	vz = g_Camera.pos.z - g_Camera.at.z;
	g_Camera.len = sqrtf(vx * vx + vz * vz);

}

//=============================================================================
// カメラの終了処理
//=============================================================================
void UninitCamera(void)
{

}

//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateCamera(void)
{
	if (GetKeyboardTrigger(DIK_TAB))
	{
		g_CameraLock = !g_CameraLock;
	}

	if (g_CameraLock == false)
	{
		if ((GetKeyboardPress(DIK_RIGHT)) || (IsButtonPressed(0, BUTTON_R_RIGHT)))
		{// 視点旋回「左」
			g_Camera.rot.y += VALUE_ROTATE_CAMERA;
			if (g_Camera.rot.y > D3DX_PI)
			{
				g_Camera.rot.y -= D3DX_PI * 2.0f;
			}
		}

		if ((GetKeyboardPress(DIK_LEFT)) || (IsButtonPressed(0, BUTTON_R_LEFT)))
		{// 視点旋回「右」
			g_Camera.rot.y -= VALUE_ROTATE_CAMERA;
			if (g_Camera.rot.y < -D3DX_PI)
			{
				g_Camera.rot.y += D3DX_PI * 2.0f;
			}
		}

		if ((GetKeyboardPress(DIK_UP)) || (IsButtonPressed(0, BUTTON_R_UP)))
		{// 視点移動「上」
			if (g_Camera.rot.x < D3DXToRadian(50))
			{
				g_Camera.rot.x += (VALUE_ROTATE_CAMERA*0.3);
			}
		}

		if ((GetKeyboardPress(DIK_DOWN)) || (IsButtonPressed(0, BUTTON_R_DOWN)))
		{// 視点移動「下」
			if (g_Camera.pos.y > 0)
			{
				g_Camera.rot.x -= (VALUE_ROTATE_CAMERA*0.3);
			}
		}
	}
	else
	{
		D3DXVECTOR3 lockat = GetEnemy()->pos - GetPlayer()->pos;
		D3DXVECTOR3 N = { 0,0,1 };
		float rotg;
		lockat.y = 0;
		lockat = Nomalise(lockat);
		float check = (Cross(N, lockat)).y;
		if (check < 0)
		{
			rotg = -acosf(Dot(N, lockat));
		}
		else
		{
			rotg = acosf(Dot(N, lockat));
		}
		rotg -= g_Camera.rot.y;
		g_Camera.rot.y += rotg*0.8;

	}

	// カメラを初期に戻す
	if (GetKeyboardPress(DIK_Z))
	{
		UninitCamera();
		InitCamera();
	}

	// カメラをプレイヤーに追従させる
	D3DXMATRIX buff;

	D3DXVECTOR3 PtoC = D3DXVECTOR3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);
	D3DXVECTOR3 PtoAt = D3DXVECTOR3(0.0f, TILT_Y_CAM, TILT_Z_CAM);

	//回転行列の初期化
	D3DXMatrixIdentity(&g_Camera.mtxRoll);

	//バッファの初期化
	D3DXMatrixIdentity(&buff);

	//回転移動
	D3DXMatrixRotationYawPitchRoll(&buff, g_Camera.rot.y, g_Camera.rot.x, g_Camera.rot.z);
	D3DXMatrixMultiply(&g_Camera.mtxRoll, &g_Camera.mtxRoll, &buff);

	//バッファの初期化
	D3DXMatrixIdentity(&buff);

	//Playerに追従
	D3DXMatrixTranslation(&buff, GetPlayer()->pos.x, GetPlayer()->pos.y, GetPlayer()->pos.z);
	D3DXMatrixMultiply(&g_Camera.mtxRoll, &g_Camera.mtxRoll, &buff);

	D3DXVec3TransformCoord(&g_Camera.at, &PtoAt, &g_Camera.mtxRoll);
	D3DXVec3TransformCoord(&g_Camera.pos, &PtoC, &g_Camera.mtxRoll);

	PrintDebugProc("*** 視点操作 ***\n");
	PrintDebugProc("上移動 : ↑\n");
	PrintDebugProc("下移動 : ↓\n");
	PrintDebugProc("左旋回 : ←\n");
	PrintDebugProc("右旋回 : →\n");
	PrintDebugProc("\n");
}


//=============================================================================
// カメラの更新
//=============================================================================
void SetCamera(void) 
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_Camera.mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_Camera.mtxView,
		&g_Camera.pos,		// カメラの視点(位置)
		&g_Camera.at,		// カメラの注視点
		&g_Camera.up);		// カメラの上方向ベクトル

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_Camera.mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_Camera.mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&g_Camera.mtxProjection,
		VIEW_ANGLE,			// ビュー平面の視野角
		VIEW_ASPECT,		// ビュー平面のアスペクト比
		VIEW_NEAR_Z,		// ビュー平面のNearZ値
		VIEW_FAR_Z);		// ビュー平面のFarZ値

	// プロジェクションマトリックスの設定(透視変換の設定)
	pDevice->SetTransform(D3DTS_PROJECTION, &g_Camera.mtxProjection);
}


//=============================================================================
// カメラの取得
//=============================================================================
CAMERA *GetCamera(void) 
{
	return &g_Camera;
}

//=============================================================================
// ビューマトリックスの取得
//=============================================================================
D3DXMATRIX GetMtxView(void)
{
	return g_Camera.mtxView;
}


//=============================================================================
//
// ライト処理 [light.cpp]
// Author : 濱崎 小春
//
//=============================================================================
#include "main.h"
#include "debugproc.h"
#include "light.h"
#include "camera.h"
#include "enemy.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_LIGHT		(3)						// ライトの数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
D3DLIGHT9				g_Light[NUM_LIGHT];		// ライト情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecPos;
	D3DXVECTOR3 vecDir;

	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&g_Light[0], sizeof(D3DLIGHT9));

	// ライトのタイプの設定
	g_Light[0].Type = D3DLIGHT_DIRECTIONAL;

	// 拡散光
	g_Light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ライト0の鏡面反射光の設定
	g_Light[0].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 環境光
	g_Light[0].Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(0.0f, -5.0f, 10.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_Light[0].Direction, &vecDir);

	// ライトをレンダリングパイプラインに設定
	pDevice->SetLight(0, &g_Light[0]);

	// ライトの設定
	pDevice->LightEnable(0, TRUE);

	//Light[1]
	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&g_Light[1], sizeof(D3DLIGHT9));

	// ライトのタイプの設定
	g_Light[1].Type = D3DLIGHT_DIRECTIONAL;

	// 拡散光
	g_Light[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 環境光
	g_Light[1].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(-0.20f, -1.0f, -0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_Light[1].Direction, &vecDir);

	// ライトをレンダリングパイプラインに設定
	pDevice->SetLight(1, &g_Light[1]);

	// ライトの設定
	pDevice->LightEnable(1, TRUE);

	/* スポットライト（flashlight）設定 */
	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&g_Light[2], sizeof(D3DLIGHT9));

	// ライト2のタイプの設定
	g_Light[2].Type = D3DLIGHT_SPOT;

	// ライト2の拡散光の設定
	g_Light[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ライト2の鏡面反射光の設定
	g_Light[2].Specular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

	// ライト2の環境光の設定
	g_Light[2].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	// ライト2の位置の設定
	vecPos = D3DXVECTOR3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM - 100.0f);
	g_Light[2].Position = vecPos;

	// ライト2の方向の設定
	vecDir = D3DXVECTOR3(0.0f, 0.0f, 3.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_Light[2].Direction, &vecDir);

	// ライト2の光源の有効距離
	g_Light[2].Range = 3000.0f;

	// ライト2の内側→外側の光の減衰度
	g_Light[2].Falloff = 1.5f;

	// ライト2のライトからの距離に対する減衰の変化
	g_Light[2].Attenuation0 = 1.0f;
	g_Light[2].Attenuation1 = 0.0f;
	g_Light[2].Attenuation2 = 0.0f;

	// ライト2の内側の光の広がり設定
	g_Light[2].Theta = D3DXToRadian(10.0f);

	// ライト2の外側の光の広がり設定
	g_Light[2].Phi = D3DXToRadian(30.0f);

	// ライト2をレンダリングパイプラインに設定
	pDevice->SetLight(2, &g_Light[2]);

	// ライト2を使用使用状態に
	pDevice->LightEnable(2, TRUE);

	// ライティングモードをON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// 環境光設定（世の中を少し白くする）
	pDevice->SetRenderState(D3DRS_AMBIENT, 0xff030303);

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateLight(void)
{
	CAMERA *cam = GetCamera();							//カメラ構造体の取得
	ENEMY *enemy = GetEnemy();							//カメラ構造体の取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイス情報の取得

	//カメラの始点に追従させる
	g_Light[2].Position.x = cam->pos.x;
	g_Light[2].Position.y = cam->pos.y;
	g_Light[2].Position.z = cam->pos.z;

	//カメラの注始点に追従させる
	D3DXVECTOR3 cam_angle = cam->at - cam->pos;								//カメラの注視点のベクトルをだす
	D3DXVec3Normalize((D3DXVECTOR3*)&g_Light[2].Direction, &cam_angle);		//カメラ注視点のベクトルを正規化し、ライト注視点にする

	// ライト2をレンダリングパイプラインに設定
	pDevice->SetLight(2, &g_Light[2]);
}

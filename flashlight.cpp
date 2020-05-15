//=============================================================================
//
// フラッシュライト処理 [flashlight.cpp]
// Author : GP11B292 29 濱崎 小春
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "flashlight.h"
#include "camera.h"
#include "debugproc.h"
#include "collision.h"
#include "player.h"
#include "enemy.h"
#include "flashlight.h"
#include "shadow.h"
#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_FLASHLIGHT		"data/MODEL/flashlight.x"	// 読み込むモデル名

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static FLASHLIGHT		g_FlashLight;						// フラッシュライト構造体

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitFlashLight(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();				//デバイス情報の取得

	// 位置・回転・スケールの初期設定
	g_FlashLight.pos = D3DXVECTOR3(POS_X_F_LIGHT, POS_Y_F_LIGHT, POS_Z_F_LIGHT);		//プレイヤー後ろから当てている設定なので少し手前から描画
	g_FlashLight.rot = D3DXVECTOR3(ROT_X_F_LIGHT, ROT_Y_F_LIGHT, ROT_Z_F_LIGHT);		//やや斜め下
	g_FlashLight.scl = D3DXVECTOR3(SCL_F_LIGHT,   SCL_F_LIGHT,   SCL_F_LIGHT);			//縮小している
	g_FlashLight.hitpos = D3DXVECTOR3(POS_X_F_LIGHT, POS_Y_F_LIGHT, POS_Z_F_LIGHT);							//衝突判定用フラッシュライトat

	g_FlashLight.spd = 0.0f;								// 移動スピードクリア

	if (type == 0)
	{
		g_FlashLight.pD3DTexture = NULL;
		g_FlashLight.pD3DXMesh = NULL;
		g_FlashLight.pD3DXBuffMat = NULL;
		g_FlashLight.nNumMat = 0;
		g_FlashLight.parent = GetCamera();						//フラッシュライトをカメラの子オブジェクトにする

		// Xファイルの読み込み(関数）
		if (FAILED(D3DXLoadMeshFromX(MODEL_FLASHLIGHT,			// 読み込むモデルファイル名(Xファイル)
			D3DXMESH_SYSTEMMEM,									// メッシュの作成オプションを指定
			pDevice,											// IDirect3DDevice9インターフェイスへのポインタ
			NULL,												// 隣接性データを含むバッファへのポインタ
			&g_FlashLight.pD3DXBuffMat,							// マテリアルデータを含むバッファへのポインタ
			NULL,												// エフェクトインスタンスの配列を含むバッファへのポインタ
			&g_FlashLight.nNumMat,								// D3DXMATERIAL構造体の数
			&g_FlashLight.pD3DXMesh)))							// ID3DXMeshインターフェイスへのポインタのアドレス
		{
			return E_FAIL;
		}
	}

#if 0	//if 0はコメントと同義
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_FILENAME,									// ファイルの名前
		&g_FlashLight.pD3DTexture);							// 読み込むメモリー
#endif

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitFlashLight(void)
{
	if (g_FlashLight.pD3DTexture != NULL)
	{// テクスチャの開放
		g_FlashLight.pD3DTexture->Release();
		g_FlashLight.pD3DTexture = NULL;
	}

	if (g_FlashLight.pD3DXMesh != NULL)
	{// メッシュの開放
		g_FlashLight.pD3DXMesh->Release();
		g_FlashLight.pD3DXMesh = NULL;
	}

	if (g_FlashLight.pD3DXBuffMat != NULL)
	{// マテリアルの開放
		g_FlashLight.pD3DXBuffMat->Release();
		g_FlashLight.pD3DXBuffMat = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateFlashLight(void)
{
	// 移動処理
	if (GetKeyboardPress(DIK_NUMPAD8))
	{
		g_FlashLight.rot.x-= 0.02;
	}

	if (GetKeyboardPress(DIK_NUMPAD2))
	{
		g_FlashLight.rot.x+= 0.02;
	}

	if (GetKeyboardPress(DIK_NUMPAD6))
	{
		g_FlashLight.rot.y+= 0.02;
	}

	if (GetKeyboardPress(DIK_NUMPAD4))
	{
		g_FlashLight.rot.y-= 0.02;
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawFlashLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	// ライティングを無効に
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 加算合成に設定(円錐が描画される位置を白っぽく透過する）
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);					// αブレンドの合成方法の設定
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);				// αソースカラーの指定(描画元のブレンドファクターを設定)
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);					// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);						// Zバッファーの書き込みをしない

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_FlashLight.mtxWorld);															

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, g_FlashLight.scl.x, g_FlashLight.scl.y, g_FlashLight.scl.z);
	D3DXMatrixMultiply(&g_FlashLight.mtxWorld, &g_FlashLight.mtxWorld, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_FlashLight.rot.y, g_FlashLight.rot.x, g_FlashLight.rot.z);
	D3DXMatrixMultiply(&g_FlashLight.mtxWorld, &g_FlashLight.mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, g_FlashLight.pos.x , g_FlashLight.pos.y, g_FlashLight.pos.z);
	D3DXMatrixMultiply(&g_FlashLight.mtxWorld, &g_FlashLight.mtxWorld, &mtxTranslate);
	
	//カメラのワールドマトリックスをフラッシュライトと乗算
	D3DXMatrixMultiply(&g_FlashLight.mtxWorld, &g_FlashLight.mtxWorld, &g_FlashLight.parent->mtxRoll);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_FlashLight.mtxWorld);

	// 現在のマテリアルを取得(バックアップ)
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)g_FlashLight.pD3DXBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_FlashLight.nNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_FlashLight.pD3DTexture);

		// 描画(3Dモデルの描画)↓関数
		g_FlashLight.pD3DXMesh->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);

	// ライティングを有効に
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// 通常ブレンドに戻す
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				// Zバッファーの書き込みをする

}

//=============================================================================
// 衝突判定用ライト
//=============================================================================
void FlashLightHitPos(void)
{
	CAMERA *cam = GetCamera();

	D3DXMATRIX buff;									//行列格納用
	D3DXVECTOR3 CtoF = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//カメラとフラッシュライトの相対位置を初期化
	g_FlashLight.hitpos = D3DXVECTOR3(GetFLashLight()->rot.x, GetFLashLight()->rot.y, GetFLashLight()->rot.z);		//衝突判定用フラッシュライトat

	//回転行列の初期化
	D3DXMatrixIdentity(&g_FlashLight.mtxRoll);

	//バッファの初期化
	D3DXMatrixIdentity(&buff);

	//回転移動（カメラを原点にしてライトを回転させる）=カメラとライトの相対位置をだす
	D3DXMatrixRotationYawPitchRoll(&buff, g_FlashLight.rot.y, g_FlashLight.rot.x, g_FlashLight.rot.z);
	D3DXMatrixMultiply(&g_FlashLight.mtxRoll, &g_FlashLight.mtxRoll, &buff);

	//バッファの初期化
	D3DXMatrixIdentity(&buff);

	//カメラに追従(ワールド原点に対してカメラに合わせてライトを移動させる）
	D3DXMatrixTranslation(&buff, GetCamera()->pos.x, GetCamera()->pos.y, GetCamera()->pos.z);
	D3DXMatrixMultiply(&g_FlashLight.mtxRoll, &g_FlashLight.mtxRoll, &buff);

	//頂点変換(変換後の頂点の格納先、変換元の頂点、変換行列)
	D3DXVec3TransformCoord(&g_FlashLight.hitpos, &CtoF, &g_FlashLight.mtxRoll);
}

//=============================================================================
// フラッシュライトの取得
//=============================================================================
FLASHLIGHT *GetFLashLight(void)
{
	return &g_FlashLight;
}

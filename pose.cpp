//=============================================================================
//
// ポーズ画面処理 [pose.cpp]
// Author : 濱崎小春
//
//=============================================================================
#include "main.h"
#include "title.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "pose.h"
#include "game.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexPose(void);
//void SetVertexLogo(void);
//void SetVertexLogoColor(void);
void SetPoseMenuDiffuse(int menu);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTexturePosebg = NULL;		// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTexturePosetitle = NULL;	// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureReturn = NULL;	// テクスチャへのポインタ

VERTEX_2D				g_vertexWkPosebg[NUM_VERTEX];					// 頂点情報格納ワークVERTEX_2D				g_vertexWkTitleLogo[NUM_VERTEX];				// 頂点情報格納ワーク
VERTEX_2D				g_vertexWkRetry[NUM_VERTEX];					// 頂点情報格納ワーク
VERTEX_2D				g_vertexWkReturn[NUM_VERTEX];					// 頂点情報格納ワーク

static D3DXCOLOR		g_color;

static int				g_posemenu; 						//メニュー用変数
static int				scene;
static bool				g_pose; 						//ポーズ画面用の変数

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPose(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		/*テクスチャの読み込み*/
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_POSE_BG,									// ファイルの名前
			&g_pD3DTexturePosebg);								// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_POSE_TITLE,									// ファイルの名前
			&g_pD3DTexturePosetitle);							// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_POSE_RETURN,								// ファイルの名前
			&g_pD3DTextureReturn);								// 読み込むメモリー
	}

	g_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_posemenu = 0;												// ポーズメニュー初期化
	g_pose = false;												// 最初はポーズ画面OFF

	// 頂点情報の作成
	MakeVertexPose();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPose(void)
{
	if (g_pD3DTexturePosebg != NULL)
	{// テクスチャの開放
		g_pD3DTexturePosebg->Release();
		g_pD3DTexturePosebg = NULL;
	}

	if (g_pD3DTexturePosetitle != NULL)
	{// テクスチャの開放
		g_pD3DTexturePosetitle->Release();
		g_pD3DTexturePosetitle = NULL;
	}

	if (g_pD3DTextureReturn != NULL)
	{// テクスチャの開放
		g_pD3DTextureReturn->Release();
		g_pD3DTextureReturn = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePose(void)
{
	// ポーズ
	if (GetKeyboardTrigger(DIK_Z) || ((IsButtonTriggered(0, BUTTON_R) && (IsButtonTriggered(0, BUTTON_L)))))
	{
		g_pose = true;
	}

	if (g_pose == true)
	{
	   //menu選択
		if (GetKeyboardRepeat(DIK_UP) || GetKeyboardRepeat(DIK_DOWN) ||
			(IsButtonTriggered(0, BUTTON_UP)) || (IsButtonTriggered(0, BUTTON_DOWN)))
		{
			g_posemenu = (g_posemenu + 1) % MENU_MAX;
		}

		switch (g_posemenu)
		{
		case 0:
			if (GetKeyboardTrigger(DIK_RETURN))
			{// Enter押したら、ステージを切り替える
				SetMode(MODE_TITLE);
				UninitGame();
			}
			// ゲームパッドで移動処理
			else if (IsButtonTriggered(0, BUTTON_X))
			{
				SetMode(MODE_TITLE);
				UninitGame();
			}
			else if (IsButtonTriggered(0, BUTTON_B))
			{
				SetMode(MODE_TITLE);
				UninitGame();
			}
			break;
		case 1:		//キャンセル
			if (GetKeyboardTrigger(DIK_RETURN) || (IsButtonTriggered(0, BUTTON_X)) || IsButtonTriggered(0, BUTTON_B))
			{
				g_pose = false;
			}
			break;
		}

		SetPoseMenuDiffuse(g_posemenu);
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPose(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTexturePosebg);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkPosebg, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTexturePosetitle);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkRetry, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureReturn);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkReturn, sizeof(VERTEX_2D));

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexPose(void)
{
	// 頂点座標の設定
	g_vertexWkPosebg[0].vtx = D3DXVECTOR3(TITLE01_POS_X, TITLE01_POS_Y, 0.0f);
	g_vertexWkPosebg[1].vtx = D3DXVECTOR3(TITLE01_POS_X + TITLE01_SIZE_X, TITLE01_POS_Y, 0.0f);
	g_vertexWkPosebg[2].vtx = D3DXVECTOR3(TITLE01_POS_X, TITLE01_POS_Y + TITLE01_SIZE_Y, 0.0f);
	g_vertexWkPosebg[3].vtx = D3DXVECTOR3(TITLE01_POS_X + TITLE01_SIZE_X, TITLE01_POS_Y + TITLE01_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkPosebg[0].rhw =
	g_vertexWkPosebg[1].rhw =
	g_vertexWkPosebg[2].rhw =
	g_vertexWkPosebg[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkPosebg[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 150);
	g_vertexWkPosebg[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 150);
	g_vertexWkPosebg[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 150);
	g_vertexWkPosebg[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 150);

	// テクスチャ座標の設定
	g_vertexWkPosebg[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkPosebg[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkPosebg[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkPosebg[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点座標の設定
	g_vertexWkRetry[0].vtx = D3DXVECTOR3(RETRY_POS_X, RETRY_POS_Y, 0.0f);
	g_vertexWkRetry[1].vtx = D3DXVECTOR3(RETRY_POS_X + RETRYMENU_SIZE_X, RETRY_POS_Y, 0.0f);
	g_vertexWkRetry[2].vtx = D3DXVECTOR3(RETRY_POS_X, RETRY_POS_Y + RETRYMENU_SIZE_Y, 0.0f);
	g_vertexWkRetry[3].vtx = D3DXVECTOR3(RETRY_POS_X + RETRYMENU_SIZE_X, RETRY_POS_Y + RETRYMENU_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkRetry[0].rhw =
	g_vertexWkRetry[1].rhw =
	g_vertexWkRetry[2].rhw =
	g_vertexWkRetry[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkRetry[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkRetry[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkRetry[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkRetry[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkRetry[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkRetry[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkRetry[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkRetry[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点座標の設定
	g_vertexWkReturn[0].vtx = D3DXVECTOR3(RETURN_POS_X, RETURN_POS_Y, 0.0f);
	g_vertexWkReturn[1].vtx = D3DXVECTOR3(RETURN_POS_X + RETRYMENU_SIZE_X, RETURN_POS_Y, 0.0f);
	g_vertexWkReturn[2].vtx = D3DXVECTOR3(RETURN_POS_X, RETURN_POS_Y + RETRYMENU_SIZE_Y, 0.0f);
	g_vertexWkReturn[3].vtx = D3DXVECTOR3(RETURN_POS_X + RETRYMENU_SIZE_X, RETURN_POS_Y + RETRYMENU_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkReturn[0].rhw =
	g_vertexWkReturn[1].rhw =
	g_vertexWkReturn[2].rhw =
	g_vertexWkReturn[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkReturn[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkReturn[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkReturn[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkReturn[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkReturn[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkReturn[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkReturn[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkReturn[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// メニュー色の設定
//=============================================================================
void SetPoseMenuDiffuse(int menu)
{
	switch (menu)
	{
	case 0:
		g_vertexWkRetry[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkRetry[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkRetry[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkRetry[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		g_vertexWkReturn[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkReturn[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkReturn[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkReturn[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		break;
	case 1:
		g_vertexWkRetry[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkRetry[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkRetry[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkRetry[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);

		g_vertexWkReturn[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkReturn[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkReturn[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkReturn[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		break;
	}
}

//=============================================================================
// ポーズの取得
//=============================================================================
bool GetPose(void)
{
	return g_pose;
}

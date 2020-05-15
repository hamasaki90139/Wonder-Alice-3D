//=============================================================================
//
// チュートリアル処理 [tutorial.cpp]
// Author : 濱崎 小春
//
//=============================================================================
#include "main.h"
#include "tutorial.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTutorial(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureTutorial01 = NULL;				// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureTutorial02 = NULL;				// テクスチャへのポインタ

VERTEX_2D				g_vertexWkTutorial01[NUM_VERTEX];			// 頂点情報格納ワーク
VERTEX_2D				g_vertexWkTutorial02[NUM_VERTEX];			// 頂点情報格納ワーク

TUTORIAL				g_tutorial;									// チュートリアルページ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTutorial(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		/*テクスチャの読み込み*/
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			TEXTURE_TUTORIAL01,						// ファイルの名前
			&g_pD3DTextureTutorial01);				// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
			TEXTURE_TUTORIAL02,						// ファイルの名前
			&g_pD3DTextureTutorial02);				// 読み込むメモリー
	}

	// 頂点情報の作成
	MakeVertexTutorial();

	// チュートリアルは操作説明から
	g_tutorial = SOUSA;

	// BGM再生
	PlaySound(SOUND_LABEL_TUTORIAL);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTutorial(void)
{
	if (g_pD3DTextureTutorial01 != NULL)
	{// テクスチャの開放
		g_pD3DTextureTutorial01->Release();
		g_pD3DTextureTutorial01 = NULL;
	}
	if (g_pD3DTextureTutorial02 != NULL)
	{// テクスチャの開放
		g_pD3DTextureTutorial02->Release();
		g_pD3DTextureTutorial02 = NULL;
	}

	// BGM停止
	StopSound(SOUND_LABEL_TUTORIAL);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTutorial(void)
{
	// 操作説明
	if (g_tutorial == SOUSA)
	{
		if (GetKeyboardTrigger(DIK_0)||(IsButtonTriggered(0, BUTTON_C)))
		{   // ページを切り替える
			g_tutorial = GAMEN;
			// SE再生
			PlaySound(SOUND_LABEL_SE_OK);
		}
	}
	// 画面説明
	else if (g_tutorial == GAMEN)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || (IsButtonTriggered(0, BUTTON_C)))
		{   // ゲーム開始
			SetFade(FADE_OUT);
			// SE再生
			PlaySound(SOUND_LABEL_SE_OK);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTutorial(void)
{
	if (g_tutorial == SOUSA)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureTutorial01);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTutorial01, sizeof(VERTEX_2D));
	}
	else/* if (g_tutorial == GAMEN)*/
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureTutorial02);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTutorial02, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTutorial(void)
{
	// 操作説明
	// 頂点座標の設定
	g_vertexWkTutorial01[0].vtx = D3DXVECTOR3(TUTORIAL_POS_X, TUTORIAL_POS_Y, 0.0f);
	g_vertexWkTutorial01[1].vtx = D3DXVECTOR3(TUTORIAL_POS_X + TUTORIAL_SIZE_X, TUTORIAL_POS_Y, 0.0f);
	g_vertexWkTutorial01[2].vtx = D3DXVECTOR3(TUTORIAL_POS_X, TUTORIAL_POS_Y + TUTORIAL_SIZE_Y, 0.0f);
	g_vertexWkTutorial01[3].vtx = D3DXVECTOR3(TUTORIAL_POS_X + TUTORIAL_SIZE_X, TUTORIAL_POS_Y + TUTORIAL_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkTutorial01[0].rhw =
		g_vertexWkTutorial01[1].rhw =
		g_vertexWkTutorial01[2].rhw =
		g_vertexWkTutorial01[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkTutorial01[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTutorial01[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTutorial01[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTutorial01[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkTutorial01[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTutorial01[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTutorial01[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTutorial01[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 画面説明
	// 頂点座標の設定
	g_vertexWkTutorial02[0].vtx = D3DXVECTOR3(TUTORIAL_POS_X, TUTORIAL_POS_Y, 0.0f);
	g_vertexWkTutorial02[1].vtx = D3DXVECTOR3(TUTORIAL_POS_X + TUTORIAL_SIZE_X, TUTORIAL_POS_Y, 0.0f);
	g_vertexWkTutorial02[2].vtx = D3DXVECTOR3(TUTORIAL_POS_X, TUTORIAL_POS_Y + TUTORIAL_SIZE_Y, 0.0f);
	g_vertexWkTutorial02[3].vtx = D3DXVECTOR3(TUTORIAL_POS_X + TUTORIAL_SIZE_X, TUTORIAL_POS_Y + TUTORIAL_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkTutorial02[0].rhw =
		g_vertexWkTutorial02[1].rhw =
		g_vertexWkTutorial02[2].rhw =
		g_vertexWkTutorial02[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkTutorial02[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTutorial02[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTutorial02[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTutorial02[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkTutorial02[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTutorial02[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTutorial02[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTutorial02[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// チュートリアルページの取得
//=============================================================================
TUTORIAL GetTutorial(void)
{
	return g_tutorial;
}

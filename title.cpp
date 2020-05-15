//=============================================================================
//
// タイトル画面処理 [title.cpp]
// Author : 濱崎 小春
//
//=============================================================================
#include "main.h"
#include "title.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "tutorial.h"
#include "meshsky.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTitle(void);
void SetVertexBG(void);
void SetMenu(int menu);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureTitle1 = NULL;						// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureTitle2 = NULL;						// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureTitleLogo = NULL;					// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureTitleNewgame = NULL;				// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureTitleExit = NULL;					// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureTitleSky[NUM_BG] = { NULL };		// テクスチャへのポインタ

VERTEX_2D				g_vertexWkTitle1[NUM_VERTEX];					// 頂点情報格納ワーク
VERTEX_2D				g_vertexWkTitle2[NUM_VERTEX];					// 頂点情報格納ワーク
VERTEX_2D				g_vertexWkTitleLogo[NUM_VERTEX];				// 頂点情報格納ワーク
VERTEX_2D				g_vertexWkTitleNewgame[NUM_VERTEX];				// 頂点情報格納ワーク
VERTEX_2D				g_vertexWkTitleExit[NUM_VERTEX];				// 頂点情報格納ワーク
VERTEX_2D				g_vertexWkTitleSky[NUM_BG][NUM_VERTEX];			// 頂点情報格納ワーク

static float			g_scale;										// メニューの拡大縮小量
static int				g_menu; 										// メニュー用変数

static D3DXVECTOR3		g_pos;											// BGの移動量
static float			g_interval[NUM_BG];								// 背景ループのインタバール

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		/*テクスチャの読み込み*/
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_TITLE01,									// ファイルの名前
			&g_pD3DTextureTitle1);								// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_TITLE02,									// ファイルの名前
			&g_pD3DTextureTitle2);								// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_TITLE_LOGO,									// ファイルの名前
			&g_pD3DTextureTitleLogo);							// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_TITLE_NEWGAME,								// ファイルの名前
			&g_pD3DTextureTitleNewgame);						// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_TITLE_EXIT,									// ファイルの名前
			&g_pD3DTextureTitleExit);							// 読み込むメモリー

		for (int i = 0; i < NUM_BG; i++)
		{
			D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
				TEXTURE_SKY,									// ファイルの名前
				&g_pD3DTextureTitleSky[i]);						// 読み込むメモリー
		}
	}

	// 背景ループのためにインタ―バルをもたせる
	for (int i = 0; i < NUM_BG; i++)
	{
		g_interval[i] = i * TITLE_SKY_SIZE_X;
	}

	g_menu = 0;										// メニュー初期化
	g_scale = 0;									// メニュー拡大縮小量の初期化
	g_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// BG移動量の初期化

	// 頂点情報の作成
	MakeVertexTitle();

	// BGM再生
	PlaySound(SOUND_LABEL_TITLE);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
{
	if (g_pD3DTextureTitle1 != NULL)
	{// テクスチャの開放
		g_pD3DTextureTitle1->Release();
		g_pD3DTextureTitle1 = NULL;
	}

	if (g_pD3DTextureTitle2 != NULL)
	{// テクスチャの開放
		g_pD3DTextureTitle2->Release();
		g_pD3DTextureTitle2 = NULL;
	}

	if (g_pD3DTextureTitleLogo != NULL)
	{// テクスチャの開放
		g_pD3DTextureTitleLogo->Release();
		g_pD3DTextureTitleLogo = NULL;
	}

	if (g_pD3DTextureTitleNewgame != NULL)
	{// テクスチャの開放
		g_pD3DTextureTitleNewgame->Release();
		g_pD3DTextureTitleNewgame = NULL;
	}

	if (g_pD3DTextureTitleExit != NULL)
	{// テクスチャの開放
		g_pD3DTextureTitleExit->Release();
		g_pD3DTextureTitleExit = NULL;
	}

	for (int i = 0; i < NUM_BG; i++)
	{
		if (g_pD3DTextureTitleSky[i] != NULL)
		{// テクスチャの開放
			g_pD3DTextureTitleSky[i]->Release();
			g_pD3DTextureTitleSky[i] = NULL;
		}
	}

	// BGM停止
	StopSound(SOUND_LABEL_TITLE);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitle(void)
{
	if (GetKeyboardRepeat(DIK_UP) || GetKeyboardRepeat(DIK_DOWN) ||
		(IsButtonTriggered(0, BUTTON_UP)) || (IsButtonTriggered(0, BUTTON_DOWN)))
	{
		// メニュー選択の切り替え
		g_menu = (g_menu + 1) % MENU_MAX;

		// SE再生
		PlaySound(SOUND_LABEL_SE_MENU);
	}

	switch (g_menu)
	{
	case NEWGAME:
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			// SE再生
			PlaySound(SOUND_LABEL_SE_OK);
			// Enter押したら、ステージを切り替える
			SetFade(FADE_OUT);
		}
		// ゲームパッドで移動処理
		else if (IsButtonTriggered(0, BUTTON_C))
		{
			// SE再生
			PlaySound(SOUND_LABEL_SE_OK);
			SetFade(FADE_OUT);
		}
		break;
	case EXIT:
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			// SE再生
			PlaySound(SOUND_LABEL_SE_OK);
			exit(-1);		//ゲーム終了
		}
		// ゲームパッドで移動処理
		else if (IsButtonTriggered(0, BUTTON_C))
		{
			// SE再生
			PlaySound(SOUND_LABEL_SE_OK);
			exit(-1);		//ゲーム終了
		}
		break;
	}

	// スクロール処理
	for (int i = 0; i < NUM_BG; i++)
	{
		// 毎フレーム左にスクロールさせる
		g_pos.x -= 1.0f;

		//ループ処理
		if (g_interval[i] + TITLE_SKY_SIZE_X + g_pos.x < 0)
		{
			g_interval[i] += NUM_BG * TITLE_SKY_SIZE_X;
		}

		// 移動後の座標で頂点を設定
		SetVertexBG();
	}

	// メニュー頂点座標の更新
	SetMenu(g_menu);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < NUM_BG; i++)
	{
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureTitleSky[i]);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitleSky[i], sizeof(VERTEX_2D));
	}

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureTitle1);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitle1, sizeof(VERTEX_2D));


	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureTitle2);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitle2, sizeof(VERTEX_2D));


	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureTitleLogo);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitleLogo, sizeof(VERTEX_2D));


	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureTitleNewgame);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitleNewgame, sizeof(VERTEX_2D));


	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureTitleExit);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitleExit, sizeof(VERTEX_2D));

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTitle(void)
{
	// 頂点座標の設定
	g_vertexWkTitle1[0].vtx = D3DXVECTOR3(TITLE01_POS_X, TITLE01_POS_Y, 0.0f);
	g_vertexWkTitle1[1].vtx = D3DXVECTOR3(TITLE01_POS_X + TITLE01_SIZE_X, TITLE01_POS_Y, 0.0f);
	g_vertexWkTitle1[2].vtx = D3DXVECTOR3(TITLE01_POS_X, TITLE01_POS_Y + TITLE01_SIZE_Y, 0.0f);
	g_vertexWkTitle1[3].vtx = D3DXVECTOR3(TITLE01_POS_X + TITLE01_SIZE_X, TITLE01_POS_Y + TITLE01_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkTitle1[0].rhw =
		g_vertexWkTitle1[1].rhw =
		g_vertexWkTitle1[2].rhw =
		g_vertexWkTitle1[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkTitle1[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitle1[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitle1[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitle1[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkTitle1[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitle1[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitle1[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitle1[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// 頂点座標の設定
	g_vertexWkTitle2[0].vtx = D3DXVECTOR3(TITLE02_POS_X, TITLE02_POS_Y, 0.0f);
	g_vertexWkTitle2[1].vtx = D3DXVECTOR3(TITLE02_POS_X + TITLE02_SIZE_X, TITLE02_POS_Y, 0.0f);
	g_vertexWkTitle2[2].vtx = D3DXVECTOR3(TITLE02_POS_X, TITLE02_POS_Y + TITLE02_SIZE_Y, 0.0f);
	g_vertexWkTitle2[3].vtx = D3DXVECTOR3(TITLE02_POS_X + TITLE02_SIZE_X, TITLE02_POS_Y + TITLE02_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkTitle2[0].rhw =
		g_vertexWkTitle2[1].rhw =
		g_vertexWkTitle2[2].rhw =
		g_vertexWkTitle2[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkTitle2[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitle2[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitle2[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitle2[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkTitle2[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitle2[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitle2[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitle2[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// 頂点座標の設定
	g_vertexWkTitleLogo[0].vtx = D3DXVECTOR3(TITLELOGO_POS_X, TITLELOGO_POS_Y, 0.0f);
	g_vertexWkTitleLogo[1].vtx = D3DXVECTOR3(TITLELOGO_POS_X + TITLELOGO_SIZE_X, TITLELOGO_POS_Y, 0.0f);
	g_vertexWkTitleLogo[2].vtx = D3DXVECTOR3(TITLELOGO_POS_X, TITLELOGO_POS_Y + TITLELOGO_SIZE_Y, 0.0f);
	g_vertexWkTitleLogo[3].vtx = D3DXVECTOR3(TITLELOGO_POS_X + TITLELOGO_SIZE_X, TITLELOGO_POS_Y + TITLELOGO_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkTitleLogo[0].rhw =
		g_vertexWkTitleLogo[1].rhw =
		g_vertexWkTitleLogo[2].rhw =
		g_vertexWkTitleLogo[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkTitleLogo[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitleLogo[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitleLogo[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitleLogo[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkTitleLogo[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitleLogo[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitleLogo[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitleLogo[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// メニュー頂点の作成
	SetMenu(g_menu);

	//SKY
	// 頂点座標の設定
	SetVertexBG();

	for (int i = 0; i < NUM_BG; i++)
	{
		// テクスチャのパースペクティブコレクト用
		g_vertexWkTitleSky[i][0].rhw =
			g_vertexWkTitleSky[i][1].rhw =
			g_vertexWkTitleSky[i][2].rhw =
			g_vertexWkTitleSky[i][3].rhw = 1.0f;

		// 反射光の設定
		g_vertexWkTitleSky[i][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTitleSky[i][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTitleSky[i][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTitleSky[i][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		g_vertexWkTitleSky[i][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		g_vertexWkTitleSky[i][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		g_vertexWkTitleSky[i][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		g_vertexWkTitleSky[i][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}

//=============================================================================
// 背景頂点座標の設定
//=============================================================================
void SetVertexBG(void)
{
	// 背景頂点座標の設定（ループ）
	for (int i = 0; i < NUM_BG; i++)
	{
		g_vertexWkTitleSky[i][0].vtx = D3DXVECTOR3(TITLE_SKY_POS_X + g_interval[i], TITLE_SKY_POS_Y, 0.0f) + g_pos;
		g_vertexWkTitleSky[i][1].vtx = D3DXVECTOR3(TITLE_SKY_POS_X + TITLE_SKY_SIZE_X + g_interval[i], TITLE_SKY_POS_Y, 0.0f) + g_pos;
		g_vertexWkTitleSky[i][2].vtx = D3DXVECTOR3(TITLE_SKY_POS_X + g_interval[i], TITLE_SKY_POS_Y + TITLE_SKY_SIZE_Y, 0.0f) + g_pos;
		g_vertexWkTitleSky[i][3].vtx = D3DXVECTOR3(TITLE_SKY_POS_X + TITLE_SKY_SIZE_X + g_interval[i], TITLE_SKY_POS_Y + TITLE_SKY_SIZE_Y, 0.0f) + g_pos;
	}
}

//=============================================================================
// メニュー色の設定
//=============================================================================
void SetMenu(int menu)
{
	switch (menu)
	{
	case NEWGAME:
		for (int cnt = 0; cnt < 60; cnt++)
		{
			g_scale += 0.001f;

			// 頂点座標の設定
			g_vertexWkTitleNewgame[0].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X - g_scale, TITLE_NEWGAME_POS_Y - g_scale, 0.0f);
			g_vertexWkTitleNewgame[1].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X + TITLE_MENU_SIZE_X + g_scale, TITLE_NEWGAME_POS_Y - g_scale, 0.0f);
			g_vertexWkTitleNewgame[2].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X - g_scale, TITLE_NEWGAME_POS_Y + TITLE_MENU_SIZE_Y + g_scale, 0.0f);
			g_vertexWkTitleNewgame[3].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X + TITLE_MENU_SIZE_X + g_scale, TITLE_NEWGAME_POS_Y + TITLE_MENU_SIZE_Y + g_scale, 0.0f);

			if (g_scale >= 2.0f)	// 拡大終了？
			{
				g_scale = 0.0f;
				g_vertexWkTitleNewgame[0].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X + g_scale, TITLE_NEWGAME_POS_Y + g_scale, 0.0f);
				g_vertexWkTitleNewgame[1].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X + TITLE_MENU_SIZE_X - g_scale, TITLE_NEWGAME_POS_Y + g_scale, 0.0f);
				g_vertexWkTitleNewgame[2].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X + g_scale, TITLE_NEWGAME_POS_Y + TITLE_MENU_SIZE_Y - g_scale, 0.0f);
				g_vertexWkTitleNewgame[3].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X + TITLE_MENU_SIZE_X - g_scale, TITLE_NEWGAME_POS_Y + TITLE_MENU_SIZE_Y - g_scale, 0.0f);
				break;
			}
		}
		// 頂点座標の設定
		g_vertexWkTitleExit[0].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X, TITLE_EXIT_POS_Y, 0.0f);
		g_vertexWkTitleExit[1].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X + TITLE_MENU_SIZE_X, TITLE_EXIT_POS_Y, 0.0f);
		g_vertexWkTitleExit[2].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X, TITLE_EXIT_POS_Y + TITLE_MENU_SIZE_Y, 0.0f);
		g_vertexWkTitleExit[3].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X + TITLE_MENU_SIZE_X, TITLE_EXIT_POS_Y + TITLE_MENU_SIZE_Y, 0.0f);

		// 反射光の設定
		g_vertexWkTitleNewgame[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTitleNewgame[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTitleNewgame[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTitleNewgame[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		g_vertexWkTitleExit[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkTitleExit[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkTitleExit[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkTitleExit[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		break;

	case EXIT:
		for (int cnt = 0; cnt < 60; cnt++)
		{
			g_scale += 0.001f;

			// 頂点座標の設定
			g_vertexWkTitleExit[0].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X - g_scale, TITLE_EXIT_POS_Y - g_scale, 0.0f);
			g_vertexWkTitleExit[1].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X + TITLE_MENU_SIZE_X + g_scale, TITLE_EXIT_POS_Y - g_scale, 0.0f);
			g_vertexWkTitleExit[2].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X - g_scale, TITLE_EXIT_POS_Y + TITLE_MENU_SIZE_Y + g_scale, 0.0f);
			g_vertexWkTitleExit[3].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X + TITLE_MENU_SIZE_X + g_scale, TITLE_EXIT_POS_Y + TITLE_MENU_SIZE_Y + g_scale, 0.0f);

			if (g_scale >= 2.0f)	// 拡大終了？
			{
				g_scale = 0.0f;
				g_vertexWkTitleExit[0].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X + g_scale, TITLE_EXIT_POS_Y + g_scale, 0.0f);
				g_vertexWkTitleExit[1].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X + TITLE_MENU_SIZE_X - g_scale, TITLE_EXIT_POS_Y + g_scale, 0.0f);
				g_vertexWkTitleExit[2].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X + g_scale, TITLE_EXIT_POS_Y + TITLE_MENU_SIZE_Y - g_scale, 0.0f);
				g_vertexWkTitleExit[3].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X + TITLE_MENU_SIZE_X - g_scale, TITLE_EXIT_POS_Y + TITLE_MENU_SIZE_Y - g_scale, 0.0f);
				break;
			}
		}
		g_vertexWkTitleNewgame[0].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X, TITLE_NEWGAME_POS_Y, 0.0f);
		g_vertexWkTitleNewgame[1].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X + TITLE_MENU_SIZE_X, TITLE_NEWGAME_POS_Y, 0.0f);
		g_vertexWkTitleNewgame[2].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X, TITLE_NEWGAME_POS_Y + TITLE_MENU_SIZE_Y, 0.0f);
		g_vertexWkTitleNewgame[3].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X + TITLE_MENU_SIZE_X, TITLE_NEWGAME_POS_Y + TITLE_MENU_SIZE_Y, 0.0f);

		// 反射光の設定
		g_vertexWkTitleNewgame[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkTitleNewgame[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkTitleNewgame[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkTitleNewgame[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);

		g_vertexWkTitleExit[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTitleExit[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTitleExit[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTitleExit[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		break;
	}
	// テクスチャのパースペクティブコレクト用
	g_vertexWkTitleNewgame[0].rhw =
		g_vertexWkTitleNewgame[1].rhw =
		g_vertexWkTitleNewgame[2].rhw =
		g_vertexWkTitleNewgame[3].rhw = 1.0f;

	// テクスチャ座標の設定
	g_vertexWkTitleNewgame[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitleNewgame[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitleNewgame[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitleNewgame[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//EXIT
	// テクスチャのパースペクティブコレクト用
	g_vertexWkTitleExit[0].rhw =
		g_vertexWkTitleExit[1].rhw =
		g_vertexWkTitleExit[2].rhw =
		g_vertexWkTitleExit[3].rhw = 1.0f;

	// テクスチャ座標の設定
	g_vertexWkTitleExit[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitleExit[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitleExit[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitleExit[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}



//=============================================================================
//
// リザルト画面処理 [result.cpp]
// Author : 濱崎 小春
//
//=============================================================================
#include "main.h"
#include "result.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "player.h"
#include "score.h"
#include "title.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexResult(void);
HRESULT MakeVertexResultScore(void);
void	SetTextureResultScore(void);
void SetResultMenuDiffuse(int menu);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureResult01 = NULL;						// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureResult02 = NULL;						// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureResult03 = NULL;						// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureResultGameclear = NULL;				// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureResultGameover = NULL;					// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureResultScore = NULL;					// テクスチャへのポインタ

LPDIRECT3DTEXTURE9		g_pD3DTextureResultmenu1 = NULL;					// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureResultmenu2 = NULL;					// テクスチャへのポインタ

VERTEX_2D				g_vertexWkResult01[NUM_VERTEX];						// 頂点情報格納ワーク
VERTEX_2D				g_vertexWkResult02[NUM_VERTEX];						// 頂点情報格納ワーク
VERTEX_2D				g_vertexWkResult03[NUM_VERTEX];						// 頂点情報格納ワーク
VERTEX_2D				g_vertexWkResultGameclear[NUM_VERTEX];				// 頂点情報格納ワーク
VERTEX_2D				g_vertexWkResultGameover[NUM_VERTEX];				// 頂点情報格納ワーク
VERTEX_2D				g_vertexWkResultScore[SCORE_DIGIT][NUM_VERTEX];		// 頂点情報格納ワーク
VERTEX_2D				g_vertexWkResultmenu1[NUM_VERTEX];					// 頂点情報格納ワーク
VERTEX_2D				g_vertexWkResultmenu2[NUM_VERTEX];					// 頂点情報格納ワーク

int						g_return_cnt;										//リザルト画面遷移用
static int				menu; 												//メニュー用変数


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitResult(int type)
{
	SCORE *score = GetScore();		// スコアのポインターを初期化

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_RESULT01,				// ファイルの名前
			&g_pD3DTextureResult01);		// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_RESULT02,				// ファイルの名前
			&g_pD3DTextureResult02);		// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_RESULT03,				// ファイルの名前
			&g_pD3DTextureResult03);		// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_RESULT_GAMECLEAR,		// ファイルの名前
			&g_pD3DTextureResultGameclear);	// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_RESULT_GAMEOVER,		// ファイルの名前
			&g_pD3DTextureResultGameover);	// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			SCORE_TEXTURE,		// ファイルの名前
			&g_pD3DTextureResultScore);	// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_TITLE_NEWGAME,		// ファイルの名前
			&g_pD3DTextureResultmenu1);	// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_TITLE_EXIT,		// ファイルの名前
			&g_pD3DTextureResultmenu2);	// 読み込むメモリー
	}

	score->posScore = D3DXVECTOR3((float)RESULTSCORE_POS_X, (float)RESULTSCORE_POS_Y, 0.0f);

	g_return_cnt = 0;		//リザルト画面遷移用変数の初期化
	menu = 0;				//メニュー用変数の初期化

	// 頂点情報の作成
	MakeVertexResult();
	MakeVertexResultScore();

	if (GetPlayer()->use == true)	//ゲームクリア
	{
		// BGM再生
		PlaySound(SOUND_LABEL_GAMECLEAR);
	}
	else
	{
		// BGM再生
		PlaySound(SOUND_LABEL_GAMEOVER);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
{
	if (g_pD3DTextureResult01 != NULL)
	{// テクスチャの開放
		g_pD3DTextureResult01->Release();
		g_pD3DTextureResult01 = NULL;
	}

	if (g_pD3DTextureResult02 != NULL)
	{// テクスチャの開放
		g_pD3DTextureResult02->Release();
		g_pD3DTextureResult02 = NULL;
	}

	if (g_pD3DTextureResult03 != NULL)
	{// テクスチャの開放
		g_pD3DTextureResult03->Release();
		g_pD3DTextureResult03 = NULL;
	}

	if (g_pD3DTextureResultGameclear != NULL)
	{// テクスチャの開放
		g_pD3DTextureResultGameclear->Release();
		g_pD3DTextureResultGameclear = NULL;
	}

	if (g_pD3DTextureResultGameover != NULL)
	{// テクスチャの開放
		g_pD3DTextureResultGameover->Release();
		g_pD3DTextureResultGameover = NULL;
	}

	if (g_pD3DTextureResultScore != NULL)
	{// テクスチャの開放
		g_pD3DTextureResultScore->Release();
		g_pD3DTextureResultScore = NULL;
	}

	if (g_pD3DTextureResultmenu1 != NULL)
	{// テクスチャの開放
		g_pD3DTextureResultmenu1->Release();
		g_pD3DTextureResultmenu1 = NULL;
	}

	if (g_pD3DTextureResultmenu2 != NULL)
	{// テクスチャの開放
		g_pD3DTextureResultmenu2->Release();
		g_pD3DTextureResultmenu2 = NULL;
	}

	if (GetPlayer()->use == true)	//ゲームクリア
	{
		// BGM停止
		StopSound(SOUND_LABEL_GAMECLEAR);
	}
	else
	{
		// BGM停止
		StopSound(SOUND_LABEL_GAMEOVER);
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{
	PLAYER *player = GetPlayer();			// プレイヤーのポインターを初期化

	if (player->use == true)	//ゲームクリア
	{
		SetTextureResultScore();	//リザルト用スコアのテクスチャ設定

		if (GetKeyboardTrigger(DIK_RETURN))
		{
			SetFade(FADE_OUT);
		}
		else if (IsButtonTriggered(0, BUTTON_C))
		{
			SetFade(FADE_OUT);
		}
	}
	else
	{
		if (GetKeyboardTrigger(DIK_RETURN))
		{// Enter押したら、ステージを切り替える
			SetFade(FADE_OUT);
		}
		else if (IsButtonTriggered(0, BUTTON_C))
		{// Cボタンでステージを切り替える
			SetFade(FADE_OUT);
		}
	}
	SetResultMenuDiffuse(menu);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
{
	PLAYER *player = GetPlayer();			// プレイヤーのポインターを初期化

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureResult01);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkResult01, sizeof(VERTEX_2D));

	if (player->use == true)	//ゲームクリア
	{	//ゲームクリア文字
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureResultGameclear);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkResultGameclear, sizeof(VERTEX_2D));

		// クイーン
		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureResult02);
		{
			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, &g_vertexWkResult02, sizeof(VERTEX_2D));
		}

		//スコア
		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureResultScore);

		for (int i = 0; i < SCORE_DIGIT; i++)
		{
			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, &g_vertexWkResultScore[i][0], sizeof(VERTEX_2D));
		}
	}
	if (player->use == false)	//ゲームオーバー
	{	//ゲームオーバー文字
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureResultGameover);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkResultGameover, sizeof(VERTEX_2D));

		// アリス
		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureResult03);
		{
			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, &g_vertexWkResult03, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexResult(void)
{
	// 頂点座標の設定
	g_vertexWkResult01[0].vtx = D3DXVECTOR3(RESULT01_POS_X, RESULT01_POS_Y, 0.0f);
	g_vertexWkResult01[1].vtx = D3DXVECTOR3(RESULT01_POS_X + RESULT01_SIZE_X, RESULT01_POS_Y, 0.0f);
	g_vertexWkResult01[2].vtx = D3DXVECTOR3(RESULT01_POS_X, RESULT01_POS_Y + RESULT01_SIZE_Y, 0.0f);
	g_vertexWkResult01[3].vtx = D3DXVECTOR3(RESULT01_POS_X + RESULT01_SIZE_X, RESULT01_POS_Y + RESULT01_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkResult01[0].rhw =
		g_vertexWkResult01[1].rhw =
		g_vertexWkResult01[2].rhw =
		g_vertexWkResult01[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkResult01[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResult01[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResult01[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResult01[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkResult01[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkResult01[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkResult01[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkResult01[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点座標の設定
	g_vertexWkResult02[0].vtx = D3DXVECTOR3(RESULT02_POS_X, RESULT02_POS_Y, 0.0f);
	g_vertexWkResult02[1].vtx = D3DXVECTOR3(RESULT02_POS_X + RESULT02_SIZE_X, RESULT02_POS_Y, 0.0f);
	g_vertexWkResult02[2].vtx = D3DXVECTOR3(RESULT02_POS_X, RESULT02_POS_Y + RESULT02_SIZE_Y, 0.0f);
	g_vertexWkResult02[3].vtx = D3DXVECTOR3(RESULT02_POS_X + RESULT02_SIZE_X, RESULT02_POS_Y + RESULT02_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkResult02[0].rhw =
		g_vertexWkResult02[1].rhw =
		g_vertexWkResult02[2].rhw =
		g_vertexWkResult02[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkResult02[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResult02[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResult02[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResult02[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkResult02[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkResult02[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkResult02[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkResult02[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点座標の設定
	g_vertexWkResult03[0].vtx = D3DXVECTOR3(RESULT03_POS_X, RESULT03_POS_Y, 0.0f);
	g_vertexWkResult03[1].vtx = D3DXVECTOR3(RESULT03_POS_X + RESULT03_SIZE_X, RESULT03_POS_Y, 0.0f);
	g_vertexWkResult03[2].vtx = D3DXVECTOR3(RESULT03_POS_X, RESULT03_POS_Y + RESULT03_SIZE_Y, 0.0f);
	g_vertexWkResult03[3].vtx = D3DXVECTOR3(RESULT03_POS_X + RESULT03_SIZE_X, RESULT03_POS_Y + RESULT03_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkResult03[0].rhw =
		g_vertexWkResult03[1].rhw =
		g_vertexWkResult03[2].rhw =
		g_vertexWkResult03[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkResult03[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResult03[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResult03[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResult03[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkResult03[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkResult03[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkResult03[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkResult03[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点座標の設定
	g_vertexWkResultGameclear[0].vtx = D3DXVECTOR3(RESULTLOGO_POS_X, RESULTLOGO_POS_Y, 0.0f);
	g_vertexWkResultGameclear[1].vtx = D3DXVECTOR3(RESULTLOGO_POS_X + RESULTLOGO_SIZE_X, RESULTLOGO_POS_Y, 0.0f);
	g_vertexWkResultGameclear[2].vtx = D3DXVECTOR3(RESULTLOGO_POS_X, RESULTLOGO_POS_Y + RESULTLOGO_SIZE_Y, 0.0f);
	g_vertexWkResultGameclear[3].vtx = D3DXVECTOR3(RESULTLOGO_POS_X + RESULTLOGO_SIZE_X, RESULTLOGO_POS_Y + RESULTLOGO_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkResultGameclear[0].rhw =
		g_vertexWkResultGameclear[1].rhw =
		g_vertexWkResultGameclear[2].rhw =
		g_vertexWkResultGameclear[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkResultGameclear[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultGameclear[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultGameclear[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultGameclear[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkResultGameclear[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkResultGameclear[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkResultGameclear[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkResultGameclear[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点座標の設定
	g_vertexWkResultGameover[0].vtx = D3DXVECTOR3(RESULTLOGO_POS_X, RESULTLOGO_POS_Y, 0.0f);
	g_vertexWkResultGameover[1].vtx = D3DXVECTOR3(RESULTLOGO_POS_X + RESULTLOGO_SIZE_X, RESULTLOGO_POS_Y, 0.0f);
	g_vertexWkResultGameover[2].vtx = D3DXVECTOR3(RESULTLOGO_POS_X, RESULTLOGO_POS_Y + RESULTLOGO_SIZE_Y, 0.0f);
	g_vertexWkResultGameover[3].vtx = D3DXVECTOR3(RESULTLOGO_POS_X + RESULTLOGO_SIZE_X, RESULTLOGO_POS_Y + RESULTLOGO_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkResultGameover[0].rhw =
		g_vertexWkResultGameover[1].rhw =
		g_vertexWkResultGameover[2].rhw =
		g_vertexWkResultGameover[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkResultGameover[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultGameover[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultGameover[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultGameover[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkResultGameover[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkResultGameover[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkResultGameover[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkResultGameover[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点座標の設定
	g_vertexWkResultmenu1[0].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X, TITLE_NEWGAME_POS_Y, 0.0f);
	g_vertexWkResultmenu1[1].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X + TITLE_MENU_SIZE_X, TITLE_NEWGAME_POS_Y, 0.0f);
	g_vertexWkResultmenu1[2].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X, TITLE_NEWGAME_POS_Y + TITLE_MENU_SIZE_Y, 0.0f);
	g_vertexWkResultmenu1[3].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X + TITLE_MENU_SIZE_X, TITLE_NEWGAME_POS_Y + TITLE_MENU_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkResultmenu1[0].rhw =
		g_vertexWkResultmenu1[1].rhw =
		g_vertexWkResultmenu1[2].rhw =
		g_vertexWkResultmenu1[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkResultmenu1[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultmenu1[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultmenu1[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultmenu1[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkResultmenu1[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkResultmenu1[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkResultmenu1[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkResultmenu1[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点座標の設定
	g_vertexWkResultmenu2[0].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X, TITLE_EXIT_POS_Y, 0.0f);
	g_vertexWkResultmenu2[1].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X + TITLE_MENU_SIZE_X, TITLE_EXIT_POS_Y, 0.0f);
	g_vertexWkResultmenu2[2].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X, TITLE_EXIT_POS_Y + TITLE_MENU_SIZE_Y, 0.0f);
	g_vertexWkResultmenu2[3].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X + TITLE_MENU_SIZE_X, TITLE_EXIT_POS_Y + TITLE_MENU_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkResultmenu2[0].rhw =
		g_vertexWkResultmenu2[1].rhw =
		g_vertexWkResultmenu2[2].rhw =
		g_vertexWkResultmenu2[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkResultmenu2[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultmenu2[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultmenu2[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultmenu2[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkResultmenu2[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkResultmenu2[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkResultmenu2[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkResultmenu2[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexResultScore(void)
{
	SCORE *score = GetScore();		// スコアのポインターを初期化

	int i;
	float habaX = RESULTSCORE_SIZE_W;	// 数字の横幅

	// 桁数分処理する
	for (i = 0; i < SCORE_DIGIT; i++)
	{
		// 頂点座標の設定
		g_vertexWkResultScore[i][0].vtx.x = -habaX * i + score->posScore.x;
		g_vertexWkResultScore[i][0].vtx.y = score->posScore.y;
		g_vertexWkResultScore[i][0].vtx.z = 0.0f;
		g_vertexWkResultScore[i][1].vtx.x = -habaX * i + score->posScore.x + RESULTSCORE_SIZE_W;
		g_vertexWkResultScore[i][1].vtx.y = score->posScore.y;
		g_vertexWkResultScore[i][1].vtx.z = 0.0f;
		g_vertexWkResultScore[i][2].vtx.x = -habaX * i + score->posScore.x;
		g_vertexWkResultScore[i][2].vtx.y = score->posScore.y + RESULTSCORE_SIZE_H;
		g_vertexWkResultScore[i][2].vtx.z = 0.0f;
		g_vertexWkResultScore[i][3].vtx.x = -habaX * i + score->posScore.x + RESULTSCORE_SIZE_W;
		g_vertexWkResultScore[i][3].vtx.y = score->posScore.y + RESULTSCORE_SIZE_H;
		g_vertexWkResultScore[i][3].vtx.z = 0.0f;

		// rhwの設定
		g_vertexWkResultScore[i][0].rhw =
			g_vertexWkResultScore[i][1].rhw =
			g_vertexWkResultScore[i][2].rhw =
			g_vertexWkResultScore[i][3].rhw = 1.0f;

		// 反射光の設定
		g_vertexWkResultScore[i][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkResultScore[i][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkResultScore[i][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkResultScore[i][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		g_vertexWkResultScore[i][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		g_vertexWkResultScore[i][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		g_vertexWkResultScore[i][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		g_vertexWkResultScore[i][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetTextureResultScore(void)
{
	SCORE *score = GetScore();		// スコアのポインターを初期化

	int i;
	int number = score->score;

	for (i = 0; i < SCORE_DIGIT; i++)
	{
		// テクスチャ座標の設定
		float x = (float)(number % 10);
		g_vertexWkResultScore[i][0].tex = D3DXVECTOR2(0.1f * x, 0.0f);
		g_vertexWkResultScore[i][1].tex = D3DXVECTOR2(0.1f * (x + 1), 0.0f);
		g_vertexWkResultScore[i][2].tex = D3DXVECTOR2(0.1f * x, 1.0f);
		g_vertexWkResultScore[i][3].tex = D3DXVECTOR2(0.1f * (x + 1), 1.0f);
		number /= 10;
	}
}

//=============================================================================
// メニュー色の設定
//=============================================================================
void SetResultMenuDiffuse(int menu)
{
	switch (menu)
	{
	case 0:
		g_vertexWkResultmenu1[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkResultmenu1[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkResultmenu1[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkResultmenu1[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		g_vertexWkResultmenu2[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkResultmenu2[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkResultmenu2[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkResultmenu2[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		break;
	case 1:
		g_vertexWkResultmenu1[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkResultmenu1[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkResultmenu1[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkResultmenu1[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);

		g_vertexWkResultmenu2[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkResultmenu2[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkResultmenu2[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkResultmenu2[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		break;
	}
}

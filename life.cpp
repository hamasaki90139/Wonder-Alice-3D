//=============================================================================
//
// ライフ処理 [life.cpp]
// Author : 濱崎 小春
//
//=============================================================================
#include "life.h"
#include "fade.h"
#include "player.h"
#include "debugproc.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexLife(LPDIRECT3DDEVICE9 pDevice);
void SetTextureLife(int nIdx);
HRESULT MakeVertexPlayerUI(void);
void SetVertexPlayerUI(void);

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureLife[2] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffLife = NULL;		// 頂点バッファインターフェースへのポインタ

LPDIRECT3DTEXTURE9		g_pD3DTexturePlayerUI = {};		// テクスチャへのポインタ
VERTEX_2D				g_vertexWkPlayerUI[NUM_VERTEX];					// 頂点情報格納ワーク

D3DXVECTOR3				g_posLife;						// 位置
D3DXVECTOR3				g_rotLife;						// 向き

int						g_nLife;						// ライフ

D3DXVECTOR3				g_posUIplayer;					// 位置
D3DXVECTOR3				g_rotUIplayer;					// 向き

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_posLife = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotLife = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_posUIplayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotUIplayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ライフの初期化
	g_nLife = NUM_PLACE;

	// 頂点情報の作成
	MakeVertexLife(pDevice);
	MakeVertexPlayerUI();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_LIFE,			// ファイルの名前
		&g_pD3DTextureLife[0]);	// 読み込むメモリー
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_FRAME_LIFE,			// ファイルの名前
		&g_pD3DTextureLife[1]);	// 読み込むメモリー
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_PLAYER_UI,			// ファイルの名前
		&g_pD3DTexturePlayerUI);	// 読み込むメモリー

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitLife(void)
{
	if (g_pD3DTexturePlayerUI != NULL)
	{// テクスチャの開放
		g_pD3DTexturePlayerUI->Release();
		g_pD3DTexturePlayerUI = NULL;
	}
	if (g_pD3DTextureLife[0] != NULL)
	{// テクスチャの開放
		g_pD3DTextureLife[0]->Release();
		g_pD3DTextureLife[0] = NULL;
	}
	if (g_pD3DTextureLife[1] != NULL)
	{// テクスチャの開放
		g_pD3DTextureLife[1]->Release();
		g_pD3DTextureLife[1] = NULL;
	}
	if (g_pD3DVtxBuffLife != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffLife->Release();
		g_pD3DVtxBuffLife = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateLife(void)
{
	PLAYER *player = GetPlayer();

	if (g_nLife <= 0)
	{
		player->use = false;
		SetFade(FADE_OUT);
	}

	SetVertexPlayerUI();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// playerUI
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTexturePlayerUI);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkPlayerUI, sizeof(VERTEX_2D));

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pD3DVtxBuffLife, 0, sizeof(VERTEX_2D));

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureLife[1]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (NUM_PLACE * 4), NUM_POLYGON);

	// LIFE
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureLife[0]);

	// ポリゴンの描画
	for (int nCntPlace = 0; nCntPlace < g_nLife; nCntPlace++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntPlace * 4), NUM_POLYGON);
	}

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexLife(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (NUM_VERTEX * NUM_PLACE + 4),		// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF_VERTEX_2D,								// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffLife,							// 頂点バッファインターフェースへのポインタ
		NULL)))										// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(LIFE_POS_X + nCntPlace * LIFE_SIZE_X + LIFE_INTERVAL_X, LIFE_POS_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(LIFE_POS_X + nCntPlace * (LIFE_INTERVAL_X + LIFE_SIZE_X) + LIFE_SIZE_X, LIFE_POS_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(LIFE_POS_X + nCntPlace * LIFE_SIZE_X + LIFE_INTERVAL_X, LIFE_POS_Y + LIFE_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(LIFE_POS_X + nCntPlace * (LIFE_INTERVAL_X + LIFE_SIZE_X) + LIFE_SIZE_X, LIFE_POS_Y + LIFE_SIZE_Y, 0.0f);

			// rhwの設定
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(LIFE_POS_X - 35, LIFE_POS_Y - 25, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(LIFE_POS_X + (LIFE_INTERVAL_X + LIFE_SIZE_X) * NUM_PLACE + 15, LIFE_POS_Y - 25, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(LIFE_POS_X - 35, LIFE_POS_Y + 75, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(LIFE_POS_X + (LIFE_INTERVAL_X + LIFE_SIZE_X) * NUM_PLACE + 15, LIFE_POS_Y + 75, 0.0f);

		// rhwの設定
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffLife->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexPlayerUI(void)
{
	// 頂点座標の設定
	g_vertexWkPlayerUI[0].vtx = D3DXVECTOR3(PLAYER_UI_POS_X, PLAYER_UI_POS_Y, 0.0f);
	g_vertexWkPlayerUI[1].vtx = D3DXVECTOR3(PLAYER_UI_POS_X + PLAYER_UI_SIZE_X, PLAYER_UI_POS_Y, 0.0f);
	g_vertexWkPlayerUI[2].vtx = D3DXVECTOR3(PLAYER_UI_POS_X, PLAYER_UI_POS_Y + PLAYER_UI_SIZE_Y, 0.0f);
	g_vertexWkPlayerUI[3].vtx = D3DXVECTOR3(PLAYER_UI_POS_X + PLAYER_UI_SIZE_X, PLAYER_UI_POS_Y + PLAYER_UI_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkPlayerUI[0].rhw =
		g_vertexWkPlayerUI[1].rhw =
		g_vertexWkPlayerUI[2].rhw =
		g_vertexWkPlayerUI[3].rhw = 1.0f;

	// 反射光の設定
	SetVertexPlayerUI();

	// テクスチャ座標の設定
	g_vertexWkPlayerUI[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkPlayerUI[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkPlayerUI[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkPlayerUI[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureLife(int nIdx)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdx * 4);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点データをアンロックする
	g_pD3DVtxBuffLife->Unlock();
}

//=============================================================================
// ライフの変更
//=============================================================================
void ChangeLife(int nValue)
{
	g_nLife += nValue;
	if (g_nLife > NUM_PLACE)
	{
		g_nLife = NUM_PLACE;
	}
	else if (g_nLife < 0)
	{
		g_nLife = 0;
	}
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexPlayerUI(void)
{
	PLAYER *player = GetPlayer();

	if (player->protect == true)
	{
		// 反射光の設定
		g_vertexWkPlayerUI[0].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
		g_vertexWkPlayerUI[1].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
		g_vertexWkPlayerUI[2].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
		g_vertexWkPlayerUI[3].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
	}
	else
	{
		// 反射光の設定
		g_vertexWkPlayerUI[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkPlayerUI[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkPlayerUI[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkPlayerUI[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	}
}


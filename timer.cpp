//=============================================================================
//
// タイマー処理 [timer.cpp]
// Author : 濱崎 小春
//
//=============================================================================
#include "timer.h"
#include "fade.h"
#include "debugproc.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTimer(LPDIRECT3DDEVICE9 pDevice);
HRESULT MakeVertexTimerCnt(void);
void SetVertexTimerCnt(void);
void SetTextureTimer(int idx, int number);
void SetTextureTimerCnt(int cntPattern);

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9			g_pD3DTextureTimer = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pD3DVtxBuffTimer = NULL;		// 頂点バッファインターフェースへのポインタ
VERTEX_2D					g_vertexWkTimer[NUM_VERTEX];	// 頂点情報格納ワーク

static TIMER				timerWk;						// タイマー構造体

static D3DXVECTOR3			g_posTimer;						// タイマー位置
static D3DXVECTOR3			g_rotTimer;						// タイマー回転
static int					g_nTimer;						// タイマーカウント
static bool					g_bEnableTimer;					// タイマー動作ON/OFF

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_FRAME_TIMER,			// ファイルの名前
		&g_pD3DTextureTimer);	// 読み込むメモリー
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_TOKEI,			// ファイルの名前
		&timerWk.g_pD3DTextureTimer);	// 読み込むメモリー

	// ケーキの初期化処理
	timerWk.use = true;										// 使用
	timerWk.pos = D3DXVECTOR3(HARI_POS_X, HARI_POS_Y, 0.0f);	// 座標データを初期化
	timerWk.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 回転データを初期化

	D3DXVECTOR2 temp = D3DXVECTOR2(HARI_SIZE_X, HARI_SIZE_Y);
	timerWk.Radius = D3DXVec2Length(&temp);					// オブジェクトの半径を初期化
	timerWk.BaseAngle = atan2f(HARI_SIZE_Y, HARI_SIZE_X);	// オブジェクトの角度を初期化

	// タイマーの初期化
	g_nTimer = 999 * 60;
	g_bEnableTimer = true;

	// 頂点情報の作成
	MakeVertexTimer(pDevice);
	MakeVertexTimerCnt();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTimer(void)
{
	if (g_pD3DTextureTimer != NULL)
	{// テクスチャの開放
		g_pD3DTextureTimer->Release();
		g_pD3DTextureTimer = NULL;
	}
	if (g_pD3DTextureTimer != NULL)
	{// テクスチャの開放
		g_pD3DTextureTimer->Release();
		g_pD3DTextureTimer = NULL;
	}
	if (timerWk.g_pD3DTextureTimer != NULL)
	{// timerWk.テクスチャの開放
		timerWk.g_pD3DTextureTimer->Release();
		timerWk.g_pD3DTextureTimer = NULL;
	}

	if (g_pD3DVtxBuffTimer != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffTimer->Release();
		g_pD3DVtxBuffTimer = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTimer(void)
{
	if (g_bEnableTimer)
	{
		g_nTimer--;
		if (g_nTimer < 0)
		{
			g_nTimer = 0;
			timerWk.rot.z = 0.0f;

			SetFade(FADE_OUT);
		}
		else if ((g_nTimer / 60) >= (int)(powf(10.0f, (float)(NUM_PLACE + 1))))
		{
			g_nTimer = (int)(powf(10.0f, (float)(NUM_PLACE + 1))) - 1;
		}
	}

	if (g_nTimer % TIMER_MAX == 0)
	{
		timerWk.rot.z += (D3DX_PI * 2) / 60;	//時計の針を60分の1ずつ回転させる
	}

	SetVertexTimerCnt();						// 移動後の座標で頂点を設定

	//PrintDebugProc("g_timer_rotz %f\n", g_rotTimer.z);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pD3DVtxBuffTimer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureTimer);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (NUM_PLACE * 4), NUM_POLYGON);

	// テクスチャの設定
	pDevice->SetTexture(0, timerWk.g_pD3DTextureTimer);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, timerWk.vertexWk, sizeof(VERTEX_2D));

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTimer(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (NUM_VERTEX * NUM_PLACE + 4),	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,								// 頂点バッファの使用法　
		FVF_VERTEX_2D,									// 使用する頂点フォーマット
		D3DPOOL_MANAGED,								// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffTimer,							// 頂点バッファインターフェースへのポインタ
		NULL)))											// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(TIMER_POS_X + nCntPlace * TIMER_SIZE_X + TIMER_INTERVAL_X, TIMER_POS_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(TIMER_POS_X + nCntPlace * (TIMER_INTERVAL_X + TIMER_SIZE_X) + TIMER_SIZE_X, TIMER_POS_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(TIMER_POS_X + nCntPlace * TIMER_SIZE_X + TIMER_INTERVAL_X, TIMER_POS_Y + TIMER_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(TIMER_POS_X + nCntPlace * (TIMER_INTERVAL_X + TIMER_SIZE_X) + TIMER_SIZE_X, TIMER_POS_Y + TIMER_SIZE_Y, 0.0f);

			// rhwの設定
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// 反射光の設定
			pVtx[0].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
			pVtx[1].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
			pVtx[2].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
			pVtx[3].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(TIMER_POS_X - 17.5f, TIMER_POS_Y - 25, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(TIMER_POS_X + (TIMER_INTERVAL_X + TIMER_SIZE_X) * NUM_PLACE + 17.5f, TIMER_POS_Y - 25, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(TIMER_POS_X - 17.5f, TIMER_POS_Y + 110, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(TIMER_POS_X + (TIMER_INTERVAL_X + TIMER_SIZE_X) * NUM_PLACE + 17.5f, TIMER_POS_Y + 110, 0.0f);

		// rhwの設定
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		// 反射光の設定
		pVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffTimer->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点の作成(ケーキ)
//=============================================================================
HRESULT MakeVertexTimerCnt(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	SetVertexTimerCnt();

	// rhwの設定
	timerWk.vertexWk[0].rhw =
		timerWk.vertexWk[1].rhw =
		timerWk.vertexWk[2].rhw =
		timerWk.vertexWk[3].rhw = 1.0f;

	timerWk.vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	timerWk.vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	timerWk.vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	timerWk.vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	timerWk.vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	timerWk.vertexWk[1].tex = D3DXVECTOR2(1.0f / TIMER_TEXTURE_PATTERN_DIVIDE_X, 0.0f);
	timerWk.vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TIMER_TEXTURE_PATTERN_DIVIDE_Y);
	timerWk.vertexWk[3].tex = D3DXVECTOR2(1.0f / TIMER_TEXTURE_PATTERN_DIVIDE_X, 1.0f / TIMER_TEXTURE_PATTERN_DIVIDE_Y);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureTimer(int idx, int number)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (idx * 4);

	// 頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(number * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(number * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 1.0f);

	// 頂点データをアンロックする
	g_pD3DVtxBuffTimer->Unlock();
}

void SetVertexTimerCnt(void)
{
	timerWk.vertexWk[0].vtx.x = timerWk.pos.x - cosf(timerWk.BaseAngle + timerWk.rot.z) * timerWk.Radius;
	timerWk.vertexWk[0].vtx.y = timerWk.pos.y - sinf(timerWk.BaseAngle + timerWk.rot.z) * timerWk.Radius;
	timerWk.vertexWk[0].vtx.z = 0.0f;

	timerWk.vertexWk[1].vtx.x = timerWk.pos.x + cosf(timerWk.BaseAngle - timerWk.rot.z) * timerWk.Radius;
	timerWk.vertexWk[1].vtx.y = timerWk.pos.y - sinf(timerWk.BaseAngle - timerWk.rot.z) * timerWk.Radius;
	timerWk.vertexWk[1].vtx.z = 0.0f;

	timerWk.vertexWk[2].vtx.x = timerWk.pos.x - cosf(timerWk.BaseAngle - timerWk.rot.z) * timerWk.Radius;
	timerWk.vertexWk[2].vtx.y = timerWk.pos.y + sinf(timerWk.BaseAngle - timerWk.rot.z) * timerWk.Radius;
	timerWk.vertexWk[2].vtx.z = 0.0f;

	timerWk.vertexWk[3].vtx.x = timerWk.pos.x + cosf(timerWk.BaseAngle + timerWk.rot.z) * timerWk.Radius;
	timerWk.vertexWk[3].vtx.y = timerWk.pos.y + sinf(timerWk.BaseAngle + timerWk.rot.z) * timerWk.Radius;
	timerWk.vertexWk[3].vtx.z = 0.0f;
}

void SetTextureTimerCnt(int cntPattern)
{
	// テクスチャ座標の設定
	int x = cntPattern % TIMER_TEXTURE_PATTERN_DIVIDE_X;
	int y = cntPattern / TIMER_TEXTURE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / TIMER_TEXTURE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / TIMER_TEXTURE_PATTERN_DIVIDE_Y;

	timerWk.vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	timerWk.vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	timerWk.vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	timerWk.vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// タイマーの開始
//=============================================================================
void EnableTimer(bool bEnable)
{
	g_bEnableTimer = bEnable;
}

//=============================================================================
// タイマーのリセット
//=============================================================================
void ResetTimer(int nTime)
{
	g_nTimer = nTime * 60;
}


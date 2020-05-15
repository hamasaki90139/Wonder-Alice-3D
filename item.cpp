//=============================================================================
//
// アイテム処理 [item.cpp]
// Author : 濱崎 小春
//
//=============================================================================
#include "item.h"
#include "camera.h"
#include "shadow.h"
#include "player.h"
#include "score.h"
#include "life.h"
#include "collision.h"
#include "debugproc.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	VALUE_ROTATE_ITEM		(D3DX_PI * 0.025f)						// 回転速度
#define	ITEM_RADIUS				(10.0f)									// 半径
#define	ITEM_TEXTURE01			"data/TEXTURE/★cake_001.png"			// 読み込むモデル名
#define	ITEM_TEXTURE02			"data/TEXTURE/★cookie_001.png"			// 読み込むモデル名
#define	ITEM_TEXTURE03			"data/TEXTURE/★tea_001.png"			// 読み込むモデル名
#define	ITEM_WIDTH				(50.0f)									// アイテムの半径幅
#define	ITEM_HEIGHT				(50.0f)									// アイテムの半径幅

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexItem(LPDIRECT3DDEVICE9 pDevice);
void SetVertexItem(int nIdxItem, float fWidth, float fHeight);
void SetColorItem(int nIdxItem, D3DXCOLOR col);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ITEM				g_Item[MAX_ITEM];						// アイテム
LPDIRECT3DTEXTURE9		g_apD3DTextureItem[ITEM_TYPE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffItem = NULL;				// 頂点バッファインターフェースへのポインタ
static int				g_nAlpha;								// アルファテストの閾値

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitItem(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		for (int i = 0; i < ITEM_TYPE_MAX; i++)
		{
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,						// デバイスのポインタ
				ITEM_TEXTURE01,										// ファイルの名前
				&g_apD3DTextureItem[ITEM_CAKE]);					// 読み込むメモリのポインタ

			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,						// デバイスのポインタ
				ITEM_TEXTURE02,										// ファイルの名前
				&g_apD3DTextureItem[ITEM_COOKIE]);					// 読み込むメモリのポインタ

			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,						// デバイスのポインタ
				ITEM_TEXTURE03,										// ファイルの名前
				&g_apD3DTextureItem[ITEM_TEA]);						// 読み込むメモリのポインタ
		}
	}

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		switch (nCntItem % ITEM_TYPE_MAX)
		{
		case ITEM_CAKE:
			// アイテムタイプの登録
			g_Item[nCntItem].type = ITEM_CAKE;
			break;

		case ITEM_COOKIE:
			// アイテムタイプの登録
			g_Item[nCntItem].type = ITEM_COOKIE;
			break;

		case ITEM_TEA:
			// アイテムタイプの登録
			g_Item[nCntItem].type = ITEM_TEA;
			break;
		}

#if 0
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,									// デバイスへのポインタ
			TEXTURE_FILENAME,												// ファイルの名前
			&g_pD3DTextureModelItem[nCntItemType]);							// 読み込むメモリー
#endif

		g_Item[nCntItem].pos = D3DXVECTOR3(0.0f, 100.0f, 0.0f);				// アイテム初期位置Yを少し浮かす
		g_Item[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Item[nCntItem].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Item[nCntItem].nIdxShadow = -1;
		g_Item[nCntItem].bUse = false;
	}

	// 頂点情報の作成
	MakeVertexItem(pDevice);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitItem(void)
{
	for (int i = 0; i < ITEM_TYPE_MAX; i++)
	{
		if (g_apD3DTextureItem[i] != NULL)
		{// テクスチャの開放
			g_apD3DTextureItem[i]->Release();
			g_apD3DTextureItem[i] = NULL;
		}
	}

	if (g_pD3DVtxBuffItem != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffItem->Release();
		g_pD3DVtxBuffItem = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateItem(void)
{
	ENEMY *enemy = GetEnemy();
	PLAYER *player = GetPlayer();
	SCORE *score = GetScore();

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_Item[nCntItem].bUse)
		{	
			// アイテムをエネミーが消滅した位置へ出現させる
			g_Item[nCntItem].pos.x = enemy[nCntItem].hit_oldpos.x;
			g_Item[nCntItem].pos.z = enemy[nCntItem].hit_oldpos.z;

			// 重力処理
			g_Item[nCntItem].pos.y -= 1.0f;
			if (g_Item[nCntItem].pos.y < 0.0f)
			{
				g_Item[nCntItem].pos.y = 0.0f;
			}

			// 影の位置設定
			D3DXVECTOR3 pos = g_Item[nCntItem].pos;
			pos.y = 0.0f;
			SetPositionShadow(g_Item[nCntItem].nIdxShadow, pos, g_Item[nCntItem].scl);

			//プレイヤーとアイテムの衝突判定
			float r1, r2;
			r1 = ITEM_R;									//アイテムカプセルの半径
			r2 = PLAYER_R;									//プレイヤーカプセルの半径

			if (CheckHitCC(g_Item[nCntItem].pos, D3DXVECTOR3(g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y, g_Item[nCntItem].pos.z), player->pos, D3DXVECTOR3(player->pos.x, player->pos.y + ENEMY_LEN, player->pos.z), r1, r2))
			{
				// SE再生
				PlaySound(SOUND_LABEL_SE_ITEM);

				if (g_Item[nCntItem].type == ITEM_CAKE)				//ケーキだったらスコア10×倍率
				{
					for (int i = 0; i < 1; i++)
					{
						score->score += 10 * score->score_mag;
					}
				}
				else if (g_Item[nCntItem].type == ITEM_COOKIE)		//クッキーだったらライフ回復
				{
					for (int i = 0; i < 1; i++)
					{
						ChangeLife(1);
					}
				}
				else if (g_Item[nCntItem].type == ITEM_TEA)			//紅茶だったらスコア2倍
				{
					for (int i = 0; i < 1; i++)
					{
						score->score_mag++;
					}
				}

				DeleteItem(nCntItem);		//消す
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, g_nAlpha);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (g_Item[i].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Item[i].mtxWorld);

			// ビューマトリックスを取得
			mtxView = GetMtxView();

			// ポリゴンを正面に向ける
			// 逆行列をもとめる
			g_Item[i].mtxWorld._11 = mtxView._11;
			g_Item[i].mtxWorld._12 = mtxView._21;
			g_Item[i].mtxWorld._13 = mtxView._31;

			g_Item[i].mtxWorld._21 = mtxView._12;
			g_Item[i].mtxWorld._22 = mtxView._22;
			g_Item[i].mtxWorld._23 = mtxView._32;

			g_Item[i].mtxWorld._31 = mtxView._13;
			g_Item[i].mtxWorld._32 = mtxView._23;
			g_Item[i].mtxWorld._33 = mtxView._33;

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, g_Item[i].scl.x,
				g_Item[i].scl.y,
				g_Item[i].scl.z);
			D3DXMatrixMultiply(&g_Item[i].mtxWorld, &g_Item[i].mtxWorld, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_Item[i].pos.x,
				g_Item[i].pos.y,
				g_Item[i].pos.z);
			D3DXMatrixMultiply(&g_Item[i].mtxWorld, &g_Item[i].mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Item[i].mtxWorld);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pD3DVtxBuffItem, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apD3DTextureItem[i % ITEM_TYPE_MAX]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * 4), NUM_POLYGON);
		}
	}

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// αテストを無効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexItem(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_ITEM,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,						// 頂点バッファの使用法　
		FVF_VERTEX_3D,							// 使用する頂点フォーマット
		D3DPOOL_MANAGED,						// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffItem,						// 頂点バッファインターフェースへのポインタ
		NULL)))									// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-ITEM_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(-ITEM_WIDTH / 2.0f, ITEM_HEIGHT, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(ITEM_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(ITEM_WIDTH / 2.0f, ITEM_HEIGHT, 0.0f);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);
		}

		// 頂点データをアンロックする
		g_pD3DVtxBuffItem->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexItem(int nIdxItem, float fWidth, float fHeight)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxItem * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fWidth / 2.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fWidth / 2.0f, fHeight, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fWidth / 2.0f, 0.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fWidth / 2.0f, fHeight, 0.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffItem->Unlock();
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorItem(int nIdxItem, D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxItem * 4);

		// 頂点座標の設定
		pVtx[0].diffuse = col;
		pVtx[1].diffuse = col;
		pVtx[2].diffuse = col;
		pVtx[3].diffuse = col;

		// 頂点データをアンロックする
		g_pD3DVtxBuffItem->Unlock();
	}
}

//=============================================================================
// アイテムの削除
//=============================================================================
void DeleteItem(int nIdxItem)
{
	if (nIdxItem >= 0 && nIdxItem < MAX_ITEM)
	{
		ReleaseShadow(g_Item[nIdxItem].nIdxShadow);
		g_Item[nIdxItem].bUse = false;
	}
}

//=============================================================================
// アイテムの取得
//=============================================================================
ITEM *GetItem(void)
{
	return &g_Item[0];
}

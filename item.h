//=============================================================================
//
// アイテム処理 [item.h]
// Author : 濱崎 小春
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"
#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_ITEM				(MAX_ENEMY)						// アイテム最大数
#define ITEM_R					(20)							//アイテムの半径

//**************************************
// アイテムのタイプ
//**************************************
enum ITEM_TYPE
{
	ITEM_CAKE,
	ITEM_COOKIE,
	ITEM_TEA,
	ITEM_TYPE_MAX
};

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 現在の位置
	D3DXVECTOR3 rot;		// 現在の向き
	D3DXVECTOR3 scl;		// 現在の大きさ
	float fRadius;			// 半径
	int nIdxShadow;			// 影ID
	bool bUse;				// 使用しているかどうか
	int type;				//アイテムタイプ

	LPDIRECT3DTEXTURE9	pD3DTexture;		// テクスチャへのポインタ
	LPD3DXMESH			pD3DXMesh;			// メッシュ情報へのポインタ
	LPD3DXBUFFER		pD3DXBuffMat;		// マテリアル情報へのポインタ
	DWORD				nNumMat;			// マテリアル情報の数
	VERTEX_2D			vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク
	D3DXMATRIX			mtxWorld;			// ワールドマトリックス

} ITEM;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitItem(int type);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void DeleteItem(int nIdxItem);
ITEM *GetItem(void);

#endif

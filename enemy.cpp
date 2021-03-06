//=============================================================================
//
// エネミーモデル処理[enemy.cpp]
// Author : 濱崎 小春
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "enemy.h"
#include "shadow.h"
#include "player.h"
#include "item.h"
#include "collision.h"
#include "debugproc.h"
#include "particle.h"

//*****************************************************************************
//プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル宣言
//*****************************************************************************

static ENEMY				g_Enemy[MAX_ENEMY];							// エネミー
static ENEMY				g_Enemyhead[MAX_ENEMY];						// プレイヤーの頭部
static ENEMY				g_EnemyarmR[MAX_ENEMY];                     // プレイヤーの腕
static ENEMY				g_EnemyarmL[MAX_ENEMY];                     // プレイヤーの腕
static ENEMY				g_EnemylegR[MAX_ENEMY];                     // プレイヤーの太もも
static ENEMY				g_EnemylegL[MAX_ENEMY];                     // プレイヤーの太もも
D3DXVECTOR3					g_Shadowpos[ENEMY_GROUP];					// ビルボード発生位置

LPDIRECT3DTEXTURE9	enemy_pD3DTexture_body;			// テクスチャへのポインタ
LPD3DXMESH			enemy_pD3DXMesh_body;			// メッシュ情報へのポインタ
LPD3DXBUFFER		enemy_pD3DXBuffMat_body;		// マテリアル情報へのポインタ
DWORD				enemy_nNumMat_body;				// マテリアル情報の数

LPDIRECT3DTEXTURE9	enemy_pD3DTexture_head;			// テクスチャへのポインタ
LPD3DXMESH			enemy_pD3DXMesh_head;			// メッシュ情報へのポインタ
LPD3DXBUFFER		enemy_pD3DXBuffMat_head;		// マテリアル情報へのポインタ
DWORD				enemy_nNumMat_head;				// マテリアル情報の数

LPDIRECT3DTEXTURE9	enemy_pD3DTexture_armL;			// テクスチャへのポインタ
LPD3DXMESH			enemy_pD3DXMesh_armL;			// メッシュ情報へのポインタ
LPD3DXBUFFER		enemy_pD3DXBuffMat_armL;		// マテリアル情報へのポインタ
DWORD				enemy_nNumMat_armL;				// マテリアル情報の数

LPDIRECT3DTEXTURE9	enemy_pD3DTexture_armR;			// テクスチャへのポインタ
LPD3DXMESH			enemy_pD3DXMesh_armR;			// メッシュ情報へのポインタ
LPD3DXBUFFER		enemy_pD3DXBuffMat_armR;		// マテリアル情報へのポインタ
DWORD				enemy_nNumMat_armR;				// マテリアル情報の数

LPDIRECT3DTEXTURE9	enemy_pD3DTexture_legL;			// テクスチャへのポインタ
LPD3DXMESH			enemy_pD3DXMesh_legL;			// メッシュ情報へのポインタ
LPD3DXBUFFER		enemy_pD3DXBuffMat_legL;		// マテリアル情報へのポインタ
DWORD				enemy_nNumMat_legL;				// マテリアル情報の数

LPDIRECT3DTEXTURE9	enemy_pD3DTexture_legR;			// テクスチャへのポインタ
LPD3DXMESH			enemy_pD3DXMesh_legR;			// メッシュ情報へのポインタ
LPD3DXBUFFER		enemy_pD3DXBuffMat_legR;		// マテリアル情報へのポインタ
DWORD				enemy_nNumMat_legR;				// マテリアル情報の数

ENEMYTBL move_tbl1[] = {
	{D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.01f},
	{D3DXVECTOR3(100.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,-2.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.02f},
	{D3DXVECTOR3(-100.0f,0.0f,-100.0f),D3DXVECTOR3(0.0f,3.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.05f},
	{D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.0f},
};

ENEMYTBL move_tbl2[] = {
	{D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.01f},
	{D3DXVECTOR3(50.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,-2.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.02f},
	{D3DXVECTOR3(100.0f,0.0f,-300.0f),D3DXVECTOR3(0.0f,8.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.04f},
	{D3DXVECTOR3(200.0f,0.0f,-100.0f),D3DXVECTOR3(0.0f,.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.06f},
	{D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.0f},
};

ENEMYTBL move_tbl3[] = {
	{D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.01f},
	{D3DXVECTOR3(50.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,-3.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.02f},
	{D3DXVECTOR3(100.0f,0.0f,-300.0f),D3DXVECTOR3(0.0f,5.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.04f},
	{D3DXVECTOR3(150.0f,0.0f,400.0f),D3DXVECTOR3(0.0f,5.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.06f},
	{D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.0f},
};

ENEMYTBL move_tbl4[] = {
	{D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.01f},
	{D3DXVECTOR3(300.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,-3.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.02f},
	{D3DXVECTOR3(500.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,5.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.04f},
	{D3DXVECTOR3(300.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,5.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.06f},
	{D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.0f},
};

ENEMYTBL move_tbl5[] = {
	{D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.01f},
	{D3DXVECTOR3(0.0f,0.0f,300.0f),D3DXVECTOR3(0.0f,-3.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.02f},
	{D3DXVECTOR3(0.0f,0.0f,500.0f),D3DXVECTOR3(0.0f,5.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.04f},
	{D3DXVECTOR3(0.0f,0.0f,300.0f),D3DXVECTOR3(0.0f,5.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.06f},
	{D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.0f},
};

// 階層アニメーション
ENEMY_SET set_tbl_ENEMY[] = {    //pos,rot,scl 各パーツの初期値
	{	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.0f, (rand() % 300 + 1.0f), 0.0f),	D3DXVECTOR3(0.5f, 0.5f, 0.5f)},//体			
	{	D3DXVECTOR3(0.0f, 50.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.9f, 0.9f, 0.9f)},//頭部		
	{	D3DXVECTOR3(-12.0f, 40.0f, -0.1f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.9f, 0.9f, 0.9f)},//右腕
	{	D3DXVECTOR3(12.0f, 40.0f, -0.1f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.9f, 0.9f, 0.9f)},//左腕
	{	D3DXVECTOR3(-5.0f,20.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f)},//右太もも
	{	D3DXVECTOR3(5.0f, 20.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f)},//左太もも
};

//歩きモーション
D3DXVECTOR3 Ebody_walk_tbl[] = { //体

	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.00000f, -1.000000f, 0.000000f),
	D3DXVECTOR3(0.0f, 1.0f, 0.000000f),
};

D3DXVECTOR3 Ehead_walk_tbl[] = { //頭部

	D3DXVECTOR3(0.000000f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.000000f, D3DX_PI * 2, 0.000000f),
	D3DXVECTOR3(0.000000f, D3DX_PI * 2, 0.000000f),

};

D3DXVECTOR3 EarmR_walk_tbl[] = { //右腕

	D3DXVECTOR3(0.374533f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.574533f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.374533f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 EarmL_walk_tbl[] = { //左腕

	D3DXVECTOR3(-0.374533f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.574533f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.374533f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 ElegR_walk_tbl[] = { //右太もも

	D3DXVECTOR3(-1.023599f, 0.000000f, 0.000000f),
	D3DXVECTOR3(1.023599f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-1.023599f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 ElegL_walk_tbl[] = { //左太もも

	D3DXVECTOR3(1.023599f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-1.023599f, 0.000000f, 0.000000f),
	D3DXVECTOR3(1.023599f, 0.000000f, 0.000000f),
};

//攻撃モーション
D3DXVECTOR3 Ebody_Attack_tbl[] = { //体

	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.000000f),
	D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.000000f),
	D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.000000f),
	D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.000000f),
	D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.000000f),
	D3DXVECTOR3(-1.50000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-1.40000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-1.30000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-1.00000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.00000f, -1.000000f, 0.000000f),
	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 Ehead_Attack_tbl[] = { //頭部

	D3DXVECTOR3(0.00000f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.0f, D3DX_PI * 2, 0.000000f),
	D3DXVECTOR3(0.0f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.0f, D3DX_PI * 2, 0.000000f),
	D3DXVECTOR3(0.0f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.0f, D3DX_PI * 2, 0.000000f),
	D3DXVECTOR3(0.0f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.0f, D3DX_PI * 2, 0.000000f),
	D3DXVECTOR3(0.0f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.0f, D3DX_PI * 2, 0.000000f),
	D3DXVECTOR3(0.00000f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.00000f, D3DX_PI * 2, 0.000000f),
};

D3DXVECTOR3 EarmR_Attack_tbl[] = { //右腕

	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 EarmL_Attack_tbl[] = { //左腕

	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 ElegR_Attack_tbl[] = { //右太もも

	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 ElegL_Attack_tbl[] = { //左太もも

	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
};

//倒れるときのモーション
D3DXVECTOR3 Ebody_Hit_tbl[] = { //体
	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.00000f, 0.0f,  D3DX_PI * 2),
};

D3DXVECTOR3 Ehead_Hit_tbl[] = { //頭部

	D3DXVECTOR3(0.00000f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.0f, D3DX_PI * 2, 0.000000f),
	D3DXVECTOR3(0.0f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.0f, D3DX_PI * 2, 0.000000f),
	D3DXVECTOR3(0.0f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.0f, D3DX_PI * 2, 0.000000f),
	D3DXVECTOR3(0.0f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.0f, D3DX_PI * 2, 0.000000f),
	D3DXVECTOR3(0.0f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.0f, D3DX_PI * 2, 0.000000f),
	D3DXVECTOR3(0.00000f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.00000f, D3DX_PI * 2, 0.000000f),
};

D3DXVECTOR3 EarmR_Hit_tbl[] = { //右腕

	D3DXVECTOR3(0.00000f, 0.000000f, -2.000000f),
	D3DXVECTOR3(0.0f, 0.0f, -1.500000f),
};

D3DXVECTOR3 EarmL_Hit_tbl[] = { //左腕

	D3DXVECTOR3(0.00000f, 0.000000f, 2.000000f),
	D3DXVECTOR3(0.0f, 0.0f, 1.500000f),
};

D3DXVECTOR3 ElegR_Hit_tbl[] = { //右太もも

	D3DXVECTOR3(1.0f, 0.000000f, -2.000000f),
	D3DXVECTOR3(0.5f, 0.0f, -1.500000f),
};

D3DXVECTOR3 ElegL_Hit_tbl[] = { //左太もも

	D3DXVECTOR3(1.0f, 0.000000f, 2.000000f),
	D3DXVECTOR3(0.5f, 0.0f, -1.500000f),
};

D3DXVECTOR3 formation[] =	{   //陣形フォーメーション
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(15.0f, 0.0f, 15.0f),
	D3DXVECTOR3(-15.0f, 0.0f, 15.0f),
	D3DXVECTOR3(20.0f, 0.0f, 20.0f),
	D3DXVECTOR3(-20.0f, 0.0f, 20.0f),
};

D3DXVECTOR3 formation1[] = {   //陣形フォーメーション
	D3DXVECTOR3(0.0f, 30.0f, 200.0f),
	D3DXVECTOR3(50.0f, 0.0f, 15.0f),
	D3DXVECTOR3(-300.0f, 20.0f, 15.0f),
	D3DXVECTOR3(20.0f, 0.0f, 300.0f),
	D3DXVECTOR3(-200.0f, 50.0f, 50.0f),
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		enemy_pD3DTexture_body = NULL;
		enemy_pD3DXMesh_body = NULL;
		enemy_pD3DXBuffMat_body = NULL;
		enemy_nNumMat_body = 0;

		enemy_pD3DTexture_head = NULL;
		enemy_pD3DXMesh_head = NULL;
		enemy_pD3DXBuffMat_head = NULL;
		enemy_nNumMat_head = 0;

		enemy_pD3DTexture_armL = NULL;
		enemy_pD3DXMesh_armL = NULL;
		enemy_pD3DXBuffMat_armL = NULL;
		enemy_nNumMat_armL = 0;

		enemy_pD3DTexture_armR = NULL;
		enemy_pD3DXMesh_armR = NULL;
		enemy_pD3DXBuffMat_armR = NULL;
		enemy_nNumMat_armR = 0;

		enemy_pD3DTexture_legL = NULL;
		enemy_pD3DXMesh_legL = NULL;
		enemy_pD3DXBuffMat_legL = NULL;
		enemy_nNumMat_legL = 0;

		enemy_pD3DTexture_legR = NULL;
		enemy_pD3DXMesh_legR = NULL;
		enemy_pD3DXBuffMat_legR = NULL;
		enemy_nNumMat_legR = 0;

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(MODEL_ENEMY,		// 読み込むモデルファイル名(xファイル)
			D3DXMESH_SYSTEMMEM,							// メッシュの作成オプションを指定
			pDevice,									// IDirect3DDevice9インターフェイスへのポインター
			NULL,										// 隣接性データを含むバッファへのポインタ
			&enemy_pD3DXBuffMat_body,					// マテリアルデータを含むバッファへのポインタ
			NULL,										// エフェクトインスタンスの配列を含むバッファへのポインタ
			&enemy_nNumMat_body,						// D3DXMATERIAL構造体の数
			&enemy_pD3DXMesh_body)))					// ID3DXMeshインターフェイスへのポインタのアドレス
		{
			return E_FAIL;
		}

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(MODEL_ENEMY_HEAD,	// 読み込むモデルファイル名(xファイル)
			D3DXMESH_SYSTEMMEM,							// メッシュの作成オプションを指定
			pDevice,									// IDirect3DDevice9インターフェイスへのポインター
			NULL,										// 隣接性データを含むバッファへのポインタ
			&enemy_pD3DXBuffMat_head,					// マテリアルデータを含むバッファへのポインタ
			NULL,										// エフェクトインスタンスの配列を含むバッファへのポインタ
			&enemy_nNumMat_head,						// D3DXMATERIAL構造体の数
			&enemy_pD3DXMesh_head)))					// ID3DXMeshインターフェイスへのポインタのアドレス
		{
			return E_FAIL;
		}

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(MODEL_ENEMY_ARM_L,	// 読み込むモデルファイル名(xファイル)
			D3DXMESH_SYSTEMMEM,							// メッシュの作成オプションを指定
			pDevice,									// IDirect3DDevice9インターフェイスへのポインター
			NULL,										// 隣接性データを含むバッファへのポインタ
			&enemy_pD3DXBuffMat_armL,					// マテリアルデータを含むバッファへのポインタ
			NULL,										// エフェクトインスタンスの配列を含むバッファへのポインタ
			&enemy_nNumMat_armL,						// D3DXMATERIAL構造体の数
			&enemy_pD3DXMesh_armL)))					// ID3DXMeshインターフェイスへのポインタのアドレス
		{
			return E_FAIL;
		}

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(MODEL_ENEMY_ARM_R,	// 読み込むモデルファイル名(xファイル)
			D3DXMESH_SYSTEMMEM,							// メッシュの作成オプションを指定
			pDevice,									// IDirect3DDevice9インターフェイスへのポインター
			NULL,										// 隣接性データを含むバッファへのポインタ
			&enemy_pD3DXBuffMat_armR,					// マテリアルデータを含むバッファへのポインタ
			NULL,										// エフェクトインスタンスの配列を含むバッファへのポインタ
			&enemy_nNumMat_armR,						// D3DXMATERIAL構造体の数
			&enemy_pD3DXMesh_armR)))					// ID3DXMeshインターフェイスへのポインタのアドレス
		{
			return E_FAIL;
		}

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(MODEL_ENEMY_LEG_L,	// 読み込むモデルファイル名(xファイル)
			D3DXMESH_SYSTEMMEM,							// メッシュの作成オプションを指定
			pDevice,									// IDirect3DDevice9インターフェイスへのポインター
			NULL,										// 隣接性データを含むバッファへのポインタ
			&enemy_pD3DXBuffMat_legL,					// マテリアルデータを含むバッファへのポインタ
			NULL,										// エフェクトインスタンスの配列を含むバッファへのポインタ
			&enemy_nNumMat_legL,						// D3DXMATERIAL構造体の数
			&enemy_pD3DXMesh_legL)))					// ID3DXMeshインターフェイスへのポインタのアドレス
		{
			return E_FAIL;
		}

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(MODEL_ENEMY_LEG_R,	// 読み込むモデルファイル名(xファイル)
			D3DXMESH_SYSTEMMEM,							// メッシュの作成オプションを指定
			pDevice,									// IDirect3DDevice9インターフェイスへのポインター
			NULL,										// 隣接性データを含むバッファへのポインタ
			&enemy_pD3DXBuffMat_legR,					// マテリアルデータを含むバッファへのポインタ
			NULL,										// エフェクトインスタンスの配列を含むバッファへのポインタ
			&enemy_nNumMat_legR,						// D3DXMATERIAL構造体の数
			&enemy_pD3DXMesh_legR)))					// ID3DXMeshインターフェイスへのポインタのアドレス
		{
			return E_FAIL;
		}
	}

#if 0
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		TEXTURE_FILENAME,				// ファイルの名前
		&enemy_pD3DTexture_body);		// 読み込むメモリー
#endif

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_Enemy[i].use = false;			// 最初は未使用
		g_Enemy[i].formation = 0;		// 陣形番号

		//エネミーの線形補間パターンの設定
		switch (i%ENEMY_MOVEPATTERN)
		{
		case 0:
			g_Enemy[i].movepattern = move_tbl1;
			g_Enemy[i].size = sizeof(move_tbl1) / sizeof(ENEMYTBL);
			break;
		case 1:
			g_Enemy[i].movepattern = move_tbl2;
			g_Enemy[i].size = sizeof(move_tbl2) / sizeof(ENEMYTBL);
			break;
		case 2:
			g_Enemy[i].movepattern = move_tbl3;
			g_Enemy[i].size = sizeof(move_tbl3) / sizeof(ENEMYTBL);
			break;
		case 3:
			g_Enemy[i].movepattern = move_tbl4;
			g_Enemy[i].size = sizeof(move_tbl4) / sizeof(ENEMYTBL);
			break;
		case 4:
			g_Enemy[i].movepattern = move_tbl5;
			g_Enemy[i].size = sizeof(move_tbl5) / sizeof(ENEMYTBL);
			break;
		}

		g_Enemy[i].walkmove_time = 0;
		g_Enemy[i].attackmove_time = 0;
		g_Enemy[i].hitmove_time = 0;

		// 体
		// 位置・回転・スケールの初期化設定
		g_Enemy[i].pos = set_tbl_ENEMY[BODY].pos;
		g_Enemy[i].rot = set_tbl_ENEMY[BODY].rot;
		g_Enemy[i].scl = set_tbl_ENEMY[BODY].scl;

		// 基準位置・回転・スケールの初期化設定
		g_Enemy[i].spos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].srot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].sscl = D3DXVECTOR3(0.5f, 0.5f, 0.5f);

		g_Enemy[i].move_time = 0.0f;
		g_Enemy[i].back_time = 0.0f;
		g_Enemy[i].spd = 1.0f;
		g_Enemy[i].following = false;
		g_Enemy[i].timestop = false;

		g_Enemy[i].oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].hit_oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].oldindex = 0;
		g_Enemy[i].pattern = 1;
		g_Enemy[i].parent = NULL;
		g_Enemy[i].status = WALK;

		g_Enemy[i].attack_dt = 0;
		g_Enemy[i].hit_dt = 0;

		g_Enemy[i].hit = 10.0f;
		g_Enemy[i].hit_cnt = 0;
		g_Enemy[i].hit_cnt_max = 40;
		g_Enemy[i].ofs = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		for (int j = 0; j < 5; j++)
		{
			// 影の初期化
			g_Shadowpos[j] = D3DXVECTOR3(g_Enemy[i].pos.x + j * 10.0f, g_Enemy[i].pos.y, g_Enemy[i].pos.z + j * 20);
			g_Shadowpos[j].y = 0.0f;
			g_Enemy[i].shadowIdx[j] = CreateShadow(g_Shadowpos[j], g_Enemy[i].scl);
		}

		// 頭
		g_Enemyhead[i].use = false;			// 最初は未使用

		switch (i%ENEMY_MOVEPATTERN)
		{
		case 0:
			g_Enemyhead[i].movepattern = move_tbl1;
			g_Enemyhead[i].size = sizeof(move_tbl1) / sizeof(ENEMYTBL);
			break;
		case 1:
			g_Enemyhead[i].movepattern = move_tbl2;
			g_Enemyhead[i].size = sizeof(move_tbl2) / sizeof(ENEMYTBL);
			break;
		case 2:
			g_Enemyhead[i].movepattern = move_tbl3;
			g_Enemyhead[i].size = sizeof(move_tbl3) / sizeof(ENEMYTBL);
			break;
		case 3:
			g_Enemyhead[i].movepattern = move_tbl4;
			g_Enemyhead[i].size = sizeof(move_tbl4) / sizeof(ENEMYTBL);
			break;
		case 4:
			g_Enemyhead[i].movepattern = move_tbl5;
			g_Enemyhead[i].size = sizeof(move_tbl5) / sizeof(ENEMYTBL);
			break;
		}

		// 位置・回転・スケールの初期化設定
		g_Enemyhead[i].pos = set_tbl_ENEMY[HEAD].pos;
		g_Enemyhead[i].rot = set_tbl_ENEMY[HEAD].rot;
		g_Enemyhead[i].scl = set_tbl_ENEMY[HEAD].scl;

		// 基準位置・回転・スケールの初期化設定
		g_Enemyhead[i].spos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemyhead[i].srot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemyhead[i].sscl = D3DXVECTOR3(0.5f, 0.5f, 0.5f);

		g_Enemyhead[i].move_time = 0.0f;
		g_Enemyhead[i].back_time = 0.0f;
		g_Enemyhead[i].spd = 1.0f;
		g_Enemyhead[i].following = false;
		g_Enemyhead[i].timestop = false;
		g_Enemyhead[i].oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemyhead[i].hit_oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemyhead[i].oldindex = 0;
		g_Enemyhead[i].pattern = 1;
		g_Enemyhead[i].parent = &g_Enemy[i];

		// 右腕
		g_EnemyarmR[i].use = false;			// 最初は未使用

		switch (i%ENEMY_MOVEPATTERN)
		{
		case 0:
			g_EnemyarmR[i].movepattern = move_tbl1;
			g_EnemyarmR[i].size = sizeof(move_tbl1) / sizeof(ENEMYTBL);
			break;
		case 1:
			g_EnemyarmR[i].movepattern = move_tbl2;
			g_EnemyarmR[i].size = sizeof(move_tbl2) / sizeof(ENEMYTBL);
			break;
		case 2:
			g_EnemyarmR[i].movepattern = move_tbl3;
			g_EnemyarmR[i].size = sizeof(move_tbl3) / sizeof(ENEMYTBL);
			break;
		case 3:
			g_EnemyarmR[i].movepattern = move_tbl4;
			g_EnemyarmR[i].size = sizeof(move_tbl4) / sizeof(ENEMYTBL);
			break;
		case 4:
			g_EnemyarmR[i].movepattern = move_tbl5;
			g_EnemyarmR[i].size = sizeof(move_tbl5) / sizeof(ENEMYTBL);
			break;
		}

		// 位置・回転・スケールの初期化設定
		g_EnemyarmR[i].pos = set_tbl_ENEMY[ARM_R].pos;
		g_EnemyarmR[i].rot = set_tbl_ENEMY[ARM_R].rot;
		g_EnemyarmR[i].scl = set_tbl_ENEMY[ARM_R].scl;

		// 基準位置・回転・スケールの初期化設定
		g_EnemyarmR[i].spos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemyarmR[i].srot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemyarmR[i].sscl = D3DXVECTOR3(0.5f, 0.5f, 0.5f);

		g_EnemyarmR[i].move_time = 0.0f;
		g_EnemyarmR[i].back_time = 0.0f;
		g_EnemyarmR[i].spd = 1.0f;
		g_EnemyarmR[i].following = false;
		g_EnemyarmR[i].timestop = false;
		g_EnemyarmR[i].oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemyarmR[i].hit_oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemyarmR[i].oldindex = 0;
		g_EnemyarmR[i].pattern = 1;
		g_EnemyarmR[i].parent = &g_Enemy[i];

		// 左腕
		g_EnemyarmL[i].use = false;			// 最初は未使用

		switch (i%ENEMY_MOVEPATTERN)
		{
		case 0:
			g_EnemyarmL[i].movepattern = move_tbl1;
			g_EnemyarmL[i].size = sizeof(move_tbl1) / sizeof(ENEMYTBL);
			break;
		case 1:
			g_EnemyarmL[i].movepattern = move_tbl2;
			g_EnemyarmL[i].size = sizeof(move_tbl2) / sizeof(ENEMYTBL);
			break;
		case 2:
			g_EnemyarmL[i].movepattern = move_tbl3;
			g_EnemyarmL[i].size = sizeof(move_tbl3) / sizeof(ENEMYTBL);
			break;
		case 3:
			g_EnemyarmL[i].movepattern = move_tbl4;
			g_EnemyarmL[i].size = sizeof(move_tbl4) / sizeof(ENEMYTBL);
			break;
		case 4:
			g_EnemyarmL[i].movepattern = move_tbl5;
			g_EnemyarmL[i].size = sizeof(move_tbl5) / sizeof(ENEMYTBL);
			break;
		}

		// 位置・回転・スケールの初期化設定
		g_EnemyarmL[i].pos = set_tbl_ENEMY[ARM_L].pos;
		g_EnemyarmL[i].rot = set_tbl_ENEMY[ARM_L].rot;
		g_EnemyarmL[i].scl = set_tbl_ENEMY[ARM_L].scl;

		// 基準位置・回転・スケールの初期化設定
		g_EnemyarmL[i].spos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemyarmL[i].srot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemyarmL[i].sscl = D3DXVECTOR3(0.5f, 0.5f, 0.5f);

		g_EnemyarmL[i].move_time = 0.0f;
		g_EnemyarmL[i].back_time = 0.0f;
		g_EnemyarmL[i].spd = 1.0f;
		g_EnemyarmL[i].following = false;
		g_EnemyarmL[i].timestop = false;
		g_EnemyarmL[i].oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemyarmL[i].hit_oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemyarmL[i].oldindex = 0;
		g_EnemyarmL[i].pattern = 1;
		g_EnemyarmL[i].parent = &g_Enemy[i];

		// 右足
		g_EnemylegR[i].use = false;			// 最初は未使用

		switch (i%ENEMY_MOVEPATTERN)
		{
		case 0:
			g_EnemylegR[i].movepattern = move_tbl1;
			g_EnemylegR[i].size = sizeof(move_tbl1) / sizeof(ENEMYTBL);
			break;
		case 1:
			g_EnemylegR[i].movepattern = move_tbl2;
			g_EnemylegR[i].size = sizeof(move_tbl2) / sizeof(ENEMYTBL);
			break;
		case 2:
			g_EnemylegR[i].movepattern = move_tbl3;
			g_EnemylegR[i].size = sizeof(move_tbl3) / sizeof(ENEMYTBL);
			break;
		case 3:
			g_EnemylegR[i].movepattern = move_tbl4;
			g_EnemylegR[i].size = sizeof(move_tbl4) / sizeof(ENEMYTBL);
			break;
		case 4:
			g_EnemylegR[i].movepattern = move_tbl5;
			g_EnemylegR[i].size = sizeof(move_tbl5) / sizeof(ENEMYTBL);
			break;
		}

		// 位置・回転・スケールの初期化設定
		g_EnemylegR[i].pos = set_tbl_ENEMY[LEG_R].pos;
		g_EnemylegR[i].rot = set_tbl_ENEMY[LEG_R].rot;
		g_EnemylegR[i].scl = set_tbl_ENEMY[LEG_R].scl;

		// 基準位置・回転・スケールの初期化設定
		g_EnemylegR[i].spos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemylegR[i].srot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemylegR[i].sscl = D3DXVECTOR3(0.5f, 0.5f, 0.5f);

		g_EnemylegR[i].move_time = 0.0f;
		g_EnemylegR[i].back_time = 0.0f;
		g_EnemylegR[i].spd = 1.0f;
		g_EnemylegR[i].following = false;
		g_EnemylegR[i].timestop = false;
		g_EnemylegR[i].oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemylegR[i].hit_oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemylegR[i].oldindex = 0;
		g_EnemylegR[i].pattern = 1;
		g_EnemylegR[i].parent = &g_Enemy[i];

		// 左足
		g_EnemylegL[i].use = false;			// 最初は未使用

		switch (i%ENEMY_MOVEPATTERN)
		{
		case 0:
			g_EnemylegL[i].movepattern = move_tbl1;
			g_EnemylegL[i].size = sizeof(move_tbl1) / sizeof(ENEMYTBL);
			break;
		case 1:
			g_EnemylegL[i].movepattern = move_tbl2;
			g_EnemylegL[i].size = sizeof(move_tbl2) / sizeof(ENEMYTBL);
			break;
		case 2:
			g_EnemylegL[i].movepattern = move_tbl3;
			g_EnemylegL[i].size = sizeof(move_tbl3) / sizeof(ENEMYTBL);
			break;
		case 3:
			g_EnemylegL[i].movepattern = move_tbl4;
			g_EnemylegL[i].size = sizeof(move_tbl4) / sizeof(ENEMYTBL);
			break;
		case 4:
			g_EnemylegL[i].movepattern = move_tbl5;
			g_EnemylegL[i].size = sizeof(move_tbl5) / sizeof(ENEMYTBL);
			break;
		}
		// 位置・回転・スケールの初期化設定
		g_EnemylegL[i].pos = set_tbl_ENEMY[LEG_L].pos;
		g_EnemylegL[i].rot = set_tbl_ENEMY[LEG_L].rot;
		g_EnemylegL[i].scl = set_tbl_ENEMY[LEG_L].scl;

		// 基準位置・回転・スケールの初期化設定
		g_EnemylegL[i].spos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemylegL[i].srot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemylegL[i].sscl = D3DXVECTOR3(0.5f, 0.5f, 0.5f);

		g_EnemylegL[i].spos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemylegL[i].move_time = 0.0f;
		g_EnemylegL[i].back_time = 0.0f;
		g_EnemylegL[i].spd = 2.0f;
		g_EnemylegL[i].following = false;
		g_EnemylegL[i].timestop = false;
		g_EnemylegL[i].oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemylegL[i].hit_oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemylegL[i].oldindex = 0;
		g_EnemylegL[i].pattern = 1;
		g_EnemylegL[i].parent = &g_Enemy[i];
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (enemy_pD3DTexture_body != NULL)
		{// テクスチャの開放
			enemy_pD3DTexture_body->Release();
			enemy_pD3DTexture_body = NULL;
		}

		if (enemy_pD3DXMesh_body != NULL)
		{// メッシュの開放
			enemy_pD3DXMesh_body->Release();
			enemy_pD3DXMesh_body = NULL;
		}

		if (enemy_pD3DXBuffMat_body != NULL)
		{//マテリアルの開放
			enemy_pD3DXBuffMat_body->Release();
			enemy_pD3DXBuffMat_body = NULL;
		}

		if (enemy_pD3DTexture_head != NULL)
		{// テクスチャの開放
			enemy_pD3DTexture_head->Release();
			enemy_pD3DTexture_head = NULL;
		}
		if (enemy_pD3DXMesh_head != NULL)
		{// メッシュの開放
			enemy_pD3DXMesh_head->Release();
			enemy_pD3DXMesh_head = NULL;
		}
		if (enemy_pD3DXBuffMat_head != NULL)
		{// マテリアルの開放
			enemy_pD3DXBuffMat_head->Release();
			enemy_pD3DXBuffMat_head = NULL;
		}

		if (enemy_pD3DTexture_armR != NULL)
		{// テクスチャの開放
			enemy_pD3DTexture_armR->Release();
			enemy_pD3DTexture_armR = NULL;
		}
		if (enemy_pD3DXMesh_armR != NULL)
		{// メッシュの開放
			enemy_pD3DXMesh_armR->Release();
			enemy_pD3DXMesh_armR = NULL;
		}
		if (enemy_pD3DXBuffMat_armR != NULL)
		{// マテリアルの開放
			enemy_pD3DXBuffMat_armR->Release();
			enemy_pD3DXBuffMat_armR = NULL;
		}

		if (enemy_pD3DTexture_armL != NULL)
		{// テクスチャの開放
			enemy_pD3DTexture_armL->Release();
			enemy_pD3DTexture_armL = NULL;
		}
		if (enemy_pD3DXMesh_armL != NULL)
		{// メッシュの開放
			enemy_pD3DXMesh_armL->Release();
			enemy_pD3DXMesh_armL = NULL;
		}
		if (enemy_pD3DXBuffMat_armL != NULL)
		{// マテリアルの開放
			enemy_pD3DXBuffMat_armL->Release();
			enemy_pD3DXBuffMat_armL = NULL;
		}

		if (enemy_pD3DTexture_legR != NULL)
		{// テクスチャの開放
			enemy_pD3DTexture_legR->Release();
			enemy_pD3DTexture_legR = NULL;
		}
		if (enemy_pD3DXMesh_legR != NULL)
		{// メッシュの開放
			enemy_pD3DXMesh_legR->Release();
			enemy_pD3DXMesh_legR = NULL;
		}
		if (enemy_pD3DXBuffMat_legR != NULL)
		{// マテリアルの開放
			enemy_pD3DXBuffMat_legR->Release();
			enemy_pD3DXBuffMat_legR = NULL;
		}

		if (enemy_pD3DTexture_legL != NULL)
		{// テクスチャの開放
			enemy_pD3DTexture_legL->Release();
			enemy_pD3DTexture_legL = NULL;
		}
		if (enemy_pD3DXMesh_legL != NULL)
		{// メッシュの開放
			enemy_pD3DXMesh_legL->Release();
			enemy_pD3DXMesh_legL = NULL;
		}
		if (enemy_pD3DXBuffMat_legL != NULL)
		{// マテリアルの開放
			enemy_pD3DXBuffMat_legL->Release();
			enemy_pD3DXBuffMat_legL = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	PLAYER *player = GetPlayer();

	int cnt = 0;

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].status != DEAD)	//エネミーが死んでいなければ
			if (g_Enemy[i].use == true)  //エネミーがいるなら
			{
				if (g_Enemy[i].status != HIT)		// エネミーが攻撃を受けていなければ
				{
					cnt++;					//生きているエネミーをカウント

					switch (g_Enemy[i].pattern) //エネミーの状態遷移のswitch
					{
					case 1://線形補間（索敵）
							//移動処理
					{
						int	index = (int)g_Enemy[i].move_time;			//線形補間のテーブルのindex
						float	time = g_Enemy[i].move_time - index;	//線形補間の処理割合(0.0〜0.99....)

						g_Enemy[i].move_time += g_Enemy[i].movepattern[index].dt;	//アニメーションの合計時間に足す
						if (index > (g_Enemy[i].size - 2))							// ゴールへオーバーしていたら、ゴールへ戻す
						{
							g_Enemy[i].move_time = 0.0f;
							index = 0;
						}

						// X座標を求める	X = StartX + (EndX - StartX) * 今の時間
						D3DXVECTOR3 vec = g_Enemy[i].movepattern[index + 1].pos - g_Enemy[i].movepattern[index].pos;
						g_Enemy[i].pos = g_Enemy[i].movepattern[index].pos + (vec * time) + g_Enemy[i].spos;

						//回転を求める　　　R = StsrtX + (EndX - startX) * 今の時間
						D3DXVECTOR3 rot = g_Enemy[i].movepattern[index + 1].rot - g_Enemy[i].movepattern[index].rot;
						g_Enemy[i].rot = g_Enemy[i].movepattern[index].rot + (rot * time) + g_Enemy[i].srot;

						//scaleを求める     S = StsrtX + (EndX - startX) * 今の時間
						D3DXVECTOR3 scl = g_Enemy[i].movepattern[index + 1].scl - g_Enemy[i].movepattern[index].scl;
						g_Enemy[i].scl = g_Enemy[i].movepattern[index].scl + (scl * time) + g_Enemy[i].sscl;

						if (CheckHitFP(player->pos, g_Enemy[i].pos, D3DXVECTOR3(-sinf(g_Enemy[i].rot.y), 0, -cosf(g_Enemy[i].rot.y)), 30, 300, 0) == true)				//プレイヤーを見つけたら）時間を止めて敵の座標を保存
						{
							g_Enemy[i].timestop = true;						//追従をOFFにする

							g_Enemy[i].oldpos = g_Enemy[i].pos;				//oldposにエネミー座標を保存

							g_Enemy[i].following = true;					//追従をONにする

							g_Enemy[i].pattern = 2;							//追従パターンに移る
						}
					}
					break;
					case 2://追従
					{
						D3DXVECTOR3 pos1 = player->pos - g_Enemy[i].pos;	//プレイヤーの座標からエネミーの座標を引いたものをpos１に入れる				

						//索敵範囲外に行ったら追うのをやめる
						if (Length(pos1) > ENEMY_SCOUTING)
						{
							g_Enemy[i].pattern = 3;
							break;
						}
						//近づいたら戻る
						if ((Length(pos1) < 40) && ((Length(pos1) > 5)))
						{
							g_Enemy[i].attackcnt++;					//エネミーが攻撃開始するまでの時間をカウント
							if (g_Enemy[i].attackcnt > 30)
							{
								g_Enemy[i].status = ATTACK;			//エネミーが攻撃モードになる
							}
							break;
						}

						//索敵範囲内の時は追う
						if (Length(pos1) <= ENEMY_SCOUTING)
						{
							//追従終了したとき元の座標へ戻るための線形補間用のback_timeを初期化
							g_Enemy[i].back_time = 0.00f;

							//追従処理開始//											
							float rot1 = atan2f(pos1.z, pos1.x);

							rot1 = (D3DX_PI / 2) * 3 - rot1;
							g_Enemy[i].rot.y = rot1;

							pos1.y = 0;
							float len = Length(pos1);

							len = 1 / len;
							pos1.x *= len;
							pos1.z *= len;
							g_Enemy[i].pos += pos1 * g_Enemy[i].spd;
								//追従処理終了//
						}
					}
					break;
					case 3://元の座標に戻る

						float	back_dt = 0.01f;
						g_Enemy[i].following = false;									//追従をOFF

						D3DXVECTOR3 pos2 = g_Enemy[i].oldpos - g_Enemy[i].pos;			//現posから戻る場所への距離
						pos2.y = 0;

						g_Enemy[i].pos = g_Enemy[i].pos + pos2 * g_Enemy[i].back_time;	//oldposへ向けて線形補間
						g_Enemy[i].back_time += back_dt;								//back_time更新

						//エネミーをoldposへ向かせる
						float rot1 = atan2f(pos2.z, pos2.x);
						rot1 = (D3DX_PI / 2) * 3 - rot1;
						g_Enemy[i].rot.y = rot1;

						if (g_Enemy[i].back_time >= 1.0)								//oldposへの線形補間がおわったら状態遷移
						{
							g_Enemy[i].timestop = false;
							g_Enemy[i].pattern = 1;
						}
						break;
					}

					// エネミーが歩くモーション
					if (g_Enemy[i].status == WALK)
					{
						float	dt = 0.03f;
						g_Enemy[i].walkmove_time += dt;		// アニメーションの合計時間に足す

						int index = (int)g_Enemy[i].walkmove_time;
						float	time = g_Enemy[i].walkmove_time - index;
						int		size = sizeof(Ebody_walk_tbl) / sizeof(D3DXVECTOR3); //テーブル全体のバイト数をテーブル一つのバイト数で割る(テーブル数を求める)

						if (index > (size - 2))	// ゴールをオーバーしていたら、ゴールへ戻す
						{
							g_Enemy[i].walkmove_time = 0.0f;
							index = 0;
						}

						// アニメーションさせる部位	　X = StartX + (EndX - StartX) * 今の時間
						g_Enemy[i].vec = Ebody_walk_tbl[index + 1] - Ebody_walk_tbl[index];
						g_Enemyhead[i].vec = Ehead_walk_tbl[index + 1] - Ehead_walk_tbl[index];
						g_EnemyarmR[i].vec = EarmR_walk_tbl[index + 1] - EarmR_walk_tbl[index];
						g_EnemyarmL[i].vec = EarmL_walk_tbl[index + 1] - EarmL_walk_tbl[index];
						g_EnemylegR[i].vec = ElegR_walk_tbl[index + 1] - ElegR_walk_tbl[index];
						g_EnemylegL[i].vec = ElegL_walk_tbl[index + 1] - ElegL_walk_tbl[index];

						g_Enemy[i].rot = Ebody_walk_tbl[index] + g_Enemy[i].vec * time;
						g_Enemyhead[i].rot = Ehead_walk_tbl[index] + g_Enemyhead[i].vec * time;
						g_EnemyarmR[i].rot = EarmR_walk_tbl[index] + g_EnemyarmR[i].vec * time;
						g_EnemyarmL[i].rot = EarmL_walk_tbl[index] + g_EnemyarmL[i].vec * time;
						g_EnemylegR[i].rot = ElegR_walk_tbl[index] + g_EnemylegR[i].vec * time;
						g_EnemylegL[i].rot = ElegL_walk_tbl[index] + g_EnemylegL[i].vec * time;
					}

					// エネミーが攻撃しているアニメーション
					if (g_Enemy[i].status == ATTACK)
					{
						g_Enemy[i].attack_dt += 0.01f;
						g_Enemy[i].attackmove_time += g_Enemy[i].attack_dt;		// アニメーションの合計時間に足す

						int a_index = (int)g_Enemy[i].attackmove_time;
						float	a_time = g_Enemy[i].attackmove_time - a_index;
						int		a_size = sizeof(Ebody_Attack_tbl) / sizeof(D3DXVECTOR3); //テーブル全体のバイト数をテーブル一つのバイト数で割る(テーブル数を求める)

						if (a_index > (a_size - 2))	// ゴールをオーバーしていたら、ゴールへ戻す
						{
							g_Enemy[i].status = WALK;
							g_Enemy[i].pattern = 3;
							g_Enemy[i].attackmove_time = 0.0f;
							a_index = 0;
							g_Enemy[i].attackcnt = 0;
						}

						// アニメーションさせる部位	　X = StartX + (EndX - StartX) * 今の時間
						g_Enemy[i].vec = Ebody_Attack_tbl[a_index + 1] - Ebody_Attack_tbl[a_index];
						g_Enemyhead[i].vec = Ehead_Attack_tbl[a_index + 1] - Ehead_Attack_tbl[a_index];
						g_EnemyarmR[i].vec = EarmR_Attack_tbl[a_index + 1] - EarmR_Attack_tbl[a_index];
						g_EnemyarmL[i].vec = EarmL_Attack_tbl[a_index + 1] - EarmL_Attack_tbl[a_index];
						g_EnemylegR[i].vec = ElegR_Attack_tbl[a_index + 1] - ElegR_Attack_tbl[a_index];
						g_EnemylegL[i].vec = ElegL_Attack_tbl[a_index + 1] - ElegL_Attack_tbl[a_index];

						g_Enemy[i].rot = Ebody_Attack_tbl[a_index] + g_Enemy[i].vec * a_time;
						g_Enemyhead[i].rot = Ehead_Attack_tbl[a_index] + g_Enemyhead[i].vec * a_time;
						g_EnemyarmR[i].rot = EarmR_Attack_tbl[a_index] + g_EnemyarmR[i].vec * a_time;
						g_EnemyarmL[i].rot = EarmL_Attack_tbl[a_index] + g_EnemyarmL[i].vec * a_time;
						g_EnemylegR[i].rot = ElegR_Attack_tbl[a_index] + g_EnemylegR[i].vec * a_time;
						g_EnemylegL[i].rot = ElegL_Attack_tbl[a_index] + g_EnemylegL[i].vec * a_time;

					}
				}
			}
		//エネミーが攻撃を受けた時のアニメーション（吹っ飛ぶ）
			else if (g_Enemy[i].status == HIT)
			{
				g_Enemy[i].formation = 10;
				g_Enemy[i].hit_dt += 0.003f;
				g_Enemy[i].hitmove_time += g_Enemy[i].hit_dt;		// アニメーションの合計時間に足す

				int a_index = (int)g_Enemy[i].hitmove_time;
				float	a_time = g_Enemy[i].hitmove_time - a_index;
				int		a_size = sizeof(Ebody_Hit_tbl) / sizeof(D3DXVECTOR3); //テーブル全体のバイト数をテーブル一つのバイト数で割る(テーブル数を求める)

				if (a_index > (a_size - 2))	// ゴールをオーバーしていたら、ゴールへ戻す
				{
					g_Enemy[i].hitmove_time = 0.0f;
					a_index = 0;
				}

				// アニメーションさせる部位	　X = StartX + (EndX - StartX) * 今の時間
				g_Enemy[i].vec = Ebody_Hit_tbl[a_index + 1] - Ebody_Hit_tbl[a_index];
				g_Enemyhead[i].vec = Ehead_Hit_tbl[a_index + 1] - Ehead_Hit_tbl[a_index];
				g_EnemyarmR[i].vec = EarmR_Hit_tbl[a_index + 1] - EarmR_Hit_tbl[a_index];
				g_EnemyarmL[i].vec = EarmL_Hit_tbl[a_index + 1] - EarmL_Hit_tbl[a_index];
				g_EnemylegR[i].vec = ElegR_Hit_tbl[a_index + 1] - ElegR_Hit_tbl[a_index];
				g_EnemylegL[i].vec = ElegL_Hit_tbl[a_index + 1] - ElegL_Hit_tbl[a_index];

				g_Enemy[i].rot = Ebody_Hit_tbl[a_index] + g_Enemy[i].vec * a_time;
				g_Enemyhead[i].rot = Ehead_Hit_tbl[a_index] + g_Enemyhead[i].vec * a_time;
				g_EnemyarmR[i].rot = EarmR_Hit_tbl[a_index] + g_EnemyarmR[i].vec * a_time;
				g_EnemyarmL[i].rot = EarmL_Hit_tbl[a_index] + g_EnemyarmL[i].vec * a_time;
				g_EnemylegR[i].rot = ElegR_Hit_tbl[a_index] + g_EnemylegR[i].vec * a_time;
				g_EnemylegL[i].rot = ElegL_Hit_tbl[a_index] + g_EnemylegL[i].vec * a_time;

				if (g_Enemy[i].hit_cnt < g_Enemy[i].hit_cnt_max)
				{
					float rag = D3DX_PI / g_Enemy[i].hit_cnt_max;
					float angle = rag * g_Enemy[i].hit_cnt;
					g_Enemy[i].ofs.x = -sinf(rand() % 100 + 1) * g_Enemy[i].hit;
					g_Enemy[i].ofs.y = sinf(angle) * g_Enemy[i].hit;
					g_Enemy[i].ofs.z = cosf((rand() % 100 + 1)) * g_Enemy[i].hit;
					g_Enemy[i].hit_cnt++;
					g_Enemy[i].pos.x += g_Enemy[i].ofs.x + g_Enemy[i].spd;
					g_Enemy[i].pos.y += g_Enemy[i].ofs.y + g_Enemy[i].spd;
					g_Enemy[i].pos.z += g_Enemy[i].ofs.z + g_Enemy[i].spd;
				}
				if (g_Enemy[i].hit_cnt >= g_Enemy[i].hit_cnt_max)		// 吹っ飛ぶの終了？
				{														// Yes
					g_Enemy[i].status = DEAD;							//死亡
					g_Enemy[i].ofs = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					g_Enemy[i].pos.y = 0.0f;
				}
			}
		for (int j = 0; j < ENEMY_GROUP; j++)			//影の設定
		{
			// 影もプレイヤーの位置に合わせる
			g_Shadowpos[j] = g_Enemy[i].pos + (formation[j] * j);
			g_Shadowpos[j].y = 0.0f;
			SetPositionShadow(g_Enemy[i].shadowIdx[j], g_Shadowpos[j], g_Enemy[i].scl);
		}
	}

	if (cnt < MAX_ENEMY / 3)							//エネミー数が3分の1未満ならエネミーを発生させる
	{
		SetEnemy(MAX_ENEMY, D3DXVECTOR3(-1500, 0, 1500), D3DXVECTOR3(1500, 0, -1500));
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//使われてないならコンテニューする
		if (g_Enemy[i].status == DEAD) continue;

		for (int j = 0; j < ENEMY_GROUP; j++)			// ひとかたまり分を描画処理
		{
			// 体以外は先に計算してしまう
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Enemyhead[i].mtxWorld);
			D3DXMatrixIdentity(&g_EnemyarmR[i].mtxWorld);
			D3DXMatrixIdentity(&g_EnemyarmL[i].mtxWorld);
			D3DXMatrixIdentity(&g_EnemylegR[i].mtxWorld);
			D3DXMatrixIdentity(&g_EnemylegL[i].mtxWorld);

			//スケールの反映
			D3DXMatrixScaling(&g_Enemyhead[i].mtxScl, g_Enemyhead[i].scl.x, g_Enemyhead[i].scl.y, g_Enemyhead[i].scl.z);
			D3DXMatrixMultiply(&g_Enemyhead[i].mtxWorld, &g_Enemyhead[i].mtxWorld, &g_Enemyhead[i].mtxScl);
			D3DXMatrixScaling(&g_EnemyarmR[i].mtxScl, g_EnemyarmR[i].scl.x, g_EnemyarmR[i].scl.y, g_EnemyarmR[i].scl.z);
			D3DXMatrixMultiply(&g_EnemyarmR[i].mtxWorld, &g_EnemyarmR[i].mtxWorld, &g_EnemyarmR[i].mtxScl);
			D3DXMatrixScaling(&g_EnemyarmL[i].mtxScl, g_EnemyarmL[i].scl.x, g_EnemyarmL[i].scl.y, g_EnemyarmL[i].scl.z);
			D3DXMatrixMultiply(&g_EnemyarmL[i].mtxWorld, &g_EnemyarmL[i].mtxWorld, &g_EnemyarmL[i].mtxScl);
			D3DXMatrixScaling(&g_EnemylegR[i].mtxScl, g_EnemylegR[i].scl.x, g_EnemylegR[i].scl.y, g_EnemylegR[i].scl.z);
			D3DXMatrixMultiply(&g_EnemylegR[i].mtxWorld, &g_EnemylegR[i].mtxWorld, &g_EnemylegR[i].mtxScl);
			D3DXMatrixScaling(&g_EnemylegL[i].mtxScl, g_EnemylegL[i].scl.x, g_EnemylegL[i].scl.y, g_EnemylegL[i].scl.z);
			D3DXMatrixMultiply(&g_EnemylegL[i].mtxWorld, &g_EnemylegL[i].mtxWorld, &g_EnemylegL[i].mtxScl);

			//回転を反映
			D3DXMatrixRotationYawPitchRoll(&g_Enemyhead[i].mtxRot, g_Enemyhead[i].rot.y, g_Enemyhead[i].rot.x, g_Enemyhead[i].rot.z);
			D3DXMatrixMultiply(&g_Enemyhead[i].mtxWorld, &g_Enemyhead[i].mtxWorld, &g_Enemyhead[i].mtxRot);
			D3DXMatrixRotationYawPitchRoll(&g_EnemyarmR[i].mtxRot, g_EnemyarmR[i].rot.y, g_EnemyarmR[i].rot.x, g_EnemyarmR[i].rot.z);
			D3DXMatrixMultiply(&g_EnemyarmR[i].mtxWorld, &g_EnemyarmR[i].mtxWorld, &g_EnemyarmR[i].mtxRot);
			D3DXMatrixRotationYawPitchRoll(&g_EnemyarmL[i].mtxRot, g_EnemyarmL[i].rot.y, g_EnemyarmL[i].rot.x, g_EnemyarmL[i].rot.z);
			D3DXMatrixMultiply(&g_EnemyarmL[i].mtxWorld, &g_EnemyarmL[i].mtxWorld, &g_EnemyarmL[i].mtxRot);
			D3DXMatrixRotationYawPitchRoll(&g_EnemylegR[i].mtxRot, g_EnemylegR[i].rot.y, g_EnemylegR[i].rot.x, g_EnemylegR[i].rot.z);
			D3DXMatrixMultiply(&g_EnemylegR[i].mtxWorld, &g_EnemylegR[i].mtxWorld, &g_EnemylegR[i].mtxRot);
			D3DXMatrixRotationYawPitchRoll(&g_EnemylegL[i].mtxRot, g_EnemylegL[i].rot.y, g_EnemylegL[i].rot.x, g_EnemylegL[i].rot.z);
			D3DXMatrixMultiply(&g_EnemylegL[i].mtxWorld, &g_EnemylegL[i].mtxWorld, &g_EnemylegL[i].mtxRot);

			//移動を反映
			D3DXMatrixTranslation(&g_Enemyhead[i].mtxTranslate, g_Enemyhead[i].pos.x, g_Enemyhead[i].pos.y, g_Enemyhead[i].pos.z);
			D3DXMatrixMultiply(&g_Enemyhead[i].mtxWorld, &g_Enemyhead[i].mtxWorld, &g_Enemyhead[i].mtxTranslate);
			D3DXMatrixTranslation(&g_EnemyarmR[i].mtxTranslate, g_EnemyarmR[i].pos.x, g_EnemyarmR[i].pos.y, g_EnemyarmR[i].pos.z);
			D3DXMatrixMultiply(&g_EnemyarmR[i].mtxWorld, &g_EnemyarmR[i].mtxWorld, &g_EnemyarmR[i].mtxTranslate);
			D3DXMatrixTranslation(&g_EnemyarmL[i].mtxTranslate, g_EnemyarmL[i].pos.x, g_EnemyarmL[i].pos.y, g_EnemyarmL[i].pos.z);
			D3DXMatrixMultiply(&g_EnemyarmL[i].mtxWorld, &g_EnemyarmL[i].mtxWorld, &g_EnemyarmL[i].mtxTranslate);
			D3DXMatrixTranslation(&g_EnemylegR[i].mtxTranslate, g_EnemylegR[i].pos.x, g_EnemylegR[i].pos.y, g_EnemylegR[i].pos.z);
			D3DXMatrixMultiply(&g_EnemylegR[i].mtxWorld, &g_EnemylegR[i].mtxWorld, &g_EnemylegR[i].mtxTranslate);
			D3DXMatrixTranslation(&g_EnemylegL[i].mtxTranslate, g_EnemylegL[i].pos.x, g_EnemylegL[i].pos.y, g_EnemylegL[i].pos.z);
			D3DXMatrixMultiply(&g_EnemylegL[i].mtxWorld, &g_EnemylegL[i].mtxWorld, &g_EnemylegL[i].mtxTranslate);

			//体
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Enemy[i].mtxWorld);

			//スケールの反映
			D3DXMatrixScaling(&g_Enemy[i].mtxScl, g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
			D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &g_Enemy[i].mtxScl);

			//回転を反映
			D3DXMatrixRotationYawPitchRoll(&g_Enemy[i].mtxRot, g_Enemy[i].rot.y, g_Enemy[i].rot.x, g_Enemy[i].rot.z);
			D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &g_Enemy[i].mtxRot);

			//移動を反映（インスタンシング）
			D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			switch (g_Enemy[i].formation)
			{
			case 0:
				pos = g_Enemy[i].pos + (formation[j] * j);
				D3DXMatrixTranslation(&g_Enemy[i].mtxTranslate, pos.x, pos.y, pos.z);
				D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &g_Enemy[i].mtxTranslate);
				break;

			case 1:
				pos = g_Enemy[i].pos + (formation[g_Enemy[i].formation] * j);
				D3DXMatrixTranslation(&g_Enemy[i].mtxTranslate, pos.x, pos.y, pos.z);
				D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &g_Enemy[i].mtxTranslate);
				break;

			case 2:
				pos = g_Enemy[i].pos + (formation[g_Enemy[i].formation] * j);
				D3DXMatrixTranslation(&g_Enemy[i].mtxTranslate, pos.x, pos.y, pos.z);
				D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &g_Enemy[i].mtxTranslate);
				break;

			case 3:
				pos = g_Enemy[i].pos + (formation[g_Enemy[i].formation] * j);
				D3DXMatrixTranslation(&g_Enemy[i].mtxTranslate, pos.x, pos.y, pos.z);
				D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &g_Enemy[i].mtxTranslate);
				break;

			case 4:
				pos = g_Enemy[i].pos + (formation[g_Enemy[i].formation] * j);
				D3DXMatrixTranslation(&g_Enemy[i].mtxTranslate, pos.x, pos.y, pos.z);
				D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &g_Enemy[i].mtxTranslate);
				break;

			case 10:
				switch (j)
				{
				case 0:
					pos = g_Enemy[i].pos + (formation1[j] * j);
					D3DXMatrixTranslation(&g_Enemy[i].mtxTranslate, pos.x, pos.y, pos.z);
					D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &g_Enemy[i].mtxTranslate);
					break;
				case 1:
					pos = g_Enemy[i].pos + (formation1[j] * j);
					D3DXMatrixTranslation(&g_Enemy[i].mtxTranslate, pos.x, pos.y, pos.z);
					D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &g_Enemy[i].mtxTranslate);
					break;
				case 2:
					pos = g_Enemy[i].pos + (formation1[j] * j);
					D3DXMatrixTranslation(&g_Enemy[i].mtxTranslate, pos.x, pos.y, pos.z);
					D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &g_Enemy[i].mtxTranslate);
					break;
				case 3:
					pos = g_Enemy[i].pos + (formation1[j] * j);
					D3DXMatrixTranslation(&g_Enemy[i].mtxTranslate, pos.x, pos.y, pos.z);
					D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &g_Enemy[i].mtxTranslate);
					break;
				case 4:
					pos = g_Enemy[i].pos + (formation1[j] * j);
					D3DXMatrixTranslation(&g_Enemy[i].mtxTranslate, pos.x, pos.y, pos.z);
					D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &g_Enemy[i].mtxTranslate);
					break;
				case 5:
					pos = g_Enemy[i].pos + (formation1[j] * j);
					D3DXMatrixTranslation(&g_Enemy[i].mtxTranslate, pos.x, pos.y, pos.z);
					D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &g_Enemy[i].mtxTranslate);
					break;
				}
			}

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Enemy[i].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&g_Enemy[i].matDef);

			// マテリアル情報に対するポインタを取得
			g_Enemy[i].pD3DXMat = (D3DXMATERIAL*)enemy_pD3DXBuffMat_body->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)enemy_nNumMat_body; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&g_Enemy[i].pD3DXMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, enemy_pD3DTexture_body);

				//描画
				enemy_pD3DXMesh_body->DrawSubset(nCntMat);
			}

			//マテリアルをデフォルメに戻す
			pDevice->SetMaterial(&g_Enemy[i].matDef);

			//頭
			//子のworld = 子のworld * 親のworld
			D3DXMatrixMultiply(&g_Enemyhead[i].mtxWorld, &g_Enemyhead[i].mtxWorld, &g_Enemyhead[i].parent->mtxWorld);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Enemyhead[i].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&g_Enemyhead[i].matDef);

			// マテリアル情報に対するポインタを取得
			g_Enemyhead[i].pD3DXMat = (D3DXMATERIAL*)enemy_pD3DXBuffMat_head->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)enemy_nNumMat_head; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&g_Enemyhead[i].pD3DXMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, enemy_pD3DTexture_head);

				//描画
				enemy_pD3DXMesh_head->DrawSubset(nCntMat);
			}

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&g_Enemyhead[i].matDef);

			//右腕
			//子のworld = 子のworld * 親のworld
			D3DXMatrixMultiply(&g_EnemyarmR[i].mtxWorld, &g_EnemyarmR[i].mtxWorld, &g_EnemyarmR[i].parent->mtxWorld);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_EnemyarmR[i].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&g_EnemyarmR[i].matDef);

			// マテリアル情報に対するポインタを取得
			g_EnemyarmR[i].pD3DXMat = (D3DXMATERIAL*)enemy_pD3DXBuffMat_armR->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)enemy_nNumMat_armR; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&g_EnemyarmR[i].pD3DXMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, enemy_pD3DTexture_armR);

				//描画
				enemy_pD3DXMesh_armR->DrawSubset(nCntMat);
			}

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&g_EnemyarmR[i].matDef);

			//左腕
			//子のworld = 子のworld * 親のworld
			D3DXMatrixMultiply(&g_EnemyarmL[i].mtxWorld, &g_EnemyarmL[i].mtxWorld, &g_EnemyarmL[i].parent->mtxWorld);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_EnemyarmL[i].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&g_EnemyarmL[i].matDef);

			// マテリアル情報に対するポインタを取得
			g_EnemyarmL[i].pD3DXMat = (D3DXMATERIAL*)enemy_pD3DXBuffMat_armL->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)enemy_nNumMat_armL; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&g_EnemyarmL[i].pD3DXMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, enemy_pD3DTexture_armL);

				//描画
				enemy_pD3DXMesh_armL->DrawSubset(nCntMat);
			}

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&g_EnemyarmL[i].matDef);

			//右腕
			//子のworld = 子のworld * 親のworld
			D3DXMatrixMultiply(&g_EnemylegR[i].mtxWorld, &g_EnemylegR[i].mtxWorld, &g_EnemylegR[i].parent->mtxWorld);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_EnemylegR[i].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&g_EnemylegR[i].matDef);

			// マテリアル情報に対するポインタを取得
			g_EnemylegR[i].pD3DXMat = (D3DXMATERIAL*)enemy_pD3DXBuffMat_legR->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)enemy_nNumMat_legR; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&g_EnemylegR[i].pD3DXMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, enemy_pD3DTexture_legR);

				//描画
				enemy_pD3DXMesh_legR->DrawSubset(nCntMat);
			}

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&g_EnemylegR[i].matDef);

			//左足
			//子のworld = 子のworld * 親のworld
			D3DXMatrixMultiply(&g_EnemylegL[i].mtxWorld, &g_EnemylegL[i].mtxWorld, &g_EnemylegL[i].parent->mtxWorld);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_EnemylegL[i].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&g_EnemylegL[i].matDef);

			// マテリアル情報に対するポインタを取得
			g_EnemylegL[i].pD3DXMat = (D3DXMATERIAL*)enemy_pD3DXBuffMat_legL->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)enemy_nNumMat_legL; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&g_EnemylegL[i].pD3DXMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, enemy_pD3DTexture_legL);

				//描画
				enemy_pD3DXMesh_legL->DrawSubset(nCntMat);
			}

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&g_EnemylegL[i].matDef);
		}
	}
}

//=============================================================================
//バトルエネミーの取得
//=============================================================================
ENEMY *GetEnemy()
{
	return &(g_Enemy[0]);
}

//=============================================================================
// バトルエネミーを配置
//=============================================================================
void SetEnemy(int enemy_spawn, D3DXVECTOR3 upperleft, D3DXVECTOR3 bottomright)
{
	ITEM *item = GetItem();

	for (int a = 0; a < enemy_spawn; a++)
	{
		for (int i = 0; i < MAX_ENEMY; i++)
		{
			if (g_Enemy[i].use == false)
			{
				g_Enemy[i].use = true;						// エネミーを使用中にする
				g_Enemy[i].status = WALK;					// エネミーのステータスを初期化
				RefreshEnemy(i, upperleft, bottomright);	// エネミーの配置

				if ((item + i)->bUse == true)				// アイテムが使用中なら
				{
					DeleteItem(i);							// 消してしまう

					for (int j = 0; j < MAX_PARTICLE; j++)
					{
						GetParticle(i, j)->bUse = false;	// パーティクル終了
					}
				}
				break;
			}
		}
	}
	return;
}
//=============================================================================
// バトルエネミーを再配置
// reflesh_enemy	: 再配置する数
// upperleft		: 出現する位置（左上）
// bottomright		: 出現する位置（右下）
//=============================================================================
void RefreshEnemy(int refresh_enemy, D3DXVECTOR3 upperleft, D3DXVECTOR3 bottomright)
{
	g_Enemy[refresh_enemy].spos.x = (rand() % (int)((bottomright.x - upperleft.x + 1)) + upperleft.x);
	g_Enemy[refresh_enemy].spos.z = (rand() % (int)((upperleft.z - bottomright.z + 1)) + bottomright.z);
}



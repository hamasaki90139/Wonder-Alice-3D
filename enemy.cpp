//=============================================================================
//
// ƒGƒlƒ~[ƒ‚ƒfƒ‹ˆ—[enemy.cpp]
// Author : à_è ¬t
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
//ƒvƒƒgƒ^ƒCƒvéŒ¾
//*****************************************************************************


//*****************************************************************************
// ƒOƒ[ƒoƒ‹éŒ¾
//*****************************************************************************

static ENEMY				g_Enemy[MAX_ENEMY];							// ƒGƒlƒ~[
static ENEMY				g_Enemyhead[MAX_ENEMY];						// ƒvƒŒƒCƒ„[‚Ì“ª•”
static ENEMY				g_EnemyarmR[MAX_ENEMY];                     // ƒvƒŒƒCƒ„[‚Ì˜r
static ENEMY				g_EnemyarmL[MAX_ENEMY];                     // ƒvƒŒƒCƒ„[‚Ì˜r
static ENEMY				g_EnemylegR[MAX_ENEMY];                     // ƒvƒŒƒCƒ„[‚Ì‘¾‚à‚à
static ENEMY				g_EnemylegL[MAX_ENEMY];                     // ƒvƒŒƒCƒ„[‚Ì‘¾‚à‚à
D3DXVECTOR3					g_Shadowpos[ENEMY_GROUP];					// ƒrƒ‹ƒ{[ƒh”­¶ˆÊ’u

LPDIRECT3DTEXTURE9	enemy_pD3DTexture_body;			// ƒeƒNƒXƒ`ƒƒ‚Ö‚Ìƒ|ƒCƒ“ƒ^
LPD3DXMESH			enemy_pD3DXMesh_body;			// ƒƒbƒVƒ…î•ñ‚Ö‚Ìƒ|ƒCƒ“ƒ^
LPD3DXBUFFER		enemy_pD3DXBuffMat_body;		// ƒ}ƒeƒŠƒAƒ‹î•ñ‚Ö‚Ìƒ|ƒCƒ“ƒ^
DWORD				enemy_nNumMat_body;				// ƒ}ƒeƒŠƒAƒ‹î•ñ‚Ì”

LPDIRECT3DTEXTURE9	enemy_pD3DTexture_head;			// ƒeƒNƒXƒ`ƒƒ‚Ö‚Ìƒ|ƒCƒ“ƒ^
LPD3DXMESH			enemy_pD3DXMesh_head;			// ƒƒbƒVƒ…î•ñ‚Ö‚Ìƒ|ƒCƒ“ƒ^
LPD3DXBUFFER		enemy_pD3DXBuffMat_head;		// ƒ}ƒeƒŠƒAƒ‹î•ñ‚Ö‚Ìƒ|ƒCƒ“ƒ^
DWORD				enemy_nNumMat_head;				// ƒ}ƒeƒŠƒAƒ‹î•ñ‚Ì”

LPDIRECT3DTEXTURE9	enemy_pD3DTexture_armL;			// ƒeƒNƒXƒ`ƒƒ‚Ö‚Ìƒ|ƒCƒ“ƒ^
LPD3DXMESH			enemy_pD3DXMesh_armL;			// ƒƒbƒVƒ…î•ñ‚Ö‚Ìƒ|ƒCƒ“ƒ^
LPD3DXBUFFER		enemy_pD3DXBuffMat_armL;		// ƒ}ƒeƒŠƒAƒ‹î•ñ‚Ö‚Ìƒ|ƒCƒ“ƒ^
DWORD				enemy_nNumMat_armL;				// ƒ}ƒeƒŠƒAƒ‹î•ñ‚Ì”

LPDIRECT3DTEXTURE9	enemy_pD3DTexture_armR;			// ƒeƒNƒXƒ`ƒƒ‚Ö‚Ìƒ|ƒCƒ“ƒ^
LPD3DXMESH			enemy_pD3DXMesh_armR;			// ƒƒbƒVƒ…î•ñ‚Ö‚Ìƒ|ƒCƒ“ƒ^
LPD3DXBUFFER		enemy_pD3DXBuffMat_armR;		// ƒ}ƒeƒŠƒAƒ‹î•ñ‚Ö‚Ìƒ|ƒCƒ“ƒ^
DWORD				enemy_nNumMat_armR;				// ƒ}ƒeƒŠƒAƒ‹î•ñ‚Ì”

LPDIRECT3DTEXTURE9	enemy_pD3DTexture_legL;			// ƒeƒNƒXƒ`ƒƒ‚Ö‚Ìƒ|ƒCƒ“ƒ^
LPD3DXMESH			enemy_pD3DXMesh_legL;			// ƒƒbƒVƒ…î•ñ‚Ö‚Ìƒ|ƒCƒ“ƒ^
LPD3DXBUFFER		enemy_pD3DXBuffMat_legL;		// ƒ}ƒeƒŠƒAƒ‹î•ñ‚Ö‚Ìƒ|ƒCƒ“ƒ^
DWORD				enemy_nNumMat_legL;				// ƒ}ƒeƒŠƒAƒ‹î•ñ‚Ì”

LPDIRECT3DTEXTURE9	enemy_pD3DTexture_legR;			// ƒeƒNƒXƒ`ƒƒ‚Ö‚Ìƒ|ƒCƒ“ƒ^
LPD3DXMESH			enemy_pD3DXMesh_legR;			// ƒƒbƒVƒ…î•ñ‚Ö‚Ìƒ|ƒCƒ“ƒ^
LPD3DXBUFFER		enemy_pD3DXBuffMat_legR;		// ƒ}ƒeƒŠƒAƒ‹î•ñ‚Ö‚Ìƒ|ƒCƒ“ƒ^
DWORD				enemy_nNumMat_legR;				// ƒ}ƒeƒŠƒAƒ‹î•ñ‚Ì”

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

// ŠK‘wƒAƒjƒ[ƒVƒ‡ƒ“
ENEMY_SET set_tbl_ENEMY[] = {    //pos,rot,scl Šeƒp[ƒc‚Ì‰Šú’l
	{	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.0f, (rand() % 300 + 1.0f), 0.0f),	D3DXVECTOR3(0.5f, 0.5f, 0.5f)},//‘Ì			
	{	D3DXVECTOR3(0.0f, 50.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.9f, 0.9f, 0.9f)},//“ª•”		
	{	D3DXVECTOR3(-12.0f, 40.0f, -0.1f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.9f, 0.9f, 0.9f)},//‰E˜r
	{	D3DXVECTOR3(12.0f, 40.0f, -0.1f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.9f, 0.9f, 0.9f)},//¶˜r
	{	D3DXVECTOR3(-5.0f,20.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f)},//‰E‘¾‚à‚à
	{	D3DXVECTOR3(5.0f, 20.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f)},//¶‘¾‚à‚à
};

//•à‚«ƒ‚[ƒVƒ‡ƒ“
D3DXVECTOR3 Ebody_walk_tbl[] = { //‘Ì

	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.00000f, -1.000000f, 0.000000f),
	D3DXVECTOR3(0.0f, 1.0f, 0.000000f),
};

D3DXVECTOR3 Ehead_walk_tbl[] = { //“ª•”

	D3DXVECTOR3(0.000000f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.000000f, D3DX_PI * 2, 0.000000f),
	D3DXVECTOR3(0.000000f, D3DX_PI * 2, 0.000000f),

};

D3DXVECTOR3 EarmR_walk_tbl[] = { //‰E˜r

	D3DXVECTOR3(0.374533f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.574533f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.374533f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 EarmL_walk_tbl[] = { //¶˜r

	D3DXVECTOR3(-0.374533f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.574533f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.374533f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 ElegR_walk_tbl[] = { //‰E‘¾‚à‚à

	D3DXVECTOR3(-1.023599f, 0.000000f, 0.000000f),
	D3DXVECTOR3(1.023599f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-1.023599f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 ElegL_walk_tbl[] = { //¶‘¾‚à‚à

	D3DXVECTOR3(1.023599f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-1.023599f, 0.000000f, 0.000000f),
	D3DXVECTOR3(1.023599f, 0.000000f, 0.000000f),
};

//UŒ‚ƒ‚[ƒVƒ‡ƒ“
D3DXVECTOR3 Ebody_Attack_tbl[] = { //‘Ì

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

D3DXVECTOR3 Ehead_Attack_tbl[] = { //“ª•”

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

D3DXVECTOR3 EarmR_Attack_tbl[] = { //‰E˜r

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

D3DXVECTOR3 EarmL_Attack_tbl[] = { //¶˜r

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

D3DXVECTOR3 ElegR_Attack_tbl[] = { //‰E‘¾‚à‚à

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

D3DXVECTOR3 ElegL_Attack_tbl[] = { //¶‘¾‚à‚à

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

//“|‚ê‚é‚Æ‚«‚Ìƒ‚[ƒVƒ‡ƒ“
D3DXVECTOR3 Ebody_Hit_tbl[] = { //‘Ì
	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.00000f, 0.0f,  D3DX_PI * 2),
};

D3DXVECTOR3 Ehead_Hit_tbl[] = { //“ª•”

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

D3DXVECTOR3 EarmR_Hit_tbl[] = { //‰E˜r

	D3DXVECTOR3(0.00000f, 0.000000f, -2.000000f),
	D3DXVECTOR3(0.0f, 0.0f, -1.500000f),
};

D3DXVECTOR3 EarmL_Hit_tbl[] = { //¶˜r

	D3DXVECTOR3(0.00000f, 0.000000f, 2.000000f),
	D3DXVECTOR3(0.0f, 0.0f, 1.500000f),
};

D3DXVECTOR3 ElegR_Hit_tbl[] = { //‰E‘¾‚à‚à

	D3DXVECTOR3(1.0f, 0.000000f, -2.000000f),
	D3DXVECTOR3(0.5f, 0.0f, -1.500000f),
};

D3DXVECTOR3 ElegL_Hit_tbl[] = { //¶‘¾‚à‚à

	D3DXVECTOR3(1.0f, 0.000000f, 2.000000f),
	D3DXVECTOR3(0.5f, 0.0f, -1.500000f),
};

D3DXVECTOR3 formation[] =	{   //wŒ`ƒtƒH[ƒ[ƒVƒ‡ƒ“
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(15.0f, 0.0f, 15.0f),
	D3DXVECTOR3(-15.0f, 0.0f, 15.0f),
	D3DXVECTOR3(20.0f, 0.0f, 20.0f),
	D3DXVECTOR3(-20.0f, 0.0f, 20.0f),
};

D3DXVECTOR3 formation1[] = {   //wŒ`ƒtƒH[ƒ[ƒVƒ‡ƒ“
	D3DXVECTOR3(0.0f, 30.0f, 200.0f),
	D3DXVECTOR3(50.0f, 0.0f, 15.0f),
	D3DXVECTOR3(-300.0f, 20.0f, 15.0f),
	D3DXVECTOR3(20.0f, 0.0f, 300.0f),
	D3DXVECTOR3(-200.0f, 50.0f, 50.0f),
};

//=============================================================================
// ‰Šú‰»ˆ—
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

		// Xƒtƒ@ƒCƒ‹‚Ì“Ç‚İ‚İ
		if (FAILED(D3DXLoadMeshFromX(MODEL_ENEMY,		// “Ç‚İ‚Şƒ‚ƒfƒ‹ƒtƒ@ƒCƒ‹–¼(xƒtƒ@ƒCƒ‹)
			D3DXMESH_SYSTEMMEM,							// ƒƒbƒVƒ…‚Ìì¬ƒIƒvƒVƒ‡ƒ“‚ğw’è
			pDevice,									// IDirect3DDevice9ƒCƒ“ƒ^[ƒtƒFƒCƒX‚Ö‚Ìƒ|ƒCƒ“ƒ^[
			NULL,										// —×Ú«ƒf[ƒ^‚ğŠÜ‚Şƒoƒbƒtƒ@‚Ö‚Ìƒ|ƒCƒ“ƒ^
			&enemy_pD3DXBuffMat_body,					// ƒ}ƒeƒŠƒAƒ‹ƒf[ƒ^‚ğŠÜ‚Şƒoƒbƒtƒ@‚Ö‚Ìƒ|ƒCƒ“ƒ^
			NULL,										// ƒGƒtƒFƒNƒgƒCƒ“ƒXƒ^ƒ“ƒX‚Ì”z—ñ‚ğŠÜ‚Şƒoƒbƒtƒ@‚Ö‚Ìƒ|ƒCƒ“ƒ^
			&enemy_nNumMat_body,						// D3DXMATERIAL\‘¢‘Ì‚Ì”
			&enemy_pD3DXMesh_body)))					// ID3DXMeshƒCƒ“ƒ^[ƒtƒFƒCƒX‚Ö‚Ìƒ|ƒCƒ“ƒ^‚ÌƒAƒhƒŒƒX
		{
			return E_FAIL;
		}

		// Xƒtƒ@ƒCƒ‹‚Ì“Ç‚İ‚İ
		if (FAILED(D3DXLoadMeshFromX(MODEL_ENEMY_HEAD,	// “Ç‚İ‚Şƒ‚ƒfƒ‹ƒtƒ@ƒCƒ‹–¼(xƒtƒ@ƒCƒ‹)
			D3DXMESH_SYSTEMMEM,							// ƒƒbƒVƒ…‚Ìì¬ƒIƒvƒVƒ‡ƒ“‚ğw’è
			pDevice,									// IDirect3DDevice9ƒCƒ“ƒ^[ƒtƒFƒCƒX‚Ö‚Ìƒ|ƒCƒ“ƒ^[
			NULL,										// —×Ú«ƒf[ƒ^‚ğŠÜ‚Şƒoƒbƒtƒ@‚Ö‚Ìƒ|ƒCƒ“ƒ^
			&enemy_pD3DXBuffMat_head,					// ƒ}ƒeƒŠƒAƒ‹ƒf[ƒ^‚ğŠÜ‚Şƒoƒbƒtƒ@‚Ö‚Ìƒ|ƒCƒ“ƒ^
			NULL,										// ƒGƒtƒFƒNƒgƒCƒ“ƒXƒ^ƒ“ƒX‚Ì”z—ñ‚ğŠÜ‚Şƒoƒbƒtƒ@‚Ö‚Ìƒ|ƒCƒ“ƒ^
			&enemy_nNumMat_head,						// D3DXMATERIAL\‘¢‘Ì‚Ì”
			&enemy_pD3DXMesh_head)))					// ID3DXMeshƒCƒ“ƒ^[ƒtƒFƒCƒX‚Ö‚Ìƒ|ƒCƒ“ƒ^‚ÌƒAƒhƒŒƒX
		{
			return E_FAIL;
		}

		// Xƒtƒ@ƒCƒ‹‚Ì“Ç‚İ‚İ
		if (FAILED(D3DXLoadMeshFromX(MODEL_ENEMY_ARM_L,	// “Ç‚İ‚Şƒ‚ƒfƒ‹ƒtƒ@ƒCƒ‹–¼(xƒtƒ@ƒCƒ‹)
			D3DXMESH_SYSTEMMEM,							// ƒƒbƒVƒ…‚Ìì¬ƒIƒvƒVƒ‡ƒ“‚ğw’è
			pDevice,									// IDirect3DDevice9ƒCƒ“ƒ^[ƒtƒFƒCƒX‚Ö‚Ìƒ|ƒCƒ“ƒ^[
			NULL,										// —×Ú«ƒf[ƒ^‚ğŠÜ‚Şƒoƒbƒtƒ@‚Ö‚Ìƒ|ƒCƒ“ƒ^
			&enemy_pD3DXBuffMat_armL,					// ƒ}ƒeƒŠƒAƒ‹ƒf[ƒ^‚ğŠÜ‚Şƒoƒbƒtƒ@‚Ö‚Ìƒ|ƒCƒ“ƒ^
			NULL,										// ƒGƒtƒFƒNƒgƒCƒ“ƒXƒ^ƒ“ƒX‚Ì”z—ñ‚ğŠÜ‚Şƒoƒbƒtƒ@‚Ö‚Ìƒ|ƒCƒ“ƒ^
			&enemy_nNumMat_armL,						// D3DXMATERIAL\‘¢‘Ì‚Ì”
			&enemy_pD3DXMesh_armL)))					// ID3DXMeshƒCƒ“ƒ^[ƒtƒFƒCƒX‚Ö‚Ìƒ|ƒCƒ“ƒ^‚ÌƒAƒhƒŒƒX
		{
			return E_FAIL;
		}

		// Xƒtƒ@ƒCƒ‹‚Ì“Ç‚İ‚İ
		if (FAILED(D3DXLoadMeshFromX(MODEL_ENEMY_ARM_R,	// “Ç‚İ‚Şƒ‚ƒfƒ‹ƒtƒ@ƒCƒ‹–¼(xƒtƒ@ƒCƒ‹)
			D3DXMESH_SYSTEMMEM,							// ƒƒbƒVƒ…‚Ìì¬ƒIƒvƒVƒ‡ƒ“‚ğw’è
			pDevice,									// IDirect3DDevice9ƒCƒ“ƒ^[ƒtƒFƒCƒX‚Ö‚Ìƒ|ƒCƒ“ƒ^[
			NULL,										// —×Ú«ƒf[ƒ^‚ğŠÜ‚Şƒoƒbƒtƒ@‚Ö‚Ìƒ|ƒCƒ“ƒ^
			&enemy_pD3DXBuffMat_armR,					// ƒ}ƒeƒŠƒAƒ‹ƒf[ƒ^‚ğŠÜ‚Şƒoƒbƒtƒ@‚Ö‚Ìƒ|ƒCƒ“ƒ^
			NULL,										// ƒGƒtƒFƒNƒgƒCƒ“ƒXƒ^ƒ“ƒX‚Ì”z—ñ‚ğŠÜ‚Şƒoƒbƒtƒ@‚Ö‚Ìƒ|ƒCƒ“ƒ^
			&enemy_nNumMat_armR,						// D3DXMATERIAL\‘¢‘Ì‚Ì”
			&enemy_pD3DXMesh_armR)))					// ID3DXMeshƒCƒ“ƒ^[ƒtƒFƒCƒX‚Ö‚Ìƒ|ƒCƒ“ƒ^‚ÌƒAƒhƒŒƒX
		{
			return E_FAIL;
		}

		// Xƒtƒ@ƒCƒ‹‚Ì“Ç‚İ‚İ
		if (FAILED(D3DXLoadMeshFromX(MODEL_ENEMY_LEG_L,	// “Ç‚İ‚Şƒ‚ƒfƒ‹ƒtƒ@ƒCƒ‹–¼(xƒtƒ@ƒCƒ‹)
			D3DXMESH_SYSTEMMEM,							// ƒƒbƒVƒ…‚Ìì¬ƒIƒvƒVƒ‡ƒ“‚ğw’è
			pDevice,									// IDirect3DDevice9ƒCƒ“ƒ^[ƒtƒFƒCƒX‚Ö‚Ìƒ|ƒCƒ“ƒ^[
			NULL,										// —×Ú«ƒf[ƒ^‚ğŠÜ‚Şƒoƒbƒtƒ@‚Ö‚Ìƒ|ƒCƒ“ƒ^
			&enemy_pD3DXBuffMat_legL,					// ƒ}ƒeƒŠƒAƒ‹ƒf[ƒ^‚ğŠÜ‚Şƒoƒbƒtƒ@‚Ö‚Ìƒ|ƒCƒ“ƒ^
			NULL,										// ƒGƒtƒFƒNƒgƒCƒ“ƒXƒ^ƒ“ƒX‚Ì”z—ñ‚ğŠÜ‚Şƒoƒbƒtƒ@‚Ö‚Ìƒ|ƒCƒ“ƒ^
			&enemy_nNumMat_legL,						// D3DXMATERIAL\‘¢‘Ì‚Ì”
			&enemy_pD3DXMesh_legL)))					// ID3DXMeshƒCƒ“ƒ^[ƒtƒFƒCƒX‚Ö‚Ìƒ|ƒCƒ“ƒ^‚ÌƒAƒhƒŒƒX
		{
			return E_FAIL;
		}

		// Xƒtƒ@ƒCƒ‹‚Ì“Ç‚İ‚İ
		if (FAILED(D3DXLoadMeshFromX(MODEL_ENEMY_LEG_R,	// “Ç‚İ‚Şƒ‚ƒfƒ‹ƒtƒ@ƒCƒ‹–¼(xƒtƒ@ƒCƒ‹)
			D3DXMESH_SYSTEMMEM,							// ƒƒbƒVƒ…‚Ìì¬ƒIƒvƒVƒ‡ƒ“‚ğw’è
			pDevice,									// IDirect3DDevice9ƒCƒ“ƒ^[ƒtƒFƒCƒX‚Ö‚Ìƒ|ƒCƒ“ƒ^[
			NULL,										// —×Ú«ƒf[ƒ^‚ğŠÜ‚Şƒoƒbƒtƒ@‚Ö‚Ìƒ|ƒCƒ“ƒ^
			&enemy_pD3DXBuffMat_legR,					// ƒ}ƒeƒŠƒAƒ‹ƒf[ƒ^‚ğŠÜ‚Şƒoƒbƒtƒ@‚Ö‚Ìƒ|ƒCƒ“ƒ^
			NULL,										// ƒGƒtƒFƒNƒgƒCƒ“ƒXƒ^ƒ“ƒX‚Ì”z—ñ‚ğŠÜ‚Şƒoƒbƒtƒ@‚Ö‚Ìƒ|ƒCƒ“ƒ^
			&enemy_nNumMat_legR,						// D3DXMATERIAL\‘¢‘Ì‚Ì”
			&enemy_pD3DXMesh_legR)))					// ID3DXMeshƒCƒ“ƒ^[ƒtƒFƒCƒX‚Ö‚Ìƒ|ƒCƒ“ƒ^‚ÌƒAƒhƒŒƒX
		{
			return E_FAIL;
		}
	}

#if 0
	// ƒeƒNƒXƒ`ƒƒ‚Ì“Ç‚İ‚İ
	D3DXCreateTextureFromFile(pDevice,	// ƒfƒoƒCƒX‚Ö‚Ìƒ|ƒCƒ“ƒ^
		TEXTURE_FILENAME,				// ƒtƒ@ƒCƒ‹‚Ì–¼‘O
		&enemy_pD3DTexture_body);		// “Ç‚İ‚Şƒƒ‚ƒŠ[
#endif

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_Enemy[i].use = false;			// Å‰‚Í–¢g—p
		g_Enemy[i].formation = 0;		// wŒ`”Ô†

		//ƒGƒlƒ~[‚ÌüŒ`•âŠÔƒpƒ^[ƒ“‚Ìİ’è
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

		// ‘Ì
		// ˆÊ’uE‰ñ“]EƒXƒP[ƒ‹‚Ì‰Šú‰»İ’è
		g_Enemy[i].pos = set_tbl_ENEMY[BODY].pos;
		g_Enemy[i].rot = set_tbl_ENEMY[BODY].rot;
		g_Enemy[i].scl = set_tbl_ENEMY[BODY].scl;

		// Šî€ˆÊ’uE‰ñ“]EƒXƒP[ƒ‹‚Ì‰Šú‰»İ’è
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
			// ‰e‚Ì‰Šú‰»
			g_Shadowpos[j] = D3DXVECTOR3(g_Enemy[i].pos.x + j * 10.0f, g_Enemy[i].pos.y, g_Enemy[i].pos.z + j * 20);
			g_Shadowpos[j].y = 0.0f;
			g_Enemy[i].shadowIdx[j] = CreateShadow(g_Shadowpos[j], g_Enemy[i].scl);
		}

		// “ª
		g_Enemyhead[i].use = false;			// Å‰‚Í–¢g—p

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

		// ˆÊ’uE‰ñ“]EƒXƒP[ƒ‹‚Ì‰Šú‰»İ’è
		g_Enemyhead[i].pos = set_tbl_ENEMY[HEAD].pos;
		g_Enemyhead[i].rot = set_tbl_ENEMY[HEAD].rot;
		g_Enemyhead[i].scl = set_tbl_ENEMY[HEAD].scl;

		// Šî€ˆÊ’uE‰ñ“]EƒXƒP[ƒ‹‚Ì‰Šú‰»İ’è
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

		// ‰E˜r
		g_EnemyarmR[i].use = false;			// Å‰‚Í–¢g—p

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

		// ˆÊ’uE‰ñ“]EƒXƒP[ƒ‹‚Ì‰Šú‰»İ’è
		g_EnemyarmR[i].pos = set_tbl_ENEMY[ARM_R].pos;
		g_EnemyarmR[i].rot = set_tbl_ENEMY[ARM_R].rot;
		g_EnemyarmR[i].scl = set_tbl_ENEMY[ARM_R].scl;

		// Šî€ˆÊ’uE‰ñ“]EƒXƒP[ƒ‹‚Ì‰Šú‰»İ’è
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

		// ¶˜r
		g_EnemyarmL[i].use = false;			// Å‰‚Í–¢g—p

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

		// ˆÊ’uE‰ñ“]EƒXƒP[ƒ‹‚Ì‰Šú‰»İ’è
		g_EnemyarmL[i].pos = set_tbl_ENEMY[ARM_L].pos;
		g_EnemyarmL[i].rot = set_tbl_ENEMY[ARM_L].rot;
		g_EnemyarmL[i].scl = set_tbl_ENEMY[ARM_L].scl;

		// Šî€ˆÊ’uE‰ñ“]EƒXƒP[ƒ‹‚Ì‰Šú‰»İ’è
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

		// ‰E‘«
		g_EnemylegR[i].use = false;			// Å‰‚Í–¢g—p

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

		// ˆÊ’uE‰ñ“]EƒXƒP[ƒ‹‚Ì‰Šú‰»İ’è
		g_EnemylegR[i].pos = set_tbl_ENEMY[LEG_R].pos;
		g_EnemylegR[i].rot = set_tbl_ENEMY[LEG_R].rot;
		g_EnemylegR[i].scl = set_tbl_ENEMY[LEG_R].scl;

		// Šî€ˆÊ’uE‰ñ“]EƒXƒP[ƒ‹‚Ì‰Šú‰»İ’è
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

		// ¶‘«
		g_EnemylegL[i].use = false;			// Å‰‚Í–¢g—p

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
		// ˆÊ’uE‰ñ“]EƒXƒP[ƒ‹‚Ì‰Šú‰»İ’è
		g_EnemylegL[i].pos = set_tbl_ENEMY[LEG_L].pos;
		g_EnemylegL[i].rot = set_tbl_ENEMY[LEG_L].rot;
		g_EnemylegL[i].scl = set_tbl_ENEMY[LEG_L].scl;

		// Šî€ˆÊ’uE‰ñ“]EƒXƒP[ƒ‹‚Ì‰Šú‰»İ’è
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
// I—¹ˆ—
//=============================================================================
void UninitEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (enemy_pD3DTexture_body != NULL)
		{// ƒeƒNƒXƒ`ƒƒ‚ÌŠJ•ú
			enemy_pD3DTexture_body->Release();
			enemy_pD3DTexture_body = NULL;
		}

		if (enemy_pD3DXMesh_body != NULL)
		{// ƒƒbƒVƒ…‚ÌŠJ•ú
			enemy_pD3DXMesh_body->Release();
			enemy_pD3DXMesh_body = NULL;
		}

		if (enemy_pD3DXBuffMat_body != NULL)
		{//ƒ}ƒeƒŠƒAƒ‹‚ÌŠJ•ú
			enemy_pD3DXBuffMat_body->Release();
			enemy_pD3DXBuffMat_body = NULL;
		}

		if (enemy_pD3DTexture_head != NULL)
		{// ƒeƒNƒXƒ`ƒƒ‚ÌŠJ•ú
			enemy_pD3DTexture_head->Release();
			enemy_pD3DTexture_head = NULL;
		}
		if (enemy_pD3DXMesh_head != NULL)
		{// ƒƒbƒVƒ…‚ÌŠJ•ú
			enemy_pD3DXMesh_head->Release();
			enemy_pD3DXMesh_head = NULL;
		}
		if (enemy_pD3DXBuffMat_head != NULL)
		{// ƒ}ƒeƒŠƒAƒ‹‚ÌŠJ•ú
			enemy_pD3DXBuffMat_head->Release();
			enemy_pD3DXBuffMat_head = NULL;
		}

		if (enemy_pD3DTexture_armR != NULL)
		{// ƒeƒNƒXƒ`ƒƒ‚ÌŠJ•ú
			enemy_pD3DTexture_armR->Release();
			enemy_pD3DTexture_armR = NULL;
		}
		if (enemy_pD3DXMesh_armR != NULL)
		{// ƒƒbƒVƒ…‚ÌŠJ•ú
			enemy_pD3DXMesh_armR->Release();
			enemy_pD3DXMesh_armR = NULL;
		}
		if (enemy_pD3DXBuffMat_armR != NULL)
		{// ƒ}ƒeƒŠƒAƒ‹‚ÌŠJ•ú
			enemy_pD3DXBuffMat_armR->Release();
			enemy_pD3DXBuffMat_armR = NULL;
		}

		if (enemy_pD3DTexture_armL != NULL)
		{// ƒeƒNƒXƒ`ƒƒ‚ÌŠJ•ú
			enemy_pD3DTexture_armL->Release();
			enemy_pD3DTexture_armL = NULL;
		}
		if (enemy_pD3DXMesh_armL != NULL)
		{// ƒƒbƒVƒ…‚ÌŠJ•ú
			enemy_pD3DXMesh_armL->Release();
			enemy_pD3DXMesh_armL = NULL;
		}
		if (enemy_pD3DXBuffMat_armL != NULL)
		{// ƒ}ƒeƒŠƒAƒ‹‚ÌŠJ•ú
			enemy_pD3DXBuffMat_armL->Release();
			enemy_pD3DXBuffMat_armL = NULL;
		}

		if (enemy_pD3DTexture_legR != NULL)
		{// ƒeƒNƒXƒ`ƒƒ‚ÌŠJ•ú
			enemy_pD3DTexture_legR->Release();
			enemy_pD3DTexture_legR = NULL;
		}
		if (enemy_pD3DXMesh_legR != NULL)
		{// ƒƒbƒVƒ…‚ÌŠJ•ú
			enemy_pD3DXMesh_legR->Release();
			enemy_pD3DXMesh_legR = NULL;
		}
		if (enemy_pD3DXBuffMat_legR != NULL)
		{// ƒ}ƒeƒŠƒAƒ‹‚ÌŠJ•ú
			enemy_pD3DXBuffMat_legR->Release();
			enemy_pD3DXBuffMat_legR = NULL;
		}

		if (enemy_pD3DTexture_legL != NULL)
		{// ƒeƒNƒXƒ`ƒƒ‚ÌŠJ•ú
			enemy_pD3DTexture_legL->Release();
			enemy_pD3DTexture_legL = NULL;
		}
		if (enemy_pD3DXMesh_legL != NULL)
		{// ƒƒbƒVƒ…‚ÌŠJ•ú
			enemy_pD3DXMesh_legL->Release();
			enemy_pD3DXMesh_legL = NULL;
		}
		if (enemy_pD3DXBuffMat_legL != NULL)
		{// ƒ}ƒeƒŠƒAƒ‹‚ÌŠJ•ú
			enemy_pD3DXBuffMat_legL->Release();
			enemy_pD3DXBuffMat_legL = NULL;
		}
	}
}

//=============================================================================
// XVˆ—
//=============================================================================
void UpdateEnemy(void)
{
	PLAYER *player = GetPlayer();

	int cnt = 0;

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].status != DEAD)	//ƒGƒlƒ~[‚ª€‚ñ‚Å‚¢‚È‚¯‚ê‚Î
			if (g_Enemy[i].use == true)  //ƒGƒlƒ~[‚ª‚¢‚é‚È‚ç
			{
				if (g_Enemy[i].status != HIT)		// ƒGƒlƒ~[‚ªUŒ‚‚ğó‚¯‚Ä‚¢‚È‚¯‚ê‚Î
				{
					cnt++;					//¶‚«‚Ä‚¢‚éƒGƒlƒ~[‚ğƒJƒEƒ“ƒg

					switch (g_Enemy[i].pattern) //ƒGƒlƒ~[‚Ìó‘Ô‘JˆÚ‚Ìswitch
					{
					case 1://üŒ`•âŠÔiõ“Gj
							//ˆÚ“®ˆ—
					{
						int	index = (int)g_Enemy[i].move_time;			//üŒ`•âŠÔ‚Ìƒe[ƒuƒ‹‚Ìindex
						float	time = g_Enemy[i].move_time - index;	//üŒ`•âŠÔ‚Ìˆ—Š„‡(0.0`0.99....)

						g_Enemy[i].move_time += g_Enemy[i].movepattern[index].dt;	//ƒAƒjƒ[ƒVƒ‡ƒ“‚Ì‡ŒvŠÔ‚É‘«‚·
						if (index > (g_Enemy[i].size - 2))							// ƒS[ƒ‹‚ÖƒI[ƒo[‚µ‚Ä‚¢‚½‚çAƒS[ƒ‹‚Ö–ß‚·
						{
							g_Enemy[i].move_time = 0.0f;
							index = 0;
						}

						// XÀ•W‚ğ‹‚ß‚é	X = StartX + (EndX - StartX) * ¡‚ÌŠÔ
						D3DXVECTOR3 vec = g_Enemy[i].movepattern[index + 1].pos - g_Enemy[i].movepattern[index].pos;
						g_Enemy[i].pos = g_Enemy[i].movepattern[index].pos + (vec * time) + g_Enemy[i].spos;

						//‰ñ“]‚ğ‹‚ß‚é@@@R = StsrtX + (EndX - startX) * ¡‚ÌŠÔ
						D3DXVECTOR3 rot = g_Enemy[i].movepattern[index + 1].rot - g_Enemy[i].movepattern[index].rot;
						g_Enemy[i].rot = g_Enemy[i].movepattern[index].rot + (rot * time) + g_Enemy[i].srot;

						//scale‚ğ‹‚ß‚é     S = StsrtX + (EndX - startX) * ¡‚ÌŠÔ
						D3DXVECTOR3 scl = g_Enemy[i].movepattern[index + 1].scl - g_Enemy[i].movepattern[index].scl;
						g_Enemy[i].scl = g_Enemy[i].movepattern[index].scl + (scl * time) + g_Enemy[i].sscl;

						if (CheckHitFP(player->pos, g_Enemy[i].pos, D3DXVECTOR3(-sinf(g_Enemy[i].rot.y), 0, -cosf(g_Enemy[i].rot.y)), 30, 300, 0) == true)				//ƒvƒŒƒCƒ„[‚ğŒ©‚Â‚¯‚½‚çjŠÔ‚ğ~‚ß‚Ä“G‚ÌÀ•W‚ğ•Û‘¶
						{
							g_Enemy[i].timestop = true;						//’Ç]‚ğOFF‚É‚·‚é

							g_Enemy[i].oldpos = g_Enemy[i].pos;				//oldpos‚ÉƒGƒlƒ~[À•W‚ğ•Û‘¶

							g_Enemy[i].following = true;					//’Ç]‚ğON‚É‚·‚é

							g_Enemy[i].pattern = 2;							//’Ç]ƒpƒ^[ƒ“‚ÉˆÚ‚é
						}
					}
					break;
					case 2://’Ç]
					{
						D3DXVECTOR3 pos1 = player->pos - g_Enemy[i].pos;	//ƒvƒŒƒCƒ„[‚ÌÀ•W‚©‚çƒGƒlƒ~[‚ÌÀ•W‚ğˆø‚¢‚½‚à‚Ì‚ğpos‚P‚É“ü‚ê‚é				

						//õ“G”ÍˆÍŠO‚És‚Á‚½‚ç’Ç‚¤‚Ì‚ğ‚â‚ß‚é
						if (Length(pos1) > ENEMY_SCOUTING)
						{
							g_Enemy[i].pattern = 3;
							break;
						}
						//‹ß‚Ã‚¢‚½‚ç–ß‚é
						if ((Length(pos1) < 40) && ((Length(pos1) > 5)))
						{
							g_Enemy[i].attackcnt++;					//ƒGƒlƒ~[‚ªUŒ‚ŠJn‚·‚é‚Ü‚Å‚ÌŠÔ‚ğƒJƒEƒ“ƒg
							if (g_Enemy[i].attackcnt > 30)
							{
								g_Enemy[i].status = ATTACK;			//ƒGƒlƒ~[‚ªUŒ‚ƒ‚[ƒh‚É‚È‚é
							}
							break;
						}

						//õ“G”ÍˆÍ“à‚Ì‚Í’Ç‚¤
						if (Length(pos1) <= ENEMY_SCOUTING)
						{
							//’Ç]I—¹‚µ‚½‚Æ‚«Œ³‚ÌÀ•W‚Ö–ß‚é‚½‚ß‚ÌüŒ`•âŠÔ—p‚Ìback_time‚ğ‰Šú‰»
							g_Enemy[i].back_time = 0.00f;

							//’Ç]ˆ—ŠJn//											
							float rot1 = atan2f(pos1.z, pos1.x);

							rot1 = (D3DX_PI / 2) * 3 - rot1;
							g_Enemy[i].rot.y = rot1;

							pos1.y = 0;
							float len = Length(pos1);

							len = 1 / len;
							pos1.x *= len;
							pos1.z *= len;
							g_Enemy[i].pos += pos1 * g_Enemy[i].spd;
								//’Ç]ˆ—I—¹//
						}
					}
					break;
					case 3://Œ³‚ÌÀ•W‚É–ß‚é

						float	back_dt = 0.01f;
						g_Enemy[i].following = false;									//’Ç]‚ğOFF

						D3DXVECTOR3 pos2 = g_Enemy[i].oldpos - g_Enemy[i].pos;			//Œ»pos‚©‚ç–ß‚éêŠ‚Ö‚Ì‹——£
						pos2.y = 0;

						g_Enemy[i].pos = g_Enemy[i].pos + pos2 * g_Enemy[i].back_time;	//oldpos‚ÖŒü‚¯‚ÄüŒ`•âŠÔ
						g_Enemy[i].back_time += back_dt;								//back_timeXV

						//ƒGƒlƒ~[‚ğoldpos‚ÖŒü‚©‚¹‚é
						float rot1 = atan2f(pos2.z, pos2.x);
						rot1 = (D3DX_PI / 2) * 3 - rot1;
						g_Enemy[i].rot.y = rot1;

						if (g_Enemy[i].back_time >= 1.0)								//oldpos‚Ö‚ÌüŒ`•âŠÔ‚ª‚¨‚í‚Á‚½‚çó‘Ô‘JˆÚ
						{
							g_Enemy[i].timestop = false;
							g_Enemy[i].pattern = 1;
						}
						break;
					}

					// ƒGƒlƒ~[‚ª•à‚­ƒ‚[ƒVƒ‡ƒ“
					if (g_Enemy[i].status == WALK)
					{
						float	dt = 0.03f;
						g_Enemy[i].walkmove_time += dt;		// ƒAƒjƒ[ƒVƒ‡ƒ“‚Ì‡ŒvŠÔ‚É‘«‚·

						int index = (int)g_Enemy[i].walkmove_time;
						float	time = g_Enemy[i].walkmove_time - index;
						int		size = sizeof(Ebody_walk_tbl) / sizeof(D3DXVECTOR3); //ƒe[ƒuƒ‹‘S‘Ì‚ÌƒoƒCƒg”‚ğƒe[ƒuƒ‹ˆê‚Â‚ÌƒoƒCƒg”‚ÅŠ„‚é(ƒe[ƒuƒ‹”‚ğ‹‚ß‚é)

						if (index > (size - 2))	// ƒS[ƒ‹‚ğƒI[ƒo[‚µ‚Ä‚¢‚½‚çAƒS[ƒ‹‚Ö–ß‚·
						{
							g_Enemy[i].walkmove_time = 0.0f;
							index = 0;
						}

						// ƒAƒjƒ[ƒVƒ‡ƒ“‚³‚¹‚é•”ˆÊ	@X = StartX + (EndX - StartX) * ¡‚ÌŠÔ
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

					// ƒGƒlƒ~[‚ªUŒ‚‚µ‚Ä‚¢‚éƒAƒjƒ[ƒVƒ‡ƒ“
					if (g_Enemy[i].status == ATTACK)
					{
						g_Enemy[i].attack_dt += 0.01f;
						g_Enemy[i].attackmove_time += g_Enemy[i].attack_dt;		// ƒAƒjƒ[ƒVƒ‡ƒ“‚Ì‡ŒvŠÔ‚É‘«‚·

						int a_index = (int)g_Enemy[i].attackmove_time;
						float	a_time = g_Enemy[i].attackmove_time - a_index;
						int		a_size = sizeof(Ebody_Attack_tbl) / sizeof(D3DXVECTOR3); //ƒe[ƒuƒ‹‘S‘Ì‚ÌƒoƒCƒg”‚ğƒe[ƒuƒ‹ˆê‚Â‚ÌƒoƒCƒg”‚ÅŠ„‚é(ƒe[ƒuƒ‹”‚ğ‹‚ß‚é)

						if (a_index > (a_size - 2))	// ƒS[ƒ‹‚ğƒI[ƒo[‚µ‚Ä‚¢‚½‚çAƒS[ƒ‹‚Ö–ß‚·
						{
							g_Enemy[i].status = WALK;
							g_Enemy[i].pattern = 3;
							g_Enemy[i].attackmove_time = 0.0f;
							a_index = 0;
							g_Enemy[i].attackcnt = 0;
						}

						// ƒAƒjƒ[ƒVƒ‡ƒ“‚³‚¹‚é•”ˆÊ	@X = StartX + (EndX - StartX) * ¡‚ÌŠÔ
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
		//ƒGƒlƒ~[‚ªUŒ‚‚ğó‚¯‚½‚ÌƒAƒjƒ[ƒVƒ‡ƒ“i‚Á”ò‚Ôj
			else if (g_Enemy[i].status == HIT)
			{
				g_Enemy[i].formation = 10;
				g_Enemy[i].hit_dt += 0.003f;
				g_Enemy[i].hitmove_time += g_Enemy[i].hit_dt;		// ƒAƒjƒ[ƒVƒ‡ƒ“‚Ì‡ŒvŠÔ‚É‘«‚·

				int a_index = (int)g_Enemy[i].hitmove_time;
				float	a_time = g_Enemy[i].hitmove_time - a_index;
				int		a_size = sizeof(Ebody_Hit_tbl) / sizeof(D3DXVECTOR3); //ƒe[ƒuƒ‹‘S‘Ì‚ÌƒoƒCƒg”‚ğƒe[ƒuƒ‹ˆê‚Â‚ÌƒoƒCƒg”‚ÅŠ„‚é(ƒe[ƒuƒ‹”‚ğ‹‚ß‚é)

				if (a_index > (a_size - 2))	// ƒS[ƒ‹‚ğƒI[ƒo[‚µ‚Ä‚¢‚½‚çAƒS[ƒ‹‚Ö–ß‚·
				{
					g_Enemy[i].hitmove_time = 0.0f;
					a_index = 0;
				}

				// ƒAƒjƒ[ƒVƒ‡ƒ“‚³‚¹‚é•”ˆÊ	@X = StartX + (EndX - StartX) * ¡‚ÌŠÔ
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
				if (g_Enemy[i].hit_cnt >= g_Enemy[i].hit_cnt_max)		// ‚Á”ò‚Ô‚ÌI—¹H
				{														// Yes
					g_Enemy[i].status = DEAD;							//€–S
					g_Enemy[i].ofs = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					g_Enemy[i].pos.y = 0.0f;
				}
			}
		for (int j = 0; j < ENEMY_GROUP; j++)			//‰e‚Ìİ’è
		{
			// ‰e‚àƒvƒŒƒCƒ„[‚ÌˆÊ’u‚É‡‚í‚¹‚é
			g_Shadowpos[j] = g_Enemy[i].pos + (formation[j] * j);
			g_Shadowpos[j].y = 0.0f;
			SetPositionShadow(g_Enemy[i].shadowIdx[j], g_Shadowpos[j], g_Enemy[i].scl);
		}
	}

	if (cnt < MAX_ENEMY / 3)							//ƒGƒlƒ~[”‚ª3•ª‚Ì1–¢–‚È‚çƒGƒlƒ~[‚ğ”­¶‚³‚¹‚é
	{
		SetEnemy(MAX_ENEMY, D3DXVECTOR3(-1500, 0, 1500), D3DXVECTOR3(1500, 0, -1500));
	}

}

//=============================================================================
// •`‰æˆ—
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//g‚í‚ê‚Ä‚È‚¢‚È‚çƒRƒ“ƒeƒjƒ…[‚·‚é
		if (g_Enemy[i].status == DEAD) continue;

		for (int j = 0; j < ENEMY_GROUP; j++)			// ‚Ğ‚Æ‚©‚½‚Ü‚è•ª‚ğ•`‰æˆ—
		{
			// ‘ÌˆÈŠO‚Íæ‚ÉŒvZ‚µ‚Ä‚µ‚Ü‚¤
			//ƒ[ƒ‹ƒhƒ}ƒgƒŠƒbƒNƒX‚Ì‰Šú‰»
			D3DXMatrixIdentity(&g_Enemyhead[i].mtxWorld);
			D3DXMatrixIdentity(&g_EnemyarmR[i].mtxWorld);
			D3DXMatrixIdentity(&g_EnemyarmL[i].mtxWorld);
			D3DXMatrixIdentity(&g_EnemylegR[i].mtxWorld);
			D3DXMatrixIdentity(&g_EnemylegL[i].mtxWorld);

			//ƒXƒP[ƒ‹‚Ì”½‰f
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

			//‰ñ“]‚ğ”½‰f
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

			//ˆÚ“®‚ğ”½‰f
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

			//‘Ì
			//ƒ[ƒ‹ƒhƒ}ƒgƒŠƒbƒNƒX‚Ì‰Šú‰»
			D3DXMatrixIdentity(&g_Enemy[i].mtxWorld);

			//ƒXƒP[ƒ‹‚Ì”½‰f
			D3DXMatrixScaling(&g_Enemy[i].mtxScl, g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
			D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &g_Enemy[i].mtxScl);

			//‰ñ“]‚ğ”½‰f
			D3DXMatrixRotationYawPitchRoll(&g_Enemy[i].mtxRot, g_Enemy[i].rot.y, g_Enemy[i].rot.x, g_Enemy[i].rot.z);
			D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &g_Enemy[i].mtxRot);

			//ˆÚ“®‚ğ”½‰fiƒCƒ“ƒXƒ^ƒ“ƒVƒ“ƒOj
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

			//ƒ[ƒ‹ƒhƒ}ƒgƒŠƒbƒNƒX‚Ìİ’è
			pDevice->SetTransform(D3DTS_WORLD, &g_Enemy[i].mtxWorld);

			//Œ»İ‚Ìƒ}ƒeƒŠƒAƒ‹‚ğæ“¾
			pDevice->GetMaterial(&g_Enemy[i].matDef);

			// ƒ}ƒeƒŠƒAƒ‹î•ñ‚É‘Î‚·‚éƒ|ƒCƒ“ƒ^‚ğæ“¾
			g_Enemy[i].pD3DXMat = (D3DXMATERIAL*)enemy_pD3DXBuffMat_body->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)enemy_nNumMat_body; nCntMat++)
			{
				//ƒ}ƒeƒŠƒAƒ‹‚Ìİ’è
				pDevice->SetMaterial(&g_Enemy[i].pD3DXMat[nCntMat].MatD3D);

				//ƒeƒNƒXƒ`ƒƒ‚Ìİ’è
				pDevice->SetTexture(0, enemy_pD3DTexture_body);

				//•`‰æ
				enemy_pD3DXMesh_body->DrawSubset(nCntMat);
			}

			//ƒ}ƒeƒŠƒAƒ‹‚ğƒfƒtƒHƒ‹ƒ‚É–ß‚·
			pDevice->SetMaterial(&g_Enemy[i].matDef);

			//“ª
			//q‚Ìworld = q‚Ìworld * e‚Ìworld
			D3DXMatrixMultiply(&g_Enemyhead[i].mtxWorld, &g_Enemyhead[i].mtxWorld, &g_Enemyhead[i].parent->mtxWorld);

			//ƒ[ƒ‹ƒhƒ}ƒgƒŠƒbƒNƒX‚Ìİ’è
			pDevice->SetTransform(D3DTS_WORLD, &g_Enemyhead[i].mtxWorld);

			//Œ»İ‚Ìƒ}ƒeƒŠƒAƒ‹‚ğæ“¾
			pDevice->GetMaterial(&g_Enemyhead[i].matDef);

			// ƒ}ƒeƒŠƒAƒ‹î•ñ‚É‘Î‚·‚éƒ|ƒCƒ“ƒ^‚ğæ“¾
			g_Enemyhead[i].pD3DXMat = (D3DXMATERIAL*)enemy_pD3DXBuffMat_head->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)enemy_nNumMat_head; nCntMat++)
			{
				//ƒ}ƒeƒŠƒAƒ‹‚Ìİ’è
				pDevice->SetMaterial(&g_Enemyhead[i].pD3DXMat[nCntMat].MatD3D);

				//ƒeƒNƒXƒ`ƒƒ‚Ìİ’è
				pDevice->SetTexture(0, enemy_pD3DTexture_head);

				//•`‰æ
				enemy_pD3DXMesh_head->DrawSubset(nCntMat);
			}

			// ƒ}ƒeƒŠƒAƒ‹‚ğƒfƒtƒHƒ‹ƒg‚É–ß‚·
			pDevice->SetMaterial(&g_Enemyhead[i].matDef);

			//‰E˜r
			//q‚Ìworld = q‚Ìworld * e‚Ìworld
			D3DXMatrixMultiply(&g_EnemyarmR[i].mtxWorld, &g_EnemyarmR[i].mtxWorld, &g_EnemyarmR[i].parent->mtxWorld);

			//ƒ[ƒ‹ƒhƒ}ƒgƒŠƒbƒNƒX‚Ìİ’è
			pDevice->SetTransform(D3DTS_WORLD, &g_EnemyarmR[i].mtxWorld);

			//Œ»İ‚Ìƒ}ƒeƒŠƒAƒ‹‚ğæ“¾
			pDevice->GetMaterial(&g_EnemyarmR[i].matDef);

			// ƒ}ƒeƒŠƒAƒ‹î•ñ‚É‘Î‚·‚éƒ|ƒCƒ“ƒ^‚ğæ“¾
			g_EnemyarmR[i].pD3DXMat = (D3DXMATERIAL*)enemy_pD3DXBuffMat_armR->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)enemy_nNumMat_armR; nCntMat++)
			{
				//ƒ}ƒeƒŠƒAƒ‹‚Ìİ’è
				pDevice->SetMaterial(&g_EnemyarmR[i].pD3DXMat[nCntMat].MatD3D);

				//ƒeƒNƒXƒ`ƒƒ‚Ìİ’è
				pDevice->SetTexture(0, enemy_pD3DTexture_armR);

				//•`‰æ
				enemy_pD3DXMesh_armR->DrawSubset(nCntMat);
			}

			// ƒ}ƒeƒŠƒAƒ‹‚ğƒfƒtƒHƒ‹ƒg‚É–ß‚·
			pDevice->SetMaterial(&g_EnemyarmR[i].matDef);

			//¶˜r
			//q‚Ìworld = q‚Ìworld * e‚Ìworld
			D3DXMatrixMultiply(&g_EnemyarmL[i].mtxWorld, &g_EnemyarmL[i].mtxWorld, &g_EnemyarmL[i].parent->mtxWorld);

			//ƒ[ƒ‹ƒhƒ}ƒgƒŠƒbƒNƒX‚Ìİ’è
			pDevice->SetTransform(D3DTS_WORLD, &g_EnemyarmL[i].mtxWorld);

			//Œ»İ‚Ìƒ}ƒeƒŠƒAƒ‹‚ğæ“¾
			pDevice->GetMaterial(&g_EnemyarmL[i].matDef);

			// ƒ}ƒeƒŠƒAƒ‹î•ñ‚É‘Î‚·‚éƒ|ƒCƒ“ƒ^‚ğæ“¾
			g_EnemyarmL[i].pD3DXMat = (D3DXMATERIAL*)enemy_pD3DXBuffMat_armL->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)enemy_nNumMat_armL; nCntMat++)
			{
				//ƒ}ƒeƒŠƒAƒ‹‚Ìİ’è
				pDevice->SetMaterial(&g_EnemyarmL[i].pD3DXMat[nCntMat].MatD3D);

				//ƒeƒNƒXƒ`ƒƒ‚Ìİ’è
				pDevice->SetTexture(0, enemy_pD3DTexture_armL);

				//•`‰æ
				enemy_pD3DXMesh_armL->DrawSubset(nCntMat);
			}

			// ƒ}ƒeƒŠƒAƒ‹‚ğƒfƒtƒHƒ‹ƒg‚É–ß‚·
			pDevice->SetMaterial(&g_EnemyarmL[i].matDef);

			//‰E˜r
			//q‚Ìworld = q‚Ìworld * e‚Ìworld
			D3DXMatrixMultiply(&g_EnemylegR[i].mtxWorld, &g_EnemylegR[i].mtxWorld, &g_EnemylegR[i].parent->mtxWorld);

			//ƒ[ƒ‹ƒhƒ}ƒgƒŠƒbƒNƒX‚Ìİ’è
			pDevice->SetTransform(D3DTS_WORLD, &g_EnemylegR[i].mtxWorld);

			//Œ»İ‚Ìƒ}ƒeƒŠƒAƒ‹‚ğæ“¾
			pDevice->GetMaterial(&g_EnemylegR[i].matDef);

			// ƒ}ƒeƒŠƒAƒ‹î•ñ‚É‘Î‚·‚éƒ|ƒCƒ“ƒ^‚ğæ“¾
			g_EnemylegR[i].pD3DXMat = (D3DXMATERIAL*)enemy_pD3DXBuffMat_legR->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)enemy_nNumMat_legR; nCntMat++)
			{
				//ƒ}ƒeƒŠƒAƒ‹‚Ìİ’è
				pDevice->SetMaterial(&g_EnemylegR[i].pD3DXMat[nCntMat].MatD3D);

				//ƒeƒNƒXƒ`ƒƒ‚Ìİ’è
				pDevice->SetTexture(0, enemy_pD3DTexture_legR);

				//•`‰æ
				enemy_pD3DXMesh_legR->DrawSubset(nCntMat);
			}

			// ƒ}ƒeƒŠƒAƒ‹‚ğƒfƒtƒHƒ‹ƒg‚É–ß‚·
			pDevice->SetMaterial(&g_EnemylegR[i].matDef);

			//¶‘«
			//q‚Ìworld = q‚Ìworld * e‚Ìworld
			D3DXMatrixMultiply(&g_EnemylegL[i].mtxWorld, &g_EnemylegL[i].mtxWorld, &g_EnemylegL[i].parent->mtxWorld);

			//ƒ[ƒ‹ƒhƒ}ƒgƒŠƒbƒNƒX‚Ìİ’è
			pDevice->SetTransform(D3DTS_WORLD, &g_EnemylegL[i].mtxWorld);

			//Œ»İ‚Ìƒ}ƒeƒŠƒAƒ‹‚ğæ“¾
			pDevice->GetMaterial(&g_EnemylegL[i].matDef);

			// ƒ}ƒeƒŠƒAƒ‹î•ñ‚É‘Î‚·‚éƒ|ƒCƒ“ƒ^‚ğæ“¾
			g_EnemylegL[i].pD3DXMat = (D3DXMATERIAL*)enemy_pD3DXBuffMat_legL->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)enemy_nNumMat_legL; nCntMat++)
			{
				//ƒ}ƒeƒŠƒAƒ‹‚Ìİ’è
				pDevice->SetMaterial(&g_EnemylegL[i].pD3DXMat[nCntMat].MatD3D);

				//ƒeƒNƒXƒ`ƒƒ‚Ìİ’è
				pDevice->SetTexture(0, enemy_pD3DTexture_legL);

				//•`‰æ
				enemy_pD3DXMesh_legL->DrawSubset(nCntMat);
			}

			// ƒ}ƒeƒŠƒAƒ‹‚ğƒfƒtƒHƒ‹ƒg‚É–ß‚·
			pDevice->SetMaterial(&g_EnemylegL[i].matDef);
		}
	}
}

//=============================================================================
//ƒoƒgƒ‹ƒGƒlƒ~[‚Ìæ“¾
//=============================================================================
ENEMY *GetEnemy()
{
	return &(g_Enemy[0]);
}

//=============================================================================
// ƒoƒgƒ‹ƒGƒlƒ~[‚ğ”z’u
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
				g_Enemy[i].use = true;						// ƒGƒlƒ~[‚ğg—p’†‚É‚·‚é
				g_Enemy[i].status = WALK;					// ƒGƒlƒ~[‚ÌƒXƒe[ƒ^ƒX‚ğ‰Šú‰»
				RefreshEnemy(i, upperleft, bottomright);	// ƒGƒlƒ~[‚Ì”z’u

				if ((item + i)->bUse == true)				// ƒAƒCƒeƒ€‚ªg—p’†‚È‚ç
				{
					DeleteItem(i);							// Á‚µ‚Ä‚µ‚Ü‚¤

					for (int j = 0; j < MAX_PARTICLE; j++)
					{
						GetParticle(i, j)->bUse = false;	// ƒp[ƒeƒBƒNƒ‹I—¹
					}
				}
				break;
			}
		}
	}
	return;
}
//=============================================================================
// ƒoƒgƒ‹ƒGƒlƒ~[‚ğÄ”z’u
// reflesh_enemy	: Ä”z’u‚·‚é”
// upperleft		: oŒ»‚·‚éˆÊ’ui¶ãj
// bottomright		: oŒ»‚·‚éˆÊ’ui‰E‰ºj
//=============================================================================
void RefreshEnemy(int refresh_enemy, D3DXVECTOR3 upperleft, D3DXVECTOR3 bottomright)
{
	g_Enemy[refresh_enemy].spos.x = (rand() % (int)((bottomright.x - upperleft.x + 1)) + upperleft.x);
	g_Enemy[refresh_enemy].spos.z = (rand() % (int)((upperleft.z - bottomright.z + 1)) + bottomright.z);
}



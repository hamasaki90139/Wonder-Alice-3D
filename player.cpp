//=============================================================================
//
// モデル処理 [player.cpp]
// Author : 濱崎 小春
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "player.h"
#include "camera.h"
#include "shadow.h"
#include "debugproc.h"
#include "collision.h"
#include "enemy.h"
#include "sound.h"
#include "effect.h"
#include "fade.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static PLAYER				g_Player;						// プレイヤー
static PLAYER				g_head;							// プレイヤーの頭部
static PLAYER				g_arm[LR];                      // プレイヤーの腕
static PLAYER				g_leg[LR];						// プレイヤーの太もも

static PLAYER				g_weapon;                       // プレイヤーの武器

float						g_move_time;					// アニメーション進行時間
float						g_walkmove_time;				// アニメーション進行時間
float						g_jumpmove_time;				// アニメーション進行時間

bool						g_WalkMotion;					// 歩きモーション再生 ON/OFF
bool						g_JumpMotion;					// ジャンプモーション再生 ON/OFF

float						roty;							// プレイヤーのY軸に対しての向き

PLAYER_SET set_tbl[] = {    //pos,rot,scl 各パーツの初期値
	{	D3DXVECTOR3(0.0f, 0.0f, 0.0f),		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.9f, 0.9f, 0.9f)},//体			
	{	D3DXVECTOR3(0.0f, 59.0f, -7.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.9f, 0.9f, 0.9f)},//頭部		
	{	D3DXVECTOR3(-9.0f, 52.0f, 0.5f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.9f, 0.9f, 0.9f)},//右腕
	{	D3DXVECTOR3(9.0f, 52.0f, 0.5f),		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.9f, 0.9f, 0.9f)},//左腕
	{	D3DXVECTOR3(-0.5f, 33.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.9f, 0.9f, 0.9f)},//右太もも
	{	D3DXVECTOR3(0.5f, 33.0f, 0.0f),		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.9f, 0.9f, 0.9f)},//左太もも
};

//歩きモーション
D3DXVECTOR3 body_walk_tbl[] = { //体

	D3DXVECTOR3(-0.100000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.143633f, -5.50000f, 0.000000f),
	D3DXVECTOR3(-0.100000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 head_walk_tbl[] = { //頭部

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.001f, -0.000050f, 0.000000f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 armR_walk_tbl[] = { //右腕

	D3DXVECTOR3(-0.374533f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.574533f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.374533f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 armL_walk_tbl[] = { //左腕

	D3DXVECTOR3(-0.374533f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.574533f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.374533f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 leg1R_walk_tbl[] = { //右太もも

	D3DXVECTOR3(-0.323599f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.323599f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.323599f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 leg1L_walk_tbl[] = { //左太もも

	D3DXVECTOR3(0.323599f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.323599f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.323599f, 0.000000f, 0.000000f),
};

// ジャンプモーション1
D3DXVECTOR3 body_jump_tbl1[] = { //体

	D3DXVECTOR3(0.000000f, 1.218163f, 0.000000f),
	D3DXVECTOR3(0.230900f, 1.218163f, 0.000000f),
	D3DXVECTOR3(0.230900f, 1.218163f, 0.000000f),
	D3DXVECTOR3(-0.130900f, 1.218161f, 0.000000f),
	D3DXVECTOR3(-0.100000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 head_jump_tbl1[] = { //頭部

	D3DXVECTOR3(-0.01f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.01f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.01f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.0f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.0f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 armR_jump_tbl1[] = { //右腕

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(1.0f, -1.0f, -0.500000f),
	D3DXVECTOR3(1.0f, -2.0f, -0.500000f),
	D3DXVECTOR3(1.0f, -1.0f, -0.5f),
	D3DXVECTOR3(-0.374533f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 armL_jump_tbl1[] = { //左腕

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.567232f, 0.654498f, 0.000000f),
	D3DXVECTOR3(0.800000f, 0.854498f, 0.000000f),
	D3DXVECTOR3(0.567232f, 0.654498f, 0.000000f),
	D3DXVECTOR3(-0.374533f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 legR_jump_tbl1[] = { //右太もも

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.518166f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.118166f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.518166f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.218166f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 legL_jump_tbl1[] = { //左太もも

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.518166f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.118166f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.518166f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.018166f, 0.000000f, 0.000000f),
};

// ジャンプモーション2
D3DXVECTOR3 body_jump_tbl2[] = { //体

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-D3DX_PI, 0.000000f, 0.000000f),
	D3DXVECTOR3(-D3DX_PI * 2, 0.000000f, 0.000000f),
	D3DXVECTOR3(-D3DX_PI * 2, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 head_jump_tbl2[] = { //頭部

	D3DXVECTOR3(-0.01f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.01f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.01f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.0f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.0f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 armR_jump_tbl2[] = { //右腕

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(1.0f, -1.0f, -0.500000f),
	D3DXVECTOR3(1.0f, -2.0f, -0.500000f),
	D3DXVECTOR3(1.0f, -1.0f, -0.5f),
	D3DXVECTOR3(-0.374533f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 armL_jump_tbl2[] = { //左腕

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.567232f, 0.654498f, 0.000000f),
	D3DXVECTOR3(0.800000f, 0.854498f, 0.000000f),
	D3DXVECTOR3(0.567232f, 0.654498f, 0.000000f),
	D3DXVECTOR3(-0.374533f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 legR_jump_tbl2[] = { //右太もも

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.818166f, 0.000000f, 0.200000f),
	D3DXVECTOR3(0.818166f, 0.000000f, 0.200000f),
	D3DXVECTOR3(0.818166f, 0.000000f, 0.100000f),
	D3DXVECTOR3(0.218166f, 0.000000f, 0.050000f),
};

D3DXVECTOR3 legL_jump_tbl2[] = { //左太もも

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.818166f, 0.000000f, -0.200000f),
	D3DXVECTOR3(0.818166f, 0.000000f, -0.200000f),
	D3DXVECTOR3(0.818166f, 0.000000f, -0.100000f),
	D3DXVECTOR3(0.018166f, 0.000000f, 0.050000f),
};

//攻撃モーション1
D3DXVECTOR3 body_attack_tbl1[] = { //体

	D3DXVECTOR3(0.000000f, -0.372664f, 0.000000f),
	D3DXVECTOR3(-0.130900f, -0.718161f, -0.118166f),
	D3DXVECTOR3(-0.130900f, -0.718163f, -0.118166f),
	D3DXVECTOR3(-0.130900f, -0.718161f, -0.118166f),
	D3DXVECTOR3(0.000000f, -0.372664f, 0.000000f),
};

D3DXVECTOR3 head_attack_tbl1[] = { //頭部

	D3DXVECTOR3(-0.01f, -0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, -0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, -0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, -0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, -0.3f, 0.000000f),
};

D3DXVECTOR3 armR_attack_tbl1[] = { //右腕

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(1.0f, 1.0f, -0.5f),
	D3DXVECTOR3(1.0f, 1.5f, -0.500000f),
	D3DXVECTOR3(1.0f, -2.0f, -0.500000f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 armL_attack_tbl1[] = { //左腕

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.000000f, 0.654498f, 0.000000f),
	D3DXVECTOR3(0.000000f, 0.654498f, 0.000000f),
	D3DXVECTOR3(-0.567232f, 0.654498f, 0.000000f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 legR_attack_tbl1[] = { //右太もも

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.218166f, 0.000000f, -0.261799f),
	D3DXVECTOR3(-0.218166f, 0.000000f, -0.261799f),
	D3DXVECTOR3(-0.218166f, 0.000000f, -0.261799f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 legL_attack_tbl1[] = { //左太もも

	D3DXVECTOR3(0.000000f, -0.785398f, -0.000000f),
	D3DXVECTOR3(0.392699f, -0.785398f, -0.300000f),
	D3DXVECTOR3(0.392699f, -0.785398f, -0.300000f),
	D3DXVECTOR3(0.392699f, -0.785398f, -0.300000f),
	D3DXVECTOR3(0.000000f, -0.785398f, -0.000000f),
};

//攻撃モーション2
D3DXVECTOR3 body_attack_tbl2[] = { //体

	D3DXVECTOR3(0.000000f, 0.872664f, 0.000000f),
	D3DXVECTOR3(-0.130900f, 0.872662f, -0.118166f),
	D3DXVECTOR3(-0.130900f, 1.218163f, -0.118166f),
	D3DXVECTOR3(-0.130900f, 1.218161f, -0.118166f),
	D3DXVECTOR3(0.000000f, 0.872664f, 0.000000f),
};

D3DXVECTOR3 head_attack_tbl2[] = { //頭部

	D3DXVECTOR3(-0.01f, 0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, 0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, 0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, 0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, 0.3f, 0.000000f),
};

D3DXVECTOR3 armR_attack_tbl2[] = { //右腕

	D3DXVECTOR3(1.0f, -2.0f, -0.500000f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(1.0f, 1.0f, -0.500000f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 armL_attack_tbl2[] = { //左腕

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.000000f, 0.654498f, 0.000000f),
	D3DXVECTOR3(0.000000f, 0.654498f, 0.000000f),
	D3DXVECTOR3(-0.567232f, 0.654498f, 0.000000f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 legR_attack_tbl2[] = { //右太もも

	D3DXVECTOR3(0.000000f, 0.785398f, 0.000000f),
	D3DXVECTOR3(-0.392699f, 0.785398f, 0.000000f),
	D3DXVECTOR3(-0.392699f, 0.785398f, -0.261799f),
	D3DXVECTOR3(-0.392699f, 0.785398f, -0.261799f),
	D3DXVECTOR3(0.000000f, 0.785398f, 0.000000f),
};

D3DXVECTOR3 legL_attack_tbl2[] = { //左太もも

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.218166f, 0.000000f, -0.261799f),
	D3DXVECTOR3(-0.218166f, 0.000000f, -0.261799f),
	D3DXVECTOR3(-0.218166f, 0.000000f, -0.261799f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
};

//攻撃モーション3
D3DXVECTOR3 body_attack_tbl3[] = { //体

	D3DXVECTOR3(0.000000f, 0.872664f, 0.000000f),
	D3DXVECTOR3(-0.130900f, 0.872662f, -0.118166f),
	D3DXVECTOR3(-0.130900f, 1.218163f, -0.118166f),
	D3DXVECTOR3(-0.130900f, 1.218161f, -0.118166f),
	D3DXVECTOR3(0.000000f, 0.872664f, 0.000000f),
};

D3DXVECTOR3 head_attack_tbl3[] = { //頭部

	D3DXVECTOR3(-0.01f, -0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, -0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, -0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, -0.3f, 0.000000f),
	D3DXVECTOR3(0.0f, 0.0f, 0.000000f),
};

D3DXVECTOR3 armR_attack_tbl3[] = { //右腕

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(1.0f, -1.0f, -0.500000f),
	D3DXVECTOR3(1.0f, -2.0f, -0.500000f),
	D3DXVECTOR3(1.0f, -1.0f, -0.5f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 armL_attack_tbl3[] = { //左腕

	D3DXVECTOR3(1.5f, 0.8f, -0.600000f),
	D3DXVECTOR3(1.50000f,0.8000f, -0.600000f),
	D3DXVECTOR3(1.5f, 0.3f, -0.600000f),
	D3DXVECTOR3(1.5f, 0.3f, -0.600000f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 legR_attack_tbl3[] = { //右太もも

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.218166f, 0.000000f, -0.261799f),
	D3DXVECTOR3(-0.218166f, 0.000000f, -0.261799f),
	D3DXVECTOR3(-0.218166f, 0.000000f, -0.261799f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 legL_attack_tbl3[] = { //左太もも

	D3DXVECTOR3(0.000000f, -0.785398f, 0.000000f),
	D3DXVECTOR3(0.392699f, -0.785398f, 0.000000f),
	D3DXVECTOR3(0.392699f, -0.785398f, -0.261799f),
	D3DXVECTOR3(0.392699f, -0.785398f, -0.261799f),
	D3DXVECTOR3(0.000000f, -0.785398f, 0.000000f),
};

//攻撃モーション4
D3DXVECTOR3 body_attack_tbl4[] = { //体

	D3DXVECTOR3(0.000000f, 0.872664f, 0.000000f),
	D3DXVECTOR3(-0.130900f, 0.872662f, -0.118166f),
	D3DXVECTOR3(-0.130900f, 1.218163f, -0.118166f),
	D3DXVECTOR3(-0.130900f, 1.218161f, -0.118166f),
	D3DXVECTOR3(0.000000f, 0.872664f, 0.000000f),
};

D3DXVECTOR3 head_attack_tbl4[] = { //頭部

	D3DXVECTOR3(-0.01f, 0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, 0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, 0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, 0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, 0.3f, 0.000000f),
};

D3DXVECTOR3 armR_attack_tbl4[] = { //右腕

	D3DXVECTOR3(1.0f, -2.0f, -0.500000f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(1.0f, 1.0f, -0.500000f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 armL_attack_tbl4[] = { //左腕

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.000000f, 0.654498f, 0.000000f),
	D3DXVECTOR3(0.000000f, 0.654498f, 0.000000f),
	D3DXVECTOR3(-0.567232f, 0.654498f, 0.000000f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 legR_attack_tbl4[] = { //右太もも

	D3DXVECTOR3(0.000000f, 0.785398f, 0.000000f),
	D3DXVECTOR3(-0.392699f, 0.785398f, 0.000000f),
	D3DXVECTOR3(-0.392699f, 0.785398f, -0.261799f),
	D3DXVECTOR3(-0.392699f, 0.785398f, -0.261799f),
	D3DXVECTOR3(0.000000f, 0.785398f, 0.000000f),
};

D3DXVECTOR3 legL_attack_tbl4[] = { //左太もも

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.218166f, 0.000000f, -0.261799f),
	D3DXVECTOR3(-0.218166f, 0.000000f, -0.261799f),
	D3DXVECTOR3(-0.218166f, 0.000000f, -0.261799f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	roty = 0.0f;
	g_move_time = 0.0f;
	g_walkmove_time = 0.0f;
	g_jumpmove_time = 0.0f;
	g_WalkMotion = false;
	g_JumpMotion = false;

	//体
	// 位置・回転・スケールの初期設定
	g_Player.pos = set_tbl[BODY].pos;
	g_Player.rot = set_tbl[BODY].rot;
	g_Player.scl = set_tbl[BODY].scl;

	g_Player.spd = 0.0f;		// 移動スピードクリア

	g_Player.AttackMotion = false;
	g_Player.use = true;					//生存
	g_Player.attackpoint = 0;
	g_Player.jumppoint = 0;
	g_Player.protect = false;
	g_Player.protect_cnt = 0;

	g_Player.player_hitstop = false;
	g_Player.player_hitstop_cnt = 0;

	// ジャンプの初期化
	g_Player.jump_y = PLAYER_JUMP_Y;							// ジャンプ時の高さ
	g_Player.jump_cnt = 0;										// ジャンプカウント初期化
	g_Player.jump_cnt_max = PLAYER_JUMP_CNT_MAX;				// ジャンプカウントマックス初期化
	g_Player.ofs = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// オフセット座標を初期化
	g_Player.jump = false;

	if (type == 0)
	{
		g_Player.pD3DTexture = NULL;
		g_Player.pD3DXMesh = NULL;
		g_Player.pD3DXBuffMat = NULL;
		g_Player.nNumMat = 0;
		g_Player.parent = NULL;

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(MODEL_PLAYER,			// 読み込むモデルファイル名(Xファイル)
			D3DXMESH_SYSTEMMEM,								// メッシュの作成オプションを指定
			pDevice,										// IDirect3DDevice9インターフェイスへのポインタ
			NULL,											// 隣接性データを含むバッファへのポインタ
			&g_Player.pD3DXBuffMat,							// マテリアルデータを含むバッファへのポインタ
			NULL,											// エフェクトインスタンスの配列を含むバッファへのポインタ
			&g_Player.nNumMat,								// D3DXMATERIAL構造体の数
			&g_Player.pD3DXMesh)))							// ID3DXMeshインターフェイスへのポインタのアドレス
		{
			return E_FAIL;
		}
	}

#if 0
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_FILENAME,		// ファイルの名前
		&g_Player.pD3DTexture);	// 読み込むメモリー
#endif

	// 影の初期化
	D3DXVECTOR3 pos = g_Player.pos;
	pos.y = 0.0f;
	g_Player.shadowIdx = CreateShadow(pos, g_Player.scl);

	//頭部
	// 位置・回転・スケールの初期設定
	g_head.pos = set_tbl[HEAD].pos;
	g_head.rot = set_tbl[HEAD].rot;
	g_head.scl = set_tbl[HEAD].scl;

	g_head.spd = 0.0f;		// 移動スピードクリア


	if (type == 0)
	{
		g_head.pD3DTexture = NULL;
		g_head.pD3DXMesh = NULL;
		g_head.pD3DXBuffMat = NULL;
		g_head.nNumMat = 0;
		g_head.parent = &g_Player;

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(MODEL_PLAYER_HEAD,			// 読み込むモデルファイル名(Xファイル)
			D3DXMESH_SYSTEMMEM,									// メッシュの作成オプションを指定
			pDevice,											// IDirect3DDevice9インターフェイスへのポインタ
			NULL,												// 隣接性データを含むバッファへのポインタ
			&g_head.pD3DXBuffMat,								// マテリアルデータを含むバッファへのポインタ
			NULL,												// エフェクトインスタンスの配列を含むバッファへのポインタ
			&g_head.nNumMat,									// D3DXMATERIAL構造体の数
			&g_head.pD3DXMesh)))								// ID3DXMeshインターフェイスへのポインタのアドレス
		{
			return E_FAIL;
		}
	}

#if 0
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_FILENAME,		// ファイルの名前
		&pD3DTexture_head);	// 読み込むメモリー
#endif

		// 位置・回転・スケールの初期設定

	for (int i = 0; i < LR; i++)
	{				//腕
				// 位置・回転・スケールの初期設定
		g_arm[i].pos = set_tbl[ARM_R + i].pos;
		g_arm[i].rot = set_tbl[ARM_R + i].rot;
		g_arm[i].scl = set_tbl[ARM_R + i].scl;

		g_arm[i].spd = 0.0f;		// 移動スピードクリア

		if (type == 0)
		{
			g_arm[i].pD3DTexture = NULL;
			g_arm[i].pD3DXMesh = NULL;
			g_arm[i].pD3DXBuffMat = NULL;
			g_arm[i].nNumMat = 0;
			g_arm[i].parent = &g_Player;

			if (i == R)
			{
				// Xファイルの読み込み
				if (FAILED(D3DXLoadMeshFromX(MODEL_PLAYER_ARM_R,			// 読み込むモデルファイル名(Xファイル)
					D3DXMESH_SYSTEMMEM,										// メッシュの作成オプションを指定
					pDevice,												// IDirect3DDevice9インターフェイスへのポインタ
					NULL,													// 隣接性データを含むバッファへのポインタ
					&g_arm[i].pD3DXBuffMat,									// マテリアルデータを含むバッファへのポインタ
					NULL,													// エフェクトインスタンスの配列を含むバッファへのポインタ
					&g_arm[i].nNumMat,										// D3DXMATERIAL構造体の数
					&g_arm[i].pD3DXMesh)))									// ID3DXMeshインターフェイスへのポインタのアドレス
				{
					return E_FAIL;
				}
			}
			if (i == L)
			{
				// Xファイルの読み込み
				if (FAILED(D3DXLoadMeshFromX(MODEL_PLAYER_ARM_L,			// 読み込むモデルファイル名(Xファイル)
					D3DXMESH_SYSTEMMEM,										// メッシュの作成オプションを指定
					pDevice,												// IDirect3DDevice9インターフェイスへのポインタ
					NULL,													// 隣接性データを含むバッファへのポインタ
					&g_arm[i].pD3DXBuffMat,									// マテリアルデータを含むバッファへのポインタ
					NULL,													// エフェクトインスタンスの配列を含むバッファへのポインタ
					&g_arm[i].nNumMat,										// D3DXMATERIAL構造体の数
					&g_arm[i].pD3DXMesh)))									// ID3DXMeshインターフェイスへのポインタのアドレス
				{
					return E_FAIL;
				}
			}
		}

#if 0
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_FILENAME,		// ファイルの名前
			&g_arm[i].pD3DTexture);	// 読み込むメモリー
#endif
		//太もも
		// 位置・回転・スケールの初期設定
		g_leg[i].pos = set_tbl[LEG_R + i].pos;
		g_leg[i].rot = set_tbl[LEG_R + i].rot;
		g_leg[i].scl = set_tbl[LEG_R + i].scl;

		g_leg[i].spd = 0.0f;		// 移動スピードクリア

		if (type == 0)
		{
			g_leg[i].pD3DTexture = NULL;
			g_leg[i].pD3DXMesh = NULL;
			g_leg[i].pD3DXBuffMat = NULL;
			g_leg[i].nNumMat = 0;
			g_leg[i].parent = &g_Player;

			if (i == R)
			{
				// Xファイルの読み込み
				if (FAILED(D3DXLoadMeshFromX(MODEL_PLAYER_LEG_R,			// 読み込むモデルファイル名(Xファイル)
					D3DXMESH_SYSTEMMEM,										// メッシュの作成オプションを指定
					pDevice,												// IDirect3DDevice9インターフェイスへのポインタ
					NULL,													// 隣接性データを含むバッファへのポインタ
					&g_leg[i].pD3DXBuffMat,									// マテリアルデータを含むバッファへのポインタ
					NULL,													// エフェクトインスタンスの配列を含むバッファへのポインタ
					&g_leg[i].nNumMat,										// D3DXMATERIAL構造体の数
					&g_leg[i].pD3DXMesh)))									// ID3DXMeshインターフェイスへのポインタのアドレス
				{
					return E_FAIL;
				}
			}
			if (i == L)
			{
				// Xファイルの読み込み
				if (FAILED(D3DXLoadMeshFromX(MODEL_PLAYER_LEG_L,			// 読み込むモデルファイル名(Xファイル)
					D3DXMESH_SYSTEMMEM,										// メッシュの作成オプションを指定
					pDevice,												// IDirect3DDevice9インターフェイスへのポインタ
					NULL,													// 隣接性データを含むバッファへのポインタ
					&g_leg[i].pD3DXBuffMat,									// マテリアルデータを含むバッファへのポインタ
					NULL,													// エフェクトインスタンスの配列を含むバッファへのポインタ
					&g_leg[i].nNumMat,										// D3DXMATERIAL構造体の数
					&g_leg[i].pD3DXMesh)))									// ID3DXMeshインターフェイスへのポインタのアドレス
				{
					return E_FAIL;
				}
			}
		}

#if 0
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_FILENAME,		// ファイルの名前
			&g_leg1[i].pD3DTexture);	// 読み込むメモリー
#endif
	}


	// 武器
	// 位置・回転・スケールの初期設定
	g_weapon.pos = D3DXVECTOR3(-11.5f, -13.5f, 25.5f);
	g_weapon.rot = D3DXVECTOR3(-1.6f, 0.0f, 0.0f);
	g_weapon.scl = D3DXVECTOR3(0.7f, 0.7f, 0.7f);

	g_weapon.spd = 0.0f;		// 移動スピードクリア

	if (type == 0)
	{
		g_weapon.pD3DTexture = NULL;
		g_weapon.pD3DXMesh = NULL;
		g_weapon.pD3DXBuffMat = NULL;
		g_weapon.nNumMat = 0;
		g_weapon.parent = &g_arm[R];

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(MODEL_PLAYER_WEAPON,			// 読み込むモデルファイル名(Xファイル)
			D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
			pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
			NULL,					// 隣接性データを含むバッファへのポインタ
			&g_weapon.pD3DXBuffMat,	// マテリアルデータを含むバッファへのポインタ
			NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
			&g_weapon.nNumMat,		// D3DXMATERIAL構造体の数
			&g_weapon.pD3DXMesh)))	// ID3DXMeshインターフェイスへのポインタのアドレス
		{
			return E_FAIL;
		}
	}

#if 0
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_FILENAME,		// ファイルの名前
		&g_weapon.pD3DTexture);	// 読み込むメモリー
#endif

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	if (g_Player.pD3DTexture != NULL)
	{// テクスチャの開放
		g_Player.pD3DTexture->Release();
		g_Player.pD3DTexture = NULL;
	}
	if (g_Player.pD3DXMesh != NULL)
	{// メッシュの開放
		g_Player.pD3DXMesh->Release();
		g_Player.pD3DXMesh = NULL;
	}
	if (g_Player.pD3DXBuffMat != NULL)
	{// マテリアルの開放
		g_Player.pD3DXBuffMat->Release();
		g_Player.pD3DXBuffMat = NULL;
	}

	if (g_head.pD3DTexture != NULL)
	{// テクスチャの開放
		g_head.pD3DTexture->Release();
		g_head.pD3DTexture = NULL;
	}
	if (g_head.pD3DXMesh != NULL)
	{// メッシュの開放
		g_head.pD3DXMesh->Release();
		g_head.pD3DXMesh = NULL;
	}
	if (g_head.pD3DXBuffMat != NULL)
	{// マテリアルの開放
		g_head.pD3DXBuffMat->Release();
		g_head.pD3DXBuffMat = NULL;
	}

	for (int i = 0; i < LR; i++)
	{
		if (g_arm[i].pD3DTexture != NULL)
		{// テクスチャの開放
			g_arm[i].pD3DTexture->Release();
			g_arm[i].pD3DTexture = NULL;
		}
		if (g_arm[i].pD3DXMesh != NULL)
		{// メッシュの開放
			g_arm[i].pD3DXMesh->Release();
			g_arm[i].pD3DXMesh = NULL;
		}
		if (g_arm[i].pD3DXBuffMat != NULL)
		{// マテリアルの開放
			g_arm[i].pD3DXBuffMat->Release();
			g_arm[i].pD3DXBuffMat = NULL;
		}

		if (g_leg[i].pD3DTexture != NULL)
		{// テクスチャの開放
			g_leg[i].pD3DTexture->Release();
			g_leg[i].pD3DTexture = NULL;
		}
		if (g_leg[i].pD3DXMesh != NULL)
		{// メッシュの開放
			g_leg[i].pD3DXMesh->Release();
			g_leg[i].pD3DXMesh = NULL;
		}
		if (g_leg[i].pD3DXBuffMat != NULL)
		{// マテリアルの開放
			g_leg[i].pD3DXBuffMat->Release();
			g_leg[i].pD3DXBuffMat = NULL;
		}
	}

	if (g_weapon.pD3DTexture != NULL)
	{// テクスチャの開放
		g_weapon.pD3DTexture->Release();
		g_weapon.pD3DTexture = NULL;
	}
	if (g_weapon.pD3DXMesh != NULL)
	{// メッシュの開放
		g_weapon.pD3DXMesh->Release();
		g_weapon.pD3DXMesh = NULL;
	}
	if (g_weapon.pD3DXBuffMat != NULL)
	{// マテリアルの開放
		g_weapon.pD3DXBuffMat->Release();
		g_weapon.pD3DXBuffMat = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	CAMERA *cam = GetCamera();
	ENEMY *enemy = GetEnemy();

	PrintDebugProc("攻撃中? %d \n", g_Player.AttackMotion);
	PrintDebugProc("攻撃回数 %d \n", g_Player.attackpoint);
	PrintDebugProc("roty	 %f \n", roty);

	float	dt = 0.08f;
	//武器座標の取得
	D3DXVECTOR3 Wpos, Ws, We;
	GetweaponPos(Wpos, Ws, We);

	if (g_Player.attackpoint < PLAYER_ATTACK_MAX)			//連続攻撃回数以内か？
	{
		if ((GetKeyboardTrigger(DIK_SPACE)) || (IsButtonTriggered(0, BUTTON_A))) //攻撃モーション 入力確認
		{
			if (g_Player.AttackMotion == false)
			{
				g_Player.AttackMotion = true;				//攻撃する
				GetEffectNo(2);
				SetEffect(D3DXVECTOR3(Wpos.x, Wpos.y + 10.0f, Wpos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), EFFECT_SIZE_X, EFFECT_SIZE_Y, 20);

				// SE再生
				PlaySound(SOUND_LABEL_SE_ATTACK2);
			}
		}
	}

	// 移動処理
	int		dir = 0;	// ０：向きは変えない

	if ((GetKeyboardPress(DIK_A)) || (IsButtonPressed(0, BUTTON_LEFT)))		//左移動
	{
		dir |= 8;
		g_Player.spd = VALUE_MOVE;
		g_WalkMotion = true;

		if ((GetKeyboardPress(DIK_W)) || (IsButtonPressed(0, BUTTON_UP)))	//左上移動
		{
			dir |= 10;
		}
		else if ((GetKeyboardPress(DIK_S)) || (IsButtonPressed(0, BUTTON_DOWN)))	//左下移動
		{
			dir |= 9;
		}

	}
	else if ((GetKeyboardPress(DIK_D)) || (IsButtonPressed(0, BUTTON_RIGHT)))	//右移動
	{
		dir |= 4;
		g_Player.spd = VALUE_MOVE;
		g_WalkMotion = true;

		if ((GetKeyboardPress(DIK_W)) || (IsButtonPressed(0, BUTTON_UP)))		//右上移動
		{
			dir |= 6;
		}
		else if ((GetKeyboardPress(DIK_S)) || (IsButtonPressed(0, BUTTON_DOWN)))	//右下移動
		{
			dir |= 5;
		}
	}
	else if ((GetKeyboardPress(DIK_W)) || (IsButtonPressed(0, BUTTON_UP)))
	{
		dir |= 2;
		g_Player.spd = VALUE_MOVE;
		g_WalkMotion = true;

	}
	else if ((GetKeyboardPress(DIK_S)) || (IsButtonPressed(0, BUTTON_DOWN)))
	{
		dir |= 1;
		g_Player.spd = VALUE_MOVE;
		g_WalkMotion = true;

	}
	else //移動してなかったらモーションをやめる
	{
		g_WalkMotion = false;
		g_walkmove_time = 0.0f;
	}

	// 入力されたキーに合わせて向きを決める
	switch (dir)
	{
	case 1:	// 下向き
		if (roty > 0.0f)
		{
			roty -= 0.1f;
			if (roty < 0.0f)
			{
				roty = 0.0f;
			}
		}
		else
		{
			roty += 0.1f;
			if (roty > 0.0f)
			{
				roty = 0.0f;
			}

		}
		break;

	case 2:	// 上向き
		if (roty > 0.0f)
		{
			roty += 0.1f;
			if (roty > D3DX_PI)
			{
				roty = D3DX_PI;
			}
		}
		else
		{
			roty -= 0.1f;
			if (roty < -D3DX_PI)
			{
				roty = D3DX_PI;
			}

		}
		break;

	case 4:	// 右向き
		if (((roty > D3DX_PI / 2) && (roty < D3DX_PI)) || ((roty < -D3DX_PI / 2)))
		{
			roty += 0.1f;
			if (roty >= D3DX_PI)
			{
				roty = -D3DX_PI;
			}
		}
		else
		{
			roty -= 0.1f;
			if (roty < -D3DX_PI / 2)
			{
				roty = -D3DX_PI / 2;
			}
		}
		break;

	case 8:	// 左向き
		if ((roty > -D3DX_PI / 2) && (roty < D3DX_PI / 2))
		{
			roty += 0.1f;
			if (roty > D3DX_PI / 2)
			{
				roty = D3DX_PI / 2;
			}
		}
		else if (((roty <= -D3DX_PI / 2) && (roty > -D3DX_PI)) || ((roty > D3DX_PI / 2)))
		{
			roty -= 0.1f;
			if (roty <= -D3DX_PI)
			{
				roty = D3DX_PI;
			}
		}
		break;

	case 5:	// 右下向き
		roty = -D3DX_PI / 4;
		break;

	case 6:	// 右上向き
		roty = -D3DX_PI / 4 * 3;
		break;

	case 9:	// 左下向き
		roty = D3DX_PI / 4;
		break;

	case 10: // 左上向き
		roty = D3DX_PI / 4 * 3;
		break;
	}

	//アニメーション(歩き)
	if (g_WalkMotion == true) //TRUEなら実行
	{
		dt = 0.05f;

		g_walkmove_time += dt;		// アニメーションの合計時間に足す

		int index = (int)g_walkmove_time;
		float	time = g_walkmove_time - index;
		int		size = sizeof(body_walk_tbl) / sizeof(D3DXVECTOR3);; //テーブル全体のバイト数をテーブル一つのバイト数で割る(テーブル数を求める)

		if (index > (size - 2))	// ゴールをオーバーしていたら、ゴールへ戻す
		{
			g_walkmove_time = 0.0f;
			index = 0;
		}

		// アニメーションさせる部位	　X = StartX + (EndX - StartX) * 今の時間
		g_Player.vec = body_walk_tbl[index + 1] - body_walk_tbl[index];
		g_head.vec = head_walk_tbl[index + 1] - head_walk_tbl[index];

		g_Player.rot = body_walk_tbl[index] + g_Player.vec * time;
		g_head.rot = head_walk_tbl[index] + g_head.vec * time;

		for (int i = 0; i < LR; i++)
		{
			if (i == R)
			{
				g_arm[i].vec = armR_walk_tbl[index + 1] - armR_walk_tbl[index];
				g_leg[i].vec = leg1R_walk_tbl[index + 1] - leg1R_walk_tbl[index];

				g_arm[i].rot = armR_walk_tbl[index] + g_arm[i].vec * time;
				g_leg[i].rot = leg1R_walk_tbl[index] + g_leg[i].vec * time;
			}
			if (i == L)
			{
				g_arm[i].vec = armL_walk_tbl[index + 1] - armL_walk_tbl[index];
				g_leg[i].vec = leg1L_walk_tbl[index + 1] - leg1L_walk_tbl[index];

				g_arm[i].rot = armL_walk_tbl[index] + g_arm[i].vec * time;
				g_leg[i].rot = leg1L_walk_tbl[index] + g_leg[i].vec * time;

			}
		}
	}

	if (g_Player.AttackMotion == TRUE) //攻撃モーション TRUEなら実行
	{
		g_WalkMotion = false;
		dt = 0.1f;
		g_move_time += dt;		// アニメーションの合計時間に足す

		int index = (int)g_move_time;
		float	time = g_move_time - index;
		int		size = sizeof(body_attack_tbl1) / sizeof(D3DXVECTOR3);; //テーブル全体のバイト数をテーブル一つのバイト数で割る(テーブル数を求める)

		if (g_Player.attackpoint < PLAYER_ATTACK_MAX)
		{
			if ((GetKeyboardTrigger(DIK_SPACE)) || (IsButtonTriggered(0, BUTTON_A))) //攻撃モーション 入力確認
			{
				g_move_time = 0.0f;
				index = 0;
				g_Player.attackpoint++;

				if (g_Player.attackpoint % 2 == 0)
				{
					GetEffectNo(1);
				}
				else if (g_Player.attackpoint % 2 == 1)
				{
					GetEffectNo(2);
				}

				SetEffect(D3DXVECTOR3(Wpos.x, Wpos.y + 10.0f, Wpos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), EFFECT_SIZE_X, EFFECT_SIZE_Y, 20);

				// SE再生
				PlaySound(SOUND_LABEL_SE_ATTACK2);
			}
		}

		if (index > (size - 2))	// ゴールをオーバーしていたら、ゴールへ戻す
		{
			g_move_time = 0.0f;
			index = 0;
			g_Player.AttackMotion = false;
			g_Player.attackpoint = 0;
		}

		switch (g_Player.attackpoint)
		{
		case 1:
			// アニメーションさせる部位	　X = StartX + (EndX - StartX) * 今の時間
			g_Player.vec = body_attack_tbl1[index + 1] - body_attack_tbl1[index];
			g_head.vec = head_attack_tbl1[index + 1] - head_attack_tbl1[index];

			g_Player.rot = body_attack_tbl1[index] + g_Player.vec * time;
			g_head.rot = head_attack_tbl1[index] + g_head.vec * time;

			for (int i = 0; i < LR; i++)
			{
				if (i == R)
				{
					g_arm[i].vec = armR_attack_tbl1[index + 1] - armR_attack_tbl1[index];
					g_leg[i].vec = legR_attack_tbl1[index + 1] - legR_attack_tbl1[index];

					g_arm[i].rot = armR_attack_tbl1[index] + g_arm[i].vec * time;
					g_leg[i].rot = legR_attack_tbl1[index] + g_leg[i].vec * time;
				}
				if (i == L)
				{
					g_arm[i].vec = armL_attack_tbl1[index + 1] - armL_attack_tbl1[index];
					g_leg[i].vec = legL_attack_tbl1[index + 1] - legL_attack_tbl1[index];

					g_arm[i].rot = armL_attack_tbl1[index] + g_arm[i].vec * time;
					g_leg[i].rot = legL_attack_tbl1[index] + g_leg[i].vec * time;
				}
			}
			break;
		case 2:
			// アニメーションさせる部位	　X = StartX + (EndX - StartX) * 今の時間
			g_Player.vec = body_attack_tbl2[index + 1] - body_attack_tbl2[index];
			g_head.vec = head_attack_tbl2[index + 1] - head_attack_tbl2[index];

			g_Player.rot = body_attack_tbl2[index] + g_Player.vec * time;
			g_head.rot = head_attack_tbl2[index] + g_head.vec * time;

			for (int i = 0; i < LR; i++)
			{
				if (i == R)
				{
					g_arm[i].vec = armR_attack_tbl2[index + 1] - armR_attack_tbl2[index];
					g_leg[i].vec = legR_attack_tbl2[index + 1] - legR_attack_tbl2[index];

					g_arm[i].rot = armR_attack_tbl2[index] + g_arm[i].vec * time;
					g_leg[i].rot = legR_attack_tbl2[index] + g_leg[i].vec * time;
				}
				if (i == L)
				{
					g_arm[i].vec = armL_attack_tbl2[index + 1] - armL_attack_tbl2[index];
					g_leg[i].vec = legL_attack_tbl2[index + 1] - legL_attack_tbl2[index];

					g_arm[i].rot = armL_attack_tbl2[index] + g_arm[i].vec * time;
					g_leg[i].rot = legL_attack_tbl2[index] + g_leg[i].vec * time;
				}
			}
			break;
		case 3:
			// アニメーションさせる部位	　X = StartX + (EndX - StartX) * 今の時間
			g_Player.vec = body_attack_tbl3[index + 1] - body_attack_tbl3[index];
			g_head.vec = head_attack_tbl3[index + 1] - head_attack_tbl3[index];

			g_Player.rot = body_attack_tbl3[index] + g_Player.vec * time;
			g_head.rot = head_attack_tbl3[index] + g_head.vec * time;

			for (int i = 0; i < LR; i++)
			{
				if (i == R)
				{
					g_arm[i].vec = armR_attack_tbl3[index + 1] - armR_attack_tbl3[index];
					g_leg[i].vec = legR_attack_tbl3[index + 1] - legR_attack_tbl3[index];

					g_arm[i].rot = armR_attack_tbl3[index] + g_arm[i].vec * time;
					g_leg[i].rot = legR_attack_tbl3[index] + g_leg[i].vec * time;
				}
				if (i == L)
				{
					g_arm[i].vec = armL_attack_tbl3[index + 1] - armL_attack_tbl3[index];
					g_leg[i].vec = legL_attack_tbl3[index + 1] - legL_attack_tbl3[index];

					g_arm[i].rot = armL_attack_tbl3[index] + g_arm[i].vec * time;
					g_leg[i].rot = legL_attack_tbl3[index] + g_leg[i].vec * time;
				}
			}
			break;
		case 4:
			// アニメーションさせる部位	　X = StartX + (EndX - StartX) * 今の時間
			g_Player.vec = body_attack_tbl4[index + 1] - body_attack_tbl4[index];
			g_head.vec = head_attack_tbl4[index + 1] - head_attack_tbl4[index];

			g_Player.rot = body_attack_tbl4[index] + g_Player.vec * time;
			g_head.rot = head_attack_tbl4[index] + g_head.vec * time;

			for (int i = 0; i < LR; i++)
			{
				if (i == R)
				{
					g_arm[i].vec = armR_attack_tbl4[index + 1] - armR_attack_tbl4[index];
					g_leg[i].vec = legR_attack_tbl4[index + 1] - legR_attack_tbl4[index];

					g_arm[i].rot = armR_attack_tbl4[index] + g_arm[i].vec * time;
					g_leg[i].rot = legR_attack_tbl4[index] + g_leg[i].vec * time;
				}
				if (i == L)
				{
					g_arm[i].vec = armL_attack_tbl4[index + 1] - armL_attack_tbl4[index];
					g_leg[i].vec = legL_attack_tbl4[index + 1] - legL_attack_tbl4[index];

					g_arm[i].rot = armL_attack_tbl4[index] + g_arm[i].vec * time;
					g_leg[i].rot = legL_attack_tbl4[index] + g_leg[i].vec * time;
				}
			}
			break;
		}
	}

	// ジャンプ？
	if (g_Player.jumppoint < PLAYER_JUMP_MAX)
	{
		if (GetKeyboardTrigger(DIK_J) || (IsButtonTriggered(0, BUTTON_B)))
		{
			if (g_Player.jump == false)	// 今はジャンプしていない？
			{								// Yes
				g_WalkMotion = false;
				g_JumpMotion = true;
				g_Player.jump = true;
				g_Player.jump_cnt = 0;
				// SE再生
				PlaySound(SOUND_LABEL_SE_JUMP);
			}
		}
	}

	// ジャンプ処理
	if (g_Player.jump == true)
	{
		float rag = D3DX_PI / g_Player.jump_cnt_max;
		float angle = rag * g_Player.jump_cnt;
		g_Player.ofs.y = sinf(angle) * g_Player.jump_y;
		g_Player.jump_cnt++;

		if (g_Player.jump_cnt >= g_Player.jump_cnt_max)	// ジャンプ終了？
		{														// Yes
			g_Player.jump = false;
			g_Player.ofs = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

	}

	if (g_JumpMotion == true) //攻撃モーション TRUEなら実行
	{
		g_WalkMotion = false;
		dt = 0.04f;
		g_jumpmove_time += dt;		// アニメーションの合計時間に足す

		int index = (int)g_jumpmove_time;
		float	time = g_jumpmove_time - index;
		int		size = sizeof(body_jump_tbl1) / sizeof(D3DXVECTOR3);; //テーブル全体のバイト数をテーブル一つのバイト数で割る(テーブル数を求める)

		if (g_Player.jumppoint < PLAYER_JUMP_MAX)
		{
			if (GetKeyboardTrigger(DIK_J) || (IsButtonTriggered(0, BUTTON_B)))
			{
				g_Player.jump_cnt = 0;
				g_jumpmove_time = 0.0f;
				index = 0;
				g_Player.jumppoint++;
				// SE再生
				PlaySound(SOUND_LABEL_SE_JUMP);
			}
		}

		if (index == 2)
		{
			// SE再生
			PlaySound(SOUND_LABEL_SE_JUMPOFF);
		}

		if (index > (size - 2))	// ゴールをオーバーしていたら、ゴールへ戻す
		{
			g_jumpmove_time = 0.0f;
			index = 0;
			g_Player.jumppoint = 0;
			g_JumpMotion = false;
			g_WalkMotion = true;
		}

		switch (g_Player.jumppoint)
		{
		case 0:		//ジャンプ1回目
			// アニメーションさせる部位	　X = StartX + (EndX - StartX) * 今の時間
			g_Player.vec = body_jump_tbl1[index + 1] - body_jump_tbl1[index];
			g_head.vec = head_jump_tbl1[index + 1] - head_jump_tbl1[index];

			g_Player.rot = body_jump_tbl1[index] + g_Player.vec * time;
			g_head.rot = head_jump_tbl1[index] + g_head.vec * time;

			for (int i = 0; i < LR; i++)
			{
				if (i == R)
				{
					g_arm[i].vec = armR_jump_tbl1[index + 1] - armR_jump_tbl1[index];
					g_leg[i].vec = legR_jump_tbl1[index + 1] - legR_jump_tbl1[index];

					g_arm[i].rot = armR_jump_tbl1[index] + g_arm[i].vec * time;
					g_leg[i].rot = legR_jump_tbl1[index] + g_leg[i].vec * time;
				}
				if (i == L)
				{
					g_arm[i].vec = armL_jump_tbl1[index + 1] - armL_jump_tbl1[index];
					g_leg[i].vec = legL_jump_tbl1[index + 1] - legL_jump_tbl1[index];

					g_arm[i].rot = armL_jump_tbl1[index] + g_arm[i].vec * time;
					g_leg[i].rot = legL_jump_tbl1[index] + g_leg[i].vec * time;
				}
			}
			break;

		case 1:		//ジャンプ2回目
			// アニメーションさせる部位	　X = StartX + (EndX - StartX) * 今の時間
			g_Player.vec = body_jump_tbl2[index + 1] - body_jump_tbl2[index];
			g_head.vec = head_jump_tbl2[index + 1] - head_jump_tbl2[index];

			g_Player.rot = body_jump_tbl2[index] + g_Player.vec * time;
			g_head.rot = head_jump_tbl2[index] + g_head.vec * time;

			for (int i = 0; i < LR; i++)
			{
				if (i == R)
				{
					g_arm[i].vec = armR_jump_tbl2[index + 1] - armR_jump_tbl2[index];
					g_leg[i].vec = legR_jump_tbl2[index + 1] - legR_jump_tbl2[index];

					g_arm[i].rot = armR_jump_tbl2[index] + g_arm[i].vec * time;
					g_leg[i].rot = legR_jump_tbl2[index] + g_leg[i].vec * time;
				}
				if (i == L)
				{
					g_arm[i].vec = armL_jump_tbl2[index + 1] - armL_jump_tbl2[index];
					g_leg[i].vec = legL_jump_tbl2[index + 1] - legL_jump_tbl2[index];

					g_arm[i].rot = armL_jump_tbl2[index] + g_arm[i].vec * time;
					g_leg[i].rot = legL_jump_tbl2[index] + g_leg[i].vec * time;
				}
			}
			break;

		}
	}

	{
		// カメラに対して入力のあった方向へプレイヤーを向かせて移動させる
		g_Player.rot.y = roty + cam->rot.y;
		g_Player.pos.x -= sinf(g_Player.rot.y) * g_Player.spd;
		g_Player.pos.y += g_Player.ofs.y + g_Player.spd;
		g_Player.pos.z -= cosf(g_Player.rot.y) * g_Player.spd;

		D3DXMATRIX CameraRoll;
		D3DXMATRIX buff;
		D3DXVECTOR3 PtoC = D3DXVECTOR3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);
		D3DXVECTOR3 PtoAt = D3DXVECTOR3(0.0f, TILT_Y_CAM, TILT_Z_CAM);

		//cam->at.x = PtoAt.x * cosf(cam->rot.y) - PtoAt.z * (-sinf(cam->rot.y));
		//cam->at.y = PtoAt.y;
		//cam->at.z = PtoAt.x * (-sinf(cam->rot.y)) + PtoAt.z * cosf(cam->rot.y);

		//cam->pos.x = PtoC.x * cosf(cam->rot.y) - PtoC.z * (-sinf(cam->rot.y));
		//cam->pos.y = PtoC.y;
		//cam->pos.z = PtoC.x * (-sinf(cam->rot.y)) + PtoC.z * cosf(cam->rot.y);

		//回転行列の初期化
		D3DXMatrixIdentity(&CameraRoll);

		//バッファの初期化
		D3DXMatrixIdentity(&buff);

		//回転移動
		D3DXMatrixRotationYawPitchRoll(&buff, cam->rot.y, cam->rot.x, cam->rot.z);
		D3DXMatrixMultiply(&CameraRoll, &CameraRoll, &buff);

		//バッファの初期化
		D3DXMatrixIdentity(&buff);

		//Playerに追従
		D3DXMatrixTranslation(&buff, g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
		D3DXMatrixMultiply(&CameraRoll, &CameraRoll, &buff);

		D3DXVec3TransformCoord(&cam->at, &PtoAt, &CameraRoll);
		D3DXVec3TransformCoord(&cam->pos, &PtoC, &CameraRoll);

		// カメラの注視点と視点を主人公に追従させる
		//cam->at += g_Player.pos; 
		//cam->pos += g_Player.pos;

	}

	g_Player.spd *= 0.7f;

	// 重力処理
	g_Player.pos.y -= 10.0f;
	if (g_Player.pos.y <= 0.0f)
	{
		g_Player.pos.y = 0.0f;
		g_Player.jumppoint = 0;
	}

	//壁設定
	if (g_Player.pos.x < -1980.0f)
	{
		g_Player.pos.x = -1980.0f;
	}
	if (g_Player.pos.x > 1980.0f)
	{
		g_Player.pos.x = 1980.0f;
	}
	if (g_Player.pos.z < -1980.0f)
	{
		g_Player.pos.z = -1980.0f;
	}
	if (g_Player.pos.z > 1980.0f)
	{
		g_Player.pos.z = 1980.0f;
	}

	// 無敵
	if (g_Player.protect == true)
	{
		g_Player.protect_cnt++;

		if (g_Player.protect_cnt >= 60)
		{
			g_Player.protect_cnt = 0;
			g_Player.protect = false;
		}
	}

	// 影もプレイヤーの位置に合わせる
	D3DXVECTOR3 pos = g_Player.pos;
	pos.y = 0.0f;
	SetPositionShadow(g_Player.shadowIdx, pos, g_Player.scl);

	PrintDebugProc("プレイヤー操作WASD\n\n");

	PrintDebugProc("PlayerPos%ff%ff%ff\n", g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DMATERIAL9 arm_matDef;
	D3DMATERIAL9 leg_matDef;
	D3DMATERIAL9 weapon_matDef;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_weapon.mtxWorld);
	D3DXMatrixIdentity(&g_head.mtxWorld);

	// スケールを反映 S
	D3DXMatrixScaling(&g_weapon.mtxScl, g_weapon.scl.x, g_weapon.scl.y, g_weapon.scl.z);
	D3DXMatrixMultiply(&g_weapon.mtxWorld, &g_weapon.mtxWorld, &g_weapon.mtxScl);
	D3DXMatrixScaling(&g_head.mtxScl, g_head.scl.x, g_head.scl.y, g_head.scl.z);
	D3DXMatrixMultiply(&g_head.mtxWorld, &g_head.mtxWorld, &g_head.mtxScl);

	// 回転を反映 R
	D3DXMatrixRotationYawPitchRoll(&g_weapon.mtxRot, g_weapon.rot.y, g_weapon.rot.x, g_weapon.rot.z);
	D3DXMatrixMultiply(&g_weapon.mtxWorld, &g_weapon.mtxWorld, &g_weapon.mtxRot);
	D3DXMatrixRotationYawPitchRoll(&g_head.mtxRot, g_head.rot.y, g_head.rot.x, g_head.rot.z);
	D3DXMatrixMultiply(&g_head.mtxWorld, &g_head.mtxWorld, &g_head.mtxRot);

	// 移動を反映 T
	D3DXMatrixTranslation(&g_weapon.mtxTranslate, g_weapon.pos.x, g_weapon.pos.y, g_weapon.pos.z);
	D3DXMatrixMultiply(&g_weapon.mtxWorld, &g_weapon.mtxWorld, &g_weapon.mtxTranslate);
	D3DXMatrixTranslation(&g_head.mtxTranslate, g_head.pos.x, g_head.pos.y, g_head.pos.z);
	D3DXMatrixMultiply(&g_head.mtxWorld, &g_head.mtxWorld, &g_head.mtxTranslate);

	for (int i = 0; i < LR; i++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_arm[i].mtxWorld);
		D3DXMatrixIdentity(&g_leg[i].mtxWorld);

		// スケールを反映 S
		D3DXMatrixScaling(&g_arm[i].mtxScl, g_arm[i].scl.x, g_arm[i].scl.y, g_arm[i].scl.z);
		D3DXMatrixMultiply(&g_arm[i].mtxWorld, &g_arm[i].mtxWorld, &g_arm[i].mtxScl);
		D3DXMatrixScaling(&g_leg[i].mtxScl, g_leg[i].scl.x, g_leg[i].scl.y, g_leg[i].scl.z);
		D3DXMatrixMultiply(&g_leg[i].mtxWorld, &g_leg[i].mtxWorld, &g_leg[i].mtxScl);

		// 回転を反映 R
		D3DXMatrixRotationYawPitchRoll(&g_arm[i].mtxRot, g_arm[i].rot.y, g_arm[i].rot.x, g_arm[i].rot.z);
		D3DXMatrixMultiply(&g_arm[i].mtxWorld, &g_arm[i].mtxWorld, &g_arm[i].mtxRot);
		D3DXMatrixRotationYawPitchRoll(&g_leg[i].mtxRot, g_leg[i].rot.y, g_leg[i].rot.x, g_leg[i].rot.z);
		D3DXMatrixMultiply(&g_leg[i].mtxWorld, &g_leg[i].mtxWorld, &g_leg[i].mtxRot);

		// 移動を反映 T
		D3DXMatrixTranslation(&g_arm[i].mtxTranslate, g_arm[i].pos.x, g_arm[i].pos.y, g_arm[i].pos.z);
		D3DXMatrixMultiply(&g_arm[i].mtxWorld, &g_arm[i].mtxWorld, &g_arm[i].mtxTranslate);
		D3DXMatrixTranslation(&g_leg[i].mtxTranslate, g_leg[i].pos.x, g_leg[i].pos.y, g_leg[i].pos.z);
		D3DXMatrixMultiply(&g_leg[i].mtxWorld, &g_leg[i].mtxWorld, &g_leg[i].mtxTranslate);
	}

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Player.mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&g_Player.mtxScl, g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &g_Player.mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&g_Player.mtxRot, g_Player.rot.y, g_Player.rot.x, g_Player.rot.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &g_Player.mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&g_Player.mtxTranslate, g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &g_Player.mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Player.mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&g_Player.matDef);

	// マテリアル情報に対するポインタを取得
	g_Player.pD3DXMat = (D3DXMATERIAL*)g_Player.pD3DXBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_Player.nNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&g_Player.pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_Player.pD3DTexture);

		// 描画
		g_Player.pD3DXMesh->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&g_Player.matDef);

	//子のworld = 子のworld * 親のworld
	D3DXMatrixMultiply(&g_head.mtxWorld, &g_head.mtxWorld, &g_head.parent->mtxWorld);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_head.mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&g_head.matDef);

	// マテリアル情報に対するポインタを取得
	g_head.pD3DXMat = (D3DXMATERIAL*)g_head.pD3DXBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_head.nNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&g_head.pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_head.pD3DTexture);

		// 描画
		g_head.pD3DXMesh->DrawSubset(nCntMat);
	}
	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&g_head.matDef);

	for (int i = 0; i < LR; i++)
	{
		//子のworld = 子のworld * 親のworld
		D3DXMatrixMultiply(&g_arm[i].mtxWorld, &g_arm[i].mtxWorld, &g_arm[i].parent->mtxWorld);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_arm[i].mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&arm_matDef);

		// マテリアル情報に対するポインタを取得
		g_arm[i].pD3DXMat = (D3DXMATERIAL*)g_arm[i].pD3DXBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_arm[i].nNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&g_arm[i].pD3DXMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_arm[i].pD3DTexture);

			// 描画
			g_arm[i].pD3DXMesh->DrawSubset(nCntMat);
		}

		//子のworld = 子のworld * 親のworld
		D3DXMatrixMultiply(&g_leg[i].mtxWorld, &g_leg[i].mtxWorld, &g_leg[i].parent->mtxWorld);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_leg[i].mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&leg_matDef);

		// マテリアル情報に対するポインタを取得
		g_leg[i].pD3DXMat = (D3DXMATERIAL*)g_leg[i].pD3DXBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_leg[i].nNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&g_leg[i].pD3DXMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_leg[i].pD3DTexture);

			// 描画
			g_leg[i].pD3DXMesh->DrawSubset(nCntMat);
		}
	}
	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&arm_matDef);
	pDevice->SetMaterial(&leg_matDef);

	//子のworld = 子のworld * 親のworld
	D3DXMatrixMultiply(&g_weapon.mtxWorld, &g_weapon.mtxWorld, &g_weapon.parent->mtxWorld);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_weapon.mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&weapon_matDef);

	// マテリアル情報に対するポインタを取得
	g_weapon.pD3DXMat = (D3DXMATERIAL*)g_weapon.pD3DXBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_weapon.nNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&g_weapon.pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_weapon.pD3DTexture);

		// 描画
		g_weapon.pD3DXMesh->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&weapon_matDef);
}

//=============================================================================
// プレイヤー情報を取得
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}

//=============================================================================
// 武器情報を取得
//=============================================================================
void GetweaponPos(D3DXVECTOR3 &pos, D3DXVECTOR3 &S, D3DXVECTOR3 &E)
{
	E = S = g_weapon.pos;
	E.y += WEAPON_LEN * sinf(1.3);
	E.z += WEAPON_LEN * cosf(1.3);
	S.y -= WEAPON_LEN * sinf(1.3);
	S.z -= WEAPON_LEN * cosf(1.3);

	D3DXVec3TransformCoord(&pos, &g_weapon.pos, &g_weapon.mtxWorld);
	D3DXVec3TransformCoord(&E, &E, &g_weapon.mtxWorld);
	D3DXVec3TransformCoord(&S, &S, &g_weapon.mtxWorld);

	return;
}

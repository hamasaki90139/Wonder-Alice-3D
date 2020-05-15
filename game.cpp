//=============================================================================
//
// ゲーム画面処理 [game.cpp]
// Author : 濱崎 小春
//
//=============================================================================
#include "game.h"
#include "light.h"
#include "camera.h"
#include "meshfield.h"
#include "meshwall.h"
#include "player.h"
#include "enemy.h"
#include "shadow.h"
#include "meshsky.h"
#include "life.h"
#include "timer.h"
#include "score.h"
#include "item.h"
#include "sound.h"
#include "collision.h"
#include "particle.h"
#include "effect.h"
#include "pose.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGame(void)
{
	// ライトの初期化
	InitLight();

	// カメラの初期化
	InitCamera();

	// フィールドの初期化
	// 空の初期化
	InitMeshSky(D3DXVECTOR3(0.0f, -100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16, 8, 8000.0f, true);

	// 地面の初期化
	InitMeshField();

	// 壁の初期化
	InitMeshWall();

	// 影の初期化
	InitShadow();

	// プレイヤーの初期化
	InitPlayer(0);

	// エネミーの初期化
	InitEnemy(0);

	// ライフの初期化
	InitLife();

	// タイマーの初期化
	InitTimer();
	ResetTimer(TIMER_MAX);

	// スコアの初期化
	InitScore(0);

	// パーティクルの初期化
	InitParticle();

	// エフェクトの初期化
	InitEffect();

	// アイテムの初期化
	InitItem(0);

	InitPose(0);

	// BGM再生
	PlaySound(SOUND_LABEL_GAME);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{
	// カメラの終了処理
	UninitCamera();

	// 空の終了処理
	UninitMeshSky();

	// 地面の終了処理
	UninitMeshField();

	// 壁の終了処理
	UninitMeshWall();

	// 影の終了処理
	UninitShadow();

	// パーティクルの終了処理
	UninitParticle();

	// エフェクトの終了処理
	UninitEffect();

	// エネミーの終了処理
	UninitEnemy();

	// プレイヤーの終了処理
	UninitPlayer();

	// ライフの終了処理
	UninitLife();

	// タイマーの終了処理
	UninitTimer();

	// スコアの終了処理
	UninitScore();

	// アイテムの終了処理
	UninitItem();

	// ポーズの終了処理
	UninitPose();

	// BGM停止
	StopSound(SOUND_LABEL_GAME);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(void)
{
	PLAYER *player = GetPlayer();

	//ゲーム画面中
	if (GetPose() == false)
	{
		if (player->player_hitstop == false)
		{//通常時
			// カメラ更新
			UpdateCamera();

			// 空の更新処理
			UpdateMeshSky();

			// 地面処理の更新
			UpdateMeshField();

			// 壁処理の更新
			UpdateMeshWall();

			// 影処理の更新
			UpdateShadow();

			// パーティクル処理の更新
			UpdateParticle();

			// プレイヤー処理の更新
			UpdatePlayer();

			// エネミー処理の更新
			UpdateEnemy();

			// エフェクト処理の更新
			UpdateEffect();

			// ライフ処理の更新
			UpdateLife();

			// アイテム処理の更新
			UpdateItem();

			//衝突判定の更新
			UpdateBattelCollision();

			//ライト処理の更新
			UpdateLight();
		}
		else if (player->player_hitstop_cnt > PLAYER_HITSTOP)
		{	//ヒットストップ終了
			player->player_hitstop = false;
			player->player_hitstop_cnt = 0;
		}
		else
		{	//ヒットストップ中
			player->player_hitstop_cnt++;
		}

		// タイマー処理の更新
		UpdateTimer();

		// スコア処理の更新
		UpdateScore();
	}

	// ポーズ画面の更新処理
	UpdatePose();

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// カメラの設定
	SetCamera();

	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF
	// 空の描画処理
	DrawMeshSky();

	// 壁処理の描画
	DrawMeshWall();

	// 地面処理の描画
	DrawMeshField();

	// 影処理の描画
	DrawShadow();
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグ：ON

	// エネミー処理の描画
	DrawEnemy();

	// プレイヤー処理の描画
	DrawPlayer();

	// アイテム処理の描画
	DrawItem();

	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF
	// パーティクル処理の描画
	DrawParticle();

	// エフェクト処理の更新
	DrawEffect();

	// ライフ処理の描画
	DrawLife();

	// タイマー処理の描画
	DrawTimer();

	// スコア処理の描画
	DrawScore();

	if (GetPose() == true)
	{
		// ポーズ処理の描画
		DrawPose();
	}
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグ：ON

}


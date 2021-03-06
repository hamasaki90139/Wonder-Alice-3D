//=============================================================================
//
// エネミーモデル処理 [enemy.h]
// Author : 濱崎 小春
//
//=============================================================================
#pragma once

#include "particle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_ENEMY				"data/MODEL/trump_body.x"	// 読み込むモデル名
#define	MODEL_ENEMY_HEAD		"data/MODEL/trump_head.x"	// 読み込むモデル名
#define	MODEL_ENEMY_ARM_L		"data/MODEL/trump_arm_L.x"	// 読み込むモデル名
#define	MODEL_ENEMY_ARM_R		"data/MODEL/trump_arm_R.x"	// 読み込むモデル名
#define	MODEL_ENEMY_LEG_L		"data/MODEL/trump_leg_L.x"	// 読み込むモデル名
#define	MODEL_ENEMY_LEG_R		"data/MODEL/trump_leg_R.x"	// 読み込むモデル名

#define	VALUE_MOVE				(6.0f);						// 移動量
#define	VALUE_ROTATE			(D3DX_PI * 0.02f);			// 回転量
#define MAX_ENEMY				(200)						// エネミーの数
#define ENEMY_LEN				(20)						// エネミーカプセルの長さ
#define ENEMY_R					(40)						// エネミーの半径
#define ENEMY_GROUP				(5)							// エネミー小隊の人数
#define ENEMY_SCOUTING			(1000)						// エネミーの最大表示数
#define ENEMY_MOVEPATTERN		(5)							// エネミー小隊の行動パターン

//エネミー線形補間テーブル用の構造体
typedef struct 
{
	D3DXVECTOR3					pos;						// モデルの位置
	D3DXVECTOR3					rot;						// モデルの向き(回転)
	D3DXVECTOR3					scl;						// モデルの大きさ(スケール)
	float						dt;							// 1フレームで動く時間
}ENEMYTBL;

struct ENEMY
{
	D3DXVECTOR3			spos;								// モデルの基準位置
	D3DXVECTOR3			srot;								// モデルの基準向き(回転)
	D3DXVECTOR3			sscl;								// モデルの基準大きさ(スケール)
	D3DXVECTOR3			pos;								// モデルの位置
	D3DXVECTOR3			rot;								// モデルの向き(回転)
	D3DXVECTOR3			scl;								// モデルの大きさ(スケール)
	D3DXVECTOR3			vec;								// テーブル間のベクトル
	D3DXVECTOR3			rey;								// 光線
	D3DXVECTOR3			move;								// 移動量
	D3DXVECTOR3			oldpos;								// モデル位置の位置保存
	D3DXVECTOR3			hit_oldpos;							// モデルが消えた時の位置保存

	ENEMYTBL			*movepattern;
	D3DXMATRIX			mtxWorld;							// ワールドマトリックス
	D3DXMATRIX          mtxScl;								// 拡大縮小
	D3DXMATRIX          mtxRot;								// 回転
	D3DXMATRIX       	mtxTranslate;						// 移動

	float				oldindex;							//indexデータの保存
	float				spd;								// 移動スピード
	float				Radius;								// エネミーの半径
	float				BaseAngle;							// エネミーの角度
	float				move_time;							// 動いた時間
	float				back_time;							// 追従終了からテーブルへ戻るための線形補間に使う
	int					pattern;							// バトルエネミーの行動パターン
	int					size;								// 動くテーブルサイズ
	int					shadowIdx[ENEMY_GROUP];				// 影のインデックス番号
	int					following;
	bool				timestop;							//線形補間を止めるか否か
	bool				use;								//使用中か未使用か

	float				walkmove_time;						//アニメーション進行時間

	float				attack_dt;							//1フレームあたり進む時間(攻撃用）
	float				attackmove_time;					//アニメーション進行時間
	int					attackcnt;							//攻撃を始めるまでのカウント

	float				hit_dt;								//1フレームあたり進む時間(攻撃受けた時用）
	float				hitmove_time;						//アニメーション進行時間

	ENEMY				*parent;							//階層アニメーション用親子設定
	STATUS				status;								//現在のステータス

	D3DXMATERIAL		*pD3DXMat;							//ワールドマトリックス用
	D3DMATERIAL9		matDef;								//ワールドマトリックス用

	float				hit;								// 攻撃受けたときの飛んでく高さ
	int					hit_cnt;							// 何フレーム飛んでるか
	int					hit_cnt_max;						// 何フレームで飛んでくか
	D3DXVECTOR3			ofs;								// posに対するオフセット座標

	PARTICLE			particle[MAX_PARTICLE];				// エネミー倒した時のパーティクル

	int					formation;							// エネミー小隊のフォーメーション
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(int type);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
ENEMY *GetEnemy();
void SetEnemy(int enemy_spawn, D3DXVECTOR3 upperleft, D3DXVECTOR3 bottomright);
void RefreshEnemy(int refresh_enemy, D3DXVECTOR3 upperleft, D3DXVECTOR3 bottomright);

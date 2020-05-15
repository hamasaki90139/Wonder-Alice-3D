//=============================================================================
//
// モデル処理 [player.h]
// Author : 濱崎 小春
//
//=============================================================================
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_PLAYER			"data/MODEL/alice_body.x"		// 読み込むモデル名
#define	MODEL_PLAYER_HEAD		"data/MODEL/alice_head.x"		// 読み込むモデル名
#define MODEL_PLAYER_ARM_L		"data/MODEL/alice_arm_L.x"		// 読み込むモデル名
#define MODEL_PLAYER_ARM_R		"data/MODEL/alice_arm_R.x"		// 読み込むモデル名
#define	MODEL_PLAYER_LEG_L		"data/MODEL/alice_leg_L.x"		// 読み込むモデル名
#define	MODEL_PLAYER_LEG_R		"data/MODEL/alice_leg_R.x"		// 読み込むモデル名

#define MODEL_PLAYER_WEAPON		"data/MODEL/spoon.x"			// 読み込むモデル名

#define WEAPON_LEN						(15)

#define	VALUE_MOVE						(5.0f)					// 移動量
#define	VALUE_ROTATE					(D3DX_PI * 0.02f)		// 回転量
#define PARTS_MAX						(2)						//本体を除いたパーツ数

#define	PLAYER_JUMP_Y					(15.0f)					// ジャンプの高さ
#define	PLAYER_JUMP_CNT_MAX				(20)					// 20フレームのジャンプ処理
#define	PLAYER_JUMP_MAX					(1)						// 連続ジャンプ回数
#define	PLAYER_ATTACK_MAX				(4)						// 連続攻撃回数

//*****************************************************************************
// マクロ定義
//*****************************************************************************
struct PLAYER
{
	D3DXVECTOR3			pos;					// モデルの位置
	D3DXVECTOR3			rot;					// モデルの向き(回転)
	D3DXVECTOR3			scl;					// モデルの大きさ(スケール)

	D3DXVECTOR3			vec;					//	テーブル間のベクトル

	float				spd;					// 移動スピード

	LPDIRECT3DTEXTURE9	pD3DTexture;			// テクスチャへのポインタ
	LPD3DXMESH			pD3DXMesh;				// メッシュ情報へのポインタ
	LPD3DXBUFFER		pD3DXBuffMat;			// マテリアル情報へのポインタ
	DWORD				nNumMat;				// マテリアル情報の数

	D3DXMATRIX			mtxWorld;				// ワールドマトリックス
	D3DXMATRIX          mtxScl;					// ワールドマトリックス
	D3DXMATRIX          mtxRot;					// ワールドマトリックス
	D3DXMATRIX       	mtxTranslate;			// ワールドマトリックス
	D3DXMATERIAL		*pD3DXMat;				// ワールドマトリックス
	D3DMATERIAL9		matDef;					// ワールドマトリックス

	int					shadowIdx;				// 影のインデックス番号

	PLAYER				*parent;				//階層アニメーションの親子判定用

	bool				use;					//生存判定

	bool				jump;					// ジャンプのチェック
	float				jump_y;					// ジャンプの高さ
	int					jump_cnt;				// 何フレームジャンプしているか
	int					jump_cnt_max;			// 何フレームでジャンプをするか
	D3DXVECTOR3			ofs;					// posに対するオフセット座標
	int					jumppoint;				// 何回連続でジャンプしているか

	bool				AttackMotion;			//攻撃中か
	int					attackpoint;			// 何回連続で攻撃しているか
	bool				protect;				//無敵状態か
	int					protect_cnt;			//無敵状態のカウント

	bool				player_hitstop;			// プレイヤーのヒットストップ判定
	int					player_hitstop_cnt;		// ヒットストップのカウント

};

#define MAX_PLAYER						(1)		// プレイヤーの数
#define PLAYER_R						(30)	// プレイヤーの半径
#define WEAPON_R						(20)	// 武器の半径
#define PLAYER_HITSTOP					(3)		// ヒットストップの時間

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(int type);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER *GetPlayer(void);

void GetweaponPos(D3DXVECTOR3 &pos, D3DXVECTOR3 &S, D3DXVECTOR3 &E);

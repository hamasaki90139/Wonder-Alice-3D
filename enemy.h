//=============================================================================
//
// ƒGƒlƒ~[ƒ‚ƒfƒ‹ˆ— [enemy.h]
// Author : à_è ¬t
//
//=============================================================================
#pragma once

#include "particle.h"

//*****************************************************************************
// ƒ}ƒNƒ’è‹`
//*****************************************************************************
#define	MODEL_ENEMY				"data/MODEL/trump_body.x"	// “Ç‚İ‚Şƒ‚ƒfƒ‹–¼
#define	MODEL_ENEMY_HEAD		"data/MODEL/trump_head.x"	// “Ç‚İ‚Şƒ‚ƒfƒ‹–¼
#define	MODEL_ENEMY_ARM_L		"data/MODEL/trump_arm_L.x"	// “Ç‚İ‚Şƒ‚ƒfƒ‹–¼
#define	MODEL_ENEMY_ARM_R		"data/MODEL/trump_arm_R.x"	// “Ç‚İ‚Şƒ‚ƒfƒ‹–¼
#define	MODEL_ENEMY_LEG_L		"data/MODEL/trump_leg_L.x"	// “Ç‚İ‚Şƒ‚ƒfƒ‹–¼
#define	MODEL_ENEMY_LEG_R		"data/MODEL/trump_leg_R.x"	// “Ç‚İ‚Şƒ‚ƒfƒ‹–¼

#define	VALUE_MOVE				(6.0f);						// ˆÚ“®—Ê
#define	VALUE_ROTATE			(D3DX_PI * 0.02f);			// ‰ñ“]—Ê
#define MAX_ENEMY				(200)						// ƒGƒlƒ~[‚Ì”
#define ENEMY_LEN				(20)						// ƒGƒlƒ~[ƒJƒvƒZƒ‹‚Ì’·‚³
#define ENEMY_R					(40)						// ƒGƒlƒ~[‚Ì”¼Œa
#define ENEMY_GROUP				(5)							// ƒGƒlƒ~[¬‘à‚Ìl”
#define ENEMY_SCOUTING			(1000)						// ƒGƒlƒ~[‚ÌÅ‘å•\¦”
#define ENEMY_MOVEPATTERN		(5)							// ƒGƒlƒ~[¬‘à‚Ìs“®ƒpƒ^[ƒ“

//ƒGƒlƒ~[üŒ`•âŠÔƒe[ƒuƒ‹—p‚Ì\‘¢‘Ì
typedef struct 
{
	D3DXVECTOR3					pos;						// ƒ‚ƒfƒ‹‚ÌˆÊ’u
	D3DXVECTOR3					rot;						// ƒ‚ƒfƒ‹‚ÌŒü‚«(‰ñ“])
	D3DXVECTOR3					scl;						// ƒ‚ƒfƒ‹‚Ì‘å‚«‚³(ƒXƒP[ƒ‹)
	float						dt;							// 1ƒtƒŒ[ƒ€‚Å“®‚­ŠÔ
}ENEMYTBL;

struct ENEMY
{
	D3DXVECTOR3			spos;								// ƒ‚ƒfƒ‹‚ÌŠî€ˆÊ’u
	D3DXVECTOR3			srot;								// ƒ‚ƒfƒ‹‚ÌŠî€Œü‚«(‰ñ“])
	D3DXVECTOR3			sscl;								// ƒ‚ƒfƒ‹‚ÌŠî€‘å‚«‚³(ƒXƒP[ƒ‹)
	D3DXVECTOR3			pos;								// ƒ‚ƒfƒ‹‚ÌˆÊ’u
	D3DXVECTOR3			rot;								// ƒ‚ƒfƒ‹‚ÌŒü‚«(‰ñ“])
	D3DXVECTOR3			scl;								// ƒ‚ƒfƒ‹‚Ì‘å‚«‚³(ƒXƒP[ƒ‹)
	D3DXVECTOR3			vec;								// ƒe[ƒuƒ‹ŠÔ‚ÌƒxƒNƒgƒ‹
	D3DXVECTOR3			rey;								// Œõü
	D3DXVECTOR3			move;								// ˆÚ“®—Ê
	D3DXVECTOR3			oldpos;								// ƒ‚ƒfƒ‹ˆÊ’u‚ÌˆÊ’u•Û‘¶
	D3DXVECTOR3			hit_oldpos;							// ƒ‚ƒfƒ‹‚ªÁ‚¦‚½‚ÌˆÊ’u•Û‘¶

	ENEMYTBL			*movepattern;
	D3DXMATRIX			mtxWorld;							// ƒ[ƒ‹ƒhƒ}ƒgƒŠƒbƒNƒX
	D3DXMATRIX          mtxScl;								// Šg‘åk¬
	D3DXMATRIX          mtxRot;								// ‰ñ“]
	D3DXMATRIX       	mtxTranslate;						// ˆÚ“®

	float				oldindex;							//indexƒf[ƒ^‚Ì•Û‘¶
	float				spd;								// ˆÚ“®ƒXƒs[ƒh
	float				Radius;								// ƒGƒlƒ~[‚Ì”¼Œa
	float				BaseAngle;							// ƒGƒlƒ~[‚ÌŠp“x
	float				move_time;							// “®‚¢‚½ŠÔ
	float				back_time;							// ’Ç]I—¹‚©‚çƒe[ƒuƒ‹‚Ö–ß‚é‚½‚ß‚ÌüŒ`•âŠÔ‚Ég‚¤
	int					pattern;							// ƒoƒgƒ‹ƒGƒlƒ~[‚Ìs“®ƒpƒ^[ƒ“
	int					size;								// “®‚­ƒe[ƒuƒ‹ƒTƒCƒY
	int					shadowIdx[ENEMY_GROUP];				// ‰e‚ÌƒCƒ“ƒfƒbƒNƒX”Ô†
	int					following;
	bool				timestop;							//üŒ`•âŠÔ‚ğ~‚ß‚é‚©”Û‚©
	bool				use;								//g—p’†‚©–¢g—p‚©

	float				walkmove_time;						//ƒAƒjƒ[ƒVƒ‡ƒ“isŠÔ

	float				attack_dt;							//1ƒtƒŒ[ƒ€‚ ‚½‚èi‚ŞŠÔ(UŒ‚—pj
	float				attackmove_time;					//ƒAƒjƒ[ƒVƒ‡ƒ“isŠÔ
	int					attackcnt;							//UŒ‚‚ğn‚ß‚é‚Ü‚Å‚ÌƒJƒEƒ“ƒg

	float				hit_dt;								//1ƒtƒŒ[ƒ€‚ ‚½‚èi‚ŞŠÔ(UŒ‚ó‚¯‚½—pj
	float				hitmove_time;						//ƒAƒjƒ[ƒVƒ‡ƒ“isŠÔ

	ENEMY				*parent;							//ŠK‘wƒAƒjƒ[ƒVƒ‡ƒ“—peqİ’è
	STATUS				status;								//Œ»İ‚ÌƒXƒe[ƒ^ƒX

	D3DXMATERIAL		*pD3DXMat;							//ƒ[ƒ‹ƒhƒ}ƒgƒŠƒbƒNƒX—p
	D3DMATERIAL9		matDef;								//ƒ[ƒ‹ƒhƒ}ƒgƒŠƒbƒNƒX—p

	float				hit;								// UŒ‚ó‚¯‚½‚Æ‚«‚Ì”ò‚ñ‚Å‚­‚‚³
	int					hit_cnt;							// ‰½ƒtƒŒ[ƒ€”ò‚ñ‚Å‚é‚©
	int					hit_cnt_max;						// ‰½ƒtƒŒ[ƒ€‚Å”ò‚ñ‚Å‚­‚©
	D3DXVECTOR3			ofs;								// pos‚É‘Î‚·‚éƒIƒtƒZƒbƒgÀ•W

	PARTICLE			particle[MAX_PARTICLE];				// ƒGƒlƒ~[“|‚µ‚½‚Ìƒp[ƒeƒBƒNƒ‹

	int					formation;							// ƒGƒlƒ~[¬‘à‚ÌƒtƒH[ƒ[ƒVƒ‡ƒ“
};

//*****************************************************************************
// ƒvƒƒgƒ^ƒCƒvéŒ¾
//*****************************************************************************
HRESULT InitEnemy(int type);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
ENEMY *GetEnemy();
void SetEnemy(int enemy_spawn, D3DXVECTOR3 upperleft, D3DXVECTOR3 bottomright);
void RefreshEnemy(int refresh_enemy, D3DXVECTOR3 upperleft, D3DXVECTOR3 bottomright);

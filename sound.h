//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 濱崎 小春
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

// サウンドファイル
typedef enum
{
	SOUND_LABEL_GAME = 0,		// GAME
	SOUND_LABEL_GAMECLEAR,		// CLEAR
	SOUND_LABEL_GAMEOVER,		// GAMEOVER
	SOUND_LABEL_TITLE,			// TITLE
	SOUND_LABEL_TUTORIAL,		// TUTORIAL
	SOUND_LABEL_SE_HIT,			// HIT時
	SOUND_LABEL_SE_DAMEGE,		// damege時
	SOUND_LABEL_SE_ITEM,		// アイテムGET音
	SOUND_LABEL_SE_ATTACK1,		// 攻撃音
	SOUND_LABEL_SE_JUMP,		// ジャンプ音
	SOUND_LABEL_SE_MENU,		// メニュー音
	SOUND_LABEL_SE_OK,			// 決定音
	SOUND_LABEL_SE_ATTACK2,		// ATTACK音
	SOUND_LABEL_SE_JUMPOFF,		// JumpOFF音
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif

//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : �_�� ���t
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

// �T�E���h�t�@�C��
typedef enum
{
	SOUND_LABEL_GAME = 0,		// GAME
	SOUND_LABEL_GAMECLEAR,		// CLEAR
	SOUND_LABEL_GAMEOVER,		// GAMEOVER
	SOUND_LABEL_TITLE,			// TITLE
	SOUND_LABEL_TUTORIAL,		// TUTORIAL
	SOUND_LABEL_SE_HIT,			// HIT��
	SOUND_LABEL_SE_DAMEGE,		// damege��
	SOUND_LABEL_SE_ITEM,		// �A�C�e��GET��
	SOUND_LABEL_SE_ATTACK1,		// �U����
	SOUND_LABEL_SE_JUMP,		// �W�����v��
	SOUND_LABEL_SE_MENU,		// ���j���[��
	SOUND_LABEL_SE_OK,			// ���艹
	SOUND_LABEL_SE_ATTACK2,		// ATTACK��
	SOUND_LABEL_SE_JUMPOFF,		// JumpOFF��
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif

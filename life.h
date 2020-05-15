//=============================================================================
//
// ���C�t���� [life.h]
// Author : �_�� ���t
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_LIFE		"data/TEXTURE/life001.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_FRAME_LIFE	"data/TEXTURE/cloud001.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_PLAYER_UI	"data/TEXTURE/��alice_001.png"		// �ǂݍ��ރe�N�X�`���t�@�C����

#define	LIFE_SIZE_X			(35.0f)								// ���C�t�̐����̕�
#define	LIFE_SIZE_Y			(45.0f)								// ���C�t�̐����̍���
#define	LIFE_INTERVAL_X		(0.0f)								// ���C�t�̐����̕\���Ԋu

#define	NUM_PLACE			(5)									// ���C�t�̌���
#define	LIFE_POS_X			(300.0f)							// ���C�t�̕\����ʒu�w���W
#define	LIFE_POS_Y			(470.0f)							// ���C�t�̕\����ʒu�x���W

#define	PLAYER_UI_SIZE_X	(550.0f)							// ���C�t�̐����̕�
#define	PLAYER_UI_SIZE_Y	(550.0f)							// ���C�t�̐����̍���
#define	PLAYER_UI_POS_X		(-120.0f)							// ���C�t�̕\����ʒu�w���W
#define	PLAYER_UI_POS_Y		(300.0f)							// ���C�t�̕\����ʒu�x���W

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitLife(void);
void UninitLife(void);
void UpdateLife(void);
void DrawLife(void);

void ChangeLife(int value);

#endif

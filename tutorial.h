//=============================================================================
//
// �`���[�g���A������ [tutorial.h]
// Author : �_�� ���t
//
//=============================================================================
#pragma once

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_TUTORIAL01		("data/TEXTURE/tutorial001.png")		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TUTORIAL02		("data/TEXTURE/tutorial002.png")		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TUTORIAL_POS_X			(0)										// �^�C�g���̕\���ʒu
#define	TUTORIAL_POS_Y			(0)										// �^�C�g���̕\���ʒu
#define	TUTORIAL_SIZE_X			(800)									// �^�C�g���̕�
#define	TUTORIAL_SIZE_Y			(600)									// �^�C�g���̍���

// ��ʑJ�ڒ萔
typedef enum
{
	SOUSA,
	GAMEN,
}TUTORIAL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTutorial(int type);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);
TUTORIAL GetTutorial(void);


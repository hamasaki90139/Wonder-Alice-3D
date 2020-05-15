//=============================================================================
//
// �X�R�A���� [score.h]
// Author : �_�� ���t
//
//=============================================================================
#pragma once

// �}�N����`
#define SCORE_TEXTURE		("data/TEXTURE/��score_001.png")	// �T���v���p�摜
#define SCORE_SIZE_W		(64)								// �e�N�X�`���T�C�Y
#define SCORE_SIZE_H		(128)								// ����
#define SCORE_POS_X			(700)								// �|���S���̏����ʒuX
#define SCORE_POS_Y			(450)								// ����
#define SCORE_MAX			(99999)								// �X�R�A�̍ő�l
#define SCORE_MIN			(0)									// �X�R�A�̍ŏ��l
#define SCORE_DIGIT			(4)									// ����
#define SCORE_MAG_MIN		(1)									// �X�R�A�����{��

//*****************************************************************************
// �\���̐錾
//*****************************************************************************
typedef struct	// �X�R�A�\����
{
	int								score;			// �X�R�A
	int								score_mag;		//�X�R�A�{���̏�����
	D3DXVECTOR3						posScore;		// �|���S���̈ړ���

}SCORE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitScore(int type);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);

void AddScore(int add, int mag);
SCORE *GetScore(void);


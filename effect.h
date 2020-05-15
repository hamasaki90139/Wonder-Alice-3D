//=============================================================================
//
// �G�t�F�N�g���� [effect.h]
// Author : �_�� ���t
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_EFFECT01		"data/TEXTURE/effect000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_EFFECT02		"data/TEXTURE/effect001.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_EFFECT03		"data/TEXTURE/effect002.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

#define	EFFECT_SIZE_X			(300.0f)						// �r���{�[�h�̕�
#define	EFFECT_SIZE_Y			(300.0f)						// �r���{�[�h�̍���
#define	VALUE_MOVE_BULLET		(2.0f)							// �ړ����x

#define	MAX_EFFECT				(4096)							// �G�t�F�N�g�ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ��]
	D3DXVECTOR3 scale;		// �X�P�[��
	D3DXVECTOR3 move;		// �ړ���
	D3DXCOLOR col;			// �F
	float fSizeX;			// ��
	float fSizeY;			// ����
	int nTimer;				// �^�C�}�[
	float nDecAlpha;		// �����l
	bool bUse;				// �g�p���Ă��邩�ǂ���
} EFFECT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);

int SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nTimer);
EFFECT *GetEffect(void);
int GetEffectNo(int no);
#endif

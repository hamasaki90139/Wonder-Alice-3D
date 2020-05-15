//=============================================================================
//
// �^�C�}�[���� [timer.h]
// Author : �_�� ���t
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_TIMER		"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_FRAME_TIMER	"data/TEXTURE/tokei.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TOKEI		"data/TEXTURE/tokei_hari.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TIMER_SIZE_X		(35.0f)							// �^�C�}�[�̐����̕�
#define	TIMER_SIZE_Y		(50.0f)							// �^�C�}�[�̐����̍���
#define	TIMER_INTERVAL_X	(0.0f)							// �^�C�}�[�̐����̕\���Ԋu
#define TIMER_MAX			(60)

#define	NUM_PLACE			(3)			// �^�C�}�[�̌���

#define	TIMER_POS_X			(SCREEN_WIDTH / 2 - ((TIMER_SIZE_X * NUM_PLACE + TIMER_INTERVAL_X * (NUM_PLACE - 1)) / 2) )	// �^�C�}�[�̕\����ʒu�w���W
#define	TIMER_POS_Y			(30.0f)																						// �^�C�}�[�̕\����ʒu�x���W

#define	HARI_POS_X			(399.0f)	// �^�C�}�[�̕\����ʒu�w���W
#define	HARI_POS_Y			(70.0f)																						// �^�C�}�[�̕\����ʒu�x���W
#define	HARI_SIZE_X			(60.0f)							// �^�C�}�[�̐����̕�
#define	HARI_SIZE_Y			(60)							// �^�C�}�[�̐����̍���

#define TIMER_TEXTURE_PATTERN_DIVIDE_X	(1)
#define TIMER_TEXTURE_PATTERN_DIVIDE_Y	(1)
#define TIMER_TIME_ANIMATION				(40)	// �A�j���[�V�����̐؂�ւ��J�E���g
#define TIMER_ANIM_PATTERN_NUM			(TIMER_TEXTURE_PATTERN_DIVIDE_X*TIMER_TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����

//*****************************************************************************
// �\���̐錾
//*****************************************************************************
typedef struct	// �P�[�L�\����
{
	LPDIRECT3DTEXTURE9		g_pD3DTextureTimer;	// �e�N�X�`���ւ̃|���S��
	bool					use;						// true:�g�p  false:���g�p
	D3DXVECTOR3				pos;						// �|���S���̈ړ���
	D3DXVECTOR3				rot;						// �|���S���̉�]��
	VERTEX_2D				vertexWk[NUM_VERTEX];		// ���_���i�[���[�N
	float					Radius;						// �I�u�W�F�N�g�̔��a
	float					BaseAngle;					// �I�u�W�F�N�g�̊p�x
} TIMER;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTimer(void);
void UninitTimer(void);
void UpdateTimer(void);
void DrawTimer(void);

void EnableTimer(bool bEnable);
void ResetTimer(int nTime = 999);
//CAKE *GetCake(void);

#endif

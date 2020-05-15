//=============================================================================
//
// �����d������ [flashlight.cpp]
// Author : GP11B292 29 �_�� ���t
//
//=============================================================================
#pragma once
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_FLASHLIGHT		(1)					// �����d���̐�
#define	VALUE_MOVE			(2.0f)				// �ړ���
#define	POS_X_F_LIGHT		(0.0f)				// ���C�g�̏����ʒu(X���W)
#define	POS_Y_F_LIGHT		(20.0f)				// ���C�g�̏����ʒu(Y���W)
#define	POS_Z_F_LIGHT		(-200.0f)			// ���C�g�̏����ʒu(Z���W)
#define	ROT_X_F_LIGHT		(0.1f)				// ���C�g�̏�����]��(X���W)
#define	ROT_Y_F_LIGHT		(0.0f)				// ���C�g�̏�����]��(Y���W)
#define	ROT_Z_F_LIGHT		(0.0f)			// ���C�g�̏�����]��(Z���W)
#define	SCL_F_LIGHT			(0.5f)				// ���C�g�̏����X�P�[��

#define LIGHT_HIT_SITA		(12.3f)				// ���C�g�̏Փ˔���p�̊p�x
#define LIGHT_HIT_R1		(500)				// ���C�g�̏Փ˔���p�̔��a(�n�_�`�I�_�j
#define LIGHT_HIT_R2		(100)				// ���C�g�̏Փ˔���p�̔��a(�n�_�`�v���C���[�̈ʒu�j

struct FLASHLIGHT
{
	D3DXVECTOR3			pos;				// ���f���̈ʒu
	D3DXVECTOR3			rot;				// ���f���̌���(��])
	D3DXVECTOR3			scl;				// ���f���̑傫��(�X�P�[��)

	float				spd;				// �ړ��X�s�[�h

	LPDIRECT3DTEXTURE9	pD3DTexture;		// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH			pD3DXMesh;			// ���b�V�����ւ̃|�C���^(x�t�@�C���\���֐��p�j
	LPD3DXBUFFER		pD3DXBuffMat;		// �}�e���A�����ւ̃|�C���^(x�t�@�C���\���֐��p�j
	DWORD				nNumMat;			// �}�e���A�����̐�(x�t�@�C���\���֐��p�j

	D3DXMATRIX			mtxWorld;			// ���[���h�}�g���b�N�X(�s��)�����[���h�ϊ��p
	CAMERA				*parent;			// �e�q����p

	D3DXMATRIX			mtxRoll;			// �t���b�V�����C�g�̉�]�s�񁦓����蔻��p
	D3DXVECTOR3			hitpos;		//�Փ˔���p�t���b�V�����C�gat

};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitFlashLight(int type);
void UninitFlashLight(void);
void UpdateFlashLight(void);
void DrawFlashLight(void);
void FlashLightHitPos(void);
FLASHLIGHT *GetFLashLight(void);

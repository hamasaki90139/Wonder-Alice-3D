//=============================================================================
//
// �J�������� [camera.h]
// Author : �_�� ���t
//
//=============================================================================
#pragma once

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "d3dx9.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TILT_Z_CAM		(100.0f)
#define	TILT_Y_CAM		(10.0f)

#define	POS_X_CAM		(0.0f)			// �J�����̏����ʒu(X���W)
#define	POS_Y_CAM		(100.0f)			// �J�����̏����ʒu(Y���W)
#define	POS_Z_CAM		(-500.0f)		// �J�����̏����ʒu(Z���W)

struct CAMERA
{
	D3DXVECTOR3			pos;			// �J�����̎��_(�ʒu)
	D3DXVECTOR3			at;				// �J�����̒����_
	D3DXVECTOR3			up;				// �J�����̏�����x�N�g��
	D3DXVECTOR3			rot;			// �J�����̉�]
	float				len;			// �J�����̎��_�ƒ����_�̋���

	D3DXVECTOR3			*moveatpattern;
	float				moveat_time;
	float				atdt;
	int					atsize;

	D3DXVECTOR3			*movepospattern;
	float				movepos_time;
	float				posdt;
	int					possize;

	float				dt;


	D3DXMATRIX			mtxRoll;
	D3DXMATRIX			mtxView;		// �r���[�}�g���b�N�X
	D3DXMATRIX			mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera();
CAMERA *GetCamera();
D3DXMATRIX GetMtxView(void);

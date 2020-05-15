//=============================================================================
//
// �J�������� [camera.cpp]
// Author : �_�� ���t
//
//=============================================================================
#include "main.h"
#include "debugproc.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "collision.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// �r���[���ʂ̎���p
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��	
#define	VIEW_NEAR_Z		(10.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z		(20000.0f)										// �r���[���ʂ�FarZ�l

#define	VALUE_MOVE_CAMERA	(2.0f)										// �J�����̈ړ���
#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.01f)							// �J�����̉�]��

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
CAMERA			g_Camera;				// �J�����f�[�^

int				g_LockEneNom;
bool			g_CameraLock;
bool			g_Cameramove;
D3DXMATRIX		g_mtxView;					// �r���[�}�g���b�N�X

D3DXVECTOR3 move_Cameratblpos[] = {
	D3DXVECTOR3(0.0f, 40.0f, -150.0f),
	D3DXVECTOR3(0.0f, 23.0f, -75.0f),
	D3DXVECTOR3(5.0f, 6.0f, 0.0f),
	D3DXVECTOR3(0.0f, 23.0f, 80.0f),
	D3DXVECTOR3(0.0f, 40.0f, 160.0f),
	D3DXVECTOR3(20.0f, 50.0f, 120.0f),
	D3DXVECTOR3(50.0f, 60.0f, 80.0f),
	D3DXVECTOR3(70.0f, 70.0f, 40.0f),
	D3DXVECTOR3(80.0f, 80.0f, 0.0f),
	D3DXVECTOR3(70.0f, 70.0f, -37.0f),
	D3DXVECTOR3(50.0f, 60.0f, -74.0f),
	D3DXVECTOR3(20.0f, 50.0f, -111.0f),
	D3DXVECTOR3(0.0f, 40.0f, -150.0f),
};

D3DXVECTOR3 move_Cameratblat[] = {
	D3DXVECTOR3(0.0f, 40.0f, 0.0f),
	D3DXVECTOR3(0.0f, 30.0f, 0.0f),
	D3DXVECTOR3(0.0f, 20.0f, 0.0f),
	D3DXVECTOR3(0.0f, 30.0f, 0.0f),
	D3DXVECTOR3(0.0f, 40.0f, 0.0f),
	D3DXVECTOR3(0.0f, 42.0f, 0.0f),
	D3DXVECTOR3(0.0f, 44.0f, 0.0f),
	D3DXVECTOR3(0.0f, 46.0f, 0.0f),
	D3DXVECTOR3(0.0f, 48.0f, 0.0f),
	D3DXVECTOR3(0.0f, 46.0f, 0.0f),
	D3DXVECTOR3(0.0f, 44.0f, 0.0f),
	D3DXVECTOR3(0.0f, 42.0f, 0.0f),
	D3DXVECTOR3(0.0f, 40.0f, 0.0f),
};

//=============================================================================
// ����������
//=============================================================================
void InitCamera(void)
{
	g_Cameramove = false;

	g_LockEneNom = NULL;
	g_CameraLock = false;
	g_Camera.pos = D3DXVECTOR3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);
	g_Camera.at = D3DXVECTOR3(0.0f, TILT_Y_CAM, TILT_Z_CAM);
	g_Camera.up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_Camera.dt = 0;

	g_Camera.moveatpattern = move_Cameratblat;
	g_Camera.atsize = sizeof(move_Cameratblat) / sizeof(D3DXVECTOR3);
	g_Camera.atdt = 0.05f;
	g_Camera.moveat_time = 0;

	g_Camera.movepospattern = move_Cameratblpos;
	g_Camera.possize = sizeof(move_Cameratblpos) / sizeof(D3DXVECTOR3);
	g_Camera.posdt = 0.05f;
	g_Camera.movepos_time = 0;

	// ���_�ƒ����_�̋������v�Z
	float vx, vz;
	vx = g_Camera.pos.x - g_Camera.at.x;
	vz = g_Camera.pos.z - g_Camera.at.z;
	g_Camera.len = sqrtf(vx * vx + vz * vz);

}

//=============================================================================
// �J�����̏I������
//=============================================================================
void UninitCamera(void)
{

}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(void)
{
	if (GetKeyboardTrigger(DIK_TAB))
	{
		g_CameraLock = !g_CameraLock;
	}

	if (g_CameraLock == false)
	{
		if ((GetKeyboardPress(DIK_RIGHT)) || (IsButtonPressed(0, BUTTON_R_RIGHT)))
		{// ���_����u���v
			g_Camera.rot.y += VALUE_ROTATE_CAMERA;
			if (g_Camera.rot.y > D3DX_PI)
			{
				g_Camera.rot.y -= D3DX_PI * 2.0f;
			}
		}

		if ((GetKeyboardPress(DIK_LEFT)) || (IsButtonPressed(0, BUTTON_R_LEFT)))
		{// ���_����u�E�v
			g_Camera.rot.y -= VALUE_ROTATE_CAMERA;
			if (g_Camera.rot.y < -D3DX_PI)
			{
				g_Camera.rot.y += D3DX_PI * 2.0f;
			}
		}

		if ((GetKeyboardPress(DIK_UP)) || (IsButtonPressed(0, BUTTON_R_UP)))
		{// ���_�ړ��u��v
			if (g_Camera.rot.x < D3DXToRadian(50))
			{
				g_Camera.rot.x += (VALUE_ROTATE_CAMERA*0.3);
			}
		}

		if ((GetKeyboardPress(DIK_DOWN)) || (IsButtonPressed(0, BUTTON_R_DOWN)))
		{// ���_�ړ��u���v
			if (g_Camera.pos.y > 0)
			{
				g_Camera.rot.x -= (VALUE_ROTATE_CAMERA*0.3);
			}
		}
	}
	else
	{
		D3DXVECTOR3 lockat = GetEnemy()->pos - GetPlayer()->pos;
		D3DXVECTOR3 N = { 0,0,1 };
		float rotg;
		lockat.y = 0;
		lockat = Nomalise(lockat);
		float check = (Cross(N, lockat)).y;
		if (check < 0)
		{
			rotg = -acosf(Dot(N, lockat));
		}
		else
		{
			rotg = acosf(Dot(N, lockat));
		}
		rotg -= g_Camera.rot.y;
		g_Camera.rot.y += rotg*0.8;

	}

	// �J�����������ɖ߂�
	if (GetKeyboardPress(DIK_Z))
	{
		UninitCamera();
		InitCamera();
	}

	// �J�������v���C���[�ɒǏ]������
	D3DXMATRIX buff;

	D3DXVECTOR3 PtoC = D3DXVECTOR3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);
	D3DXVECTOR3 PtoAt = D3DXVECTOR3(0.0f, TILT_Y_CAM, TILT_Z_CAM);

	//��]�s��̏�����
	D3DXMatrixIdentity(&g_Camera.mtxRoll);

	//�o�b�t�@�̏�����
	D3DXMatrixIdentity(&buff);

	//��]�ړ�
	D3DXMatrixRotationYawPitchRoll(&buff, g_Camera.rot.y, g_Camera.rot.x, g_Camera.rot.z);
	D3DXMatrixMultiply(&g_Camera.mtxRoll, &g_Camera.mtxRoll, &buff);

	//�o�b�t�@�̏�����
	D3DXMatrixIdentity(&buff);

	//Player�ɒǏ]
	D3DXMatrixTranslation(&buff, GetPlayer()->pos.x, GetPlayer()->pos.y, GetPlayer()->pos.z);
	D3DXMatrixMultiply(&g_Camera.mtxRoll, &g_Camera.mtxRoll, &buff);

	D3DXVec3TransformCoord(&g_Camera.at, &PtoAt, &g_Camera.mtxRoll);
	D3DXVec3TransformCoord(&g_Camera.pos, &PtoC, &g_Camera.mtxRoll);

	PrintDebugProc("*** ���_���� ***\n");
	PrintDebugProc("��ړ� : ��\n");
	PrintDebugProc("���ړ� : ��\n");
	PrintDebugProc("������ : ��\n");
	PrintDebugProc("�E���� : ��\n");
	PrintDebugProc("\n");
}


//=============================================================================
// �J�����̍X�V
//=============================================================================
void SetCamera(void) 
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Camera.mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_Camera.mtxView,
		&g_Camera.pos,		// �J�����̎��_(�ʒu)
		&g_Camera.at,		// �J�����̒����_
		&g_Camera.up);		// �J�����̏�����x�N�g��

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_Camera.mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Camera.mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_Camera.mtxProjection,
		VIEW_ANGLE,			// �r���[���ʂ̎���p
		VIEW_ASPECT,		// �r���[���ʂ̃A�X�y�N�g��
		VIEW_NEAR_Z,		// �r���[���ʂ�NearZ�l
		VIEW_FAR_Z);		// �r���[���ʂ�FarZ�l

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�(�����ϊ��̐ݒ�)
	pDevice->SetTransform(D3DTS_PROJECTION, &g_Camera.mtxProjection);
}


//=============================================================================
// �J�����̎擾
//=============================================================================
CAMERA *GetCamera(void) 
{
	return &g_Camera;
}

//=============================================================================
// �r���[�}�g���b�N�X�̎擾
//=============================================================================
D3DXMATRIX GetMtxView(void)
{
	return g_Camera.mtxView;
}


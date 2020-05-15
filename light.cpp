//=============================================================================
//
// ���C�g���� [light.cpp]
// Author : �_�� ���t
//
//=============================================================================
#include "main.h"
#include "debugproc.h"
#include "light.h"
#include "camera.h"
#include "enemy.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	NUM_LIGHT		(3)						// ���C�g�̐�

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
D3DLIGHT9				g_Light[NUM_LIGHT];		// ���C�g���

//=============================================================================
// ����������
//=============================================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecPos;
	D3DXVECTOR3 vecDir;

	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory(&g_Light[0], sizeof(D3DLIGHT9));

	// ���C�g�̃^�C�v�̐ݒ�
	g_Light[0].Type = D3DLIGHT_DIRECTIONAL;

	// �g�U��
	g_Light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ���C�g0�̋��ʔ��ˌ��̐ݒ�
	g_Light[0].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ����
	g_Light[0].Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	// ���C�g�̕����̐ݒ�
	vecDir = D3DXVECTOR3(0.0f, -5.0f, 10.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_Light[0].Direction, &vecDir);

	// ���C�g�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight(0, &g_Light[0]);

	// ���C�g�̐ݒ�
	pDevice->LightEnable(0, TRUE);

	//Light[1]
	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory(&g_Light[1], sizeof(D3DLIGHT9));

	// ���C�g�̃^�C�v�̐ݒ�
	g_Light[1].Type = D3DLIGHT_DIRECTIONAL;

	// �g�U��
	g_Light[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ����
	g_Light[1].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	// ���C�g�̕����̐ݒ�
	vecDir = D3DXVECTOR3(-0.20f, -1.0f, -0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_Light[1].Direction, &vecDir);

	// ���C�g�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight(1, &g_Light[1]);

	// ���C�g�̐ݒ�
	pDevice->LightEnable(1, TRUE);

	/* �X�|�b�g���C�g�iflashlight�j�ݒ� */
	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory(&g_Light[2], sizeof(D3DLIGHT9));

	// ���C�g2�̃^�C�v�̐ݒ�
	g_Light[2].Type = D3DLIGHT_SPOT;

	// ���C�g2�̊g�U���̐ݒ�
	g_Light[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ���C�g2�̋��ʔ��ˌ��̐ݒ�
	g_Light[2].Specular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

	// ���C�g2�̊����̐ݒ�
	g_Light[2].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	// ���C�g2�̈ʒu�̐ݒ�
	vecPos = D3DXVECTOR3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM - 100.0f);
	g_Light[2].Position = vecPos;

	// ���C�g2�̕����̐ݒ�
	vecDir = D3DXVECTOR3(0.0f, 0.0f, 3.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_Light[2].Direction, &vecDir);

	// ���C�g2�̌����̗L������
	g_Light[2].Range = 3000.0f;

	// ���C�g2�̓������O���̌��̌����x
	g_Light[2].Falloff = 1.5f;

	// ���C�g2�̃��C�g����̋����ɑ΂��錸���̕ω�
	g_Light[2].Attenuation0 = 1.0f;
	g_Light[2].Attenuation1 = 0.0f;
	g_Light[2].Attenuation2 = 0.0f;

	// ���C�g2�̓����̌��̍L����ݒ�
	g_Light[2].Theta = D3DXToRadian(10.0f);

	// ���C�g2�̊O���̌��̍L����ݒ�
	g_Light[2].Phi = D3DXToRadian(30.0f);

	// ���C�g2�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight(2, &g_Light[2]);

	// ���C�g2���g�p�g�p��Ԃ�
	pDevice->LightEnable(2, TRUE);

	// ���C�e�B���O���[�h��ON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �����ݒ�i���̒���������������j
	pDevice->SetRenderState(D3DRS_AMBIENT, 0xff030303);

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateLight(void)
{
	CAMERA *cam = GetCamera();							//�J�����\���̂̎擾
	ENEMY *enemy = GetEnemy();							//�J�����\���̂̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X���̎擾

	//�J�����̎n�_�ɒǏ]������
	g_Light[2].Position.x = cam->pos.x;
	g_Light[2].Position.y = cam->pos.y;
	g_Light[2].Position.z = cam->pos.z;

	//�J�����̒��n�_�ɒǏ]������
	D3DXVECTOR3 cam_angle = cam->at - cam->pos;								//�J�����̒����_�̃x�N�g��������
	D3DXVec3Normalize((D3DXVECTOR3*)&g_Light[2].Direction, &cam_angle);		//�J���������_�̃x�N�g���𐳋K�����A���C�g�����_�ɂ���

	// ���C�g2�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight(2, &g_Light[2]);
}

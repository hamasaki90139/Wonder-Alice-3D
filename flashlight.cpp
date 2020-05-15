//=============================================================================
//
// �t���b�V�����C�g���� [flashlight.cpp]
// Author : GP11B292 29 �_�� ���t
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "flashlight.h"
#include "camera.h"
#include "debugproc.h"
#include "collision.h"
#include "player.h"
#include "enemy.h"
#include "flashlight.h"
#include "shadow.h"
#include "fade.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_FLASHLIGHT		"data/MODEL/flashlight.x"	// �ǂݍ��ރ��f����

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static FLASHLIGHT		g_FlashLight;						// �t���b�V�����C�g�\����

//=============================================================================
// ����������
//=============================================================================
HRESULT InitFlashLight(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();				//�f�o�C�X���̎擾

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	g_FlashLight.pos = D3DXVECTOR3(POS_X_F_LIGHT, POS_Y_F_LIGHT, POS_Z_F_LIGHT);		//�v���C���[��납�瓖�ĂĂ���ݒ�Ȃ̂ŏ�����O����`��
	g_FlashLight.rot = D3DXVECTOR3(ROT_X_F_LIGHT, ROT_Y_F_LIGHT, ROT_Z_F_LIGHT);		//���΂߉�
	g_FlashLight.scl = D3DXVECTOR3(SCL_F_LIGHT,   SCL_F_LIGHT,   SCL_F_LIGHT);			//�k�����Ă���
	g_FlashLight.hitpos = D3DXVECTOR3(POS_X_F_LIGHT, POS_Y_F_LIGHT, POS_Z_F_LIGHT);							//�Փ˔���p�t���b�V�����C�gat

	g_FlashLight.spd = 0.0f;								// �ړ��X�s�[�h�N���A

	if (type == 0)
	{
		g_FlashLight.pD3DTexture = NULL;
		g_FlashLight.pD3DXMesh = NULL;
		g_FlashLight.pD3DXBuffMat = NULL;
		g_FlashLight.nNumMat = 0;
		g_FlashLight.parent = GetCamera();						//�t���b�V�����C�g���J�����̎q�I�u�W�F�N�g�ɂ���

		// X�t�@�C���̓ǂݍ���(�֐��j
		if (FAILED(D3DXLoadMeshFromX(MODEL_FLASHLIGHT,			// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
			D3DXMESH_SYSTEMMEM,									// ���b�V���̍쐬�I�v�V�������w��
			pDevice,											// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
			NULL,												// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			&g_FlashLight.pD3DXBuffMat,							// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			NULL,												// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
			&g_FlashLight.nNumMat,								// D3DXMATERIAL�\���̂̐�
			&g_FlashLight.pD3DXMesh)))							// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
		{
			return E_FAIL;
		}
	}

#if 0	//if 0�̓R�����g�Ɠ��`
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
		TEXTURE_FILENAME,									// �t�@�C���̖��O
		&g_FlashLight.pD3DTexture);							// �ǂݍ��ރ������[
#endif

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitFlashLight(void)
{
	if (g_FlashLight.pD3DTexture != NULL)
	{// �e�N�X�`���̊J��
		g_FlashLight.pD3DTexture->Release();
		g_FlashLight.pD3DTexture = NULL;
	}

	if (g_FlashLight.pD3DXMesh != NULL)
	{// ���b�V���̊J��
		g_FlashLight.pD3DXMesh->Release();
		g_FlashLight.pD3DXMesh = NULL;
	}

	if (g_FlashLight.pD3DXBuffMat != NULL)
	{// �}�e���A���̊J��
		g_FlashLight.pD3DXBuffMat->Release();
		g_FlashLight.pD3DXBuffMat = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateFlashLight(void)
{
	// �ړ�����
	if (GetKeyboardPress(DIK_NUMPAD8))
	{
		g_FlashLight.rot.x-= 0.02;
	}

	if (GetKeyboardPress(DIK_NUMPAD2))
	{
		g_FlashLight.rot.x+= 0.02;
	}

	if (GetKeyboardPress(DIK_NUMPAD6))
	{
		g_FlashLight.rot.y+= 0.02;
	}

	if (GetKeyboardPress(DIK_NUMPAD4))
	{
		g_FlashLight.rot.y-= 0.02;
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawFlashLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	// ���C�e�B���O�𖳌���
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���Z�����ɐݒ�(�~�����`�悳���ʒu�𔒂��ۂ����߂���j
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);					// ���u�����h�̍������@�̐ݒ�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);				// ���\�[�X�J���[�̎w��(�`�挳�̃u�����h�t�@�N�^�[��ݒ�)
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);					// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);						// Z�o�b�t�@�[�̏������݂����Ȃ�

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_FlashLight.mtxWorld);															

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, g_FlashLight.scl.x, g_FlashLight.scl.y, g_FlashLight.scl.z);
	D3DXMatrixMultiply(&g_FlashLight.mtxWorld, &g_FlashLight.mtxWorld, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_FlashLight.rot.y, g_FlashLight.rot.x, g_FlashLight.rot.z);
	D3DXMatrixMultiply(&g_FlashLight.mtxWorld, &g_FlashLight.mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_FlashLight.pos.x , g_FlashLight.pos.y, g_FlashLight.pos.z);
	D3DXMatrixMultiply(&g_FlashLight.mtxWorld, &g_FlashLight.mtxWorld, &mtxTranslate);
	
	//�J�����̃��[���h�}�g���b�N�X���t���b�V�����C�g�Ə�Z
	D3DXMatrixMultiply(&g_FlashLight.mtxWorld, &g_FlashLight.mtxWorld, &g_FlashLight.parent->mtxRoll);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_FlashLight.mtxWorld);

	// ���݂̃}�e���A�����擾(�o�b�N�A�b�v)
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)g_FlashLight.pD3DXBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_FlashLight.nNumMat; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_FlashLight.pD3DTexture);

		// �`��(3D���f���̕`��)���֐�
		g_FlashLight.pD3DXMesh->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);

	// ���C�e�B���O��L����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �ʏ�u�����h�ɖ߂�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				// Z�o�b�t�@�[�̏������݂�����

}

//=============================================================================
// �Փ˔���p���C�g
//=============================================================================
void FlashLightHitPos(void)
{
	CAMERA *cam = GetCamera();

	D3DXMATRIX buff;									//�s��i�[�p
	D3DXVECTOR3 CtoF = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�J�����ƃt���b�V�����C�g�̑��Έʒu��������
	g_FlashLight.hitpos = D3DXVECTOR3(GetFLashLight()->rot.x, GetFLashLight()->rot.y, GetFLashLight()->rot.z);		//�Փ˔���p�t���b�V�����C�gat

	//��]�s��̏�����
	D3DXMatrixIdentity(&g_FlashLight.mtxRoll);

	//�o�b�t�@�̏�����
	D3DXMatrixIdentity(&buff);

	//��]�ړ��i�J���������_�ɂ��ă��C�g����]������j=�J�����ƃ��C�g�̑��Έʒu������
	D3DXMatrixRotationYawPitchRoll(&buff, g_FlashLight.rot.y, g_FlashLight.rot.x, g_FlashLight.rot.z);
	D3DXMatrixMultiply(&g_FlashLight.mtxRoll, &g_FlashLight.mtxRoll, &buff);

	//�o�b�t�@�̏�����
	D3DXMatrixIdentity(&buff);

	//�J�����ɒǏ](���[���h���_�ɑ΂��ăJ�����ɍ��킹�ă��C�g���ړ�������j
	D3DXMatrixTranslation(&buff, GetCamera()->pos.x, GetCamera()->pos.y, GetCamera()->pos.z);
	D3DXMatrixMultiply(&g_FlashLight.mtxRoll, &g_FlashLight.mtxRoll, &buff);

	//���_�ϊ�(�ϊ���̒��_�̊i�[��A�ϊ����̒��_�A�ϊ��s��)
	D3DXVec3TransformCoord(&g_FlashLight.hitpos, &CtoF, &g_FlashLight.mtxRoll);
}

//=============================================================================
// �t���b�V�����C�g�̎擾
//=============================================================================
FLASHLIGHT *GetFLashLight(void)
{
	return &g_FlashLight;
}

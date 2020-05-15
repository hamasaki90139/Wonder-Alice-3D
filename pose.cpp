//=============================================================================
//
// �|�[�Y��ʏ��� [pose.cpp]
// Author : �_�菬�t
//
//=============================================================================
#include "main.h"
#include "title.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "pose.h"
#include "game.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexPose(void);
//void SetVertexLogo(void);
//void SetVertexLogoColor(void);
void SetPoseMenuDiffuse(int menu);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTexturePosebg = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTexturePosetitle = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTextureReturn = NULL;	// �e�N�X�`���ւ̃|�C���^

VERTEX_2D				g_vertexWkPosebg[NUM_VERTEX];					// ���_���i�[���[�NVERTEX_2D				g_vertexWkTitleLogo[NUM_VERTEX];				// ���_���i�[���[�N
VERTEX_2D				g_vertexWkRetry[NUM_VERTEX];					// ���_���i�[���[�N
VERTEX_2D				g_vertexWkReturn[NUM_VERTEX];					// ���_���i�[���[�N

static D3DXCOLOR		g_color;

static int				g_posemenu; 						//���j���[�p�ϐ�
static int				scene;
static bool				g_pose; 						//�|�[�Y��ʗp�̕ϐ�

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPose(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		/*�e�N�X�`���̓ǂݍ���*/
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_POSE_BG,									// �t�@�C���̖��O
			&g_pD3DTexturePosebg);								// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_POSE_TITLE,									// �t�@�C���̖��O
			&g_pD3DTexturePosetitle);							// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_POSE_RETURN,								// �t�@�C���̖��O
			&g_pD3DTextureReturn);								// �ǂݍ��ރ������[
	}

	g_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_posemenu = 0;												// �|�[�Y���j���[������
	g_pose = false;												// �ŏ��̓|�[�Y���OFF

	// ���_���̍쐬
	MakeVertexPose();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPose(void)
{
	if (g_pD3DTexturePosebg != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTexturePosebg->Release();
		g_pD3DTexturePosebg = NULL;
	}

	if (g_pD3DTexturePosetitle != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTexturePosetitle->Release();
		g_pD3DTexturePosetitle = NULL;
	}

	if (g_pD3DTextureReturn != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureReturn->Release();
		g_pD3DTextureReturn = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePose(void)
{
	// �|�[�Y
	if (GetKeyboardTrigger(DIK_Z) || ((IsButtonTriggered(0, BUTTON_R) && (IsButtonTriggered(0, BUTTON_L)))))
	{
		g_pose = true;
	}

	if (g_pose == true)
	{
	   //menu�I��
		if (GetKeyboardRepeat(DIK_UP) || GetKeyboardRepeat(DIK_DOWN) ||
			(IsButtonTriggered(0, BUTTON_UP)) || (IsButtonTriggered(0, BUTTON_DOWN)))
		{
			g_posemenu = (g_posemenu + 1) % MENU_MAX;
		}

		switch (g_posemenu)
		{
		case 0:
			if (GetKeyboardTrigger(DIK_RETURN))
			{// Enter��������A�X�e�[�W��؂�ւ���
				SetMode(MODE_TITLE);
				UninitGame();
			}
			// �Q�[���p�b�h�ňړ�����
			else if (IsButtonTriggered(0, BUTTON_X))
			{
				SetMode(MODE_TITLE);
				UninitGame();
			}
			else if (IsButtonTriggered(0, BUTTON_B))
			{
				SetMode(MODE_TITLE);
				UninitGame();
			}
			break;
		case 1:		//�L�����Z��
			if (GetKeyboardTrigger(DIK_RETURN) || (IsButtonTriggered(0, BUTTON_X)) || IsButtonTriggered(0, BUTTON_B))
			{
				g_pose = false;
			}
			break;
		}

		SetPoseMenuDiffuse(g_posemenu);
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPose(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTexturePosebg);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkPosebg, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTexturePosetitle);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkRetry, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureReturn);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkReturn, sizeof(VERTEX_2D));

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexPose(void)
{
	// ���_���W�̐ݒ�
	g_vertexWkPosebg[0].vtx = D3DXVECTOR3(TITLE01_POS_X, TITLE01_POS_Y, 0.0f);
	g_vertexWkPosebg[1].vtx = D3DXVECTOR3(TITLE01_POS_X + TITLE01_SIZE_X, TITLE01_POS_Y, 0.0f);
	g_vertexWkPosebg[2].vtx = D3DXVECTOR3(TITLE01_POS_X, TITLE01_POS_Y + TITLE01_SIZE_Y, 0.0f);
	g_vertexWkPosebg[3].vtx = D3DXVECTOR3(TITLE01_POS_X + TITLE01_SIZE_X, TITLE01_POS_Y + TITLE01_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkPosebg[0].rhw =
	g_vertexWkPosebg[1].rhw =
	g_vertexWkPosebg[2].rhw =
	g_vertexWkPosebg[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkPosebg[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 150);
	g_vertexWkPosebg[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 150);
	g_vertexWkPosebg[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 150);
	g_vertexWkPosebg[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 150);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkPosebg[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkPosebg[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkPosebg[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkPosebg[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_���W�̐ݒ�
	g_vertexWkRetry[0].vtx = D3DXVECTOR3(RETRY_POS_X, RETRY_POS_Y, 0.0f);
	g_vertexWkRetry[1].vtx = D3DXVECTOR3(RETRY_POS_X + RETRYMENU_SIZE_X, RETRY_POS_Y, 0.0f);
	g_vertexWkRetry[2].vtx = D3DXVECTOR3(RETRY_POS_X, RETRY_POS_Y + RETRYMENU_SIZE_Y, 0.0f);
	g_vertexWkRetry[3].vtx = D3DXVECTOR3(RETRY_POS_X + RETRYMENU_SIZE_X, RETRY_POS_Y + RETRYMENU_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkRetry[0].rhw =
	g_vertexWkRetry[1].rhw =
	g_vertexWkRetry[2].rhw =
	g_vertexWkRetry[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkRetry[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkRetry[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkRetry[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkRetry[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkRetry[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkRetry[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkRetry[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkRetry[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_���W�̐ݒ�
	g_vertexWkReturn[0].vtx = D3DXVECTOR3(RETURN_POS_X, RETURN_POS_Y, 0.0f);
	g_vertexWkReturn[1].vtx = D3DXVECTOR3(RETURN_POS_X + RETRYMENU_SIZE_X, RETURN_POS_Y, 0.0f);
	g_vertexWkReturn[2].vtx = D3DXVECTOR3(RETURN_POS_X, RETURN_POS_Y + RETRYMENU_SIZE_Y, 0.0f);
	g_vertexWkReturn[3].vtx = D3DXVECTOR3(RETURN_POS_X + RETRYMENU_SIZE_X, RETURN_POS_Y + RETRYMENU_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkReturn[0].rhw =
	g_vertexWkReturn[1].rhw =
	g_vertexWkReturn[2].rhw =
	g_vertexWkReturn[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkReturn[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkReturn[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkReturn[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkReturn[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkReturn[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkReturn[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkReturn[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkReturn[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// ���j���[�F�̐ݒ�
//=============================================================================
void SetPoseMenuDiffuse(int menu)
{
	switch (menu)
	{
	case 0:
		g_vertexWkRetry[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkRetry[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkRetry[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkRetry[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		g_vertexWkReturn[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkReturn[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkReturn[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkReturn[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		break;
	case 1:
		g_vertexWkRetry[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkRetry[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkRetry[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkRetry[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);

		g_vertexWkReturn[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkReturn[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkReturn[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkReturn[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		break;
	}
}

//=============================================================================
// �|�[�Y�̎擾
//=============================================================================
bool GetPose(void)
{
	return g_pose;
}

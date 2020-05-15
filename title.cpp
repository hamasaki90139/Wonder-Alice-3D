//=============================================================================
//
// �^�C�g����ʏ��� [title.cpp]
// Author : �_�� ���t
//
//=============================================================================
#include "main.h"
#include "title.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "tutorial.h"
#include "meshsky.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexTitle(void);
void SetVertexBG(void);
void SetMenu(int menu);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureTitle1 = NULL;						// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTextureTitle2 = NULL;						// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTextureTitleLogo = NULL;					// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTextureTitleNewgame = NULL;				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTextureTitleExit = NULL;					// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTextureTitleSky[NUM_BG] = { NULL };		// �e�N�X�`���ւ̃|�C���^

VERTEX_2D				g_vertexWkTitle1[NUM_VERTEX];					// ���_���i�[���[�N
VERTEX_2D				g_vertexWkTitle2[NUM_VERTEX];					// ���_���i�[���[�N
VERTEX_2D				g_vertexWkTitleLogo[NUM_VERTEX];				// ���_���i�[���[�N
VERTEX_2D				g_vertexWkTitleNewgame[NUM_VERTEX];				// ���_���i�[���[�N
VERTEX_2D				g_vertexWkTitleExit[NUM_VERTEX];				// ���_���i�[���[�N
VERTEX_2D				g_vertexWkTitleSky[NUM_BG][NUM_VERTEX];			// ���_���i�[���[�N

static float			g_scale;										// ���j���[�̊g��k����
static int				g_menu; 										// ���j���[�p�ϐ�

static D3DXVECTOR3		g_pos;											// BG�̈ړ���
static float			g_interval[NUM_BG];								// �w�i���[�v�̃C���^�o�[��

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTitle(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		/*�e�N�X�`���̓ǂݍ���*/
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_TITLE01,									// �t�@�C���̖��O
			&g_pD3DTextureTitle1);								// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_TITLE02,									// �t�@�C���̖��O
			&g_pD3DTextureTitle2);								// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_TITLE_LOGO,									// �t�@�C���̖��O
			&g_pD3DTextureTitleLogo);							// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_TITLE_NEWGAME,								// �t�@�C���̖��O
			&g_pD3DTextureTitleNewgame);						// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_TITLE_EXIT,									// �t�@�C���̖��O
			&g_pD3DTextureTitleExit);							// �ǂݍ��ރ������[

		for (int i = 0; i < NUM_BG; i++)
		{
			D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
				TEXTURE_SKY,									// �t�@�C���̖��O
				&g_pD3DTextureTitleSky[i]);						// �ǂݍ��ރ������[
		}
	}

	// �w�i���[�v�̂��߂ɃC���^�\�o������������
	for (int i = 0; i < NUM_BG; i++)
	{
		g_interval[i] = i * TITLE_SKY_SIZE_X;
	}

	g_menu = 0;										// ���j���[������
	g_scale = 0;									// ���j���[�g��k���ʂ̏�����
	g_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// BG�ړ��ʂ̏�����

	// ���_���̍쐬
	MakeVertexTitle();

	// BGM�Đ�
	PlaySound(SOUND_LABEL_TITLE);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTitle(void)
{
	if (g_pD3DTextureTitle1 != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureTitle1->Release();
		g_pD3DTextureTitle1 = NULL;
	}

	if (g_pD3DTextureTitle2 != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureTitle2->Release();
		g_pD3DTextureTitle2 = NULL;
	}

	if (g_pD3DTextureTitleLogo != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureTitleLogo->Release();
		g_pD3DTextureTitleLogo = NULL;
	}

	if (g_pD3DTextureTitleNewgame != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureTitleNewgame->Release();
		g_pD3DTextureTitleNewgame = NULL;
	}

	if (g_pD3DTextureTitleExit != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureTitleExit->Release();
		g_pD3DTextureTitleExit = NULL;
	}

	for (int i = 0; i < NUM_BG; i++)
	{
		if (g_pD3DTextureTitleSky[i] != NULL)
		{// �e�N�X�`���̊J��
			g_pD3DTextureTitleSky[i]->Release();
			g_pD3DTextureTitleSky[i] = NULL;
		}
	}

	// BGM��~
	StopSound(SOUND_LABEL_TITLE);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTitle(void)
{
	if (GetKeyboardRepeat(DIK_UP) || GetKeyboardRepeat(DIK_DOWN) ||
		(IsButtonTriggered(0, BUTTON_UP)) || (IsButtonTriggered(0, BUTTON_DOWN)))
	{
		// ���j���[�I���̐؂�ւ�
		g_menu = (g_menu + 1) % MENU_MAX;

		// SE�Đ�
		PlaySound(SOUND_LABEL_SE_MENU);
	}

	switch (g_menu)
	{
	case NEWGAME:
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			// SE�Đ�
			PlaySound(SOUND_LABEL_SE_OK);
			// Enter��������A�X�e�[�W��؂�ւ���
			SetFade(FADE_OUT);
		}
		// �Q�[���p�b�h�ňړ�����
		else if (IsButtonTriggered(0, BUTTON_C))
		{
			// SE�Đ�
			PlaySound(SOUND_LABEL_SE_OK);
			SetFade(FADE_OUT);
		}
		break;
	case EXIT:
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			// SE�Đ�
			PlaySound(SOUND_LABEL_SE_OK);
			exit(-1);		//�Q�[���I��
		}
		// �Q�[���p�b�h�ňړ�����
		else if (IsButtonTriggered(0, BUTTON_C))
		{
			// SE�Đ�
			PlaySound(SOUND_LABEL_SE_OK);
			exit(-1);		//�Q�[���I��
		}
		break;
	}

	// �X�N���[������
	for (int i = 0; i < NUM_BG; i++)
	{
		// ���t���[�����ɃX�N���[��������
		g_pos.x -= 1.0f;

		//���[�v����
		if (g_interval[i] + TITLE_SKY_SIZE_X + g_pos.x < 0)
		{
			g_interval[i] += NUM_BG * TITLE_SKY_SIZE_X;
		}

		// �ړ���̍��W�Œ��_��ݒ�
		SetVertexBG();
	}

	// ���j���[���_���W�̍X�V
	SetMenu(g_menu);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < NUM_BG; i++)
	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureTitleSky[i]);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitleSky[i], sizeof(VERTEX_2D));
	}

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureTitle1);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitle1, sizeof(VERTEX_2D));


	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureTitle2);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitle2, sizeof(VERTEX_2D));


	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureTitleLogo);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitleLogo, sizeof(VERTEX_2D));


	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureTitleNewgame);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitleNewgame, sizeof(VERTEX_2D));


	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureTitleExit);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkTitleExit, sizeof(VERTEX_2D));

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexTitle(void)
{
	// ���_���W�̐ݒ�
	g_vertexWkTitle1[0].vtx = D3DXVECTOR3(TITLE01_POS_X, TITLE01_POS_Y, 0.0f);
	g_vertexWkTitle1[1].vtx = D3DXVECTOR3(TITLE01_POS_X + TITLE01_SIZE_X, TITLE01_POS_Y, 0.0f);
	g_vertexWkTitle1[2].vtx = D3DXVECTOR3(TITLE01_POS_X, TITLE01_POS_Y + TITLE01_SIZE_Y, 0.0f);
	g_vertexWkTitle1[3].vtx = D3DXVECTOR3(TITLE01_POS_X + TITLE01_SIZE_X, TITLE01_POS_Y + TITLE01_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkTitle1[0].rhw =
		g_vertexWkTitle1[1].rhw =
		g_vertexWkTitle1[2].rhw =
		g_vertexWkTitle1[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkTitle1[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitle1[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitle1[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitle1[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkTitle1[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitle1[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitle1[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitle1[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// ���_���W�̐ݒ�
	g_vertexWkTitle2[0].vtx = D3DXVECTOR3(TITLE02_POS_X, TITLE02_POS_Y, 0.0f);
	g_vertexWkTitle2[1].vtx = D3DXVECTOR3(TITLE02_POS_X + TITLE02_SIZE_X, TITLE02_POS_Y, 0.0f);
	g_vertexWkTitle2[2].vtx = D3DXVECTOR3(TITLE02_POS_X, TITLE02_POS_Y + TITLE02_SIZE_Y, 0.0f);
	g_vertexWkTitle2[3].vtx = D3DXVECTOR3(TITLE02_POS_X + TITLE02_SIZE_X, TITLE02_POS_Y + TITLE02_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkTitle2[0].rhw =
		g_vertexWkTitle2[1].rhw =
		g_vertexWkTitle2[2].rhw =
		g_vertexWkTitle2[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkTitle2[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitle2[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitle2[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitle2[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkTitle2[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitle2[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitle2[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitle2[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// ���_���W�̐ݒ�
	g_vertexWkTitleLogo[0].vtx = D3DXVECTOR3(TITLELOGO_POS_X, TITLELOGO_POS_Y, 0.0f);
	g_vertexWkTitleLogo[1].vtx = D3DXVECTOR3(TITLELOGO_POS_X + TITLELOGO_SIZE_X, TITLELOGO_POS_Y, 0.0f);
	g_vertexWkTitleLogo[2].vtx = D3DXVECTOR3(TITLELOGO_POS_X, TITLELOGO_POS_Y + TITLELOGO_SIZE_Y, 0.0f);
	g_vertexWkTitleLogo[3].vtx = D3DXVECTOR3(TITLELOGO_POS_X + TITLELOGO_SIZE_X, TITLELOGO_POS_Y + TITLELOGO_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkTitleLogo[0].rhw =
		g_vertexWkTitleLogo[1].rhw =
		g_vertexWkTitleLogo[2].rhw =
		g_vertexWkTitleLogo[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkTitleLogo[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitleLogo[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitleLogo[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkTitleLogo[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkTitleLogo[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitleLogo[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitleLogo[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitleLogo[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���j���[���_�̍쐬
	SetMenu(g_menu);

	//SKY
	// ���_���W�̐ݒ�
	SetVertexBG();

	for (int i = 0; i < NUM_BG; i++)
	{
		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		g_vertexWkTitleSky[i][0].rhw =
			g_vertexWkTitleSky[i][1].rhw =
			g_vertexWkTitleSky[i][2].rhw =
			g_vertexWkTitleSky[i][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		g_vertexWkTitleSky[i][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTitleSky[i][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTitleSky[i][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTitleSky[i][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		g_vertexWkTitleSky[i][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		g_vertexWkTitleSky[i][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		g_vertexWkTitleSky[i][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		g_vertexWkTitleSky[i][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}

//=============================================================================
// �w�i���_���W�̐ݒ�
//=============================================================================
void SetVertexBG(void)
{
	// �w�i���_���W�̐ݒ�i���[�v�j
	for (int i = 0; i < NUM_BG; i++)
	{
		g_vertexWkTitleSky[i][0].vtx = D3DXVECTOR3(TITLE_SKY_POS_X + g_interval[i], TITLE_SKY_POS_Y, 0.0f) + g_pos;
		g_vertexWkTitleSky[i][1].vtx = D3DXVECTOR3(TITLE_SKY_POS_X + TITLE_SKY_SIZE_X + g_interval[i], TITLE_SKY_POS_Y, 0.0f) + g_pos;
		g_vertexWkTitleSky[i][2].vtx = D3DXVECTOR3(TITLE_SKY_POS_X + g_interval[i], TITLE_SKY_POS_Y + TITLE_SKY_SIZE_Y, 0.0f) + g_pos;
		g_vertexWkTitleSky[i][3].vtx = D3DXVECTOR3(TITLE_SKY_POS_X + TITLE_SKY_SIZE_X + g_interval[i], TITLE_SKY_POS_Y + TITLE_SKY_SIZE_Y, 0.0f) + g_pos;
	}
}

//=============================================================================
// ���j���[�F�̐ݒ�
//=============================================================================
void SetMenu(int menu)
{
	switch (menu)
	{
	case NEWGAME:
		for (int cnt = 0; cnt < 60; cnt++)
		{
			g_scale += 0.001f;

			// ���_���W�̐ݒ�
			g_vertexWkTitleNewgame[0].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X - g_scale, TITLE_NEWGAME_POS_Y - g_scale, 0.0f);
			g_vertexWkTitleNewgame[1].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X + TITLE_MENU_SIZE_X + g_scale, TITLE_NEWGAME_POS_Y - g_scale, 0.0f);
			g_vertexWkTitleNewgame[2].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X - g_scale, TITLE_NEWGAME_POS_Y + TITLE_MENU_SIZE_Y + g_scale, 0.0f);
			g_vertexWkTitleNewgame[3].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X + TITLE_MENU_SIZE_X + g_scale, TITLE_NEWGAME_POS_Y + TITLE_MENU_SIZE_Y + g_scale, 0.0f);

			if (g_scale >= 2.0f)	// �g��I���H
			{
				g_scale = 0.0f;
				g_vertexWkTitleNewgame[0].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X + g_scale, TITLE_NEWGAME_POS_Y + g_scale, 0.0f);
				g_vertexWkTitleNewgame[1].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X + TITLE_MENU_SIZE_X - g_scale, TITLE_NEWGAME_POS_Y + g_scale, 0.0f);
				g_vertexWkTitleNewgame[2].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X + g_scale, TITLE_NEWGAME_POS_Y + TITLE_MENU_SIZE_Y - g_scale, 0.0f);
				g_vertexWkTitleNewgame[3].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X + TITLE_MENU_SIZE_X - g_scale, TITLE_NEWGAME_POS_Y + TITLE_MENU_SIZE_Y - g_scale, 0.0f);
				break;
			}
		}
		// ���_���W�̐ݒ�
		g_vertexWkTitleExit[0].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X, TITLE_EXIT_POS_Y, 0.0f);
		g_vertexWkTitleExit[1].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X + TITLE_MENU_SIZE_X, TITLE_EXIT_POS_Y, 0.0f);
		g_vertexWkTitleExit[2].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X, TITLE_EXIT_POS_Y + TITLE_MENU_SIZE_Y, 0.0f);
		g_vertexWkTitleExit[3].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X + TITLE_MENU_SIZE_X, TITLE_EXIT_POS_Y + TITLE_MENU_SIZE_Y, 0.0f);

		// ���ˌ��̐ݒ�
		g_vertexWkTitleNewgame[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTitleNewgame[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTitleNewgame[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTitleNewgame[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		g_vertexWkTitleExit[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkTitleExit[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkTitleExit[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkTitleExit[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		break;

	case EXIT:
		for (int cnt = 0; cnt < 60; cnt++)
		{
			g_scale += 0.001f;

			// ���_���W�̐ݒ�
			g_vertexWkTitleExit[0].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X - g_scale, TITLE_EXIT_POS_Y - g_scale, 0.0f);
			g_vertexWkTitleExit[1].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X + TITLE_MENU_SIZE_X + g_scale, TITLE_EXIT_POS_Y - g_scale, 0.0f);
			g_vertexWkTitleExit[2].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X - g_scale, TITLE_EXIT_POS_Y + TITLE_MENU_SIZE_Y + g_scale, 0.0f);
			g_vertexWkTitleExit[3].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X + TITLE_MENU_SIZE_X + g_scale, TITLE_EXIT_POS_Y + TITLE_MENU_SIZE_Y + g_scale, 0.0f);

			if (g_scale >= 2.0f)	// �g��I���H
			{
				g_scale = 0.0f;
				g_vertexWkTitleExit[0].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X + g_scale, TITLE_EXIT_POS_Y + g_scale, 0.0f);
				g_vertexWkTitleExit[1].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X + TITLE_MENU_SIZE_X - g_scale, TITLE_EXIT_POS_Y + g_scale, 0.0f);
				g_vertexWkTitleExit[2].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X + g_scale, TITLE_EXIT_POS_Y + TITLE_MENU_SIZE_Y - g_scale, 0.0f);
				g_vertexWkTitleExit[3].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X + TITLE_MENU_SIZE_X - g_scale, TITLE_EXIT_POS_Y + TITLE_MENU_SIZE_Y - g_scale, 0.0f);
				break;
			}
		}
		g_vertexWkTitleNewgame[0].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X, TITLE_NEWGAME_POS_Y, 0.0f);
		g_vertexWkTitleNewgame[1].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X + TITLE_MENU_SIZE_X, TITLE_NEWGAME_POS_Y, 0.0f);
		g_vertexWkTitleNewgame[2].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X, TITLE_NEWGAME_POS_Y + TITLE_MENU_SIZE_Y, 0.0f);
		g_vertexWkTitleNewgame[3].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X + TITLE_MENU_SIZE_X, TITLE_NEWGAME_POS_Y + TITLE_MENU_SIZE_Y, 0.0f);

		// ���ˌ��̐ݒ�
		g_vertexWkTitleNewgame[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkTitleNewgame[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkTitleNewgame[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkTitleNewgame[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);

		g_vertexWkTitleExit[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTitleExit[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTitleExit[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTitleExit[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		break;
	}
	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkTitleNewgame[0].rhw =
		g_vertexWkTitleNewgame[1].rhw =
		g_vertexWkTitleNewgame[2].rhw =
		g_vertexWkTitleNewgame[3].rhw = 1.0f;

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkTitleNewgame[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitleNewgame[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitleNewgame[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitleNewgame[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//EXIT
	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkTitleExit[0].rhw =
		g_vertexWkTitleExit[1].rhw =
		g_vertexWkTitleExit[2].rhw =
		g_vertexWkTitleExit[3].rhw = 1.0f;

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkTitleExit[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkTitleExit[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkTitleExit[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkTitleExit[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}



//=============================================================================
//
// ���C�t���� [life.cpp]
// Author : �_�� ���t
//
//=============================================================================
#include "life.h"
#include "fade.h"
#include "player.h"
#include "debugproc.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexLife(LPDIRECT3DDEVICE9 pDevice);
void SetTextureLife(int nIdx);
HRESULT MakeVertexPlayerUI(void);
void SetVertexPlayerUI(void);

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureLife[2] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffLife = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

LPDIRECT3DTEXTURE9		g_pD3DTexturePlayerUI = {};		// �e�N�X�`���ւ̃|�C���^
VERTEX_2D				g_vertexWkPlayerUI[NUM_VERTEX];					// ���_���i�[���[�N

D3DXVECTOR3				g_posLife;						// �ʒu
D3DXVECTOR3				g_rotLife;						// ����

int						g_nLife;						// ���C�t

D3DXVECTOR3				g_posUIplayer;					// �ʒu
D3DXVECTOR3				g_rotUIplayer;					// ����

//=============================================================================
// ����������
//=============================================================================
HRESULT InitLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_posLife = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotLife = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_posUIplayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotUIplayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ���C�t�̏�����
	g_nLife = NUM_PLACE;

	// ���_���̍쐬
	MakeVertexLife(pDevice);
	MakeVertexPlayerUI();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_LIFE,			// �t�@�C���̖��O
		&g_pD3DTextureLife[0]);	// �ǂݍ��ރ������[
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_FRAME_LIFE,			// �t�@�C���̖��O
		&g_pD3DTextureLife[1]);	// �ǂݍ��ރ������[
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_PLAYER_UI,			// �t�@�C���̖��O
		&g_pD3DTexturePlayerUI);	// �ǂݍ��ރ������[

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitLife(void)
{
	if (g_pD3DTexturePlayerUI != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTexturePlayerUI->Release();
		g_pD3DTexturePlayerUI = NULL;
	}
	if (g_pD3DTextureLife[0] != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureLife[0]->Release();
		g_pD3DTextureLife[0] = NULL;
	}
	if (g_pD3DTextureLife[1] != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureLife[1]->Release();
		g_pD3DTextureLife[1] = NULL;
	}
	if (g_pD3DVtxBuffLife != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffLife->Release();
		g_pD3DVtxBuffLife = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateLife(void)
{
	PLAYER *player = GetPlayer();

	if (g_nLife <= 0)
	{
		player->use = false;
		SetFade(FADE_OUT);
	}

	SetVertexPlayerUI();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// playerUI
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTexturePlayerUI);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkPlayerUI, sizeof(VERTEX_2D));

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pD3DVtxBuffLife, 0, sizeof(VERTEX_2D));

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureLife[1]);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (NUM_PLACE * 4), NUM_POLYGON);

	// LIFE
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureLife[0]);

	// �|���S���̕`��
	for (int nCntPlace = 0; nCntPlace < g_nLife; nCntPlace++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntPlace * 4), NUM_POLYGON);
	}

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexLife(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (NUM_VERTEX * NUM_PLACE + 4),		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,								// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffLife,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))										// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(LIFE_POS_X + nCntPlace * LIFE_SIZE_X + LIFE_INTERVAL_X, LIFE_POS_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(LIFE_POS_X + nCntPlace * (LIFE_INTERVAL_X + LIFE_SIZE_X) + LIFE_SIZE_X, LIFE_POS_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(LIFE_POS_X + nCntPlace * LIFE_SIZE_X + LIFE_INTERVAL_X, LIFE_POS_Y + LIFE_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(LIFE_POS_X + nCntPlace * (LIFE_INTERVAL_X + LIFE_SIZE_X) + LIFE_SIZE_X, LIFE_POS_Y + LIFE_SIZE_Y, 0.0f);

			// rhw�̐ݒ�
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(LIFE_POS_X - 35, LIFE_POS_Y - 25, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(LIFE_POS_X + (LIFE_INTERVAL_X + LIFE_SIZE_X) * NUM_PLACE + 15, LIFE_POS_Y - 25, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(LIFE_POS_X - 35, LIFE_POS_Y + 75, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(LIFE_POS_X + (LIFE_INTERVAL_X + LIFE_SIZE_X) * NUM_PLACE + 15, LIFE_POS_Y + 75, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffLife->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexPlayerUI(void)
{
	// ���_���W�̐ݒ�
	g_vertexWkPlayerUI[0].vtx = D3DXVECTOR3(PLAYER_UI_POS_X, PLAYER_UI_POS_Y, 0.0f);
	g_vertexWkPlayerUI[1].vtx = D3DXVECTOR3(PLAYER_UI_POS_X + PLAYER_UI_SIZE_X, PLAYER_UI_POS_Y, 0.0f);
	g_vertexWkPlayerUI[2].vtx = D3DXVECTOR3(PLAYER_UI_POS_X, PLAYER_UI_POS_Y + PLAYER_UI_SIZE_Y, 0.0f);
	g_vertexWkPlayerUI[3].vtx = D3DXVECTOR3(PLAYER_UI_POS_X + PLAYER_UI_SIZE_X, PLAYER_UI_POS_Y + PLAYER_UI_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkPlayerUI[0].rhw =
		g_vertexWkPlayerUI[1].rhw =
		g_vertexWkPlayerUI[2].rhw =
		g_vertexWkPlayerUI[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	SetVertexPlayerUI();

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkPlayerUI[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkPlayerUI[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkPlayerUI[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkPlayerUI[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureLife(int nIdx)
{
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdx * 4);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�f�[�^���A�����b�N����
	g_pD3DVtxBuffLife->Unlock();
}

//=============================================================================
// ���C�t�̕ύX
//=============================================================================
void ChangeLife(int nValue)
{
	g_nLife += nValue;
	if (g_nLife > NUM_PLACE)
	{
		g_nLife = NUM_PLACE;
	}
	else if (g_nLife < 0)
	{
		g_nLife = 0;
	}
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexPlayerUI(void)
{
	PLAYER *player = GetPlayer();

	if (player->protect == true)
	{
		// ���ˌ��̐ݒ�
		g_vertexWkPlayerUI[0].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
		g_vertexWkPlayerUI[1].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
		g_vertexWkPlayerUI[2].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
		g_vertexWkPlayerUI[3].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
	}
	else
	{
		// ���ˌ��̐ݒ�
		g_vertexWkPlayerUI[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkPlayerUI[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkPlayerUI[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkPlayerUI[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	}
}


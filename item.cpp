//=============================================================================
//
// �A�C�e������ [item.cpp]
// Author : �_�� ���t
//
//=============================================================================
#include "item.h"
#include "camera.h"
#include "shadow.h"
#include "player.h"
#include "score.h"
#include "life.h"
#include "collision.h"
#include "debugproc.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	VALUE_ROTATE_ITEM		(D3DX_PI * 0.025f)						// ��]���x
#define	ITEM_RADIUS				(10.0f)									// ���a
#define	ITEM_TEXTURE01			"data/TEXTURE/��cake_001.png"			// �ǂݍ��ރ��f����
#define	ITEM_TEXTURE02			"data/TEXTURE/��cookie_001.png"			// �ǂݍ��ރ��f����
#define	ITEM_TEXTURE03			"data/TEXTURE/��tea_001.png"			// �ǂݍ��ރ��f����
#define	ITEM_WIDTH				(50.0f)									// �A�C�e���̔��a��
#define	ITEM_HEIGHT				(50.0f)									// �A�C�e���̔��a��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexItem(LPDIRECT3DDEVICE9 pDevice);
void SetVertexItem(int nIdxItem, float fWidth, float fHeight);
void SetColorItem(int nIdxItem, D3DXCOLOR col);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ITEM				g_Item[MAX_ITEM];						// �A�C�e��
LPDIRECT3DTEXTURE9		g_apD3DTextureItem[ITEM_TYPE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffItem = NULL;				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
static int				g_nAlpha;								// �A���t�@�e�X�g��臒l

//=============================================================================
// ����������
//=============================================================================
HRESULT InitItem(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���[�̏��������s���H
	if (type == 0)
	{
		for (int i = 0; i < ITEM_TYPE_MAX; i++)
		{
			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�̃|�C���^
				ITEM_TEXTURE01,										// �t�@�C���̖��O
				&g_apD3DTextureItem[ITEM_CAKE]);					// �ǂݍ��ރ������̃|�C���^

			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�̃|�C���^
				ITEM_TEXTURE02,										// �t�@�C���̖��O
				&g_apD3DTextureItem[ITEM_COOKIE]);					// �ǂݍ��ރ������̃|�C���^

			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�̃|�C���^
				ITEM_TEXTURE03,										// �t�@�C���̖��O
				&g_apD3DTextureItem[ITEM_TEA]);						// �ǂݍ��ރ������̃|�C���^
		}
	}

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		switch (nCntItem % ITEM_TYPE_MAX)
		{
		case ITEM_CAKE:
			// �A�C�e���^�C�v�̓o�^
			g_Item[nCntItem].type = ITEM_CAKE;
			break;

		case ITEM_COOKIE:
			// �A�C�e���^�C�v�̓o�^
			g_Item[nCntItem].type = ITEM_COOKIE;
			break;

		case ITEM_TEA:
			// �A�C�e���^�C�v�̓o�^
			g_Item[nCntItem].type = ITEM_TEA;
			break;
		}

#if 0
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,									// �f�o�C�X�ւ̃|�C���^
			TEXTURE_FILENAME,												// �t�@�C���̖��O
			&g_pD3DTextureModelItem[nCntItemType]);							// �ǂݍ��ރ������[
#endif

		g_Item[nCntItem].pos = D3DXVECTOR3(0.0f, 100.0f, 0.0f);				// �A�C�e�������ʒuY������������
		g_Item[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Item[nCntItem].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Item[nCntItem].nIdxShadow = -1;
		g_Item[nCntItem].bUse = false;
	}

	// ���_���̍쐬
	MakeVertexItem(pDevice);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitItem(void)
{
	for (int i = 0; i < ITEM_TYPE_MAX; i++)
	{
		if (g_apD3DTextureItem[i] != NULL)
		{// �e�N�X�`���̊J��
			g_apD3DTextureItem[i]->Release();
			g_apD3DTextureItem[i] = NULL;
		}
	}

	if (g_pD3DVtxBuffItem != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffItem->Release();
		g_pD3DVtxBuffItem = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateItem(void)
{
	ENEMY *enemy = GetEnemy();
	PLAYER *player = GetPlayer();
	SCORE *score = GetScore();

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_Item[nCntItem].bUse)
		{	
			// �A�C�e�����G�l�~�[�����ł����ʒu�֏o��������
			g_Item[nCntItem].pos.x = enemy[nCntItem].hit_oldpos.x;
			g_Item[nCntItem].pos.z = enemy[nCntItem].hit_oldpos.z;

			// �d�͏���
			g_Item[nCntItem].pos.y -= 1.0f;
			if (g_Item[nCntItem].pos.y < 0.0f)
			{
				g_Item[nCntItem].pos.y = 0.0f;
			}

			// �e�̈ʒu�ݒ�
			D3DXVECTOR3 pos = g_Item[nCntItem].pos;
			pos.y = 0.0f;
			SetPositionShadow(g_Item[nCntItem].nIdxShadow, pos, g_Item[nCntItem].scl);

			//�v���C���[�ƃA�C�e���̏Փ˔���
			float r1, r2;
			r1 = ITEM_R;									//�A�C�e���J�v�Z���̔��a
			r2 = PLAYER_R;									//�v���C���[�J�v�Z���̔��a

			if (CheckHitCC(g_Item[nCntItem].pos, D3DXVECTOR3(g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y, g_Item[nCntItem].pos.z), player->pos, D3DXVECTOR3(player->pos.x, player->pos.y + ENEMY_LEN, player->pos.z), r1, r2))
			{
				// SE�Đ�
				PlaySound(SOUND_LABEL_SE_ITEM);

				if (g_Item[nCntItem].type == ITEM_CAKE)				//�P�[�L��������X�R�A10�~�{��
				{
					for (int i = 0; i < 1; i++)
					{
						score->score += 10 * score->score_mag;
					}
				}
				else if (g_Item[nCntItem].type == ITEM_COOKIE)		//�N�b�L�[�������烉�C�t��
				{
					for (int i = 0; i < 1; i++)
					{
						ChangeLife(1);
					}
				}
				else if (g_Item[nCntItem].type == ITEM_TEA)			//�g����������X�R�A2�{
				{
					for (int i = 0; i < 1; i++)
					{
						score->score_mag++;
					}
				}

				DeleteItem(nCntItem);		//����
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, g_nAlpha);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (g_Item[i].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Item[i].mtxWorld);

			// �r���[�}�g���b�N�X���擾
			mtxView = GetMtxView();

			// �|���S���𐳖ʂɌ�����
			// �t�s������Ƃ߂�
			g_Item[i].mtxWorld._11 = mtxView._11;
			g_Item[i].mtxWorld._12 = mtxView._21;
			g_Item[i].mtxWorld._13 = mtxView._31;

			g_Item[i].mtxWorld._21 = mtxView._12;
			g_Item[i].mtxWorld._22 = mtxView._22;
			g_Item[i].mtxWorld._23 = mtxView._32;

			g_Item[i].mtxWorld._31 = mtxView._13;
			g_Item[i].mtxWorld._32 = mtxView._23;
			g_Item[i].mtxWorld._33 = mtxView._33;

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, g_Item[i].scl.x,
				g_Item[i].scl.y,
				g_Item[i].scl.z);
			D3DXMatrixMultiply(&g_Item[i].mtxWorld, &g_Item[i].mtxWorld, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_Item[i].pos.x,
				g_Item[i].pos.y,
				g_Item[i].pos.z);
			D3DXMatrixMultiply(&g_Item[i].mtxWorld, &g_Item[i].mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_Item[i].mtxWorld);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pD3DVtxBuffItem, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apD3DTextureItem[i % ITEM_TYPE_MAX]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * 4), NUM_POLYGON);
		}
	}

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// ���e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexItem(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_ITEM,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,						// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,							// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,						// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffItem,						// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))									// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-ITEM_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(-ITEM_WIDTH / 2.0f, ITEM_HEIGHT, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(ITEM_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(ITEM_WIDTH / 2.0f, ITEM_HEIGHT, 0.0f);

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);
		}

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffItem->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexItem(int nIdxItem, float fWidth, float fHeight)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxItem * 4);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-fWidth / 2.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fWidth / 2.0f, fHeight, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fWidth / 2.0f, 0.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fWidth / 2.0f, fHeight, 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffItem->Unlock();
	}
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorItem(int nIdxItem, D3DXCOLOR col)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxItem * 4);

		// ���_���W�̐ݒ�
		pVtx[0].diffuse = col;
		pVtx[1].diffuse = col;
		pVtx[2].diffuse = col;
		pVtx[3].diffuse = col;

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffItem->Unlock();
	}
}

//=============================================================================
// �A�C�e���̍폜
//=============================================================================
void DeleteItem(int nIdxItem)
{
	if (nIdxItem >= 0 && nIdxItem < MAX_ITEM)
	{
		ReleaseShadow(g_Item[nIdxItem].nIdxShadow);
		g_Item[nIdxItem].bUse = false;
	}
}

//=============================================================================
// �A�C�e���̎擾
//=============================================================================
ITEM *GetItem(void)
{
	return &g_Item[0];
}

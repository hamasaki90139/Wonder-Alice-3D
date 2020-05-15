//=============================================================================
//
// �^�C�}�[���� [timer.cpp]
// Author : �_�� ���t
//
//=============================================================================
#include "timer.h"
#include "fade.h"
#include "debugproc.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexTimer(LPDIRECT3DDEVICE9 pDevice);
HRESULT MakeVertexTimerCnt(void);
void SetVertexTimerCnt(void);
void SetTextureTimer(int idx, int number);
void SetTextureTimerCnt(int cntPattern);

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9			g_pD3DTextureTimer = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pD3DVtxBuffTimer = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
VERTEX_2D					g_vertexWkTimer[NUM_VERTEX];	// ���_���i�[���[�N

static TIMER				timerWk;						// �^�C�}�[�\����

static D3DXVECTOR3			g_posTimer;						// �^�C�}�[�ʒu
static D3DXVECTOR3			g_rotTimer;						// �^�C�}�[��]
static int					g_nTimer;						// �^�C�}�[�J�E���g
static bool					g_bEnableTimer;					// �^�C�}�[����ON/OFF

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_FRAME_TIMER,			// �t�@�C���̖��O
		&g_pD3DTextureTimer);	// �ǂݍ��ރ������[
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_TOKEI,			// �t�@�C���̖��O
		&timerWk.g_pD3DTextureTimer);	// �ǂݍ��ރ������[

	// �P�[�L�̏���������
	timerWk.use = true;										// �g�p
	timerWk.pos = D3DXVECTOR3(HARI_POS_X, HARI_POS_Y, 0.0f);	// ���W�f�[�^��������
	timerWk.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ��]�f�[�^��������

	D3DXVECTOR2 temp = D3DXVECTOR2(HARI_SIZE_X, HARI_SIZE_Y);
	timerWk.Radius = D3DXVec2Length(&temp);					// �I�u�W�F�N�g�̔��a��������
	timerWk.BaseAngle = atan2f(HARI_SIZE_Y, HARI_SIZE_X);	// �I�u�W�F�N�g�̊p�x��������

	// �^�C�}�[�̏�����
	g_nTimer = 999 * 60;
	g_bEnableTimer = true;

	// ���_���̍쐬
	MakeVertexTimer(pDevice);
	MakeVertexTimerCnt();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTimer(void)
{
	if (g_pD3DTextureTimer != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureTimer->Release();
		g_pD3DTextureTimer = NULL;
	}
	if (g_pD3DTextureTimer != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureTimer->Release();
		g_pD3DTextureTimer = NULL;
	}
	if (timerWk.g_pD3DTextureTimer != NULL)
	{// timerWk.�e�N�X�`���̊J��
		timerWk.g_pD3DTextureTimer->Release();
		timerWk.g_pD3DTextureTimer = NULL;
	}

	if (g_pD3DVtxBuffTimer != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffTimer->Release();
		g_pD3DVtxBuffTimer = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTimer(void)
{
	if (g_bEnableTimer)
	{
		g_nTimer--;
		if (g_nTimer < 0)
		{
			g_nTimer = 0;
			timerWk.rot.z = 0.0f;

			SetFade(FADE_OUT);
		}
		else if ((g_nTimer / 60) >= (int)(powf(10.0f, (float)(NUM_PLACE + 1))))
		{
			g_nTimer = (int)(powf(10.0f, (float)(NUM_PLACE + 1))) - 1;
		}
	}

	if (g_nTimer % TIMER_MAX == 0)
	{
		timerWk.rot.z += (D3DX_PI * 2) / 60;	//���v�̐j��60����1����]������
	}

	SetVertexTimerCnt();						// �ړ���̍��W�Œ��_��ݒ�

	//PrintDebugProc("g_timer_rotz %f\n", g_rotTimer.z);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pD3DVtxBuffTimer, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureTimer);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (NUM_PLACE * 4), NUM_POLYGON);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, timerWk.g_pD3DTextureTimer);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, timerWk.vertexWk, sizeof(VERTEX_2D));

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexTimer(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (NUM_VERTEX * NUM_PLACE + 4),	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,								// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,									// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,								// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffTimer,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))											// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(TIMER_POS_X + nCntPlace * TIMER_SIZE_X + TIMER_INTERVAL_X, TIMER_POS_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(TIMER_POS_X + nCntPlace * (TIMER_INTERVAL_X + TIMER_SIZE_X) + TIMER_SIZE_X, TIMER_POS_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(TIMER_POS_X + nCntPlace * TIMER_SIZE_X + TIMER_INTERVAL_X, TIMER_POS_Y + TIMER_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(TIMER_POS_X + nCntPlace * (TIMER_INTERVAL_X + TIMER_SIZE_X) + TIMER_SIZE_X, TIMER_POS_Y + TIMER_SIZE_Y, 0.0f);

			// rhw�̐ݒ�
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
			pVtx[1].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
			pVtx[2].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
			pVtx[3].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(TIMER_POS_X - 17.5f, TIMER_POS_Y - 25, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(TIMER_POS_X + (TIMER_INTERVAL_X + TIMER_SIZE_X) * NUM_PLACE + 17.5f, TIMER_POS_Y - 25, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(TIMER_POS_X - 17.5f, TIMER_POS_Y + 110, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(TIMER_POS_X + (TIMER_INTERVAL_X + TIMER_SIZE_X) * NUM_PLACE + 17.5f, TIMER_POS_Y + 110, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffTimer->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_�̍쐬(�P�[�L)
//=============================================================================
HRESULT MakeVertexTimerCnt(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���W�̐ݒ�
	SetVertexTimerCnt();

	// rhw�̐ݒ�
	timerWk.vertexWk[0].rhw =
		timerWk.vertexWk[1].rhw =
		timerWk.vertexWk[2].rhw =
		timerWk.vertexWk[3].rhw = 1.0f;

	timerWk.vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	timerWk.vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	timerWk.vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	timerWk.vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	timerWk.vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	timerWk.vertexWk[1].tex = D3DXVECTOR2(1.0f / TIMER_TEXTURE_PATTERN_DIVIDE_X, 0.0f);
	timerWk.vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TIMER_TEXTURE_PATTERN_DIVIDE_Y);
	timerWk.vertexWk[3].tex = D3DXVECTOR2(1.0f / TIMER_TEXTURE_PATTERN_DIVIDE_X, 1.0f / TIMER_TEXTURE_PATTERN_DIVIDE_Y);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureTimer(int idx, int number)
{
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (idx * 4);

	// ���_���W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(number * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(number * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 1.0f);

	// ���_�f�[�^���A�����b�N����
	g_pD3DVtxBuffTimer->Unlock();
}

void SetVertexTimerCnt(void)
{
	timerWk.vertexWk[0].vtx.x = timerWk.pos.x - cosf(timerWk.BaseAngle + timerWk.rot.z) * timerWk.Radius;
	timerWk.vertexWk[0].vtx.y = timerWk.pos.y - sinf(timerWk.BaseAngle + timerWk.rot.z) * timerWk.Radius;
	timerWk.vertexWk[0].vtx.z = 0.0f;

	timerWk.vertexWk[1].vtx.x = timerWk.pos.x + cosf(timerWk.BaseAngle - timerWk.rot.z) * timerWk.Radius;
	timerWk.vertexWk[1].vtx.y = timerWk.pos.y - sinf(timerWk.BaseAngle - timerWk.rot.z) * timerWk.Radius;
	timerWk.vertexWk[1].vtx.z = 0.0f;

	timerWk.vertexWk[2].vtx.x = timerWk.pos.x - cosf(timerWk.BaseAngle - timerWk.rot.z) * timerWk.Radius;
	timerWk.vertexWk[2].vtx.y = timerWk.pos.y + sinf(timerWk.BaseAngle - timerWk.rot.z) * timerWk.Radius;
	timerWk.vertexWk[2].vtx.z = 0.0f;

	timerWk.vertexWk[3].vtx.x = timerWk.pos.x + cosf(timerWk.BaseAngle + timerWk.rot.z) * timerWk.Radius;
	timerWk.vertexWk[3].vtx.y = timerWk.pos.y + sinf(timerWk.BaseAngle + timerWk.rot.z) * timerWk.Radius;
	timerWk.vertexWk[3].vtx.z = 0.0f;
}

void SetTextureTimerCnt(int cntPattern)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TIMER_TEXTURE_PATTERN_DIVIDE_X;
	int y = cntPattern / TIMER_TEXTURE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / TIMER_TEXTURE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / TIMER_TEXTURE_PATTERN_DIVIDE_Y;

	timerWk.vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	timerWk.vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	timerWk.vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	timerWk.vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// �^�C�}�[�̊J�n
//=============================================================================
void EnableTimer(bool bEnable)
{
	g_bEnableTimer = bEnable;
}

//=============================================================================
// �^�C�}�[�̃��Z�b�g
//=============================================================================
void ResetTimer(int nTime)
{
	g_nTimer = nTime * 60;
}


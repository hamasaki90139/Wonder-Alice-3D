//=============================================================================
//
// ���U���g��ʏ��� [result.cpp]
// Author : �_�� ���t
//
//=============================================================================
#include "main.h"
#include "result.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "player.h"
#include "score.h"
#include "title.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexResult(void);
HRESULT MakeVertexResultScore(void);
void	SetTextureResultScore(void);
void SetResultMenuDiffuse(int menu);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureResult01 = NULL;						// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTextureResult02 = NULL;						// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTextureResult03 = NULL;						// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTextureResultGameclear = NULL;				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTextureResultGameover = NULL;					// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTextureResultScore = NULL;					// �e�N�X�`���ւ̃|�C���^

LPDIRECT3DTEXTURE9		g_pD3DTextureResultmenu1 = NULL;					// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTextureResultmenu2 = NULL;					// �e�N�X�`���ւ̃|�C���^

VERTEX_2D				g_vertexWkResult01[NUM_VERTEX];						// ���_���i�[���[�N
VERTEX_2D				g_vertexWkResult02[NUM_VERTEX];						// ���_���i�[���[�N
VERTEX_2D				g_vertexWkResult03[NUM_VERTEX];						// ���_���i�[���[�N
VERTEX_2D				g_vertexWkResultGameclear[NUM_VERTEX];				// ���_���i�[���[�N
VERTEX_2D				g_vertexWkResultGameover[NUM_VERTEX];				// ���_���i�[���[�N
VERTEX_2D				g_vertexWkResultScore[SCORE_DIGIT][NUM_VERTEX];		// ���_���i�[���[�N
VERTEX_2D				g_vertexWkResultmenu1[NUM_VERTEX];					// ���_���i�[���[�N
VERTEX_2D				g_vertexWkResultmenu2[NUM_VERTEX];					// ���_���i�[���[�N

int						g_return_cnt;										//���U���g��ʑJ�ڗp
static int				menu; 												//���j���[�p�ϐ�


//=============================================================================
// ����������
//=============================================================================
HRESULT InitResult(int type)
{
	SCORE *score = GetScore();		// �X�R�A�̃|�C���^�[��������

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_RESULT01,				// �t�@�C���̖��O
			&g_pD3DTextureResult01);		// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_RESULT02,				// �t�@�C���̖��O
			&g_pD3DTextureResult02);		// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_RESULT03,				// �t�@�C���̖��O
			&g_pD3DTextureResult03);		// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_RESULT_GAMECLEAR,		// �t�@�C���̖��O
			&g_pD3DTextureResultGameclear);	// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_RESULT_GAMEOVER,		// �t�@�C���̖��O
			&g_pD3DTextureResultGameover);	// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			SCORE_TEXTURE,		// �t�@�C���̖��O
			&g_pD3DTextureResultScore);	// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_TITLE_NEWGAME,		// �t�@�C���̖��O
			&g_pD3DTextureResultmenu1);	// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_TITLE_EXIT,		// �t�@�C���̖��O
			&g_pD3DTextureResultmenu2);	// �ǂݍ��ރ������[
	}

	score->posScore = D3DXVECTOR3((float)RESULTSCORE_POS_X, (float)RESULTSCORE_POS_Y, 0.0f);

	g_return_cnt = 0;		//���U���g��ʑJ�ڗp�ϐ��̏�����
	menu = 0;				//���j���[�p�ϐ��̏�����

	// ���_���̍쐬
	MakeVertexResult();
	MakeVertexResultScore();

	if (GetPlayer()->use == true)	//�Q�[���N���A
	{
		// BGM�Đ�
		PlaySound(SOUND_LABEL_GAMECLEAR);
	}
	else
	{
		// BGM�Đ�
		PlaySound(SOUND_LABEL_GAMEOVER);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitResult(void)
{
	if (g_pD3DTextureResult01 != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureResult01->Release();
		g_pD3DTextureResult01 = NULL;
	}

	if (g_pD3DTextureResult02 != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureResult02->Release();
		g_pD3DTextureResult02 = NULL;
	}

	if (g_pD3DTextureResult03 != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureResult03->Release();
		g_pD3DTextureResult03 = NULL;
	}

	if (g_pD3DTextureResultGameclear != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureResultGameclear->Release();
		g_pD3DTextureResultGameclear = NULL;
	}

	if (g_pD3DTextureResultGameover != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureResultGameover->Release();
		g_pD3DTextureResultGameover = NULL;
	}

	if (g_pD3DTextureResultScore != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureResultScore->Release();
		g_pD3DTextureResultScore = NULL;
	}

	if (g_pD3DTextureResultmenu1 != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureResultmenu1->Release();
		g_pD3DTextureResultmenu1 = NULL;
	}

	if (g_pD3DTextureResultmenu2 != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureResultmenu2->Release();
		g_pD3DTextureResultmenu2 = NULL;
	}

	if (GetPlayer()->use == true)	//�Q�[���N���A
	{
		// BGM��~
		StopSound(SOUND_LABEL_GAMECLEAR);
	}
	else
	{
		// BGM��~
		StopSound(SOUND_LABEL_GAMEOVER);
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateResult(void)
{
	PLAYER *player = GetPlayer();			// �v���C���[�̃|�C���^�[��������

	if (player->use == true)	//�Q�[���N���A
	{
		SetTextureResultScore();	//���U���g�p�X�R�A�̃e�N�X�`���ݒ�

		if (GetKeyboardTrigger(DIK_RETURN))
		{
			SetFade(FADE_OUT);
		}
		else if (IsButtonTriggered(0, BUTTON_C))
		{
			SetFade(FADE_OUT);
		}
	}
	else
	{
		if (GetKeyboardTrigger(DIK_RETURN))
		{// Enter��������A�X�e�[�W��؂�ւ���
			SetFade(FADE_OUT);
		}
		else if (IsButtonTriggered(0, BUTTON_C))
		{// C�{�^���ŃX�e�[�W��؂�ւ���
			SetFade(FADE_OUT);
		}
	}
	SetResultMenuDiffuse(menu);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResult(void)
{
	PLAYER *player = GetPlayer();			// �v���C���[�̃|�C���^�[��������

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureResult01);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkResult01, sizeof(VERTEX_2D));

	if (player->use == true)	//�Q�[���N���A
	{	//�Q�[���N���A����
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureResultGameclear);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkResultGameclear, sizeof(VERTEX_2D));

		// �N�C�[��
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureResult02);
		{
			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, &g_vertexWkResult02, sizeof(VERTEX_2D));
		}

		//�X�R�A
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureResultScore);

		for (int i = 0; i < SCORE_DIGIT; i++)
		{
			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, &g_vertexWkResultScore[i][0], sizeof(VERTEX_2D));
		}
	}
	if (player->use == false)	//�Q�[���I�[�o�[
	{	//�Q�[���I�[�o�[����
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureResultGameover);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkResultGameover, sizeof(VERTEX_2D));

		// �A���X
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureResult03);
		{
			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, &g_vertexWkResult03, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexResult(void)
{
	// ���_���W�̐ݒ�
	g_vertexWkResult01[0].vtx = D3DXVECTOR3(RESULT01_POS_X, RESULT01_POS_Y, 0.0f);
	g_vertexWkResult01[1].vtx = D3DXVECTOR3(RESULT01_POS_X + RESULT01_SIZE_X, RESULT01_POS_Y, 0.0f);
	g_vertexWkResult01[2].vtx = D3DXVECTOR3(RESULT01_POS_X, RESULT01_POS_Y + RESULT01_SIZE_Y, 0.0f);
	g_vertexWkResult01[3].vtx = D3DXVECTOR3(RESULT01_POS_X + RESULT01_SIZE_X, RESULT01_POS_Y + RESULT01_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkResult01[0].rhw =
		g_vertexWkResult01[1].rhw =
		g_vertexWkResult01[2].rhw =
		g_vertexWkResult01[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkResult01[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResult01[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResult01[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResult01[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkResult01[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkResult01[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkResult01[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkResult01[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_���W�̐ݒ�
	g_vertexWkResult02[0].vtx = D3DXVECTOR3(RESULT02_POS_X, RESULT02_POS_Y, 0.0f);
	g_vertexWkResult02[1].vtx = D3DXVECTOR3(RESULT02_POS_X + RESULT02_SIZE_X, RESULT02_POS_Y, 0.0f);
	g_vertexWkResult02[2].vtx = D3DXVECTOR3(RESULT02_POS_X, RESULT02_POS_Y + RESULT02_SIZE_Y, 0.0f);
	g_vertexWkResult02[3].vtx = D3DXVECTOR3(RESULT02_POS_X + RESULT02_SIZE_X, RESULT02_POS_Y + RESULT02_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkResult02[0].rhw =
		g_vertexWkResult02[1].rhw =
		g_vertexWkResult02[2].rhw =
		g_vertexWkResult02[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkResult02[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResult02[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResult02[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResult02[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkResult02[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkResult02[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkResult02[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkResult02[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_���W�̐ݒ�
	g_vertexWkResult03[0].vtx = D3DXVECTOR3(RESULT03_POS_X, RESULT03_POS_Y, 0.0f);
	g_vertexWkResult03[1].vtx = D3DXVECTOR3(RESULT03_POS_X + RESULT03_SIZE_X, RESULT03_POS_Y, 0.0f);
	g_vertexWkResult03[2].vtx = D3DXVECTOR3(RESULT03_POS_X, RESULT03_POS_Y + RESULT03_SIZE_Y, 0.0f);
	g_vertexWkResult03[3].vtx = D3DXVECTOR3(RESULT03_POS_X + RESULT03_SIZE_X, RESULT03_POS_Y + RESULT03_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkResult03[0].rhw =
		g_vertexWkResult03[1].rhw =
		g_vertexWkResult03[2].rhw =
		g_vertexWkResult03[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkResult03[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResult03[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResult03[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResult03[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkResult03[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkResult03[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkResult03[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkResult03[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_���W�̐ݒ�
	g_vertexWkResultGameclear[0].vtx = D3DXVECTOR3(RESULTLOGO_POS_X, RESULTLOGO_POS_Y, 0.0f);
	g_vertexWkResultGameclear[1].vtx = D3DXVECTOR3(RESULTLOGO_POS_X + RESULTLOGO_SIZE_X, RESULTLOGO_POS_Y, 0.0f);
	g_vertexWkResultGameclear[2].vtx = D3DXVECTOR3(RESULTLOGO_POS_X, RESULTLOGO_POS_Y + RESULTLOGO_SIZE_Y, 0.0f);
	g_vertexWkResultGameclear[3].vtx = D3DXVECTOR3(RESULTLOGO_POS_X + RESULTLOGO_SIZE_X, RESULTLOGO_POS_Y + RESULTLOGO_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkResultGameclear[0].rhw =
		g_vertexWkResultGameclear[1].rhw =
		g_vertexWkResultGameclear[2].rhw =
		g_vertexWkResultGameclear[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkResultGameclear[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultGameclear[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultGameclear[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultGameclear[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkResultGameclear[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkResultGameclear[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkResultGameclear[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkResultGameclear[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_���W�̐ݒ�
	g_vertexWkResultGameover[0].vtx = D3DXVECTOR3(RESULTLOGO_POS_X, RESULTLOGO_POS_Y, 0.0f);
	g_vertexWkResultGameover[1].vtx = D3DXVECTOR3(RESULTLOGO_POS_X + RESULTLOGO_SIZE_X, RESULTLOGO_POS_Y, 0.0f);
	g_vertexWkResultGameover[2].vtx = D3DXVECTOR3(RESULTLOGO_POS_X, RESULTLOGO_POS_Y + RESULTLOGO_SIZE_Y, 0.0f);
	g_vertexWkResultGameover[3].vtx = D3DXVECTOR3(RESULTLOGO_POS_X + RESULTLOGO_SIZE_X, RESULTLOGO_POS_Y + RESULTLOGO_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkResultGameover[0].rhw =
		g_vertexWkResultGameover[1].rhw =
		g_vertexWkResultGameover[2].rhw =
		g_vertexWkResultGameover[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkResultGameover[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultGameover[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultGameover[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultGameover[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkResultGameover[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkResultGameover[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkResultGameover[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkResultGameover[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_���W�̐ݒ�
	g_vertexWkResultmenu1[0].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X, TITLE_NEWGAME_POS_Y, 0.0f);
	g_vertexWkResultmenu1[1].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X + TITLE_MENU_SIZE_X, TITLE_NEWGAME_POS_Y, 0.0f);
	g_vertexWkResultmenu1[2].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X, TITLE_NEWGAME_POS_Y + TITLE_MENU_SIZE_Y, 0.0f);
	g_vertexWkResultmenu1[3].vtx = D3DXVECTOR3(TITLE_NEWGAME_POS_X + TITLE_MENU_SIZE_X, TITLE_NEWGAME_POS_Y + TITLE_MENU_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkResultmenu1[0].rhw =
		g_vertexWkResultmenu1[1].rhw =
		g_vertexWkResultmenu1[2].rhw =
		g_vertexWkResultmenu1[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkResultmenu1[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultmenu1[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultmenu1[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultmenu1[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkResultmenu1[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkResultmenu1[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkResultmenu1[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkResultmenu1[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_���W�̐ݒ�
	g_vertexWkResultmenu2[0].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X, TITLE_EXIT_POS_Y, 0.0f);
	g_vertexWkResultmenu2[1].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X + TITLE_MENU_SIZE_X, TITLE_EXIT_POS_Y, 0.0f);
	g_vertexWkResultmenu2[2].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X, TITLE_EXIT_POS_Y + TITLE_MENU_SIZE_Y, 0.0f);
	g_vertexWkResultmenu2[3].vtx = D3DXVECTOR3(TITLE_EXIT_POS_X + TITLE_MENU_SIZE_X, TITLE_EXIT_POS_Y + TITLE_MENU_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkResultmenu2[0].rhw =
		g_vertexWkResultmenu2[1].rhw =
		g_vertexWkResultmenu2[2].rhw =
		g_vertexWkResultmenu2[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkResultmenu2[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultmenu2[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultmenu2[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultmenu2[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkResultmenu2[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkResultmenu2[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkResultmenu2[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkResultmenu2[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexResultScore(void)
{
	SCORE *score = GetScore();		// �X�R�A�̃|�C���^�[��������

	int i;
	float habaX = RESULTSCORE_SIZE_W;	// �����̉���

	// ��������������
	for (i = 0; i < SCORE_DIGIT; i++)
	{
		// ���_���W�̐ݒ�
		g_vertexWkResultScore[i][0].vtx.x = -habaX * i + score->posScore.x;
		g_vertexWkResultScore[i][0].vtx.y = score->posScore.y;
		g_vertexWkResultScore[i][0].vtx.z = 0.0f;
		g_vertexWkResultScore[i][1].vtx.x = -habaX * i + score->posScore.x + RESULTSCORE_SIZE_W;
		g_vertexWkResultScore[i][1].vtx.y = score->posScore.y;
		g_vertexWkResultScore[i][1].vtx.z = 0.0f;
		g_vertexWkResultScore[i][2].vtx.x = -habaX * i + score->posScore.x;
		g_vertexWkResultScore[i][2].vtx.y = score->posScore.y + RESULTSCORE_SIZE_H;
		g_vertexWkResultScore[i][2].vtx.z = 0.0f;
		g_vertexWkResultScore[i][3].vtx.x = -habaX * i + score->posScore.x + RESULTSCORE_SIZE_W;
		g_vertexWkResultScore[i][3].vtx.y = score->posScore.y + RESULTSCORE_SIZE_H;
		g_vertexWkResultScore[i][3].vtx.z = 0.0f;

		// rhw�̐ݒ�
		g_vertexWkResultScore[i][0].rhw =
			g_vertexWkResultScore[i][1].rhw =
			g_vertexWkResultScore[i][2].rhw =
			g_vertexWkResultScore[i][3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		g_vertexWkResultScore[i][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkResultScore[i][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkResultScore[i][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkResultScore[i][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		g_vertexWkResultScore[i][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		g_vertexWkResultScore[i][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		g_vertexWkResultScore[i][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		g_vertexWkResultScore[i][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetTextureResultScore(void)
{
	SCORE *score = GetScore();		// �X�R�A�̃|�C���^�[��������

	int i;
	int number = score->score;

	for (i = 0; i < SCORE_DIGIT; i++)
	{
		// �e�N�X�`�����W�̐ݒ�
		float x = (float)(number % 10);
		g_vertexWkResultScore[i][0].tex = D3DXVECTOR2(0.1f * x, 0.0f);
		g_vertexWkResultScore[i][1].tex = D3DXVECTOR2(0.1f * (x + 1), 0.0f);
		g_vertexWkResultScore[i][2].tex = D3DXVECTOR2(0.1f * x, 1.0f);
		g_vertexWkResultScore[i][3].tex = D3DXVECTOR2(0.1f * (x + 1), 1.0f);
		number /= 10;
	}
}

//=============================================================================
// ���j���[�F�̐ݒ�
//=============================================================================
void SetResultMenuDiffuse(int menu)
{
	switch (menu)
	{
	case 0:
		g_vertexWkResultmenu1[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkResultmenu1[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkResultmenu1[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkResultmenu1[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		g_vertexWkResultmenu2[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkResultmenu2[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkResultmenu2[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkResultmenu2[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		break;
	case 1:
		g_vertexWkResultmenu1[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkResultmenu1[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkResultmenu1[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_vertexWkResultmenu1[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);

		g_vertexWkResultmenu2[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkResultmenu2[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkResultmenu2[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkResultmenu2[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		break;
	}
}

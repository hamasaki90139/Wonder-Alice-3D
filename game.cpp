//=============================================================================
//
// �Q�[����ʏ��� [game.cpp]
// Author : �_�� ���t
//
//=============================================================================
#include "game.h"
#include "light.h"
#include "camera.h"
#include "meshfield.h"
#include "meshwall.h"
#include "player.h"
#include "enemy.h"
#include "shadow.h"
#include "meshsky.h"
#include "life.h"
#include "timer.h"
#include "score.h"
#include "item.h"
#include "sound.h"
#include "collision.h"
#include "particle.h"
#include "effect.h"
#include "pose.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//=============================================================================
// ����������
//=============================================================================
HRESULT InitGame(void)
{
	// ���C�g�̏�����
	InitLight();

	// �J�����̏�����
	InitCamera();

	// �t�B�[���h�̏�����
	// ��̏�����
	InitMeshSky(D3DXVECTOR3(0.0f, -100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16, 8, 8000.0f, true);

	// �n�ʂ̏�����
	InitMeshField();

	// �ǂ̏�����
	InitMeshWall();

	// �e�̏�����
	InitShadow();

	// �v���C���[�̏�����
	InitPlayer(0);

	// �G�l�~�[�̏�����
	InitEnemy(0);

	// ���C�t�̏�����
	InitLife();

	// �^�C�}�[�̏�����
	InitTimer();
	ResetTimer(TIMER_MAX);

	// �X�R�A�̏�����
	InitScore(0);

	// �p�[�e�B�N���̏�����
	InitParticle();

	// �G�t�F�N�g�̏�����
	InitEffect();

	// �A�C�e���̏�����
	InitItem(0);

	InitPose(0);

	// BGM�Đ�
	PlaySound(SOUND_LABEL_GAME);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGame(void)
{
	// �J�����̏I������
	UninitCamera();

	// ��̏I������
	UninitMeshSky();

	// �n�ʂ̏I������
	UninitMeshField();

	// �ǂ̏I������
	UninitMeshWall();

	// �e�̏I������
	UninitShadow();

	// �p�[�e�B�N���̏I������
	UninitParticle();

	// �G�t�F�N�g�̏I������
	UninitEffect();

	// �G�l�~�[�̏I������
	UninitEnemy();

	// �v���C���[�̏I������
	UninitPlayer();

	// ���C�t�̏I������
	UninitLife();

	// �^�C�}�[�̏I������
	UninitTimer();

	// �X�R�A�̏I������
	UninitScore();

	// �A�C�e���̏I������
	UninitItem();

	// �|�[�Y�̏I������
	UninitPose();

	// BGM��~
	StopSound(SOUND_LABEL_GAME);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame(void)
{
	PLAYER *player = GetPlayer();

	//�Q�[����ʒ�
	if (GetPose() == false)
	{
		if (player->player_hitstop == false)
		{//�ʏ펞
			// �J�����X�V
			UpdateCamera();

			// ��̍X�V����
			UpdateMeshSky();

			// �n�ʏ����̍X�V
			UpdateMeshField();

			// �Ǐ����̍X�V
			UpdateMeshWall();

			// �e�����̍X�V
			UpdateShadow();

			// �p�[�e�B�N�������̍X�V
			UpdateParticle();

			// �v���C���[�����̍X�V
			UpdatePlayer();

			// �G�l�~�[�����̍X�V
			UpdateEnemy();

			// �G�t�F�N�g�����̍X�V
			UpdateEffect();

			// ���C�t�����̍X�V
			UpdateLife();

			// �A�C�e�������̍X�V
			UpdateItem();

			//�Փ˔���̍X�V
			UpdateBattelCollision();

			//���C�g�����̍X�V
			UpdateLight();
		}
		else if (player->player_hitstop_cnt > PLAYER_HITSTOP)
		{	//�q�b�g�X�g�b�v�I��
			player->player_hitstop = false;
			player->player_hitstop_cnt = 0;
		}
		else
		{	//�q�b�g�X�g�b�v��
			player->player_hitstop_cnt++;
		}

		// �^�C�}�[�����̍X�V
		UpdateTimer();

		// �X�R�A�����̍X�V
		UpdateScore();
	}

	// �|�[�Y��ʂ̍X�V����
	UpdatePose();

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �J�����̐ݒ�
	SetCamera();

	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF
	// ��̕`�揈��
	DrawMeshSky();

	// �Ǐ����̕`��
	DrawMeshWall();

	// �n�ʏ����̕`��
	DrawMeshField();

	// �e�����̕`��
	DrawShadow();
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON

	// �G�l�~�[�����̕`��
	DrawEnemy();

	// �v���C���[�����̕`��
	DrawPlayer();

	// �A�C�e�������̕`��
	DrawItem();

	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF
	// �p�[�e�B�N�������̕`��
	DrawParticle();

	// �G�t�F�N�g�����̍X�V
	DrawEffect();

	// ���C�t�����̕`��
	DrawLife();

	// �^�C�}�[�����̕`��
	DrawTimer();

	// �X�R�A�����̕`��
	DrawScore();

	if (GetPose() == true)
	{
		// �|�[�Y�����̕`��
		DrawPose();
	}
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON

}


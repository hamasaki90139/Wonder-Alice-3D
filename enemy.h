//=============================================================================
//
// �G�l�~�[���f������ [enemy.h]
// Author : �_�� ���t
//
//=============================================================================
#pragma once

#include "particle.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_ENEMY				"data/MODEL/trump_body.x"	// �ǂݍ��ރ��f����
#define	MODEL_ENEMY_HEAD		"data/MODEL/trump_head.x"	// �ǂݍ��ރ��f����
#define	MODEL_ENEMY_ARM_L		"data/MODEL/trump_arm_L.x"	// �ǂݍ��ރ��f����
#define	MODEL_ENEMY_ARM_R		"data/MODEL/trump_arm_R.x"	// �ǂݍ��ރ��f����
#define	MODEL_ENEMY_LEG_L		"data/MODEL/trump_leg_L.x"	// �ǂݍ��ރ��f����
#define	MODEL_ENEMY_LEG_R		"data/MODEL/trump_leg_R.x"	// �ǂݍ��ރ��f����

#define	VALUE_MOVE				(6.0f);						// �ړ���
#define	VALUE_ROTATE			(D3DX_PI * 0.02f);			// ��]��
#define MAX_ENEMY				(200)						// �G�l�~�[�̐�
#define ENEMY_LEN				(20)						// �G�l�~�[�J�v�Z���̒���
#define ENEMY_R					(40)						// �G�l�~�[�̔��a
#define ENEMY_GROUP				(5)							// �G�l�~�[�����̐l��
#define ENEMY_SCOUTING			(1000)						// �G�l�~�[�̍ő�\����
#define ENEMY_MOVEPATTERN		(5)							// �G�l�~�[�����̍s���p�^�[��

//�G�l�~�[���`��ԃe�[�u���p�̍\����
typedef struct 
{
	D3DXVECTOR3					pos;						// ���f���̈ʒu
	D3DXVECTOR3					rot;						// ���f���̌���(��])
	D3DXVECTOR3					scl;						// ���f���̑傫��(�X�P�[��)
	float						dt;							// 1�t���[���œ�������
}ENEMYTBL;

struct ENEMY
{
	D3DXVECTOR3			spos;								// ���f���̊�ʒu
	D3DXVECTOR3			srot;								// ���f���̊����(��])
	D3DXVECTOR3			sscl;								// ���f���̊�傫��(�X�P�[��)
	D3DXVECTOR3			pos;								// ���f���̈ʒu
	D3DXVECTOR3			rot;								// ���f���̌���(��])
	D3DXVECTOR3			scl;								// ���f���̑傫��(�X�P�[��)
	D3DXVECTOR3			vec;								// �e�[�u���Ԃ̃x�N�g��
	D3DXVECTOR3			rey;								// ����
	D3DXVECTOR3			move;								// �ړ���
	D3DXVECTOR3			oldpos;								// ���f���ʒu�̈ʒu�ۑ�
	D3DXVECTOR3			hit_oldpos;							// ���f�������������̈ʒu�ۑ�

	ENEMYTBL			*movepattern;
	D3DXMATRIX			mtxWorld;							// ���[���h�}�g���b�N�X
	D3DXMATRIX          mtxScl;								// �g��k��
	D3DXMATRIX          mtxRot;								// ��]
	D3DXMATRIX       	mtxTranslate;						// �ړ�

	float				oldindex;							//index�f�[�^�̕ۑ�
	float				spd;								// �ړ��X�s�[�h
	float				Radius;								// �G�l�~�[�̔��a
	float				BaseAngle;							// �G�l�~�[�̊p�x
	float				move_time;							// ����������
	float				back_time;							// �Ǐ]�I������e�[�u���֖߂邽�߂̐��`��ԂɎg��
	int					pattern;							// �o�g���G�l�~�[�̍s���p�^�[��
	int					size;								// �����e�[�u���T�C�Y
	int					shadowIdx[ENEMY_GROUP];				// �e�̃C���f�b�N�X�ԍ�
	int					following;
	bool				timestop;							//���`��Ԃ��~�߂邩�ۂ�
	bool				use;								//�g�p�������g�p��

	float				walkmove_time;						//�A�j���[�V�����i�s����

	float				attack_dt;							//1�t���[��������i�ގ���(�U���p�j
	float				attackmove_time;					//�A�j���[�V�����i�s����
	int					attackcnt;							//�U�����n�߂�܂ł̃J�E���g

	float				hit_dt;								//1�t���[��������i�ގ���(�U���󂯂����p�j
	float				hitmove_time;						//�A�j���[�V�����i�s����

	ENEMY				*parent;							//�K�w�A�j���[�V�����p�e�q�ݒ�
	STATUS				status;								//���݂̃X�e�[�^�X

	D3DXMATERIAL		*pD3DXMat;							//���[���h�}�g���b�N�X�p
	D3DMATERIAL9		matDef;								//���[���h�}�g���b�N�X�p

	float				hit;								// �U���󂯂��Ƃ��̔��ł�����
	int					hit_cnt;							// ���t���[�����ł邩
	int					hit_cnt_max;						// ���t���[���Ŕ��ł���
	D3DXVECTOR3			ofs;								// pos�ɑ΂���I�t�Z�b�g���W

	PARTICLE			particle[MAX_PARTICLE];				// �G�l�~�[�|�������̃p�[�e�B�N��

	int					formation;							// �G�l�~�[�����̃t�H�[���[�V����
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(int type);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
ENEMY *GetEnemy();
void SetEnemy(int enemy_spawn, D3DXVECTOR3 upperleft, D3DXVECTOR3 bottomright);
void RefreshEnemy(int refresh_enemy, D3DXVECTOR3 upperleft, D3DXVECTOR3 bottomright);

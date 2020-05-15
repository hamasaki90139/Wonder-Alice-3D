//=============================================================================
//
// ���f������ [player.h]
// Author : �_�� ���t
//
//=============================================================================
#pragma once

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_PLAYER			"data/MODEL/alice_body.x"		// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_HEAD		"data/MODEL/alice_head.x"		// �ǂݍ��ރ��f����
#define MODEL_PLAYER_ARM_L		"data/MODEL/alice_arm_L.x"		// �ǂݍ��ރ��f����
#define MODEL_PLAYER_ARM_R		"data/MODEL/alice_arm_R.x"		// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_LEG_L		"data/MODEL/alice_leg_L.x"		// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_LEG_R		"data/MODEL/alice_leg_R.x"		// �ǂݍ��ރ��f����

#define MODEL_PLAYER_WEAPON		"data/MODEL/spoon.x"			// �ǂݍ��ރ��f����

#define WEAPON_LEN						(15)

#define	VALUE_MOVE						(5.0f)					// �ړ���
#define	VALUE_ROTATE					(D3DX_PI * 0.02f)		// ��]��
#define PARTS_MAX						(2)						//�{�̂��������p�[�c��

#define	PLAYER_JUMP_Y					(15.0f)					// �W�����v�̍���
#define	PLAYER_JUMP_CNT_MAX				(20)					// 20�t���[���̃W�����v����
#define	PLAYER_JUMP_MAX					(1)						// �A���W�����v��
#define	PLAYER_ATTACK_MAX				(4)						// �A���U����

//*****************************************************************************
// �}�N����`
//*****************************************************************************
struct PLAYER
{
	D3DXVECTOR3			pos;					// ���f���̈ʒu
	D3DXVECTOR3			rot;					// ���f���̌���(��])
	D3DXVECTOR3			scl;					// ���f���̑傫��(�X�P�[��)

	D3DXVECTOR3			vec;					//	�e�[�u���Ԃ̃x�N�g��

	float				spd;					// �ړ��X�s�[�h

	LPDIRECT3DTEXTURE9	pD3DTexture;			// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH			pD3DXMesh;				// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER		pD3DXBuffMat;			// �}�e���A�����ւ̃|�C���^
	DWORD				nNumMat;				// �}�e���A�����̐�

	D3DXMATRIX			mtxWorld;				// ���[���h�}�g���b�N�X
	D3DXMATRIX          mtxScl;					// ���[���h�}�g���b�N�X
	D3DXMATRIX          mtxRot;					// ���[���h�}�g���b�N�X
	D3DXMATRIX       	mtxTranslate;			// ���[���h�}�g���b�N�X
	D3DXMATERIAL		*pD3DXMat;				// ���[���h�}�g���b�N�X
	D3DMATERIAL9		matDef;					// ���[���h�}�g���b�N�X

	int					shadowIdx;				// �e�̃C���f�b�N�X�ԍ�

	PLAYER				*parent;				//�K�w�A�j���[�V�����̐e�q����p

	bool				use;					//��������

	bool				jump;					// �W�����v�̃`�F�b�N
	float				jump_y;					// �W�����v�̍���
	int					jump_cnt;				// ���t���[���W�����v���Ă��邩
	int					jump_cnt_max;			// ���t���[���ŃW�����v�����邩
	D3DXVECTOR3			ofs;					// pos�ɑ΂���I�t�Z�b�g���W
	int					jumppoint;				// ����A���ŃW�����v���Ă��邩

	bool				AttackMotion;			//�U������
	int					attackpoint;			// ����A���ōU�����Ă��邩
	bool				protect;				//���G��Ԃ�
	int					protect_cnt;			//���G��Ԃ̃J�E���g

	bool				player_hitstop;			// �v���C���[�̃q�b�g�X�g�b�v����
	int					player_hitstop_cnt;		// �q�b�g�X�g�b�v�̃J�E���g

};

#define MAX_PLAYER						(1)		// �v���C���[�̐�
#define PLAYER_R						(30)	// �v���C���[�̔��a
#define WEAPON_R						(20)	// ����̔��a
#define PLAYER_HITSTOP					(3)		// �q�b�g�X�g�b�v�̎���

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(int type);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER *GetPlayer(void);

void GetweaponPos(D3DXVECTOR3 &pos, D3DXVECTOR3 &S, D3DXVECTOR3 &E);

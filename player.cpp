//=============================================================================
//
// ���f������ [player.cpp]
// Author : �_�� ���t
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "player.h"
#include "camera.h"
#include "shadow.h"
#include "debugproc.h"
#include "collision.h"
#include "enemy.h"
#include "sound.h"
#include "effect.h"
#include "fade.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static PLAYER				g_Player;						// �v���C���[
static PLAYER				g_head;							// �v���C���[�̓���
static PLAYER				g_arm[LR];                      // �v���C���[�̘r
static PLAYER				g_leg[LR];						// �v���C���[�̑�����

static PLAYER				g_weapon;                       // �v���C���[�̕���

float						g_move_time;					// �A�j���[�V�����i�s����
float						g_walkmove_time;				// �A�j���[�V�����i�s����
float						g_jumpmove_time;				// �A�j���[�V�����i�s����

bool						g_WalkMotion;					// �������[�V�����Đ� ON/OFF
bool						g_JumpMotion;					// �W�����v���[�V�����Đ� ON/OFF

float						roty;							// �v���C���[��Y���ɑ΂��Ă̌���

PLAYER_SET set_tbl[] = {    //pos,rot,scl �e�p�[�c�̏����l
	{	D3DXVECTOR3(0.0f, 0.0f, 0.0f),		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.9f, 0.9f, 0.9f)},//��			
	{	D3DXVECTOR3(0.0f, 59.0f, -7.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.9f, 0.9f, 0.9f)},//����		
	{	D3DXVECTOR3(-9.0f, 52.0f, 0.5f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.9f, 0.9f, 0.9f)},//�E�r
	{	D3DXVECTOR3(9.0f, 52.0f, 0.5f),		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.9f, 0.9f, 0.9f)},//���r
	{	D3DXVECTOR3(-0.5f, 33.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.9f, 0.9f, 0.9f)},//�E������
	{	D3DXVECTOR3(0.5f, 33.0f, 0.0f),		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.9f, 0.9f, 0.9f)},//��������
};

//�������[�V����
D3DXVECTOR3 body_walk_tbl[] = { //��

	D3DXVECTOR3(-0.100000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.143633f, -5.50000f, 0.000000f),
	D3DXVECTOR3(-0.100000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 head_walk_tbl[] = { //����

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.001f, -0.000050f, 0.000000f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 armR_walk_tbl[] = { //�E�r

	D3DXVECTOR3(-0.374533f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.574533f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.374533f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 armL_walk_tbl[] = { //���r

	D3DXVECTOR3(-0.374533f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.574533f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.374533f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 leg1R_walk_tbl[] = { //�E������

	D3DXVECTOR3(-0.323599f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.323599f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.323599f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 leg1L_walk_tbl[] = { //��������

	D3DXVECTOR3(0.323599f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.323599f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.323599f, 0.000000f, 0.000000f),
};

// �W�����v���[�V����1
D3DXVECTOR3 body_jump_tbl1[] = { //��

	D3DXVECTOR3(0.000000f, 1.218163f, 0.000000f),
	D3DXVECTOR3(0.230900f, 1.218163f, 0.000000f),
	D3DXVECTOR3(0.230900f, 1.218163f, 0.000000f),
	D3DXVECTOR3(-0.130900f, 1.218161f, 0.000000f),
	D3DXVECTOR3(-0.100000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 head_jump_tbl1[] = { //����

	D3DXVECTOR3(-0.01f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.01f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.01f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.0f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.0f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 armR_jump_tbl1[] = { //�E�r

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(1.0f, -1.0f, -0.500000f),
	D3DXVECTOR3(1.0f, -2.0f, -0.500000f),
	D3DXVECTOR3(1.0f, -1.0f, -0.5f),
	D3DXVECTOR3(-0.374533f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 armL_jump_tbl1[] = { //���r

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.567232f, 0.654498f, 0.000000f),
	D3DXVECTOR3(0.800000f, 0.854498f, 0.000000f),
	D3DXVECTOR3(0.567232f, 0.654498f, 0.000000f),
	D3DXVECTOR3(-0.374533f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 legR_jump_tbl1[] = { //�E������

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.518166f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.118166f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.518166f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.218166f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 legL_jump_tbl1[] = { //��������

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.518166f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.118166f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.518166f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.018166f, 0.000000f, 0.000000f),
};

// �W�����v���[�V����2
D3DXVECTOR3 body_jump_tbl2[] = { //��

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-D3DX_PI, 0.000000f, 0.000000f),
	D3DXVECTOR3(-D3DX_PI * 2, 0.000000f, 0.000000f),
	D3DXVECTOR3(-D3DX_PI * 2, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 head_jump_tbl2[] = { //����

	D3DXVECTOR3(-0.01f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.01f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.01f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.0f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.0f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 armR_jump_tbl2[] = { //�E�r

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(1.0f, -1.0f, -0.500000f),
	D3DXVECTOR3(1.0f, -2.0f, -0.500000f),
	D3DXVECTOR3(1.0f, -1.0f, -0.5f),
	D3DXVECTOR3(-0.374533f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 armL_jump_tbl2[] = { //���r

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.567232f, 0.654498f, 0.000000f),
	D3DXVECTOR3(0.800000f, 0.854498f, 0.000000f),
	D3DXVECTOR3(0.567232f, 0.654498f, 0.000000f),
	D3DXVECTOR3(-0.374533f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 legR_jump_tbl2[] = { //�E������

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.818166f, 0.000000f, 0.200000f),
	D3DXVECTOR3(0.818166f, 0.000000f, 0.200000f),
	D3DXVECTOR3(0.818166f, 0.000000f, 0.100000f),
	D3DXVECTOR3(0.218166f, 0.000000f, 0.050000f),
};

D3DXVECTOR3 legL_jump_tbl2[] = { //��������

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.818166f, 0.000000f, -0.200000f),
	D3DXVECTOR3(0.818166f, 0.000000f, -0.200000f),
	D3DXVECTOR3(0.818166f, 0.000000f, -0.100000f),
	D3DXVECTOR3(0.018166f, 0.000000f, 0.050000f),
};

//�U�����[�V����1
D3DXVECTOR3 body_attack_tbl1[] = { //��

	D3DXVECTOR3(0.000000f, -0.372664f, 0.000000f),
	D3DXVECTOR3(-0.130900f, -0.718161f, -0.118166f),
	D3DXVECTOR3(-0.130900f, -0.718163f, -0.118166f),
	D3DXVECTOR3(-0.130900f, -0.718161f, -0.118166f),
	D3DXVECTOR3(0.000000f, -0.372664f, 0.000000f),
};

D3DXVECTOR3 head_attack_tbl1[] = { //����

	D3DXVECTOR3(-0.01f, -0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, -0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, -0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, -0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, -0.3f, 0.000000f),
};

D3DXVECTOR3 armR_attack_tbl1[] = { //�E�r

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(1.0f, 1.0f, -0.5f),
	D3DXVECTOR3(1.0f, 1.5f, -0.500000f),
	D3DXVECTOR3(1.0f, -2.0f, -0.500000f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 armL_attack_tbl1[] = { //���r

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.000000f, 0.654498f, 0.000000f),
	D3DXVECTOR3(0.000000f, 0.654498f, 0.000000f),
	D3DXVECTOR3(-0.567232f, 0.654498f, 0.000000f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 legR_attack_tbl1[] = { //�E������

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.218166f, 0.000000f, -0.261799f),
	D3DXVECTOR3(-0.218166f, 0.000000f, -0.261799f),
	D3DXVECTOR3(-0.218166f, 0.000000f, -0.261799f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 legL_attack_tbl1[] = { //��������

	D3DXVECTOR3(0.000000f, -0.785398f, -0.000000f),
	D3DXVECTOR3(0.392699f, -0.785398f, -0.300000f),
	D3DXVECTOR3(0.392699f, -0.785398f, -0.300000f),
	D3DXVECTOR3(0.392699f, -0.785398f, -0.300000f),
	D3DXVECTOR3(0.000000f, -0.785398f, -0.000000f),
};

//�U�����[�V����2
D3DXVECTOR3 body_attack_tbl2[] = { //��

	D3DXVECTOR3(0.000000f, 0.872664f, 0.000000f),
	D3DXVECTOR3(-0.130900f, 0.872662f, -0.118166f),
	D3DXVECTOR3(-0.130900f, 1.218163f, -0.118166f),
	D3DXVECTOR3(-0.130900f, 1.218161f, -0.118166f),
	D3DXVECTOR3(0.000000f, 0.872664f, 0.000000f),
};

D3DXVECTOR3 head_attack_tbl2[] = { //����

	D3DXVECTOR3(-0.01f, 0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, 0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, 0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, 0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, 0.3f, 0.000000f),
};

D3DXVECTOR3 armR_attack_tbl2[] = { //�E�r

	D3DXVECTOR3(1.0f, -2.0f, -0.500000f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(1.0f, 1.0f, -0.500000f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 armL_attack_tbl2[] = { //���r

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.000000f, 0.654498f, 0.000000f),
	D3DXVECTOR3(0.000000f, 0.654498f, 0.000000f),
	D3DXVECTOR3(-0.567232f, 0.654498f, 0.000000f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 legR_attack_tbl2[] = { //�E������

	D3DXVECTOR3(0.000000f, 0.785398f, 0.000000f),
	D3DXVECTOR3(-0.392699f, 0.785398f, 0.000000f),
	D3DXVECTOR3(-0.392699f, 0.785398f, -0.261799f),
	D3DXVECTOR3(-0.392699f, 0.785398f, -0.261799f),
	D3DXVECTOR3(0.000000f, 0.785398f, 0.000000f),
};

D3DXVECTOR3 legL_attack_tbl2[] = { //��������

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.218166f, 0.000000f, -0.261799f),
	D3DXVECTOR3(-0.218166f, 0.000000f, -0.261799f),
	D3DXVECTOR3(-0.218166f, 0.000000f, -0.261799f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
};

//�U�����[�V����3
D3DXVECTOR3 body_attack_tbl3[] = { //��

	D3DXVECTOR3(0.000000f, 0.872664f, 0.000000f),
	D3DXVECTOR3(-0.130900f, 0.872662f, -0.118166f),
	D3DXVECTOR3(-0.130900f, 1.218163f, -0.118166f),
	D3DXVECTOR3(-0.130900f, 1.218161f, -0.118166f),
	D3DXVECTOR3(0.000000f, 0.872664f, 0.000000f),
};

D3DXVECTOR3 head_attack_tbl3[] = { //����

	D3DXVECTOR3(-0.01f, -0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, -0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, -0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, -0.3f, 0.000000f),
	D3DXVECTOR3(0.0f, 0.0f, 0.000000f),
};

D3DXVECTOR3 armR_attack_tbl3[] = { //�E�r

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(1.0f, -1.0f, -0.500000f),
	D3DXVECTOR3(1.0f, -2.0f, -0.500000f),
	D3DXVECTOR3(1.0f, -1.0f, -0.5f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 armL_attack_tbl3[] = { //���r

	D3DXVECTOR3(1.5f, 0.8f, -0.600000f),
	D3DXVECTOR3(1.50000f,0.8000f, -0.600000f),
	D3DXVECTOR3(1.5f, 0.3f, -0.600000f),
	D3DXVECTOR3(1.5f, 0.3f, -0.600000f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 legR_attack_tbl3[] = { //�E������

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.218166f, 0.000000f, -0.261799f),
	D3DXVECTOR3(-0.218166f, 0.000000f, -0.261799f),
	D3DXVECTOR3(-0.218166f, 0.000000f, -0.261799f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 legL_attack_tbl3[] = { //��������

	D3DXVECTOR3(0.000000f, -0.785398f, 0.000000f),
	D3DXVECTOR3(0.392699f, -0.785398f, 0.000000f),
	D3DXVECTOR3(0.392699f, -0.785398f, -0.261799f),
	D3DXVECTOR3(0.392699f, -0.785398f, -0.261799f),
	D3DXVECTOR3(0.000000f, -0.785398f, 0.000000f),
};

//�U�����[�V����4
D3DXVECTOR3 body_attack_tbl4[] = { //��

	D3DXVECTOR3(0.000000f, 0.872664f, 0.000000f),
	D3DXVECTOR3(-0.130900f, 0.872662f, -0.118166f),
	D3DXVECTOR3(-0.130900f, 1.218163f, -0.118166f),
	D3DXVECTOR3(-0.130900f, 1.218161f, -0.118166f),
	D3DXVECTOR3(0.000000f, 0.872664f, 0.000000f),
};

D3DXVECTOR3 head_attack_tbl4[] = { //����

	D3DXVECTOR3(-0.01f, 0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, 0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, 0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, 0.3f, 0.000000f),
	D3DXVECTOR3(-0.01f, 0.3f, 0.000000f),
};

D3DXVECTOR3 armR_attack_tbl4[] = { //�E�r

	D3DXVECTOR3(1.0f, -2.0f, -0.500000f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(1.0f, 1.0f, -0.500000f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 armL_attack_tbl4[] = { //���r

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.000000f, 0.654498f, 0.000000f),
	D3DXVECTOR3(0.000000f, 0.654498f, 0.000000f),
	D3DXVECTOR3(-0.567232f, 0.654498f, 0.000000f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 legR_attack_tbl4[] = { //�E������

	D3DXVECTOR3(0.000000f, 0.785398f, 0.000000f),
	D3DXVECTOR3(-0.392699f, 0.785398f, 0.000000f),
	D3DXVECTOR3(-0.392699f, 0.785398f, -0.261799f),
	D3DXVECTOR3(-0.392699f, 0.785398f, -0.261799f),
	D3DXVECTOR3(0.000000f, 0.785398f, 0.000000f),
};

D3DXVECTOR3 legL_attack_tbl4[] = { //��������

	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.218166f, 0.000000f, -0.261799f),
	D3DXVECTOR3(-0.218166f, 0.000000f, -0.261799f),
	D3DXVECTOR3(-0.218166f, 0.000000f, -0.261799f),
	D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	roty = 0.0f;
	g_move_time = 0.0f;
	g_walkmove_time = 0.0f;
	g_jumpmove_time = 0.0f;
	g_WalkMotion = false;
	g_JumpMotion = false;

	//��
	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	g_Player.pos = set_tbl[BODY].pos;
	g_Player.rot = set_tbl[BODY].rot;
	g_Player.scl = set_tbl[BODY].scl;

	g_Player.spd = 0.0f;		// �ړ��X�s�[�h�N���A

	g_Player.AttackMotion = false;
	g_Player.use = true;					//����
	g_Player.attackpoint = 0;
	g_Player.jumppoint = 0;
	g_Player.protect = false;
	g_Player.protect_cnt = 0;

	g_Player.player_hitstop = false;
	g_Player.player_hitstop_cnt = 0;

	// �W�����v�̏�����
	g_Player.jump_y = PLAYER_JUMP_Y;							// �W�����v���̍���
	g_Player.jump_cnt = 0;										// �W�����v�J�E���g������
	g_Player.jump_cnt_max = PLAYER_JUMP_CNT_MAX;				// �W�����v�J�E���g�}�b�N�X������
	g_Player.ofs = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �I�t�Z�b�g���W��������
	g_Player.jump = false;

	if (type == 0)
	{
		g_Player.pD3DTexture = NULL;
		g_Player.pD3DXMesh = NULL;
		g_Player.pD3DXBuffMat = NULL;
		g_Player.nNumMat = 0;
		g_Player.parent = NULL;

		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(MODEL_PLAYER,			// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
			D3DXMESH_SYSTEMMEM,								// ���b�V���̍쐬�I�v�V�������w��
			pDevice,										// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
			NULL,											// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			&g_Player.pD3DXBuffMat,							// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			NULL,											// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
			&g_Player.nNumMat,								// D3DXMATERIAL�\���̂̐�
			&g_Player.pD3DXMesh)))							// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
		{
			return E_FAIL;
		}
	}

#if 0
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_FILENAME,		// �t�@�C���̖��O
		&g_Player.pD3DTexture);	// �ǂݍ��ރ������[
#endif

	// �e�̏�����
	D3DXVECTOR3 pos = g_Player.pos;
	pos.y = 0.0f;
	g_Player.shadowIdx = CreateShadow(pos, g_Player.scl);

	//����
	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	g_head.pos = set_tbl[HEAD].pos;
	g_head.rot = set_tbl[HEAD].rot;
	g_head.scl = set_tbl[HEAD].scl;

	g_head.spd = 0.0f;		// �ړ��X�s�[�h�N���A


	if (type == 0)
	{
		g_head.pD3DTexture = NULL;
		g_head.pD3DXMesh = NULL;
		g_head.pD3DXBuffMat = NULL;
		g_head.nNumMat = 0;
		g_head.parent = &g_Player;

		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(MODEL_PLAYER_HEAD,			// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
			D3DXMESH_SYSTEMMEM,									// ���b�V���̍쐬�I�v�V�������w��
			pDevice,											// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
			NULL,												// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			&g_head.pD3DXBuffMat,								// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			NULL,												// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
			&g_head.nNumMat,									// D3DXMATERIAL�\���̂̐�
			&g_head.pD3DXMesh)))								// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
		{
			return E_FAIL;
		}
	}

#if 0
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_FILENAME,		// �t�@�C���̖��O
		&pD3DTexture_head);	// �ǂݍ��ރ������[
#endif

		// �ʒu�E��]�E�X�P�[���̏����ݒ�

	for (int i = 0; i < LR; i++)
	{				//�r
				// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_arm[i].pos = set_tbl[ARM_R + i].pos;
		g_arm[i].rot = set_tbl[ARM_R + i].rot;
		g_arm[i].scl = set_tbl[ARM_R + i].scl;

		g_arm[i].spd = 0.0f;		// �ړ��X�s�[�h�N���A

		if (type == 0)
		{
			g_arm[i].pD3DTexture = NULL;
			g_arm[i].pD3DXMesh = NULL;
			g_arm[i].pD3DXBuffMat = NULL;
			g_arm[i].nNumMat = 0;
			g_arm[i].parent = &g_Player;

			if (i == R)
			{
				// X�t�@�C���̓ǂݍ���
				if (FAILED(D3DXLoadMeshFromX(MODEL_PLAYER_ARM_R,			// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
					D3DXMESH_SYSTEMMEM,										// ���b�V���̍쐬�I�v�V�������w��
					pDevice,												// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
					NULL,													// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
					&g_arm[i].pD3DXBuffMat,									// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
					NULL,													// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
					&g_arm[i].nNumMat,										// D3DXMATERIAL�\���̂̐�
					&g_arm[i].pD3DXMesh)))									// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
				{
					return E_FAIL;
				}
			}
			if (i == L)
			{
				// X�t�@�C���̓ǂݍ���
				if (FAILED(D3DXLoadMeshFromX(MODEL_PLAYER_ARM_L,			// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
					D3DXMESH_SYSTEMMEM,										// ���b�V���̍쐬�I�v�V�������w��
					pDevice,												// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
					NULL,													// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
					&g_arm[i].pD3DXBuffMat,									// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
					NULL,													// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
					&g_arm[i].nNumMat,										// D3DXMATERIAL�\���̂̐�
					&g_arm[i].pD3DXMesh)))									// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
				{
					return E_FAIL;
				}
			}
		}

#if 0
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_FILENAME,		// �t�@�C���̖��O
			&g_arm[i].pD3DTexture);	// �ǂݍ��ރ������[
#endif
		//������
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_leg[i].pos = set_tbl[LEG_R + i].pos;
		g_leg[i].rot = set_tbl[LEG_R + i].rot;
		g_leg[i].scl = set_tbl[LEG_R + i].scl;

		g_leg[i].spd = 0.0f;		// �ړ��X�s�[�h�N���A

		if (type == 0)
		{
			g_leg[i].pD3DTexture = NULL;
			g_leg[i].pD3DXMesh = NULL;
			g_leg[i].pD3DXBuffMat = NULL;
			g_leg[i].nNumMat = 0;
			g_leg[i].parent = &g_Player;

			if (i == R)
			{
				// X�t�@�C���̓ǂݍ���
				if (FAILED(D3DXLoadMeshFromX(MODEL_PLAYER_LEG_R,			// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
					D3DXMESH_SYSTEMMEM,										// ���b�V���̍쐬�I�v�V�������w��
					pDevice,												// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
					NULL,													// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
					&g_leg[i].pD3DXBuffMat,									// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
					NULL,													// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
					&g_leg[i].nNumMat,										// D3DXMATERIAL�\���̂̐�
					&g_leg[i].pD3DXMesh)))									// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
				{
					return E_FAIL;
				}
			}
			if (i == L)
			{
				// X�t�@�C���̓ǂݍ���
				if (FAILED(D3DXLoadMeshFromX(MODEL_PLAYER_LEG_L,			// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
					D3DXMESH_SYSTEMMEM,										// ���b�V���̍쐬�I�v�V�������w��
					pDevice,												// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
					NULL,													// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
					&g_leg[i].pD3DXBuffMat,									// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
					NULL,													// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
					&g_leg[i].nNumMat,										// D3DXMATERIAL�\���̂̐�
					&g_leg[i].pD3DXMesh)))									// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
				{
					return E_FAIL;
				}
			}
		}

#if 0
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_FILENAME,		// �t�@�C���̖��O
			&g_leg1[i].pD3DTexture);	// �ǂݍ��ރ������[
#endif
	}


	// ����
	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	g_weapon.pos = D3DXVECTOR3(-11.5f, -13.5f, 25.5f);
	g_weapon.rot = D3DXVECTOR3(-1.6f, 0.0f, 0.0f);
	g_weapon.scl = D3DXVECTOR3(0.7f, 0.7f, 0.7f);

	g_weapon.spd = 0.0f;		// �ړ��X�s�[�h�N���A

	if (type == 0)
	{
		g_weapon.pD3DTexture = NULL;
		g_weapon.pD3DXMesh = NULL;
		g_weapon.pD3DXBuffMat = NULL;
		g_weapon.nNumMat = 0;
		g_weapon.parent = &g_arm[R];

		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(MODEL_PLAYER_WEAPON,			// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
			D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
			pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
			NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			&g_weapon.pD3DXBuffMat,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
			&g_weapon.nNumMat,		// D3DXMATERIAL�\���̂̐�
			&g_weapon.pD3DXMesh)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
		{
			return E_FAIL;
		}
	}

#if 0
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_FILENAME,		// �t�@�C���̖��O
		&g_weapon.pD3DTexture);	// �ǂݍ��ރ������[
#endif

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	if (g_Player.pD3DTexture != NULL)
	{// �e�N�X�`���̊J��
		g_Player.pD3DTexture->Release();
		g_Player.pD3DTexture = NULL;
	}
	if (g_Player.pD3DXMesh != NULL)
	{// ���b�V���̊J��
		g_Player.pD3DXMesh->Release();
		g_Player.pD3DXMesh = NULL;
	}
	if (g_Player.pD3DXBuffMat != NULL)
	{// �}�e���A���̊J��
		g_Player.pD3DXBuffMat->Release();
		g_Player.pD3DXBuffMat = NULL;
	}

	if (g_head.pD3DTexture != NULL)
	{// �e�N�X�`���̊J��
		g_head.pD3DTexture->Release();
		g_head.pD3DTexture = NULL;
	}
	if (g_head.pD3DXMesh != NULL)
	{// ���b�V���̊J��
		g_head.pD3DXMesh->Release();
		g_head.pD3DXMesh = NULL;
	}
	if (g_head.pD3DXBuffMat != NULL)
	{// �}�e���A���̊J��
		g_head.pD3DXBuffMat->Release();
		g_head.pD3DXBuffMat = NULL;
	}

	for (int i = 0; i < LR; i++)
	{
		if (g_arm[i].pD3DTexture != NULL)
		{// �e�N�X�`���̊J��
			g_arm[i].pD3DTexture->Release();
			g_arm[i].pD3DTexture = NULL;
		}
		if (g_arm[i].pD3DXMesh != NULL)
		{// ���b�V���̊J��
			g_arm[i].pD3DXMesh->Release();
			g_arm[i].pD3DXMesh = NULL;
		}
		if (g_arm[i].pD3DXBuffMat != NULL)
		{// �}�e���A���̊J��
			g_arm[i].pD3DXBuffMat->Release();
			g_arm[i].pD3DXBuffMat = NULL;
		}

		if (g_leg[i].pD3DTexture != NULL)
		{// �e�N�X�`���̊J��
			g_leg[i].pD3DTexture->Release();
			g_leg[i].pD3DTexture = NULL;
		}
		if (g_leg[i].pD3DXMesh != NULL)
		{// ���b�V���̊J��
			g_leg[i].pD3DXMesh->Release();
			g_leg[i].pD3DXMesh = NULL;
		}
		if (g_leg[i].pD3DXBuffMat != NULL)
		{// �}�e���A���̊J��
			g_leg[i].pD3DXBuffMat->Release();
			g_leg[i].pD3DXBuffMat = NULL;
		}
	}

	if (g_weapon.pD3DTexture != NULL)
	{// �e�N�X�`���̊J��
		g_weapon.pD3DTexture->Release();
		g_weapon.pD3DTexture = NULL;
	}
	if (g_weapon.pD3DXMesh != NULL)
	{// ���b�V���̊J��
		g_weapon.pD3DXMesh->Release();
		g_weapon.pD3DXMesh = NULL;
	}
	if (g_weapon.pD3DXBuffMat != NULL)
	{// �}�e���A���̊J��
		g_weapon.pD3DXBuffMat->Release();
		g_weapon.pD3DXBuffMat = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	CAMERA *cam = GetCamera();
	ENEMY *enemy = GetEnemy();

	PrintDebugProc("�U����? %d \n", g_Player.AttackMotion);
	PrintDebugProc("�U���� %d \n", g_Player.attackpoint);
	PrintDebugProc("roty	 %f \n", roty);

	float	dt = 0.08f;
	//������W�̎擾
	D3DXVECTOR3 Wpos, Ws, We;
	GetweaponPos(Wpos, Ws, We);

	if (g_Player.attackpoint < PLAYER_ATTACK_MAX)			//�A���U���񐔈ȓ����H
	{
		if ((GetKeyboardTrigger(DIK_SPACE)) || (IsButtonTriggered(0, BUTTON_A))) //�U�����[�V���� ���͊m�F
		{
			if (g_Player.AttackMotion == false)
			{
				g_Player.AttackMotion = true;				//�U������
				GetEffectNo(2);
				SetEffect(D3DXVECTOR3(Wpos.x, Wpos.y + 10.0f, Wpos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), EFFECT_SIZE_X, EFFECT_SIZE_Y, 20);

				// SE�Đ�
				PlaySound(SOUND_LABEL_SE_ATTACK2);
			}
		}
	}

	// �ړ�����
	int		dir = 0;	// �O�F�����͕ς��Ȃ�

	if ((GetKeyboardPress(DIK_A)) || (IsButtonPressed(0, BUTTON_LEFT)))		//���ړ�
	{
		dir |= 8;
		g_Player.spd = VALUE_MOVE;
		g_WalkMotion = true;

		if ((GetKeyboardPress(DIK_W)) || (IsButtonPressed(0, BUTTON_UP)))	//����ړ�
		{
			dir |= 10;
		}
		else if ((GetKeyboardPress(DIK_S)) || (IsButtonPressed(0, BUTTON_DOWN)))	//�����ړ�
		{
			dir |= 9;
		}

	}
	else if ((GetKeyboardPress(DIK_D)) || (IsButtonPressed(0, BUTTON_RIGHT)))	//�E�ړ�
	{
		dir |= 4;
		g_Player.spd = VALUE_MOVE;
		g_WalkMotion = true;

		if ((GetKeyboardPress(DIK_W)) || (IsButtonPressed(0, BUTTON_UP)))		//�E��ړ�
		{
			dir |= 6;
		}
		else if ((GetKeyboardPress(DIK_S)) || (IsButtonPressed(0, BUTTON_DOWN)))	//�E���ړ�
		{
			dir |= 5;
		}
	}
	else if ((GetKeyboardPress(DIK_W)) || (IsButtonPressed(0, BUTTON_UP)))
	{
		dir |= 2;
		g_Player.spd = VALUE_MOVE;
		g_WalkMotion = true;

	}
	else if ((GetKeyboardPress(DIK_S)) || (IsButtonPressed(0, BUTTON_DOWN)))
	{
		dir |= 1;
		g_Player.spd = VALUE_MOVE;
		g_WalkMotion = true;

	}
	else //�ړ����ĂȂ������烂�[�V��������߂�
	{
		g_WalkMotion = false;
		g_walkmove_time = 0.0f;
	}

	// ���͂��ꂽ�L�[�ɍ��킹�Č��������߂�
	switch (dir)
	{
	case 1:	// ������
		if (roty > 0.0f)
		{
			roty -= 0.1f;
			if (roty < 0.0f)
			{
				roty = 0.0f;
			}
		}
		else
		{
			roty += 0.1f;
			if (roty > 0.0f)
			{
				roty = 0.0f;
			}

		}
		break;

	case 2:	// �����
		if (roty > 0.0f)
		{
			roty += 0.1f;
			if (roty > D3DX_PI)
			{
				roty = D3DX_PI;
			}
		}
		else
		{
			roty -= 0.1f;
			if (roty < -D3DX_PI)
			{
				roty = D3DX_PI;
			}

		}
		break;

	case 4:	// �E����
		if (((roty > D3DX_PI / 2) && (roty < D3DX_PI)) || ((roty < -D3DX_PI / 2)))
		{
			roty += 0.1f;
			if (roty >= D3DX_PI)
			{
				roty = -D3DX_PI;
			}
		}
		else
		{
			roty -= 0.1f;
			if (roty < -D3DX_PI / 2)
			{
				roty = -D3DX_PI / 2;
			}
		}
		break;

	case 8:	// ������
		if ((roty > -D3DX_PI / 2) && (roty < D3DX_PI / 2))
		{
			roty += 0.1f;
			if (roty > D3DX_PI / 2)
			{
				roty = D3DX_PI / 2;
			}
		}
		else if (((roty <= -D3DX_PI / 2) && (roty > -D3DX_PI)) || ((roty > D3DX_PI / 2)))
		{
			roty -= 0.1f;
			if (roty <= -D3DX_PI)
			{
				roty = D3DX_PI;
			}
		}
		break;

	case 5:	// �E������
		roty = -D3DX_PI / 4;
		break;

	case 6:	// �E�����
		roty = -D3DX_PI / 4 * 3;
		break;

	case 9:	// ��������
		roty = D3DX_PI / 4;
		break;

	case 10: // �������
		roty = D3DX_PI / 4 * 3;
		break;
	}

	//�A�j���[�V����(����)
	if (g_WalkMotion == true) //TRUE�Ȃ���s
	{
		dt = 0.05f;

		g_walkmove_time += dt;		// �A�j���[�V�����̍��v���Ԃɑ���

		int index = (int)g_walkmove_time;
		float	time = g_walkmove_time - index;
		int		size = sizeof(body_walk_tbl) / sizeof(D3DXVECTOR3);; //�e�[�u���S�̂̃o�C�g�����e�[�u����̃o�C�g���Ŋ���(�e�[�u���������߂�)

		if (index > (size - 2))	// �S�[�����I�[�o�[���Ă�����A�S�[���֖߂�
		{
			g_walkmove_time = 0.0f;
			index = 0;
		}

		// �A�j���[�V���������镔��	�@X = StartX + (EndX - StartX) * ���̎���
		g_Player.vec = body_walk_tbl[index + 1] - body_walk_tbl[index];
		g_head.vec = head_walk_tbl[index + 1] - head_walk_tbl[index];

		g_Player.rot = body_walk_tbl[index] + g_Player.vec * time;
		g_head.rot = head_walk_tbl[index] + g_head.vec * time;

		for (int i = 0; i < LR; i++)
		{
			if (i == R)
			{
				g_arm[i].vec = armR_walk_tbl[index + 1] - armR_walk_tbl[index];
				g_leg[i].vec = leg1R_walk_tbl[index + 1] - leg1R_walk_tbl[index];

				g_arm[i].rot = armR_walk_tbl[index] + g_arm[i].vec * time;
				g_leg[i].rot = leg1R_walk_tbl[index] + g_leg[i].vec * time;
			}
			if (i == L)
			{
				g_arm[i].vec = armL_walk_tbl[index + 1] - armL_walk_tbl[index];
				g_leg[i].vec = leg1L_walk_tbl[index + 1] - leg1L_walk_tbl[index];

				g_arm[i].rot = armL_walk_tbl[index] + g_arm[i].vec * time;
				g_leg[i].rot = leg1L_walk_tbl[index] + g_leg[i].vec * time;

			}
		}
	}

	if (g_Player.AttackMotion == TRUE) //�U�����[�V���� TRUE�Ȃ���s
	{
		g_WalkMotion = false;
		dt = 0.1f;
		g_move_time += dt;		// �A�j���[�V�����̍��v���Ԃɑ���

		int index = (int)g_move_time;
		float	time = g_move_time - index;
		int		size = sizeof(body_attack_tbl1) / sizeof(D3DXVECTOR3);; //�e�[�u���S�̂̃o�C�g�����e�[�u����̃o�C�g���Ŋ���(�e�[�u���������߂�)

		if (g_Player.attackpoint < PLAYER_ATTACK_MAX)
		{
			if ((GetKeyboardTrigger(DIK_SPACE)) || (IsButtonTriggered(0, BUTTON_A))) //�U�����[�V���� ���͊m�F
			{
				g_move_time = 0.0f;
				index = 0;
				g_Player.attackpoint++;

				if (g_Player.attackpoint % 2 == 0)
				{
					GetEffectNo(1);
				}
				else if (g_Player.attackpoint % 2 == 1)
				{
					GetEffectNo(2);
				}

				SetEffect(D3DXVECTOR3(Wpos.x, Wpos.y + 10.0f, Wpos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), EFFECT_SIZE_X, EFFECT_SIZE_Y, 20);

				// SE�Đ�
				PlaySound(SOUND_LABEL_SE_ATTACK2);
			}
		}

		if (index > (size - 2))	// �S�[�����I�[�o�[���Ă�����A�S�[���֖߂�
		{
			g_move_time = 0.0f;
			index = 0;
			g_Player.AttackMotion = false;
			g_Player.attackpoint = 0;
		}

		switch (g_Player.attackpoint)
		{
		case 1:
			// �A�j���[�V���������镔��	�@X = StartX + (EndX - StartX) * ���̎���
			g_Player.vec = body_attack_tbl1[index + 1] - body_attack_tbl1[index];
			g_head.vec = head_attack_tbl1[index + 1] - head_attack_tbl1[index];

			g_Player.rot = body_attack_tbl1[index] + g_Player.vec * time;
			g_head.rot = head_attack_tbl1[index] + g_head.vec * time;

			for (int i = 0; i < LR; i++)
			{
				if (i == R)
				{
					g_arm[i].vec = armR_attack_tbl1[index + 1] - armR_attack_tbl1[index];
					g_leg[i].vec = legR_attack_tbl1[index + 1] - legR_attack_tbl1[index];

					g_arm[i].rot = armR_attack_tbl1[index] + g_arm[i].vec * time;
					g_leg[i].rot = legR_attack_tbl1[index] + g_leg[i].vec * time;
				}
				if (i == L)
				{
					g_arm[i].vec = armL_attack_tbl1[index + 1] - armL_attack_tbl1[index];
					g_leg[i].vec = legL_attack_tbl1[index + 1] - legL_attack_tbl1[index];

					g_arm[i].rot = armL_attack_tbl1[index] + g_arm[i].vec * time;
					g_leg[i].rot = legL_attack_tbl1[index] + g_leg[i].vec * time;
				}
			}
			break;
		case 2:
			// �A�j���[�V���������镔��	�@X = StartX + (EndX - StartX) * ���̎���
			g_Player.vec = body_attack_tbl2[index + 1] - body_attack_tbl2[index];
			g_head.vec = head_attack_tbl2[index + 1] - head_attack_tbl2[index];

			g_Player.rot = body_attack_tbl2[index] + g_Player.vec * time;
			g_head.rot = head_attack_tbl2[index] + g_head.vec * time;

			for (int i = 0; i < LR; i++)
			{
				if (i == R)
				{
					g_arm[i].vec = armR_attack_tbl2[index + 1] - armR_attack_tbl2[index];
					g_leg[i].vec = legR_attack_tbl2[index + 1] - legR_attack_tbl2[index];

					g_arm[i].rot = armR_attack_tbl2[index] + g_arm[i].vec * time;
					g_leg[i].rot = legR_attack_tbl2[index] + g_leg[i].vec * time;
				}
				if (i == L)
				{
					g_arm[i].vec = armL_attack_tbl2[index + 1] - armL_attack_tbl2[index];
					g_leg[i].vec = legL_attack_tbl2[index + 1] - legL_attack_tbl2[index];

					g_arm[i].rot = armL_attack_tbl2[index] + g_arm[i].vec * time;
					g_leg[i].rot = legL_attack_tbl2[index] + g_leg[i].vec * time;
				}
			}
			break;
		case 3:
			// �A�j���[�V���������镔��	�@X = StartX + (EndX - StartX) * ���̎���
			g_Player.vec = body_attack_tbl3[index + 1] - body_attack_tbl3[index];
			g_head.vec = head_attack_tbl3[index + 1] - head_attack_tbl3[index];

			g_Player.rot = body_attack_tbl3[index] + g_Player.vec * time;
			g_head.rot = head_attack_tbl3[index] + g_head.vec * time;

			for (int i = 0; i < LR; i++)
			{
				if (i == R)
				{
					g_arm[i].vec = armR_attack_tbl3[index + 1] - armR_attack_tbl3[index];
					g_leg[i].vec = legR_attack_tbl3[index + 1] - legR_attack_tbl3[index];

					g_arm[i].rot = armR_attack_tbl3[index] + g_arm[i].vec * time;
					g_leg[i].rot = legR_attack_tbl3[index] + g_leg[i].vec * time;
				}
				if (i == L)
				{
					g_arm[i].vec = armL_attack_tbl3[index + 1] - armL_attack_tbl3[index];
					g_leg[i].vec = legL_attack_tbl3[index + 1] - legL_attack_tbl3[index];

					g_arm[i].rot = armL_attack_tbl3[index] + g_arm[i].vec * time;
					g_leg[i].rot = legL_attack_tbl3[index] + g_leg[i].vec * time;
				}
			}
			break;
		case 4:
			// �A�j���[�V���������镔��	�@X = StartX + (EndX - StartX) * ���̎���
			g_Player.vec = body_attack_tbl4[index + 1] - body_attack_tbl4[index];
			g_head.vec = head_attack_tbl4[index + 1] - head_attack_tbl4[index];

			g_Player.rot = body_attack_tbl4[index] + g_Player.vec * time;
			g_head.rot = head_attack_tbl4[index] + g_head.vec * time;

			for (int i = 0; i < LR; i++)
			{
				if (i == R)
				{
					g_arm[i].vec = armR_attack_tbl4[index + 1] - armR_attack_tbl4[index];
					g_leg[i].vec = legR_attack_tbl4[index + 1] - legR_attack_tbl4[index];

					g_arm[i].rot = armR_attack_tbl4[index] + g_arm[i].vec * time;
					g_leg[i].rot = legR_attack_tbl4[index] + g_leg[i].vec * time;
				}
				if (i == L)
				{
					g_arm[i].vec = armL_attack_tbl4[index + 1] - armL_attack_tbl4[index];
					g_leg[i].vec = legL_attack_tbl4[index + 1] - legL_attack_tbl4[index];

					g_arm[i].rot = armL_attack_tbl4[index] + g_arm[i].vec * time;
					g_leg[i].rot = legL_attack_tbl4[index] + g_leg[i].vec * time;
				}
			}
			break;
		}
	}

	// �W�����v�H
	if (g_Player.jumppoint < PLAYER_JUMP_MAX)
	{
		if (GetKeyboardTrigger(DIK_J) || (IsButtonTriggered(0, BUTTON_B)))
		{
			if (g_Player.jump == false)	// ���̓W�����v���Ă��Ȃ��H
			{								// Yes
				g_WalkMotion = false;
				g_JumpMotion = true;
				g_Player.jump = true;
				g_Player.jump_cnt = 0;
				// SE�Đ�
				PlaySound(SOUND_LABEL_SE_JUMP);
			}
		}
	}

	// �W�����v����
	if (g_Player.jump == true)
	{
		float rag = D3DX_PI / g_Player.jump_cnt_max;
		float angle = rag * g_Player.jump_cnt;
		g_Player.ofs.y = sinf(angle) * g_Player.jump_y;
		g_Player.jump_cnt++;

		if (g_Player.jump_cnt >= g_Player.jump_cnt_max)	// �W�����v�I���H
		{														// Yes
			g_Player.jump = false;
			g_Player.ofs = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

	}

	if (g_JumpMotion == true) //�U�����[�V���� TRUE�Ȃ���s
	{
		g_WalkMotion = false;
		dt = 0.04f;
		g_jumpmove_time += dt;		// �A�j���[�V�����̍��v���Ԃɑ���

		int index = (int)g_jumpmove_time;
		float	time = g_jumpmove_time - index;
		int		size = sizeof(body_jump_tbl1) / sizeof(D3DXVECTOR3);; //�e�[�u���S�̂̃o�C�g�����e�[�u����̃o�C�g���Ŋ���(�e�[�u���������߂�)

		if (g_Player.jumppoint < PLAYER_JUMP_MAX)
		{
			if (GetKeyboardTrigger(DIK_J) || (IsButtonTriggered(0, BUTTON_B)))
			{
				g_Player.jump_cnt = 0;
				g_jumpmove_time = 0.0f;
				index = 0;
				g_Player.jumppoint++;
				// SE�Đ�
				PlaySound(SOUND_LABEL_SE_JUMP);
			}
		}

		if (index == 2)
		{
			// SE�Đ�
			PlaySound(SOUND_LABEL_SE_JUMPOFF);
		}

		if (index > (size - 2))	// �S�[�����I�[�o�[���Ă�����A�S�[���֖߂�
		{
			g_jumpmove_time = 0.0f;
			index = 0;
			g_Player.jumppoint = 0;
			g_JumpMotion = false;
			g_WalkMotion = true;
		}

		switch (g_Player.jumppoint)
		{
		case 0:		//�W�����v1���
			// �A�j���[�V���������镔��	�@X = StartX + (EndX - StartX) * ���̎���
			g_Player.vec = body_jump_tbl1[index + 1] - body_jump_tbl1[index];
			g_head.vec = head_jump_tbl1[index + 1] - head_jump_tbl1[index];

			g_Player.rot = body_jump_tbl1[index] + g_Player.vec * time;
			g_head.rot = head_jump_tbl1[index] + g_head.vec * time;

			for (int i = 0; i < LR; i++)
			{
				if (i == R)
				{
					g_arm[i].vec = armR_jump_tbl1[index + 1] - armR_jump_tbl1[index];
					g_leg[i].vec = legR_jump_tbl1[index + 1] - legR_jump_tbl1[index];

					g_arm[i].rot = armR_jump_tbl1[index] + g_arm[i].vec * time;
					g_leg[i].rot = legR_jump_tbl1[index] + g_leg[i].vec * time;
				}
				if (i == L)
				{
					g_arm[i].vec = armL_jump_tbl1[index + 1] - armL_jump_tbl1[index];
					g_leg[i].vec = legL_jump_tbl1[index + 1] - legL_jump_tbl1[index];

					g_arm[i].rot = armL_jump_tbl1[index] + g_arm[i].vec * time;
					g_leg[i].rot = legL_jump_tbl1[index] + g_leg[i].vec * time;
				}
			}
			break;

		case 1:		//�W�����v2���
			// �A�j���[�V���������镔��	�@X = StartX + (EndX - StartX) * ���̎���
			g_Player.vec = body_jump_tbl2[index + 1] - body_jump_tbl2[index];
			g_head.vec = head_jump_tbl2[index + 1] - head_jump_tbl2[index];

			g_Player.rot = body_jump_tbl2[index] + g_Player.vec * time;
			g_head.rot = head_jump_tbl2[index] + g_head.vec * time;

			for (int i = 0; i < LR; i++)
			{
				if (i == R)
				{
					g_arm[i].vec = armR_jump_tbl2[index + 1] - armR_jump_tbl2[index];
					g_leg[i].vec = legR_jump_tbl2[index + 1] - legR_jump_tbl2[index];

					g_arm[i].rot = armR_jump_tbl2[index] + g_arm[i].vec * time;
					g_leg[i].rot = legR_jump_tbl2[index] + g_leg[i].vec * time;
				}
				if (i == L)
				{
					g_arm[i].vec = armL_jump_tbl2[index + 1] - armL_jump_tbl2[index];
					g_leg[i].vec = legL_jump_tbl2[index + 1] - legL_jump_tbl2[index];

					g_arm[i].rot = armL_jump_tbl2[index] + g_arm[i].vec * time;
					g_leg[i].rot = legL_jump_tbl2[index] + g_leg[i].vec * time;
				}
			}
			break;

		}
	}

	{
		// �J�����ɑ΂��ē��͂̂����������փv���C���[���������Ĉړ�������
		g_Player.rot.y = roty + cam->rot.y;
		g_Player.pos.x -= sinf(g_Player.rot.y) * g_Player.spd;
		g_Player.pos.y += g_Player.ofs.y + g_Player.spd;
		g_Player.pos.z -= cosf(g_Player.rot.y) * g_Player.spd;

		D3DXMATRIX CameraRoll;
		D3DXMATRIX buff;
		D3DXVECTOR3 PtoC = D3DXVECTOR3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);
		D3DXVECTOR3 PtoAt = D3DXVECTOR3(0.0f, TILT_Y_CAM, TILT_Z_CAM);

		//cam->at.x = PtoAt.x * cosf(cam->rot.y) - PtoAt.z * (-sinf(cam->rot.y));
		//cam->at.y = PtoAt.y;
		//cam->at.z = PtoAt.x * (-sinf(cam->rot.y)) + PtoAt.z * cosf(cam->rot.y);

		//cam->pos.x = PtoC.x * cosf(cam->rot.y) - PtoC.z * (-sinf(cam->rot.y));
		//cam->pos.y = PtoC.y;
		//cam->pos.z = PtoC.x * (-sinf(cam->rot.y)) + PtoC.z * cosf(cam->rot.y);

		//��]�s��̏�����
		D3DXMatrixIdentity(&CameraRoll);

		//�o�b�t�@�̏�����
		D3DXMatrixIdentity(&buff);

		//��]�ړ�
		D3DXMatrixRotationYawPitchRoll(&buff, cam->rot.y, cam->rot.x, cam->rot.z);
		D3DXMatrixMultiply(&CameraRoll, &CameraRoll, &buff);

		//�o�b�t�@�̏�����
		D3DXMatrixIdentity(&buff);

		//Player�ɒǏ]
		D3DXMatrixTranslation(&buff, g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
		D3DXMatrixMultiply(&CameraRoll, &CameraRoll, &buff);

		D3DXVec3TransformCoord(&cam->at, &PtoAt, &CameraRoll);
		D3DXVec3TransformCoord(&cam->pos, &PtoC, &CameraRoll);

		// �J�����̒����_�Ǝ��_����l���ɒǏ]������
		//cam->at += g_Player.pos; 
		//cam->pos += g_Player.pos;

	}

	g_Player.spd *= 0.7f;

	// �d�͏���
	g_Player.pos.y -= 10.0f;
	if (g_Player.pos.y <= 0.0f)
	{
		g_Player.pos.y = 0.0f;
		g_Player.jumppoint = 0;
	}

	//�ǐݒ�
	if (g_Player.pos.x < -1980.0f)
	{
		g_Player.pos.x = -1980.0f;
	}
	if (g_Player.pos.x > 1980.0f)
	{
		g_Player.pos.x = 1980.0f;
	}
	if (g_Player.pos.z < -1980.0f)
	{
		g_Player.pos.z = -1980.0f;
	}
	if (g_Player.pos.z > 1980.0f)
	{
		g_Player.pos.z = 1980.0f;
	}

	// ���G
	if (g_Player.protect == true)
	{
		g_Player.protect_cnt++;

		if (g_Player.protect_cnt >= 60)
		{
			g_Player.protect_cnt = 0;
			g_Player.protect = false;
		}
	}

	// �e���v���C���[�̈ʒu�ɍ��킹��
	D3DXVECTOR3 pos = g_Player.pos;
	pos.y = 0.0f;
	SetPositionShadow(g_Player.shadowIdx, pos, g_Player.scl);

	PrintDebugProc("�v���C���[����WASD\n\n");

	PrintDebugProc("PlayerPos%ff%ff%ff\n", g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DMATERIAL9 arm_matDef;
	D3DMATERIAL9 leg_matDef;
	D3DMATERIAL9 weapon_matDef;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_weapon.mtxWorld);
	D3DXMatrixIdentity(&g_head.mtxWorld);

	// �X�P�[���𔽉f S
	D3DXMatrixScaling(&g_weapon.mtxScl, g_weapon.scl.x, g_weapon.scl.y, g_weapon.scl.z);
	D3DXMatrixMultiply(&g_weapon.mtxWorld, &g_weapon.mtxWorld, &g_weapon.mtxScl);
	D3DXMatrixScaling(&g_head.mtxScl, g_head.scl.x, g_head.scl.y, g_head.scl.z);
	D3DXMatrixMultiply(&g_head.mtxWorld, &g_head.mtxWorld, &g_head.mtxScl);

	// ��]�𔽉f R
	D3DXMatrixRotationYawPitchRoll(&g_weapon.mtxRot, g_weapon.rot.y, g_weapon.rot.x, g_weapon.rot.z);
	D3DXMatrixMultiply(&g_weapon.mtxWorld, &g_weapon.mtxWorld, &g_weapon.mtxRot);
	D3DXMatrixRotationYawPitchRoll(&g_head.mtxRot, g_head.rot.y, g_head.rot.x, g_head.rot.z);
	D3DXMatrixMultiply(&g_head.mtxWorld, &g_head.mtxWorld, &g_head.mtxRot);

	// �ړ��𔽉f T
	D3DXMatrixTranslation(&g_weapon.mtxTranslate, g_weapon.pos.x, g_weapon.pos.y, g_weapon.pos.z);
	D3DXMatrixMultiply(&g_weapon.mtxWorld, &g_weapon.mtxWorld, &g_weapon.mtxTranslate);
	D3DXMatrixTranslation(&g_head.mtxTranslate, g_head.pos.x, g_head.pos.y, g_head.pos.z);
	D3DXMatrixMultiply(&g_head.mtxWorld, &g_head.mtxWorld, &g_head.mtxTranslate);

	for (int i = 0; i < LR; i++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_arm[i].mtxWorld);
		D3DXMatrixIdentity(&g_leg[i].mtxWorld);

		// �X�P�[���𔽉f S
		D3DXMatrixScaling(&g_arm[i].mtxScl, g_arm[i].scl.x, g_arm[i].scl.y, g_arm[i].scl.z);
		D3DXMatrixMultiply(&g_arm[i].mtxWorld, &g_arm[i].mtxWorld, &g_arm[i].mtxScl);
		D3DXMatrixScaling(&g_leg[i].mtxScl, g_leg[i].scl.x, g_leg[i].scl.y, g_leg[i].scl.z);
		D3DXMatrixMultiply(&g_leg[i].mtxWorld, &g_leg[i].mtxWorld, &g_leg[i].mtxScl);

		// ��]�𔽉f R
		D3DXMatrixRotationYawPitchRoll(&g_arm[i].mtxRot, g_arm[i].rot.y, g_arm[i].rot.x, g_arm[i].rot.z);
		D3DXMatrixMultiply(&g_arm[i].mtxWorld, &g_arm[i].mtxWorld, &g_arm[i].mtxRot);
		D3DXMatrixRotationYawPitchRoll(&g_leg[i].mtxRot, g_leg[i].rot.y, g_leg[i].rot.x, g_leg[i].rot.z);
		D3DXMatrixMultiply(&g_leg[i].mtxWorld, &g_leg[i].mtxWorld, &g_leg[i].mtxRot);

		// �ړ��𔽉f T
		D3DXMatrixTranslation(&g_arm[i].mtxTranslate, g_arm[i].pos.x, g_arm[i].pos.y, g_arm[i].pos.z);
		D3DXMatrixMultiply(&g_arm[i].mtxWorld, &g_arm[i].mtxWorld, &g_arm[i].mtxTranslate);
		D3DXMatrixTranslation(&g_leg[i].mtxTranslate, g_leg[i].pos.x, g_leg[i].pos.y, g_leg[i].pos.z);
		D3DXMatrixMultiply(&g_leg[i].mtxWorld, &g_leg[i].mtxWorld, &g_leg[i].mtxTranslate);
	}

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Player.mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&g_Player.mtxScl, g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &g_Player.mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&g_Player.mtxRot, g_Player.rot.y, g_Player.rot.x, g_Player.rot.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &g_Player.mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&g_Player.mtxTranslate, g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &g_Player.mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_Player.mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&g_Player.matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	g_Player.pD3DXMat = (D3DXMATERIAL*)g_Player.pD3DXBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_Player.nNumMat; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&g_Player.pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_Player.pD3DTexture);

		// �`��
		g_Player.pD3DXMesh->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&g_Player.matDef);

	//�q��world = �q��world * �e��world
	D3DXMatrixMultiply(&g_head.mtxWorld, &g_head.mtxWorld, &g_head.parent->mtxWorld);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_head.mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&g_head.matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	g_head.pD3DXMat = (D3DXMATERIAL*)g_head.pD3DXBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_head.nNumMat; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&g_head.pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_head.pD3DTexture);

		// �`��
		g_head.pD3DXMesh->DrawSubset(nCntMat);
	}
	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&g_head.matDef);

	for (int i = 0; i < LR; i++)
	{
		//�q��world = �q��world * �e��world
		D3DXMatrixMultiply(&g_arm[i].mtxWorld, &g_arm[i].mtxWorld, &g_arm[i].parent->mtxWorld);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_arm[i].mtxWorld);

		// ���݂̃}�e���A�����擾
		pDevice->GetMaterial(&arm_matDef);

		// �}�e���A�����ɑ΂���|�C���^���擾
		g_arm[i].pD3DXMat = (D3DXMATERIAL*)g_arm[i].pD3DXBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_arm[i].nNumMat; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&g_arm[i].pD3DXMat[nCntMat].MatD3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_arm[i].pD3DTexture);

			// �`��
			g_arm[i].pD3DXMesh->DrawSubset(nCntMat);
		}

		//�q��world = �q��world * �e��world
		D3DXMatrixMultiply(&g_leg[i].mtxWorld, &g_leg[i].mtxWorld, &g_leg[i].parent->mtxWorld);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_leg[i].mtxWorld);

		// ���݂̃}�e���A�����擾
		pDevice->GetMaterial(&leg_matDef);

		// �}�e���A�����ɑ΂���|�C���^���擾
		g_leg[i].pD3DXMat = (D3DXMATERIAL*)g_leg[i].pD3DXBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_leg[i].nNumMat; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&g_leg[i].pD3DXMat[nCntMat].MatD3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_leg[i].pD3DTexture);

			// �`��
			g_leg[i].pD3DXMesh->DrawSubset(nCntMat);
		}
	}
	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&arm_matDef);
	pDevice->SetMaterial(&leg_matDef);

	//�q��world = �q��world * �e��world
	D3DXMatrixMultiply(&g_weapon.mtxWorld, &g_weapon.mtxWorld, &g_weapon.parent->mtxWorld);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_weapon.mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&weapon_matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	g_weapon.pD3DXMat = (D3DXMATERIAL*)g_weapon.pD3DXBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_weapon.nNumMat; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&g_weapon.pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_weapon.pD3DTexture);

		// �`��
		g_weapon.pD3DXMesh->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&weapon_matDef);
}

//=============================================================================
// �v���C���[�����擾
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}

//=============================================================================
// ��������擾
//=============================================================================
void GetweaponPos(D3DXVECTOR3 &pos, D3DXVECTOR3 &S, D3DXVECTOR3 &E)
{
	E = S = g_weapon.pos;
	E.y += WEAPON_LEN * sinf(1.3);
	E.z += WEAPON_LEN * cosf(1.3);
	S.y -= WEAPON_LEN * sinf(1.3);
	S.z -= WEAPON_LEN * cosf(1.3);

	D3DXVec3TransformCoord(&pos, &g_weapon.pos, &g_weapon.mtxWorld);
	D3DXVec3TransformCoord(&E, &E, &g_weapon.mtxWorld);
	D3DXVec3TransformCoord(&S, &S, &g_weapon.mtxWorld);

	return;
}

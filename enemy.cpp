//=============================================================================
//
// �G�l�~�[���f������[enemy.cpp]
// Author : �_�� ���t
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "enemy.h"
#include "shadow.h"
#include "player.h"
#include "item.h"
#include "collision.h"
#include "debugproc.h"
#include "particle.h"

//*****************************************************************************
//�v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���錾
//*****************************************************************************

static ENEMY				g_Enemy[MAX_ENEMY];							// �G�l�~�[
static ENEMY				g_Enemyhead[MAX_ENEMY];						// �v���C���[�̓���
static ENEMY				g_EnemyarmR[MAX_ENEMY];                     // �v���C���[�̘r
static ENEMY				g_EnemyarmL[MAX_ENEMY];                     // �v���C���[�̘r
static ENEMY				g_EnemylegR[MAX_ENEMY];                     // �v���C���[�̑�����
static ENEMY				g_EnemylegL[MAX_ENEMY];                     // �v���C���[�̑�����
D3DXVECTOR3					g_Shadowpos[ENEMY_GROUP];					// �r���{�[�h�����ʒu

LPDIRECT3DTEXTURE9	enemy_pD3DTexture_body;			// �e�N�X�`���ւ̃|�C���^
LPD3DXMESH			enemy_pD3DXMesh_body;			// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		enemy_pD3DXBuffMat_body;		// �}�e���A�����ւ̃|�C���^
DWORD				enemy_nNumMat_body;				// �}�e���A�����̐�

LPDIRECT3DTEXTURE9	enemy_pD3DTexture_head;			// �e�N�X�`���ւ̃|�C���^
LPD3DXMESH			enemy_pD3DXMesh_head;			// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		enemy_pD3DXBuffMat_head;		// �}�e���A�����ւ̃|�C���^
DWORD				enemy_nNumMat_head;				// �}�e���A�����̐�

LPDIRECT3DTEXTURE9	enemy_pD3DTexture_armL;			// �e�N�X�`���ւ̃|�C���^
LPD3DXMESH			enemy_pD3DXMesh_armL;			// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		enemy_pD3DXBuffMat_armL;		// �}�e���A�����ւ̃|�C���^
DWORD				enemy_nNumMat_armL;				// �}�e���A�����̐�

LPDIRECT3DTEXTURE9	enemy_pD3DTexture_armR;			// �e�N�X�`���ւ̃|�C���^
LPD3DXMESH			enemy_pD3DXMesh_armR;			// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		enemy_pD3DXBuffMat_armR;		// �}�e���A�����ւ̃|�C���^
DWORD				enemy_nNumMat_armR;				// �}�e���A�����̐�

LPDIRECT3DTEXTURE9	enemy_pD3DTexture_legL;			// �e�N�X�`���ւ̃|�C���^
LPD3DXMESH			enemy_pD3DXMesh_legL;			// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		enemy_pD3DXBuffMat_legL;		// �}�e���A�����ւ̃|�C���^
DWORD				enemy_nNumMat_legL;				// �}�e���A�����̐�

LPDIRECT3DTEXTURE9	enemy_pD3DTexture_legR;			// �e�N�X�`���ւ̃|�C���^
LPD3DXMESH			enemy_pD3DXMesh_legR;			// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		enemy_pD3DXBuffMat_legR;		// �}�e���A�����ւ̃|�C���^
DWORD				enemy_nNumMat_legR;				// �}�e���A�����̐�

ENEMYTBL move_tbl1[] = {
	{D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.01f},
	{D3DXVECTOR3(100.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,-2.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.02f},
	{D3DXVECTOR3(-100.0f,0.0f,-100.0f),D3DXVECTOR3(0.0f,3.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.05f},
	{D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.0f},
};

ENEMYTBL move_tbl2[] = {
	{D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.01f},
	{D3DXVECTOR3(50.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,-2.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.02f},
	{D3DXVECTOR3(100.0f,0.0f,-300.0f),D3DXVECTOR3(0.0f,8.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.04f},
	{D3DXVECTOR3(200.0f,0.0f,-100.0f),D3DXVECTOR3(0.0f,.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.06f},
	{D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.0f},
};

ENEMYTBL move_tbl3[] = {
	{D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.01f},
	{D3DXVECTOR3(50.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,-3.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.02f},
	{D3DXVECTOR3(100.0f,0.0f,-300.0f),D3DXVECTOR3(0.0f,5.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.04f},
	{D3DXVECTOR3(150.0f,0.0f,400.0f),D3DXVECTOR3(0.0f,5.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.06f},
	{D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.0f},
};

ENEMYTBL move_tbl4[] = {
	{D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.01f},
	{D3DXVECTOR3(300.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,-3.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.02f},
	{D3DXVECTOR3(500.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,5.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.04f},
	{D3DXVECTOR3(300.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,5.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.06f},
	{D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.0f},
};

ENEMYTBL move_tbl5[] = {
	{D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.01f},
	{D3DXVECTOR3(0.0f,0.0f,300.0f),D3DXVECTOR3(0.0f,-3.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.02f},
	{D3DXVECTOR3(0.0f,0.0f,500.0f),D3DXVECTOR3(0.0f,5.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.04f},
	{D3DXVECTOR3(0.0f,0.0f,300.0f),D3DXVECTOR3(0.0f,5.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.06f},
	{D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f),0.0f},
};

// �K�w�A�j���[�V����
ENEMY_SET set_tbl_ENEMY[] = {    //pos,rot,scl �e�p�[�c�̏����l
	{	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.0f, (rand() % 300 + 1.0f), 0.0f),	D3DXVECTOR3(0.5f, 0.5f, 0.5f)},//��			
	{	D3DXVECTOR3(0.0f, 50.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.9f, 0.9f, 0.9f)},//����		
	{	D3DXVECTOR3(-12.0f, 40.0f, -0.1f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.9f, 0.9f, 0.9f)},//�E�r
	{	D3DXVECTOR3(12.0f, 40.0f, -0.1f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.9f, 0.9f, 0.9f)},//���r
	{	D3DXVECTOR3(-5.0f,20.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f)},//�E������
	{	D3DXVECTOR3(5.0f, 20.0f, 0.0f),	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(1.0f, 1.0f, 1.0f)},//��������
};

//�������[�V����
D3DXVECTOR3 Ebody_walk_tbl[] = { //��

	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.00000f, -1.000000f, 0.000000f),
	D3DXVECTOR3(0.0f, 1.0f, 0.000000f),
};

D3DXVECTOR3 Ehead_walk_tbl[] = { //����

	D3DXVECTOR3(0.000000f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.000000f, D3DX_PI * 2, 0.000000f),
	D3DXVECTOR3(0.000000f, D3DX_PI * 2, 0.000000f),

};

D3DXVECTOR3 EarmR_walk_tbl[] = { //�E�r

	D3DXVECTOR3(0.374533f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.574533f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.374533f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 EarmL_walk_tbl[] = { //���r

	D3DXVECTOR3(-0.374533f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.574533f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-0.374533f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 ElegR_walk_tbl[] = { //�E������

	D3DXVECTOR3(-1.023599f, 0.000000f, 0.000000f),
	D3DXVECTOR3(1.023599f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-1.023599f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 ElegL_walk_tbl[] = { //��������

	D3DXVECTOR3(1.023599f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-1.023599f, 0.000000f, 0.000000f),
	D3DXVECTOR3(1.023599f, 0.000000f, 0.000000f),
};

//�U�����[�V����
D3DXVECTOR3 Ebody_Attack_tbl[] = { //��

	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.000000f),
	D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.000000f),
	D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.000000f),
	D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.000000f),
	D3DXVECTOR3(-D3DX_PI / 2, 0.0f, 0.000000f),
	D3DXVECTOR3(-1.50000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-1.40000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-1.30000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(-1.00000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.00000f, -1.000000f, 0.000000f),
	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 Ehead_Attack_tbl[] = { //����

	D3DXVECTOR3(0.00000f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.0f, D3DX_PI * 2, 0.000000f),
	D3DXVECTOR3(0.0f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.0f, D3DX_PI * 2, 0.000000f),
	D3DXVECTOR3(0.0f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.0f, D3DX_PI * 2, 0.000000f),
	D3DXVECTOR3(0.0f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.0f, D3DX_PI * 2, 0.000000f),
	D3DXVECTOR3(0.0f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.0f, D3DX_PI * 2, 0.000000f),
	D3DXVECTOR3(0.00000f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.00000f, D3DX_PI * 2, 0.000000f),
};

D3DXVECTOR3 EarmR_Attack_tbl[] = { //�E�r

	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 EarmL_Attack_tbl[] = { //���r

	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 ElegR_Attack_tbl[] = { //�E������

	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
};

D3DXVECTOR3 ElegL_Attack_tbl[] = { //��������

	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.1f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
};

//�|���Ƃ��̃��[�V����
D3DXVECTOR3 Ebody_Hit_tbl[] = { //��
	D3DXVECTOR3(0.00000f, 0.000000f, 0.000000f),
	D3DXVECTOR3(0.00000f, 0.0f,  D3DX_PI * 2),
};

D3DXVECTOR3 Ehead_Hit_tbl[] = { //����

	D3DXVECTOR3(0.00000f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.0f, D3DX_PI * 2, 0.000000f),
	D3DXVECTOR3(0.0f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.0f, D3DX_PI * 2, 0.000000f),
	D3DXVECTOR3(0.0f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.0f, D3DX_PI * 2, 0.000000f),
	D3DXVECTOR3(0.0f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.0f, D3DX_PI * 2, 0.000000f),
	D3DXVECTOR3(0.0f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.0f, D3DX_PI * 2, 0.000000f),
	D3DXVECTOR3(0.00000f, 0.0f, 0.000000f),
	D3DXVECTOR3(0.00000f, D3DX_PI * 2, 0.000000f),
};

D3DXVECTOR3 EarmR_Hit_tbl[] = { //�E�r

	D3DXVECTOR3(0.00000f, 0.000000f, -2.000000f),
	D3DXVECTOR3(0.0f, 0.0f, -1.500000f),
};

D3DXVECTOR3 EarmL_Hit_tbl[] = { //���r

	D3DXVECTOR3(0.00000f, 0.000000f, 2.000000f),
	D3DXVECTOR3(0.0f, 0.0f, 1.500000f),
};

D3DXVECTOR3 ElegR_Hit_tbl[] = { //�E������

	D3DXVECTOR3(1.0f, 0.000000f, -2.000000f),
	D3DXVECTOR3(0.5f, 0.0f, -1.500000f),
};

D3DXVECTOR3 ElegL_Hit_tbl[] = { //��������

	D3DXVECTOR3(1.0f, 0.000000f, 2.000000f),
	D3DXVECTOR3(0.5f, 0.0f, -1.500000f),
};

D3DXVECTOR3 formation[] =	{   //�w�`�t�H�[���[�V����
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(15.0f, 0.0f, 15.0f),
	D3DXVECTOR3(-15.0f, 0.0f, 15.0f),
	D3DXVECTOR3(20.0f, 0.0f, 20.0f),
	D3DXVECTOR3(-20.0f, 0.0f, 20.0f),
};

D3DXVECTOR3 formation1[] = {   //�w�`�t�H�[���[�V����
	D3DXVECTOR3(0.0f, 30.0f, 200.0f),
	D3DXVECTOR3(50.0f, 0.0f, 15.0f),
	D3DXVECTOR3(-300.0f, 20.0f, 15.0f),
	D3DXVECTOR3(20.0f, 0.0f, 300.0f),
	D3DXVECTOR3(-200.0f, 50.0f, 50.0f),
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		enemy_pD3DTexture_body = NULL;
		enemy_pD3DXMesh_body = NULL;
		enemy_pD3DXBuffMat_body = NULL;
		enemy_nNumMat_body = 0;

		enemy_pD3DTexture_head = NULL;
		enemy_pD3DXMesh_head = NULL;
		enemy_pD3DXBuffMat_head = NULL;
		enemy_nNumMat_head = 0;

		enemy_pD3DTexture_armL = NULL;
		enemy_pD3DXMesh_armL = NULL;
		enemy_pD3DXBuffMat_armL = NULL;
		enemy_nNumMat_armL = 0;

		enemy_pD3DTexture_armR = NULL;
		enemy_pD3DXMesh_armR = NULL;
		enemy_pD3DXBuffMat_armR = NULL;
		enemy_nNumMat_armR = 0;

		enemy_pD3DTexture_legL = NULL;
		enemy_pD3DXMesh_legL = NULL;
		enemy_pD3DXBuffMat_legL = NULL;
		enemy_nNumMat_legL = 0;

		enemy_pD3DTexture_legR = NULL;
		enemy_pD3DXMesh_legR = NULL;
		enemy_pD3DXBuffMat_legR = NULL;
		enemy_nNumMat_legR = 0;

		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(MODEL_ENEMY,		// �ǂݍ��ރ��f���t�@�C����(x�t�@�C��)
			D3DXMESH_SYSTEMMEM,							// ���b�V���̍쐬�I�v�V�������w��
			pDevice,									// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^�[
			NULL,										// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			&enemy_pD3DXBuffMat_body,					// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			NULL,										// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
			&enemy_nNumMat_body,						// D3DXMATERIAL�\���̂̐�
			&enemy_pD3DXMesh_body)))					// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
		{
			return E_FAIL;
		}

		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(MODEL_ENEMY_HEAD,	// �ǂݍ��ރ��f���t�@�C����(x�t�@�C��)
			D3DXMESH_SYSTEMMEM,							// ���b�V���̍쐬�I�v�V�������w��
			pDevice,									// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^�[
			NULL,										// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			&enemy_pD3DXBuffMat_head,					// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			NULL,										// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
			&enemy_nNumMat_head,						// D3DXMATERIAL�\���̂̐�
			&enemy_pD3DXMesh_head)))					// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
		{
			return E_FAIL;
		}

		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(MODEL_ENEMY_ARM_L,	// �ǂݍ��ރ��f���t�@�C����(x�t�@�C��)
			D3DXMESH_SYSTEMMEM,							// ���b�V���̍쐬�I�v�V�������w��
			pDevice,									// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^�[
			NULL,										// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			&enemy_pD3DXBuffMat_armL,					// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			NULL,										// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
			&enemy_nNumMat_armL,						// D3DXMATERIAL�\���̂̐�
			&enemy_pD3DXMesh_armL)))					// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
		{
			return E_FAIL;
		}

		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(MODEL_ENEMY_ARM_R,	// �ǂݍ��ރ��f���t�@�C����(x�t�@�C��)
			D3DXMESH_SYSTEMMEM,							// ���b�V���̍쐬�I�v�V�������w��
			pDevice,									// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^�[
			NULL,										// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			&enemy_pD3DXBuffMat_armR,					// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			NULL,										// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
			&enemy_nNumMat_armR,						// D3DXMATERIAL�\���̂̐�
			&enemy_pD3DXMesh_armR)))					// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
		{
			return E_FAIL;
		}

		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(MODEL_ENEMY_LEG_L,	// �ǂݍ��ރ��f���t�@�C����(x�t�@�C��)
			D3DXMESH_SYSTEMMEM,							// ���b�V���̍쐬�I�v�V�������w��
			pDevice,									// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^�[
			NULL,										// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			&enemy_pD3DXBuffMat_legL,					// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			NULL,										// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
			&enemy_nNumMat_legL,						// D3DXMATERIAL�\���̂̐�
			&enemy_pD3DXMesh_legL)))					// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
		{
			return E_FAIL;
		}

		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(MODEL_ENEMY_LEG_R,	// �ǂݍ��ރ��f���t�@�C����(x�t�@�C��)
			D3DXMESH_SYSTEMMEM,							// ���b�V���̍쐬�I�v�V�������w��
			pDevice,									// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^�[
			NULL,										// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			&enemy_pD3DXBuffMat_legR,					// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			NULL,										// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
			&enemy_nNumMat_legR,						// D3DXMATERIAL�\���̂̐�
			&enemy_pD3DXMesh_legR)))					// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
		{
			return E_FAIL;
		}
	}

#if 0
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_FILENAME,				// �t�@�C���̖��O
		&enemy_pD3DTexture_body);		// �ǂݍ��ރ������[
#endif

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_Enemy[i].use = false;			// �ŏ��͖��g�p
		g_Enemy[i].formation = 0;		// �w�`�ԍ�

		//�G�l�~�[�̐��`��ԃp�^�[���̐ݒ�
		switch (i%ENEMY_MOVEPATTERN)
		{
		case 0:
			g_Enemy[i].movepattern = move_tbl1;
			g_Enemy[i].size = sizeof(move_tbl1) / sizeof(ENEMYTBL);
			break;
		case 1:
			g_Enemy[i].movepattern = move_tbl2;
			g_Enemy[i].size = sizeof(move_tbl2) / sizeof(ENEMYTBL);
			break;
		case 2:
			g_Enemy[i].movepattern = move_tbl3;
			g_Enemy[i].size = sizeof(move_tbl3) / sizeof(ENEMYTBL);
			break;
		case 3:
			g_Enemy[i].movepattern = move_tbl4;
			g_Enemy[i].size = sizeof(move_tbl4) / sizeof(ENEMYTBL);
			break;
		case 4:
			g_Enemy[i].movepattern = move_tbl5;
			g_Enemy[i].size = sizeof(move_tbl5) / sizeof(ENEMYTBL);
			break;
		}

		g_Enemy[i].walkmove_time = 0;
		g_Enemy[i].attackmove_time = 0;
		g_Enemy[i].hitmove_time = 0;

		// ��
		// �ʒu�E��]�E�X�P�[���̏������ݒ�
		g_Enemy[i].pos = set_tbl_ENEMY[BODY].pos;
		g_Enemy[i].rot = set_tbl_ENEMY[BODY].rot;
		g_Enemy[i].scl = set_tbl_ENEMY[BODY].scl;

		// ��ʒu�E��]�E�X�P�[���̏������ݒ�
		g_Enemy[i].spos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].srot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].sscl = D3DXVECTOR3(0.5f, 0.5f, 0.5f);

		g_Enemy[i].move_time = 0.0f;
		g_Enemy[i].back_time = 0.0f;
		g_Enemy[i].spd = 1.0f;
		g_Enemy[i].following = false;
		g_Enemy[i].timestop = false;

		g_Enemy[i].oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].hit_oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].oldindex = 0;
		g_Enemy[i].pattern = 1;
		g_Enemy[i].parent = NULL;
		g_Enemy[i].status = WALK;

		g_Enemy[i].attack_dt = 0;
		g_Enemy[i].hit_dt = 0;

		g_Enemy[i].hit = 10.0f;
		g_Enemy[i].hit_cnt = 0;
		g_Enemy[i].hit_cnt_max = 40;
		g_Enemy[i].ofs = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		for (int j = 0; j < 5; j++)
		{
			// �e�̏�����
			g_Shadowpos[j] = D3DXVECTOR3(g_Enemy[i].pos.x + j * 10.0f, g_Enemy[i].pos.y, g_Enemy[i].pos.z + j * 20);
			g_Shadowpos[j].y = 0.0f;
			g_Enemy[i].shadowIdx[j] = CreateShadow(g_Shadowpos[j], g_Enemy[i].scl);
		}

		// ��
		g_Enemyhead[i].use = false;			// �ŏ��͖��g�p

		switch (i%ENEMY_MOVEPATTERN)
		{
		case 0:
			g_Enemyhead[i].movepattern = move_tbl1;
			g_Enemyhead[i].size = sizeof(move_tbl1) / sizeof(ENEMYTBL);
			break;
		case 1:
			g_Enemyhead[i].movepattern = move_tbl2;
			g_Enemyhead[i].size = sizeof(move_tbl2) / sizeof(ENEMYTBL);
			break;
		case 2:
			g_Enemyhead[i].movepattern = move_tbl3;
			g_Enemyhead[i].size = sizeof(move_tbl3) / sizeof(ENEMYTBL);
			break;
		case 3:
			g_Enemyhead[i].movepattern = move_tbl4;
			g_Enemyhead[i].size = sizeof(move_tbl4) / sizeof(ENEMYTBL);
			break;
		case 4:
			g_Enemyhead[i].movepattern = move_tbl5;
			g_Enemyhead[i].size = sizeof(move_tbl5) / sizeof(ENEMYTBL);
			break;
		}

		// �ʒu�E��]�E�X�P�[���̏������ݒ�
		g_Enemyhead[i].pos = set_tbl_ENEMY[HEAD].pos;
		g_Enemyhead[i].rot = set_tbl_ENEMY[HEAD].rot;
		g_Enemyhead[i].scl = set_tbl_ENEMY[HEAD].scl;

		// ��ʒu�E��]�E�X�P�[���̏������ݒ�
		g_Enemyhead[i].spos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemyhead[i].srot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemyhead[i].sscl = D3DXVECTOR3(0.5f, 0.5f, 0.5f);

		g_Enemyhead[i].move_time = 0.0f;
		g_Enemyhead[i].back_time = 0.0f;
		g_Enemyhead[i].spd = 1.0f;
		g_Enemyhead[i].following = false;
		g_Enemyhead[i].timestop = false;
		g_Enemyhead[i].oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemyhead[i].hit_oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemyhead[i].oldindex = 0;
		g_Enemyhead[i].pattern = 1;
		g_Enemyhead[i].parent = &g_Enemy[i];

		// �E�r
		g_EnemyarmR[i].use = false;			// �ŏ��͖��g�p

		switch (i%ENEMY_MOVEPATTERN)
		{
		case 0:
			g_EnemyarmR[i].movepattern = move_tbl1;
			g_EnemyarmR[i].size = sizeof(move_tbl1) / sizeof(ENEMYTBL);
			break;
		case 1:
			g_EnemyarmR[i].movepattern = move_tbl2;
			g_EnemyarmR[i].size = sizeof(move_tbl2) / sizeof(ENEMYTBL);
			break;
		case 2:
			g_EnemyarmR[i].movepattern = move_tbl3;
			g_EnemyarmR[i].size = sizeof(move_tbl3) / sizeof(ENEMYTBL);
			break;
		case 3:
			g_EnemyarmR[i].movepattern = move_tbl4;
			g_EnemyarmR[i].size = sizeof(move_tbl4) / sizeof(ENEMYTBL);
			break;
		case 4:
			g_EnemyarmR[i].movepattern = move_tbl5;
			g_EnemyarmR[i].size = sizeof(move_tbl5) / sizeof(ENEMYTBL);
			break;
		}

		// �ʒu�E��]�E�X�P�[���̏������ݒ�
		g_EnemyarmR[i].pos = set_tbl_ENEMY[ARM_R].pos;
		g_EnemyarmR[i].rot = set_tbl_ENEMY[ARM_R].rot;
		g_EnemyarmR[i].scl = set_tbl_ENEMY[ARM_R].scl;

		// ��ʒu�E��]�E�X�P�[���̏������ݒ�
		g_EnemyarmR[i].spos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemyarmR[i].srot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemyarmR[i].sscl = D3DXVECTOR3(0.5f, 0.5f, 0.5f);

		g_EnemyarmR[i].move_time = 0.0f;
		g_EnemyarmR[i].back_time = 0.0f;
		g_EnemyarmR[i].spd = 1.0f;
		g_EnemyarmR[i].following = false;
		g_EnemyarmR[i].timestop = false;
		g_EnemyarmR[i].oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemyarmR[i].hit_oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemyarmR[i].oldindex = 0;
		g_EnemyarmR[i].pattern = 1;
		g_EnemyarmR[i].parent = &g_Enemy[i];

		// ���r
		g_EnemyarmL[i].use = false;			// �ŏ��͖��g�p

		switch (i%ENEMY_MOVEPATTERN)
		{
		case 0:
			g_EnemyarmL[i].movepattern = move_tbl1;
			g_EnemyarmL[i].size = sizeof(move_tbl1) / sizeof(ENEMYTBL);
			break;
		case 1:
			g_EnemyarmL[i].movepattern = move_tbl2;
			g_EnemyarmL[i].size = sizeof(move_tbl2) / sizeof(ENEMYTBL);
			break;
		case 2:
			g_EnemyarmL[i].movepattern = move_tbl3;
			g_EnemyarmL[i].size = sizeof(move_tbl3) / sizeof(ENEMYTBL);
			break;
		case 3:
			g_EnemyarmL[i].movepattern = move_tbl4;
			g_EnemyarmL[i].size = sizeof(move_tbl4) / sizeof(ENEMYTBL);
			break;
		case 4:
			g_EnemyarmL[i].movepattern = move_tbl5;
			g_EnemyarmL[i].size = sizeof(move_tbl5) / sizeof(ENEMYTBL);
			break;
		}

		// �ʒu�E��]�E�X�P�[���̏������ݒ�
		g_EnemyarmL[i].pos = set_tbl_ENEMY[ARM_L].pos;
		g_EnemyarmL[i].rot = set_tbl_ENEMY[ARM_L].rot;
		g_EnemyarmL[i].scl = set_tbl_ENEMY[ARM_L].scl;

		// ��ʒu�E��]�E�X�P�[���̏������ݒ�
		g_EnemyarmL[i].spos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemyarmL[i].srot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemyarmL[i].sscl = D3DXVECTOR3(0.5f, 0.5f, 0.5f);

		g_EnemyarmL[i].move_time = 0.0f;
		g_EnemyarmL[i].back_time = 0.0f;
		g_EnemyarmL[i].spd = 1.0f;
		g_EnemyarmL[i].following = false;
		g_EnemyarmL[i].timestop = false;
		g_EnemyarmL[i].oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemyarmL[i].hit_oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemyarmL[i].oldindex = 0;
		g_EnemyarmL[i].pattern = 1;
		g_EnemyarmL[i].parent = &g_Enemy[i];

		// �E��
		g_EnemylegR[i].use = false;			// �ŏ��͖��g�p

		switch (i%ENEMY_MOVEPATTERN)
		{
		case 0:
			g_EnemylegR[i].movepattern = move_tbl1;
			g_EnemylegR[i].size = sizeof(move_tbl1) / sizeof(ENEMYTBL);
			break;
		case 1:
			g_EnemylegR[i].movepattern = move_tbl2;
			g_EnemylegR[i].size = sizeof(move_tbl2) / sizeof(ENEMYTBL);
			break;
		case 2:
			g_EnemylegR[i].movepattern = move_tbl3;
			g_EnemylegR[i].size = sizeof(move_tbl3) / sizeof(ENEMYTBL);
			break;
		case 3:
			g_EnemylegR[i].movepattern = move_tbl4;
			g_EnemylegR[i].size = sizeof(move_tbl4) / sizeof(ENEMYTBL);
			break;
		case 4:
			g_EnemylegR[i].movepattern = move_tbl5;
			g_EnemylegR[i].size = sizeof(move_tbl5) / sizeof(ENEMYTBL);
			break;
		}

		// �ʒu�E��]�E�X�P�[���̏������ݒ�
		g_EnemylegR[i].pos = set_tbl_ENEMY[LEG_R].pos;
		g_EnemylegR[i].rot = set_tbl_ENEMY[LEG_R].rot;
		g_EnemylegR[i].scl = set_tbl_ENEMY[LEG_R].scl;

		// ��ʒu�E��]�E�X�P�[���̏������ݒ�
		g_EnemylegR[i].spos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemylegR[i].srot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemylegR[i].sscl = D3DXVECTOR3(0.5f, 0.5f, 0.5f);

		g_EnemylegR[i].move_time = 0.0f;
		g_EnemylegR[i].back_time = 0.0f;
		g_EnemylegR[i].spd = 1.0f;
		g_EnemylegR[i].following = false;
		g_EnemylegR[i].timestop = false;
		g_EnemylegR[i].oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemylegR[i].hit_oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemylegR[i].oldindex = 0;
		g_EnemylegR[i].pattern = 1;
		g_EnemylegR[i].parent = &g_Enemy[i];

		// ����
		g_EnemylegL[i].use = false;			// �ŏ��͖��g�p

		switch (i%ENEMY_MOVEPATTERN)
		{
		case 0:
			g_EnemylegL[i].movepattern = move_tbl1;
			g_EnemylegL[i].size = sizeof(move_tbl1) / sizeof(ENEMYTBL);
			break;
		case 1:
			g_EnemylegL[i].movepattern = move_tbl2;
			g_EnemylegL[i].size = sizeof(move_tbl2) / sizeof(ENEMYTBL);
			break;
		case 2:
			g_EnemylegL[i].movepattern = move_tbl3;
			g_EnemylegL[i].size = sizeof(move_tbl3) / sizeof(ENEMYTBL);
			break;
		case 3:
			g_EnemylegL[i].movepattern = move_tbl4;
			g_EnemylegL[i].size = sizeof(move_tbl4) / sizeof(ENEMYTBL);
			break;
		case 4:
			g_EnemylegL[i].movepattern = move_tbl5;
			g_EnemylegL[i].size = sizeof(move_tbl5) / sizeof(ENEMYTBL);
			break;
		}
		// �ʒu�E��]�E�X�P�[���̏������ݒ�
		g_EnemylegL[i].pos = set_tbl_ENEMY[LEG_L].pos;
		g_EnemylegL[i].rot = set_tbl_ENEMY[LEG_L].rot;
		g_EnemylegL[i].scl = set_tbl_ENEMY[LEG_L].scl;

		// ��ʒu�E��]�E�X�P�[���̏������ݒ�
		g_EnemylegL[i].spos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemylegL[i].srot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemylegL[i].sscl = D3DXVECTOR3(0.5f, 0.5f, 0.5f);

		g_EnemylegL[i].spos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemylegL[i].move_time = 0.0f;
		g_EnemylegL[i].back_time = 0.0f;
		g_EnemylegL[i].spd = 2.0f;
		g_EnemylegL[i].following = false;
		g_EnemylegL[i].timestop = false;
		g_EnemylegL[i].oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemylegL[i].hit_oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EnemylegL[i].oldindex = 0;
		g_EnemylegL[i].pattern = 1;
		g_EnemylegL[i].parent = &g_Enemy[i];
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (enemy_pD3DTexture_body != NULL)
		{// �e�N�X�`���̊J��
			enemy_pD3DTexture_body->Release();
			enemy_pD3DTexture_body = NULL;
		}

		if (enemy_pD3DXMesh_body != NULL)
		{// ���b�V���̊J��
			enemy_pD3DXMesh_body->Release();
			enemy_pD3DXMesh_body = NULL;
		}

		if (enemy_pD3DXBuffMat_body != NULL)
		{//�}�e���A���̊J��
			enemy_pD3DXBuffMat_body->Release();
			enemy_pD3DXBuffMat_body = NULL;
		}

		if (enemy_pD3DTexture_head != NULL)
		{// �e�N�X�`���̊J��
			enemy_pD3DTexture_head->Release();
			enemy_pD3DTexture_head = NULL;
		}
		if (enemy_pD3DXMesh_head != NULL)
		{// ���b�V���̊J��
			enemy_pD3DXMesh_head->Release();
			enemy_pD3DXMesh_head = NULL;
		}
		if (enemy_pD3DXBuffMat_head != NULL)
		{// �}�e���A���̊J��
			enemy_pD3DXBuffMat_head->Release();
			enemy_pD3DXBuffMat_head = NULL;
		}

		if (enemy_pD3DTexture_armR != NULL)
		{// �e�N�X�`���̊J��
			enemy_pD3DTexture_armR->Release();
			enemy_pD3DTexture_armR = NULL;
		}
		if (enemy_pD3DXMesh_armR != NULL)
		{// ���b�V���̊J��
			enemy_pD3DXMesh_armR->Release();
			enemy_pD3DXMesh_armR = NULL;
		}
		if (enemy_pD3DXBuffMat_armR != NULL)
		{// �}�e���A���̊J��
			enemy_pD3DXBuffMat_armR->Release();
			enemy_pD3DXBuffMat_armR = NULL;
		}

		if (enemy_pD3DTexture_armL != NULL)
		{// �e�N�X�`���̊J��
			enemy_pD3DTexture_armL->Release();
			enemy_pD3DTexture_armL = NULL;
		}
		if (enemy_pD3DXMesh_armL != NULL)
		{// ���b�V���̊J��
			enemy_pD3DXMesh_armL->Release();
			enemy_pD3DXMesh_armL = NULL;
		}
		if (enemy_pD3DXBuffMat_armL != NULL)
		{// �}�e���A���̊J��
			enemy_pD3DXBuffMat_armL->Release();
			enemy_pD3DXBuffMat_armL = NULL;
		}

		if (enemy_pD3DTexture_legR != NULL)
		{// �e�N�X�`���̊J��
			enemy_pD3DTexture_legR->Release();
			enemy_pD3DTexture_legR = NULL;
		}
		if (enemy_pD3DXMesh_legR != NULL)
		{// ���b�V���̊J��
			enemy_pD3DXMesh_legR->Release();
			enemy_pD3DXMesh_legR = NULL;
		}
		if (enemy_pD3DXBuffMat_legR != NULL)
		{// �}�e���A���̊J��
			enemy_pD3DXBuffMat_legR->Release();
			enemy_pD3DXBuffMat_legR = NULL;
		}

		if (enemy_pD3DTexture_legL != NULL)
		{// �e�N�X�`���̊J��
			enemy_pD3DTexture_legL->Release();
			enemy_pD3DTexture_legL = NULL;
		}
		if (enemy_pD3DXMesh_legL != NULL)
		{// ���b�V���̊J��
			enemy_pD3DXMesh_legL->Release();
			enemy_pD3DXMesh_legL = NULL;
		}
		if (enemy_pD3DXBuffMat_legL != NULL)
		{// �}�e���A���̊J��
			enemy_pD3DXBuffMat_legL->Release();
			enemy_pD3DXBuffMat_legL = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	PLAYER *player = GetPlayer();

	int cnt = 0;

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].status != DEAD)	//�G�l�~�[������ł��Ȃ����
			if (g_Enemy[i].use == true)  //�G�l�~�[������Ȃ�
			{
				if (g_Enemy[i].status != HIT)		// �G�l�~�[���U�����󂯂Ă��Ȃ����
				{
					cnt++;					//�����Ă���G�l�~�[���J�E���g

					switch (g_Enemy[i].pattern) //�G�l�~�[�̏�ԑJ�ڂ�switch
					{
					case 1://���`��ԁi���G�j
							//�ړ�����
					{
						int	index = (int)g_Enemy[i].move_time;			//���`��Ԃ̃e�[�u����index
						float	time = g_Enemy[i].move_time - index;	//���`��Ԃ̏�������(0.0�`0.99....)

						g_Enemy[i].move_time += g_Enemy[i].movepattern[index].dt;	//�A�j���[�V�����̍��v���Ԃɑ���
						if (index > (g_Enemy[i].size - 2))							// �S�[���փI�[�o�[���Ă�����A�S�[���֖߂�
						{
							g_Enemy[i].move_time = 0.0f;
							index = 0;
						}

						// X���W�����߂�	X = StartX + (EndX - StartX) * ���̎���
						D3DXVECTOR3 vec = g_Enemy[i].movepattern[index + 1].pos - g_Enemy[i].movepattern[index].pos;
						g_Enemy[i].pos = g_Enemy[i].movepattern[index].pos + (vec * time) + g_Enemy[i].spos;

						//��]�����߂�@�@�@R = StsrtX + (EndX - startX) * ���̎���
						D3DXVECTOR3 rot = g_Enemy[i].movepattern[index + 1].rot - g_Enemy[i].movepattern[index].rot;
						g_Enemy[i].rot = g_Enemy[i].movepattern[index].rot + (rot * time) + g_Enemy[i].srot;

						//scale�����߂�     S = StsrtX + (EndX - startX) * ���̎���
						D3DXVECTOR3 scl = g_Enemy[i].movepattern[index + 1].scl - g_Enemy[i].movepattern[index].scl;
						g_Enemy[i].scl = g_Enemy[i].movepattern[index].scl + (scl * time) + g_Enemy[i].sscl;

						if (CheckHitFP(player->pos, g_Enemy[i].pos, D3DXVECTOR3(-sinf(g_Enemy[i].rot.y), 0, -cosf(g_Enemy[i].rot.y)), 30, 300, 0) == true)				//�v���C���[����������j���Ԃ��~�߂ēG�̍��W��ۑ�
						{
							g_Enemy[i].timestop = true;						//�Ǐ]��OFF�ɂ���

							g_Enemy[i].oldpos = g_Enemy[i].pos;				//oldpos�ɃG�l�~�[���W��ۑ�

							g_Enemy[i].following = true;					//�Ǐ]��ON�ɂ���

							g_Enemy[i].pattern = 2;							//�Ǐ]�p�^�[���Ɉڂ�
						}
					}
					break;
					case 2://�Ǐ]
					{
						D3DXVECTOR3 pos1 = player->pos - g_Enemy[i].pos;	//�v���C���[�̍��W����G�l�~�[�̍��W�����������̂�pos�P�ɓ����				

						//���G�͈͊O�ɍs������ǂ��̂���߂�
						if (Length(pos1) > ENEMY_SCOUTING)
						{
							g_Enemy[i].pattern = 3;
							break;
						}
						//�߂Â�����߂�
						if ((Length(pos1) < 40) && ((Length(pos1) > 5)))
						{
							g_Enemy[i].attackcnt++;					//�G�l�~�[���U���J�n����܂ł̎��Ԃ��J�E���g
							if (g_Enemy[i].attackcnt > 30)
							{
								g_Enemy[i].status = ATTACK;			//�G�l�~�[���U�����[�h�ɂȂ�
							}
							break;
						}

						//���G�͈͓��̎��͒ǂ�
						if (Length(pos1) <= ENEMY_SCOUTING)
						{
							//�Ǐ]�I�������Ƃ����̍��W�֖߂邽�߂̐��`��ԗp��back_time��������
							g_Enemy[i].back_time = 0.00f;

							//�Ǐ]�����J�n//											
							float rot1 = atan2f(pos1.z, pos1.x);

							rot1 = (D3DX_PI / 2) * 3 - rot1;
							g_Enemy[i].rot.y = rot1;

							pos1.y = 0;
							float len = Length(pos1);

							len = 1 / len;
							pos1.x *= len;
							pos1.z *= len;
							g_Enemy[i].pos += pos1 * g_Enemy[i].spd;
								//�Ǐ]�����I��//
						}
					}
					break;
					case 3://���̍��W�ɖ߂�

						float	back_dt = 0.01f;
						g_Enemy[i].following = false;									//�Ǐ]��OFF

						D3DXVECTOR3 pos2 = g_Enemy[i].oldpos - g_Enemy[i].pos;			//��pos����߂�ꏊ�ւ̋���
						pos2.y = 0;

						g_Enemy[i].pos = g_Enemy[i].pos + pos2 * g_Enemy[i].back_time;	//oldpos�֌����Đ��`���
						g_Enemy[i].back_time += back_dt;								//back_time�X�V

						//�G�l�~�[��oldpos�֌�������
						float rot1 = atan2f(pos2.z, pos2.x);
						rot1 = (D3DX_PI / 2) * 3 - rot1;
						g_Enemy[i].rot.y = rot1;

						if (g_Enemy[i].back_time >= 1.0)								//oldpos�ւ̐��`��Ԃ�����������ԑJ��
						{
							g_Enemy[i].timestop = false;
							g_Enemy[i].pattern = 1;
						}
						break;
					}

					// �G�l�~�[���������[�V����
					if (g_Enemy[i].status == WALK)
					{
						float	dt = 0.03f;
						g_Enemy[i].walkmove_time += dt;		// �A�j���[�V�����̍��v���Ԃɑ���

						int index = (int)g_Enemy[i].walkmove_time;
						float	time = g_Enemy[i].walkmove_time - index;
						int		size = sizeof(Ebody_walk_tbl) / sizeof(D3DXVECTOR3); //�e�[�u���S�̂̃o�C�g�����e�[�u����̃o�C�g���Ŋ���(�e�[�u���������߂�)

						if (index > (size - 2))	// �S�[�����I�[�o�[���Ă�����A�S�[���֖߂�
						{
							g_Enemy[i].walkmove_time = 0.0f;
							index = 0;
						}

						// �A�j���[�V���������镔��	�@X = StartX + (EndX - StartX) * ���̎���
						g_Enemy[i].vec = Ebody_walk_tbl[index + 1] - Ebody_walk_tbl[index];
						g_Enemyhead[i].vec = Ehead_walk_tbl[index + 1] - Ehead_walk_tbl[index];
						g_EnemyarmR[i].vec = EarmR_walk_tbl[index + 1] - EarmR_walk_tbl[index];
						g_EnemyarmL[i].vec = EarmL_walk_tbl[index + 1] - EarmL_walk_tbl[index];
						g_EnemylegR[i].vec = ElegR_walk_tbl[index + 1] - ElegR_walk_tbl[index];
						g_EnemylegL[i].vec = ElegL_walk_tbl[index + 1] - ElegL_walk_tbl[index];

						g_Enemy[i].rot = Ebody_walk_tbl[index] + g_Enemy[i].vec * time;
						g_Enemyhead[i].rot = Ehead_walk_tbl[index] + g_Enemyhead[i].vec * time;
						g_EnemyarmR[i].rot = EarmR_walk_tbl[index] + g_EnemyarmR[i].vec * time;
						g_EnemyarmL[i].rot = EarmL_walk_tbl[index] + g_EnemyarmL[i].vec * time;
						g_EnemylegR[i].rot = ElegR_walk_tbl[index] + g_EnemylegR[i].vec * time;
						g_EnemylegL[i].rot = ElegL_walk_tbl[index] + g_EnemylegL[i].vec * time;
					}

					// �G�l�~�[���U�����Ă���A�j���[�V����
					if (g_Enemy[i].status == ATTACK)
					{
						g_Enemy[i].attack_dt += 0.01f;
						g_Enemy[i].attackmove_time += g_Enemy[i].attack_dt;		// �A�j���[�V�����̍��v���Ԃɑ���

						int a_index = (int)g_Enemy[i].attackmove_time;
						float	a_time = g_Enemy[i].attackmove_time - a_index;
						int		a_size = sizeof(Ebody_Attack_tbl) / sizeof(D3DXVECTOR3); //�e�[�u���S�̂̃o�C�g�����e�[�u����̃o�C�g���Ŋ���(�e�[�u���������߂�)

						if (a_index > (a_size - 2))	// �S�[�����I�[�o�[���Ă�����A�S�[���֖߂�
						{
							g_Enemy[i].status = WALK;
							g_Enemy[i].pattern = 3;
							g_Enemy[i].attackmove_time = 0.0f;
							a_index = 0;
							g_Enemy[i].attackcnt = 0;
						}

						// �A�j���[�V���������镔��	�@X = StartX + (EndX - StartX) * ���̎���
						g_Enemy[i].vec = Ebody_Attack_tbl[a_index + 1] - Ebody_Attack_tbl[a_index];
						g_Enemyhead[i].vec = Ehead_Attack_tbl[a_index + 1] - Ehead_Attack_tbl[a_index];
						g_EnemyarmR[i].vec = EarmR_Attack_tbl[a_index + 1] - EarmR_Attack_tbl[a_index];
						g_EnemyarmL[i].vec = EarmL_Attack_tbl[a_index + 1] - EarmL_Attack_tbl[a_index];
						g_EnemylegR[i].vec = ElegR_Attack_tbl[a_index + 1] - ElegR_Attack_tbl[a_index];
						g_EnemylegL[i].vec = ElegL_Attack_tbl[a_index + 1] - ElegL_Attack_tbl[a_index];

						g_Enemy[i].rot = Ebody_Attack_tbl[a_index] + g_Enemy[i].vec * a_time;
						g_Enemyhead[i].rot = Ehead_Attack_tbl[a_index] + g_Enemyhead[i].vec * a_time;
						g_EnemyarmR[i].rot = EarmR_Attack_tbl[a_index] + g_EnemyarmR[i].vec * a_time;
						g_EnemyarmL[i].rot = EarmL_Attack_tbl[a_index] + g_EnemyarmL[i].vec * a_time;
						g_EnemylegR[i].rot = ElegR_Attack_tbl[a_index] + g_EnemylegR[i].vec * a_time;
						g_EnemylegL[i].rot = ElegL_Attack_tbl[a_index] + g_EnemylegL[i].vec * a_time;

					}
				}
			}
		//�G�l�~�[���U�����󂯂����̃A�j���[�V�����i������ԁj
			else if (g_Enemy[i].status == HIT)
			{
				g_Enemy[i].formation = 10;
				g_Enemy[i].hit_dt += 0.003f;
				g_Enemy[i].hitmove_time += g_Enemy[i].hit_dt;		// �A�j���[�V�����̍��v���Ԃɑ���

				int a_index = (int)g_Enemy[i].hitmove_time;
				float	a_time = g_Enemy[i].hitmove_time - a_index;
				int		a_size = sizeof(Ebody_Hit_tbl) / sizeof(D3DXVECTOR3); //�e�[�u���S�̂̃o�C�g�����e�[�u����̃o�C�g���Ŋ���(�e�[�u���������߂�)

				if (a_index > (a_size - 2))	// �S�[�����I�[�o�[���Ă�����A�S�[���֖߂�
				{
					g_Enemy[i].hitmove_time = 0.0f;
					a_index = 0;
				}

				// �A�j���[�V���������镔��	�@X = StartX + (EndX - StartX) * ���̎���
				g_Enemy[i].vec = Ebody_Hit_tbl[a_index + 1] - Ebody_Hit_tbl[a_index];
				g_Enemyhead[i].vec = Ehead_Hit_tbl[a_index + 1] - Ehead_Hit_tbl[a_index];
				g_EnemyarmR[i].vec = EarmR_Hit_tbl[a_index + 1] - EarmR_Hit_tbl[a_index];
				g_EnemyarmL[i].vec = EarmL_Hit_tbl[a_index + 1] - EarmL_Hit_tbl[a_index];
				g_EnemylegR[i].vec = ElegR_Hit_tbl[a_index + 1] - ElegR_Hit_tbl[a_index];
				g_EnemylegL[i].vec = ElegL_Hit_tbl[a_index + 1] - ElegL_Hit_tbl[a_index];

				g_Enemy[i].rot = Ebody_Hit_tbl[a_index] + g_Enemy[i].vec * a_time;
				g_Enemyhead[i].rot = Ehead_Hit_tbl[a_index] + g_Enemyhead[i].vec * a_time;
				g_EnemyarmR[i].rot = EarmR_Hit_tbl[a_index] + g_EnemyarmR[i].vec * a_time;
				g_EnemyarmL[i].rot = EarmL_Hit_tbl[a_index] + g_EnemyarmL[i].vec * a_time;
				g_EnemylegR[i].rot = ElegR_Hit_tbl[a_index] + g_EnemylegR[i].vec * a_time;
				g_EnemylegL[i].rot = ElegL_Hit_tbl[a_index] + g_EnemylegL[i].vec * a_time;

				if (g_Enemy[i].hit_cnt < g_Enemy[i].hit_cnt_max)
				{
					float rag = D3DX_PI / g_Enemy[i].hit_cnt_max;
					float angle = rag * g_Enemy[i].hit_cnt;
					g_Enemy[i].ofs.x = -sinf(rand() % 100 + 1) * g_Enemy[i].hit;
					g_Enemy[i].ofs.y = sinf(angle) * g_Enemy[i].hit;
					g_Enemy[i].ofs.z = cosf((rand() % 100 + 1)) * g_Enemy[i].hit;
					g_Enemy[i].hit_cnt++;
					g_Enemy[i].pos.x += g_Enemy[i].ofs.x + g_Enemy[i].spd;
					g_Enemy[i].pos.y += g_Enemy[i].ofs.y + g_Enemy[i].spd;
					g_Enemy[i].pos.z += g_Enemy[i].ofs.z + g_Enemy[i].spd;
				}
				if (g_Enemy[i].hit_cnt >= g_Enemy[i].hit_cnt_max)		// ������Ԃ̏I���H
				{														// Yes
					g_Enemy[i].status = DEAD;							//���S
					g_Enemy[i].ofs = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					g_Enemy[i].pos.y = 0.0f;
				}
			}
		for (int j = 0; j < ENEMY_GROUP; j++)			//�e�̐ݒ�
		{
			// �e���v���C���[�̈ʒu�ɍ��킹��
			g_Shadowpos[j] = g_Enemy[i].pos + (formation[j] * j);
			g_Shadowpos[j].y = 0.0f;
			SetPositionShadow(g_Enemy[i].shadowIdx[j], g_Shadowpos[j], g_Enemy[i].scl);
		}
	}

	if (cnt < MAX_ENEMY / 3)							//�G�l�~�[����3����1�����Ȃ�G�l�~�[�𔭐�������
	{
		SetEnemy(MAX_ENEMY, D3DXVECTOR3(-1500, 0, 1500), D3DXVECTOR3(1500, 0, -1500));
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//�g���ĂȂ��Ȃ�R���e�j���[����
		if (g_Enemy[i].status == DEAD) continue;

		for (int j = 0; j < ENEMY_GROUP; j++)			// �ЂƂ����܂蕪��`�揈��
		{
			// �̈ȊO�͐�Ɍv�Z���Ă��܂�
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Enemyhead[i].mtxWorld);
			D3DXMatrixIdentity(&g_EnemyarmR[i].mtxWorld);
			D3DXMatrixIdentity(&g_EnemyarmL[i].mtxWorld);
			D3DXMatrixIdentity(&g_EnemylegR[i].mtxWorld);
			D3DXMatrixIdentity(&g_EnemylegL[i].mtxWorld);

			//�X�P�[���̔��f
			D3DXMatrixScaling(&g_Enemyhead[i].mtxScl, g_Enemyhead[i].scl.x, g_Enemyhead[i].scl.y, g_Enemyhead[i].scl.z);
			D3DXMatrixMultiply(&g_Enemyhead[i].mtxWorld, &g_Enemyhead[i].mtxWorld, &g_Enemyhead[i].mtxScl);
			D3DXMatrixScaling(&g_EnemyarmR[i].mtxScl, g_EnemyarmR[i].scl.x, g_EnemyarmR[i].scl.y, g_EnemyarmR[i].scl.z);
			D3DXMatrixMultiply(&g_EnemyarmR[i].mtxWorld, &g_EnemyarmR[i].mtxWorld, &g_EnemyarmR[i].mtxScl);
			D3DXMatrixScaling(&g_EnemyarmL[i].mtxScl, g_EnemyarmL[i].scl.x, g_EnemyarmL[i].scl.y, g_EnemyarmL[i].scl.z);
			D3DXMatrixMultiply(&g_EnemyarmL[i].mtxWorld, &g_EnemyarmL[i].mtxWorld, &g_EnemyarmL[i].mtxScl);
			D3DXMatrixScaling(&g_EnemylegR[i].mtxScl, g_EnemylegR[i].scl.x, g_EnemylegR[i].scl.y, g_EnemylegR[i].scl.z);
			D3DXMatrixMultiply(&g_EnemylegR[i].mtxWorld, &g_EnemylegR[i].mtxWorld, &g_EnemylegR[i].mtxScl);
			D3DXMatrixScaling(&g_EnemylegL[i].mtxScl, g_EnemylegL[i].scl.x, g_EnemylegL[i].scl.y, g_EnemylegL[i].scl.z);
			D3DXMatrixMultiply(&g_EnemylegL[i].mtxWorld, &g_EnemylegL[i].mtxWorld, &g_EnemylegL[i].mtxScl);

			//��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&g_Enemyhead[i].mtxRot, g_Enemyhead[i].rot.y, g_Enemyhead[i].rot.x, g_Enemyhead[i].rot.z);
			D3DXMatrixMultiply(&g_Enemyhead[i].mtxWorld, &g_Enemyhead[i].mtxWorld, &g_Enemyhead[i].mtxRot);
			D3DXMatrixRotationYawPitchRoll(&g_EnemyarmR[i].mtxRot, g_EnemyarmR[i].rot.y, g_EnemyarmR[i].rot.x, g_EnemyarmR[i].rot.z);
			D3DXMatrixMultiply(&g_EnemyarmR[i].mtxWorld, &g_EnemyarmR[i].mtxWorld, &g_EnemyarmR[i].mtxRot);
			D3DXMatrixRotationYawPitchRoll(&g_EnemyarmL[i].mtxRot, g_EnemyarmL[i].rot.y, g_EnemyarmL[i].rot.x, g_EnemyarmL[i].rot.z);
			D3DXMatrixMultiply(&g_EnemyarmL[i].mtxWorld, &g_EnemyarmL[i].mtxWorld, &g_EnemyarmL[i].mtxRot);
			D3DXMatrixRotationYawPitchRoll(&g_EnemylegR[i].mtxRot, g_EnemylegR[i].rot.y, g_EnemylegR[i].rot.x, g_EnemylegR[i].rot.z);
			D3DXMatrixMultiply(&g_EnemylegR[i].mtxWorld, &g_EnemylegR[i].mtxWorld, &g_EnemylegR[i].mtxRot);
			D3DXMatrixRotationYawPitchRoll(&g_EnemylegL[i].mtxRot, g_EnemylegL[i].rot.y, g_EnemylegL[i].rot.x, g_EnemylegL[i].rot.z);
			D3DXMatrixMultiply(&g_EnemylegL[i].mtxWorld, &g_EnemylegL[i].mtxWorld, &g_EnemylegL[i].mtxRot);

			//�ړ��𔽉f
			D3DXMatrixTranslation(&g_Enemyhead[i].mtxTranslate, g_Enemyhead[i].pos.x, g_Enemyhead[i].pos.y, g_Enemyhead[i].pos.z);
			D3DXMatrixMultiply(&g_Enemyhead[i].mtxWorld, &g_Enemyhead[i].mtxWorld, &g_Enemyhead[i].mtxTranslate);
			D3DXMatrixTranslation(&g_EnemyarmR[i].mtxTranslate, g_EnemyarmR[i].pos.x, g_EnemyarmR[i].pos.y, g_EnemyarmR[i].pos.z);
			D3DXMatrixMultiply(&g_EnemyarmR[i].mtxWorld, &g_EnemyarmR[i].mtxWorld, &g_EnemyarmR[i].mtxTranslate);
			D3DXMatrixTranslation(&g_EnemyarmL[i].mtxTranslate, g_EnemyarmL[i].pos.x, g_EnemyarmL[i].pos.y, g_EnemyarmL[i].pos.z);
			D3DXMatrixMultiply(&g_EnemyarmL[i].mtxWorld, &g_EnemyarmL[i].mtxWorld, &g_EnemyarmL[i].mtxTranslate);
			D3DXMatrixTranslation(&g_EnemylegR[i].mtxTranslate, g_EnemylegR[i].pos.x, g_EnemylegR[i].pos.y, g_EnemylegR[i].pos.z);
			D3DXMatrixMultiply(&g_EnemylegR[i].mtxWorld, &g_EnemylegR[i].mtxWorld, &g_EnemylegR[i].mtxTranslate);
			D3DXMatrixTranslation(&g_EnemylegL[i].mtxTranslate, g_EnemylegL[i].pos.x, g_EnemylegL[i].pos.y, g_EnemylegL[i].pos.z);
			D3DXMatrixMultiply(&g_EnemylegL[i].mtxWorld, &g_EnemylegL[i].mtxWorld, &g_EnemylegL[i].mtxTranslate);

			//��
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Enemy[i].mtxWorld);

			//�X�P�[���̔��f
			D3DXMatrixScaling(&g_Enemy[i].mtxScl, g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
			D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &g_Enemy[i].mtxScl);

			//��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&g_Enemy[i].mtxRot, g_Enemy[i].rot.y, g_Enemy[i].rot.x, g_Enemy[i].rot.z);
			D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &g_Enemy[i].mtxRot);

			//�ړ��𔽉f�i�C���X�^���V���O�j
			D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			switch (g_Enemy[i].formation)
			{
			case 0:
				pos = g_Enemy[i].pos + (formation[j] * j);
				D3DXMatrixTranslation(&g_Enemy[i].mtxTranslate, pos.x, pos.y, pos.z);
				D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &g_Enemy[i].mtxTranslate);
				break;

			case 1:
				pos = g_Enemy[i].pos + (formation[g_Enemy[i].formation] * j);
				D3DXMatrixTranslation(&g_Enemy[i].mtxTranslate, pos.x, pos.y, pos.z);
				D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &g_Enemy[i].mtxTranslate);
				break;

			case 2:
				pos = g_Enemy[i].pos + (formation[g_Enemy[i].formation] * j);
				D3DXMatrixTranslation(&g_Enemy[i].mtxTranslate, pos.x, pos.y, pos.z);
				D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &g_Enemy[i].mtxTranslate);
				break;

			case 3:
				pos = g_Enemy[i].pos + (formation[g_Enemy[i].formation] * j);
				D3DXMatrixTranslation(&g_Enemy[i].mtxTranslate, pos.x, pos.y, pos.z);
				D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &g_Enemy[i].mtxTranslate);
				break;

			case 4:
				pos = g_Enemy[i].pos + (formation[g_Enemy[i].formation] * j);
				D3DXMatrixTranslation(&g_Enemy[i].mtxTranslate, pos.x, pos.y, pos.z);
				D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &g_Enemy[i].mtxTranslate);
				break;

			case 10:
				switch (j)
				{
				case 0:
					pos = g_Enemy[i].pos + (formation1[j] * j);
					D3DXMatrixTranslation(&g_Enemy[i].mtxTranslate, pos.x, pos.y, pos.z);
					D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &g_Enemy[i].mtxTranslate);
					break;
				case 1:
					pos = g_Enemy[i].pos + (formation1[j] * j);
					D3DXMatrixTranslation(&g_Enemy[i].mtxTranslate, pos.x, pos.y, pos.z);
					D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &g_Enemy[i].mtxTranslate);
					break;
				case 2:
					pos = g_Enemy[i].pos + (formation1[j] * j);
					D3DXMatrixTranslation(&g_Enemy[i].mtxTranslate, pos.x, pos.y, pos.z);
					D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &g_Enemy[i].mtxTranslate);
					break;
				case 3:
					pos = g_Enemy[i].pos + (formation1[j] * j);
					D3DXMatrixTranslation(&g_Enemy[i].mtxTranslate, pos.x, pos.y, pos.z);
					D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &g_Enemy[i].mtxTranslate);
					break;
				case 4:
					pos = g_Enemy[i].pos + (formation1[j] * j);
					D3DXMatrixTranslation(&g_Enemy[i].mtxTranslate, pos.x, pos.y, pos.z);
					D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &g_Enemy[i].mtxTranslate);
					break;
				case 5:
					pos = g_Enemy[i].pos + (formation1[j] * j);
					D3DXMatrixTranslation(&g_Enemy[i].mtxTranslate, pos.x, pos.y, pos.z);
					D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &g_Enemy[i].mtxTranslate);
					break;
				}
			}

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_Enemy[i].mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&g_Enemy[i].matDef);

			// �}�e���A�����ɑ΂���|�C���^���擾
			g_Enemy[i].pD3DXMat = (D3DXMATERIAL*)enemy_pD3DXBuffMat_body->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)enemy_nNumMat_body; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&g_Enemy[i].pD3DXMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, enemy_pD3DTexture_body);

				//�`��
				enemy_pD3DXMesh_body->DrawSubset(nCntMat);
			}

			//�}�e���A�����f�t�H�����ɖ߂�
			pDevice->SetMaterial(&g_Enemy[i].matDef);

			//��
			//�q��world = �q��world * �e��world
			D3DXMatrixMultiply(&g_Enemyhead[i].mtxWorld, &g_Enemyhead[i].mtxWorld, &g_Enemyhead[i].parent->mtxWorld);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_Enemyhead[i].mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&g_Enemyhead[i].matDef);

			// �}�e���A�����ɑ΂���|�C���^���擾
			g_Enemyhead[i].pD3DXMat = (D3DXMATERIAL*)enemy_pD3DXBuffMat_head->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)enemy_nNumMat_head; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&g_Enemyhead[i].pD3DXMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, enemy_pD3DTexture_head);

				//�`��
				enemy_pD3DXMesh_head->DrawSubset(nCntMat);
			}

			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&g_Enemyhead[i].matDef);

			//�E�r
			//�q��world = �q��world * �e��world
			D3DXMatrixMultiply(&g_EnemyarmR[i].mtxWorld, &g_EnemyarmR[i].mtxWorld, &g_EnemyarmR[i].parent->mtxWorld);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_EnemyarmR[i].mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&g_EnemyarmR[i].matDef);

			// �}�e���A�����ɑ΂���|�C���^���擾
			g_EnemyarmR[i].pD3DXMat = (D3DXMATERIAL*)enemy_pD3DXBuffMat_armR->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)enemy_nNumMat_armR; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&g_EnemyarmR[i].pD3DXMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, enemy_pD3DTexture_armR);

				//�`��
				enemy_pD3DXMesh_armR->DrawSubset(nCntMat);
			}

			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&g_EnemyarmR[i].matDef);

			//���r
			//�q��world = �q��world * �e��world
			D3DXMatrixMultiply(&g_EnemyarmL[i].mtxWorld, &g_EnemyarmL[i].mtxWorld, &g_EnemyarmL[i].parent->mtxWorld);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_EnemyarmL[i].mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&g_EnemyarmL[i].matDef);

			// �}�e���A�����ɑ΂���|�C���^���擾
			g_EnemyarmL[i].pD3DXMat = (D3DXMATERIAL*)enemy_pD3DXBuffMat_armL->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)enemy_nNumMat_armL; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&g_EnemyarmL[i].pD3DXMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, enemy_pD3DTexture_armL);

				//�`��
				enemy_pD3DXMesh_armL->DrawSubset(nCntMat);
			}

			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&g_EnemyarmL[i].matDef);

			//�E�r
			//�q��world = �q��world * �e��world
			D3DXMatrixMultiply(&g_EnemylegR[i].mtxWorld, &g_EnemylegR[i].mtxWorld, &g_EnemylegR[i].parent->mtxWorld);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_EnemylegR[i].mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&g_EnemylegR[i].matDef);

			// �}�e���A�����ɑ΂���|�C���^���擾
			g_EnemylegR[i].pD3DXMat = (D3DXMATERIAL*)enemy_pD3DXBuffMat_legR->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)enemy_nNumMat_legR; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&g_EnemylegR[i].pD3DXMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, enemy_pD3DTexture_legR);

				//�`��
				enemy_pD3DXMesh_legR->DrawSubset(nCntMat);
			}

			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&g_EnemylegR[i].matDef);

			//����
			//�q��world = �q��world * �e��world
			D3DXMatrixMultiply(&g_EnemylegL[i].mtxWorld, &g_EnemylegL[i].mtxWorld, &g_EnemylegL[i].parent->mtxWorld);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_EnemylegL[i].mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&g_EnemylegL[i].matDef);

			// �}�e���A�����ɑ΂���|�C���^���擾
			g_EnemylegL[i].pD3DXMat = (D3DXMATERIAL*)enemy_pD3DXBuffMat_legL->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)enemy_nNumMat_legL; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&g_EnemylegL[i].pD3DXMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, enemy_pD3DTexture_legL);

				//�`��
				enemy_pD3DXMesh_legL->DrawSubset(nCntMat);
			}

			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&g_EnemylegL[i].matDef);
		}
	}
}

//=============================================================================
//�o�g���G�l�~�[�̎擾
//=============================================================================
ENEMY *GetEnemy()
{
	return &(g_Enemy[0]);
}

//=============================================================================
// �o�g���G�l�~�[��z�u
//=============================================================================
void SetEnemy(int enemy_spawn, D3DXVECTOR3 upperleft, D3DXVECTOR3 bottomright)
{
	ITEM *item = GetItem();

	for (int a = 0; a < enemy_spawn; a++)
	{
		for (int i = 0; i < MAX_ENEMY; i++)
		{
			if (g_Enemy[i].use == false)
			{
				g_Enemy[i].use = true;						// �G�l�~�[���g�p���ɂ���
				g_Enemy[i].status = WALK;					// �G�l�~�[�̃X�e�[�^�X��������
				RefreshEnemy(i, upperleft, bottomright);	// �G�l�~�[�̔z�u

				if ((item + i)->bUse == true)				// �A�C�e�����g�p���Ȃ�
				{
					DeleteItem(i);							// �����Ă��܂�

					for (int j = 0; j < MAX_PARTICLE; j++)
					{
						GetParticle(i, j)->bUse = false;	// �p�[�e�B�N���I��
					}
				}
				break;
			}
		}
	}
	return;
}
//=============================================================================
// �o�g���G�l�~�[���Ĕz�u
// reflesh_enemy	: �Ĕz�u���鐔
// upperleft		: �o������ʒu�i����j
// bottomright		: �o������ʒu�i�E���j
//=============================================================================
void RefreshEnemy(int refresh_enemy, D3DXVECTOR3 upperleft, D3DXVECTOR3 bottomright)
{
	g_Enemy[refresh_enemy].spos.x = (rand() % (int)((bottomright.x - upperleft.x + 1)) + upperleft.x);
	g_Enemy[refresh_enemy].spos.z = (rand() % (int)((upperleft.z - bottomright.z + 1)) + bottomright.z);
}



//=============================================================================
//
// ���C������ [main.h]
// Author : �_�� ���t
//
//=============================================================================
#pragma once

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "windows.h"
#include "d3dx9.h"

#define DIRECTINPUT_VERSION (0x0800)	// �x���΍�
#include "dinput.h"
#include "xaudio2.h"					// XAudio2�֘A�̃w�b�_�[

//*****************************************************************************
// ���C�u�����̃����N
//*****************************************************************************
#if 1	// [������"0"�ɂ����ꍇ�A"�\���v���p�e�B" -> "�����J" -> "����" -> "�ǉ��̈ˑ��t�@�C��"�ɑΏۃ��C�u������ݒ肷��]
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")
#endif

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �Q�c�|���S�����_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// �R�c�|���S�����_�t�H�[�}�b�g( ���_���W[3D] / �@�� / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#define SCREEN_WIDTH	(800)				// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(600)				// �E�C���h�E�̍���
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// �E�C���h�E�̒��S�w���W
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// �E�C���h�E�̒��S�x���W

#define	NUM_VERTEX		(4)		// ���_��
#define	NUM_POLYGON		(2)		// �|���S����

#define LR					(2)				//���E
#define R					(0)				//�E
#define L					(1)				//��

// ��L�Q�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 vtx;		// ���_���W
	float		rhw;		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	D3DCOLOR	diffuse;	// ���ˌ�
	D3DXVECTOR2 tex;		// �e�N�X�`�����W
} VERTEX_2D;

// ��L�R�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 vtx;		// ���_���W
	D3DXVECTOR3 nor;		// �@���x�N�g��
	D3DCOLOR	diffuse;	// ���ˌ�
	D3DXVECTOR2 tex;		// �e�N�X�`�����W
} VERTEX_3D;

//�p�[�c�̏����l�Z�b�g�p�\���̒�`
typedef struct
{
	D3DXVECTOR3	pos;
	D3DXVECTOR3	rot;
	D3DXVECTOR3	scl;

} PLAYER_SET;

//�p�[�c�̏����l�Z�b�g�p�\���̒�`
typedef struct
{
	D3DXVECTOR3	pos;
	D3DXVECTOR3	rot;
	D3DXVECTOR3	scl;

} ENEMY_SET;

enum E_PARTS //�p�[�c��
{
	BODY,
	HEAD,
	ARM_R,
	ARM_L,
	LEG_R,
	LEG_L,
	PARTS_MAX
};

// ��ʑJ�ڒ萔
typedef enum
{
	STOP,
	WALK,
	JUMP,
	ATTACK,
	HIT,
	DEAD,
}STATUS;

// ��ʑJ�ڒ萔
enum E_SCENE
{
	SCENE_TITLE,
	SCENE_TUTORIAL,
	SCENE_GAME,
	SCENE_RETRY,
	SCENE_RESULT,
	SCENE_MAX
};

//*************************************
// ���[�h�̎��
//*************************************
typedef enum
{
	MODE_TITLE = 0,			// �^�C�g�����
	MODE_TUTORIAL,
	MODE_GAME,				// �Q�[�����
	MODE_RESULT,			// ���U���g���
	MODE_MAX
} MODE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);
void SetScene(int Scene);
//void InitGame(void);
void SetMode(MODE mode);
MODE GetMode(void);


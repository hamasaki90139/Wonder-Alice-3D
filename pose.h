//=============================================================================
//
// �|�[�Y��ʏ��� [pose.h]
// Author : �_�菬�t
//
//=============================================================================
#pragma once

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_POSE_TITLE		("data/TEXTURE/��pose_001.png")		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_POSE_RETURN		("data/TEXTURE/��pose_002.png")		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_POSE_BG			("data/TEXTURE/��titlebg_002.png")		// �ǂݍ��ރe�N�X�`���t�@�C����

#define	RETRY_POS_X			(160)								// ���g���C�̕\���ʒu
#define	RETRY_POS_Y			(200)								// ���g���C�̕\���ʒu
#define	RETRYMENU_SIZE_X	(500)							// �^�C�g���̕�
#define	RETRYMENU_SIZE_Y	(120)							// �^�C�g���̍���
#define	RETURN_POS_X		(160)							// �^�C�g���̕\���ʒu
#define RETURN_POS_Y		(350)							// �^�C�g���̕\���ʒu
//#define	TITLE02_SIZE_X			(550)							// �^�C�g���̕�
//#define	TITLE02_SIZE_Y			(550)							// �^�C�g���̍���
//
//#define	TITLELOGO_POS_X			(15)							// �^�C�g�����S�̕\���ʒu
//#define	TITLELOGO_POS_Y			(80)							// �^�C�g�����S�̕\���ʒu
//#define	TITLELOGO_SIZE_X		(400)							// �^�C�g�����S�̕�
//#define	TITLELOGO_SIZE_Y		(80)							// �^�C�g�����S�̍���
//#define	TITLE_NEWGAME_POS_X		(190)							// �^�C�g�����S�̕\���ʒu
//#define	TITLE_NEWGAME_POS_Y		(250)							// �^�C�g�����S�̕\���ʒu
//#define	TITLE_EXIT_POS_X		(190)							// �^�C�g�����S�̕\���ʒu
//#define	TITLE_EXIT_POS_Y		(350)							// �^�C�g�����S�̕\���ʒu
//#define	TITLE_MENU_SIZE_X		(250)							// �^�C�g�����S�̕�
//#define	TITLE_MENU_SIZE_Y		(60)							// �^�C�g�����S�̍���
//
//#define MENU_MAX				(2)								//���j���[��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPose(int type);
void UninitPose(void);
void UpdatePose(void);
void DrawPose(void);
bool GetPose(void);

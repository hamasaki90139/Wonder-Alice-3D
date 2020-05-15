//=============================================================================
//
// �^�C�g����ʏ��� [title.h]
// Author : �_�� ���t
//
//=============================================================================
#pragma once

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_TITLE01			("data/TEXTURE/title.png")				// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TITLE02			("data/TEXTURE/��titlealice.png")		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TITLE_LOGO		("data/TEXTURE/��title_001.png")		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TITLE_NEWGAME	("data/TEXTURE/��newgame_001.png")		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TITLE_EXIT		("data/TEXTURE/��exit_001.png")			// �ǂݍ��ރe�N�X�`���t�@�C����

#define	TITLE01_POS_X			(0)								// �^�C�g���̕\���ʒu
#define	TITLE01_POS_Y			(0)								// �^�C�g���̕\���ʒu
#define	TITLE01_SIZE_X			(800)							// �^�C�g���̕�
#define	TITLE01_SIZE_Y			(600)							// �^�C�g���̍���
#define	TITLE02_POS_X			(80)							// �^�C�g���̕\���ʒu
#define	TITLE02_POS_Y			(-30)							// �^�C�g���̕\���ʒu
#define	TITLE02_SIZE_X			(700)							// �^�C�g���̕�
#define	TITLE02_SIZE_Y			(700)							// �^�C�g���̍���

#define	TITLELOGO_POS_X			(15)							// �^�C�g�����S�̕\���ʒu
#define	TITLELOGO_POS_Y			(80)							// �^�C�g�����S�̕\���ʒu
#define	TITLELOGO_SIZE_X		(400)							// �^�C�g�����S�̕�
#define	TITLELOGO_SIZE_Y		(80)							// �^�C�g�����S�̍���
#define	TITLE_NEWGAME_POS_X		(190)							// �^�C�g�����S�̕\���ʒu
#define	TITLE_NEWGAME_POS_Y		(250)							// �^�C�g�����S�̕\���ʒu
#define	TITLE_EXIT_POS_X		(190)							// �^�C�g�����S�̕\���ʒu
#define	TITLE_EXIT_POS_Y		(350)							// �^�C�g�����S�̕\���ʒu
#define	TITLE_MENU_SIZE_X		(250)							// �^�C�g�����S�̕�
#define	TITLE_MENU_SIZE_Y		(60)							// �^�C�g�����S�̍���

#define	TITLE_SKY_POS_X			(0)							// �^�C�g���̕\���ʒu
#define	TITLE_SKY_POS_Y			(0)							// �^�C�g���̕\���ʒu
#define	TITLE_SKY_SIZE_X			(2560)							// �^�C�g���̕�
#define	TITLE_SKY_SIZE_Y			(480)							// �^�C�g���̍���

#define MENU_MAX				(2)								//���j���[��
#define NUM_BG					(2)		//�w�i�̐�

// ��ʑJ�ڒ萔
typedef enum
{
	NEWGAME,
	EXIT,
}TITLEMENU;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTitle(int type);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);


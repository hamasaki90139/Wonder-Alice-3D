//=============================================================================
//
// �p�[�e�B�N������ [particle.h]
// Author : �_�� ���t
//
//=============================================================================
#pragma once

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_PARTICLE			"data/TEXTURE/effect000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	PARTICLE_SIZE_X				(50.0f)							// �r���{�[�h�̕�
#define	PARTICLE_SIZE_Y				(50.0f)							// �r���{�[�h�̍���
#define	VALUE_MOVE_PARTICLE			(20.0f)							// �ړ����x
#define	MAX_PARTICLE				(512)							// �r���{�[�h�ő吔

#define	DISP_SHADOW													// �e�̕\��
//#undef DISP_SHADOW

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ��]
	D3DXVECTOR3 scale;		// �X�P�[��
	D3DXVECTOR3 move;		// �ړ���
	D3DXCOLOR col;			// �F
	float fSizeX;			// ��
	float fSizeY;			// ����
	int nIdxShadow;			// �eID
	float nLife;				// ����
	bool bUse;				// �g�p���Ă��邩�ǂ���
	int cnt;
} PARTICLE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);

int SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nLife);
PARTICLE *GetParticle(int enemyNo, int particleNo);
D3DXVECTOR3 GetParticlePos(int enemyNo, int particleNo);
D3DXVECTOR3 GetParticleMove(int enemyNo, int particleNo);
int GetTime(int enemyNo);
float GetSize(int enemyNo);
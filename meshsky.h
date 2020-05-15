//=============================================================================
//
// ���b�V����̏��� [meshsky.h]
// Author : �_�� ���t
//
//=============================================================================
#pragma once

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_SKY	"data/TEXTURE/sky000.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	VALUE_MOVE_SKY		(5.0f)							// �ړ����x
#define	VALUE_ROTATE_SKY	(D3DX_PI * 0.001f)				// ��]���x

#define	SKY_HEIGHT_RATE		(2.0f)		// ��h�[���̍����W��

#define	TEX_COUNT_LOOP		(2)			// �e�N�X�`���̌J��Ԃ���

#define	MAX_MESH_SKY		(2)			// ��̑���

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMeshSky(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
	int nNumBlockX, int nNumBlockZ,
	float fRadius, float fRotY, int nTexType = -1, bool bReverse = false);
void UninitMeshSky(void);
void UpdateMeshSky(void);
void DrawMeshSky(void);


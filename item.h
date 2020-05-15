//=============================================================================
//
// �A�C�e������ [item.h]
// Author : �_�� ���t
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"
#include "enemy.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_ITEM				(MAX_ENEMY)						// �A�C�e���ő吔
#define ITEM_R					(20)							//�A�C�e���̔��a

//**************************************
// �A�C�e���̃^�C�v
//**************************************
enum ITEM_TYPE
{
	ITEM_CAKE,
	ITEM_COOKIE,
	ITEM_TEA,
	ITEM_TYPE_MAX
};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// ���݂̈ʒu
	D3DXVECTOR3 rot;		// ���݂̌���
	D3DXVECTOR3 scl;		// ���݂̑傫��
	float fRadius;			// ���a
	int nIdxShadow;			// �eID
	bool bUse;				// �g�p���Ă��邩�ǂ���
	int type;				//�A�C�e���^�C�v

	LPDIRECT3DTEXTURE9	pD3DTexture;		// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH			pD3DXMesh;			// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER		pD3DXBuffMat;		// �}�e���A�����ւ̃|�C���^
	DWORD				nNumMat;			// �}�e���A�����̐�
	VERTEX_2D			vertexWk[NUM_VERTEX];		// ���_���i�[���[�N
	D3DXMATRIX			mtxWorld;			// ���[���h�}�g���b�N�X

} ITEM;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitItem(int type);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void DeleteItem(int nIdxItem);
ITEM *GetItem(void);

#endif

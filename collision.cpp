//=============================================================================
//
// �Փ˔��菈�� [collision.cpp]
// Author : �_�� ���t
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "item.h"
#include "debugproc.h"
#include "collision.h"
#include "shadow.h"
#include "fade.h"
#include "score.h"
#include "particle.h"
#include "effect.h"
#include "time.h"
#include "life.h"
#include "sound.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//=============================================================================
// ���C���֐�
//=============================================================================

//=============================================================================
// �v���V�[�W��
//=============================================================================

//=============================================================================
// 0�`1�̊ԂɃN�����v�i�n�_���I�_�Ɋ񂹂�j
// t : �x�N�g���W��
//=============================================================================
void Clamp(float &t)
{
	if (t < 0.0f)		//�x�N�g���W�����n�_���O����������
		t = 0.0f;		//�x�N�g���W�����n�_�ɖ߂�
	else if (t > 1.0f)	//�x�N�g���W�����I�_���O����������
		t = 1.0f;		//�x�N�g���W�����I�_�ɖ߂�
}

//=============================================================================
// p1p2p3�͉s�p�H
// p1 : �_
// p2 : �����̎n�_
// p3 : �����̏I�_
//=============================================================================
bool SharpAngle(D3DXVECTOR3 &p1, D3DXVECTOR3 &p2, D3DXVECTOR3 &p3)
{
	D3DXVECTOR3 p1p2 = p2 - p1;		//�x�N�g��p1p2
	D3DXVECTOR3 p2p3 = p3 - p2;		//�x�N�g��p2p3

	float sita = Dot(p1p2, p2p3);	//�Ƃ̒l�����߂�

	if (sita < 0)					//sita���}�C�i�X�Ȃ�݊p
	{
		return false;
	}
	return true;					//sita���v���X�Ȃ�s�p
}

//=============================================================================
// �_�ƒ����̍ŒZ����
// p1:�_
// l1:������̓_1
// v1:�����̃x�N�g��
// h :�ŒZ�����i�߂�l�j
//=============================================================================
float DistancePL(D3DXVECTOR3 &p1, D3DXVECTOR3 &l1, D3DXVECTOR3 &v1)
{
	float len = LengthSq(v1);						//�����̕����x�N�g���̑傫�������߂�
	float t = 0.0f;									//�x�N�g���W���̏�����
	if (len > 0.0f)									//�����̕����x�N�g���̑傫����0�ȏ�Ȃ�
	{	//�x�N�g���W�������߂�i(�����ƒ�����̓_1�`�_�̓���)/�����x�N�g���̑傫��)
		t = Dot(v1, p1 - l1) / len;
	}
	//�ŒZ����=�����̒���h-�_p1 = ( l1 + t * v1 ) - p1�i�[���x�N�g���̏ꍇ��l1-p1�̋���)
	return Length((l1 + t * v1) - p1);				//�ŒZ������Ԃ�
}

//=============================================================================
// �_�Ɛ����̍ŒZ����
// s1s:�����̎n�_1
// s1g:�����̏I�_2
// p1 :�_
// h  :�ŒZ����(�߂�l)
//=============================================================================
float DistancePS(D3DXVECTOR3 &s1s, D3DXVECTOR3 &s1g, D3DXVECTOR3 &p1)
{
	D3DXVECTOR3 v1 = s1g - s1s;				//s1s2�x�N�g��(�����̃x�N�g��)
	D3DXVECTOR3 v2 = p1 - s1s;				//p3s1�x�N�g��
	D3DXVECTOR3 v3 = p1 - s1g;				//p3s2�x�N�g��

	D3DXVECTOR3 v1org = v1;					//��Uv1��ۑ�
	Nomalise(v1);							//v1�𐳋K��

	float t = Dot(v1, v2) / Length(v1);		//�x�N�g���W�������߂�
	if (t < 0)								//�_p1��s1s���̊O���ɂ���ꍇ
	{
		return Length(v2);					//�ŒZ����=p1�`s1s�̃x�N�g���̒���
	}
	else if (t > 1)							//�_p1��s1g���̊O���ɂ���ꍇ
	{
		return Length(v3);					//�ŒZ����=p1�`s1g�̃x�N�g���̒���
	}
	else
	{										//�_p1�������̏�ɂ���ꍇ
		return Length(v1org * t - v2);		//�ŒZ����=�_p1��������ɉ���������
	}
}

//=============================================================================
// 2�����̍ŒZ����
// l1 : ����1��̓_
// v1 : ����1�̃x�N�g��
// l2 : ����2��̓_
// v2 : ����2�̃x�N�g��
// t1 : ����1�̃x�N�g���W��
// t2 : ����2�̃x�N�g���W��
// p1 : ����1�̐����̑��̓_
// p2 : ����2�̐����̑��̓_
// �߂�l:�ŒZ����
//=============================================================================
float DistanceLL(D3DXVECTOR3 &l1, D3DXVECTOR3 &v1, D3DXVECTOR3 &l2, D3DXVECTOR3 &v2)
{
	// 2���������s?
	if (Parallel(v1, v2) == true)
	{
		// l1��̓_�ƒ���l2�̋��������߂ĕԂ�
		float len = DistancePL(l1, l2, v2);
		return len;
	}

	// 2�����͕��s�łȂ�
	float dotl1l2 = Dot(v1, v2);							//����1��l2�̓���
	float lenl1 = LengthSq(v1);								//����1�ׂ̂���̒���
	float lenl2 = LengthSq(v2);								//����2�ׂ̂���̒���
	D3DXVECTOR3 vl1l2 = l1 - l2;							//����1��̓_�ƒ���2��̓_�����񂾃x�N�g��

	float t1 = (dotl1l2 * Dot(v2, vl1l2) - lenl2 * Dot(v1, vl1l2)) /
		(lenl1 * lenl2 - dotl1l2 * dotl1l2);			//����1�̃x�N�g���W�������߂�
	D3DXVECTOR3 p1 = GetPoint(l1, t1, v1);					//����1�̐����̑��̓_�����߂�
	float t2 = Dot(v2, p1 - l2) / lenl2;					//����2�̃x�N�g���W�������߂�
	D3DXVECTOR3 p2 = GetPoint(l2, t2, v1);					//����2�̐����̑��̓_�����߂�

	return Length(p2 - p1);									//�����̑��̓_���m�̋���(�ŒZ����)��Ԃ�
}

//=============================================================================
// 2�����̍ŒZ����
// s1s : S1(����1)�̎n�_
// s1g : S1(����1)�̏I�_
// s2s : S2(����2)�̎n�_
// s2g : S2(����2)�̏I�_
// p1�@: ����1�̐����̑��̓_
// p2�@: ����2�̐����̑��̓_
// t1�@: ����1�̃x�N�g���W��
// t2�@: ����2�̃x�N�g���W��
// �߂�l�F�ŒZ����
//=============================================================================
float DistanceSS(D3DXVECTOR3 &s1s, D3DXVECTOR3 &s1g, D3DXVECTOR3 &s2s, D3DXVECTOR3 &s2g)
{
	D3DXVECTOR3 v1 = s1g - s1s;										//s1�x�N�g��(�����̃x�N�g��)
	D3DXVECTOR3 v2 = s2g - s2s;										//s2�x�N�g��(�����̃x�N�g��)

	float t1 = VectorT(v1, v2);										//����1�̃x�N�g���W��
	float t2 = VectorT(v2, v1);										//����2�̃x�N�g���W��

	//s1���덷�ȉ��̒���)��?
	if (LengthSq(v1) < EPSILON)										//s1�̃x�N�g�����덷���Z�����
	{
		//s2���덷�ȉ��̒�����?
		if (LengthSq(v2) < EPSILON)									//s1s2�̃x�N�g�����Ƃ��Ɍ덷���Z�����
		{
			float len = Length(s2s - s1s);							//�ŒZ����=s1�̎n�_�`s2�̎n�_
			D3DXVECTOR3 p1 = s1s;									//����1�̑���s1�̎n�_�ɐݒ�
			D3DXVECTOR3 p2 = s2s;									//����2�̑���s2�̎n�_�ɐݒ�
			t1 = t2 = 0.0f;											//s1��s2���k�ނ��Ă���̂ŁA�x�N�g���W����0
			return len;												//�ŒZ������Ԃ�
		}
		//s1�̂݌덷�ȉ��̒����̂Ƃ�
		else
		{
			float len = DistancePS(s2s, s2g, s1s);					//�ŒZ����=s1�̎n�_�`s2��̓_
			D3DXVECTOR3 p1 = s1s;									//����1�̑���s1�̎n�_�ɐݒ�	
			t1 = 0.0f;												//s1�̃x�N�g���W����0
			Clamp(t2);												//s2�̃x�N�g���W����0.0�`1.0�Ɏ��߂�
			return len;												//�ŒZ������Ԃ�
		}
	}

	//s2���덷�ȉ��̒�����?
	else if (LengthSq(v2) < EPSILON)								//s2�̃x�N�g�����덷���Z�����
	{
		float len = DistancePS(s1s, s1g, s2s);						//�ŒZ����=s2�̎n�_�`�����ƂȂ�s1��̓_
		D3DXVECTOR3 p2 = s2s;										//����2�̑���s2�̎n�_�ɐݒ�
		t2 = 0.0f;													//s2�̃x�N�g���W����0
		Clamp(t1);													//s1�̃x�N�g���W����0.0�`1.0�Ɏ��߂�
		return len;													//�ŒZ������Ԃ�
	}

	/* �������m */
	if (Parallel(v1, v2) == true)									//2���������s��������
	{//S1�̎n�_�𐂐��̑�p1�ɐݒ肷��
		t1 = 0.0f;													//s1�̃x�N�g���W����0�ɂ���
		D3DXVECTOR3 p1 = s1s;										//����1�̑���s1�̎n�_�ɐݒ�
		float len = DistancePS(s2s, s2g, p1);						//�ŒZ����=s1�̎n�_�`�����ƂȂ�s2��̓_
		if (0.0f <= t2 && t2 <= 1.0f)								//�����ƂȂ�_��s2��ɂ����
		{
			return len;												//�ŒZ������Ԃ�
		}
	}
	else
	{
		//�����͂˂���̊֌W�Ȃ̂ŁA2�����Ԃ̍ŒZ���������߂ĉ���t1,t2�����߂�
		float len = DistanceLL(s1s, v1, s2s, v2);					//�ŒZ����=s1s2�̐����̑��̓_���m�̋���
		if (0.0f <= t1 && t1 <= 1.0f &&0.0f <= t2 && t2 <= 1.0f)	//�ŒZ���������ԓ_��s1s2�Ƃ��ɐ�����ɂ����
		{
			return len;												//�ŒZ������Ԃ�
		}
	}

	//�ǂ��炩�̐����̑��̓_�������̊O
	Clamp(t1);														//s1����t1��0�`1�̊ԂɃN�����v
	D3DXVECTOR3 p1 = GetPoint(s1s, t1, v1);							//s1��̔C�ӂ̓_��p1�ɐݒ�
	float lenss = DistancePS(s2s, s2g, p1);							//�ŒZ����=s1��̓_�`�����̑��ƂȂ�s2��̓_
	if (0.0f <= t2 && t2 <= 1.0f)									//�����̑���s2��ɂ����
	{
		return lenss;												//�ŒZ������Ԃ�
	}

	//s2���̐����̓_�������̊O
	Clamp(t2);														//s2����t2��0�`1�̊ԂɃN�����v
	D3DXVECTOR3 p2 = GetPoint(s2s, t2, v2);							//s2��̔C�ӂ̓_��p2�ɐݒ�
	lenss = DistancePS(s1s, s1g, p2);								//�ŒZ����=s2��̓_�`�����̑��ƂȂ�s1��̓_
	if (0.0f <= t1 && t1 <= 1.0f)									//�����̑���s1��ɂ����
	{
		return lenss;												//�ŒZ������Ԃ�
	}

	//�ŒZ����=s2�̒[�_�`s1�̒[�_
	Clamp(t1);														//s1����t1��0�`1�̊ԂɃN�����v
	p1 = GetPoint(s1s, t1, v1);										//s1��̔C�ӂ̓_��p1�ɐݒ�
	return Length(p2 - p1);											//�ŒZ������Ԃ�
}

//=============================================================================
// �J�v�Z�����m�̏Փ˔���
// s1s : S1(����1)�̎n�_
// s1g : S1(����1)�̏I�_
// s2s : S2(����2)�̎n�_
// s2g : S2(����2)�̏I�_
// r1 : �J�v�Z��1�̔��a
// r2 : �J�v�Z��2�̔��a
// �߂�l: �Փ˂��Ă�����true
//=============================================================================
bool CheckHitCC(D3DXVECTOR3 &s1s, D3DXVECTOR3 &s1g, D3DXVECTOR3 &s2s, D3DXVECTOR3 &s2g, float &r1, float &r2)
{
	float d = DistanceSS(s1s, s1g, s2s, s2g);		//2�����̍ŒZ���������߂�
	return (d <= r1 + r2);							//�ŒZ���������ꂼ��̃J�v�Z�����ar�̍��v�������Z����ΏՓ˂��Ă���(true)
}

//=============================================================================
// �Փ˔��菈���̍X�V
// �߂�l : �Ȃ�
//=============================================================================
void UpdateBattelCollision(void)
{
	srand((unsigned)time(NULL));	//�����_�������l�̏�����

	CAMERA *cam = GetCamera();
	PLAYER *player = GetPlayer();
	ENEMY *enemy = GetEnemy();
	ITEM *item = GetItem();
	SCORE *score = GetScore();

	//������W�̎擾
	D3DXVECTOR3 Wpos, Ws, We;
	GetweaponPos(Wpos, Ws, We);

	// ���f���̔��a��ݒ�
	float r1, r2, r3;
	r1 = WEAPON_R;									//����J�v�Z���̔��a
	r2 = ENEMY_R;									//�G�l�~�[�J�v�Z���̔��a
	r3 = PLAYER_R;									//�v���C���[�J�v�Z���̔��a

	// �X�R�A�v�Z�p�ϐ�
	int hitcnt = 0;

	//����ƓG�̓����蔻��(�X�R�A�v�Z�p�j
	if (GetPlayer()->AttackMotion == true)							//�v���C���[���U�����̎�
	{
		for (int i = 0; i < MAX_ENEMY; i++)							//�G�l�~�[���J��Ԃ�
		{
			if (CheckHitBC(Wpos, (enemy + i)->pos, WEAPON_R * 3, ENEMY_R) == false) continue;	//����̎���ɂ��Ȃ��G�l�~�[�͔��肵�Ȃ�

			for (int j = 0; j < ENEMY_GROUP; j++)					//�C���X�^���V���O�ŕ\�����Ă��鐔���J��Ԃ�
			{
				if ((enemy + i)->use == true)						//�G�l�~�[�������Ă�����
				{
					//�G�l�~�[�ƕ���ŃJ�v�Z�����m�̓����蔻��
					if (CheckHitCC(Ws, We, (enemy + i)->pos, D3DXVECTOR3((enemy + i)->pos.x + j * 10.0f, (enemy + i)->pos.y + ENEMY_LEN, (enemy + i)->pos.z + j * 20), r1, r2) == true)
					{
						hitcnt++;				//���������񐔂��J�E���g
					}
				}
			}
		}
	}

	//����ƓG�̓����蔻��
	if (GetPlayer()->AttackMotion == true)							//�v���C���[���U�����̎�
	{
		for (int i = 0; i < MAX_ENEMY; i++)							//�G�l�~�[���J��Ԃ�
		{
			if (CheckHitBC(Wpos, (enemy + i)->pos, WEAPON_R * 3, ENEMY_R) == false) continue;	//����̎���ɂ��Ȃ��G�l�~�[�͔��肵�Ȃ�

			for (int j = 0; j < ENEMY_GROUP; j++)					//�C���X�^���V���O�ŕ\�����Ă��鐔���J��Ԃ�
			{
				if ((enemy + i)->use == true)						//�G�l�~�[�������Ă�����
				{
					//�G�l�~�[�ƕ���ŃJ�v�Z�����m�̓����蔻��
					if (CheckHitCC(Ws, We, (enemy + i)->pos, D3DXVECTOR3((enemy + i)->pos.x + j * 10.0f, (enemy + i)->pos.y + ENEMY_LEN, (enemy + i)->pos.z + j * 20), r1, r2) == true)
					{
						(enemy + i)->hit_oldpos = (enemy + i)->pos;	//�G�l�~�[�̍���pos��ۑ�
						player->player_hitstop = true;
						(enemy + i)->status = HIT;					//�G�l�~�[�𐁂���΂�
						(enemy + i)->use = false;					//����������G�l�~�[������
						ReleaseShadow(enemy[i].shadowIdx[j]);

						for (int n = 0; n < MAX_PARTICLE; n++)
						{
							GetParticle(i, n)->bUse = true;			//�p�[�e�B�N�����g�p��Ԃ�
						}

						GetEffectNo(0);
						SetEffect((enemy + i)->hit_oldpos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.1f, 0.3f, 0.5f), EFFECT_SIZE_X, EFFECT_SIZE_Y, 60);
						SetEffect((enemy + i)->hit_oldpos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.1f, 0.3f, 0.5f), EFFECT_SIZE_X, EFFECT_SIZE_Y, 60);
						SetEffect((enemy + i)->hit_oldpos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.1f, 0.3f, 0.5f), EFFECT_SIZE_X, EFFECT_SIZE_Y, 60);

						// SE�Đ�
						PlaySound(SOUND_LABEL_SE_ATTACK1);
						PlaySound(SOUND_LABEL_SE_HIT);

						if ((rand() % 100 + 1) <= 30)				//�m���ŃA�C�e�����h���b�v
						{
							(item + i)->bUse = true;
						}
						//�X�R�A���Z
						AddScore(hitcnt, score->score_mag);
					}
				}
			}
		}
	}

	for (int i = 0; i < MAX_ENEMY; i++)						//�G�l�~�[���J��Ԃ�
	{
		if ((enemy + i)->use == true)						//�G�l�~�[�������Ă�����
		{
			if (player->protect == false)					//�v���C���[�����G��Ԃ���Ȃ����
			{
				if ((enemy + i)->status == ATTACK)			//�G�l�~�[���U�����Ȃ�
				{	//�v���C���[�ƃG�l�~�[�̓����蔻��
					if (CheckHitCC(player->pos, D3DXVECTOR3(player->pos.x, player->pos.y + ENEMY_LEN, player->pos.z), (enemy + i)->pos, D3DXVECTOR3((enemy + i)->pos.x, (enemy + i)->pos.y + ENEMY_LEN, (enemy + i)->pos.z), r3, r2) == true)
					{
						// SE�Đ�
						PlaySound(SOUND_LABEL_SE_DAMEGE);

						player->protect = true;				//�v���C���[�𖳓G��Ԃ�
						ChangeLife(-1);						//���C�t������
					}
				}
			}
		}
	}
}

//=============================================================================
// ��`�Ɠ_�̏Փ˔���
// p     : �_(�Փ˔���̑��葤��pos�j
// Fp    : ��`��pos�i�Փ˔�����s����������pos�j
// Fat   : ��`�̕����x�N�g��(�Փ˔�����s���ۂɌ����Ă�������j
// sita  : ��`�̊p�x�i����p�j
// r1    : ��`�̔��a�i���G�͈́j
// r2	 : �J�����`�v���C���[�܂ł̋���(�~���̔��ar1����r2�������������Փ˂Ƃ���)
// �߂�l: �Փ˂��Ă�����true
//=============================================================================
bool CheckHitFP(D3DXVECTOR3 p, D3DXVECTOR3 Fp, D3DXVECTOR3 Fat, float sita, float r1, float r2)
{
	//y�̒l�͍l���Ȃ��̂őS�ď���������
	p.y = 0;
	Fp.y = 0;
	Fat.y = 0;

	if (r1*r1 > LengthSq(p - Fp))							//�_�Ɣ��ar�̉~���Փ˂��Ă��邩�i�~�̔��a(��`�̒���)r�Ɠ_p�܂ł̋������~�̔��a�ȉ��j
	{
		if (r2*r2 < LengthSq(p - Fp))						//�v���C���[���O�ŏՓ˂��Ă��邩
		{
			Fat = Nomalise(Fat);							//��`�̕����x�N�g���𐳋K��
			D3DXVECTOR3 buf = Nomalise(p - Fp);				//�_�Ɛ�`�̒��S�_�̃x�N�g���𐳋K������
			//PrintDebugProc("Fat %ff%ff Dot:%f\n", Fat.x, Fat.z, D3DXToDegree(acosf(Dot(buf, Fat))));
			if (sita > D3DXToDegree(acosf(Dot(buf, Fat))))	//��`�̕����x�N�g���Ɠ_�`��`���S�_�̃x�N�g���ō����p�x����`�̊p�x��菬�������
			{
				return true;								//�Փ˂��Ă���
			}
		}
	}
	return false;											//�Փ˂��Ă��Ȃ�
}

//=============================================================================
// BB�ɂ�铖���蔻�菈��
// ��]�͍l�����Ȃ�
// �߂�l�F�������Ă���true
//=============================================================================
bool CheckHitBB(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR2 size1, D3DXVECTOR2 size2)
{
	if ((pos1.x + size1.x > pos2.x - size2.x) && (pos1.x - size1.x < pos2.x + size2.x) &&
		(pos1.y + size1.y > pos2.y - size2.y) && (pos1.y - size1.y < pos2.y + size2.y))
	{
		return true;
	}

	return false;
}

//=============================================================================
// BC�ɂ�铖���蔻�菈��
// �T�C�Y�͔��a
// �߂�l�F�������Ă���true
//=============================================================================
bool CheckHitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2)
{
	float len = (size1 + size2) * (size1 + size2);	// ���a��2�悵����
	D3DXVECTOR3 temp = pos1 - pos2;
	float fLengthSq = D3DXVec3LengthSq(&temp);		// 2�_�Ԃ̋����i2�悵�����j

	if (len > fLengthSq)
	{
		return true;
	}

	return false;
}

//=============================================================================
//����
//=============================================================================
float Dot(D3DXVECTOR3 &a, D3DXVECTOR3 &b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

//=============================================================================
//�O��
//=============================================================================
D3DXVECTOR3 Cross(D3DXVECTOR3 &a, D3DXVECTOR3 &b)
{
	return D3DXVECTOR3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

//=============================================================================
//�ׂ���̒���
//=============================================================================
float LengthSq(D3DXVECTOR3 &a)
{
	return a.x * a.x + a.y * a.y + a.z * a.z;
}

//=============================================================================
//�x�N�g���̑傫��������(sqrtf=���������v�Z�����ʂ�float�^�ŕԂ�)
//=============================================================================
float Length(D3DXVECTOR3 &a)
{
	float lengthsq = a.x * a.x + a.y * a.y + a.z * a.z;
	return sqrtf(lengthsq);
}

//=============================================================================
//���s�֌W?
//=============================================================================
bool Parallel(D3DXVECTOR3 &a, D3DXVECTOR3 &b)
{
	D3DXVECTOR3 crossV = Cross(a, b);		//a��b�̊O�ς����߂�
	float d = LengthSq(crossV);
	if (-EPSILON < d && d < EPSILON)
	{
		return true;						//�덷�͈͓��Ȃ畽�s�Ȃ̂�true��Ԃ�
	}
	return false;							//����ȊO�͕��s�łȂ��̂�false��Ԃ�
}

//=============================================================================
//�x�N�g���ɌW��t����Z���Đ���̔C�ӂ̓_���擾
//=============================================================================
D3DXVECTOR3 GetPoint(D3DXVECTOR3 &p, float t, D3DXVECTOR3&v)
{
	return p + t * v;						//������̓_p+�x�N�g���W��t*�����̃x�N�g��v
}

//=============================================================================
//�x�N�g���W��t�����߂�
//=============================================================================
float VectorT(D3DXVECTOR3 &v1, D3DXVECTOR3 &v2)
{
	D3DXVECTOR3 newv1 = Nomalise(v1);				//v1�̐��K��
	float t = Dot(newv1, v2) / Length(newv1);		//�x�N�g���W�������߂�
	return t;										//�x�N�g���W����Ԃ�
}

//=============================================================================
//���K��
//=============================================================================
D3DXVECTOR3 Nomalise(D3DXVECTOR3 v1)
{
	float len = Length(v1);		//�@�����̕����x�N�g���̑傫�������߂�
	len = 1 / len;				//�A�P�ʃx�N�g�������߂邽�߂̒l����������1���@�Ŋ���
	v1 *= len;					//�B�P�ʃx�N�g���i����1�̃x�N�g���j�����߂�
	return v1;
}
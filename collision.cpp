//=============================================================================
//
// Õ“Ë”»’èˆ— [collision.cpp]
// Author : à_è ¬t
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
// ƒ}ƒNƒ’è‹`
//*****************************************************************************

//*****************************************************************************
// ƒvƒƒ^ƒCƒvéŒ¾
//*****************************************************************************

//*****************************************************************************
// ƒOƒ[ƒoƒ‹•Ï”
//*****************************************************************************

//=============================================================================
// ƒƒCƒ“ŠÖ”
//=============================================================================

//=============================================================================
// ƒvƒƒV[ƒWƒƒ
//=============================================================================

//=============================================================================
// 0`1‚ÌŠÔ‚ÉƒNƒ‰ƒ“ƒvin“_‚©I“_‚ÉŠñ‚¹‚éj
// t : ƒxƒNƒgƒ‹ŒW”
//=============================================================================
void Clamp(float &t)
{
	if (t < 0.0f)		//ƒxƒNƒgƒ‹ŒW”‚ªn“_‚æ‚èŠO‘¤‚¾‚Á‚½‚ç
		t = 0.0f;		//ƒxƒNƒgƒ‹ŒW”‚ğn“_‚É–ß‚·
	else if (t > 1.0f)	//ƒxƒNƒgƒ‹ŒW”‚ªI“_‚æ‚èŠO‘¤‚¾‚Á‚½‚ç
		t = 1.0f;		//ƒxƒNƒgƒ‹ŒW”‚ğI“_‚É–ß‚·
}

//=============================================================================
// p1p2p3‚Í‰sŠpH
// p1 : “_
// p2 : ü•ª‚Ìn“_
// p3 : ü•ª‚ÌI“_
//=============================================================================
bool SharpAngle(D3DXVECTOR3 &p1, D3DXVECTOR3 &p2, D3DXVECTOR3 &p3)
{
	D3DXVECTOR3 p1p2 = p2 - p1;		//ƒxƒNƒgƒ‹p1p2
	D3DXVECTOR3 p2p3 = p3 - p2;		//ƒxƒNƒgƒ‹p2p3

	float sita = Dot(p1p2, p2p3);	//ƒÆ‚Ì’l‚ğ‹‚ß‚é

	if (sita < 0)					//sita‚ªƒ}ƒCƒiƒX‚È‚ç“İŠp
	{
		return false;
	}
	return true;					//sita‚ªƒvƒ‰ƒX‚È‚ç‰sŠp
}

//=============================================================================
// “_‚Æ’¼ü‚ÌÅ’Z‹——£
// p1:“_
// l1:’¼üã‚Ì“_1
// v1:’¼ü‚ÌƒxƒNƒgƒ‹
// h :Å’Z‹——£i–ß‚è’lj
//=============================================================================
float DistancePL(D3DXVECTOR3 &p1, D3DXVECTOR3 &l1, D3DXVECTOR3 &v1)
{
	float len = LengthSq(v1);						//’¼ü‚Ì•ûŒüƒxƒNƒgƒ‹‚Ì‘å‚«‚³‚ğ‹‚ß‚é
	float t = 0.0f;									//ƒxƒNƒgƒ‹ŒW”‚Ì‰Šú‰»
	if (len > 0.0f)									//’¼ü‚Ì•ûŒüƒxƒNƒgƒ‹‚Ì‘å‚«‚³‚ª0ˆÈã‚È‚ç
	{	//ƒxƒNƒgƒ‹ŒW”‚ğ‹‚ß‚éi(’¼ü‚Æ’¼üã‚Ì“_1`“_‚Ì“àÏ)/•ûŒüƒxƒNƒgƒ‹‚Ì‘å‚«‚³)
		t = Dot(v1, p1 - l1) / len;
	}
	//Å’Z‹——£=‚ü‚Ì’·‚³h-“_p1 = ( l1 + t * v1 ) - p1iƒ[ƒƒxƒNƒgƒ‹‚Ìê‡‚Íl1-p1‚Ì‹——£)
	return Length((l1 + t * v1) - p1);				//Å’Z‹——£‚ğ•Ô‚·
}

//=============================================================================
// “_‚Æü•ª‚ÌÅ’Z‹——£
// s1s:ü•ª‚Ìn“_1
// s1g:ü•ª‚ÌI“_2
// p1 :“_
// h  :Å’Z‹——£(–ß‚è’l)
//=============================================================================
float DistancePS(D3DXVECTOR3 &s1s, D3DXVECTOR3 &s1g, D3DXVECTOR3 &p1)
{
	D3DXVECTOR3 v1 = s1g - s1s;				//s1s2ƒxƒNƒgƒ‹(’¼ü‚ÌƒxƒNƒgƒ‹)
	D3DXVECTOR3 v2 = p1 - s1s;				//p3s1ƒxƒNƒgƒ‹
	D3DXVECTOR3 v3 = p1 - s1g;				//p3s2ƒxƒNƒgƒ‹

	D3DXVECTOR3 v1org = v1;					//ˆê’Uv1‚ğ•Û‘¶
	Nomalise(v1);							//v1‚ğ³‹K‰»

	float t = Dot(v1, v2) / Length(v1);		//ƒxƒNƒgƒ‹ŒW”‚ğ‹‚ß‚é
	if (t < 0)								//“_p1‚ªs1s‘¤‚ÌŠO‘¤‚É‚ ‚éê‡
	{
		return Length(v2);					//Å’Z‹——£=p1`s1s‚ÌƒxƒNƒgƒ‹‚Ì’·‚³
	}
	else if (t > 1)							//“_p1‚ªs1g‘¤‚ÌŠO‘¤‚É‚ ‚éê‡
	{
		return Length(v3);					//Å’Z‹——£=p1`s1g‚ÌƒxƒNƒgƒ‹‚Ì’·‚³
	}
	else
	{										//“_p1‚ªü•ª‚Ìã‚É‚ ‚éê‡
		return Length(v1org * t - v2);		//Å’Z‹——£=“_p1‚©‚çü•ª‚É‰º‚µ‚½‚ü
	}
}

//=============================================================================
// 2’¼ü‚ÌÅ’Z‹——£
// l1 : ’¼ü1ã‚Ì“_
// v1 : ’¼ü1‚ÌƒxƒNƒgƒ‹
// l2 : ’¼ü2ã‚Ì“_
// v2 : ’¼ü2‚ÌƒxƒNƒgƒ‹
// t1 : ’¼ü1‚ÌƒxƒNƒgƒ‹ŒW”
// t2 : ’¼ü2‚ÌƒxƒNƒgƒ‹ŒW”
// p1 : ’¼ü1‚Ì‚ü‚Ì‘«‚Ì“_
// p2 : ’¼ü2‚Ì‚ü‚Ì‘«‚Ì“_
// –ß‚è’l:Å’Z‹——£
//=============================================================================
float DistanceLL(D3DXVECTOR3 &l1, D3DXVECTOR3 &v1, D3DXVECTOR3 &l2, D3DXVECTOR3 &v2)
{
	// 2’¼ü‚ª•½s?
	if (Parallel(v1, v2) == true)
	{
		// l1ã‚Ì“_‚Æ’¼ül2‚Ì‹——£‚ğ‹‚ß‚Ä•Ô‚·
		float len = DistancePL(l1, l2, v2);
		return len;
	}

	// 2’¼ü‚Í•½s‚Å‚È‚¢
	float dotl1l2 = Dot(v1, v2);							//’¼ü1‚Æl2‚Ì“àÏ
	float lenl1 = LengthSq(v1);								//’¼ü1‚Ì‚×‚«æ‚Ì’·‚³
	float lenl2 = LengthSq(v2);								//’¼ü2‚Ì‚×‚«æ‚Ì’·‚³
	D3DXVECTOR3 vl1l2 = l1 - l2;							//’¼ü1ã‚Ì“_‚Æ’¼ü2ã‚Ì“_‚ğŒ‹‚ñ‚¾ƒxƒNƒgƒ‹

	float t1 = (dotl1l2 * Dot(v2, vl1l2) - lenl2 * Dot(v1, vl1l2)) /
		(lenl1 * lenl2 - dotl1l2 * dotl1l2);			//’¼ü1‚ÌƒxƒNƒgƒ‹ŒW”‚ğ‹‚ß‚é
	D3DXVECTOR3 p1 = GetPoint(l1, t1, v1);					//’¼ü1‚Ì‚ü‚Ì‘«‚Ì“_‚ğ‹‚ß‚é
	float t2 = Dot(v2, p1 - l2) / lenl2;					//’¼ü2‚ÌƒxƒNƒgƒ‹ŒW”‚ğ‹‚ß‚é
	D3DXVECTOR3 p2 = GetPoint(l2, t2, v1);					//’¼ü2‚Ì‚ü‚Ì‘«‚Ì“_‚ğ‹‚ß‚é

	return Length(p2 - p1);									//‚ü‚Ì‘«‚Ì“_“¯m‚Ì‹——£(Å’Z‹——£)‚ğ•Ô‚·
}

//=============================================================================
// 2ü•ª‚ÌÅ’Z‹——£
// s1s : S1(ü•ª1)‚Ìn“_
// s1g : S1(ü•ª1)‚ÌI“_
// s2s : S2(ü•ª2)‚Ìn“_
// s2g : S2(ü•ª2)‚ÌI“_
// p1@: ü•ª1‚Ì‚ü‚Ì‘«‚Ì“_
// p2@: ü•ª2‚Ì‚ü‚Ì‘«‚Ì“_
// t1@: ü•ª1‚ÌƒxƒNƒgƒ‹ŒW”
// t2@: ü•ª2‚ÌƒxƒNƒgƒ‹ŒW”
// –ß‚è’lFÅ’Z‹——£
//=============================================================================
float DistanceSS(D3DXVECTOR3 &s1s, D3DXVECTOR3 &s1g, D3DXVECTOR3 &s2s, D3DXVECTOR3 &s2g)
{
	D3DXVECTOR3 v1 = s1g - s1s;										//s1ƒxƒNƒgƒ‹(’¼ü‚ÌƒxƒNƒgƒ‹)
	D3DXVECTOR3 v2 = s2g - s2s;										//s2ƒxƒNƒgƒ‹(’¼ü‚ÌƒxƒNƒgƒ‹)

	float t1 = VectorT(v1, v2);										//ü•ª1‚ÌƒxƒNƒgƒ‹ŒW”
	float t2 = VectorT(v2, v1);										//ü•ª2‚ÌƒxƒNƒgƒ‹ŒW”

	//s1‚ªŒë·ˆÈ‰º‚Ì’·‚³)‚©?
	if (LengthSq(v1) < EPSILON)										//s1‚ÌƒxƒNƒgƒ‹‚ªŒë·‚æ‚è’Z‚¯‚ê‚Î
	{
		//s2‚àŒë·ˆÈ‰º‚Ì’·‚³‚©?
		if (LengthSq(v2) < EPSILON)									//s1s2‚ÌƒxƒNƒgƒ‹‚ª‚Æ‚à‚ÉŒë·‚æ‚è’Z‚¯‚ê‚Î
		{
			float len = Length(s2s - s1s);							//Å’Z‹——£=s1‚Ìn“_`s2‚Ìn“_
			D3DXVECTOR3 p1 = s1s;									//‚ü1‚Ì‘«‚ğs1‚Ìn“_‚Éİ’è
			D3DXVECTOR3 p2 = s2s;									//‚ü2‚Ì‘«‚ğs2‚Ìn“_‚Éİ’è
			t1 = t2 = 0.0f;											//s1‚às2‚àk‘Ş‚µ‚Ä‚¢‚é‚Ì‚ÅAƒxƒNƒgƒ‹ŒW”‚Í0
			return len;												//Å’Z‹——£‚ğ•Ô‚·
		}
		//s1‚Ì‚İŒë·ˆÈ‰º‚Ì’·‚³‚Ì‚Æ‚«
		else
		{
			float len = DistancePS(s2s, s2g, s1s);					//Å’Z‹——£=s1‚Ìn“_`s2ã‚Ì“_
			D3DXVECTOR3 p1 = s1s;									//‚ü1‚Ì‘«‚ğs1‚Ìn“_‚Éİ’è	
			t1 = 0.0f;												//s1‚ÌƒxƒNƒgƒ‹ŒW”‚Í0
			Clamp(t2);												//s2‚ÌƒxƒNƒgƒ‹ŒW”‚ğ0.0`1.0‚Éû‚ß‚é
			return len;												//Å’Z‹——£‚ğ•Ô‚·
		}
	}

	//s2‚ªŒë·ˆÈ‰º‚Ì’·‚³‚©?
	else if (LengthSq(v2) < EPSILON)								//s2‚ÌƒxƒNƒgƒ‹‚ªŒë·‚æ‚è’Z‚¯‚ê‚Î
	{
		float len = DistancePS(s1s, s1g, s2s);						//Å’Z‹——£=s2‚Ìn“_`‚ü‚Æ‚È‚és1ã‚Ì“_
		D3DXVECTOR3 p2 = s2s;										//‚ü2‚Ì‘«‚ğs2‚Ìn“_‚Éİ’è
		t2 = 0.0f;													//s2‚ÌƒxƒNƒgƒ‹ŒW”‚Í0
		Clamp(t1);													//s1‚ÌƒxƒNƒgƒ‹ŒW”‚ğ0.0`1.0‚Éû‚ß‚é
		return len;													//Å’Z‹——£‚ğ•Ô‚·
	}

	/* ü•ª“¯m */
	if (Parallel(v1, v2) == true)									//2ü•ª‚ª•½s‚¾‚Á‚½‚ç
	{//S1‚Ìn“_‚ğ‚ü‚Ì‘«p1‚Éİ’è‚·‚é
		t1 = 0.0f;													//s1‚ÌƒxƒNƒgƒ‹ŒW”‚ğ0‚É‚·‚é
		D3DXVECTOR3 p1 = s1s;										//‚ü1‚Ì‘«‚ğs1‚Ìn“_‚Éİ’è
		float len = DistancePS(s2s, s2g, p1);						//Å’Z‹——£=s1‚Ìn“_`‚ü‚Æ‚È‚és2ã‚Ì“_
		if (0.0f <= t2 && t2 <= 1.0f)								//‚ü‚Æ‚È‚é“_‚ªs2ã‚É‚ ‚ê‚Î
		{
			return len;												//Å’Z‹——£‚ğ•Ô‚·
		}
	}
	else
	{
		//ü•ª‚Í‚Ë‚¶‚ê‚ÌŠÖŒW‚È‚Ì‚ÅA2’¼üŠÔ‚ÌÅ’Z‹——£‚ğ‹‚ß‚Ä‰¼‚Ìt1,t2‚ğ‹‚ß‚é
		float len = DistanceLL(s1s, v1, s2s, v2);					//Å’Z‹——£=s1s2‚Ì‚ü‚Ì‘«‚Ì“_“¯m‚Ì‹——£
		if (0.0f <= t1 && t1 <= 1.0f &&0.0f <= t2 && t2 <= 1.0f)	//Å’Z‹——£‚ğŒ‹‚Ô“_‚ªs1s2‚Æ‚à‚Éü•ªã‚É‚ ‚ê‚Î
		{
			return len;												//Å’Z‹——£‚ğ•Ô‚·
		}
	}

	//‚Ç‚¿‚ç‚©‚Ì‚ü‚Ì‘«‚Ì“_‚ªü•ª‚ÌŠO
	Clamp(t1);														//s1‘¤‚Ìt1‚ğ0`1‚ÌŠÔ‚ÉƒNƒ‰ƒ“ƒv
	D3DXVECTOR3 p1 = GetPoint(s1s, t1, v1);							//s1ã‚Ì”CˆÓ‚Ì“_‚ğp1‚Éİ’è
	float lenss = DistancePS(s2s, s2g, p1);							//Å’Z‹——£=s1ã‚Ì“_`‚ü‚Ì‘«‚Æ‚È‚és2ã‚Ì“_
	if (0.0f <= t2 && t2 <= 1.0f)									//‚ü‚Ì‘«‚ªs2ã‚É‚ ‚ê‚Î
	{
		return lenss;												//Å’Z‹——£‚ğ•Ô‚·
	}

	//s2‘¤‚Ì‚ü‚Ì“_‚ªü•ª‚ÌŠO
	Clamp(t2);														//s2‘¤‚Ìt2‚ğ0`1‚ÌŠÔ‚ÉƒNƒ‰ƒ“ƒv
	D3DXVECTOR3 p2 = GetPoint(s2s, t2, v2);							//s2ã‚Ì”CˆÓ‚Ì“_‚ğp2‚Éİ’è
	lenss = DistancePS(s1s, s1g, p2);								//Å’Z‹——£=s2ã‚Ì“_`‚ü‚Ì‘«‚Æ‚È‚és1ã‚Ì“_
	if (0.0f <= t1 && t1 <= 1.0f)									//‚ü‚Ì‘«‚ªs1ã‚É‚ ‚ê‚Î
	{
		return lenss;												//Å’Z‹——£‚ğ•Ô‚·
	}

	//Å’Z‹——£=s2‚Ì’[“_`s1‚Ì’[“_
	Clamp(t1);														//s1‘¤‚Ìt1‚ğ0`1‚ÌŠÔ‚ÉƒNƒ‰ƒ“ƒv
	p1 = GetPoint(s1s, t1, v1);										//s1ã‚Ì”CˆÓ‚Ì“_‚ğp1‚Éİ’è
	return Length(p2 - p1);											//Å’Z‹——£‚ğ•Ô‚·
}

//=============================================================================
// ƒJƒvƒZƒ‹“¯m‚ÌÕ“Ë”»’è
// s1s : S1(ü•ª1)‚Ìn“_
// s1g : S1(ü•ª1)‚ÌI“_
// s2s : S2(ü•ª2)‚Ìn“_
// s2g : S2(ü•ª2)‚ÌI“_
// r1 : ƒJƒvƒZƒ‹1‚Ì”¼Œa
// r2 : ƒJƒvƒZƒ‹2‚Ì”¼Œa
// –ß‚è’l: Õ“Ë‚µ‚Ä‚¢‚½‚çtrue
//=============================================================================
bool CheckHitCC(D3DXVECTOR3 &s1s, D3DXVECTOR3 &s1g, D3DXVECTOR3 &s2s, D3DXVECTOR3 &s2g, float &r1, float &r2)
{
	float d = DistanceSS(s1s, s1g, s2s, s2g);		//2ü•ª‚ÌÅ’Z‹——£‚ğ‹‚ß‚é
	return (d <= r1 + r2);							//Å’Z‹——£‚ª‚»‚ê‚¼‚ê‚ÌƒJƒvƒZƒ‹”¼Œar‚Ì‡Œv‹——£‚æ‚è’Z‚¯‚ê‚ÎÕ“Ë‚µ‚Ä‚¢‚é(true)
}

//=============================================================================
// Õ“Ë”»’èˆ—‚ÌXV
// –ß‚è’l : ‚È‚µ
//=============================================================================
void UpdateBattelCollision(void)
{
	srand((unsigned)time(NULL));	//ƒ‰ƒ“ƒ_ƒ€‰Šú’l‚Ì‰Šú‰»

	CAMERA *cam = GetCamera();
	PLAYER *player = GetPlayer();
	ENEMY *enemy = GetEnemy();
	ITEM *item = GetItem();
	SCORE *score = GetScore();

	//•ŠíÀ•W‚Ìæ“¾
	D3DXVECTOR3 Wpos, Ws, We;
	GetweaponPos(Wpos, Ws, We);

	// ƒ‚ƒfƒ‹‚Ì”¼Œa‚ğİ’è
	float r1, r2, r3;
	r1 = WEAPON_R;									//•ŠíƒJƒvƒZƒ‹‚Ì”¼Œa
	r2 = ENEMY_R;									//ƒGƒlƒ~[ƒJƒvƒZƒ‹‚Ì”¼Œa
	r3 = PLAYER_R;									//ƒvƒŒƒCƒ„[ƒJƒvƒZƒ‹‚Ì”¼Œa

	// ƒXƒRƒAŒvZ—p•Ï”
	int hitcnt = 0;

	//•Ší‚Æ“G‚Ì“–‚½‚è”»’è(ƒXƒRƒAŒvZ—pj
	if (GetPlayer()->AttackMotion == true)							//ƒvƒŒƒCƒ„[‚ªUŒ‚’†‚Ì
	{
		for (int i = 0; i < MAX_ENEMY; i++)							//ƒGƒlƒ~[•ªŒJ‚è•Ô‚·
		{
			if (CheckHitBC(Wpos, (enemy + i)->pos, WEAPON_R * 3, ENEMY_R) == false) continue;	//•Ší‚Ìü‚è‚É‚¢‚È‚¢ƒGƒlƒ~[‚Í”»’è‚µ‚È‚¢

			for (int j = 0; j < ENEMY_GROUP; j++)					//ƒCƒ“ƒXƒ^ƒ“ƒVƒ“ƒO‚Å•\¦‚µ‚Ä‚¢‚é”•ªŒJ‚è•Ô‚·
			{
				if ((enemy + i)->use == true)						//ƒGƒlƒ~[‚ª¶‚«‚Ä‚¢‚½‚ç
				{
					//ƒGƒlƒ~[‚Æ•Ší‚ÅƒJƒvƒZƒ‹“¯m‚Ì“–‚½‚è”»’è
					if (CheckHitCC(Ws, We, (enemy + i)->pos, D3DXVECTOR3((enemy + i)->pos.x + j * 10.0f, (enemy + i)->pos.y + ENEMY_LEN, (enemy + i)->pos.z + j * 20), r1, r2) == true)
					{
						hitcnt++;				//“–‚½‚Á‚½‰ñ”‚ğƒJƒEƒ“ƒg
					}
				}
			}
		}
	}

	//•Ší‚Æ“G‚Ì“–‚½‚è”»’è
	if (GetPlayer()->AttackMotion == true)							//ƒvƒŒƒCƒ„[‚ªUŒ‚’†‚Ì
	{
		for (int i = 0; i < MAX_ENEMY; i++)							//ƒGƒlƒ~[•ªŒJ‚è•Ô‚·
		{
			if (CheckHitBC(Wpos, (enemy + i)->pos, WEAPON_R * 3, ENEMY_R) == false) continue;	//•Ší‚Ìü‚è‚É‚¢‚È‚¢ƒGƒlƒ~[‚Í”»’è‚µ‚È‚¢

			for (int j = 0; j < ENEMY_GROUP; j++)					//ƒCƒ“ƒXƒ^ƒ“ƒVƒ“ƒO‚Å•\¦‚µ‚Ä‚¢‚é”•ªŒJ‚è•Ô‚·
			{
				if ((enemy + i)->use == true)						//ƒGƒlƒ~[‚ª¶‚«‚Ä‚¢‚½‚ç
				{
					//ƒGƒlƒ~[‚Æ•Ší‚ÅƒJƒvƒZƒ‹“¯m‚Ì“–‚½‚è”»’è
					if (CheckHitCC(Ws, We, (enemy + i)->pos, D3DXVECTOR3((enemy + i)->pos.x + j * 10.0f, (enemy + i)->pos.y + ENEMY_LEN, (enemy + i)->pos.z + j * 20), r1, r2) == true)
					{
						(enemy + i)->hit_oldpos = (enemy + i)->pos;	//ƒGƒlƒ~[‚Ì¡‚Ìpos‚ğ•Û‘¶
						player->player_hitstop = true;
						(enemy + i)->status = HIT;					//ƒGƒlƒ~[‚ğ‚Á”ò‚Î‚·
						(enemy + i)->use = false;					//“–‚½‚Á‚½‚çƒGƒlƒ~[‚ğÁ‚·
						ReleaseShadow(enemy[i].shadowIdx[j]);

						for (int n = 0; n < MAX_PARTICLE; n++)
						{
							GetParticle(i, n)->bUse = true;			//ƒp[ƒeƒBƒNƒ‹‚ğg—pó‘Ô‚É
						}

						GetEffectNo(0);
						SetEffect((enemy + i)->hit_oldpos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.1f, 0.3f, 0.5f), EFFECT_SIZE_X, EFFECT_SIZE_Y, 60);
						SetEffect((enemy + i)->hit_oldpos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.1f, 0.3f, 0.5f), EFFECT_SIZE_X, EFFECT_SIZE_Y, 60);
						SetEffect((enemy + i)->hit_oldpos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.1f, 0.3f, 0.5f), EFFECT_SIZE_X, EFFECT_SIZE_Y, 60);

						// SEÄ¶
						PlaySound(SOUND_LABEL_SE_ATTACK1);
						PlaySound(SOUND_LABEL_SE_HIT);

						if ((rand() % 100 + 1) <= 30)				//Šm—¦‚ÅƒAƒCƒeƒ€‚ğƒhƒƒbƒv
						{
							(item + i)->bUse = true;
						}
						//ƒXƒRƒA‰ÁZ
						AddScore(hitcnt, score->score_mag);
					}
				}
			}
		}
	}

	for (int i = 0; i < MAX_ENEMY; i++)						//ƒGƒlƒ~[•ªŒJ‚è•Ô‚·
	{
		if ((enemy + i)->use == true)						//ƒGƒlƒ~[‚ª¶‚«‚Ä‚¢‚½‚ç
		{
			if (player->protect == false)					//ƒvƒŒƒCƒ„[‚ª–³“Gó‘Ô‚¶‚á‚È‚¯‚ê‚Î
			{
				if ((enemy + i)->status == ATTACK)			//ƒGƒlƒ~[‚ªUŒ‚’†‚È‚ç
				{	//ƒvƒŒƒCƒ„[‚ÆƒGƒlƒ~[‚Ì“–‚½‚è”»’è
					if (CheckHitCC(player->pos, D3DXVECTOR3(player->pos.x, player->pos.y + ENEMY_LEN, player->pos.z), (enemy + i)->pos, D3DXVECTOR3((enemy + i)->pos.x, (enemy + i)->pos.y + ENEMY_LEN, (enemy + i)->pos.z), r3, r2) == true)
					{
						// SEÄ¶
						PlaySound(SOUND_LABEL_SE_DAMEGE);

						player->protect = true;				//ƒvƒŒƒCƒ„[‚ğ–³“Gó‘Ô‚É
						ChangeLife(-1);						//ƒ‰ƒCƒt‚ªŒ¸‚é
					}
				}
			}
		}
	}
}

//=============================================================================
// îŒ`‚Æ“_‚ÌÕ“Ë”»’è
// p     : “_(Õ“Ë”»’è‚Ì‘Šè‘¤‚Ìposj
// Fp    : îŒ`‚ÌposiÕ“Ë”»’è‚ğs‚¤©•ª‘¤‚Ìposj
// Fat   : îŒ`‚Ì•ûŒüƒxƒNƒgƒ‹(Õ“Ë”»’è‚ğs‚¤Û‚ÉŒü‚¢‚Ä‚¢‚é•ûŒüj
// sita  : îŒ`‚ÌŠp“xi‹–ìŠpj
// r1    : îŒ`‚Ì”¼Œaiõ“G”ÍˆÍj
// r2	 : ƒJƒƒ‰`ƒvƒŒƒCƒ„[‚Ü‚Å‚Ì‹——£(‰~‚Ì”¼Œar1‚©‚çr2‚ğœ‚¢‚½‹æˆæ‚ğÕ“Ë‚Æ‚·‚é)
// –ß‚è’l: Õ“Ë‚µ‚Ä‚¢‚½‚çtrue
//=============================================================================
bool CheckHitFP(D3DXVECTOR3 p, D3DXVECTOR3 Fp, D3DXVECTOR3 Fat, float sita, float r1, float r2)
{
	//y‚Ì’l‚Íl‚¦‚È‚¢‚Ì‚Å‘S‚Ä‰Šú‰»‚·‚é
	p.y = 0;
	Fp.y = 0;
	Fat.y = 0;

	if (r1*r1 > LengthSq(p - Fp))							//“_‚Æ”¼Œar‚Ì‰~‚ªÕ“Ë‚µ‚Ä‚¢‚é‚©i‰~‚Ì”¼Œa(îŒ`‚Ì’·‚³)r‚Æ“_p‚Ü‚Å‚Ì‹——£‚ª‰~‚Ì”¼ŒaˆÈ‰ºj
	{
		if (r2*r2 < LengthSq(p - Fp))						//ƒvƒŒƒCƒ„[‚æ‚è‘O‚ÅÕ“Ë‚µ‚Ä‚¢‚é‚©
		{
			Fat = Nomalise(Fat);							//îŒ`‚Ì•ûŒüƒxƒNƒgƒ‹‚ğ³‹K‰»
			D3DXVECTOR3 buf = Nomalise(p - Fp);				//“_‚ÆîŒ`‚Ì’†S“_‚ÌƒxƒNƒgƒ‹‚ğ³‹K‰»‚·‚é
			//PrintDebugProc("Fat %ff%ff Dot:%f\n", Fat.x, Fat.z, D3DXToDegree(acosf(Dot(buf, Fat))));
			if (sita > D3DXToDegree(acosf(Dot(buf, Fat))))	//îŒ`‚Ì•ûŒüƒxƒNƒgƒ‹‚Æ“_`îŒ`’†S“_‚ÌƒxƒNƒgƒ‹‚Åì‚ç‚ê‚éŠp“x‚ªîŒ`‚ÌŠp“x‚æ‚è¬‚³‚¯‚ê‚Î
			{
				return true;								//Õ“Ë‚µ‚Ä‚¢‚é
			}
		}
	}
	return false;											//Õ“Ë‚µ‚Ä‚¢‚È‚¢
}

//=============================================================================
// BB‚É‚æ‚é“–‚½‚è”»’èˆ—
// ‰ñ“]‚Íl—¶‚µ‚È‚¢
// –ß‚è’lF“–‚½‚Á‚Ä‚½‚çtrue
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
// BC‚É‚æ‚é“–‚½‚è”»’èˆ—
// ƒTƒCƒY‚Í”¼Œa
// –ß‚è’lF“–‚½‚Á‚Ä‚½‚çtrue
//=============================================================================
bool CheckHitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2)
{
	float len = (size1 + size2) * (size1 + size2);	// ”¼Œa‚ğ2æ‚µ‚½•¨
	D3DXVECTOR3 temp = pos1 - pos2;
	float fLengthSq = D3DXVec3LengthSq(&temp);		// 2“_ŠÔ‚Ì‹——£i2æ‚µ‚½•¨j

	if (len > fLengthSq)
	{
		return true;
	}

	return false;
}

//=============================================================================
//“àÏ
//=============================================================================
float Dot(D3DXVECTOR3 &a, D3DXVECTOR3 &b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

//=============================================================================
//ŠOÏ
//=============================================================================
D3DXVECTOR3 Cross(D3DXVECTOR3 &a, D3DXVECTOR3 &b)
{
	return D3DXVECTOR3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

//=============================================================================
//‚×‚«æ‚Ì’·‚³
//=============================================================================
float LengthSq(D3DXVECTOR3 &a)
{
	return a.x * a.x + a.y * a.y + a.z * a.z;
}

//=============================================================================
//ƒxƒNƒgƒ‹‚Ì‘å‚«‚³¦’·‚³(sqrtf=•½•ûª‚ğŒvZ‚µŒ‹‰Ê‚ğfloatŒ^‚Å•Ô‚·)
//=============================================================================
float Length(D3DXVECTOR3 &a)
{
	float lengthsq = a.x * a.x + a.y * a.y + a.z * a.z;
	return sqrtf(lengthsq);
}

//=============================================================================
//•½sŠÖŒW?
//=============================================================================
bool Parallel(D3DXVECTOR3 &a, D3DXVECTOR3 &b)
{
	D3DXVECTOR3 crossV = Cross(a, b);		//a‚Æb‚ÌŠOÏ‚ğ‹‚ß‚é
	float d = LengthSq(crossV);
	if (-EPSILON < d && d < EPSILON)
	{
		return true;						//Œë·”ÍˆÍ“à‚È‚ç•½s‚È‚Ì‚Åtrue‚ğ•Ô‚·
	}
	return false;							//‚»‚êˆÈŠO‚Í•½s‚Å‚È‚¢‚Ì‚Åfalse‚ğ•Ô‚·
}

//=============================================================================
//ƒxƒNƒgƒ‹‚ÉŒW”t‚ğæZ‚µ‚Äüã‚Ì”CˆÓ‚Ì“_‚ğæ“¾
//=============================================================================
D3DXVECTOR3 GetPoint(D3DXVECTOR3 &p, float t, D3DXVECTOR3&v)
{
	return p + t * v;						//’¼üã‚Ì“_p+ƒxƒNƒgƒ‹ŒW”t*’¼ü‚ÌƒxƒNƒgƒ‹v
}

//=============================================================================
//ƒxƒNƒgƒ‹ŒW”t‚ğ‹‚ß‚é
//=============================================================================
float VectorT(D3DXVECTOR3 &v1, D3DXVECTOR3 &v2)
{
	D3DXVECTOR3 newv1 = Nomalise(v1);				//v1‚Ì³‹K‰»
	float t = Dot(newv1, v2) / Length(newv1);		//ƒxƒNƒgƒ‹ŒW”‚ğ‹‚ß‚é
	return t;										//ƒxƒNƒgƒ‹ŒW”‚ğ•Ô‚·
}

//=============================================================================
//³‹K‰»
//=============================================================================
D3DXVECTOR3 Nomalise(D3DXVECTOR3 v1)
{
	float len = Length(v1);		//‡@’¼ü‚Ì•ûŒüƒxƒNƒgƒ‹‚Ì‘å‚«‚³‚ğ‹‚ß‚é
	len = 1 / len;				//‡A’PˆÊƒxƒNƒgƒ‹‚ğ‹‚ß‚é‚½‚ß‚Ì’l‚ğ‚¾‚·‚½‚ß1‚ğ‡@‚ÅŠ„‚é
	v1 *= len;					//‡B’PˆÊƒxƒNƒgƒ‹i’·‚³1‚ÌƒxƒNƒgƒ‹j‚ğ‹‚ß‚é
	return v1;
}
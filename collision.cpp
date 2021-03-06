//=============================================================================
//
// 衝突判定処理 [collision.cpp]
// Author : 濱崎 小春
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
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//=============================================================================
// メイン関数
//=============================================================================

//=============================================================================
// プロシージャ
//=============================================================================

//=============================================================================
// 0〜1の間にクランプ（始点か終点に寄せる）
// t : ベクトル係数
//=============================================================================
void Clamp(float &t)
{
	if (t < 0.0f)		//ベクトル係数が始点より外側だったら
		t = 0.0f;		//ベクトル係数を始点に戻す
	else if (t > 1.0f)	//ベクトル係数が終点より外側だったら
		t = 1.0f;		//ベクトル係数を終点に戻す
}

//=============================================================================
// p1p2p3は鋭角？
// p1 : 点
// p2 : 線分の始点
// p3 : 線分の終点
//=============================================================================
bool SharpAngle(D3DXVECTOR3 &p1, D3DXVECTOR3 &p2, D3DXVECTOR3 &p3)
{
	D3DXVECTOR3 p1p2 = p2 - p1;		//ベクトルp1p2
	D3DXVECTOR3 p2p3 = p3 - p2;		//ベクトルp2p3

	float sita = Dot(p1p2, p2p3);	//θの値を求める

	if (sita < 0)					//sitaがマイナスなら鈍角
	{
		return false;
	}
	return true;					//sitaがプラスなら鋭角
}

//=============================================================================
// 点と直線の最短距離
// p1:点
// l1:直線上の点1
// v1:直線のベクトル
// h :最短距離（戻り値）
//=============================================================================
float DistancePL(D3DXVECTOR3 &p1, D3DXVECTOR3 &l1, D3DXVECTOR3 &v1)
{
	float len = LengthSq(v1);						//直線の方向ベクトルの大きさを求める
	float t = 0.0f;									//ベクトル係数の初期化
	if (len > 0.0f)									//直線の方向ベクトルの大きさが0以上なら
	{	//ベクトル係数を求める（(直線と直線上の点1〜点の内積)/方向ベクトルの大きさ)
		t = Dot(v1, p1 - l1) / len;
	}
	//最短距離=垂線の長さh-点p1 = ( l1 + t * v1 ) - p1（ゼロベクトルの場合はl1-p1の距離)
	return Length((l1 + t * v1) - p1);				//最短距離を返す
}

//=============================================================================
// 点と線分の最短距離
// s1s:線分の始点1
// s1g:線分の終点2
// p1 :点
// h  :最短距離(戻り値)
//=============================================================================
float DistancePS(D3DXVECTOR3 &s1s, D3DXVECTOR3 &s1g, D3DXVECTOR3 &p1)
{
	D3DXVECTOR3 v1 = s1g - s1s;				//s1s2ベクトル(直線のベクトル)
	D3DXVECTOR3 v2 = p1 - s1s;				//p3s1ベクトル
	D3DXVECTOR3 v3 = p1 - s1g;				//p3s2ベクトル

	D3DXVECTOR3 v1org = v1;					//一旦v1を保存
	Nomalise(v1);							//v1を正規化

	float t = Dot(v1, v2) / Length(v1);		//ベクトル係数を求める
	if (t < 0)								//点p1がs1s側の外側にある場合
	{
		return Length(v2);					//最短距離=p1〜s1sのベクトルの長さ
	}
	else if (t > 1)							//点p1がs1g側の外側にある場合
	{
		return Length(v3);					//最短距離=p1〜s1gのベクトルの長さ
	}
	else
	{										//点p1が線分の上にある場合
		return Length(v1org * t - v2);		//最短距離=点p1から線分に下した垂線
	}
}

//=============================================================================
// 2直線の最短距離
// l1 : 直線1上の点
// v1 : 直線1のベクトル
// l2 : 直線2上の点
// v2 : 直線2のベクトル
// t1 : 直線1のベクトル係数
// t2 : 直線2のベクトル係数
// p1 : 直線1の垂線の足の点
// p2 : 直線2の垂線の足の点
// 戻り値:最短距離
//=============================================================================
float DistanceLL(D3DXVECTOR3 &l1, D3DXVECTOR3 &v1, D3DXVECTOR3 &l2, D3DXVECTOR3 &v2)
{
	// 2直線が平行?
	if (Parallel(v1, v2) == true)
	{
		// l1上の点と直線l2の距離を求めて返す
		float len = DistancePL(l1, l2, v2);
		return len;
	}

	// 2直線は平行でない
	float dotl1l2 = Dot(v1, v2);							//直線1とl2の内積
	float lenl1 = LengthSq(v1);								//直線1のべき乗の長さ
	float lenl2 = LengthSq(v2);								//直線2のべき乗の長さ
	D3DXVECTOR3 vl1l2 = l1 - l2;							//直線1上の点と直線2上の点を結んだベクトル

	float t1 = (dotl1l2 * Dot(v2, vl1l2) - lenl2 * Dot(v1, vl1l2)) /
		(lenl1 * lenl2 - dotl1l2 * dotl1l2);			//直線1のベクトル係数を求める
	D3DXVECTOR3 p1 = GetPoint(l1, t1, v1);					//直線1の垂線の足の点を求める
	float t2 = Dot(v2, p1 - l2) / lenl2;					//直線2のベクトル係数を求める
	D3DXVECTOR3 p2 = GetPoint(l2, t2, v1);					//直線2の垂線の足の点を求める

	return Length(p2 - p1);									//垂線の足の点同士の距離(最短距離)を返す
}

//=============================================================================
// 2線分の最短距離
// s1s : S1(線分1)の始点
// s1g : S1(線分1)の終点
// s2s : S2(線分2)の始点
// s2g : S2(線分2)の終点
// p1　: 線分1の垂線の足の点
// p2　: 線分2の垂線の足の点
// t1　: 線分1のベクトル係数
// t2　: 線分2のベクトル係数
// 戻り値：最短距離
//=============================================================================
float DistanceSS(D3DXVECTOR3 &s1s, D3DXVECTOR3 &s1g, D3DXVECTOR3 &s2s, D3DXVECTOR3 &s2g)
{
	D3DXVECTOR3 v1 = s1g - s1s;										//s1ベクトル(直線のベクトル)
	D3DXVECTOR3 v2 = s2g - s2s;										//s2ベクトル(直線のベクトル)

	float t1 = VectorT(v1, v2);										//線分1のベクトル係数
	float t2 = VectorT(v2, v1);										//線分2のベクトル係数

	//s1が誤差以下の長さ)か?
	if (LengthSq(v1) < EPSILON)										//s1のベクトルが誤差より短ければ
	{
		//s2も誤差以下の長さか?
		if (LengthSq(v2) < EPSILON)									//s1s2のベクトルがともに誤差より短ければ
		{
			float len = Length(s2s - s1s);							//最短距離=s1の始点〜s2の始点
			D3DXVECTOR3 p1 = s1s;									//垂線1の足をs1の始点に設定
			D3DXVECTOR3 p2 = s2s;									//垂線2の足をs2の始点に設定
			t1 = t2 = 0.0f;											//s1もs2も縮退しているので、ベクトル係数は0
			return len;												//最短距離を返す
		}
		//s1のみ誤差以下の長さのとき
		else
		{
			float len = DistancePS(s2s, s2g, s1s);					//最短距離=s1の始点〜s2上の点
			D3DXVECTOR3 p1 = s1s;									//垂線1の足をs1の始点に設定	
			t1 = 0.0f;												//s1のベクトル係数は0
			Clamp(t2);												//s2のベクトル係数を0.0〜1.0に収める
			return len;												//最短距離を返す
		}
	}

	//s2が誤差以下の長さか?
	else if (LengthSq(v2) < EPSILON)								//s2のベクトルが誤差より短ければ
	{
		float len = DistancePS(s1s, s1g, s2s);						//最短距離=s2の始点〜垂線となるs1上の点
		D3DXVECTOR3 p2 = s2s;										//垂線2の足をs2の始点に設定
		t2 = 0.0f;													//s2のベクトル係数は0
		Clamp(t1);													//s1のベクトル係数を0.0〜1.0に収める
		return len;													//最短距離を返す
	}

	/* 線分同士 */
	if (Parallel(v1, v2) == true)									//2線分が平行だったら
	{//S1の始点を垂線の足p1に設定する
		t1 = 0.0f;													//s1のベクトル係数を0にする
		D3DXVECTOR3 p1 = s1s;										//垂線1の足をs1の始点に設定
		float len = DistancePS(s2s, s2g, p1);						//最短距離=s1の始点〜垂線となるs2上の点
		if (0.0f <= t2 && t2 <= 1.0f)								//垂線となる点がs2上にあれば
		{
			return len;												//最短距離を返す
		}
	}
	else
	{
		//線分はねじれの関係なので、2直線間の最短距離を求めて仮のt1,t2を求める
		float len = DistanceLL(s1s, v1, s2s, v2);					//最短距離=s1s2の垂線の足の点同士の距離
		if (0.0f <= t1 && t1 <= 1.0f &&0.0f <= t2 && t2 <= 1.0f)	//最短距離を結ぶ点がs1s2ともに線分上にあれば
		{
			return len;												//最短距離を返す
		}
	}

	//どちらかの垂線の足の点が線分の外
	Clamp(t1);														//s1側のt1を0〜1の間にクランプ
	D3DXVECTOR3 p1 = GetPoint(s1s, t1, v1);							//s1上の任意の点をp1に設定
	float lenss = DistancePS(s2s, s2g, p1);							//最短距離=s1上の点〜垂線の足となるs2上の点
	if (0.0f <= t2 && t2 <= 1.0f)									//垂線の足がs2上にあれば
	{
		return lenss;												//最短距離を返す
	}

	//s2側の垂線の点が線分の外
	Clamp(t2);														//s2側のt2を0〜1の間にクランプ
	D3DXVECTOR3 p2 = GetPoint(s2s, t2, v2);							//s2上の任意の点をp2に設定
	lenss = DistancePS(s1s, s1g, p2);								//最短距離=s2上の点〜垂線の足となるs1上の点
	if (0.0f <= t1 && t1 <= 1.0f)									//垂線の足がs1上にあれば
	{
		return lenss;												//最短距離を返す
	}

	//最短距離=s2の端点〜s1の端点
	Clamp(t1);														//s1側のt1を0〜1の間にクランプ
	p1 = GetPoint(s1s, t1, v1);										//s1上の任意の点をp1に設定
	return Length(p2 - p1);											//最短距離を返す
}

//=============================================================================
// カプセル同士の衝突判定
// s1s : S1(線分1)の始点
// s1g : S1(線分1)の終点
// s2s : S2(線分2)の始点
// s2g : S2(線分2)の終点
// r1 : カプセル1の半径
// r2 : カプセル2の半径
// 戻り値: 衝突していたらtrue
//=============================================================================
bool CheckHitCC(D3DXVECTOR3 &s1s, D3DXVECTOR3 &s1g, D3DXVECTOR3 &s2s, D3DXVECTOR3 &s2g, float &r1, float &r2)
{
	float d = DistanceSS(s1s, s1g, s2s, s2g);		//2線分の最短距離を求める
	return (d <= r1 + r2);							//最短距離がそれぞれのカプセル半径rの合計距離より短ければ衝突している(true)
}

//=============================================================================
// 衝突判定処理の更新
// 戻り値 : なし
//=============================================================================
void UpdateBattelCollision(void)
{
	srand((unsigned)time(NULL));	//ランダム初期値の初期化

	CAMERA *cam = GetCamera();
	PLAYER *player = GetPlayer();
	ENEMY *enemy = GetEnemy();
	ITEM *item = GetItem();
	SCORE *score = GetScore();

	//武器座標の取得
	D3DXVECTOR3 Wpos, Ws, We;
	GetweaponPos(Wpos, Ws, We);

	// モデルの半径を設定
	float r1, r2, r3;
	r1 = WEAPON_R;									//武器カプセルの半径
	r2 = ENEMY_R;									//エネミーカプセルの半径
	r3 = PLAYER_R;									//プレイヤーカプセルの半径

	// スコア計算用変数
	int hitcnt = 0;

	//武器と敵の当たり判定(スコア計算用）
	if (GetPlayer()->AttackMotion == true)							//プレイヤーが攻撃中の時
	{
		for (int i = 0; i < MAX_ENEMY; i++)							//エネミー分繰り返す
		{
			if (CheckHitBC(Wpos, (enemy + i)->pos, WEAPON_R * 3, ENEMY_R) == false) continue;	//武器の周りにいないエネミーは判定しない

			for (int j = 0; j < ENEMY_GROUP; j++)					//インスタンシングで表示している数分繰り返す
			{
				if ((enemy + i)->use == true)						//エネミーが生きていたら
				{
					//エネミーと武器でカプセル同士の当たり判定
					if (CheckHitCC(Ws, We, (enemy + i)->pos, D3DXVECTOR3((enemy + i)->pos.x + j * 10.0f, (enemy + i)->pos.y + ENEMY_LEN, (enemy + i)->pos.z + j * 20), r1, r2) == true)
					{
						hitcnt++;				//当たった回数をカウント
					}
				}
			}
		}
	}

	//武器と敵の当たり判定
	if (GetPlayer()->AttackMotion == true)							//プレイヤーが攻撃中の時
	{
		for (int i = 0; i < MAX_ENEMY; i++)							//エネミー分繰り返す
		{
			if (CheckHitBC(Wpos, (enemy + i)->pos, WEAPON_R * 3, ENEMY_R) == false) continue;	//武器の周りにいないエネミーは判定しない

			for (int j = 0; j < ENEMY_GROUP; j++)					//インスタンシングで表示している数分繰り返す
			{
				if ((enemy + i)->use == true)						//エネミーが生きていたら
				{
					//エネミーと武器でカプセル同士の当たり判定
					if (CheckHitCC(Ws, We, (enemy + i)->pos, D3DXVECTOR3((enemy + i)->pos.x + j * 10.0f, (enemy + i)->pos.y + ENEMY_LEN, (enemy + i)->pos.z + j * 20), r1, r2) == true)
					{
						(enemy + i)->hit_oldpos = (enemy + i)->pos;	//エネミーの今のposを保存
						player->player_hitstop = true;
						(enemy + i)->status = HIT;					//エネミーを吹っ飛ばす
						(enemy + i)->use = false;					//当たったらエネミーを消す
						ReleaseShadow(enemy[i].shadowIdx[j]);

						for (int n = 0; n < MAX_PARTICLE; n++)
						{
							GetParticle(i, n)->bUse = true;			//パーティクルを使用状態に
						}

						GetEffectNo(0);
						SetEffect((enemy + i)->hit_oldpos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.1f, 0.3f, 0.5f), EFFECT_SIZE_X, EFFECT_SIZE_Y, 60);
						SetEffect((enemy + i)->hit_oldpos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.1f, 0.3f, 0.5f), EFFECT_SIZE_X, EFFECT_SIZE_Y, 60);
						SetEffect((enemy + i)->hit_oldpos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.1f, 0.3f, 0.5f), EFFECT_SIZE_X, EFFECT_SIZE_Y, 60);

						// SE再生
						PlaySound(SOUND_LABEL_SE_ATTACK1);
						PlaySound(SOUND_LABEL_SE_HIT);

						if ((rand() % 100 + 1) <= 30)				//確率でアイテムをドロップ
						{
							(item + i)->bUse = true;
						}
						//スコア加算
						AddScore(hitcnt, score->score_mag);
					}
				}
			}
		}
	}

	for (int i = 0; i < MAX_ENEMY; i++)						//エネミー分繰り返す
	{
		if ((enemy + i)->use == true)						//エネミーが生きていたら
		{
			if (player->protect == false)					//プレイヤーが無敵状態じゃなければ
			{
				if ((enemy + i)->status == ATTACK)			//エネミーが攻撃中なら
				{	//プレイヤーとエネミーの当たり判定
					if (CheckHitCC(player->pos, D3DXVECTOR3(player->pos.x, player->pos.y + ENEMY_LEN, player->pos.z), (enemy + i)->pos, D3DXVECTOR3((enemy + i)->pos.x, (enemy + i)->pos.y + ENEMY_LEN, (enemy + i)->pos.z), r3, r2) == true)
					{
						// SE再生
						PlaySound(SOUND_LABEL_SE_DAMEGE);

						player->protect = true;				//プレイヤーを無敵状態に
						ChangeLife(-1);						//ライフが減る
					}
				}
			}
		}
	}
}

//=============================================================================
// 扇形と点の衝突判定
// p     : 点(衝突判定の相手側のpos）
// Fp    : 扇形のpos（衝突判定を行う自分側のpos）
// Fat   : 扇形の方向ベクトル(衝突判定を行う際に向いている方向）
// sita  : 扇形の角度（視野角）
// r1    : 扇形の半径（索敵範囲）
// r2	 : カメラ〜プレイヤーまでの距離(円錐の半径r1からr2を除いた区域を衝突とする)
// 戻り値: 衝突していたらtrue
//=============================================================================
bool CheckHitFP(D3DXVECTOR3 p, D3DXVECTOR3 Fp, D3DXVECTOR3 Fat, float sita, float r1, float r2)
{
	//yの値は考えないので全て初期化する
	p.y = 0;
	Fp.y = 0;
	Fat.y = 0;

	if (r1*r1 > LengthSq(p - Fp))							//点と半径rの円が衝突しているか（円の半径(扇形の長さ)rと点pまでの距離が円の半径以下）
	{
		if (r2*r2 < LengthSq(p - Fp))						//プレイヤーより前で衝突しているか
		{
			Fat = Nomalise(Fat);							//扇形の方向ベクトルを正規化
			D3DXVECTOR3 buf = Nomalise(p - Fp);				//点と扇形の中心点のベクトルを正規化する
			//PrintDebugProc("Fat %ff%ff Dot:%f\n", Fat.x, Fat.z, D3DXToDegree(acosf(Dot(buf, Fat))));
			if (sita > D3DXToDegree(acosf(Dot(buf, Fat))))	//扇形の方向ベクトルと点〜扇形中心点のベクトルで作られる角度が扇形の角度より小さければ
			{
				return true;								//衝突している
			}
		}
	}
	return false;											//衝突していない
}

//=============================================================================
// BBによる当たり判定処理
// 回転は考慮しない
// 戻り値：当たってたらtrue
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
// BCによる当たり判定処理
// サイズは半径
// 戻り値：当たってたらtrue
//=============================================================================
bool CheckHitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2)
{
	float len = (size1 + size2) * (size1 + size2);	// 半径を2乗した物
	D3DXVECTOR3 temp = pos1 - pos2;
	float fLengthSq = D3DXVec3LengthSq(&temp);		// 2点間の距離（2乗した物）

	if (len > fLengthSq)
	{
		return true;
	}

	return false;
}

//=============================================================================
//内積
//=============================================================================
float Dot(D3DXVECTOR3 &a, D3DXVECTOR3 &b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

//=============================================================================
//外積
//=============================================================================
D3DXVECTOR3 Cross(D3DXVECTOR3 &a, D3DXVECTOR3 &b)
{
	return D3DXVECTOR3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

//=============================================================================
//べき乗の長さ
//=============================================================================
float LengthSq(D3DXVECTOR3 &a)
{
	return a.x * a.x + a.y * a.y + a.z * a.z;
}

//=============================================================================
//ベクトルの大きさ※長さ(sqrtf=平方根を計算し結果をfloat型で返す)
//=============================================================================
float Length(D3DXVECTOR3 &a)
{
	float lengthsq = a.x * a.x + a.y * a.y + a.z * a.z;
	return sqrtf(lengthsq);
}

//=============================================================================
//平行関係?
//=============================================================================
bool Parallel(D3DXVECTOR3 &a, D3DXVECTOR3 &b)
{
	D3DXVECTOR3 crossV = Cross(a, b);		//aとbの外積を求める
	float d = LengthSq(crossV);
	if (-EPSILON < d && d < EPSILON)
	{
		return true;						//誤差範囲内なら平行なのでtrueを返す
	}
	return false;							//それ以外は平行でないのでfalseを返す
}

//=============================================================================
//ベクトルに係数tを乗算して線上の任意の点を取得
//=============================================================================
D3DXVECTOR3 GetPoint(D3DXVECTOR3 &p, float t, D3DXVECTOR3&v)
{
	return p + t * v;						//直線上の点p+ベクトル係数t*直線のベクトルv
}

//=============================================================================
//ベクトル係数tを求める
//=============================================================================
float VectorT(D3DXVECTOR3 &v1, D3DXVECTOR3 &v2)
{
	D3DXVECTOR3 newv1 = Nomalise(v1);				//v1の正規化
	float t = Dot(newv1, v2) / Length(newv1);		//ベクトル係数を求める
	return t;										//ベクトル係数を返す
}

//=============================================================================
//正規化
//=============================================================================
D3DXVECTOR3 Nomalise(D3DXVECTOR3 v1)
{
	float len = Length(v1);		//�@直線の方向ベクトルの大きさを求める
	len = 1 / len;				//�A単位ベクトルを求めるための値をだすため1を�@で割る
	v1 *= len;					//�B単位ベクトル（長さ1のベクトル）を求める
	return v1;
}
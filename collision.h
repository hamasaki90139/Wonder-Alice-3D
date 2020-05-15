//=============================================================================
//
// 衝突判定処理 [collision.h]
// Author : 濱崎 小春
//
//=============================================================================
#include "stdio.h"
#include "math.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#ifndef __IKD_OX_PRIMITIVE_H__
#define __IKD_OX_PRIMITIVE_H__
#define EPSILON   (0.000001f)	// 誤差

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void Clamp(float &v);
bool SharpAngle(D3DXVECTOR3 &p1, D3DXVECTOR3 &p2, D3DXVECTOR3 &p3);
float DistancePL(D3DXVECTOR3 &p1, D3DXVECTOR3 &s1, D3DXVECTOR3 &v1);
float DistancePS(D3DXVECTOR3 &s1s, D3DXVECTOR3 &s1g, D3DXVECTOR3 &p1);
float DistanceLL(D3DXVECTOR3 &l1, D3DXVECTOR3 &v1, D3DXVECTOR3 &l2, D3DXVECTOR3 &v2);
float DistanceSS(D3DXVECTOR3 &s1s, D3DXVECTOR3 &s1g, D3DXVECTOR3 &s2s, D3DXVECTOR3 &s2g);
bool CheckHitCC(D3DXVECTOR3 &s1s, D3DXVECTOR3 &s1g, D3DXVECTOR3 &s2s, D3DXVECTOR3 &s2g, float &r1, float &r2);
float Dot(D3DXVECTOR3 &a, D3DXVECTOR3 &b);
D3DXVECTOR3 Cross(D3DXVECTOR3 &a, D3DXVECTOR3 &b);
float Length(D3DXVECTOR3 &a);
float LengthSq(D3DXVECTOR3 &a);
bool Parallel(D3DXVECTOR3 &a, D3DXVECTOR3 &b);
D3DXVECTOR3 GetPoint(D3DXVECTOR3 &p, float t, D3DXVECTOR3&v);
float VectorT(D3DXVECTOR3 &v1, D3DXVECTOR3 &v2);
void UpdateBattelCollision(void);
bool CheckHitFP(D3DXVECTOR3 p, D3DXVECTOR3 Fp, D3DXVECTOR3 Fat, float sita, float r1, float r2);
D3DXVECTOR3 Nomalise(D3DXVECTOR3 v1);
bool CheckHitBB(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR2 size1, D3DXVECTOR2 size2);
bool CheckHitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2);

#endif

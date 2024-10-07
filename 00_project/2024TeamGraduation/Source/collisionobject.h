//=============================================================================
// 
// 当たり判定オブジェクト処理 [collisionobject.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _COLLISIONOBJECT_H_
#define _COLLISIONOBJECT_H_		// 二重インクルード防止のマクロを定義する

#include "objectBillboard.h"

//==========================================================================
// クラス定義
//==========================================================================
// 当たり判定オブジェクトクラス定義
class CCollisionObject : public CObjectBillboard
{
public:

	enum eMyTag
	{
		TAG_NONE = 0,	// なし
		TAG_PLAYER ,	// プレイヤー
		TAG_ENEMY,		// 敵
		TAG_MAX
	};

	CCollisionObject(int nPriority = 5);
	~CCollisionObject();

	static CCollisionObject *Create();
	static CCollisionObject *Create(const MyLib::Vector3 pos, const MyLib::Vector3 move, const float fRadius, const int nLife, const int nDamage, eMyTag tag);

	//  オーバーライドされた関数
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

private:

	// メンバ関数
	void CollisionEnemy();	// 敵との当たり判定
	void CollisionPlayer();	// プレイヤーとの当たり判定

	// メンバ変数
	float m_fRadius;		// 半径
	int m_nDamage;			// ダメージ
	int m_nLife;			// 寿命
	int m_nMaxLife;			// 最大寿命(固定)
	eMyTag m_MyTag;			// 自分のタグ
	static int m_nIdxTex;	// テクスチャのインデックス番号
};

#endif
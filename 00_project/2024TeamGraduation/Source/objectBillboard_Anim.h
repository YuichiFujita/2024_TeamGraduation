//=============================================================================
// 
//  オブジェクトビルボードアニメヘッダー [objectBillboard_Anim.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _OBJECTBILLBOARD_ANIM_H_
#define _OBJECTBILLBOARD_ANIM_H_	// 二重インクルード防止

#include "objectBillboard.h"

//==========================================================================
// クラス定義
//==========================================================================
// オブジェクトビルボードアニメクラス定義
class CObjectBillboardAnim : public CObjectBillboard
{
public:

	CObjectBillboardAnim(int nPriority = mylib_const::PRIORITY_DEF2D);
	virtual ~CObjectBillboardAnim();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual HRESULT Init(const int nDivisionU, const int nDivisionV, const float fInterval, bool bAutoDeath = true);
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void SetVtx() override;

	void SetDesableAutoDeath() { m_bAutoDeath = false; }	// 自動削除の判定削除
	bool IsFinish() { return m_bFinish; }					// 終了の判定
	static CObjectBillboardAnim *Create(const MyLib::Vector3& pos, const int nDivisionU, const int nDivisionV, const float fInterval, bool bAutoDeath = true);
	CObjectBillboardAnim *GetObject3DAnim() { return this; }
protected:

private:
	float m_fTimerAnim;		// アニメーションのカウンター
	float m_fIntervalAnim;	// アニメーションのインターバル
	int m_nPatternAnim;		// アニメーションのパターン
	int m_nDivisionU;		// Uの分割数
	int m_nDivisionV;		// Vの分割数
	float m_fSplitValueU;	// Uのスプライト量
	float m_fSplitValueV;	// Vのスプライト量
	bool m_bAutoDeath;		// 自動削除のフラグ
	bool m_bFinish;			// アニメーションが終わった判定
};



#endif
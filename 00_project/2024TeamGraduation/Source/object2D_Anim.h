//=============================================================================
// 
//  オブジェクト2Dアニメヘッダー [object2D_Anim.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _OBJECT2D_ANIM_H_
#define _OBJECT2D_ANIM_H_	// 二重インクルード防止

#include "object2D.h"

//==========================================================================
// クラス定義
//==========================================================================
// オブジェクト2Dアニメクラス定義
class CObject2D_Anim : public CObject2D
{
public:

	CObject2D_Anim(int nPriority = mylib_const::PRIORITY_DEF2D);
	virtual ~CObject2D_Anim();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual HRESULT Init(const int nDivisionU, const int nDivisionV, const int nInterval, bool bAutoDeath = true);
	virtual void Update() override;
	virtual void SetVtx() override;

	void SetDesableAutoDeath() { m_bAutoDeath = false; }	// 自動削除の判定削除
	bool IsFinish() { return m_bFinish; }					// 終了の判定
	static CObject2D_Anim *Create(MyLib::Vector3 pos, const int nDivisionU, const int nDivisionV, const int nInterval, bool bAutoDeath = true);
	CObject2D_Anim *GetObject2DAnim() { return this; }
protected:

private:
	int m_nCntAnim;			// アニメーションのカウンター
	int m_nPatternAnim;		// アニメーションのパターン
	int m_nDivisionU;		// Uの分割数
	int m_nDivisionV;		// Vの分割数
	int m_nIntervalAnim;	// アニメーションのインターバル
	float m_fSplitValueU;	// Uのスプライト量
	float m_fSplitValueV;	// Vのスプライト量
	bool m_bAutoDeath;		// 自動削除のフラグ
	bool m_bFinish;			// アニメーションが終わった判定
};



#endif
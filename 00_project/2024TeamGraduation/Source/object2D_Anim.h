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
	virtual HRESULT Init(const int nDivisionU, const int nDivisionV, const float fInterval, bool bAutoDeath = true);
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void SetVtx() override;

	//--------------------------
	// 設定
	//--------------------------
	void SetDesableAutoDeath() { m_bAutoDeath = false; }		// 自動削除の判定削除
	void SetEnableAutoPlay(bool bAuto) { m_bAutoPlay = bAuto; }	// 自動再生の判定設定
	bool IsFinish() { return m_bFinish; }						// 終了の判定
	
	//--------------------------
	// アニメーション関連
	//--------------------------
	void SetPatternAnim(int nPattern)	{ m_nPatternAnim = nPattern; }	// アニメーションパターン設定
	int GetPatternAnim() const			{ return m_nPatternAnim; }		// アニメーションパターン取得

	//--------------------------
	// その他
	//--------------------------
	static CObject2D_Anim *Create(const MyLib::Vector3& pos, const int nDivisionU, const int nDivisionV, const float fInterval, bool bAutoDeath = true, int nPriority = mylib_const::PRIORITY_DEF2D);
	CObject2D_Anim *GetObject2DAnim() { return this; }

private:

	//=============================
	// メンバ関数
	//=============================
	float m_fTimerAnim;		// アニメーションのカウンター
	float m_fIntervalAnim;	// アニメーションのインターバル
	int m_nPatternAnim;		// アニメーションのパターン
	int m_nDivisionU;		// Uの分割数
	int m_nDivisionV;		// Vの分割数
	float m_fSplitValueU;	// Uのスプライト量
	float m_fSplitValueV;	// Vのスプライト量
	bool m_bAutoDeath;		// 自動削除のフラグ
	bool m_bAutoPlay;		// 自動再生のフラグ
	bool m_bFinish;			// アニメーションが終わった判定
};



#endif
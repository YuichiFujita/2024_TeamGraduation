//=============================================================================
// 
//  遷移なしフェードヘッダー [instantfade.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _INSTANTFADE_H_
#define _INSTANTFADE_H_	// 二重インクルード防止

//==========================================================================
// インクルード
//==========================================================================
#include "object2D.h"

//==========================================================================
// 前方宣言
//==========================================================================

//==========================================================================
// クラス定義
//==========================================================================
// 遷移なしフェードクラス
class CInstantFade
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum EState
	{
		STATE_NONE = 0,			// 何もしていない
		STATE_FADEOUT,			// フェードアウト
		STATE_FADEIN,			// フェードイン
		STATE_FADECOMPLETION,	// フェード完了
		STATE_MAX
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CInstantFade();
	~CInstantFade();

	//=============================
	// メンバ関数
	//=============================
	HRESULT Init();
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void Draw();

	bool IsCompletion() { return (m_state == EState::STATE_FADECOMPLETION); }	// 完了フラグ取得
	EState GetState() { return m_state; }	// 状態取得
	void SetFade(const D3DXCOLOR& FadeColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), float fDuration = 0.5f);	// フェード設定

	//=============================
	// 静的メンバ関数
	//=============================
	static CInstantFade* Create();
private:

	float m_fDuration;			// フェードまでの時間
	float m_fTimerMove;			// フェード時間
	CObject2D *m_aObject2D;		// オブジェクト2Dのオブジェクト
	EState m_state;				// 状態
};

#endif
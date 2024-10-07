//=============================================================================
// 
//  スキップUIヘッダー [skip_ui.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _SKIP_UI_H_
#define _SKIP_UI_H_	// 二重インクルード防止

#include "object2d.h"
class CObjectCircleGauge2D;

//==========================================================================
// クラス定義
//==========================================================================
// スキップUIクラス
class CSkip_UI : public CObject
{
public:
	//=============================
	// 状態列挙
	//=============================
	enum State
	{
		STATE_NONE = 0,	// なにもない
		STATE_FADEOUT,	// フェードアウト
		STATE_MAX
	};

	CSkip_UI(int nPriority = 8, CObject::LAYER layer = CObject::LAYER::LAYER_2D);
	~CSkip_UI();

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;


	void Kill();		// 削除
	void SetState(State state);	// 状態設定
	bool IsComplete() { return m_bCompleteSkip; }	// 完了判定

	//=============================
	// 静的関数
	//=============================
	static CSkip_UI* Create();

private:
	//=============================
	// メンバ関数
	//=============================
	// 状態系
	void StateFadeout();	// フェードアウト

	// その他
	void CreateButton();	// ボタン作成
	void CreateGauge();		// ゲージ作成
	void CreateUI();	// UI生成
	void KillUI();		// UI削除

	//=============================
	// メンバ変数
	//=============================
	// 状態系
	State m_state;				// 状態
	float m_fStateTime;			// 状態タイマー

	// その他
	float m_fPressTimer;	// 押下時間
	bool m_bCompleteSkip;	// スキップ完了
	CObject2D* m_pButton;	// ボタン
	CObjectCircleGauge2D* m_pObjCircleGauge;	// 円ゲージ
};


#endif
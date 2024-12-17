//=============================================================================
//
// 操作待機処理 [title_controllwait.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _TITLE_CONTROLLWAIT_H_		// このマクロ定義がされていなかったら
#define _TITLE_CONTROLLWAIT_H_		// 二重インクルード防止のマクロを定義する

//==========================================================================
// インクルードファイル
//==========================================================================
#include "titlescene.h"
#include "player.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CDressup;
class CTitleStudent;	// タイトルの生徒

//==========================================================================
// クラス定義
//==========================================================================
// 操作待機クラス
class CTitle_ControllWait : public CTitleScene
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum EState
	{
		STATE_WAIT = 0,	// 待機
		STATE_CONTROLL,	// 操作
		STATE_MAX
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CTitle_ControllWait();
	~CTitle_ControllWait();

	//=============================
	// メンバ関数
	//=============================
	virtual HRESULT Init() override;	// 初期化
	virtual void Uninit() override;		// 終了
	virtual void Kill() override;		// 削除
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CTitle_ControllWait::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];	// 状態関数

	//=============================
	// メンバ関数
	//=============================
	// 状態関数
	//--------------------------
	virtual void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// 状態更新
	void StateWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 待機
	void StateControll(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 操作


	//=============================
	// メンバ変数
	//=============================
	//--------------------------
	// 状態
	//--------------------------
	EState m_state;			// 状態
	float m_fStateTime;		// 状態時間

	//--------------------------
	// その他
	//--------------------------
	CTitleLogo* m_pLogo;	// ロゴ
	std::vector<CTitleStudent*> m_vecTitleStudent;	// タイトルの生徒
};

#endif
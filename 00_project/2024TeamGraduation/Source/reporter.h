//=============================================================================
// 
//  実況者ヘッダー [reporter.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _REPORTER_H_
#define _REPORTER_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "gamemanager.h"
#include "objectChara.h"
#include "listmanager.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CShadow;
class CReporterSet;

//==========================================================================
// クラス定義
//==========================================================================
// 実況者クラス定義
class CReporter : public CObjectChara
{
public:

	//=============================
	// 列挙型定義
	//=============================
	// モーション列挙
	enum EMotion
	{
		MOTION_DEF = 0,		// ニュートラル
		MOTION_DEF_INV,		// 逆ニュートラル
		MOTION_THOUT,		// 実況
		MOTION_WAIT,		// 待機
		MOTION_MAX
	};

	// 状態定義
	enum EState
	{
		STATE_NONE = 0,		// なにもない
		STATE_THOUT,		// 実況
		STATE_WAIT,			// 待機
		STATE_MAX
	};
	
	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CReporter(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CReporter();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill() override;	// 動的削除処理

	//=============================
	// メンバ関数
	//=============================
	void SetState(EState state);	// 状態設定

	//=============================
	// 静的関数
	//=============================
	// 生成処理
	static CReporter* Create(const CGameManager::ETeamSide& side);
	static CListManager<CReporter> GetList(const CGameManager::ETeamSide& side) { return m_List[side]; };	// リスト取得

protected:

	//=============================
	// メンバ関数
	//=============================
	void Debug();			// デバッグ処理

private:
	
	//=============================
	// 関数リスト
	//=============================
	typedef void(CReporter::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];							// 状態関数

	//=============================
	// メンバ関数
	//=============================
	//-----------------------------
	// 状態関数
	//-----------------------------
	virtual void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// 状態更新
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// なし
	void StateThout(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 実況
	void StateWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 待機

	//-----------------------------
	// モーション系関数
	//-----------------------------
	virtual void AttackAction(CMotionManager::AttackInfo ATKInfo, int nCntATK) override;		// 攻撃時処理
	virtual void AttackInDicision(CMotionManager::AttackInfo ATKInfo, int nCntATK) override;	// 攻撃判定中処理

	//-----------------------------
	// その他
	//-----------------------------
	void SetDefMotion();	// デフォモーション設定

	//=============================
	// メンバ変数
	//=============================
	//-----------------------------
	// 状態
	//-----------------------------
	EState m_state;		// 状態
	float m_fStateTime;	// 状態時間
	float m_fWaitTime;	// 待機時間

	//-----------------------------
	// その他変数
	//-----------------------------
	CGameManager::ETeamSide m_TeamSide;		// チームサイド
	CShadow* m_pShadow;						// 影
	CReporterSet* m_pReporterSet;			// 実況セット
	static CListManager<CReporter> m_List[CGameManager::ETeamSide::SIDE_MAX];	// リスト
};


#endif

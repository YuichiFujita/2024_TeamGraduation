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
		MOTION_DEF = 0,		// ニュートラルモーション
		MOTION_MAX
	};

	// 状態定義
	enum EState
	{
		STATE_NONE = 0,		// なにもない
		STATE_GO,			// 向かう
		STATE_COLLECT,		// 回収
		STATE_BACK,			// 戻る
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
	static CReporter* Create(const MyLib::Vector3& pos, const CGameManager::ETeamSide& side);

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
	void StateGo(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 向かう
	void StateCollect(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 回収
	void StateBack(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 戻る

	//-----------------------------
	// モーション系関数
	//-----------------------------
	virtual void AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK) override;		// 攻撃時処理
	virtual void AttackInDicision(CMotion::AttackInfo ATKInfo, int nCntATK) override;	// 攻撃判定中処理

	//=============================
	// メンバ変数
	//=============================
	//-----------------------------
	// 状態
	//-----------------------------
	EState m_state;		// 状態
	float m_fStateTime;	// 状態時間

	//-----------------------------
	// その他変数
	//-----------------------------
	CGameManager::ETeamSide m_TeamSide;		// チームサイド
	CShadow* m_pShadow;						// 影
	static CListManager<CReporter> m_List;	// リスト
};


#endif

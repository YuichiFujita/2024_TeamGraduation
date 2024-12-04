//=============================================================================
// 
//  保険の先生ヘッダー [schoolnurse.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _SCHOOLNURSE_H_
#define _SCHOOLNURSE_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "gamemanager.h"
#include "objectChara.h"
#include "listmanager.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayer;		// プレイヤー
class CShadow;		// 影
class CStretcher;	// 担架

//==========================================================================
// クラス定義
//==========================================================================
// 保険の先生クラス定義
class CSchoolNurse : public CObjectChara
{
public:

	//=============================
	// 列挙型定義
	//=============================
	// モーション列挙
	enum EMotion
	{
		MOTION_DEF = 0,				// ニュートラルモーション
		MOTION_WALK,				// 移動
		MOTION_COLLECT,				// 回収
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
	CSchoolNurse(CStretcher* pStretcher, int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CSchoolNurse();

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
	void AddCollectPlayer(CPlayer* pPlayer) { m_pCollectPlayer.push_back(pPlayer); }	// 回収するプレイヤー追加

	//=============================
	// 静的関数
	//=============================
	// 生成処理
	static CSchoolNurse* Create(CStretcher* pStretcher);

protected:

	//=============================
	// メンバ関数
	//=============================
	void Debug();			// デバッグ処理

private:
	
	//=============================
	// 関数リスト
	//=============================
	typedef void(CSchoolNurse::* STATE_FUNC)(const float, const float, const float);
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
	EState m_Oldstate;							// 前回の状態
	EState m_state;								// 状態
	float m_fStateTime;							// 状態時間

	//-----------------------------
	// その他変数
	//-----------------------------
	std::vector<CPlayer*> m_pCollectPlayer;		// 回収するプレイヤー
	CShadow* m_pShadow;							// 影
	CStretcher* m_pStretcher;					// 担架
	static CListManager<CSchoolNurse> m_List;	// リスト
};


#endif

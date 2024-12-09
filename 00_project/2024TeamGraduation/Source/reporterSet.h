//=============================================================================
// 
//  実況者セットヘッダー [reporterSet.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _REPORTERSET_H_
#define _REPORTERSET_H_		// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "gamemanager.h"
#include "objectChara.h"
#include "listmanager.h"
#include "objectX.h"

//==========================================================================
// 前方宣言
//==========================================================================

//==========================================================================
// クラス定義
//==========================================================================
// 実況者セットクラス
class CReporterSet : public CObjectX
{
public:
	
	//=============================
	// 列挙型定義
	//=============================
	// 状態定義
	enum EState
	{
		STATE_NONE = 0,		// なにもない
		STATE_GO,			// 向かう
		STATE_COLLECT,		// 回収
		STATE_BACK,			// 戻る
		STATE_MAX
	};

	CReporterSet(int nPriority = 6);
	~CReporterSet();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	void Kill() override;		// 削除

	//=============================
	// メンバ関数
	//=============================
	void SetState(EState state);							// 状態設定

	//=============================
	// 静的関数
	//=============================
	// 生成処理
	static CReporterSet* Create(const MyLib::Vector3& pos, const CGameManager::ETeamSide& side);

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CReporterSet::* STATE_FUNC)(const float, const float, const float);
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
	// その他
	//-----------------------------
	void CreateChair();	// 椅子生成
	void CreateMic();	// マイク生成

	//-----------------------------
	// その他
	//-----------------------------

	//=============================
	// メンバ変数
	//=============================
	//-----------------------------
	// 状態
	//-----------------------------
	EState m_Oldstate;		// 前回の状態
	EState m_state;			// 状態
	float m_fStateTime;		// 状態時間

	//-----------------------------
	// モデル
	//-----------------------------
	CObjectX* m_pChair;		// 椅子
	CObjectX* m_pMic;		// マイク

	//-----------------------------
	// その他変数
	//-----------------------------
	CShadow* m_pShadow;		// 影
	CGameManager::ETeamSide m_TeamSide;		// チームサイド
};


#endif
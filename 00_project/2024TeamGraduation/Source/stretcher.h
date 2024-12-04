//=============================================================================
// 
//  担架ヘッダー [stretcher.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _STRETCHER_H_
#define _STRETCHER_H_		// 二重インクルード防止

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
class CSchoolNurse;	// 保険の先生
class CPlayer;		// プレイヤー

//==========================================================================
// クラス定義
//==========================================================================
// 担架クラス
class CStretcher : public CObjectX
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

	CStretcher(int nPriority = 6);
	~CStretcher();

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
	void SetEnableEnd(bool bEnd) { m_bEndSetting = bEnd; }	// 終了の設定
	CPlayer* GetCollectPlayer();							// 回収するプレイヤー取得

	//=============================
	// 静的関数
	//=============================
	// 生成処理
	static CStretcher* Create(CPlayer* pPlayer);

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CStretcher::* STATE_FUNC)(const float, const float, const float);
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
	void PosAdjNurse();	// 先生の位置補正
	void AddCollectPlayer(CPlayer* pPlayer) { m_pCollectPlayer.push_back(pPlayer); }	// 回収するプレイヤー追加
	void CollectEndSetting();				// 回収終了の設定

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
	std::vector<CPlayer*> m_pCarryPlayer;		// 運ぶプレイヤー
	CSchoolNurse* m_pLeftNurse;					// 左の先生
	CSchoolNurse* m_pRightNurse;				// 右の先生
	CShadow* m_pShadow;							// 影
	float m_fWidth;								// 幅
	MyLib::Vector3 m_startPos;					// 開始位置
	bool m_bEndSetting;							// 終了のセッティング
	static CStretcher* m_pThisPtr;				// 自身のポインタ
};


#endif
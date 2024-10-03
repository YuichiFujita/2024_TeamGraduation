//=============================================================================
// 
//  死亡プレイヤーヘッダー [deadplayer.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _DEADPLAYER_H_
#define _DEADPLAYER_H_	// 二重インクルード防止

#include "listmanager.h"
#include "objectChara.h"
#include "motion.h"

// 前方宣言
class CShadow;
class CEffekseerObj;

//==========================================================================
// クラス定義
//==========================================================================
// 人クラス
class CDeadPlayer : public CObjectChara
{
public:
	
	//=============================
	// 状態列挙
	//=============================
	enum STATE
	{
		STATE_NONE = 0,	// なにもない
		STATE_UP,		// 上昇
		STATE_FADEOUT,	// フェードアウト
		STATE_MAX
	};
	
	enum MOTION
	{
		MOTION_DEF = 0,		// ニュートラル
		MOTION_DEAD,		// 移動
		MOTION_MAX
	};


	CDeadPlayer(int nPriority = mylib_const::ENEMY_PRIORITY);
	virtual ~CDeadPlayer();


	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	virtual void Kill();	// 削除

	void SetState(STATE state);		// 状態設定
	STATE GetState() { return m_state; }
	void SetStateTime(float time) { m_fStateTime = time; }	// 状態時間設定

	// モーション
	void SetMotion(int motionIdx);	// モーションの設定

	HRESULT LoadText(const std::string& filename);

	static CListManager<CDeadPlayer> GetListObj() { return m_List; }	// リスト取得
	static CDeadPlayer* Create(const MyLib::Vector3& pos);

protected:

	//=============================
	// メンバ関数
	//=============================
	// 状態更新系
	virtual void StateNone();		// 何もない状態
	virtual void StateUP();			// 上昇
	virtual void StateFadeOut();	// フェードアウト

	// その他関数
	virtual void AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK) override;		// 攻撃時処理
	virtual void AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK) override;	// 攻撃判定中処理

	//=============================
	// メンバ変数
	//=============================
	STATE m_state;							// 状態
	STATE m_Oldstate;						// 前回の状態
	float m_fStateTime;						// 状態カウンター
	D3DXCOLOR m_mMatcol;					// マテリアルの色

private:
	
	//=============================
	// 関数リスト
	//=============================
	typedef void(CDeadPlayer::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];	// 状態関数リスト

	void UpdateState();					// 状態更新処理
	void LimitArea(); // 大人の壁判定

	//=============================
	// メンバ変数
	//=============================
	CShadow *m_pShadow;			// 影の情報
	CEffekseerObj* m_pEfkEffect;	// エフェクシアオブジェクト
	static CListManager<CDeadPlayer> m_List;	// リスト
};



#endif
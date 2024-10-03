//=============================================================================
// 
//  人ヘッダー [people.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _ENEMY_H_
#define _ENEMY_H_	// 二重インクルード防止

#include "listmanager.h"
#include "objectChara.h"
#include "motion.h"

// 前方宣言
class CShadow;

//==========================================================================
// クラス定義
//==========================================================================
// 人クラス
class CPeople : public CObjectChara
{
public:
	
	// 状態列挙
	enum STATE
	{
		STATE_NONE = 0,		// なにもない
		STATE_FADEIN,		// フェードイン
		STATE_FADEOUT,		// フェードアウト
		STATE_MAX
	};

	enum MOTION
	{
		MOTION_DEF = 0,			// ニュートラル
		MOTION_WALK,			// 移動
		MOTION_MAX
	};

	enum TYPE
	{
		TYPE_KITE = 8,	// 凧
		TYPE_KITE2,	// 凧
		TYPE_KITE3,	// 凧
	};

	CPeople(int nPriority = mylib_const::ENEMY_PRIORITY);
	virtual ~CPeople();


	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	virtual void Kill();	// 削除

	void SetState(STATE state);				// 状態設定
	STATE GetState() { return m_state; }	// 状態取得
	void SetStateTime(float time) { m_fStateTime = time; }	// 状態時間設定

	// モーション
	void SetMotion(int motionIdx);	// モーションの設定

	HRESULT LoadText(const char *pFileName);

	static CListManager<CPeople> GetListObj() { return m_List; }	// リスト取得
	static CPeople* Create(const std::string& filename, const MyLib::Vector3& pos, const TYPE& type);

protected:

	//=============================
	// 構造体定義
	//=============================
	// モーションの判定
	struct SMotionFrag
	{
		bool bJump;			// ジャンプ中かどうか
		bool bATK;			// 攻撃中かどうか
		bool bKnockback;	// ノックバック中かどうか
		bool bMove;			// 移動中かどうか
		bool bCharge;		// チャージ中かどうか
		SMotionFrag() : bJump(false), bATK(false), bKnockback(false), bMove(false), bCharge(false) {}
	};

	//=============================
	// メンバ関数
	//=============================
	// 状態更新系
	virtual void StateNone();		// 何もない状態
	virtual void StateFadeIn();		// フェードイン
	virtual void StateFadeOut();	// フェードアウト

	// その他関数
	virtual void ProcessLanding();	// 着地時処理
	virtual void AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK) override;		// 攻撃時処理
	virtual void AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK) override;	// 攻撃判定中処理

	//=============================
	// メンバ変数
	//=============================
	STATE m_state;							// 状態
	STATE m_Oldstate;						// 前回の状態
	float m_fStateTime;						// 状態カウンター
	SMotionFrag m_sMotionFrag;				// モーションのフラグ
	D3DXCOLOR m_mMatcol;					// マテリアルの色
	MyLib::Vector3 m_TargetPosition;		// 目標の位置
	int m_flame;

private:
	
	//=============================
	// 関数リスト
	//=============================
	typedef void(CPeople::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];	// 状態関数リスト

	void UpdateState();					// 状態更新処理
	void Collision();					// 当たり判定
	void LimitArea(); // 大人の壁判定

	//=============================
	// メンバ変数
	//=============================
	float m_fMoveVelocity;	// 移動速度
	CShadow *m_pShadow;			// 影の情報
	static CListManager<CPeople> m_List;	// リスト
};



#endif
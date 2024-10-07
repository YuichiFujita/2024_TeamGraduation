//=============================================================================
// 
//  依頼人ヘッダー [result_people.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _RESULT_PEOPLE_H_
#define _RESULT_PEOPLE_H_	// 二重インクルード防止

#include "listmanager.h"
#include "objectChara.h"
#include "motion.h"

// 前方宣言
class CShadow;

//==========================================================================
// クラス定義
//==========================================================================
// 依頼人クラス
class CResultPeople : public CObjectChara
{
public:
	
	// 状態列挙
	enum STATE
	{
		STATE_NONE = 0,		// なにもない
		STATE_FADEIN,		// フェードイン
		STATE_FADEOUT,		// フェードアウト
		STATE_WAIT,			// 待機状態
		STATE_PASS,			// パス状態
		STATE_BYEBYE,		// バイバイ
		STATE_MAX
	};

	enum MOTION
	{
		MOTION_DEF = 0,	// ニュートラル
		MOTION_WALK,	// 移動
		MOTION_PASS,	// 渡す
		MOTION_BYEBYE,	// バイバイ
		MOTION_RESULT = 9,	// リザルト
		MOTION_MAX
	};


	CResultPeople(int nPriority = mylib_const::ENEMY_PRIORITY);
	virtual ~CResultPeople();


	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	virtual void Kill();	// 削除

	// 状態系
	void SetState(STATE state);		// 状態設定
	STATE GetState() { return m_state; }
	void SetStateTime(float time) { m_fStateTime = time; }	// 状態時間設定

	// モーション
	void SetMotion(int motionIdx);	// モーションの設定

	HRESULT LoadText(const std::string& pFileName);

	static CListManager<CResultPeople> GetListObj() { return m_List; }	// リスト取得
	static CResultPeople* Create(const MyLib::Vector3& pos);

protected:

	
	//=============================
	// メンバ関数
	//=============================
	// 状態更新系
	virtual void StateNone();		// 何もない状態
	virtual void StateFadeIn();		// フェードイン
	virtual void StateFadeOut();	// フェードアウト
	virtual void StateWait();		// 待機
	virtual void StateResult();		// パス
	virtual void StateByeBye();		// バイバイ

	// その他関数
	virtual void AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK) override;		// 攻撃時処理
	virtual void AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK) override;	// 攻撃判定中処理
	void CreateScroll();	// 巻き物生成

	//=============================
	// メンバ変数
	//=============================
	STATE m_state;			// 状態
	STATE m_Oldstate;		// 前回の状態
	float m_fStateTime;		// 状態カウンター
	D3DXCOLOR m_mMatcol;	// マテリアルの色


private:
	
	//=============================
	// 関数リスト
	//=============================
	typedef void(CResultPeople::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];	// 状態関数リスト

	//=============================
	// メンバ関数
	//=============================
	void UpdateState();		// 状態更新処理
	void Collision();		// 当たり判定
	void UpdateScroll();	// 巻き物の更新処理

	//=============================
	// メンバ変数
	//=============================
	CShadow *m_pShadow;				// 影の情報
	CObjectX* m_pScroll;			// 巻き物
	MyLib::Vector3 m_posDestScroll;	// 巻き物の元の位置
	float m_fScrollThrowTimer;		// 巻き物タイマー

	static CListManager<CResultPeople> m_List;	// リスト
};



#endif
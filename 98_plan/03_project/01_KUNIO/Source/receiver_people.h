//=============================================================================
// 
//  届け先ヘッダー [receiver_people.h]
//  Author : IbukiOkusada
// 
//=============================================================================

#ifndef _RECEIVER_PEOPLE_H_
#define _RECEIVER_PEOPLE_H_	// 二重インクルード防止

#include "listmanager.h"
#include "objectChara.h"
#include "motion.h"

// 前方宣言
class CShadow;
class CBaggage;

//==========================================================================
// クラス定義
//==========================================================================
// 依頼人クラス
class CReceiverPeople : public CObjectChara
{
private:

	// 回収距離列挙
	enum DISTANCE
	{
		DISTANCE_NEAR,	// 近い
		DISTANCE_FAR,	// 遠い
		DISTANCE_MAX
	};

public:
	
	// 状態列挙
	enum STATE
	{
		STATE_NONE = 0,		// なにもない
		STATE_FADEIN,		// フェードイン
		STATE_FADEOUT,		// フェードアウト
		STATE_WAIT,			// 待機状態
		STATE_GET,			// キャッチ状態
		STATE_RETURN,		// レシーブ状態
		STATE_YABAI,		// ヤバイ状態
		STATE_DIVE,			// ダイブ状態
		STATE_DROWN,		// 溺れ状態
		STATE_BYEBYE,		// バイバイ
		STATE_WALK,			// 歩く
		STATE_MAX
	};

	enum MOTION
	{
		MOTION_DEF = 0,	// ニュートラル
		MOTION_WALK,	// 移動
		MOTION_PASS,	// 渡す
		MOTION_BYEBYE,	// バイバイ
		MOTION_GET,		// キャッチ
		MOTION_RETURN,	// 遠くに飛ばしすぎてアウト
		MOTION_DROWN,	// 溺れる
		MOTION_DIVE,	// ダイブ
		MOTION_YABAI,	// ﾔﾊﾞｲ！
		MOTION_MAX
	};


	CReceiverPeople(int nPriority = mylib_const::ENEMY_PRIORITY);
	virtual ~CReceiverPeople();


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

	static CListManager<CReceiverPeople> GetListObj() { return m_List; }	// リスト取得
	static CReceiverPeople* Create(const MyLib::Vector3& pos);

protected:

	
	//=============================
	// メンバ関数
	//=============================
	// 状態更新系
	virtual void StateNone();		// 何もない状態
	virtual void StateFadeIn();		// フェードイン
	virtual void StateFadeOut();	// フェードアウト
	virtual void StateWait();		// 待機
	virtual void StatePass();		// パス
	virtual void StateGet();		// キャッチ
	virtual void StateReturn();		// レシーブ
	virtual void StateYabai();		// やばい
	virtual void StateDive();		// ダイブ
	virtual void StateDrown();		// 溺れる
	virtual void StateByeBye();		// バイバイ
	virtual void StateWalk();		// 歩く

	// その他関数
	virtual void AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK) override;		// 攻撃時処理
	virtual void AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK) override;	// 攻撃判定中処理

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
	typedef void(CReceiverPeople::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];	// 状態関数リスト

	void UpdateState();		// 状態更新処理
	void Collision();		// 当たり判定

	//=============================
	// メンバ変数
	//=============================
	MyLib::Vector3 m_StartPos;	// 開始地点
	MyLib::Vector3 m_StartRot;	// 向き
	CShadow *m_pShadow;			// 影の情報
	float m_fMoveTimer;			// 移動時間
	bool m_bEnd;				// 終了
	DISTANCE m_Distance;		// 回収距離
	static CListManager<CReceiverPeople> m_List;	// リスト
};



#endif
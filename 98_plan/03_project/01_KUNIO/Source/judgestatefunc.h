//=============================================================================
// 
//  判定状態関数ヘッダー [judgestatefunc.h]
//  Author : 石原颯馬
// 
//=============================================================================

#ifndef _JUDGE_STATE_FUNC_H_
#define _JUDGE_STATE_FUNC_H_	// 二重インクルード防止

//前方宣言
class CJudgeObj;

//==========================================================================
// クラス定義
//==========================================================================
// 基底
class CJudgeStateFunc
{
public:
	//状態列挙
	enum STATE
	{
		STATE_APPEAR = 0,
		STATE_WAIT,
		STATE_FADEOUT,
		STATE_MAX
	};

	// コンストラクタ・デストラクタ
	CJudgeStateFunc(CJudgeObj* pObj);
	~CJudgeStateFunc() {}

	// 状態関数
	void ActionState(){ (this->*(m_StateFuncList[m_state]))(); }
	virtual void StateAppear() = 0;		// 出現
	virtual void StateWait() = 0;		// 待ち
	virtual void StateFadeOut() = 0;	// フェードアウト

	// 取得・設定
	CJudgeObj* GetObj() { return m_pObj; }
	STATE GetState() { return m_state; }
	void SetState(const STATE state) { m_state = state; }
	float GetStateTime() { return m_fStateTime; }
	void SetStateTime(const float time) { m_fStateTime = time; }
	void AddStateTime(const float time) { m_fStateTime += time; }
private:
	//=============================
	// 関数リスト
	//=============================
	typedef void(CJudgeStateFunc::* STATE_FUNC)();
	static STATE_FUNC m_StateFuncList[];// 状態関数のリスト

	//=============================
	// 変数
	//=============================
	CJudgeObj* m_pObj;	//操作するオブジェクト
	STATE m_state;		// 状態
	float m_fStateTime;	// 状態タイマー
};

// AAA
class CJudgeStateFunc_AAA : public CJudgeStateFunc
{
public:
	// コンストラクタ・デストラクタ
	CJudgeStateFunc_AAA(CJudgeObj* pObj) : CJudgeStateFunc(pObj) {}
	~CJudgeStateFunc_AAA(){}

	// 状態関数
	void StateAppear();		// 出現
	void StateWait();		// 待ち
	void StateFadeOut();	// フェードアウト
};

// BBB
class CJudgeStateFunc_BBB : public CJudgeStateFunc
{
public:
	// コンストラクタ・デストラクタ
	CJudgeStateFunc_BBB(CJudgeObj* pObj) : CJudgeStateFunc(pObj) {}
	~CJudgeStateFunc_BBB(){}

	// 状態関数
	void StateAppear();		// 出現
	void StateWait();		// 待ち
	void StateFadeOut();	// フェードアウト
};

// CCC
class CJudgeStateFunc_CCC : public CJudgeStateFunc
{
public:
	// コンストラクタ・デストラクタ
	CJudgeStateFunc_CCC(CJudgeObj* pObj) : CJudgeStateFunc(pObj) {}
	~CJudgeStateFunc_CCC(){}

	// 状態関数
	void StateAppear();		// 出現
	void StateWait();		// 待ち
	void StateFadeOut();	// フェードアウト
};

// DDD
class CJudgeStateFunc_DDD : public CJudgeStateFunc
{
public:
	// コンストラクタ・デストラクタ
	CJudgeStateFunc_DDD(CJudgeObj* pObj) : CJudgeStateFunc(pObj) {}
	~CJudgeStateFunc_DDD(){}

	// 状態関数
	void StateAppear();		// 出現
	void StateWait();		// 待ち
	void StateFadeOut();	// フェードアウト
};


#endif
//==========================================================================
// 
//  リザルト王冠処理 [resultCrown.cpp]
//  Author : Kai Takada
// 
//==========================================================================

#ifndef _RESULTCROWN_H_
#define _RESULTCROWN_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "object.h"

//==========================================================================
// 前方宣言
//==========================================================================

//==========================================================================
// クラス定義
//==========================================================================
// タイトルロゴクラス
class CResultCrown : public CObject
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum EState
	{
		STATE_NONE = 0,	// なにもなし
		STATE_SPAWN,	// 登場
		STATE_LOOP,		// ループ
		STATE_START,	// 開始
		STATE_WAIT,		// 待機
		STATE_MAX
	};

	enum EResult
	{
		RESULT_WIN = 0,		// 王冠
		RESULT_DRAW,		// 引き分け
		RESULT_MAX
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CResultCrown(int nPriority = mylib_const::PRIORITY_DEFAULT, const LAYER layer = LAYER::LAYER_DEFAULT);
	~CResultCrown();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual HRESULT Init(EResult result);
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill();	// 削除
	virtual void SetEnableDisp(bool bDisp);	// 描画状況設定

	//=============================
	// メンバ関数
	//=============================
	void SetState(EState state);			// 状態設定
	EState GetState() { return m_state; }	// 状態取得
	void SetResult(EResult result) { m_result = result; }	// 結果設定
	EResult GetResult() { return m_result; }				// 結果取得
	void SetPosition(const MyLib::Vector3& pos) override;	// 位置設定
	void BindXData(std::string filepass);

	//=============================
	// 静的メンバ関数
	//=============================
	static CResultCrown* Create(EResult result = EResult::RESULT_DRAW);	// 生成

private:

	//=============================
	// 関数リスト
	//=============================
	// 状態関数
	typedef void(CResultCrown::*STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];	// 状態関数

	//=============================
	// メンバ変数
	//=============================
	// 状態
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 状態更新
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {}	// なし
	void StateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 登場
	void StateLoop(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ループ
	void StateStart(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 開始
	void StateWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 待機
	
	// 生成
	HRESULT CreateMain();	// 主生成

	// その他
	void UpdateMain();		// 主更新

	//=============================
	// メンバ変数
	//=============================
	// 状態
	EState m_state;		// 状態
	float m_fStateTime;	// 状態タイマー

	// 回転
	float m_fRotationTime;	// 回転タイマー
	float m_fIntervalRotate;	// 回転までの間隔
	float m_fRotationY;			// Y軸回転量

	// その他
	CObjectX* m_pMain;		// メインロゴ
	float m_fTime;			// タイマー
	EResult m_result;		// 結果
};

#endif

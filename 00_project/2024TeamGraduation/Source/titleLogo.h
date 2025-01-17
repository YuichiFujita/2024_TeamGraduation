//==========================================================================
// 
//  タイトルロゴヘッダー [titleLogo.h]
//  Author : Kai Takada
// 
//==========================================================================

#ifndef _TITLELOGO_H_
#define _TITLELOGO_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "object.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CEffekseerObj;

//==========================================================================
// クラス定義
//==========================================================================
// タイトルロゴクラス
class CTitleLogo : public CObject
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

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CTitleLogo(int nPriority = mylib_const::PRIORITY_DEFAULT, const LAYER layer = LAYER::LAYER_DEFAULT);
	~CTitleLogo();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
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

	//=============================
	// 静的メンバ関数
	//=============================
	static CTitleLogo* Create();	// 生成

private:

	//=============================
	// 関数リスト
	//=============================
	// 状態関数
	typedef void(CTitleLogo::*STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];	// 状態関数

	//=============================
	// メンバ変数
	//=============================
	// 状態
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 状態更新
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {}
	void StateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 登場
	void StateLoop(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ループ
	void StateStart(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 開始
	void StateWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 待機
	
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
	float m_fRotationTime;		// 回転タイマー
	float m_fIntervalRotate;	// 回転までの間隔
	float m_fRotationY;			// Y軸回転量

	// その他
	CObjectX* m_pMain;			// メインロゴ
	float m_fTime;				// タイマー
	CEffekseerObj* m_pEffect;	// エフェクト
};

#endif

//============================================================
//
//	タイマーUIヘッダー [timerUI.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TIMER_UI_H_
#define _TIMER_UI_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "timeUI.h"
#include "timer.h"

//************************************************************
//	クラス定義
//************************************************************
// タイマーUIクラス
class CTimerUI : public CTimeUI
{
public:
	// コンストラクタ
	CTimerUI();

	// デストラクタ
	~CTimerUI() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Kill() override;			// 動的削除
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新
	void Draw(void) override;						// 描画
	void SetTime(const float fTime) override;		// 時間設定
	float GetTime(void) const override	{ return m_pTimer->GetTime(); }	// 時間取得
	int GetMin(void) const override		{ return m_pTimer->GetMin(); }	// 分取得
	int GetSec(void) const override		{ return m_pTimer->GetSec(); }	// 秒取得
	int GetMSec(void) const override	{ return m_pTimer->GetMSec(); }	// ミリ秒取得

	//--------------------------
	// 静的メンバ関数
	//--------------------------
	/*
		@brief	生成処理
		@param	fTime			[in]	開始時間
		@param	fLimit			[in]	制限時間
		@param	rPos			[in]	位置
		@param	rSizeValue		[in]	数字の大きさ
		@param	rSizePart		[in]	区切りの大きさ
		@param	rSpaceValue		[in]	数字の空白
		@param	rSpacePart		[in]	区切りの空白
		@param	alignX			[in]	横配置
		@param	alignY			[in]	縦配置
		@param	rRot			[in]	向き
		@param	rCol			[in]	色
		@return	CTimerUI*
	*/
	static CTimerUI *Create	// 生成
	( // 引数
		const float fTime,				// 開始時間
		const float fLimit,				// 制限時間
		const D3DXVECTOR3& rPos,		// 位置
		const D3DXVECTOR2& rSizeValue,	// 数字の大きさ
		const D3DXVECTOR2& rSizePart,	// 区切りの大きさ
		const D3DXVECTOR2& rSpaceValue,	// 数字の空白
		const D3DXVECTOR2& rSpacePart,	// 区切りの空白
		const EAlignX alignX = XALIGN_CENTER,			// 横配置
		const EAlignY alignY = YALIGN_CENTER,			// 縦配置
		const D3DXVECTOR3& rRot = VEC3_ZERO,			// 向き
		const D3DXCOLOR& rCol = MyLib::color::White()	// 色
	);

	// メンバ関数
	void Start(void)	{ m_pTimer->Start(); }	// 計測開始
	void End(void)		{ m_pTimer->End(); }	// 計測終了
	void EnableStop(const bool bStop)	{ m_pTimer->EnableStop(bStop);}			// 計測停止設定
	void AddTime(const float fTime)		{ m_pTimer->AddTime(fTime); }			// 時間加算
	void SetLimit(const float fLimit)	{ m_pTimer->SetLimit(fLimit); }			// 制限時間設定
	float GetLimit(void) const			{ return m_pTimer->GetLimit(); }		// 制限時間取得
	CTimer::EState GetState(void) const	{ return m_pTimer->GetState(); }		// 計測状態取得
	bool IsEnd() { return m_pTimer->GetState() == CTimer::EState::STATE_END; }	// 終了判定

private:
	// メンバ変数
	CTimer *m_pTimer;	// タイマー管理情報
};

#endif	// _TIMER_UI_H_

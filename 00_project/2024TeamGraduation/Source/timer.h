//============================================================
//
//	タイマーヘッダー [timer.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TIMER_H_
#define _TIMER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"
#include "listmanager.h"

//************************************************************
//	定数宣言
//************************************************************
namespace timer
{
	const float	TIME_MIN = 0.0f;	// 最少タイム
	const float	TIME_MAX = 60.0f * 99.0f + 59.0f + 0.999f;	// 最大タイム
}

//************************************************************
//	クラス定義
//************************************************************
// タイマークラス
class CTimer : public CObject
{
public:
	// 計測列挙
	enum EState
	{
		STATE_NONE = 0,	// 処理なし
		STATE_COUNT,	// 計測中
		STATE_END,		// 計測終了 
		STATE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CTimer();

	// デストラクタ
	~CTimer() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Kill() override;			// 動的削除処理
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新
	void Draw(void) override;		// 描画

	// 静的メンバ関数
	static CTimer *Create	// 生成
	( // 引数
		const float fTime,	// 開始時間
		const float fLimit	// 制限時間
	);
	static void EnableStopAll(const bool bStop);	// 全タイマー計測状況設定
	static CListManager<CTimer> GetList(void) { return m_list; }	// リスト取得

	// メンバ関数
	void Start(void);	// 計測開始
	void End(void);		// 計測終了
	void EnableStop(const bool bStop);	// 計測停止設定
	void AddTime(const float fTime);	// 時間加算
	void SetTime(const float fTime);	// 時間設定
	void SetLimit(const float fLimit);	// 制限時間設定
	float GetTime(void) const	{ return m_fTime; }		// 時間取得
	float GetLimit(void) const	{ return m_fLimit; }	// 制限時間取得
	EState GetState(void) const	{ return m_state; }		// 計測状態取得
	int GetMin(void) const		{ return m_nMin; }		// 分取得
	int GetSec(void) const		{ return m_nSec; }		// 秒取得
	int GetMSec(void) const		{ return m_nMSec; }		// ミリ秒取得

private:
	// メンバ関数
	void CountUp(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// カウントアップ
	void CountDown(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// カウントダウン
	void CalcTime(void);	// 時間計算

	// 静的メンバ変数
	static CListManager<CTimer> m_list;	// オブジェクトリスト

	// メンバ変数
	std::function<void(const float, const float, const float)> m_funcCount;	// 計測関数ポインタ
	EState	m_state;	// 計測状態
	float	m_fTime;	// 計測時間
	float	m_fLimit;	// 制限時間
	bool	m_bStop;	// 計測停止状況
	long	m_lTime;	// 計測ミリ秒
	int		m_nMin;		// 分
	int		m_nSec;		// 秒
	int		m_nMSec;	// ミリ秒
};

#endif	// _TIMER_H_

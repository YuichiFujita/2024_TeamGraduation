//==========================================================================
// 
//  オブジェクト3Dアニメヘッダー [object3D_Anim.h]
//  Author : 相馬靜雅
//  Adder  : 藤田勇一
// 
//==========================================================================

#ifndef _OBJECT3D_ANIM_H_
#define _OBJECT3D_ANIM_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "object3D.h"

//==========================================================================
// クラス定義
//==========================================================================
// オブジェクト3Dアニメクラス定義
class CObject3DAnim : public CObject3D
{
public:

	//=============================
	// 定数
	//=============================
	static constexpr float DEF_NEXT = 1.0f;	// パターン変更時間 (デフォルト)

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CObject3DAnim(int nPriority = mylib_const::PRIORITY_DEF2D);
	virtual ~CObject3DAnim();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void SetVtx() override;

	//=============================
	// メンバ関数
	//=============================
	void SetCurPtrn(const int nPtrn);				// 現在パターン設定
	void SetTexPtrn(const MyLib::PosGrid2& rPtrn);	// テクスチャ分割数設定
	void SetTexPtrnWidth(const int nTexPtrnW);		// テクスチャ横分割数設定
	void SetTexPtrnHeight(const int nTexPtrnH);		// テクスチャ縦分割数設定
	void SetEnableStop(const bool bStop);			// 停止フラグ設定
	void SetEnablePlayBack(const bool bPlayBack);	// 逆再生フラグ設定
	void SetEnableLoop(const bool bLoop);			// ループフラグ設定
	void ResetCurPtrn();							// 現在パターン初期化
	void SetNextTime(const int nPtrnIdx, const float fNextTime);	// パターン変更時間設定 (パターン指定)
	void SetNextTime(const float fNextTime);						// パターン変更時間設定 (全パターン)

	inline int GetCurPtrn() const	{ return m_nCurPtrn; }	// 現在パターン取得
	inline void ResetNumLoop()		{ m_nNumLoop = 0; }		// パターン繰り返し数初期化
	inline int GetLoopAnim() const	{ return m_nNumLoop; }	// パターン繰り返し数取得
	inline bool IsStop() const		{ return m_bStop; }		// 停止フラグ取得
	inline bool IsPlayBack() const	{ return m_bPlayBack; }	// 逆再生フラグ取得
	inline bool IsLoop() const		{ return m_bLoop; }		// ループフラグ取得
	inline bool IsFinish() const	{ return m_bFinish; }	// 終了フラグ取得
	inline float GetCurTime() const			{ return m_fCurTime; }		// 現在の待機時間
	inline float GetCurWholeTime() const	{ return m_fCurWholeTime; }	// 現在の全体時間
	inline float GetMaxWholeTime() const	{ return m_fMaxWholeTime; }	// 総全体時間
	inline void SetDesableAutoDeath()		{ m_bAutoDeath = false; }	// 自動削除の判定削除
	inline CObject3DAnim *GetObject3DAnim()	{ return this; }			// 自身の型オブジェクト取得

	//=============================
	// 静的メンバ関数
	//=============================
	static CObject3DAnim* Create	// 生成
	( // 引数
		const MyLib::PosGrid2& rPtrn,		// テクスチャ分割数
		const MyLib::Vector3& rPos,			// 位置
		const float fNextTime = DEF_NEXT,	// パターン変更時間
		const bool bAutoDeath = false		// 自動破棄フラグ
	);

private:

	//=============================
	// メンバ関数
	//=============================
	HRESULT SetMaxPtrn(const int nMaxPtrn);	// パターン総数設定
	bool NextPtrn(const float fDeltaTime, const float fSlowRate);	// パターン加算
	bool BackPtrn(const float fDeltaTime, const float fSlowRate);	// パターン減算

	//=============================
	// メンバ変数
	//=============================
	std::function<bool(float, float)> m_funcPattern;	// パターン変更関数ポインタ
	MyLib::PosGrid2 m_ptrn;	// テクスチャ分割数
	MyLib::Vector2 m_split;	// スプライト量
	float* m_pNextTime;		// パターン変更時間
	float m_fCurTime;		// 現在の待機時間
	float m_fCurWholeTime;	// 現在の全体時間
	float m_fMaxWholeTime;	// 総全体時間
	int m_nCurPtrn;			// 現在のパターン
	int m_nMaxPtrn;			// パターン総数
	int m_nNumLoop;			// パターン繰り返し数
	bool m_bStop;			// 停止フラグ
	bool m_bPlayBack;		// 逆再生フラグ
	bool m_bLoop;			// ループフラグ
	bool m_bFinish;			// 終了フラグ
	bool m_bAutoDeath;		// 自動破棄フラグ
};



#endif
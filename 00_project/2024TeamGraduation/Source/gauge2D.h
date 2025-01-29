//============================================================
//
//	ゲージ2Dヘッダー [gauge2D.h]
//	Author：Kai Takada
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _GAUGE2D_H_
#define _GAUGE2D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object2D.h"
#include "gamemanager.h"

//************************************************************
//　前方宣言
//************************************************************
class CString2D;

//************************************************************
//	クラス定義
//************************************************************
// ゲージ2Dクラス
class CGauge2D : public CObject
{
public:
	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,	// 通常状態
		STATE_CHANGE,	// ゲージ変動状態
		STATE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	explicit CGauge2D(const float nFrame);

	// デストラクタ
	~CGauge2D() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Kill() override;		// 削除
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新
	void Draw() override;		// 描画
	void SetEnableDisp(const bool bDisp) override;	// 描画状況設定

	// 静的メンバ関数
	static CGauge2D* Create	// 生成
	( // 引数
		const float fMax,				// 最大表示値
		const float fFrame,				// 表示値変動フレーム
		const std::string& rTeamName,	// チーム名
		const MyLib::Vector3& rPos,		// 位置
		const MyLib::Vector3& rOffsetFrame,		// フレームオフセット
		const MyLib::Vector3& rOffsetAssist,	// アシストオフセット
		const MyLib::Vector3& rOffsetName,		// チーム名オフセット
		const MyLib::Vector2& rSizeGauge,		// ゲージ大きさ
		const MyLib::Vector2& rSizeFrame,		// フレーム大きさ
		const MyLib::Vector2& rSizeAssist,		// アシスト大きさ
		const D3DXCOLOR& rColFront = MyLib::color::White(),	// 表ゲージ色
		const D3DXCOLOR& rColBack = MyLib::color::Black() 	// 裏ゲージ色
	);
	static inline void  SetBrightTime(float time)		{ m_fBrightTime = time; }		// maxの時光るカウンター設定
	static inline float GetBrightTime()					{ return m_fBrightTime; }		// maxの時光るカウンター取得
	static inline void  SetBrightTimeEnd(float time)	{ m_fBrightTimeEnd = time; }	// maxの時光る終了タイミング設定
	static inline float GetBrightTimeEnd()				{ return m_fBrightTimeEnd; }	// maxの時光る終了タイミング取得
	static inline void  SetAssistTime(float time)		{ m_fAssistTime = time; }		// アシストカウンター設定
	static inline float GetAssistTime()					{ return m_fAssistTime; }		// アシストカウンター取得
	static inline void  SetAssistTimeEnd(float time)	{ m_fAssistTimeEnd = time; }	// アシスト終了タイミング設定
	static inline float GetAssistTimeEnd()				{ return m_fAssistTimeEnd; }	// アシスト終了タイミング取得

	// メンバ関数
	CObject2D* GetBg() { return m_pBg; }		// 背景取得
	CObject2D* GetBar() { return m_pBar; }		// ゲージ取得
	CObject2D* GetFrame() { return m_pFrame; }	// フレーム取得
	void BindTextureBG(const int nTexID);		// 背景テクスチャ割当
	void BindTextureBar(const int nTexID);		// ゲージテクスチャ割当
	void BindTextureFrame(const int nTexID);	// フレームテクスチャ割当
	void BindTextureAssist(const int nTexID);	// アシストテクスチャ割当
	void AddNum(const float fAdd);				// ゲージ加算
	void SubNum(const float fSub);				// ゲージ加算
	void SetNum(const float nNum);				// ゲージ設定
	void SetMaxNum(const float nMax);			// ゲージ最大値設定
	inline float GetNum() const					{ return m_fNumGauge; }		// ゲージ取得
	inline float GetMaxNum() const				{ return m_fMaxNumGauge; }	// ゲージ最大値取得
	void InitPosition();									// 位置初期化
	void InitSize();										// 大きさ初期化
	void SetPosition(const MyLib::Vector3& rPos) override;	// 位置設定
	void SetTexUV(const std::vector<D3DXVECTOR2>& uv);		// テクスチャ座標の設定
	void SetAnchorType(const CObject2D::AnchorPoint& type);	// アンカーポイント設定
	void SetSizeGaugeRadius(const float fRadius);			// 割合からゲージ大きさ設定
	void SetSizeGauge(const MyLib::Vector2& rSize);			// ゲージ大きさ設定
	void SetSizeFrame(const MyLib::Vector2& rSize);			// フレーム大きさ設定
	void SetSizeAssist(const MyLib::Vector2& rSize);		// アシスト大きさ設定
	void SetOffsetFrame(const MyLib::Vector3& rOffset);		// フレームオフセット設定
	void SetOffsetAssist(const MyLib::Vector3& rOffset);	// アシストオフセット設定
	void SetOffsetTeamName(const MyLib::Vector3& rOffset);	// チーム名オフセット設定
	void SetColorFront(const D3DXCOLOR& rCol);				// ゲージ色設定
	void SetColorBack(const D3DXCOLOR& rCol);				// 背景ゲージ色設定
	void SetAlignX(const EAlignX alignX);					// 横配置設定
	void SetString(const std::string& rStr);				// 文字列設定
	inline void SetTeam(CGameManager::ETeamSide team)		{ m_team = team; }	// チーム設定
	inline CGameManager::ETeamSide	GetTeam() const			{ return m_team; }	// チーム取得

	// デバッグ
	void Debug();	// デバッグ

private:

	// メンバ関数
	void BrightBar();				// ゲージ発光
	void BrightAssist();			// アシスト発光
	void SetPositionRelative();		// 相対位置設定

	// 静的メンバ変数
	static float m_fBrightTime;		// maxの時光るカウンター
	static float m_fBrightTimeEnd;	// maxの時光るカウンター
	static float m_fAssistTime;		// アシストカウンター
	static float m_fAssistTimeEnd;	// アシストカウンター

	// メンバ変数
	CObject2D* m_pBg;				// 背景
	CObject2D* m_pBar;				// ゲージ
	CObject2D* m_pFrame;			// フレーム
	CObject2D* m_pAssist;			// ボタンアシスト
	CString2D* m_pTeamName;			// チーム名
	MyLib::Vector3 m_offsetFrame;	// フレームオフセット
	MyLib::Vector3 m_offsetAssist;	// アシストオフセット
	MyLib::Vector3 m_offsetName;	// チーム名オフセット
	CGameManager::ETeamSide m_team;	// チーム
	EState m_state;					// 状態
	float m_fChange;				// ゲージ変動量
	float m_fStateTime;				// 状態管理カウンター
	float m_fCurrentNumGauge;		// 現在表示値
	float m_fNumGauge;				// 目標値
	float m_fMaxNumGauge;			// 表示値の最大値
	const float m_fFrame;			// 表示値の変動フレーム定数

	// デバッグ変数
	float m_fSizeFrame;	// 枠大きさ倍率
};

#endif	// _GAUGE2D_H_

//============================================================
//
//	着せ替えUIヘッダー [dressupUI.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _DRESSUP_UI_H_
#define _DRESSUP_UI_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"
#include "player.h"
#include "gamemanager.h"

//************************************************************
// 前方宣言
//************************************************************
class CDressup;			// 着せ替えクラス
class CBallFake;		// 利き手表示用ボールクラス
class CObject2D;		// オブジェクト2Dクラス
class CObject2D_Anim;	// オブジェクト2Dアニメクラス
class CEntry_Dressup;	// ドレスアップ設定クラス

//************************************************************
//	クラス定義
//************************************************************
// 着せ替えUIクラス
class CDressupUI : public CObject
{
public:
	// エディットする種類
	enum EEditType
	{
		EDIT_PROCESS = 0,	// 実際の変更
		EDIT_CHANGETYPE,	// 変更する種類
		EDIT_MAX
	};

	// 変更する種類
	enum EChangeType
	{
		TYPE_HAIR = 0,		// 髪
		TYPE_ACCESSORY,		// アクセサリー
		TYPE_FACE,			// 顔
		TYPE_BODY,			// 体型
		TYPE_HANDEDNESS,	// 利き手
		TYPE_MAX
	};

	// コンストラクタ
	CDressupUI(CEntry_Dressup* pParent, const CPlayer::EFieldArea typeArea, const int nPlayerIdx);

	// デストラクタ
	~CDressupUI() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Kill() override;		// 削除
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新
	void Draw() override;		// 描画
	void SetType(const TYPE type) override;					// 種類設定
	void SetEnableDisp(const bool bDraw) override;			// 描画状況設定
	void SetPosition(const MyLib::Vector3& pos) override;	// 位置設定

	// 静的メンバ関数
	static CDressupUI* Create	// 生成
	( // 引数
		CEntry_Dressup* pParent,			// 親クラス情報
		const CPlayer::EFieldArea typeArea,	// プレイヤーポジション
		const int nPlayerIdx,				// プレイヤーインデックス
		const MyLib::Vector3& rPos			// 原点位置
	);

	// メンバ関数
	void SetReady(const bool bReady);			// 準備完了フラグ設定
	void SetPadIdx(const int nPadIdx);			// 操作権インデックス設定
	int GetMyPlayerIdx() const;					// プレイヤーインデックス取得
	CGameManager::ETeamSide GetTeam() const;	// チームサイド取得
	int GetHairNowIdx() const;					// 髪のインデックス番号取得
	int GetAccessoryNowIdx() const;				// アクセのインデックス番号取得
	int GetFaceNowIdx() const;					// 顔のインデックス番号取得
	CPlayer::EBody GetBodyType() const;			// 体型取得
	CPlayer::EHandedness GetHandedness() const;	// 利き手取得
	inline CPlayer::EFieldArea GetArea() const	{ return m_typeArea; }		// プレイヤーポジション取得
	inline int GetPadIdx() const				{ return m_nPadIdx; }		// 操作権インデックス取得
	inline EEditType GetTypeEdit() const		{ return m_typeEdit; }		// エディット種類取得
	inline EChangeType GetTypeChange() const	{ return m_typeChange; }	// 変更種類取得
	inline bool IsReady() const					{ return m_bReady; }		// 準備完了フラグ取得

private:
	// メンバ関数
	HRESULT CreateUI();					// UI生成
	HRESULT CreateChangeIcon();			// 変更種類アイコン生成
	HRESULT CreateReadyCheck();			// 準備完了チェック生成
	HRESULT CreatePlayerFrame();		// プレイヤーフレーム生成
	HRESULT CreatePlayerUI();			// プレイヤーUI生成
	HRESULT CreateSetup();				// セットアップ生成
	HRESULT CreateRenderTexture();		// レンダーテクスチャ生成
	void CreateTexture();				// テクスチャ作成
	void SetPositionRelative();			// 相対位置設定
	void UpdateControl(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 操作更新
	void UpdateUI();					// UI更新
	void UpdatePlayerUI();				// プレイヤーUI更新
	void ChangeEditType(int nPadIdx);	// エディットする種類変更
	void ChangeChangeType(int nPadIdx);	// 変更する箇所の種類変更
	void ChangeBodyType(int nPadIdx);	// 体型変更
	void ChangeHandedness(int nPadIdx);	// 利き手変更
	HRESULT ReCreatePlayer(CPlayer::EHandedness handedness, CPlayer::EBody body);	// プレイヤー再生成

	// 静的メンバ変数
	static int m_nNumAI;	// AI総数

	// メンバ変数
	CEntry_Dressup* m_pParent;		// 親クラス情報
	CRenderTexture* m_pRenderScene;	// シーンレンダーテクスチャ
	CObject2D_Anim* m_pChangeIcon;	// 変更種類アイコン情報
	CObject2D_Anim* m_pReadyCheck;	// 準備完了チェック情報
	CObject2D_Anim* m_pPlayerFrame;	// プレイヤーフレーム情報
	CObject2D* m_pPlayerUI;			// プレイヤーUI情報
	int m_nOrdinalAI;				// 自身が生成された順番 (AIのみ)
	int m_nPadIdx;					// 操作権インデックス
	bool m_bReady;					// 準備完了フラグ
	CBallFake* m_pBall;				// ボール
	CPlayer* m_pPlayer;				// プレイヤー
	CDressup* m_pHair;				// 髪着せ替え
	CDressup* m_pAccessory;			// アクセ着せ替え
	CDressup* m_pFace;				// 顔着せ替え
	EEditType m_typeEdit;			// エディットする種類
	EChangeType m_typeChange;		// 変更する種類
	const int m_nPlayerIdx;					// プレイヤーインデックス
	const CPlayer::EFieldArea m_typeArea;	// プレイヤーポジション
};

#endif	// _DRESSUP_UI_H_

//==========================================================================
// 
//  オプションメニューヘッダー [optionMenu.h]
//  Author : 相馬靜雅
// 
//==========================================================================

#ifndef _OPTIONMENU_H_
#define _OPTIONMENU_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "object.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CMultiNumber;

//==========================================================================
// クラス定義
//==========================================================================
// オプションメニュークラス
class COptionMenu : public CObject
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum EState // 状態
	{
		STATE_NONE = 0,	// なにもなし
		STATE_SELECT,	// 選択
		STATE_EDIT,		// 編集
		STATE_MAX
	};

	enum ESelect // 選択肢
	{
		SELECT_MASTERVOLUME = 0,	// マスターボリューム
		SELECT_SE,					// SE
		SELECT_BGM,					// BGM
		SELECT_VIB,					// バイブレーション
		SELECT_BACK,				// 戻る
		SELECT_RANKING,				// ランキング
		SELECT_MAX
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	COptionMenu(int nPriority = mylib_const::PRIORITY_DEFAULT, const LAYER layer = LAYER::LAYER_DEFAULT);
	~COptionMenu();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill() override;	// 削除

	//=============================
	// メンバ関数
	//=============================
	void SetState(EState state);			// 状態設定
	EState GetState() { return m_state; }	// 状態取得

	void Debug();	// デバッグ処理

	//=============================
	// 静的メンバ関数
	//=============================
	static COptionMenu* Create();	// 生成

private:

	//=============================
	// 関数リスト
	//=============================
	// 状態関数
	typedef void(COptionMenu::*STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];	// 状態関数

	//=============================
	// メンバ変数
	//=============================
	// 状態
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 状態更新
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {}
	void StateSelect(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 選択
	void StateEdit(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 編集

	// 更新
	void UpdateObj(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// オブジェクト更新
	void UpdateColor(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 色更新
	void UpdateSelect(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 選択肢更新
	void UpdateMarker(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// マーカー更新
	void UpdateSound(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// サウンド更新
	void UpdateVib(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// バイブフラグ更新

	// 生成
	HRESULT CreateBoard();		// ボード生成
	HRESULT CreatePaper();		// 紙生成
	HRESULT CreateTitle(CObject2D** ppObj2D, const std::string& texture, const MyLib::Vector3& pos);	// 見出し生成
	HRESULT CreateSelect();		// 選択肢生成
	HRESULT CreateNumber();		// 数字生成
	HRESULT CreatePin();		// ピン生成
	HRESULT CreateVibFrag();	// バイブフラグ生成
	HRESULT CreateDesign(CObject2D** ppObj2D, const std::string& texture, const MyLib::Vector3& pos, const MyLib::Vector3& rot);	// デザイン生成
	HRESULT CreateSelectMarker();	// 選択肢のマーカー生成

	//=============================
	// メンバ変数
	//=============================
	// 状態
	EState m_state;		// 状態
	float m_fStateTime;	// 状態タイマー

	// 選択肢
	ESelect m_select;		// 選択肢
	ESelect m_oldSelect;	// 前回の選択肢

	// その他
	CObject2D* m_pBoard;	// ボード
	CObject2D* m_pPaper;	// 紙
	CObject2D* m_pTitleSound;	// サウンドの見出し
	CObject2D* m_pTitlePad;		// コントローラーの見出し
	CObject2D* m_pSelectUI[ESelect::SELECT_MAX];	// 選択肢のUI
	CMultiNumber* m_pNumberMaster;	// マスターボリューム
	CMultiNumber* m_pNumberSE;		// SE
	CMultiNumber* m_pNumberBGM;		// BGM
	CObject2D* m_pVibFrag;			// バイブのフラグ
	CObject2D* m_pPin;				// ピンs
	CObject2D* m_pDesign_01;		// デザイン
	CObject2D* m_pDesign_02;		// デザイン
	CObject2D* m_pSelectMarker;		// 選択肢のマーカー
	float m_fMarkerTime;			// マーカーのタイマー
};

#endif

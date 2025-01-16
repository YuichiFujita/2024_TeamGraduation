//============================================================
//
//	選択UIヘッダー [selectUI.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SELECT_UI_H_
#define _SELECT_UI_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
// 前方宣言
//************************************************************
class CObject2D;		// オブジェクト2Dクラス
class CObject2D_Anim;	// オブジェクト2Dアニメクラス
class CEntry_Dressup;	// 着せ替えシーンクラス

//************************************************************
//	クラス定義
//************************************************************
// 選択UIクラス
class CSelectUI : public CObject
{
public:
	// コンストラクタ
	CSelectUI(const int nPlayerIdx, const int nPadIdx);

	// デストラクタ
	~CSelectUI() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Kill() override;		// 削除
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新
	void Draw() override;		// 描画
	void SetEnableDisp(const bool bDraw) override;			// 描画状況設定
	void SetPosition(const MyLib::Vector3& pos) override;	// 位置設定

	// 静的メンバ関数
	static CSelectUI* Create(const int nPlayerIdx, const int nPadIdx, const MyLib::Vector3& pos);	// 生成

	// メンバ関数
	inline void SetSelect(const bool bSelect) { m_bSelect = bSelect; }	// 選択操作フラグ設定
	inline bool IsSelect() const	{ return m_bSelect; }				// 選択操作フラグ取得
	inline int GetPadIdx() const	{ return m_nPadIdx; }				// 操作権インデックス取得
	inline int GetSelectIdx() const	{ return m_nSelectPlayerIdx; }		// 選択プレイヤーインデックス取得

private:
	// メンバ関数
	HRESULT CreateUI();			// UI生成
	void UpdateSelect();		// 選択更新
	void UpdateDecide();		// 決定更新
	void UpdateCancel();		// キャンセル更新
	void SetPositionRelative();	// 相対位置設定

	// メンバ変数
	CObject2D_Anim* m_pPadUI;	// コントローラーUI情報
	CObject2D* m_pFrame;		// フレーム情報
	const int m_nPadIdx;		// 操作権インデックス
	int m_nSelectPlayerIdx;		// 選択プレイヤーインデックス
	bool m_bSelect;				// 選択操作フラグ
};

#endif	// _SELECT_UI_H_

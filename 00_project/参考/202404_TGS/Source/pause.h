//=============================================================================
// 
//  ポーズヘッダー [pause.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _PAUSE_H_
#define _PAUSE_H_	// 二重インクルード防止

#include "main.h"
#include "object2D.h"
#include "manager.h"

//==========================================================================
// クラス定義
//==========================================================================
// ポーズクラス定義
class CPause
{
public:

	CPause();
	~CPause();

	// メンバ関数
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	void Kill();	// 削除
	void SetPause();	// 使用状況変更
	bool IsPause();		// 使用状況取得

	static CPause *Create(CScene::MODE mode);	// 生成

protected:

	// 列挙型定義
	enum VTX
	{
		VTX_FADE = 0,		// 黒い幕
		VTX_WINDOW,			// メニューウィンドウ
		VTX_CONTINUE,		// ゲームに戻る
		VTX_RETRY,			// リトライ
		VTX_QUIT,			// タイトルに戻る	
		VTX_MAX
	};

	// メニュー
	enum MENU
	{
		MENU_RETURNGAME = 0,	// ポーズの解除
		MENU_RETRY,				// リトライ
		MENU_RETURNTITLE,		// タイトルに戻る	
		MENU_MAX
	};

	void UpdateSelect();	// 選択肢の更新
	void UpdateColor();		// 色更新
	void Decide();			// 決定処理
	virtual void DecideByMode() = 0;	// モード別決定処理

	CObject2D* m_aObject2D[VTX_MAX];				// オブジェクト2Dのオブジェクト
	bool m_bPause;									// ポーズの判定
	int m_nSelect;									// 選択肢
	float m_fFlashTime;								// 点滅時間
};

#endif
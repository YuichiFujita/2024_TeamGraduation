//=============================================================================
// 
//  リトライUIヘッダー [retryui.h]
//  Author : 堀川萩大
// 
//=============================================================================

#ifndef _RETRY_UI_H_
#define _RETRY_UI_H_	// 二重インクルード防止

#include "object2d.h"

// 前方宣言
class CControlKeyDisp;

//==========================================================================
// クラス定義
//==========================================================================
// サンプル_オブジェクト2Dクラス
class CRetry_Ui : public CObject2D
{
public:

	//=============================
	// UI列挙
	//=============================
	enum UINUM
	{
		BUTTON_RESTART = 0,		// リスタートボタン
		BUTTON_RETRY,			// リトライボタン
		BUTTON_POSE,			// ポーズボタン
		BUTTON_MAX
	};

	CRetry_Ui(int nPriority = 7);
	~CRetry_Ui();

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//=============================
	// 静的関数
	//=============================
	static CRetry_Ui* Create();

private:

	//=============================
	// メンバ関数
	//=============================
	// 状態系

	// その他関数
	void Moveui();	// 移動処理
	//=============================
	// メンバ変数
	//=============================
	// 状態系

	// その他変数
	CObject2D* m_Button[BUTTON_MAX];	// 2Dオブジェクト
	CControlKeyDisp* m_apKeyDisp[BUTTON_MAX];	// キーコンフィグ
	MyLib::Vector3 m_DispPos[BUTTON_MAX];
	float m_fRetryPushTime;			// リトライの押下時間
};


#endif
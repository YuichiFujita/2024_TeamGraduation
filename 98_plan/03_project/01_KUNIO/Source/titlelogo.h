//=============================================================================
// 
// タイトルロゴ処理 [titlelogo.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _TITLELOGO_H_
#define _TITLELOGO_H_		// 二重インクルード防止のマクロを定義する

#include "object.h"

class CObject2D;

//==========================================================================
// クラス定義
//==========================================================================
// タイトルロゴ
class CTitleLogo : public CObject
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum State
	{
		STATE_NONE = 0,			// なにもなし
		STATE_WAIT,				// 待機
		STATE_FADEIN_WATER,		// 水フェードイン
		STATE_FADEIN_PLAYER,	// プレイヤーフェードイン
		STATE_BRESS,			// 息
		STATE_WAIT_AFTER,		// 待機(後)
		STATE_AFTERMOVEMENT,	// 後の動き
		STATE_MAX
	};

	enum LOGO_LOVE	// LOVE部分
	{
		L = 0,
		O,
		V,
		E,
		MAX
	};

	CTitleLogo(float fadetime, int nPriority = 0);
	~CTitleLogo();
	
	//  オーバーライドされた関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetState(State state);	// 状態設定
	static CTitleLogo* Create(float fadetime);	// 生成処理

private:

	//=============================
	// 関数ポインタ
	//=============================
	typedef void(CTitleLogo::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// メンバ関数
	//=============================
	// 状態
	void UpdateState();	// 状態更新
	void StateNone();			// なにもなし
	void StateWait();			// 待機
	void StateFadeIn_Water();	// 水フェードイン
	void StateFadeIn_Player();	// プレイヤーフェードイン
	void StateBress();			// 息
	void StateWait_After();		// 後の待機
	void StateAfterMovement();	// 後の動き

	// その他
	void CreateWater();		// 水生成
	void CreatePlayer();	// プレイヤー生成
	void CreateBress();		// 息生成
	void CreateLOVE();		// LOVE生成
	void CreateSUITON();	// SUITON生成
	void DrawingLOVEDistance();	// LOVEの間隔ランダム選定

	//=============================
	// メンバ変数
	//=============================
	State m_state;		// 状態
	float m_fStateTime;	// 状態カウンター

	CObject2D* m_pWater;	// 水
	CObject2D* m_pPlayer;	// プレイヤー
	CObject2D* m_pBress;	// 息
	CObject2D* m_pLOVE[LOGO_LOVE::MAX];	// LOVE
	CObject2D* m_pSUITON;	// SUITON
	float m_fDistance_LOVE[LOGO_LOVE::MAX];	// LOVEの距離
	const float m_fFadeOutTime;	// フェードにかかる時間

};

#endif
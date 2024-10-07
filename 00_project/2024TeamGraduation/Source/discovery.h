//=============================================================================
// 
//  発見マークヘッダー [discovery.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _DISCOVERY_H_
#define _DISCOVERY_H_	// 二重インクルード防止

#include "objectBillboard.h"

//==========================================================================
// クラス定義
//==========================================================================
// 発見マーククラス
class CDiscovery : public CObjectBillboard
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum State
	{
		STATE_NONE = 0,		// なにもない
		STATE_APPEARANCE,	// 出現
		STATE_WAIT,			// 待機
		STATE_FADEOUT,		// フェードアウト
		STATE_MAX
	};

	CDiscovery(int nPriority = 7);
	~CDiscovery();

	// オーバーライドされた関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Kill();

	static CDiscovery* Create(const MyLib::Vector3& pos);

private:
	//=============================
	// 関数リスト
	//=============================
	typedef void(CDiscovery::* STATE_FUNC)();
	static STATE_FUNC m_StateFuncList[];	// 関数のリスト

	//=============================
	// メンバ関数
	//=============================
	// 状態系
	void UpdateState();		// 状態更新
	void SetState(State state);		// 状態設定
	void StateNone();		// 通常
	void StateAppearance();	// 出現
	void StateWait();		// 待機
	void StateFadeOut();	// フェードアウト

	//=============================
	// メンバ変数
	//=============================
	State m_state;			// 状態
	float m_fStateTime;		// 状態カウンター
};


#endif
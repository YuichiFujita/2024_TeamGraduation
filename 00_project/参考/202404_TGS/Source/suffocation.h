//=============================================================================
// 
//  サフォケーションヘッダー [suffocation.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _SUFFOCATION_H_
#define _SUFFOCATION_H_	// 二重インクルード防止

#include "object2d.h"

//==========================================================================
// クラス定義
//==========================================================================
// サフォケーションクラス
class CSuffocation : public CObject2D
{
public:

	//=============================
	// 状態列挙
	//=============================
	enum State
	{
		STATE_SURFACING = 0,	// 浮上
		STATE_BURST,			// 破裂
		STATE_MAX
	};

	CSuffocation(int nPriority = 7);
	~CSuffocation();

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
	static CSuffocation* Create();

private:

	//=============================
	// 関数リスト
	//=============================
	// 状態リスト
	typedef void(CSuffocation::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// メンバ関数
	//=============================
	// 状態系
	void UpdateState();		// 状態更新
	void StateSurfacing();	// 浮上
	void StateBurst();		// 破裂

	// その他関数
	void Reset();

	//=============================
	// メンバ変数
	//=============================
	// 状態系
	float m_fStateTime;		// 状態カウンター
	State m_state;			// 状態
	int m_nTexID[6];		// ID

	// その他変数
	float m_fDestWidth;	// 目標の幅
};


#endif
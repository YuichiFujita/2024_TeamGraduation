//=============================================================================
// 
//  ピックアップガイドヘッダー [pickupguide.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _PICKUPGUIDE_H_
#define _PICKUPGUIDE_H_	// 二重インクルード防止

#include "objectBillboard.h"

//==========================================================================
// クラス定義
//==========================================================================
// ピックアップガイドクラス
class CPickupGuide : public CObjectBillboard
{
public:

	//=============================
	// 列挙型定義
	//=============================
	// 種類
	enum TYPE
	{
		TYPE_TRANSFER = 0,		// 転移
		TYPE_SKILLTREE_BEGIN,	// スキルツリー開始
		TYPE_MAX
	};

	// 状態
	enum STATE
	{
		STATE_NONE = 0,	// なにもなし
		STATE_FADEIN,	// フェードイン
		STATE_FADEOUT,	// フェードアウト
		STATE_MAX
	};

	CPickupGuide(int nPriority = 7);
	~CPickupGuide();


	// オーバーライドされた関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetState(STATE state);	// 状態設定
	static CPickupGuide* Create(const MyLib::Vector3& pos, TYPE type);

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CPickupGuide::* STATE_FUNC)();
	static STATE_FUNC m_StateFuncList[];	// 状態関数のリスト

	//=============================
	// メンバ関数
	//=============================
	// 状態関数系
	void StateNone() {}		// なにもなし
	void StateFadeIn();		// フェードイン
	void StateFadeOut();	// フェードアウト

	//=============================
	// メンバ変数
	//=============================
	STATE m_state;	// 状態
	float m_fStateTime;	// 状態カウンター
	TYPE m_type;		// 種類
};


#endif
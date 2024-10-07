//=============================================================================
// 
//  葉っぱヘッダー [leaf.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _LEAF_H_
#define _LEAF_H_	// 二重インクルード防止

#include "object3D.h"

//==========================================================================
// クラス定義
//==========================================================================
// 葉っぱクラス
class CLeaf : public CObject3D
{
public:

	//=============================
	// 状態列挙
	//=============================
	enum State
	{
		STATE_FALL = 0,		// 落下
		STATE_WAIT,			// 待機
		STATE_FADEIN,		// フェードイン
		STATE_FADEOUT,		// フェードアウト
		STATE_MAX
	};

	enum Type
	{
		TYPE_FALL = 0,		// 落下
		TYPE_FLOW,			// 流れ
		TYPE_MAX
	};

	CLeaf(int nPriority = 7, const LAYER layer = LAYER::LAYER_LEAF);
	~CLeaf();

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
	static CLeaf* Create(const MyLib::Vector3& pos, const Type& type);

private:

	//=============================
	// 関数リスト
	//=============================
	// 状態リスト
	typedef void(CLeaf::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// メンバ関数
	//=============================
	// 状態系
	void UpdateState();		// 状態更新
	void StateFall();		// 落下
	void StateWait();		// 待機
	void StateFadeIn();		// フェードイン
	void StateFadeOut();	// フェードアウト

	// その他関数
	void CalDestRotation();	// 目標の向き計算
	void CheckLand();		// 着地確認

	//=============================
	// メンバ変数
	//=============================
	// 状態系
	float m_fStateTimer;	// 状態カウンター
	State m_state;			// 状態

	// その他変数
	MyLib::Vector3 m_rotStart;	// 開始時の向き
	MyLib::Vector3 m_rotDest;	// 目標の向き
	float m_fFallWidth;			// 落下幅
	float m_fWidthMoveTimer;	// 幅移動のタイマー
	float m_fRotateTimer;		// 回転までの時間
	float m_fRotateInterval;	// 回転までの間隔
};


#endif
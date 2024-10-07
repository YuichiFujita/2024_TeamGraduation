//=============================================================================
// 
//  泡ヘッダー [meshbubble.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _MESHBUBBLE_H_
#define _MESHBUBBLE_H_	// 二重インクルード防止

#include "meshsphere.h"

//==========================================================================
// クラス定義
//==========================================================================
// 泡クラス
class CMeshBubble : public CMeshSphere
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum STATE
	{
		STATE_MOVE = 0,
		STATE_BURST,
		STATE_FADEOUT,
		STATE_MAX
	};

	CMeshBubble(int nPriority = 6);
	~CMeshBubble();

	// オーバーライドされた関数
	virtual HRESULT Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	virtual void SetVtx(void) override;

	static CMeshBubble *Create(const MyLib::Vector3& pos, const MyLib::Vector3& move, const float radius, const float destRadius, float repeatCycle);	// 生成処理

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CMeshBubble::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// メンバ関数
	//=============================
	// 状態系
	void StateMove();		// 移動処理
	void StateBurst();		// 弾け処理
	void StateFadeOut();	// フェードアウト処理

	float m_fOriginSize;	// 元のサイズ
	float m_fDestSize;		// 目標のサイズ
	float m_fRepeatCycle;	// 反復周期
	float m_fStateTime;		// 状態タイマー
	STATE m_state;			// 状態
};


#endif
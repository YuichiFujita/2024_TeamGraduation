//=============================================================================
// 
//  エリア制限ヘッダー [limitarea.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _LIMITAREA_H_
#define _LIMITAREA_H_	// 二重インクルード防止

#include "meshwall.h"
#include "listmanager.h"

//==========================================================================
// クラス定義
//==========================================================================
// エリア制限クラス定義
class CLimitArea : public CObject
{
public:

	// 状態列挙
	enum STATE
	{
		STATE_NONE = 0,
		STATE_APPEARANCE,	// 出現
		STATE_FADEOUT,		// 消滅
		STATE_MAX
	};

	// エリア構造体
	struct sLimitEreaInfo
	{
		float fMinX;	// 最小X
		float fMaxX;	// 最大X
		float fMinZ;	// 最小Z
		float fMaxZ;	// 最大Z
	};

	CLimitArea(int nPriority = 7);
	~CLimitArea();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	void Kill();	// 削除処理

	static CLimitArea *Create(sLimitEreaInfo info);	// 生成処理
	STATE GetState();		// 状態取得
	void SetState(STATE state);	// 状態設定
	sLimitEreaInfo GetLimitEreaInfo();	// エリア情報取得
	static CListManager<CLimitArea> GetListObj() { return m_List; }	// リスト取得

protected:

private:

	// 壁位置の列挙
	enum eWallTag
	{
		WALL_UP = 0,	// 奥
		WALL_RIGHT,		// 右
		WALL_DOWN,		// 手前
		WALL_LEFT,		// 左
		WALL_MAX
	};

	//=============================
	// 関数リスト
	//=============================
	typedef void(CLimitArea::*STATE_FUNC)();
	static STATE_FUNC m_StateFuncList[];	// 状態の関数リスト

	//=============================
	// メンバ関数
	//=============================
	void StateNone();		// 通常
	void StateAppearance();	// 出現状態
	void StateFadeout();	// フェードアウト

	//=============================
	// メンバ変数
	//=============================
	STATE m_state;						// 状態
	float m_fTimeState;					// 状態カウンター
	sLimitEreaInfo m_sLimitEreaInfo;	// エリア制限情報
	CMeshWall *m_pMeshWall[mylib_const::SHAPE_LIMITEREA];	// メッシュウォールのオブジェクト
	static CListManager<CLimitArea> m_List;	// リスト
};



#endif
//=============================================================================
// 
// ハンドル処理 [edithandle.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _EDITHANDLE_H_
#define _EDITHANDLE_H_		// 二重インクルード防止のマクロを定義する

#include "objectX.h"
class CObjectLine;

//==========================================================================
// クラス定義
//==========================================================================
// ハンドルクラス
class CHandle : public CObject
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum HandleType
	{
		TYPE_MOVE = 0,	// 移動
		TYPE_SCALE,		// サイズ
		TYPE_MAX
	};

	enum HandleAngle
	{
		ANGLE_X = 0,
		ANGLE_Y,
		ANGLE_Z,
		ANGLE_MAX
	};

	enum State
	{
		STATE_NONE = 0,	// 何もなし
		STATE_GRAB,		// 掴み
		STATE_MAX
	};

	//=============================
	// 構造体定義
	//=============================
	struct SEditHandleInfo
	{
		MyLib::Vector3 pos;		// 位置
		MyLib::Vector3 rot;		// 向き
		MyLib::Vector3 scale;	// スケール

		SEditHandleInfo() : pos(0.0f), rot(0.0f), scale(0.0f) {}
	};


	CHandle(int nPriority = 8, const LAYER layer = LAYER::LAYER_LINE);
	~CHandle();

	//  オーバーライド関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void SetPosition(const MyLib::Vector3& pos) override;		// 位置設定

	float GetScale() { return m_scale; }						// マップに対してのスケール取得
	void SetAngle(HandleAngle angle) { m_moveAngle = angle; }	// 向き設定
	void Kill();
	bool IsHoverHandle() { return m_bHoverHandle; }				// ハンドルとのホバー判定
	HandleAngle GetHoverAngle() { return m_moveAngle; }			// ホバーしている向き取得
	void SetState(State state) { m_state = state; }				// 状態設定

	virtual SEditHandleInfo GetDiff(HandleAngle angle) = 0;		// 差分取得
	virtual void Grab() = 0;	// 掴み中処理

	static CHandle* Create(const HandleType type, const MyLib::Vector3& pos);
	static CListManager<CObjectX> GetList() { return m_List; }

protected:

	//=============================
	// 共有変数
	//=============================
	float m_scale;							// マップに対してのスケール

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CHandle::*STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// メンバ関数
	//=============================
	// 状態関数
	void StateNone();	// なにもなし
	void StateGrab();	// 掴み

	// その他
	void ReSizeToCamera();	// カメラに対してリサイズ
	void CollisionHandle();	// ハンドルとの判定

	//=============================
	// メンバ変数
	//=============================
	MyLib::AABB m_AABB;
	CObjectX* m_pArrow[ANGLE_MAX];			// ラインのオブジェクト
	CObjectLine* m_pLine[ANGLE_MAX];		// ラインのオブジェクト
	CObjectX* m_pCenterSphere;				// 中心
	HandleAngle m_moveAngle;				// 移動の向き
	HandleType m_HandleType;				// ハンドルの種類
	bool m_bHoverHandle;					// ハンドルのホバー判定
	State m_state;							// 状態
	static CListManager<CObjectX> m_List;	// 移動ハンドルリスト
};
#endif
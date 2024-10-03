//=============================================================================
//
// 入力(マウス)処理 [input_mouse.h]
// Author : 相馬靜雅
//
//=============================================================================

#ifndef _INPUT_MOUSE_H_	//このマクロ定義がされていなかったら
#define _INPUT_MOUSE_H_	//二重インクルード防止のマクロを定義する

#include "input.h"

//==========================================================================
// クラス定義
//==========================================================================
// マウスクラス
class CInputMouse : public CInput
{
public:

	// 列挙型定義
	enum BUTTON
	{
		BUTTON_LEFT = 0,
		BUTTON_RIGHT,
		BUTTON_WHEEL,
		BUTTON_SIDE_1,
		BUTTON_SIDE_2,
		BUTTON_MAX
	};

	CInputMouse();
	~CInputMouse();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit();
	void Update();
	bool GetPress(BUTTON nKey);		// プレス情報取得
	bool GetTrigger(BUTTON nKey);	// トリガー情報取得
	bool GetRelease(BUTTON nKey);	// リリース情報取得
	MyLib::Vector3 GetMouseMove();
	D3DXVECTOR2 GetPosition();				// スクリーン位置取得
	MyLib::Vector3 GetOldWorldPosition();	// 前回のスクリーン位置取得
	MyLib::Vector3 GetWorldPosition();	// 位置取得
	MyLib::Vector3 GetRay();			// レイ取得
	MyLib::Vector3 GetNearPosition();	// レイの始点取得
	MyLib::Vector3 GetWorldDiffPosition();	// ワールド座標の差分取得
	void SetCameraMtx(D3DXMATRIX* viewMtx, D3DXMATRIX* prjMtx);	// カメラマトリックス設定
	void SetFarDistance(float distance) { m_fFarDistance = distance; }	// 遠方壁までの距離設定
	float GetFarDistance() { return m_fFarDistance; }					// 遠方壁までの距離取得
	float GetScreenDiffFactor() { return m_fScreenDiffFactor; }			// スクリーン差分の係数取得
	int GetDeltaX() { return m_nDeltaX; }	// Xの差分取得

	static CInputMouse* GetInstance() { return m_pThisPtr; }	// インスタンス取得
	static CInputMouse* Create(HINSTANCE hInstance, HWND hWnd);	// 生成処理

private:

	//=============================
	// メンバ変数
	//=============================
	DIMOUSESTATE2 m_MouseState;			// 全入力情報の保管
	DIMOUSESTATE2 m_MouseStateTrigger;	// トリガー情報
	DIMOUSESTATE2 m_MouseStateRerease;	// リリース情報
	DIMOUSESTATE2 m_MouseStateRepeat;	// リピート情報
	BYTE m_aOldState[BUTTON::BUTTON_MAX];	// 前回の入力情報を保存
	POINT m_pos;							// スクリーン座標
	POINT m_posOld;							// 前回のスクリーン座標
	int m_nDeltaX;							// X差分
	MyLib::Vector3 m_OldWorldPos;		// 前回のワールド座標
	MyLib::Vector3 m_WorldPos;			// ワールド座標
	MyLib::Vector3 m_WorldDiffposition;	// ワールド座標の差分
	MyLib::Vector3 m_NearPos;			// レイの始点
	D3DXMATRIX* m_pViewMtx;				// ビューマトリックス
	D3DXMATRIX* m_pPrjMtx;				// プロジェクションマトリックス
	float m_fFarDistance;				// 遠方壁までの距離
	float m_fScreenDiffFactor;			// スクリーン差分の係数

	static CInputMouse* m_pThisPtr;	// 自身のポインタ
};

#endif
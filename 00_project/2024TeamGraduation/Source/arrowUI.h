//==========================================================================
//
//	矢印UIヘッダー [arrowUI.h]
//	Author：相馬靜雅
//
//==========================================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ARROW_UI_H_
#define _ARROW_UI_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object2D_Anim.h"

//************************************************************
// 前方宣言
//************************************************************
class CDressup;			// 着せ替えクラス
class CObject2D;		// オブジェクト2Dクラス
class CObject2D_Anim;	// オブジェクト2Dアニメクラス
class CEntry_Dressup;	// ドレスアップ設定クラス

//************************************************************
//	クラス定義
//************************************************************
// 矢印UIクラス
class CArrowUI : public CObject2D_Anim
{
public:

	//=============================
	// 列挙型定義
	//=============================
	// 方向
	enum EDirection
	{
		DIRECTION_L = 0,	// 左
		DIRECTION_R,		// 右
		DIRECTION_MAX
	};

	// 状態
	enum EState
	{
		STATE_NONE = 0,		// NONE
		STATE_NORMAL,		// 通常
		STATE_SELECTMOVE,	// 選択時移動
		STATE_MAX
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CArrowUI(int nPriority);
	~CArrowUI() override;

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Kill() override;		// 削除
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新

	//=============================
	// メンバ関数
	//=============================
	// 状態
	void SetState(const EState state);					// 状態設定
	inline EState GetState() const { return m_state; }	// 状態取得
	
	// その他
	void SetSizeByWidth(const float width);								// 横幅からサイズ設定
	inline void SetValueFloat(float value)	{ m_fValueFloat = value; }	// ふわふわ量設定
	EDirection GetDirection()				{ return m_direction; }		// 方向

	//=============================
	// 静的メンバ関数
	//=============================
	static CArrowUI* Create(EDirection dir, const MyLib::Vector3& pos, const float width, const MyLib::Color& color, int nPriority = mylib_const::PRIORITY_DEF2D);	// 生成

private:

	//=============================
	// 関数リスト
	//=============================
	// 状態関数
	typedef void(CArrowUI::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];	// 状態関数

	//=============================
	// メンバ関数
	//=============================
	// 状態
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 状態更新
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {}		// NONE
	void StateNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 通常
	void StateSelectMove(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 選択時移動

	//=============================
	// メンバ変数
	//=============================
	// 状態
	EState m_state;		// 状態
	float m_fStateTime;	// 状態タイマー

	// その他
	EDirection m_direction;	// 方向
	float m_fValueFloat;	// ふわふわ量
};

#endif	// _ARROW_UI_H_

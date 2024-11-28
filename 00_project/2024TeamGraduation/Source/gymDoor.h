//==========================================================================
// 
//  体育館ドアヘッダー [gymDoor.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _GYM_DOOR_
#define _GYM_DOOR_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "object.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CObjectX;

//==========================================================================
// クラス定義
//==========================================================================
// 体育館ドアクラス
class CGymDoor : public CObject
{
public:

	//=============================
	// 列挙型定義
	//=============================
	// 状態列挙
	enum EState
	{
		STATE_CLOSE = 0,	// 閉じている
		STATE_MOVE,			// 開閉中
		STATE_OPEN,			// 開いている
		STATE_MAX			// この列挙型の総数
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CGymDoor(int nPriority = mylib_const::PRIORITY_DEFAULT, const LAYER layer = LAYER::LAYER_DEFAULT);
	~CGymDoor();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill() override;	// 削除
	virtual void SetEnableDisp(bool bDisp) override;				// 描画状況設定
	virtual void SetPosition(const MyLib::Vector3& pos) override;	// 位置設定

	//=============================
	// 静的メンバ関数
	//=============================
	static CGymDoor* Create(const MyLib::Vector3& rPos);	// 生成

	//=============================
	// メンバ関数
	//=============================
	void SetEnableOpen(const bool bOpen, const float fMoveTime = 0.0f);		// ドア開放フラグ設定
	inline bool IsOpen() const { return m_bOpen; }							// ドア開放フラグ取得
	inline bool IsMove() const { return (m_state == EState::STATE_MOVE); }	// ドア稼働中フラグ取得

private:

	//=============================
	// 列挙型定義
	//=============================
	// ドア列挙
	enum EDoor
	{
		DOOR_LEFT = 0,	// 左ドア
		DOOR_RIGHT,		// 右ドア
		DOOR_MAX		// この列挙型の総数
	};

	//=============================
	// メンバ関数
	//=============================
	void SetPositionRelative(void);	// 相対位置設定

	//=============================
	// メンバ変数
	//=============================
	CObjectX* m_apDoor[DOOR_MAX];	// ドア情報
	EState m_state;			// 状態
	float m_fCurMoveTime;	// 現在のドア開閉タイマー
	float m_fEndMoveTime;	// 終了のドア開閉タイマー
	float m_fCurOffset;		// 現在のドア配置オフセット
	float m_fInitOffset;	// 開始のドア配置オフセット
	float m_fDestOffset;	// 目標のドア配置オフセット
	bool m_bOpen;			// 開閉フラグ
};

#endif

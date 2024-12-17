//=============================================================================
// 
//  タイトルの生徒ヘッダー [titlestudent.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _TITLESTUDENT_H_
#define _TITLESTUDENT_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "gamemanager.h"
#include "objectChara.h"
#include "listmanager.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayer;		// プレイヤー
class CShadow;		// 影
class CDressup;		// ドレスアップ

//==========================================================================
// クラス定義
//==========================================================================
// タイトルの生徒クラス定義
class CTitleStudent : public CObjectChara
{
public:

	//=============================
	// 列挙型定義
	//=============================
	// モーション列挙
	enum EMotion
	{
		MOTION_DEF = 0,				// ニュートラルモーション
		MOTION_SUSURU,				// SUSURU
		MOTION_WAIT,				// 待機
		MOTION_WALK,				// 移動
		MOTION_WARMUP_01,			// 準備体操01
		MOTION_WARMUP_02,			// 準備体操02
		MOTION_WARMUP_03,			// 準備体操03
		MOTION_WARMUP_04,			// 準備体操04
		MOTION_MAX
	};

	// 状態定義
	enum EState
	{
		STATE_NONE = 0,		// なにもない
		STATE_GO,			// 向かう
		STATE_COLLECT,		// 回収
		STATE_BACK,			// 戻る
		STATE_MAX
	};
	
	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CTitleStudent(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CTitleStudent();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill() override;	// 動的削除処理

	//=============================
	// メンバ関数
	//=============================
	void SetState(EState state);	// 状態設定

	//=============================
	// 静的関数
	//=============================
	// 生成処理
	static CTitleStudent* Create();

protected:

	//=============================
	// メンバ関数
	//=============================
	void Debug();			// デバッグ処理

private:
	
	//=============================
	// 関数リスト
	//=============================
	typedef void(CTitleStudent::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];							// 状態関数

	//=============================
	// メンバ関数
	//=============================
	//-----------------------------
	// 状態関数
	//-----------------------------
	virtual void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// 状態更新
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// なし
	
	//-----------------------------
	// モーション系関数
	//-----------------------------
	virtual void AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK) override;		// 攻撃時処理
	virtual void AttackInDicision(CMotion::AttackInfo ATKInfo, int nCntATK) override;	// 攻撃判定中処理

	// その他
	void SetRandWarmUp();	// ランダムの準備体操設定

	//=============================
	// メンバ変数
	//=============================
	//-----------------------------
	// 状態
	//-----------------------------
	EState m_state;			// 状態
	float m_fStateTime;		// 状態時間

	//-----------------------------
	// ドレスアップ
	//-----------------------------
	CDressup* m_pDressUp_Hair;		// ドレスアップ(髪)
	CDressup* m_pDressUp_Accessory;	// ドレスアップ(アクセ)
	CDressup* m_pDressUp_Face;		// ドレスアップ(顔)

	//-----------------------------
	// その他変数
	//-----------------------------
	CShadow* m_pShadow;							// 影
	static CListManager<CTitleStudent> m_List;	// リスト
};


#endif

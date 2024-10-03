//=============================================================================
// 
//  サンプル_キャラクターヘッダー [sample_character.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_	// 二重インクルード防止

#include "objectChara.h"
#include "listmanager.h"

// 前方宣言
class CShadow;

//==========================================================================
// クラス定義
//==========================================================================
// サンプル_キャラクタークラス
class CSample_Character : public CObjectChara
{
public:
	
	//=============================
	// 列挙型定義
	//=============================
	// 状態定義
	enum STATE
	{
		STATE_NONE = 0,		// なにもない
		STATE_MAX
	};

	// モーション定義
	enum MOTION
	{
		MOTION_DEFAULT = 0,	// デフォルト
		MOTION_MAX
	};

	
	CSample_Character(int nPriority = 2);
	~CSample_Character();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	//=============================
	// メンバ関数
	//=============================
	// 状態
	void SetState(STATE state)			{ m_state = state; }	// 状態設定
	CSample_Character::STATE GetState() { return m_state; }		// 状態取得

	// その他
	void Kill();	// 削除


	//=============================
	// 静的関数
	//=============================
	static CSample_Character* Create();	// 生成
	static CListManager<CSample_Character> GetListObj() { return m_List; }	// リスト取得

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CSample_Character::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// メンバ関数
	//=============================
	// 状態関数
	void UpdateState();		// 状態更新
	void StateNone();		// なし

	// その他関数

	// モーション系関数
	void AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK) override;		// 攻撃時処理
	void AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK) override;	// 攻撃判定中処理

	//=============================
	// メンバ変数
	//=============================
	STATE m_state;			// 状態
	CShadow* m_pShadow;		// 影の情報
	D3DXCOLOR m_mMatcol;	// マテリアルの色
	static CListManager<CSample_Character> m_List;	// リスト
};


#endif
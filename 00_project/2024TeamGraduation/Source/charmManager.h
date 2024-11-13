//=============================================================================
//
// モテマネージャ処理 [charmManager.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _CHARMMANAGER_H_		// このマクロ定義がされていなかったら
#define _CHARMMANAGER_H_		// 二重インクルード防止のマクロを定義する

//==========================================================================
// 前方宣言
//==========================================================================

//==========================================================================
// クラス定義
//==========================================================================
// モテマネージャクラス
class CCharmManager
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum ETypeAdd
	{
		ADD_HIT = 0,		// ヒット
		ADD_JUSTCATCH,		// ジャストキャッチ
		ADD_COVERCATCH,		// カバーキャッチ
		ADD_DODGE,			// 回避
		ADD_SPECIAL,		// スペシャル
		ADD_MAX
	};

	//=============================
	// 列挙型定義
	//=============================
	enum ETypeSub
	{
		SUB_INVADE_RETURN_HIT = 0,		// ライン越えて戻ってる最中にあたる
		SUB_EDGE_ESCAPE,				// 端に逃げまくる
		SUB_INVADE_RUN,					// 走っていってライン越え(ボール所持)
		SUB_LONG_HOLD,					// ずっとボール持って投げない
		SUB_MAX
	};

	CCharmManager();
	~CCharmManager();

	//=============================
	// メンバ関数
	//=============================
	virtual HRESULT Init();		// 初期化
	virtual void Uninit();		// 終了

	//--------------------------
	// その他
	//--------------------------
	virtual float GetAddValue(ETypeAdd type);	// 加算量取得
	virtual float GetSubValue(ETypeSub type);	// 減算量取得

	// 静的関数
	static CCharmManager* Create();	// 生成処理
	static CCharmManager* GetInstance() { return m_pThisPtr; }	// インスタンス取得

private:


	//=============================
	// メンバ変数
	//=============================
	float m_fAddValue[ETypeAdd::ADD_MAX];	// 加算量
	float m_fSubValue[ETypeSub::SUB_MAX];	// 減算量
	static CCharmManager* m_pThisPtr;	// 自身のポインタ
};

#endif
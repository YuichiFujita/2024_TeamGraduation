//=============================================================================
//
// スペシャル値マネージャ処理 [specialValueManager.h]
// Author: Kai Takada
//
//=============================================================================
#ifndef _SPECIALVALUEMANAGER_H_		// このマクロ定義がされていなかったら
#define _SPECIALVALUEMANAGER_H_		// 二重インクルード防止のマクロを定義する

//==========================================================================
// 前方宣言
//==========================================================================

//==========================================================================
// クラス定義
//==========================================================================
// モテマネージャクラス
class CSpecialValueManager
{
public:

	//=============================
	// 列挙型定義
	//=============================
	// 値増加列挙
	enum ETypeAdd
	{
		ADD_ALLWAYS = 0,	// 常に
		ADD_HIT ,			// ヒット
		ADD_CATCH,			// キャッチ
		ADD_JUSTCATCH,		// ジャストキャッチ
		ADD_COVERCATCH,		// カバーキャッチ
		ADD_DODGE,			// 回避
		ADD_CATCH_SPECIAL,	// スペシャルをキャッチした
		ADD_MAX
	};

	CSpecialValueManager();
	~CSpecialValueManager();

	//=============================
	// メンバ関数
	//=============================
	virtual HRESULT Init();		// 初期化
	virtual void Uninit();		// 終了

	//--------------------------
	// その他
	//--------------------------
	virtual float GetAddValue(ETypeAdd type);	// 加算量取得

	// 静的関数
	static CSpecialValueManager* Create();	// 生成処理
	static CSpecialValueManager* GetInstance() { return m_pThisPtr; }	// インスタンス取得

private:


	//=============================
	// メンバ変数
	//=============================
	float m_fAddValue[ETypeAdd::ADD_MAX];	// 加算量
	static CSpecialValueManager* m_pThisPtr;	// 自身のポインタ
};

#endif
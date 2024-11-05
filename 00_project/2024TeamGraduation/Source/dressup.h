//=============================================================================
//
// 着せ替え処理 [dressup.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _DRESSUP_H_		// このマクロ定義がされていなかったら
#define _DRESSUP_H_		// 二重インクルード防止のマクロを定義する

//==========================================================================
// 前方宣言
//==========================================================================
class CObjectChara;

//==========================================================================
// クラス定義
//==========================================================================
// 着せ替えクラス
class CDressup
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum EType
	{
		TYPE_HAIR = 0,	// 髪
		TYPE_ACCESSORY,	// アクセサリー
		TYPE_MAX
	};

	CDressup();
	~CDressup();

	//=============================
	// メンバ関数
	//=============================
	virtual HRESULT Init();		// 初期化
	virtual void Uninit();		// 終了
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 更新

	//--------------------------
	// その他
	//--------------------------
	virtual void Debug();

	// 静的関数
	static CDressup* Create(EType type, CObjectChara* pObjChara, int nSwitchType);	// 生成処理

protected:

	void LoadAll(const std::wstring& folder);	// 全読み込み

	//=============================
	// メンバ変数
	//=============================
	int m_nSwitchIdx;			// 切り替えるインデックス
	int m_nNowIdx;				// 現在のインデックス
	CObjectChara* m_pObjChara;	// キャラクターのポインタ
	std::vector<std::string> m_vecModelName;	// モデル名

};

#endif
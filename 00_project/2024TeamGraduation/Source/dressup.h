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
		TYPE_FACE,		// 顔
		TYPE_UNIFORM,	// ユニフォーム
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
	// キャラクター
	//--------------------------
	void BindObjectCharacter(CObjectChara* pObjChara) { m_pObjChara = pObjChara; }	// キャラクターのポインタ

	//--------------------------
	// その他
	//--------------------------
	void SetControllIdx(int idx)	{ m_nControllIdx = idx; }	// 操作するインデックス設定
	void SetNowIdx(int idx)			{ m_nNowIdx = idx; }		// 現在のインデックス設定
	int GetNowIdx()					{ return m_nNowIdx; }		// 現在のインデックス取得
	virtual void ReRegist(int idx);								// 再割り当て
	virtual void ReRegist() = 0;								// 再割り当て
	virtual void RandSet();										// ランダム設定
	virtual void Debug();

	// 静的関数
	static CDressup* Create(EType type, CObjectChara* pObjChara, int nSwitchType);	// 生成処理

protected:

	void LoadAllModel(const std::wstring& folder);		// 全読み込み
	void LoadAllTexture(const std::wstring& folder);	// 全読み込み

	//=============================
	// メンバ変数
	//=============================
	int m_nSwitchIdx;			// 切り替えるインデックス
	int m_nNowIdx;				// 現在のインデックス
	int m_nControllIdx;			// 操作するインデックス	// TODO：ここはプレイヤーのMyIdxを参照しよう
	CObjectChara* m_pObjChara;	// キャラクターのポインタ
	std::vector<std::string> m_vecModelName;	// モデル名

};

#endif
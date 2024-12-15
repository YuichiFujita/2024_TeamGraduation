//==========================================================================
// 
//	吹き出しの枠ヘッダー [balloonFrame.h]
//  Author : 相馬靜雅
// 
//==========================================================================

#ifndef _BALLOONFRAME_H_
#define _BALLOONFRAME_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "object2D.h"
#include "gameManager.h"

//==========================================================================
// 前方宣言
//==========================================================================

//==========================================================================
// クラス定義
//==========================================================================
// 吹き出しの枠クラス
class CBalloonFrame : public CObject2D
{
public:

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CBalloonFrame(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CBalloonFrame();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;		// 描画
	virtual void Kill() override;	// 削除
	void SetVtx() override;			// 頂点座標

	//=============================
	// メンバ関数
	//=============================

	//=============================
	// 静的メンバ関数
	//=============================
	static CBalloonFrame* Create(const MyLib::Vector2& size, CGameManager::ETeamSide side);	// 生成

private:

	//=============================
	// メンバ関数
	//=============================
	void CreateFillPoly();	// 埋めるポリゴン生成
	void CreateTail();		// 尾生成
	HRESULT CreateIndex();	// インデックス生成
	void DrawIdxBuff();	// インデックスバッファ描画

	//=============================
	// メンバ変数
	//=============================
	MyLib::Vector2 m_TextSize;	// 文字サイズ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		// インデックスバッファへのポインタ
	int m_nNumIndex;						// インデックス数
	CObject2D* m_pFillPoly;					// 埋める為のポリゴン
	CObject2D* m_pTail;						// 尾
	CGameManager::ETeamSide m_TeamSide;		// チームサイド
};

#endif

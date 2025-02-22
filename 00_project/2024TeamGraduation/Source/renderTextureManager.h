//==========================================================================
// 
//  レンダーテクスチャマネージャーヘッダー [renderTextureManager.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _RENDER_TEXTURE_MANAGER_H_
#define _RENDER_TEXTURE_MANAGER_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "manager.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CRenderTexture;

//==========================================================================
// クラス定義
//==========================================================================
// レンダーテクスチャマネージャークラス
class CRenderTextureManager
{
public:

	//=============================
	// 列挙型定義
	//=============================
	// レイヤー列挙
	enum ELayer
	{
		LAYER_BALLOON = 0,	// 吹き出し
		LAYER_BBS,			// 掲示板
		LAYER_PLAYER,		// プレイヤー
		LAYER_MAIN,			// メイン画面
		LAYER_MAX			// この列挙型の総数
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CRenderTextureManager();
	virtual ~CRenderTextureManager();

	//=============================
	// メンバ関数
	//=============================
	HRESULT Init();	// 初期化
	void Uninit();	// 終了
	void Draw();	// 描画
	void RegistRenderTexture(CRenderTexture* pRenderTexture, const ELayer layer);	// レンダーテクスチャ登録
	HRESULT DeleteRenderTexture(CRenderTexture* pRenderTexture);					// レンダーテクスチャ削除

	//=============================
	// 静的メンバ関数
	//=============================
	static CRenderTextureManager* Create();			// 生成
	static CRenderTextureManager* GetInstance();	// インスタンス取得

private:

	//=============================
	// 静的メンバ変数
	//=============================
	static CRenderTextureManager* m_pInstance;	// 自身のインスタンス

	//=============================
	// メンバ変数
	//=============================
	std::vector<CRenderTexture*> m_vecRenderTexture[LAYER_MAX];	// レンダーテクスチャ
};

#endif

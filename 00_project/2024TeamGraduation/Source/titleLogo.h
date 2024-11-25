//==========================================================================
// 
//  タイトルロゴヘッダー [titleLogo.h]
//  Author : Kai Takada
// 
//==========================================================================

#ifndef _TITLELOGO_H_
#define _TITLELOGO_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "object.h"

//==========================================================================
// 前方宣言
//==========================================================================

//==========================================================================
// クラス定義
//==========================================================================
// ボール所持マーカークラス
class CTitleLogo : public CObject
{
public:

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CTitleLogo(int nPriority = mylib_const::PRIORITY_DEFAULT, const LAYER layer = LAYER::LAYER_DEFAULT);
	~CTitleLogo();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill();	// 削除
	virtual void SetEnableDisp(bool bDisp);	// 描画状況設定

	//=============================
	// メンバ関数
	//=============================

	//=============================
	// 静的メンバ関数
	//=============================
	static CTitleLogo* Create();	// 生成

private:

	//=============================
	// メンバ変数
	//=============================
	HRESULT CreateMain();	// 主生成
	HRESULT CreateSub();	// 副生成

	void UpdateMain();		// 主更新
	void UpdateSub();		// 副更新

	//=============================
	// メンバ変数
	//=============================
	CObject2D* m_pMain;		// 主
	CObject2D* m_pSub;		// 副
	float m_fTime;			// タイマー
};

#endif

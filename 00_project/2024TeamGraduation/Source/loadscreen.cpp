//==========================================================================
// 
//  ロード画面処理 [loadscreen.cpp]
//  Author : 相馬靜雅
//  Adder  : 藤田勇一
// 
//==========================================================================
#include "loadscreen.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* TEXTURE_FILE	= "data\\TEXTURE\\loading000.tga";	// ロード画面テクスチャ
	const MyLib::PosGrid2 PTRN	= MyLib::PosGrid2(12, 4);			// テクスチャ分割数
	const int	PRIORITY	= 7;		// ローディングの優先順位
	const int	ANIM_WAIT	= 2;		// アニメーションの変更フレーム
	const float	FADE_LEVEL	= 0.05f;	// フェードのα値の加減量
}

//==========================================================================
// コンストラクタ
//==========================================================================
CLoadScreen::CLoadScreen() : m_pLoad(nullptr)
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CLoadScreen::~CLoadScreen()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CLoadScreen* CLoadScreen::Create()
{
	// メモリの確保
	CLoadScreen* pScreen = DEBUG_NEW CLoadScreen;
	if (pScreen != nullptr)
	{ // メモリ確保に成功した場合

		// 初期化処理
		if (FAILED(pScreen->Init()))
		{ // 生成に失敗した場合

			return nullptr;
		}
	}

	return pScreen;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CLoadScreen::Init()
{
	CTexture* pTexture = CTexture::GetInstance();	// テクスチャ情報

	// ロード画面の生成
	m_pLoad = CObject2D_Anim::Create
	( // 引数
		VEC3_SCREEN_CENT,	// 位置
		PTRN.x,	// テクスチャ横分割数
		PTRN.y,	// テクスチャ縦分割数
		0.029f,	// 待機時間
		false,	// 自動破棄
		6		// 優先順位
	);
	if (m_pLoad == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// テクスチャの割当
	m_pLoad->BindTexture(pTexture->Regist(TEXTURE_FILE));

	// 種類指定なしにする
	m_pLoad->SetType(CObject::TYPE::TYPE_NONE);

	// 自動再生をONにする
	m_pLoad->SetEnableAutoPlay(true);

	// 大きさの設定
	m_pLoad->SetSize(VEC2_SCREEN_SIZE * 0.5f);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CLoadScreen::Uninit()
{
	// ロード画面の終了
	SAFE_UNINIT(m_pLoad);
}

//==========================================================================
// 削除処理
//==========================================================================
void CLoadScreen::Kill()
{
	// ロード画面の削除
	SAFE_KILL(m_pLoad);
}

//==========================================================================
// 更新処理
//==========================================================================
void CLoadScreen::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ロード画面の更新
	m_pLoad->Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 描画処理
//==========================================================================
void CLoadScreen::Draw()
{
	// ロード画面の描画
	m_pLoad->Draw();
}

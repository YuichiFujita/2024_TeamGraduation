//==========================================================================
// 
//  タイトルロゴ処理 [titleLogo.cpp]
//  Author : Kai Takada
// 
//==========================================================================
#include "titleLogo.h"
#include "object2D.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE_MAIN = "data\\TEXTURE\\title\\titlelogo4.png";	// 主のテクスチャ
	const std::string TEXTURE_SUB = "data\\TEXTURE\\title\\player.png";			// 副のテクスチャ
}

//==========================================================================
// コンストラクタ
//==========================================================================
CTitleLogo::CTitleLogo(int nPriority, const LAYER layer) : CObject(nPriority, layer),
	m_fTime(0.0f)	// 副のタイマー
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CTitleLogo::~CTitleLogo()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CTitleLogo* CTitleLogo::Create()
{
	// メモリの確保
	CTitleLogo* pLogo = DEBUG_NEW CTitleLogo;

	if (pLogo != nullptr)
	{
		// クラスの初期化
		if (FAILED(pLogo->Init()))
		{ // 初期化に失敗した場合
			SAFE_UNINIT(pLogo);
			return nullptr;
		}
	}

	return pLogo;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CTitleLogo::Init()
{
	// オブジェクトの種類設定
	CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

	// 主生成
	if (FAILED(CreateMain()))
	{
		return E_FAIL;
	}

	// 副生成
	if (FAILED(CreateSub()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// 主生成
//==========================================================================
HRESULT CTitleLogo::CreateMain()
{
	// 生成処理
	m_pMain = CObject2D::Create(GetPriority());
	if (m_pMain == nullptr) return E_FAIL;

	// オブジェクトの種類設定
	m_pMain->CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

	// テクスチャ設定
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_MAIN);
	m_pMain->BindTexture(texID);

	// サイズ設定
	MyLib::Vector2 size = CTexture::GetInstance()->GetImageSize(texID);

	// 横幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, SCREEN_WIDTH * 0.5f);
	m_pMain->SetSize(MyLib::Vector2(size.x, size.y));
	m_pMain->SetSizeOrigin(m_pMain->GetSize());
	
	// 位置設定
	m_pMain->SetPosition(MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

	return S_OK;
}

//==========================================================================
// 副生成
//==========================================================================
HRESULT CTitleLogo::CreateSub()
{
	// 生成処理
	m_pSub = CObject2D::Create(GetPriority());
	if (m_pSub == nullptr) return E_FAIL;

	// オブジェクトの種類設定
	m_pSub->CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

	// テクスチャ設定
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_SUB);
	m_pSub->BindTexture(texID);

	// サイズ設定
	MyLib::Vector2 size = CTexture::GetInstance()->GetImageSize(texID);

	// 横幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, SCREEN_WIDTH * 0.2f);
	m_pSub->SetSize(MyLib::Vector2(size.x, size.y));
	m_pSub->SetSizeOrigin(m_pSub->GetSize());

	// 位置設定
	m_pSub->SetPosition(MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.3f, 0.0f));

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CTitleLogo::Uninit()
{
	// オブジェクトの破棄
	Release();
}

//==========================================================================
// 削除処理
//==========================================================================
void CTitleLogo::Kill()
{
	// 自身の終了
	Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CTitleLogo::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ポリゴン更新
	UpdateMain();
	UpdateSub();

	// タイマー更新
	m_fTime += fDeltaTime * fDeltaRate * fSlowRate;
	m_fTime = UtilFunc::Transformation::Clamp(m_fTime, 0.0f, 1.0f);
	if (m_fTime >= 1.0f)
	{
		m_fTime = 0.0f;
	}

#if _DEBUG
	ImGui::Text("Time: %f", m_fTime);
#endif
}

//==========================================================================
// 主更新
//==========================================================================
void CTitleLogo::UpdateMain()
{
	//// サイズ拡縮
	//MyLib::Vector2 size = MyLib::Vector2();
	//size = UtilFunc::Transformation::AdjustSizeByWidth(size, SCREEN_WIDTH * 0.2f);

	//// サイズ設定
	//m_pMain->SetSize(MyLib::Vector2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f));
}

//==========================================================================
// 副更新
//==========================================================================
void CTitleLogo::UpdateSub()
{
	// 位置揺れ
	MyLib::Vector3 pos = m_pSub->GetPosition();
	//MyLib::Vector3 pos = m_pSub->GetOriginPosition();
	MyLib::Vector3 posDef = MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);

	pos = posDef * m_fTime;

	// 位置設定
	m_pSub->SetPosition(pos);
}

//==========================================================================
// 描画処理
//==========================================================================
void CTitleLogo::Draw()
{

}

//==========================================================================
// 描画状況の設定処理
//==========================================================================
void CTitleLogo::SetEnableDisp(bool bDisp)
{
	// 基底クラスの描画状況設定
	CObject::SetEnableDisp(bDisp);

	// 主と副の描画状況設定
	m_pMain->SetEnableDisp(bDisp);
	m_pSub->SetEnableDisp(bDisp);
}

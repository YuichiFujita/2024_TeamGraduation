//=============================================================================
// 
//  スコア処理 [resultscore.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "resultscore.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "gamemanager.h"
#include "game.h"
#include "game_pressenter.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* TEXTURE_NUMBER = "data\\TEXTURE\\number\\number_blackclover_01.png";		// 数字のテクスチャ
	const char* TEXTURE_TEXT = "data\\TEXTURE\\resultscore\\resultscore.png";		// 数字のテクスチャ
	const D3DXVECTOR2 SCORESIZE = D3DXVECTOR2(60.0f, 60.0f);
	const float TIME_SET = 0.2f;
}

//==========================================================================
// コンストラクタ
//==========================================================================
CResultScore::CResultScore(int nPriority) : CObject(nPriority)
{
	m_fStateTime = 0.0f;
	m_pScore = nullptr;
	m_pObj2D = nullptr;
}

//==========================================================================
// デストラクタ
//==========================================================================
CResultScore::~CResultScore()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CResultScore* CResultScore::Create(int score)
{
	// メモリの確保
	CResultScore* pMarker = DEBUG_NEW CResultScore;

	if (pMarker != nullptr)
	{
		pMarker->m_nScore = score;

		// 初期化処理
		pMarker->Init();
	}

	return pMarker;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CResultScore::Init()
{
	// 数字生成
	CreateScore();		// 被ダメージ生成
	
	m_pObj2D = CObject2D::Create(GetPriority());
	m_pObj2D->SetPosition(MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

	int nIdxTex = CTexture::GetInstance()->Regist(TEXTURE_TEXT);
	m_pObj2D->BindTexture(nIdxTex);

	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(nIdxTex);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 640.0f);
	m_pObj2D->SetSize(size * 10.0f);
	m_pObj2D->SetSizeOrigin(size);

	m_pObj2D->SetPosition(MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));



	CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CResultScore::Uninit()
{
	if (m_pScore != nullptr)
	{
		m_pScore->Uninit();
		m_pScore = nullptr;
	}

	Release();
}

//==========================================================================
// 削除
//==========================================================================
void CResultScore::Kill()
{
	if (m_pScore != nullptr)
	{
		m_pScore->Kill();
		m_pScore = nullptr;
	}
}

//==========================================================================
// 更新処理
//==========================================================================
void CResultScore::Update()
{
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	if (CGame::GetInstance()->GetGameManager()->GetType() != CGameManager::SceneType::SCENE_RESULT &&
		m_fStateTime >= TIME_SET * 1.5f)
	{
		CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_RESULT);
		CGame_PressEnter::Create(0.5f);
	}

	D3DXVECTOR2 size = m_pScore->GetSize();
	size.x = UtilFunc::Correction::EasingEaseInOut(SCORESIZE.x * 10.0f, SCORESIZE.x, 0.0f, TIME_SET, m_fStateTime);
	size.y = UtilFunc::Correction::EasingEaseInOut(SCORESIZE.y * 10.0f, SCORESIZE.y, 0.0f, TIME_SET, m_fStateTime);
	m_pScore->SetSize(size);

	D3DXVECTOR2 sizetext = m_pObj2D->GetSize();
	D3DXVECTOR2 sizetextOR = m_pObj2D->GetSizeOrigin();
	sizetext.x = UtilFunc::Correction::EasingEaseInOut(sizetextOR.x * 10.0f, sizetextOR.x, 0.0f, TIME_SET, m_fStateTime);
	sizetext.y = UtilFunc::Correction::EasingEaseInOut(sizetextOR.y * 10.0f, sizetextOR.y, 0.0f, TIME_SET, m_fStateTime);
	m_pObj2D->SetSize(sizetext);

	m_pScore->SetValue(m_nScore);
	m_pScore->Update();
}

//==========================================================================
// 死亡回数生成
//==========================================================================
void CResultScore::CreateScore()
{
	// 今回の評価情報取得
	m_pScore = CMultiNumber::Create(
		MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f),
		SCORESIZE,
		5,
		CNumber::EObjectType::OBJECTTYPE_2D,
		TEXTURE_NUMBER,
		true, GetPriority());
	if (m_pScore == nullptr)
	{
		return;
	}

	// 右寄せに設定
	m_pScore->SetAlignmentType(CMultiNumber::AlignmentType::ALIGNMENT_RIGHT);

	m_pScore->SetValue(m_nScore);
	m_pScore->SetSize(SCORESIZE * 10.0f);

	// 色設定
	m_pScore->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//==========================================================================
// 描画処理
//==========================================================================
void CResultScore::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// オブジェクト2Dの描画
	m_pScore->Draw();

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

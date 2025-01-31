//==========================================================================
// 
//  ボール着地点マーカー処理 [ballLandingMarker.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "ballLandingMarker.h"
#include "manager.h"
#include "renderer.h"
#include "ball.h"

// TODO：ボール着地点マーカー表示
#if 1
#define DISP
#endif

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\ballmarker\\warning000.png";	// ボール着地点マーカーテクスチャ
}

//==========================================================================
// コンストラクタ
//==========================================================================
CBallLandingMarker::CBallLandingMarker(int nPriority) : CObject3D(nPriority),
	m_pBall	(nullptr)	// 親ボール情報
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CBallLandingMarker::~CBallLandingMarker()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CBallLandingMarker* CBallLandingMarker::Create(CBall* pBall)
{
	// メモリの確保
	CBallLandingMarker* pMarker = DEBUG_NEW CBallLandingMarker;
	if (pMarker != nullptr)
	{
		// プレイヤー割当
		pMarker->m_pBall = pBall;

		// クラスの初期化
		if (FAILED(pMarker->Init()))
		{ // 初期化に失敗した場合

			SAFE_UNINIT(pMarker);
			return nullptr;
		}
	}

	return pMarker;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CBallLandingMarker::Init()
{
	// オブジェクトの種類設定
	CObject::SetType(CObject::TYPE::TYPE_OBJECTBILLBOARD);

	// 親クラスの初期化
	HRESULT hr = CObject3D::Init();
	if (FAILED(hr)) { return E_FAIL; }

	// テクスチャの割当
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(nTexID);

	// テクスチャの大きさを取得
	MyLib::Vector2 size = pTexture->GetImageSize(nTexID);

	// 横幅を元にサイズを設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 60.0f);
	SetSize(size);
	SetSizeOrigin(GetSize());

	// 向き設定
	SetRotation(MyLib::Vector3(D3DX_PI * 0.5f, 0.0f, 0.0f));
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CBallLandingMarker::Uninit()
{
	// 親クラスの終了
	CObject3D::Uninit();
}

//==========================================================================
// 削除処理
//==========================================================================
void CBallLandingMarker::Kill()
{
	// 自身の終了
	Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CBallLandingMarker::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
#ifdef DISP
	// 外野から内野へのパスの場合は描画ON
	bool bDisp = (m_pBall->GetState() == CBall::EState::STATE_PASS) ? true : false;
	SetEnableDisp(bDisp);

	// 描画しない場合は抜ける
	if (!bDisp) { return; }

	// パス終了地点に位置を設定
	MyLib::Vector3 pos = m_pBall->GetPosPassEnd();
	pos.y += 0.1f;

	// 位置の反映
	SetPosition(pos);

	// 親クラスの更新
	CObject3D::Update(fDeltaTime, fDeltaRate, fSlowRate);
#else
	SetEnableDisp(false);
#endif
}

//==========================================================================
// 描画処理
//==========================================================================
void CBallLandingMarker::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイス情報

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 親クラスの描画
	CObject3D::Draw();

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

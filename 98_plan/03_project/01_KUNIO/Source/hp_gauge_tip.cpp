//=============================================================================
// 
//  HPゲージの先端処理 [hp_gauge_tip.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "hp_gauge_tip.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "hp_gauge_tip_billboard.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\hpgauge\\tip.png";	// テクスチャファイル
}

//==========================================================================
// コンストラクタ
//==========================================================================
CHPGaugeTip::CHPGaugeTip(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	memset(m_pObj2D, 0, sizeof(m_pObj2D));
}

//==========================================================================
// デストラクタ
//==========================================================================
CHPGaugeTip::~CHPGaugeTip()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CHPGaugeTip* CHPGaugeTip::Create(const MyLib::Vector3& leftpos, const MyLib::Vector3& rightpos, TYPE type)
{
	// メモリの確保
	CHPGaugeTip* pMarker = nullptr;

	switch (type)
	{
	case CHPGaugeTip::TYPE_2D:
		pMarker = DEBUG_NEW CHPGaugeTip;
		break;

	case CHPGaugeTip::TYPE_BILLBOARD:
		pMarker = DEBUG_NEW CHPGaugeTip_Billboard;
		break;

	default:
		return pMarker;
		break;
	}

	if (pMarker != nullptr)
	{
		// 初期化処理
		pMarker->Init(leftpos, rightpos);
	}

	return pMarker;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CHPGaugeTip::Init(const MyLib::Vector3& leftpos, const MyLib::Vector3& rightpos)
{
	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	int nTexIdx = CTexture::GetInstance()->Regist(TEXTURE);

	CreateTip(nTexIdx);

	return S_OK;
}

//==========================================================================
// 先端生成
//==========================================================================
void CHPGaugeTip::CreateTip(int nTexIdx)
{
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(nTexIdx);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 15.0f);

	for (int i = 0; i < VTXTYPE_MAX; i++)
	{
		// 生成処理
		m_pObj2D[i] = CObject2D::Create(GetPriority());
		if (m_pObj2D[i] == nullptr)
		{
			return;
		}
		CObject2D* pObj = m_pObj2D[i];

		// テクスチャ割り当て
		pObj->BindTexture(nTexIdx);

		// 情報設定
		pObj->SetSize(size);
		pObj->SetSizeOrigin(size);

		float angle = 0.0f;

		//MyLib::Vector3 pos = leftpos;
		switch (i)
		{
		case VTXTYPE::VTXTYPE_LEFT:
			break;

		case VTXTYPE::VTXTYPE_RIGHT:
			angle = D3DX_PI;
			//pos = rightpos;
			break;
		}

		// 向き設定
		pObj->SetRotation(MyLib::Vector3(0.0f, 0.0f, angle));
		pObj->SetOriginRotation(pObj->GetRotation());

		//pObj->SetPosition(pos);
		pObj->SetOriginPosition(pObj->GetPosition());
	}
}

//==========================================================================
// 終了処理
//==========================================================================
void CHPGaugeTip::Uninit()
{
	for (int i = 0; i < VTXTYPE_MAX; i++)
	{
		m_pObj2D[i] = nullptr;
	}
	Release();
}

//==========================================================================
// 終了処理
//==========================================================================
void CHPGaugeTip::Kill()
{
	for (int i = 0; i < VTXTYPE_MAX; i++)
	{
		if (m_pObj2D[i] != nullptr)
		{
			// 終了処理
			m_pObj2D[i]->Uninit();
			m_pObj2D[i] = nullptr;
		}
	}
	Release();

}

//==========================================================================
// 更新処理
//==========================================================================
void CHPGaugeTip::Update()
{
	
}

void CHPGaugeTip::SetLeftPosition(const MyLib::Vector3& pos)
{
	m_pObj2D[VTXTYPE::VTXTYPE_LEFT]->SetPosition(pos);
}

void CHPGaugeTip::SetRightPosition(const MyLib::Vector3& pos)
{
	m_pObj2D[VTXTYPE::VTXTYPE_RIGHT]->SetPosition(pos);
}

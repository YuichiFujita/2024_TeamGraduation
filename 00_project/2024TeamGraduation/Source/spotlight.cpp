//==========================================================================
// 
//  スポットライト処理 [spotlight.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "spotlight.h"
#include "lightPoint.h"
#include "EffekseerObj.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const int	PRIORITY	= 4;		// 優先順位
	const float	LIGHT_RANGE	= 600.0f;	// 光源範囲
}

//==========================================================================
// コンストラクタ
//==========================================================================
CSpotLight::CSpotLight() : CObject(PRIORITY, CObject::LAYER::LAYER_DEFAULT),
	m_pEffect	(nullptr)	// ライトエフェクト
{
	// メンバ変数をクリア
	for (int i = 0; i < NUM_LIGHT; i++)
	{ // ライト数分繰り返す

		m_apLight[i] = nullptr;	// ライト情報
	}
}

//==========================================================================
// デストラクタ
//==========================================================================
CSpotLight::~CSpotLight()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CSpotLight* CSpotLight::Create()
{
	// メモリの確保
	CSpotLight* pObj = DEBUG_NEW CSpotLight;
	if (pObj != nullptr)
	{
		// クラスの初期化
		if (FAILED(pObj->Init()))
		{ // 初期化に失敗した場合

			// クラスの終了
			SAFE_UNINIT(pObj);
			return nullptr;
		}
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CSpotLight::Init()
{
	for (int i = 0; i < NUM_LIGHT; i++)
	{ // ライト数分繰り返す

		// ライトの生成
		m_apLight[i] = CLightPoint::Create();
		if (m_apLight[i] == nullptr)
		{ // 生成に失敗した場合

			return E_FAIL;
		}

		// 拡散光を設定
		m_apLight[i]->SetDiffuse(MyLib::color::White());

		// 光源範囲を設定
		m_apLight[i]->SetRange(LIGHT_RANGE);
	}

	// エフェクトの生成
	m_pEffect = CEffekseerObj::Create
	( // 引数
		CMyEffekseer::EEfkLabel::EFKLABEL_SPOTLIGHT,
		VEC3_ZERO,
		VEC3_ZERO,
		VEC3_ZERO,
		17.0f,
		false
	);
	if (m_pEffect == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CSpotLight::Uninit()
{
	for (int i = 0; i < NUM_LIGHT; i++)
	{ // ライト数分繰り返す

		// ライトの終了
		SAFE_UNINIT(m_apLight[i]);
	}

	// エフェクトの終了
	SAFE_UNINIT(m_pEffect);
}

//==========================================================================
// 削除
//==========================================================================
void CSpotLight::Kill()
{
	// 自身の終了
	Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CSpotLight::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// 描画処理
//==========================================================================
void CSpotLight::Draw()
{

}

//==========================================================================
// ライト位置の設定処理
//==========================================================================
void CSpotLight::SetLightPosition(const MyLib::Vector3& rPos)
{
	float fRot = 0.0f;	// オフセット方向
	for (int i = 0; i < NUM_LIGHT; i++)
	{ // ライト数分繰り返す

		if (m_apLight[i] != nullptr)
		{
			// ライト位置の設定
			MyLib::Vector3 posOffset = MyLib::Vector3(sinf(fRot) * 80.0f, 160.0f, cosf(fRot) * 80.0f);	// オフセット
			m_apLight[i]->SetPosition(rPos + posOffset);
		}

		// オフセット方向を回転
		fRot += HALF_PI;
	}

	// エフェクト位置の設定
	m_pEffect->SetPosition(rPos);
}


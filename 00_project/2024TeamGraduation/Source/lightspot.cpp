#if 0
//==========================================================================
// 
//  スポットライト処理 [lightspot.cpp]
//  Author : 相馬靜雅
// 
//==========================================================================
#include "lightspot.h"
#include "manager.h"
#include "renderer.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CLightSpot::CLightSpot()
{
	// スポットライト情報のクリア
	ZeroMemory(&m_light, sizeof(D3DLIGHT9));
}

//==========================================================================
// デストラクタ
//==========================================================================
CLightSpot::~CLightSpot()
{

}

//==========================================================================
// スポットライトの初期化処理
//==========================================================================
HRESULT CLightSpot::Init()
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイス情報
	MyLib::Vector3 vecDir;	// 設定用方向ベクトル

	for (int nCntLightSpot = 0; nCntLightSpot < TYPE_MAX; nCntLightSpot++)
	{
		// スポットライトの情報をクリアする
		ZeroMemory(&m_aLightSpot[nCntLightSpot], sizeof(D3DLIGHT9));

		switch (nCntLightSpot)
		{
		case TYPE_DIRECTIONAL_01:

			// スポットライトの種類を設定
			m_aLightSpot[nCntLightSpot].Type = D3DLIGHT_DIRECTIONAL;

			// スポットライトの拡散光を設定
			m_aLightSpot[nCntLightSpot].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// スポットライトの方向を設定
			vecDir = MyLib::Vector3(0.22f, -0.87f, 0.44f);
			break;

		case TYPE_DIRECTIONAL_02:

			// スポットライトの種類を設定
			m_aLightSpot[nCntLightSpot].Type = D3DLIGHT_DIRECTIONAL;

			// スポットライトの拡散光を設定
			m_aLightSpot[nCntLightSpot].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);

			// スポットライトの方向を設定
			vecDir = MyLib::Vector3(-0.18f, 0.88f, -0.44f);
			break;

		case TYPE_DIRECTIONAL_03:

			// スポットライトの種類を設定
			m_aLightSpot[nCntLightSpot].Type = D3DLIGHT_DIRECTIONAL;

			// スポットライトの拡散光を設定
			m_aLightSpot[nCntLightSpot].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

			// スポットライトの方向を設定
			vecDir = MyLib::Vector3(0.89f, -0.11f, 0.44f);
			break;

		case TYPE_SPOT_01:

			// スポットライトの種類をスポットスポットライトに設定
			m_aLightSpot[nCntLightSpot].Type = D3DLIGHT_DIRECTIONAL;

			// スポットスポットライトの拡散光を設定
			m_aLightSpot[nCntLightSpot].Diffuse = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);

			// スポットスポットライトの方向を設定
			vecDir = MyLib::Vector3(0.0f, 0.0f, 0.0f);
			break;
		}

		// ベクトルを正規化する(1.0にする)
		D3DXVec3Normalize(&vecDir, &vecDir);
		m_aLightSpot[nCntLightSpot].Direction = vecDir;

		// スポットライトを設定する
		pDevice->SetLightSpot(nCntLightSpot, &m_aLightSpot[nCntLightSpot]);

		// スポットライトを有効にする
		pDevice->LightSpotEnable(nCntLightSpot, TRUE);
	}

	return S_OK;
}

//==========================================================================
// スポットライトの終了処理
//==========================================================================
void CLightSpot::Uninit()
{

}

//==========================================================================
// スポットライトの更新処理
//==========================================================================
void CLightSpot::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// スポットスポットライトの向き更新
//==========================================================================
void CLightSpot::UpdateSpotLightSpotDirection(MyLib::Vector3 vec)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// スポットスポットライトの方向を設定
	MyLib::Vector3 vecDir = vec;

	// ベクトルを正規化する(1.0にする)
	D3DXVec3Normalize(&vecDir, &vecDir);
	m_aLightSpot[TYPE_SPOT_01].Direction = vecDir;

	// スポットライトを設定する
	pDevice->SetLightSpot(TYPE_SPOT_01, &m_aLightSpot[TYPE_SPOT_01]);

	// スポットライトを有効にする
	pDevice->LightSpotEnable(TYPE_SPOT_01, TRUE);
}
#endif

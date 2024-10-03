//=============================================================================
// 
//  ロックオンマーカー処理 [rockon_marker.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "rockon_marker.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "calculation.h"
#include "camera.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\rockon\\rockon_marker2.png";	// テクスチャのファイル
	const char* POINT_TEXTURE = "data\\TEXTURE\\rockon\\rockon_point2.png";	// テクスチャのファイル
	const float DIV_ANGLE = (D3DX_PI * 2.0f) * 0.25f;	// 1分割あたりの向き
	const float ORIGIN_DISTANCE = 35.0f;
	const float TIME_SCALE = 5.0f;	// 拡縮の時間
}

//==========================================================================
// コンストラクタ
//==========================================================================
CRockOnMarker::CRockOnMarker(int nPriority) : CObjectBillboard(nPriority)
{
	// 値のクリア
	memset(m_RockOnInfo, 0, sizeof(m_RockOnInfo));
}

//==========================================================================
// デストラクタ
//==========================================================================
CRockOnMarker::~CRockOnMarker()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CRockOnMarker* CRockOnMarker::Create(const MyLib::Vector3& pos)
{
	// メモリの確保
	CRockOnMarker* pMarker = DEBUG_NEW CRockOnMarker;

	if (pMarker != nullptr)
	{// メモリの確保が出来ていたら

		// 位置設定
		pMarker->SetPosition(pos);

		// 初期化処理
		pMarker->Init();
	}

	return pMarker;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CRockOnMarker::Init()
{
	// ビルボードの初期化
	CObjectBillboard::Init();
	int pointTex = CTexture::GetInstance()->Regist(POINT_TEXTURE);
	BindTexture(pointTex);
	SetSize(D3DXVECTOR2(30.0f, 30.0f));

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECTBILLBOARD);

	int nTexIdx = CTexture::GetInstance()->Regist(TEXTURE);

	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(nTexIdx);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 30.0f);

	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		// 生成処理
		m_RockOnInfo[nCntGauge].pBillboard = CObjectBillboard::Create();
		if (m_RockOnInfo[nCntGauge].pBillboard == nullptr)
		{
			return E_FAIL;
		}

		CObjectBillboard* pObj = m_RockOnInfo[nCntGauge].pBillboard;

		// テクスチャ割り当て
		pObj->BindTexture(nTexIdx);

		// 種類の設定
		pObj->SetType(CObject::TYPE_NONE);

		// 情報設定
		pObj->SetSize(size);

		float rot = D3DX_PI * 0.25f + nCntGauge * DIV_ANGLE;
		UtilFunc::Transformation::RotNormalize(rot);

		pObj->SetRotation(rot);
		pObj->SetOriginPosition(GetPosition());

		m_RockOnInfo[nCntGauge].Angle = 1;
		m_RockOnInfo[nCntGauge].HeightAngle = 1;
		if (nCntGauge >= VTXTYPE_MAX / 2)
		{
			m_RockOnInfo[nCntGauge].Angle = -1;
		}

		if (nCntGauge == VTXTYPE_RIGHTDW ||
			nCntGauge == VTXTYPE_LEFTDW)
		{
			m_RockOnInfo[nCntGauge].HeightAngle = -1;
		}
	}


	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CRockOnMarker::Uninit()
{
	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		m_RockOnInfo[nCntGauge].pBillboard->Uninit();
		m_RockOnInfo[nCntGauge].pBillboard = nullptr;
	}

	CObjectBillboard::Uninit();
}

//==========================================================================
// 終了処理
//==========================================================================
void CRockOnMarker::Kill()
{
	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		if (m_RockOnInfo[nCntGauge].pBillboard != nullptr)
		{
			// 終了処理
			m_RockOnInfo[nCntGauge].pBillboard->Uninit();
			m_RockOnInfo[nCntGauge].pBillboard = nullptr;
		}
	}

	CObjectBillboard::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CRockOnMarker::Update()
{
	// 位置取得
	MyLib::Vector3 pos = GetPosition();

	// カメラの向き取得
	MyLib::Vector3 camerarot = CManager::GetInstance()->GetCamera()->GetRotation();

	float deltaTime = CManager::GetInstance()->GetDeltaTime();

	for (auto& info : m_RockOnInfo)
	{
		info.distancetimer += deltaTime;

		// 拡縮
		info.distance = ORIGIN_DISTANCE + fabsf(sinf(D3DX_PI * (info.distancetimer / TIME_SCALE))) * 10.0f;

		MyLib::Vector3 setpos = GetPosition();

		setpos = MyLib::Vector3(
			pos.x + sinf(D3DX_PI * 0.5f + camerarot.y) * (info.distance * info.Angle),
			pos.y + info.distance * info.HeightAngle,
			pos.z + cosf(D3DX_PI * 0.5f + camerarot.y) * (info.distance * info.Angle));	// 位置

		info.pBillboard->SetPosition(setpos);
		info.pBillboard->Update();
	}

	CObjectBillboard::Update();
}

//==========================================================================
// 描画処理
//==========================================================================
void CRockOnMarker::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//常に描画する

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ビルボードの描画
	CObjectBillboard::Draw();

	// 描画処理
	for (const auto& info : m_RockOnInfo)
	{
		info.pBillboard->Draw();
	}

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

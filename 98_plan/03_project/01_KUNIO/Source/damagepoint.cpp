//=============================================================================
// 
//  ダメージポイント処理 [damagepoint.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "damagepoint.h"
#include "manager.h"
#include "texture.h"
#include "renderer.h"
#include "multinumber.h"

//==========================================================================
// マクロ定義
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\number\\number_blackclover_01.png";
	const float SIZE_NUMBER = 20.0f;	// サイズ
	const int MAX_DIGIT = 3;			// 最大桁
	const float MAX_LIFE = 0.7f;		// 最大寿命
	const float LIFE_DOWNALPHA = 0.1f;	// 透明度減算寿命
	const float VELOCITY_UP = 40.0f;	// 上昇速度
}

//==========================================================================
// コンストラクタ
//==========================================================================
CDamagePoint::CDamagePoint(int nPriority)
{
	// 値のクリア
	m_fLife = 0.0f;	// 寿命
	m_apNumber = nullptr;
}

//==========================================================================
// デストラクタ
//==========================================================================
CDamagePoint::~CDamagePoint()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CDamagePoint* CDamagePoint::Create(MyLib::Vector3 pos, int nDamage)
{
	// 生成用のオブジェクト
	CDamagePoint* pDMGPoint = nullptr;

	// メモリの確保
	pDMGPoint = DEBUG_NEW CDamagePoint;

	if (pDMGPoint != nullptr)
	{
		// 初期化処理
		pDMGPoint->SetPosition(pos);
		pDMGPoint->Init();

		// 値の設定処理
		pDMGPoint->m_apNumber->SetValue(nDamage);
	}

	return pDMGPoint;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CDamagePoint::Init()
{
	SetType(CObject::TYPE_OBJECT2D);

	// 寿命設定
	m_fLife = MAX_LIFE;

	// 生成処理
	m_apNumber = CMultiNumber::Create(
		GetPosition(),
		D3DXVECTOR2(SIZE_NUMBER, SIZE_NUMBER),
		MAX_DIGIT, CNumber::EObjectType::OBJECTTYPE_BILLBOARD, TEXTURE, true, mylib_const::PRIORITY_ZSORT);
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CDamagePoint::Uninit()
{
	// 数字のオブジェクトの終了処理
	if (m_apNumber != nullptr)
	{
		m_apNumber->Uninit();
		m_apNumber = nullptr;
	}

	// 開放処理
	Release();
}

//==========================================================================
// 削除処理
//==========================================================================
void CDamagePoint::Kill()
{
	// 数字のオブジェクトの削除処理
	if (m_apNumber != nullptr)
	{
		m_apNumber->Kill();
		m_apNumber = nullptr;
	}

	// 削除処理
	Release();
}

//==========================================================================
// 更新処理
//==========================================================================
void CDamagePoint::Update()
{
	float deltatime = CManager::GetInstance()->GetDeltaTime();

	// 位置設定
	MyLib::Vector3 pos = GetPosition();
	pos.y += VELOCITY_UP * deltatime;
	SetPosition(pos);

	// 位置更新
	m_apNumber->SetPosition(GetPosition());
	m_apNumber->Update();

	// 色設定
	if (m_fLife <= LIFE_DOWNALPHA)
	{
		D3DXCOLOR col = m_apNumber->GetColor();
		col.a = m_fLife / LIFE_DOWNALPHA;
		m_apNumber->SetColor(col);
	}

	// 寿命減算
	m_fLife -= deltatime;
	if (m_fLife <= 0.0f)
	{
		Kill();
		return;
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CDamagePoint::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	// 常に描画する

	if (m_apNumber != nullptr)
	{
		m_apNumber->Draw();
	}

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

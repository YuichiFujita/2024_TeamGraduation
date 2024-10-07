//=============================================================================
// 
//  依頼人セリフエフェクト [serifeffect.cpp]
//  Author : 石原颯馬
// 
//=============================================================================
#include "serifeffect.h"
#include "texture.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string EFFECT_TEXTURE = "data\\TEXTURE\\effect\\voice_effect_00.png";
	const float EFFECT_DEF_SIZE = 20.0f;
	const float EFFECT_MAX_SIZE = EFFECT_DEF_SIZE * 1.8f;
	const float MOVE = 50.0f;
}

//==========================================================================
// コンストラクタ
//==========================================================================
CSerifEffect::CSerifEffect(int nPriority) : CObject3D(nPriority)
{
	m_Life = 0;
	m_MaxLife = 0;
}

//==========================================================================
// デストラクタ
//==========================================================================
CSerifEffect::~CSerifEffect()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CSerifEffect* CSerifEffect::Create(MyLib::Vector3 pos, MyLib::Vector3 rot, int life)
{
	// メモリの確保
	CSerifEffect* pObject = DEBUG_NEW CSerifEffect;

	if (pObject != nullptr)
	{
		pObject->m_MaxLife = life;
		pObject->m_Life = pObject->m_MaxLife;
		pObject->SetPosition(pos);
		pObject->SetOriginPosition(pos);
		pObject->SetRotation(rot);

		// 初期化処理
		pObject->Init();
	}

	return pObject;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CSerifEffect::Init()
{
	CObject3D::Init();

	SetType(CObject::TYPE::TYPE_OBJECT3D);

	BindTexture(CTexture::GetInstance()->Regist(EFFECT_TEXTURE));
	SetSize(MyLib::Vector3(EFFECT_DEF_SIZE, EFFECT_DEF_SIZE,0.0f));
	SetVtx();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CSerifEffect::Uninit()
{
	CObject3D::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CSerifEffect::Kill()
{
	
}

//==========================================================================
// 更新処理
//==========================================================================
void CSerifEffect::Update()
{
	// ライフ尽きたら消す
	m_Life--;
	if (m_Life <= 0)
	{
		Uninit();
		return;
	}

	// 進行度計算
	float progress = 1.0f - static_cast<float>(m_Life) / static_cast<float>(m_MaxLife);

	// 移動
	MyLib::Vector3 rot = GetRotation();
	float move = UtilFunc::Correction::EasingEaseOut(0.0f, MOVE, progress);
	MyLib::Vector3 vecMove = MyLib::Vector3();
	vecMove.x = -move * cosf(rot.z);
	vecMove.y = -move * sinf(rot.z);

	// 拡大
	float size = UtilFunc::Correction::EasingEaseOut(EFFECT_DEF_SIZE, EFFECT_MAX_SIZE, progress);

	// フェードアウト
	float alpha = UtilFunc::Correction::EasingEaseIn(1.0f, 0.0f, progress);

	
	// 設定
	D3DXCOLOR col = GetColor();
	col.a = alpha;
	SetPosition(GetOriginPosition() + vecMove);
	SetSize(size);
	SetColor(col);

	CObject3D::Update();
}

//==========================================================================
// 描画処理
//==========================================================================
void CSerifEffect::Draw()
{
	CObject3D::Draw();
}

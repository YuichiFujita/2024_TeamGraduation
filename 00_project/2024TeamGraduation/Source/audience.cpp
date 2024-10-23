//==========================================================================
// 
//  観客処理 [audience.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "audience.h"

// 派生先
#include "audienceAnim.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CAudience::CAudience(int nPriority, const LAYER layer) : CObject(nPriority, layer),
	m_type	(OBJTYPE_ANIM)	// オブジェクト種類
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CAudience::~CAudience()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CAudience* CAudience::Create(EObjType type)
{
	// メモリの確保
	CAudience* pAudience = nullptr;
	switch (type)
	{ // オブジェクト種類ごとの処理
	case CAudience::OBJTYPE_ANIM:
		pAudience = DEBUG_NEW CAudienceAnim;
		break;

	default:
		assert(false);
		break;
	}

	if (pAudience != nullptr)
	{
		// クラスの初期化
		if (FAILED(pAudience->Init()))
		{ // 初期化に失敗した場合

			// クラスの終了
			SAFE_UNINIT(pAudience);
			return nullptr;
		}

		// オブジェクトの種類
		pAudience->m_type = type;
	}

	return pAudience;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CAudience::Init()
{
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CAudience::Uninit()
{
	// オブジェクトの破棄
	Release();
}

//==========================================================================
// 削除処理
//==========================================================================
void CAudience::Kill()
{
	// 自身の終了
	Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CAudience::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	static float fRotSin = 0.0f;

	fRotSin += 0.035f;
	UtilFunc::Transformation::RotNormalize(fRotSin);

	pos.y = sinf(fRotSin) * 250.0f + 250.0f;
	rot.z = fRotSin;

	SetPosition(pos);
	SetRotation(rot);
}

//==========================================================================
// 描画処理
//==========================================================================
void CAudience::Draw()
{

}

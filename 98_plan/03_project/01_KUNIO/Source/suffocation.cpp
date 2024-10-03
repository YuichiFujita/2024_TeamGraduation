//=============================================================================
// 
//  サフォケーション処理 [suffocation.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "suffocation.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	// テクスチャのファイル
	const std::string TEXTURE_SAMPLE[] = 
	{ 
		"data\\TEXTURE\\subtitle\\suffocation_000.png",
		"data\\TEXTURE\\subtitle\\suffocation_001.png",
		"data\\TEXTURE\\subtitle\\suffocation_002.png",
		"data\\TEXTURE\\subtitle\\suffocation_003.png",
		"data\\TEXTURE\\subtitle\\suffocation_004.png",
		"data\\TEXTURE\\subtitle\\suffocation_005.png",
	};

	static int RANDOM_MOVEX = 250;
	static int RANDOM_MOVEINTERVAL = 2;
	static float VELOCITY_UP = 0.08f;

}

namespace StateTime	// 状態別時間
{
	const float BURST = 0.3f;	// 破裂
}


//==========================================================================
// 関数ポインタ
//==========================================================================
CSuffocation::STATE_FUNC CSuffocation::m_StateFunc[] =
{
	&CSuffocation::StateSurfacing,	// なし
	&CSuffocation::StateBurst,		// オープン
};

//==========================================================================
// コンストラクタ
//==========================================================================
CSuffocation::CSuffocation(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_fStateTime = 0.0f;		// 状態カウンター
	m_state = State::STATE_SURFACING;			// 状態
	m_fDestWidth = 0.0f;	// 目標の幅

	for (int i = 0; i < 6; i++)
	{
		m_nTexID[i] = 0;
	}
}

//==========================================================================
// デストラクタ
//==========================================================================
CSuffocation::~CSuffocation()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CSuffocation* CSuffocation::Create()
{
	// メモリの確保
	CSuffocation* pObj = DEBUG_NEW CSuffocation;

	if (pObj != nullptr)
	{
		// 初期化処理
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CSuffocation::Init()
{

	// オブジェクト2Dの初期化
	CObject2D::Init();

	for (int nCnt = 0; nCnt < 6; nCnt++)
	{
		// テクスチャ設定
		m_nTexID[nCnt] = CTexture::GetInstance()->Regist(TEXTURE_SAMPLE[nCnt]);
	}

	int r = rand() % 6;

	BindTexture(m_nTexID[r]);

	// リセット
	Reset();

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CSuffocation::Uninit()
{
	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CSuffocation::Update()
{
	// 状態更新
	UpdateState();
	if (IsDeath()) return;

	// 更新処理
	CObject2D::Update();
}

//==========================================================================
// 状態更新
//==========================================================================
void CSuffocation::UpdateState()
{
	// 状態タイマー
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// 状態リスト
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// 浮上
//==========================================================================
void CSuffocation::StateSurfacing()
{
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 move = GetMove();

	// 横揺れのランダム
	if (UtilFunc::Transformation::Random(0, RANDOM_MOVEINTERVAL) == 0)
	{
		m_fDestWidth = UtilFunc::Transformation::Random(-RANDOM_MOVEX, RANDOM_MOVEX) * 0.1f;
	}

	// 浮上
	move.y -= (VELOCITY_UP + UtilFunc::Transformation::Random(0, 30) * 0.01f);
	pos.y += move.y;

	// 横揺れ
	move.x += (m_fDestWidth - move.x) * 0.1f;
	pos.x = 640.0f + move.x;

	// 中央
	if (pos.y <= SCREEN_HEIGHT * 0.5f)
	{
		CSound::GetInstance()->PlaySound(CSound::LABEL_SE_CLUCKBUBBLES);

		move.y = 0.0f;
		pos.y = SCREEN_HEIGHT * 0.5f;
		SetPosition(pos);

		// 状態遷移
		m_state = State::STATE_BURST;
		m_fStateTime = 0.0f;
	}
	SetMove(move);
	SetPosition(pos);
}

//==========================================================================
// 破裂
//==========================================================================
void CSuffocation::StateBurst()
{
	// 位置更新
	MyLib::Vector3 pos = GetPosition();
	pos.x += (640.0f - pos.x) * 0.2f;
	SetPosition(pos);

	// サイズ更新
	D3DXVECTOR2 size = GetSize(), sizeOrigin = GetSizeOrigin();
	size.x = UtilFunc::Correction::EasingEaseInOut(sizeOrigin.x, sizeOrigin.x * 5.0f, 0.0f, StateTime::BURST, m_fStateTime);
	size.y = UtilFunc::Correction::EasingEaseInOut(sizeOrigin.y, sizeOrigin.y * 5.0f, 0.0f, StateTime::BURST, m_fStateTime);
	SetSize(size);

	// 不透明度
	float alpha = UtilFunc::Correction::EasingEaseInOut(1.0f, 0.0f, 0.0f, StateTime::BURST, m_fStateTime);
	SetAlpha(alpha);

	if (m_fStateTime >= StateTime::BURST)
	{
		// リセット
		Reset();
	}
}

//==========================================================================
// リセット処理
//==========================================================================
void CSuffocation::Reset()
{
	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(GetIdxTexture());

	// 縦幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);
	SetSize(size);
	SetSizeOrigin(size);

	SetPosition(MyLib::Vector3(SCREEN_WIDTH * 0.5f, 720.0f + size.y, 0.0f));
	SetOriginPosition(GetPosition());

	m_fStateTime = 0.0f;		// 状態カウンター
	m_state = State::STATE_SURFACING;	// 状態

	SetAlpha(1.0f);

	int r = rand() % 6;

	BindTexture(m_nTexID[r]);
}

//==========================================================================
// 描画処理
//==========================================================================
void CSuffocation::Draw()
{
	// 描画処理
	CObject2D::Draw();
}

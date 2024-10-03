//=============================================================================
// 
//  葉っぱ処理 [leaf.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "leaf.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "game.h"
#include "course.h"

// 派生クラス
#include "leaf_flow.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\effect\\leaf.png";	// テクスチャのファイル
}

namespace StateTime	// 状態別時間
{
	const float WAIT = 0.2f;	// 待機
	const float FADEIN = 0.5f;	// フェードイン
	const float FADEOUT = 0.3f;	// フェードアウト
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CLeaf::STATE_FUNC CLeaf::m_StateFunc[] =
{
	&CLeaf::StateFall,		// 落下
	&CLeaf::StateWait,		// 待機
	&CLeaf::StateFadeIn,	// フェードイン
	&CLeaf::StateFadeOut,	// フェードアウト
};

//==========================================================================
// コンストラクタ
//==========================================================================
CLeaf::CLeaf(int nPriority, const LAYER layer) : CObject3D(nPriority, layer)
{
	// 値のクリア
	m_fStateTimer = 0.0f;			// 状態カウンター
	m_state = State::STATE_FALL;	// 状態
	m_rotStart = MyLib::Vector3();	// 開始時の向き
	m_rotDest = MyLib::Vector3();	// 目標の向き
	m_fFallWidth = 0.0f;			// 落下幅
	m_fWidthMoveTimer = 0.0f;		// 幅移動のタイマー
	m_fRotateTimer = 0.0f;			// 回転までの時間
	m_fRotateInterval = 0.0f;		// 回転までの間隔
}

//==========================================================================
// デストラクタ
//==========================================================================
CLeaf::~CLeaf()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CLeaf* CLeaf::Create(const MyLib::Vector3& pos, const Type& type)
{
	// メモリの確保
	CLeaf* pObj = nullptr;

	switch (type)
	{
	case Type::TYPE_FALL:
		pObj = DEBUG_NEW CLeaf;
		break;

	case Type::TYPE_FLOW:
		pObj = DEBUG_NEW CLeafFlow;
		break;

	default:
		return nullptr;
		break;
	}

	if (pObj == nullptr)
	{// 失敗
		return nullptr;
	}

	// 引数情報設定
	pObj->SetPosition(pos);
	pObj->SetOriginPosition(pos);

	// 初期化処理
	pObj->Init();

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CLeaf::Init()
{

	// オブジェクト3Dの初期化
	CObject3D::Init();

	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texID);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

	// 縦幅を元にサイズ設定
	float randSize = UtilFunc::Transformation::Random(200, 400) * 0.1f;
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, randSize);

	MyLib::Vector3 setsize = MyLib::Vector3(size.x, size.y, 0.0f);
	SetSize(setsize);
	SetSizeOrigin(setsize);

	// 向き
	MyLib::Vector3 rot;
	rot.y = UtilFunc::Transformation::Random(-31, 31) * 0.1f;
	rot.x = UtilFunc::Transformation::Random(-31, 31) * 0.01f;
	rot.z = UtilFunc::Transformation::Random(-31, 31) * 0.01f;
	SetRotation(rot);

	// 目標の向き
	CalDestRotation();

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT3D);


	// 状態
	m_fRotateInterval = UtilFunc::Transformation::Random(60, 90) * 0.01f;
	m_fFallWidth = static_cast<float>(UtilFunc::Transformation::Random(80, 150));	// 落下幅
	m_state = State::STATE_FADEIN;	// 状態

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CLeaf::Uninit()
{
	// 終了処理
	CObject3D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CLeaf::Update()
{
	// 状態更新
	UpdateState();
	if (IsDeath()) return;

	// 更新処理
	CObject3D::Update();
}

//==========================================================================
// 目標の向き計算
//==========================================================================
void CLeaf::CalDestRotation()
{
	// 向き取得
	MyLib::Vector3 rot = GetRotation();

	// 目標の向き
	m_rotDest.y = rot.y + UtilFunc::Transformation::Random(11, 31) * 0.01f;
	m_rotDest.x = rot.x + UtilFunc::Transformation::Random(21, 51) * 0.01f;
	m_rotDest.z = rot.z + UtilFunc::Transformation::Random(21, 51) * 0.01f;
	UtilFunc::Transformation::RotNormalize(m_rotDest);

	// 開始時の向き
	m_rotStart = rot;
}

//==========================================================================
// 状態更新
//==========================================================================
void CLeaf::UpdateState()
{
	// 状態タイマー
	m_fStateTimer += CManager::GetInstance()->GetDeltaTime();

	// 状態更新
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// 落下
//==========================================================================
void CLeaf::StateFall()
{
	MyLib::Vector3 pos = GetPosition(), posOrigin = GetOriginPosition();
	MyLib::Vector3 rot = GetRotation();

	// 回転までの時間加算
	m_fRotateTimer += CManager::GetInstance()->GetDeltaTime();

	// 幅移動のタイマー
	m_fWidthMoveTimer += CManager::GetInstance()->GetDeltaTime();


	// 移動
	float ratio = sinf(D3DX_PI * (1.0f * (m_fWidthMoveTimer / m_fRotateInterval)));
	float len = ratio * m_fFallWidth;
	pos.x = posOrigin.x + sinf(rot.y) * len;
	pos.z = posOrigin.z + cosf(rot.y) * len;


	// 回転線形補間
	rot = UtilFunc::Correction::EasingEaseOut(m_rotStart, m_rotDest, 0.0f, m_fRotateInterval, m_fRotateTimer);
	UtilFunc::Transformation::RotNormalize(rot);
	SetRotation(rot);

	// 落下
	pos.y -= UtilFunc::Transformation::Random(150, 190) * 0.01f;

	// 時間経過、新しい目標向き算出
	if (m_fRotateTimer >= m_fRotateInterval)
	{
		m_fRotateTimer = 0.0f;

		// 目標の向き
		CalDestRotation();

		// 落下幅
		m_fFallWidth = static_cast<float>(UtilFunc::Transformation::Random(80, 150));
	}

	// 情報設定
	SetPosition(pos);

	// 着地判定
	CheckLand();
}

//==========================================================================
// 着地確認
//==========================================================================
void CLeaf::CheckLand()
{
	bool IsLand = false;


	// 位置取得
	MyLib::Vector3 pos = GetPosition();

	// ゲーム取得
	CGame* pGame = CGame::GetInstance();

	float height = 0.0f;
	if (pGame != nullptr)
	{
		// コース取得
		CCourse* pCourse = pGame->GetCourse();
		if (pCourse == nullptr) return;

		// 高さ取得
		height = pCourse->GetHeight(pos, &IsLand);
	}

	if (pos.y <= 0.0f)
	{
		// 情報設定
		pos.y = height;
		SetPosition(pos);

		// フェードアウトに設定
		m_state = State::STATE_FADEOUT;
		m_fStateTimer = 0.0f;
	}
}

//==========================================================================
// 待機
//==========================================================================
void CLeaf::StateWait()
{
	MyLib::Vector3 pos = GetPosition(), posOrigin = GetOriginPosition();
	pos = UtilFunc::Correction::EasingEaseOut(posOrigin, posOrigin + MyLib::Vector3(0.0f, -10.0f, 0.0f), 0.0f, StateTime::WAIT, m_fStateTimer);
	SetPosition(pos);

	if (m_fStateTimer >= StateTime::WAIT)
	{// 時間経過

		m_state = State::STATE_FALL;
		m_fStateTimer = 0.0f;

		SetOriginPosition(pos);
	}
}

//==========================================================================
// フェードイン
//==========================================================================
void CLeaf::StateFadeIn()
{
	// 不透明度設定
	float alpha = m_fStateTimer / StateTime::FADEIN;
	SetAlpha(alpha);

	// 降下は行う
	StateFall();

	if (m_fStateTimer >= StateTime::FADEIN)
	{// 時間経過

		// 不透明度設定
		SetAlpha(1.0f);

		// 状態
		m_state = State::STATE_FALL;
		m_fStateTimer = 0.0f;
	}
}

//==========================================================================
// フェードアウト
//==========================================================================
void CLeaf::StateFadeOut()
{
	float alpha = 1.0f - (m_fStateTimer / StateTime::FADEOUT);
	SetAlpha(alpha);

	if (m_fStateTimer >= StateTime::FADEOUT)
	{// 時間経過

		Uninit();
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CLeaf::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 背面のカリングなし
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// 描画処理
	CObject3D::Draw();

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// カリングデフォルト
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//=============================================================================
// 
//  判定表示 [judgeobj.cpp]
//  Author : 石原颯馬
// 
//=============================================================================
#include "judgeobj.h"
#include "judge.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "judgestatefunc.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::vector<std::string> TEXTURE =
	{
		"data\\TEXTURE\\judge\\judge_texture_01.png",
		"data\\TEXTURE\\judge\\judge_texture_02.png",
		"data\\TEXTURE\\judge\\judge_texture_03.png",
		"data\\TEXTURE\\judge\\judge_texture_04.png",
	};
}

//**************************************************************************
// 判定オブジェクト
//**************************************************************************
//==========================================================================
// コンストラクタ
//==========================================================================
CJudgeObj::CJudgeObj(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	memset(m_RockOnInfo, 0, sizeof(m_RockOnInfo));
	m_pStateFunc = nullptr;
}

//==========================================================================
// デストラクタ
//==========================================================================
CJudgeObj::~CJudgeObj()
{
	if (m_pStateFunc != nullptr)
	{
		delete m_pStateFunc;
		m_pStateFunc = nullptr;
	}
}

//==========================================================================
// 生成処理
//==========================================================================
CJudgeObj* CJudgeObj::Create(const MyLib::Vector3& pos, const CJudge::JUDGE judge)
{
	// メモリの確保
	CJudgeObj* pMarker = DEBUG_NEW CJudgeObj;

	if (pMarker != nullptr)
	{// メモリの確保が出来ていたら

		// 位置設定
		pMarker->SetPosition(pos);
		pMarker->SetOriginPosition(pos);
		pMarker->m_Judge = judge;

		// 初期化処理
		pMarker->Init();
	}

	return pMarker;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CJudgeObj::Init()
{
	// オブジェクト2Dの初期化
	CObject2D::Init();

	// テクスチャ設定
	int pointTex = CTexture::GetInstance()->Regist(TEXTURE[m_Judge]);
	BindTexture(pointTex);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(pointTex);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 120.0f);
	SetSize(D3DXVECTOR2(size.x, 0.0f));
	SetSizeOrigin(size);

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// 状態関数生成
	CreateState();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CJudgeObj::Uninit()
{
	CObject2D::Uninit();
}

//==========================================================================
// 終了処理
//==========================================================================
void CJudgeObj::Kill()
{
	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		if (m_RockOnInfo[nCntGauge].p2D != nullptr)
		{
			// 終了処理
			m_RockOnInfo[nCntGauge].p2D->Uninit();
			m_RockOnInfo[nCntGauge].p2D = nullptr;
		}
	}

	CObject2D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CJudgeObj::Update()
{
	// 状態関数処理
	if (m_pStateFunc != nullptr)
	{
		// 状態タイマー加算
		m_pStateFunc->AddStateTime(CManager::GetInstance()->GetDeltaTime());

		// 状態別処理
		m_pStateFunc->ActionState();
	}
	else
	{//ない（生成しとけ～）
		assert(0 && "生成しとけ～");
	}

	if (IsDeath())
	{
		return;
	}

	CObject2D::Update();
}

//==========================================================================
// 描画処理
//==========================================================================
void CJudgeObj::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// オブジェクト2Dの描画
	CObject2D::Draw();

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//==========================================================================
// 状態関数生成処理
//==========================================================================
void CJudgeObj::CreateState()
{
	if (m_Judge == CJudge::JUDGE::JUDGE_AAA)
	{
		m_pStateFunc = DEBUG_NEW CJudgeStateFunc_AAA(this);
		CSound::GetInstance()->PlaySound(CSound::LABEL_SE_AWESOME);
	}
	else if (m_Judge == CJudge::JUDGE::JUDGE_BBB)
	{
		m_pStateFunc = DEBUG_NEW CJudgeStateFunc_BBB(this);
		CSound::GetInstance()->PlaySound(CSound::LABEL_SE_GOOD);
	}
	else if (m_Judge == CJudge::JUDGE::JUDGE_CCC)
	{
		m_pStateFunc = DEBUG_NEW CJudgeStateFunc_CCC(this);
		CSound::GetInstance()->PlaySound(CSound::LABEL_SE_BAD);
	}
	else if (m_Judge == CJudge::JUDGE::JUDGE_DDD)
	{
		m_pStateFunc = DEBUG_NEW CJudgeStateFunc_DDD(this);
		CSound::GetInstance()->PlaySound(CSound::LABEL_SE_WORST);
	}
}

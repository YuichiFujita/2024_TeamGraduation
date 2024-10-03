//=============================================================================
// 
//  戦闘開始処理 [battlestart.cpp]
//  Author : 相馬靜雅, 石原颯馬
// 
//=============================================================================
#include "battlestart.h"
#include "battlestart_effect.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "calculation.h"
#include "particle.h"
#include "game.h"
#include "camera.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string SWORD_TEXTURE = "data\\TEXTURE\\battlestart\\sword.png";				// テクスチャのファイル
	const std::string SWORD_TEXTURE_COMPLETE = "data\\TEXTURE\\battlestart\\sword_complete.png";				// テクスチャのファイル
	const std::string TEXT_TEXTURE_COMPLETE = "data\\TEXTURE\\battlestart\\don2.png";				// テクスチャのファイル
	const std::string TEXT_TEXTURE = "data\\TEXTURE\\battlestart\\don.png";	// テクスチャのファイル
	const float TIME_EXPANSION = 1.0f;	// 拡大時間
	const float TIME_WAIT = 0.4f;		// 待ち時間
	const float TIME_DROP = 0.5f;		// 落とす時間
	const float TIME_FADEOUT = 0.6f;	// フェードアウト時間
	const MyLib::Vector3 DESTROTATION_GETTOGETHER = MyLib::Vector3(0.0f, 0.0f, D3DX_PI * 0.75f);
	const MyLib::Vector3 DESTPOSITION_GETTOGETHER = MyLib::Vector3(130.0f, 230.0f, 0.0f);
	const MyLib::Vector3 DESTPOSITION_STINGS = MyLib::Vector3(0.0f, 360.0f, 0.0f);
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CBattleStart::STATE_FUNC CBattleStart::m_StateFuncList[] =
{
	&CBattleStart::StateExpansion,		//拡大
	&CBattleStart::StateWait,			//待ち
	&CBattleStart::StateDrop,			//落とす
	&CBattleStart::StateFadeOut,		//フェードアウト
};

//==========================================================================
// コンストラクタ
//==========================================================================
CBattleStart::CBattleStart(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_state = STATE::STATE_EXPANTION;		// 状態
	m_fStateTime = 0.0f;	// 状態タイマー
	memset(m_RockOnInfo, 0, sizeof(m_RockOnInfo));
}

//==========================================================================
// デストラクタ
//==========================================================================
CBattleStart::~CBattleStart()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CBattleStart* CBattleStart::Create(const MyLib::Vector3& pos)
{
	// メモリの確保
	CBattleStart* pMarker = DEBUG_NEW CBattleStart;

	if (pMarker != nullptr)
	{// メモリの確保が出来ていたら

		// 位置設定
		pMarker->SetPosition(pos);
		pMarker->SetOriginPosition(pos);

		// 初期化処理
		pMarker->Init();
	}

	return pMarker;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CBattleStart::Init()
{

	CTexture::GetInstance()->Regist(TEXT_TEXTURE);
	CTexture::GetInstance()->Regist(TEXT_TEXTURE_COMPLETE);

	// オブジェクト2Dの初期化
	CObject2D::Init();
	int pointTex = CTexture::GetInstance()->Regist(TEXT_TEXTURE);
	BindTexture(pointTex);
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(pointTex);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);
	SetSize(D3DXVECTOR2(size.x, 0.0f));
	SetSizeOrigin(size);

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	int nTexIdx = CTexture::GetInstance()->Regist(SWORD_TEXTURE);

	size = CTexture::GetInstance()->GetImageSize(nTexIdx);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 120.0f);

	MyLib::Vector3 pos = GetPosition();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CBattleStart::Uninit()
{
	CObject2D::Uninit();
}

//==========================================================================
// 終了処理
//==========================================================================
void CBattleStart::Kill()
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
void CBattleStart::Update()
{
	// 状態タイマー加算
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// 状態別処理
	(this->*(m_StateFuncList[m_state]))();

	if (IsDeath())
	{
		return;
	}

	CObject2D::Update();
}

//==========================================================================
// 拡大
//==========================================================================
void CBattleStart::StateExpansion()
{
	if (m_fStateTime >= TIME_EXPANSION)
	{
		m_fStateTime = 0.0f;
		m_state = STATE_WAIT;
		//CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_BATTLESTART_CHARGE);
		return;
	}

	float ratio = m_fStateTime / TIME_EXPANSION;

	// サイズ設定
	D3DXVECTOR2 size = GetSize();
	size.x = UtilFunc::Correction::EasingEaseIn(0.0f, GetSizeOrigin().x * 1.6f, ratio);
	size.y = UtilFunc::Correction::EasingEaseIn(0.0f, GetSizeOrigin().y * 1.6f, ratio);
	SetSize(size);

}

//==========================================================================
// 落とす
//==========================================================================
void CBattleStart::StateWait()
{
	if (m_fStateTime >= TIME_WAIT)
	{
		m_fStateTime = 0.0f;
		m_state = STATE_DROP;
	}
}

//==========================================================================
// 落とす
//==========================================================================
void CBattleStart::StateDrop()
{
	if (m_fStateTime >= TIME_DROP)
	{
		m_fStateTime = 0.0f;
		m_state = STATE_FADEOUT;
		//CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_BATTLESTART_CHARGE);

		SetSize(GetSizeOrigin());

		// 完了後のテクスチャに切替
		int nTexIdx = CTexture::GetInstance()->Regist(TEXT_TEXTURE_COMPLETE);
		BindTexture(nTexIdx);
 
		// 刺さりパーティクル生成
		my_particle::Create(GetPosition(), my_particle::TYPE::TYPE_BATTLESTART);

		// 文字エフェクト生成
		CBattleStart_Effect::Create(GetPosition());
		return;
	}

	float ratio = m_fStateTime / TIME_DROP;

	// サイズ設定
	D3DXVECTOR2 size = GetSize();
	size.x = UtilFunc::Correction::EasingEaseIn(GetSizeOrigin().x * 1.6f, GetSizeOrigin().x, ratio);
	size.y = UtilFunc::Correction::EasingEaseIn(GetSizeOrigin().y * 1.6f, GetSizeOrigin().y, ratio);
	SetSize(size);
}

//==========================================================================
// フェードアウト
//==========================================================================
void CBattleStart::StateFadeOut()
{
	if (m_fStateTime >= TIME_FADEOUT)
	{
		m_fStateTime = 0.0f;
		Uninit();

		// 戦闘開始に遷移
		CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_MAIN);
		return;
	}
	float ratio = 1.0f - m_fStateTime / TIME_FADEOUT;

	// 不透明度設定
	SetAlpha(ratio);
}

//==========================================================================
// 描画処理
//==========================================================================
void CBattleStart::Draw()
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

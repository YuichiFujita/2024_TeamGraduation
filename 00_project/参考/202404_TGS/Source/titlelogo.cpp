//=============================================================================
// 
// タイトルロゴ処理 [titlelogo.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "titlelogo.h"
#include "renderer.h"
#include "texture.h"
#include "manager.h"
#include "sound.h"
#include "calculation.h"
#include "fade.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\title\\titlelogo4.png";
}

namespace TexturePath	// テクスチャパス
{
	const std::string WATER = "data\\TEXTURE\\title\\water.png";
	const std::string PLAYER = "data\\TEXTURE\\title\\player.png";
	const std::string BRESS = "data\\TEXTURE\\title\\bress.png";
	const std::string LOVE[] =
	{
		"data\\TEXTURE\\title\\L.png",
		"data\\TEXTURE\\title\\O.png",
		"data\\TEXTURE\\title\\V.png",
		"data\\TEXTURE\\title\\E.png",
	};
	const std::string SUITON = "data\\TEXTURE\\title\\suiton.png";
}

namespace Size	// サイズ
{
	const float WATER = 250.0f;
	const float PLAYER = 100.0f;
	const float BRESS = 80.0f;
	const float LOVE = 60.0f;
	const float SUITON = 100.0f;
}

namespace BasePoint	// 基点の位置
{
	const MyLib::Vector3 WATER = MyLib::Vector3(640.0f, 270.0f, 0.0f);
	const MyLib::Vector3 PLAYER = MyLib::Vector3(740.0f, 415.0f, 0.0f);
	const MyLib::Vector3 BRESS = MyLib::Vector3(495.0f, 265.0f, 0.0f);
	const MyLib::Vector3 LOVE = MyLib::Vector3(313.0f, 175.0f, 0.0f);
	const MyLib::Vector3 SUITON = MyLib::Vector3(830.0f, 170.0f, 0.0f);
}

namespace DestPoint	// 目標の位置
{
	const MyLib::Vector3 PLAYER = MyLib::Vector3(640.0f, 395.0f, 0.0f);
	const MyLib::Vector3 BRESS = MyLib::Vector3(510.0f, 347.0f, 0.0f);
	const MyLib::Vector3 LOVE[] =	// LOVEの場合は差分
	{
		MyLib::Vector3(-10.0f, -20.0f, 0.0f),
		MyLib::Vector3(-5.0f, -60.0f, 0.0f),
		MyLib::Vector3(0.0f, -50.0f, 0.0f),
		MyLib::Vector3(5.0f, -20.0f, 0.0f),
	};
}


namespace DestRotation	// 目標の向き
{
	const MyLib::Vector3 LOVE[] =
	{
		MyLib::Vector3(0.0f, 0.0f, D3DX_PI * 0.15f),
		MyLib::Vector3(0.0f, 0.0f, D3DX_PI * 0.08f),
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		MyLib::Vector3(0.0f, 0.0f, D3DX_PI * -0.1f),
	};
}

namespace StateTime	// 状態時間
{
	const float WAIT = 1.0f;
	const float WATER = 1.0f;
	const float PLAYER = 1.2f;
	const float BRESS = 1.5f;
	const float AFTERMOVEMENT = 2.0f;
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CTitleLogo::STATE_FUNC CTitleLogo::m_StateFunc[] =
{
	&CTitleLogo::StateNone,					// なし
	&CTitleLogo::StateWait,					// 待機
	&CTitleLogo::StateFadeIn_Water,			// 水フェードイン
	&CTitleLogo::StateFadeIn_Player,	// プレイヤーと名前フェードイン
	&CTitleLogo::StateBress,				// 息
	&CTitleLogo::StateWait_After,			// 後の待機
	&CTitleLogo::StateAfterMovement,		// 後の動き
};

//==========================================================================
// コンストラクタ
//==========================================================================
CTitleLogo::CTitleLogo(float fadetime, int nPriority) : m_fFadeOutTime(fadetime), CObject(nPriority)
{
	// 値のクリア
	m_state = STATE_NONE;	// 状態
	m_fStateTime = 0.0f;	// 状態カウンター
}

//==========================================================================
// デストラクタ
//==========================================================================
CTitleLogo::~CTitleLogo()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CTitleLogo* CTitleLogo::Create(float fadetime)
{
	// メモリの確保
	CTitleLogo* pScreen = DEBUG_NEW CTitleLogo(fadetime);

	if (pScreen != nullptr)
	{
		// 初期化処理
		pScreen->Init();
	}

	return pScreen;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CTitleLogo::Init()
{
	// 種類の設定
	SetType(TYPE_OBJECT2D);

	// 水生成
	CreateWater();

	// プレイヤー生成
	CreatePlayer();

	// 息生成
	CreateBress();

	// LOVE生成
	CreateLOVE();

	// SUITON生成
	CreateSUITON();

	// LOVEの間隔ランダム選定
	DrawingLOVEDistance();

	// 状態遷移
	SetState(State::STATE_WAIT);

	return S_OK;
}

//==========================================================================
// 水生成
//==========================================================================
void CTitleLogo::CreateWater()
{
	// 生成
	m_pWater = CObject2D::Create(GetPriority() + 1);
	CObject2D* pObj2D = m_pWater;
	pObj2D->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// 位置設定
	pObj2D->SetPosition(BasePoint::WATER);
	pObj2D->SetOriginPosition(BasePoint::WATER);


	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TexturePath::WATER);
	pObj2D->BindTexture(texID);

	// 縦幅を元にサイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, Size::WATER);

	// サイズ設定
	pObj2D->SetSize(size);
	pObj2D->SetSizeOrigin(size);

	// 透明度設定
	pObj2D->SetAlpha(0.0f);
}

//==========================================================================
// プレイヤー生成
//==========================================================================
void CTitleLogo::CreatePlayer()
{
	// 生成
	m_pPlayer = CObject2D::Create(GetPriority());
	CObject2D* pObj2D = m_pPlayer;
	pObj2D->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// 位置設定
	pObj2D->SetPosition(BasePoint::PLAYER);
	pObj2D->SetOriginPosition(BasePoint::PLAYER);


	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TexturePath::PLAYER);
	pObj2D->BindTexture(texID);

	// 縦幅を元にサイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, Size::PLAYER);

	// サイズ設定
	pObj2D->SetSize(size);
	pObj2D->SetSizeOrigin(size);

	// 透明度設定
	pObj2D->SetAlpha(0.0f);
}

//==========================================================================
// 息生成
//==========================================================================
void CTitleLogo::CreateBress()
{
	// 生成
	m_pBress = CObject2D::Create(GetPriority());
	CObject2D* pObj2D = m_pBress;
	pObj2D->SetType(CObject::TYPE::TYPE_OBJECT2D);
	pObj2D->SetAnchorType(CObject2D::AnchorPoint::UNDER_CENTER);

	// 位置設定
	pObj2D->SetPosition(BasePoint::BRESS);
	pObj2D->SetOriginPosition(BasePoint::BRESS);

	// 向き
	pObj2D->SetRotation(MyLib::Vector3(0.0f, 0.0f, D3DX_PI * -0.15f));


	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TexturePath::BRESS);
	pObj2D->BindTexture(texID);

	// 縦幅を元にサイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, Size::BRESS);

	// サイズ設定
	pObj2D->SetSize(D3DXVECTOR2(size.x, 0.0f));
	pObj2D->SetSizeOrigin(size);

	// 透明度設定
	pObj2D->SetAlpha(0.0f);
}

//==========================================================================
// LOVE生成
//==========================================================================
void CTitleLogo::CreateLOVE()
{
	for (int i = 0; i < LOGO_LOVE::MAX; i++)
	{
		// 生成
		m_pLOVE[i] = CObject2D::Create(GetPriority());
		CObject2D* pObj2D = m_pLOVE[i];
		pObj2D->SetType(CObject::TYPE::TYPE_OBJECT2D);

		// テクスチャ設定
		int texID = CTexture::GetInstance()->Regist(TexturePath::LOVE[i]);
		pObj2D->BindTexture(texID);

		// 縦幅を元にサイズ設定
		D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
		size = UtilFunc::Transformation::AdjustSizeByHeight(size, Size::LOVE);

		// サイズ設定
		pObj2D->SetSize(size);
		pObj2D->SetSizeOrigin(size);

		// 位置設定
		pObj2D->SetPosition(BasePoint::LOVE + (MyLib::Vector3(size.x * 1.45f, 0.0f, 0.0f) * i));
		pObj2D->SetOriginPosition(pObj2D->GetPosition());

		// 透明度設定
		pObj2D->SetAlpha(0.0f);
	}
}

//==========================================================================
// SUITON生成
//==========================================================================
void CTitleLogo::CreateSUITON()
{
	// 生成
	m_pSUITON = CObject2D::Create(GetPriority());
	CObject2D* pObj2D = m_pSUITON;
	pObj2D->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// 位置設定
	pObj2D->SetPosition(BasePoint::SUITON);
	pObj2D->SetOriginPosition(BasePoint::SUITON);


	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TexturePath::SUITON);
	pObj2D->BindTexture(texID);

	// 縦幅を元にサイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, Size::SUITON);

	// サイズ設定
	pObj2D->SetSize(size);
	pObj2D->SetSizeOrigin(size);

	// 透明度設定
	pObj2D->SetAlpha(0.0f);
}

//==========================================================================
// 終了処理
//==========================================================================
void CTitleLogo::Uninit()
{
	// 終了処理
	Release();
}

//==========================================================================
// 更新処理
//==========================================================================
void CTitleLogo::Update()
{
	if (CManager::GetInstance()->GetFade()->GetState() != CFade::STATE_NONE)
	{// フェード中は抜ける
		return;
	}

	// 状態別更新
	UpdateState();
	if (IsDeath())
	{
		return;
	}

#if 0
	MyLib::Vector3 posWater = m_pWater->GetPosition();
	ImGui::DragFloat3("posWater", (float*)&posWater, 0.5f, 0.0f, 0.0f, "%.2f");
	m_pWater->SetPosition(posWater);

	MyLib::Vector3 posPlayer = m_pPlayer->GetPosition();
	ImGui::DragFloat3("posPlayer", (float*)&posPlayer, 0.5f, 0.0f, 0.0f, "%.2f");
	m_pPlayer->SetPosition(posPlayer);

	MyLib::Vector3 posBress = m_pBress->GetPosition();
	ImGui::DragFloat3("posBress", (float*)&posBress, 0.5f, 0.0f, 0.0f, "%.2f");
	m_pBress->SetPosition(posBress);

	for (int i = 0; i < LOGO_LOVE::MAX; i++)
	{
		CObject2D* pObj2D = m_pLOVE[i];

		// 位置設定
		MyLib::Vector3 pos = pObj2D->GetPosition();
		ImGui::Text("[%d]", i);
		ImGui::SameLine();
		ImGui::DragFloat3("posLove", (float*)&pos, 0.5f, 0.0f, 0.0f, "%.2f");
		pObj2D->SetPosition(pos);
	}

	MyLib::Vector3 posSUITON = m_pSUITON->GetPosition();
	ImGui::DragFloat3("posSUITON", (float*)&posSUITON, 0.5f, 0.0f, 0.0f, "%.2f");
	m_pSUITON->SetPosition(posSUITON);
#endif
}

//==========================================================================
// 状態更新
//==========================================================================
void CTitleLogo::UpdateState()
{
	// 状態遷移カウンター加算
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// なにもなし
//==========================================================================
void CTitleLogo::StateNone()
{
	m_fStateTime = 0.0f;
}

//==========================================================================
// 待機
//==========================================================================
void CTitleLogo::StateWait()
{
	if (m_fStateTime >= StateTime::WAIT)
	{// 時間経過
		SetState(State::STATE_FADEIN_WATER);
	}
}

//==========================================================================
// 水フェードイン
//==========================================================================
void CTitleLogo::StateFadeIn_Water()
{
	//=============================
	// 名前
	//=============================
	{
		// 不透明度設定
		float alpha = UtilFunc::Transformation::Clamp(m_fStateTime / StateTime::WATER, 0.0f, 1.0f);
		for (const auto& love : m_pLOVE)
		{
			love->SetAlpha(alpha);
		}
		m_pSUITON->SetAlpha(alpha);

		// サイズ更新
		D3DXVECTOR2 size, sizeOrigin = m_pSUITON->GetSizeOrigin();
		size.x = UtilFunc::Correction::EaseOutBack(0.0f, sizeOrigin.x, 0.0f, StateTime::WATER, m_fStateTime, 4.5f);
		size.y = UtilFunc::Correction::EaseOutBack(0.0f, sizeOrigin.y, 0.0f, StateTime::WATER, m_fStateTime, 4.5f);
		m_pSUITON->SetSize(size);

		for (const auto& love : m_pLOVE)
		{
			D3DXVECTOR2 size, sizeOrigin = love->GetSizeOrigin();
			size.x = UtilFunc::Correction::EaseOutBack(0.0f, sizeOrigin.x, 0.0f, StateTime::WATER, m_fStateTime, 4.5f);
			size.y = UtilFunc::Correction::EaseOutBack(0.0f, sizeOrigin.y, 0.0f, StateTime::WATER, m_fStateTime, 4.5f);
			love->SetSize(size);
		}

	}

	// 不透明度設定
	float alpha = UtilFunc::Transformation::Clamp(m_fStateTime / StateTime::WATER, 0.0f, 1.0f);
	m_pWater->SetAlpha(alpha);

	if (m_fStateTime >= StateTime::WATER)
	{// 時間経過
		SetState(State::STATE_FADEIN_PLAYER);
	}
}

//==========================================================================
// プレイヤーと名前フェードイン
//==========================================================================
void CTitleLogo::StateFadeIn_Player()
{
	//=============================
	// プレイヤー
	//=============================
	{
		// 不透明度設定
		float alpha = UtilFunc::Transformation::Clamp(m_fStateTime / StateTime::PLAYER, 0.0f, 1.0f);
		m_pPlayer->SetAlpha(alpha);

		// プレイヤー移動
		MyLib::Vector3 playerpos = UtilFunc::Correction::EasingEaseOut(BasePoint::PLAYER, DestPoint::PLAYER, 0.0f, StateTime::PLAYER, m_fStateTime);
		m_pPlayer->SetPosition(playerpos);
	}

	if (m_fStateTime >= StateTime::PLAYER)
	{// 時間経過
		SetState(State::STATE_BRESS);
	}
}

//==========================================================================
// 息
//==========================================================================
void CTitleLogo::StateBress()
{
	//=============================
	// 息
	//=============================
	{
		// 不透明度設定
		float alpha = UtilFunc::Correction::EaseOutExpo(0.0f, 1.0f, 0.0f, StateTime::BRESS, m_fStateTime);
		m_pBress->SetAlpha(alpha);

		// 移動
		MyLib::Vector3 pos = UtilFunc::Correction::EaseOutExpo(BasePoint::BRESS, DestPoint::BRESS, 0.0f, StateTime::BRESS, m_fStateTime);
		m_pBress->SetPosition(pos);

		// サイズ
		float sizeX = m_pBress->GetSizeOrigin().x;
		float sizeY = UtilFunc::Correction::EaseOutExpo(0.0f, m_pBress->GetSizeOrigin().y, 0.0f, StateTime::BRESS, m_fStateTime);
		m_pBress->SetSize(D3DXVECTOR2(sizeX, sizeY));

		D3DXVECTOR2* pTex = m_pBress->GetTex();
		pTex[2].y = pTex[3].y = (sizeY / m_pBress->GetSizeOrigin().y);
	}

	//=============================
	// LOVE
	//=============================
	{
		// 移動
		for (int i = 0; i < LOGO_LOVE::MAX; i++)
		{
			// 移動
			MyLib::Vector3 pos = UtilFunc::Correction::EaseOutExpo(
				m_pLOVE[i]->GetOriginPosition(),
				m_pLOVE[i]->GetOriginPosition() + DestPoint::LOVE[i],
				0.0f, StateTime::BRESS, m_fStateTime);
			m_pLOVE[i]->SetPosition(pos);

			// 向き
			MyLib::Vector3 rot = UtilFunc::Correction::EaseOutExpo(
				0.0f,
				DestRotation::LOVE[i],
				0.0f, StateTime::BRESS, m_fStateTime);
			m_pLOVE[i]->SetRotation(rot);
		}
	}


	if (m_fStateTime >= StateTime::BRESS)
	{// 時間経過
		SetState(State::STATE_WAIT_AFTER);
	}
}

//==========================================================================
// 後の待機
//==========================================================================
void CTitleLogo::StateWait_After()
{
	if (m_fStateTime >= StateTime::WAIT)
	{// 時間経過
		SetState(State::STATE_AFTERMOVEMENT);
	}
}

//==========================================================================
// 後の動き
//==========================================================================
void CTitleLogo::StateAfterMovement()
{
	//=============================
	// 水
	//=============================
	{
		// スクロール
		D3DXVECTOR2* pTex = m_pWater->GetTex();
		pTex[0].x -= 0.001f;
		pTex[1].x = pTex[0].x + 1.0f;
		pTex[2].x -= 0.001f;
		pTex[3].x = pTex[2].x + 1.0f;
	}

	//=============================
	// 息
	//=============================
	{
		// 移動
		MyLib::Vector3 pos = m_pBress->GetPosition();
		pos = DestPoint::BRESS + sinf(D3DX_PI * (m_fStateTime / StateTime::AFTERMOVEMENT)) * MyLib::Vector3(-10.0f, -10.0f, 0.0f);
		m_pBress->SetPosition(pos);
	}

	//=============================
	// プレイヤー
	//=============================
	{
		// 移動
		MyLib::Vector3 pos = m_pPlayer->GetPosition();
		pos = DestPoint::PLAYER + sinf(D3DX_PI * (m_fStateTime / StateTime::AFTERMOVEMENT)) * MyLib::Vector3(-10.0f, -10.0f, 0.0f);
		m_pPlayer->SetPosition(pos);
	}

	//=============================
	// LOVE
	//=============================
	{
		for (int i = 0; i < LOGO_LOVE::MAX; i++)
		{
			// 向き
			float ratio = D3DX_PI * (m_fStateTime / StateTime::AFTERMOVEMENT);
			MyLib::Vector3 rot = DestRotation::LOVE[i] + sinf(ratio) * (DestRotation::LOVE[i] * 0.1f);
			m_pLOVE[i]->SetRotation(rot);

			// 移動
			MyLib::Vector3 pos, posOrigin = m_pLOVE[i]->GetPosition();
			pos.x = sinf(ratio) * (sinf(rot.z) * m_fDistance_LOVE[i]);
			pos.y = sinf(ratio) * (cosf(rot.z) * m_fDistance_LOVE[i]);
			pos += m_pLOVE[i]->GetOriginPosition() + DestPoint::LOVE[i];
			m_pLOVE[i]->SetPosition(pos);
		}
	}

	if (m_fStateTime >= StateTime::AFTERMOVEMENT * 2.0f)
	{
		// LOVEの間隔ランダム選定
		DrawingLOVEDistance();

		// タイマーリセット
		m_fStateTime = 0.0f;
	}
}

//==========================================================================
// LOVEの間隔ランダム選定
//==========================================================================
void CTitleLogo::DrawingLOVEDistance()
{
	for (int i = 0; i < LOGO_LOVE::MAX; i++)
	{
		m_fDistance_LOVE[i] = 8.0f + UtilFunc::Transformation::Random(-30, 30) * 0.1f;
		m_fDistance_LOVE[i] *= -1;
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CTitleLogo::Draw()
{
	
}

//==========================================================================
// 状態設定
//==========================================================================
void CTitleLogo::SetState(State state)
{ 
	m_state = state; 
	m_fStateTime = 0.0f;
}

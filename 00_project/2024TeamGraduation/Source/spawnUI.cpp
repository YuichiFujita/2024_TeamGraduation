//============================================================
//
//	登場演出チーム名表示UI処理 [spawnUI.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "spawnUI.h"
#include "manager.h"
#include "object2D_Anim.h"
#include "scrollString2D.h"
#include "loadtext.h"
#include "entryRuleManager.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 6;	// UIの優先順位
	namespace gameset
	{
		const char* TEXTURE = "data\\TEXTURE\\teamNameGB000.png";	// 背景テクスチャ
		const MyLib::PosGrid2 PTRN = MyLib::PosGrid2(1, 2);			// テクスチャ分割数
		const float WIDTH = 300.0f;		// 横幅
		const float MOVE_TIME = 0.25f;	// 移動時間
		const float TRANS_TIME = 0.25f;	// 遷移待機時間

		namespace l
		{
			const MyLib::Vector3 INIT_POS = MyLib::Vector3(-WIDTH, 630.0f, 0.0f);				// 初期位置
			const MyLib::Vector3 DEST_POS = MyLib::Vector3(WIDTH - 10.0f, INIT_POS.y, 0.0f);	// 目標位置
		}
		namespace r
		{
			const MyLib::Vector3 INIT_POS = MyLib::Vector3(VEC3_SCREEN_SIZE.x + WIDTH, 630.0f, 0.0f);				// 初期位置
			const MyLib::Vector3 DEST_POS = MyLib::Vector3(VEC3_SCREEN_SIZE.x - WIDTH + 10.0f, INIT_POS.y, 0.0f);	// 目標位置
		}

		const MyLib::Vector3 INIT_POS[] = { l::INIT_POS, r::INIT_POS };	// 初期位置
		const MyLib::Vector3 DEST_POS[] = { l::DEST_POS, r::DEST_POS };	// 目標位置
	}

	namespace str
	{
		const EAlignX ALIGN_X[] = { XALIGN_LEFT, XALIGN_RIGHT };	// 横配置
		const char*	FONT	= "data\\FONT\\Kazesawa Bold.ttf";		// フォントパス
		const bool	ITALIC	= false;	// イタリック
		const float	HEIGHT	= 40.0f;	// 文字縦幅
		const float	OFFSET_X[] = { -gameset::WIDTH + 30.0f, gameset::WIDTH - 30.0f };	// 文字縦幅

		namespace two
		{
			const float	OFFSET_Y = -30.0f;	// オフセット
			const float	WAIT_TIIME = 0.02f;	// 文字送りの待機時間
			const float	TRANS_TIME = 0.45f;	// 遷移待機時間
		}
		namespace name
		{
			const float	OFFSET_Y = 30.0f;		// オフセット
			const float	WAIT_TIIME = 0.035f;	// 文字送りの待機時間
			const float	TRANS_TIME = 1.5f;		// 遷移待機時間
		}
	}
}

//************************************************************
//	関数ポインタ
//************************************************************
// 状態関数
CSpawnUI::STATE_FUNC CSpawnUI::m_StateFuncList[] =
{
	&CSpawnUI::UpdateNone,		// 何もしないの更新
	&CSpawnUI::UpdateSpawn,		// 生成の更新
	&CSpawnUI::UpdateHutatuna,	// 二つ名の更新
	&CSpawnUI::UpdateTeamname,	// チーム名の更新
	&CSpawnUI::UpdateDespawn,	// 消失の更新
};

//************************************************************
//	子クラス [CSpawnUI] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSpawnUI::CSpawnUI(const CGameManager::ETeamSide team) : CObject(PRIORITY, LAYER::LAYER_2D),
	m_pBG		(nullptr),		// 背景情報
	m_pHutatuna	(nullptr),		// 二つ名情報
	m_pTeamName	(nullptr),		// チーム名情報
	m_team		(team),			// チーム
	m_state		(STATE_SPAWN),	// 状態
	m_fCurTime	(0.0f)			// 経過時間
{
	// スタティックアサート
	static_assert(NUM_ARRAY(m_StateFuncList) == CSpawnUI::STATE_MAX, "ERROR : State Count Mismatch");
}

//============================================================
//	デストラクタ
//============================================================
CSpawnUI::~CSpawnUI()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSpawnUI::Init()
{
	// UIの生成
	if (FAILED(CreateUI()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 種類をオブジェクト2Dにする
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CSpawnUI::Uninit()
{
	// 既に破棄済みの場合抜ける
	if (IsDeath()) { return; }

	// 背景の終了
	SAFE_UNINIT(m_pBG);

	// オブジェクトを破棄
	Release();
}

//============================================================
//	削除処理
//============================================================
void CSpawnUI::Kill()
{
	// 自身の終了
	CSpawnUI::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CSpawnUI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_StateFuncList[m_state] != nullptr)
	{ // 状態更新関数がある場合

		// 状態別処理
		(this->*(m_StateFuncList[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//============================================================
//	描画処理
//============================================================
void CSpawnUI::Draw()
{

}

//============================================================
//	位置設定の設定処理
//============================================================
void CSpawnUI::SetPosition(const MyLib::Vector3& rPos)
{
	// 自身の位置を設定
	CObject::SetPosition(rPos);

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	描画状況の設定処理
//============================================================
void CSpawnUI::SetEnableDisp(const bool bDisp)
{
	// 自身の描画状況を設定
	CObject::SetEnableDisp(bDisp);

	// 背景の描画状況を設定
	m_pBG->SetEnableDisp(bDisp);

	// 二つ名の描画状況を設定
	m_pHutatuna->SetEnableDisp(bDisp);

	// チーム名の描画状況を設定
	m_pTeamName->SetEnableDisp(bDisp);
}

//============================================================
//	生成処理
//============================================================
CSpawnUI *CSpawnUI::Create(const CGameManager::ETeamSide team)
{
	// インスタンスの生成
	CSpawnUI* pTrans = DEBUG_NEW CSpawnUI(team);
	if (pTrans != nullptr)
	{ // 生成に成功した場合

		// クラスの初期化
		if (FAILED(pTrans->Init()))
		{ // 初期化に失敗した場合

			// クラスの終了
			SAFE_UNINIT(pTrans);
			return nullptr;
		}
	}

	// インスタンスを返す
	return pTrans;
}

//============================================================
//	何もしないの更新処理
//============================================================
void CSpawnUI::UpdateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//============================================================
//	生成の更新処理
//============================================================
void CSpawnUI::UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 経過時間を加算
	m_fCurTime += fDeltaTime * fSlowRate;

	// 背景の位置を移動
	MyLib::Vector3 posBG = UtilFunc::Correction::EasingCubicOut(gameset::INIT_POS[m_team], gameset::DEST_POS[m_team], 0.0f, gameset::MOVE_TIME, m_fCurTime);
	SetPosition(posBG);

	if (m_fCurTime >= gameset::MOVE_TIME + gameset::TRANS_TIME)
	{ // 経過しきった場合

		// 経過時間を初期化
		m_fCurTime = 0.0f;

		// 二つ名の文字送り開始
		m_pHutatuna->SetEnableScroll(true);

		// 二つ名状態にする
		m_state = STATE_HUTATUNA;
	}
}

//============================================================
//	二つ名の更新処理
//============================================================
void CSpawnUI::UpdateHutatuna(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (!m_pHutatuna->IsScroll())
	{ // 文字送りが終了した場合

		// 経過時間を加算
		m_fCurTime += fDeltaTime * fSlowRate;
		if (m_fCurTime >= str::two::TRANS_TIME)
		{ // 経過しきった場合

			// 経過時間を初期化
			m_fCurTime = 0.0f;

			// チーム名の文字送り開始
			m_pTeamName->SetEnableScroll(true);

			// チーム名状態にする
			m_state = STATE_TEAMNAME;
		}
	}
}

//============================================================
//	チーム名の更新処理
//============================================================
void CSpawnUI::UpdateTeamname(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (!m_pTeamName->IsScroll())
	{ // 文字送りが終了した場合

		// 経過時間を加算
		m_fCurTime += fDeltaTime * fSlowRate;
		if (m_fCurTime >= str::name::TRANS_TIME)
		{ // 経過しきった場合

			// 経過時間を初期化
			m_fCurTime = 0.0f;

			// 消失状態にする
			m_state = STATE_DESPAWN;
		}
	}
}

//============================================================
//	消失の更新処理
//============================================================
void CSpawnUI::UpdateDespawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 経過時間を加算
	m_fCurTime += fDeltaTime * fSlowRate;

	// 背景の位置を移動
	MyLib::Vector3 posBG = UtilFunc::Correction::EasingCubicIn(gameset::DEST_POS[m_team], gameset::INIT_POS[m_team], 0.0f, gameset::MOVE_TIME, m_fCurTime);
	SetPosition(posBG);

	if (m_fCurTime >= gameset::MOVE_TIME)
	{ // 経過しきった場合

		// 経過時間を初期化
		m_fCurTime = 0.0f;

		// 自身の終了
		Uninit();
	}
}

//============================================================
//	相対位置の設定処理
//============================================================
void CSpawnUI::SetPositionRelative()
{
	// 自身の位置を取得
	MyLib::Vector3 posThis = GetPosition();

	// 背景の位置設定
	m_pBG->SetPosition(posThis);

	// 二つ名の位置設定
	m_pHutatuna->SetPosition(posThis + MyLib::Vector3(str::OFFSET_X[m_team], str::two::OFFSET_Y, 0.0f));

	// チーム名の位置設定
	m_pTeamName->SetPosition(posThis + MyLib::Vector3(str::OFFSET_X[m_team], str::name::OFFSET_Y, 0.0f));
}

//============================================================
//	UIの生成処理
//============================================================
HRESULT CSpawnUI::CreateUI()
{
	// 背景の生成
	m_pBG = CObject2D_Anim::Create
	( // 引数
		VEC3_ZERO,	// 位置
		gameset::PTRN.x,	// テクスチャ横分割数
		gameset::PTRN.y,	// テクスチャ縦分割数
		0.0f,		// 再生時間
		false,		// 自動破棄
		PRIORITY	// 優先順位
	);
	if (m_pBG == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 自動再生をOFFにする
	m_pBG->SetEnableAutoPlay(false);

	// テクスチャパターンの初期化
	m_pBG->SetPatternAnim((int)m_team);

	// テクスチャの割当
	CTexture* pTexture = CTexture::GetInstance();	// テクスチャ情報
	int texID = pTexture->Regist(gameset::TEXTURE);
	m_pBG->BindTexture(texID);

	// 横幅を元に大きさ設定
	MyLib::Vector2 size = pTexture->GetImageSize(texID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, gameset::WIDTH);
	size.y /= (float)gameset::PTRN.y;
	m_pBG->SetSize(size);

	// 二つ名の生成
	m_pHutatuna = CScrollString2D::Create
	( // 引数
		str::FONT,				// フォントパス
		str::ITALIC,			// イタリック
		L"",					// 指定文字列
		VEC3_ZERO,				// 原点位置
		str::two::WAIT_TIIME,	// 文字表示の待機時間
		str::HEIGHT,			// 文字縦幅
		str::ALIGN_X[m_team],	// 横配置
		VEC3_ZERO,				// 原点向き
		MyLib::color::White(),	// 色
		PRIORITY				// 優先順位
	);
	if (m_pHutatuna == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// ランダムに二つ名を割当
	loadtext::BindString(m_pHutatuna, loadtext::LoadText("data\\TEXT\\game\\hutatuna.txt", UtilFunc::Transformation::Random(0, 9)));

	// チーム名の取得
	std::string sTeamName;
	CEntryRuleManager::LoadName(&sTeamName, m_team);

	// チーム名の生成
	m_pTeamName = CScrollString2D::Create
	( // 引数
		str::FONT,				// フォントパス
		str::ITALIC,			// イタリック
		sTeamName,				// 指定文字列
		VEC3_ZERO,				// 原点位置
		str::name::WAIT_TIIME,	// 文字表示の待機時間
		str::HEIGHT,			// 文字縦幅
		str::ALIGN_X[m_team],	// 横配置
		VEC3_ZERO,				// 原点向き
		MyLib::color::Black(),	// 色
		PRIORITY				// 優先順位
	);
	if (m_pTeamName == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

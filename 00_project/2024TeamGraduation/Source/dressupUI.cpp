//============================================================
//
//	着せ替えUI処理 [dressupUI.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "dressupUI.h"
#include "dressup.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "game.h"
#include "renderTexture.h"
#include "entry_setupTeam.h"
#include "entry_dressup.h"
#include "object2D.h"
#include "object2D_Anim.h"
#include "ballFake.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 5;	// 着せ替えUIの優先順位
	const D3DXCOLOR COL_CHOICE	= MyLib::color::White();				// 選択中色
	const D3DXCOLOR COL_DEFAULT	= D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);	// 非選択中色
	const float MUL = 0.8f;

	namespace player
	{
		namespace frame
		{
			const MyLib::PosGrid2 PTRN = MyLib::PosGrid2(mylib_const::MAX_PLAYER + 1, 1);	// テクスチャ分割数
			const std::string TEXTURE = "data\\TEXTURE\\entry\\PlayerFrame000.png";			// テクスチャ
			const MyLib::Vector3 OFFSET = MyLib::Vector3(0.0f, -16.0f, 0.0f) * MUL;			// オフセット
			const float WIDTH = 118.0f * MUL;	// 横幅

			namespace bg
			{
				const std::string TEXTURE = "data\\TEXTURE\\entry\\PlayerFrame001.png";	// テクスチャ
				const float WIDTH = 118.0f * MUL;	// 横幅
			}
		}
		namespace ui
		{
			namespace body
			{
				D3DXVECTOR2 TEX_POS[] =		// テクスチャ座標
				{
					D3DXVECTOR2(0.1f, 0.0f),	// 左上
					D3DXVECTOR2(0.9f, 0.0f),	// 右上
					D3DXVECTOR2(0.1f, 0.8f),	// 左下
					D3DXVECTOR2(0.9f, 0.8f),	// 右下
				};
				const MyLib::Vector3 OFFSET = MyLib::Vector3(0.0f, -42.0f, 0.0f) * MUL;	// オフセット
				const float WIDTH = 290.0f * MUL;	// 横幅
			}
			namespace face
			{
				D3DXVECTOR2 TEX_POS[] =		// テクスチャ座標
				{
					D3DXVECTOR2(0.425f, 0.0f),	// 左上
					D3DXVECTOR2(0.575f, 0.0f),	// 右上
					D3DXVECTOR2(0.425f, 0.38f),	// 左下
					D3DXVECTOR2(0.575f, 0.38f),	// 右下
				};
				const MyLib::Vector3 OFFSET = MyLib::Vector3(0.0f, -32.0f, 0.0f) * MUL;	// オフセット
				const float WIDTH = 700.0f * MUL;	// 横幅
			}
		}
	}

	namespace change
	{
		const MyLib::PosGrid2	PTRN	= MyLib::PosGrid2(CDressupUI::EChangeType::TYPE_MAX, 1);	// テクスチャ分割数
		const std::string		TEXTURE	= "data\\TEXTURE\\entry\\ChangeType000.png";	// 変更種類アイコンテクスチャ
		const MyLib::Vector3	OFFSET	= MyLib::Vector3(0.0f, 175.0f, 0.0f) * MUL;		// オフセット
		const float WIDTH = 100.0f * MUL;	// 横幅
	}

	namespace ready
	{
		const MyLib::PosGrid2	PTRN	= MyLib::PosGrid2(1, 2);						// テクスチャ分割数
		const std::string		TEXTURE	= "data\\TEXTURE\\entry\\ReadyCheck000.png";	// 準備完了チェックテクスチャ
		const MyLib::Vector3	OFFSET	= MyLib::Vector3(0.0f, 222.0f, 0.0f) * MUL;		// オフセット
		const float WIDTH = 112.0f * MUL;	// 横幅
	}

	namespace arrow
	{
		const float				SIZE = 18.0f;	// 大きさ
		const MyLib::Vector3	SPACE = MyLib::Vector3(83.0f, 0.0f, 0.0f);	// 空白
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
int CDressupUI::m_nNumAI = 0;	// AI総数

//************************************************************
//	子クラス [CDressupUI] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CDressupUI::CDressupUI(CEntry_Dressup* pParent, const CPlayer::EFieldArea typeArea, const int nPlayerIdx, const CPlayerManager::LoadInfo& info) : CObject(PRIORITY, LAYER::LAYER_2D),
	m_pParent			(pParent),		// 親クラス情報
	m_pRenderScene		(nullptr),		// シーンレンダーテクスチャ
	m_pChangeIcon		(nullptr),		// 変更種類アイコン情報
	m_pReadyCheck		(nullptr),		// 準備完了チェック情報
	m_pPlayerFrameBG	(nullptr),		// プレイヤーフレーム背景情報
	m_pPlayerFrame		(nullptr),		// プレイヤーフレーム情報
	m_pPlayerUI			(nullptr),		// プレイヤーUI情報
	m_nOrdinalAI		(-1),			// 自身が生成された順番 (AIのみ)
	m_nPadIdx			(-1),			// 操作権インデックス
	m_bReady			(false),		// 準備完了フラグ
	m_pBall				(nullptr),		// ボール
	m_pPlayer			(nullptr),		// プレイヤー
	m_pHair				(nullptr),		// 髪着せ替え
	m_pAccessory		(nullptr),		// アクセ着せ替え
	m_pFace				(nullptr),		// 顔着せ替え
	m_typeEdit			(EEditType::EDIT_PROCESS),	// エディットする種類
	m_typeChange		(EChangeType::TYPE_BODY),	// 変更する種類
	m_nPlayerIdx		(nPlayerIdx),				// プレイヤーインデックス
	m_typeArea			(typeArea),					// プレイヤーポジション
	m_info				(info)						// 着せ替え情報
{
	for (int i = 0; i < CArrowUI::EDirection::DIRECTION_MAX; i++)
	{
		m_apArrow[i] = nullptr;	// 矢印の情報
	}
}

//============================================================
//	デストラクタ
//============================================================
CDressupUI::~CDressupUI()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CDressupUI::Init()
{
	// セットアップの生成
	if (FAILED(CreateSetup()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// レンダーテクスチャの生成
	if (FAILED(CreateRenderTexture()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

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
void CDressupUI::Uninit()
{
	// 既に破棄済みの場合抜ける
	if (IsDeath()) { return; }

	// チームセットアップ情報の取得
	assert(m_pParent != nullptr);
	CEntry_SetUpTeam* pSetupTeam = m_pParent->GetSetupTeam();
	if (pSetupTeam == nullptr)
	{ // チームセットアップ情報が破棄されている場合

		// AIの総数を初期化
		m_nNumAI = 0;
	}
	else
	{ // チームセットアップ情報が破棄されていない場合

		// 破棄したプレイヤーが内野且つAIの場合はAI総数を減算
		if (m_typeArea == CPlayer::EFieldArea::FIELD_IN
		&&  pSetupTeam->PlayerIdxToPadIdx(m_nPlayerIdx) <= -1) { m_nNumAI--; }
	}

	// レンダーテクスチャの破棄
	SAFE_REF_RELEASE(m_pRenderScene);

	// プレイヤーの終了
	SAFE_UNINIT(m_pPlayer);

	// 変更種類アイコンの終了
	SAFE_UNINIT(m_pChangeIcon);

	// 準備完了チェックの終了
	SAFE_UNINIT(m_pReadyCheck);

	// プレイヤーフレーム背景の終了
	SAFE_UNINIT(m_pPlayerFrameBG);

	// プレイヤーUIの終了
	SAFE_UNINIT(m_pPlayerUI);

	// プレイヤーフレームの終了
	SAFE_UNINIT(m_pPlayerFrame);

	// 髪着せ替えの終了
	SAFE_UNINIT(m_pHair);

	// アクセ切り替えの終了
	SAFE_UNINIT(m_pAccessory);

	// 顔切り替えの終了
	SAFE_UNINIT(m_pFace);

	// ボールの終了
	SAFE_UNINIT(m_pBall);

	for (int i = 0; i < CArrowUI::EDirection::DIRECTION_MAX; i++)
	{
		// 矢印の終了
		MyAssert::TrueAssert(i < 0 || i >= sizeof(m_apArrow), "配列オーバー");
		SAFE_UNINIT(m_apArrow[i]);
	}

	// オブジェクトを破棄
	Release();
}

//============================================================
//	削除処理
//============================================================
void CDressupUI::Kill()
{
	// プレイヤーの削除
	SAFE_KILL(m_pPlayer);

	// 自身の終了
	CDressupUI::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CDressupUI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 操作の更新
	UpdateControl(fDeltaTime, fDeltaRate, fSlowRate);

	// UIの更新
	UpdateUI();

	// 相対位置の設定
	SetPositionRelative();

	// 変更種類アイコンの更新
	m_pChangeIcon->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// 準備完了チェックの更新
	m_pReadyCheck->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// プレイヤーフレーム背景の更新
	m_pPlayerFrameBG->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// プレイヤーUIの更新
	m_pPlayerUI->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// プレイヤーフレームの更新
	m_pPlayerFrame->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// プレイヤーの更新
	m_pPlayer->Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//============================================================
//	描画処理
//============================================================
void CDressupUI::Draw()
{
	// 変更種類アイコンの描画
	m_pChangeIcon->Draw();

	// 準備完了チェックの描画
	m_pReadyCheck->Draw();

	// プレイヤーフレーム背景の描画
	m_pPlayerFrameBG->Draw();

	// プレイヤーUIの描画
	m_pPlayerUI->Draw();

	// プレイヤーフレームの描画
	m_pPlayerFrame->Draw();
}

//============================================================
//	種類の設定処理
//============================================================
void CDressupUI::SetType(const TYPE type)
{
	// 自身の種類を設定
	CObject::SetType(type);
}

//============================================================
//	描画状況の設定処理
//============================================================
void CDressupUI::SetEnableDisp(const bool bDraw)
{
	// 自身の描画状況を設定
	CObject::SetEnableDisp(bDraw);
}

//============================================================
//	位置の設定処理
//============================================================
void CDressupUI::SetPosition(const MyLib::Vector3& pos)
{
	// 設定する位置を保存
	CObject::SetPosition(pos);

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	生成処理
//============================================================
CDressupUI *CDressupUI::Create
(
	CEntry_Dressup* pParent,				// 親クラス情報
	const CPlayer::EFieldArea typeArea,		// プレイヤーポジション
	const int nPlayerIdx,					// プレイヤーインデックス
	const MyLib::Vector3& rPos,				// 原点位置
	const CPlayerManager::LoadInfo& info	// 着せ替え情報
)
{
	// 着せ替えUIの生成
	CDressupUI* pDressupUI = DEBUG_NEW CDressupUI(pParent, typeArea, nPlayerIdx, info);
	if (pDressupUI == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 着せ替えUIの初期化
		if (FAILED(pDressupUI->Init()))
		{ // 初期化に失敗した場合

			// 着せ替えUIの破棄
			SAFE_DELETE(pDressupUI);
			return nullptr;
		}

		// 原点位置を設定
		pDressupUI->SetPosition(rPos);

		// 確保したアドレスを返す
		return pDressupUI;
	}
}

//============================================================
// 準備完了フラグの設定処理
//============================================================
void CDressupUI::SetReady(const bool bReady)
{
	// 準備完了フラグの設定
	m_bReady = bReady;

	// テクスチャパターンの更新
	m_pReadyCheck->SetPatternAnim((int)m_bReady);
}

//============================================================
// 操作権インデックスの設定処理
//============================================================
void CDressupUI::SetPadIdx(const int nPadIdx)
{
	// 操作権インデックスの設定
	m_nPadIdx = nPadIdx;

	// 着せ替え情報の操作権インデックスの設定
	m_pHair->SetControllIdx(m_nPadIdx);
	m_pAccessory->SetControllIdx(m_nPadIdx);
	m_pFace->SetControllIdx(m_nPadIdx);
}

//============================================================
// プレイヤーインデックスの取得処理
//============================================================
int CDressupUI::GetMyPlayerIdx() const
{
	// プレイヤーインデックスを返す
	return m_pPlayer->GetMyPlayerIdx();
}

//============================================================
// チームサイドの取得処理
//============================================================
CGameManager::ETeamSide CDressupUI::GetTeam() const
{
	// チームサイドを返す
	return m_pPlayer->GetTeam();
}

//============================================================
// 髪のインデックス番号の取得処理
//============================================================
int CDressupUI::GetHairNowIdx() const
{
	// 髪のインデックス番号を返す
	return m_pHair->GetNowIdx();
}

//============================================================
// アクセのインデックス番号の取得処理
//============================================================
int CDressupUI::GetAccessoryNowIdx() const
{
	// アクセのインデックス番号を返す
	return m_pAccessory->GetNowIdx();
}

//============================================================
// 顔のインデックス番号の取得処理
//============================================================
int CDressupUI::GetFaceNowIdx() const
{
	// 顔のインデックス番号を返す
	return m_pFace->GetNowIdx();
}

//============================================================
// 体型の取得処理
//============================================================
CPlayer::EBody CDressupUI::GetBodyType() const
{
	// 体型を返す
	return m_pPlayer->GetBodyType();
}

//============================================================
// 利き手の取得処理
//============================================================
CPlayer::EHandedness CDressupUI::GetHandedness() const
{
	// 利き手を返す
	return m_pPlayer->GetHandedness();
}

//============================================================
// UIの生成処理
//============================================================
HRESULT CDressupUI::CreateUI()
{
	// 変更種類アイコンの生成
	if (FAILED(CreateChangeIcon()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 準備完了チェックの生成
	if (FAILED(CreateReadyCheck()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// プレイヤーフレーム背景の生成
	if (FAILED(CreatePlayerFrameBG()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// プレイヤーUIの生成
	if (FAILED(CreatePlayerUI()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// プレイヤーフレームの生成
	if (FAILED(CreatePlayerFrame()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 矢印UIの生成
	if (FAILED(CreateArrowUI()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 相対位置の設定
	SetPositionRelative();

	return S_OK;
}

//============================================================
// 変更種類アイコンの生成処理
//============================================================
HRESULT CDressupUI::CreateChangeIcon()
{
	// 変更種類アイコンの生成
	m_pChangeIcon = CObject2D_Anim::Create
	( // 引数
		VEC3_ZERO,		// 位置
		change::PTRN.x,	// テクスチャ横分割数
		change::PTRN.y,	// テクスチャ縦分割数
		0.0f,			// 再生時間
		false,			// 自動破棄
		PRIORITY		// 優先順位
	);
	if (m_pChangeIcon == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 種類を自動破棄/更新/描画しないものにする
	m_pChangeIcon->SetType(CObject::TYPE::TYPE_NONE);

	// 自動再生をOFFにする
	m_pChangeIcon->SetEnableAutoPlay(false);

	// テクスチャの割当
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = CTexture::GetInstance()->Regist(change::TEXTURE);
	m_pChangeIcon->BindTexture(nTexID);

	// 横幅を元にサイズを設定
	MyLib::Vector2 size = pTexture->GetImageSize(nTexID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, change::WIDTH);
	size.y *= (float)change::PTRN.x;
	m_pChangeIcon->SetSize(size);
	m_pChangeIcon->SetSizeOrigin(m_pChangeIcon->GetSize());

	return S_OK;
}

//============================================================
// 準備完了チェックの生成処理
//============================================================
HRESULT CDressupUI::CreateReadyCheck()
{
	// 準備完了チェックの生成
	m_pReadyCheck = CObject2D_Anim::Create
	( // 引数
		VEC3_ZERO,		// 位置
		ready::PTRN.x,	// テクスチャ横分割数
		ready::PTRN.y,	// テクスチャ縦分割数
		0.0f,			// 再生時間
		false,			// 自動破棄
		PRIORITY		// 優先順位
	);
	if (m_pReadyCheck == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 種類を自動破棄/更新/描画しないものにする
	m_pReadyCheck->SetType(CObject::TYPE::TYPE_NONE);

	// 自動再生をOFFにする
	m_pReadyCheck->SetEnableAutoPlay(false);

	// テクスチャパターンの初期化
	m_pReadyCheck->SetPatternAnim((int)m_bReady);

	// テクスチャの割当
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = CTexture::GetInstance()->Regist(ready::TEXTURE);
	m_pReadyCheck->BindTexture(nTexID);

	// 横幅を元にサイズを設定
	MyLib::Vector2 size = pTexture->GetImageSize(nTexID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, ready::WIDTH);
	size.y /= (float)ready::PTRN.y;
	m_pReadyCheck->SetSize(size);
	m_pReadyCheck->SetSizeOrigin(m_pReadyCheck->GetSize());

	return S_OK;
}

//============================================================
// プレイヤーフレームの生成処理
//============================================================
HRESULT CDressupUI::CreatePlayerFrame()
{
	// チームセットアップ情報の取得
	CEntry_SetUpTeam* pSetupTeam = CEntry::GetInstance()->GetSetupTeam();
	if (pSetupTeam == nullptr) { return E_FAIL; }

	// プレイヤーフレームの生成
	m_pPlayerFrame = CObject2D_Anim::Create
	( // 引数
		VEC3_ZERO,				// 位置
		player::frame::PTRN.x,	// テクスチャ横分割数
		player::frame::PTRN.y,	// テクスチャ縦分割数
		0.0f,					// 再生時間
		false,					// 自動破棄
		PRIORITY				// 優先順位
	);
	if (m_pPlayerFrame == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 種類を自動破棄/更新/描画しないものにする
	m_pPlayerFrame->SetType(CObject::TYPE::TYPE_NONE);

	// 自動再生をOFFにする
	m_pPlayerFrame->SetEnableAutoPlay(false);

	switch (m_typeArea)
	{ // ポジションごとの処理
	case CPlayer::FIELD_IN:
	{
		// テクスチャパターンの初期化
		m_pPlayerFrame->SetPatternAnim(pSetupTeam->PlayerIdxToPadIdx(m_nPlayerIdx));
		break;
	}
	case CPlayer::FIELD_OUT:
	{
		// テクスチャパターンの初期化
		m_pPlayerFrame->SetPatternAnim(-1);
		break;
	}
	default:
		assert(false);
		break;
	}

	// テクスチャの割当
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = CTexture::GetInstance()->Regist(player::frame::TEXTURE);
	m_pPlayerFrame->BindTexture(nTexID);

	// 横幅を元にサイズを設定
	MyLib::Vector2 size = pTexture->GetImageSize(nTexID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, player::frame::WIDTH);
	size.y *= (float)player::frame::PTRN.x;
	m_pPlayerFrame->SetSize(size);
	m_pPlayerFrame->SetSizeOrigin(m_pPlayerFrame->GetSize());

	return S_OK;
}

//============================================================
// プレイヤーフレーム背景の生成処理
//============================================================
HRESULT CDressupUI::CreatePlayerFrameBG()
{
	// チームセットアップ情報の取得
	CEntry_SetUpTeam* pSetupTeam = CEntry::GetInstance()->GetSetupTeam();
	if (pSetupTeam == nullptr) { return E_FAIL; }

	// プレイヤーフレーム背景の生成
	m_pPlayerFrameBG = CObject2D_Anim::Create
	( // 引数
		VEC3_ZERO,				// 位置
		player::frame::PTRN.x,	// テクスチャ横分割数
		player::frame::PTRN.y,	// テクスチャ縦分割数
		0.0f,					// 再生時間
		false,					// 自動破棄
		PRIORITY				// 優先順位
	);
	if (m_pPlayerFrameBG == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 種類を自動破棄/更新/描画しないものにする
	m_pPlayerFrameBG->SetType(CObject::TYPE::TYPE_NONE);

	// 自動再生をOFFにする
	m_pPlayerFrameBG->SetEnableAutoPlay(false);

	switch (m_typeArea)
	{ // ポジションごとの処理
	case CPlayer::FIELD_IN:
	{
		// テクスチャパターンの初期化
		m_pPlayerFrameBG->SetPatternAnim(pSetupTeam->PlayerIdxToPadIdx(m_nPlayerIdx));
		break;
	}
	case CPlayer::FIELD_OUT:
	{
		// テクスチャパターンの初期化
		m_pPlayerFrameBG->SetPatternAnim(-1);
		break;
	}
	default:
		assert(false);
		break;
	}

	// テクスチャの割当
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = CTexture::GetInstance()->Regist(player::frame::bg::TEXTURE);
	m_pPlayerFrameBG->BindTexture(nTexID);

	// 横幅を元にサイズを設定
	MyLib::Vector2 size = pTexture->GetImageSize(nTexID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, player::frame::bg::WIDTH);
	size.y *= (float)player::frame::PTRN.x;
	m_pPlayerFrameBG->SetSize(size);
	m_pPlayerFrameBG->SetSizeOrigin(m_pPlayerFrameBG->GetSize());

	return S_OK;
}

//============================================================
// プレイヤーUIの生成処理
//============================================================
HRESULT CDressupUI::CreatePlayerUI()
{
	// プレイヤーUIの生成
	m_pPlayerUI = CObject2D::Create(PRIORITY);
	if (m_pPlayerUI == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 種類を自動破棄/更新/描画しないものにする
	m_pPlayerUI->SetType(CObject::TYPE::TYPE_NONE);

	// テクスチャの割当
	m_pPlayerUI->BindTexture(m_pRenderScene->GetTextureIndex());

	// プレイヤーUIの更新
	UpdatePlayerUI();

	// 元の大きさを設定
	m_pPlayerUI->SetSizeOrigin(m_pPlayerUI->GetSize());

	return S_OK;
}

//============================================================
// 矢印UIの生成
//============================================================
HRESULT CDressupUI::CreateArrowUI()
{
	// 位置取得
	MyLib::Vector3 pos = GetPosition();

	for (int i = 0; i < CArrowUI::EDirection::DIRECTION_MAX; i++)
	{
		// 矢印の生成
		MyAssert::TrueAssert(i < 0 || i >= sizeof(m_apArrow), "配列オーバー");
		m_apArrow[i] = CArrowUI::Create
		( // 引数
			(CArrowUI::EDirection)i,	// 方向
			pos,						// 位置
			arrow::SIZE,				// サイズ
			MyLib::color::White(1.0f),	// 色
			GetPriority()				// 優先順位
		);

		if (m_apArrow[i] == nullptr)
		{ // 生成に失敗した場合
			return E_FAIL;
		}

		// オフセット位置設定
		m_apArrow[i]->SetOffset(arrow::SPACE);
		m_apArrow[i]->SetOffsetOrigin(m_apArrow[i]->GetOffset());
	}

	return S_OK;
}

//============================================================
// セットアップの生成処理
//============================================================
HRESULT CDressupUI::CreateSetup()
{
	// チームセットアップ情報の取得
	CEntry_SetUpTeam* pSetupTeam = CEntry::GetInstance()->GetSetupTeam();
	if (pSetupTeam == nullptr) { return E_FAIL; }

	CGameManager::ETeamSide side = pSetupTeam->GetTeamSide(m_nPlayerIdx);	// チーム	// TODO/FUJITA：こっちはパッドインデックスからチーム取ってる
	switch (m_typeArea)
	{ // ポジションごとの処理
	case CPlayer::FIELD_IN:
	{
		// エントリーインデックスを取得
		const int nPadIdx = pSetupTeam->PlayerIdxToPadIdx(m_nPlayerIdx);
		if (nPadIdx <= -1)
		{ // AIの場合

			// 自身のAI生成順を保存
			m_nOrdinalAI = m_nNumAI;

			// 準備完了済みにする
			m_bReady = true;

			// AI生成数を加算
			m_nNumAI++;
		}
		break;
	}
	case CPlayer::FIELD_OUT:
	{
		// 外野のチームを指定
		side = (CGameManager::ETeamSide)(m_nPlayerIdx / (CPlayerManager::OUT_MAX / 2));

		// 準備完了済みにする
		m_bReady = true;
		break;
	}
	default:
		assert(false);
		break;
	}

	// プレイヤー生成
	m_pPlayer = CPlayer::Create
	( // 引数
		VEC3_ZERO,	// 位置
		side,		// チームサイド
		CPlayer::EHuman::HUMAN_ENTRY,	// ポジション
		m_info.eBody,	// 体系
		m_info.eHanded	// 利き手
	);
	if (m_pPlayer == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 種類を自動破棄/更新/描画しないものにする
	m_pPlayer->SetType(CObject::TYPE::TYPE_NONE);

	// 元の位置設定
	m_pPlayer->CObject::SetOriginPosition(VEC3_ZERO);

	switch (m_typeArea)
	{ // ポジションごとの処理
	case CPlayer::FIELD_IN:
	{
		// インデックスの上書き
		m_pPlayer->SetMyPlayerIdx(pSetupTeam->PlayerIdxToPadIdx(m_nPlayerIdx));
		break;
	}
	case CPlayer::FIELD_OUT:
	{
		// インデックスの上書き
		m_pPlayer->SetMyPlayerIdx(-1);
		break;
	}
	default:
		assert(false);
		break;
	}

	// 髪着せ替え生成
	m_pHair = CDressup::Create
	( // 引数
		CDressup::EType::TYPE_HAIR,	// 着せ替えの種類
		m_pPlayer,					// 変更するプレイヤー
		CPlayer::ID_HAIR			// 変更箇所のインデックス
	);
	if (m_pHair == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}
	
	// アクセ切り替え生成
	m_pAccessory = CDressup::Create
	( // 引数
		CDressup::EType::TYPE_ACCESSORY,	// 着せ替えの種類
		m_pPlayer,							// 変更するプレイヤー
		CPlayer::ID_ACCESSORY				// 変更箇所のインデックス
	);
	if (m_pAccessory == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 顔切り替え生成
	m_pFace = CDressup::Create
	( // 引数
		CDressup::EType::TYPE_FACE,	// 着せ替えの種類
		m_pPlayer,					// 変更するプレイヤー
		CPlayer::ID_FACE			// 変更箇所のインデックス
	);
	if (m_pFace == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// インデックス割当
	m_pHair->SetNowIdx(m_info.nHair);
	m_pAccessory->SetNowIdx(m_info.nAccessory);
	m_pFace->SetNowIdx(m_info.nFace);

	// ドレスアップ反映
	m_pPlayer->BindDressUp(m_info.nHair, m_info.nAccessory, m_info.nFace);

	// ボールの生成
	m_pBall = CBallFake::Create();
	if (m_pBall == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 種類を自動破棄/更新/描画しないものにする
	m_pBall->SetType(CObject::TYPE::TYPE_NONE);

	// ボールを即キャッチ
	m_pBall->CatchLand(m_pPlayer);

	return S_OK;
}

//============================================================
// レンダーテクスチャ生成処理
//============================================================
HRESULT CDressupUI::CreateRenderTexture()
{
	// シーンレンダーテクスチャの生成
	m_pRenderScene = CRenderTexture::Create
	( // 引数
		CRenderTextureManager::LAYER_PLAYER,			// 描画順レイヤー
		std::bind(&CDressupUI::CreateTexture, this),	// テクスチャ作成関数ポインタ
		std::bind(&CCamera::SetCameraDressup, GET_MANAGER->GetCamera())	// カメラ設定関数ポインタ
	);
	if (m_pRenderScene == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//============================================================
// テクスチャ作成処理
//============================================================
void CDressupUI::CreateTexture()
{
	// プレイヤーの描画
	assert(m_pPlayer != nullptr);
	m_pPlayer->Draw();

	if (m_typeChange == TYPE_HANDEDNESS
	||  m_typeChange == TYPE_BODY)
	{ // 全身が見える場合

		// ボールの描画
		assert(m_pBall != nullptr);
		m_pBall->Draw();
	}
}

//============================================================
//	相対位置の設定処理
//============================================================
void CDressupUI::SetPositionRelative()
{
	// 自身の位置を取得
	MyLib::Vector3 posThis = GetPosition();

	// 変更種類アイコンの位置設定
	m_pChangeIcon->SetPosition(posThis + change::OFFSET);

	// 準備完了チェックの位置設定
	m_pReadyCheck->SetPosition(posThis + ready::OFFSET);

	// プレイヤーフレームの位置設定
	m_pPlayerFrame->SetPosition(posThis + player::frame::OFFSET);
	m_pPlayerFrameBG->SetPosition(posThis + player::frame::OFFSET);

	switch (m_typeChange)
	{ // 変更種類ごとの処理
	case EChangeType::TYPE_HAIR:
	case EChangeType::TYPE_ACCESSORY:
	case EChangeType::TYPE_FACE:
	{ // 顔周りの着せ替えの場合

		// プレイヤーUIの位置設定
		m_pPlayerUI->SetPosition(posThis + player::ui::face::OFFSET);
		break;
	}
	case EChangeType::TYPE_BODY:
	case EChangeType::TYPE_HANDEDNESS:
	{ // 体周りの着せ替えの場合

		// プレイヤーUIの位置設定
		m_pPlayerUI->SetPosition(posThis + player::ui::body::OFFSET);
		break;
	}
	default:
		assert(false);
		break;
	}

	switch (m_typeEdit)
	{ // エディット種類ごとの処理
	case EEditType::EDIT_PROCESS:
	{
		for (int i = 0; i < CArrowUI::EDirection::DIRECTION_MAX; i++)
		{
			MyAssert::TrueAssert(i < 0 || i >= sizeof(m_apArrow), "配列オーバー");
			MyAssert::TrueAssert(m_apArrow[i] == nullptr, "ヌルやん");
			// 矢印の生成
			m_apArrow[i]->SetPosition(m_pPlayerFrame->GetPosition());
			m_apArrow[i]->SetOriginPosition(m_pPlayerFrame->GetPosition());

			bool bDisp = (m_nPadIdx > -1);
			m_apArrow[i]->SetEnableDisp(bDisp);
		}
		break;
	}
	case EEditType::EDIT_CHANGETYPE:
	{
		for (int i = 0; i < CArrowUI::EDirection::DIRECTION_MAX; i++)
		{
			MyAssert::TrueAssert(i < 0 || i >= sizeof(m_apArrow), "配列オーバー");
			MyAssert::TrueAssert(m_apArrow[i] == nullptr, "ヌルやん");
			// 矢印の生成
			m_apArrow[i]->SetPosition(m_pChangeIcon->GetPosition());
			m_apArrow[i]->SetOriginPosition(m_pChangeIcon->GetPosition());

			bool bDisp = (m_nPadIdx > -1);
			m_apArrow[i]->SetEnableDisp(bDisp);
		}
		break;
	}
	default:
		assert(false);
		break;
	}
}

//============================================================
// 操作の更新処理
//============================================================
void CDressupUI::UpdateControl(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 着せ替え状態ではない場合抜ける
	if (m_pParent->GetState() != CEntry_Dressup::EState::STATE_DRESSUP) { return; }

	//--------------------------
	// デバッグ完了操作
	//--------------------------
#if _DEBUG
	if (CInputKeyboard::GetInstance()->GetTrigger(DIK_RETURN))
	{
		// 準備完了状態にする
		m_bReady = true;

		// エディット種類を実際の変更にする
		m_typeEdit = EEditType::EDIT_PROCESS;

		// 変更種類を体型の変更にする
		m_typeChange = EChangeType::TYPE_BODY;

		// プレイヤーUIの更新
		UpdatePlayerUI();

		// テクスチャパターンの更新
		m_pReadyCheck->SetPatternAnim((int)m_bReady);
	}
#endif // _DEBUG

	// 操作権インデックスが範囲外の場合抜ける
	if (m_nPadIdx <= -1 || m_nPadIdx >= mylib_const::MAX_PLAYER) { return; }

	CInputGamepad* pPad = CInputGamepad::GetInstance();	// パッド情報
	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, m_nPadIdx)
	||  pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_X, m_nPadIdx))
	{
		// 着せ替えシーンの取得
		CEntry* pEntry = CEntry::GetInstance();						// エントリーモード情報
		if (pEntry == nullptr) { assert(false); return; }			// エントリーモードがない場合抜ける
		CEntryScene* pEntryScene = pEntry->GetEntryScene();			// エントリーシーン情報
		if (pEntryScene == nullptr) { assert(false); return; }		// エントリーシーンがない場合抜ける
		CEntry_Dressup* pDressup = pEntryScene->GetDressupTeam();	// 着せ替えシーン情報
		if (pDressup == nullptr) { assert(false); return; }			// 着せ替えシーンがない場合抜ける

		// 準備完了状態にする
		SetReady(true);

		// エディット種類を実際の変更にする
		m_typeEdit = EEditType::EDIT_PROCESS;

		// 変更種類を体型の変更にする
		m_typeChange = EChangeType::TYPE_BODY;

		// プレイヤーUIの更新
		UpdatePlayerUI();

		// 選択UIを操作可能にする
		pDressup->SetSelectUISelect(m_nPadIdx, true);

		// 操作権の初期化
		SetPadIdx(-1);

		// サウンドの再生
		PLAY_SOUND(CSound::ELabel::LABEL_SE_DECIDE_01);
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_B, m_nPadIdx)
		 ||  pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_Y, m_nPadIdx))
	{
		// 準備未完了状態にする
		SetReady(false);
	}

	// 準備完了している場合抜ける
	if (m_bReady) { return; }

	// エディット種類の変更
	ChangeEditType(m_nPadIdx);

	switch (m_typeEdit)
	{ // エディット種類ごとの処理
	case EEditType::EDIT_PROCESS:

		switch (m_typeChange)
		{ // 変更種類ごとの処理
		case EChangeType::TYPE_HAIR:

			// 髪の更新
			m_pHair->Update(fDeltaTime, fDeltaRate, fSlowRate);
			break;

		case EChangeType::TYPE_ACCESSORY:

			// アクセの更新
			m_pAccessory->Update(fDeltaTime, fDeltaRate, fSlowRate);
			break;

		case EChangeType::TYPE_FACE:

			// 顔の更新
			m_pFace->Update(fDeltaTime, fDeltaRate, fSlowRate);
			break;

		case EChangeType::TYPE_BODY:

			// 体型の変更
			ChangeBodyType(m_nPadIdx);
			break;

		case EChangeType::TYPE_HANDEDNESS:

			// 利き手の変更
			ChangeHandedness(m_nPadIdx);
			break;

		default:
			assert(false);
			break;
		}
		break;

	case EEditType::EDIT_CHANGETYPE:

		// 変更する箇所の種類変更
		ChangeChangeType(m_nPadIdx);
		break;

	default:
		assert(false);
		break;
	}
}

//============================================================
// UIの更新処理
//============================================================
void CDressupUI::UpdateUI()
{
	// テクスチャ情報の取得
	CTexture* pTexture = CTexture::GetInstance();
	assert(pTexture != nullptr);

	// 変更種類をアイコンに反映
	m_pChangeIcon->SetPatternAnim(m_typeChange);

	if (m_nPadIdx <= -1)
	{ // 操作権がない場合

		// 変更種類アイコンの色を白にする
		m_pChangeIcon->SetColor(COL_CHOICE);

		// プレイヤーフレームの色を白にする
		m_pPlayerFrame->SetColor(COL_CHOICE);
		m_pPlayerFrameBG->SetColor(COL_CHOICE);

		// プレイヤーUIの色を白にする
		m_pPlayerUI->SetColor(COL_CHOICE);
	}
	else
	{ // 操作権がある場合

		switch (m_typeEdit)
		{ // エディット種類ごとの処理
		case EEditType::EDIT_PROCESS:
		{
			// 変更種類アイコンの色をグレーにする
			m_pChangeIcon->SetColor(COL_DEFAULT);

			// プレイヤーフレームの色を白にする
			m_pPlayerFrame->SetColor(COL_CHOICE);
			m_pPlayerFrameBG->SetColor(COL_CHOICE);

			// プレイヤーUIの色を白にする
			m_pPlayerUI->SetColor(COL_CHOICE);
			break;
		}
		case EEditType::EDIT_CHANGETYPE:
		{
			// 変更種類アイコンの色を白にする
			m_pChangeIcon->SetColor(COL_CHOICE);

			// プレイヤーフレームの色をグレーにする
			m_pPlayerFrame->SetColor(COL_DEFAULT);
			m_pPlayerFrameBG->SetColor(COL_DEFAULT);

			// プレイヤーUIの色をグレーにする
			m_pPlayerUI->SetColor(COL_DEFAULT);
			break;
		}
		default:
			assert(false);
			break;
		}
	}

	CInputGamepad* pPad = CInputGamepad::GetInstance();
	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, m_nPadIdx)
	||  pPad->GetLStickTrigger(m_nPadIdx, CInputGamepad::STICK_CROSS_AXIS::STICK_CROSS_RIGHT))
	{// ループ

		// 右移動
		SetAction(CArrowUI::EDirection::DIRECTION_R);

		// サウンドの再生
		PLAY_SOUND(CSound::ELabel::LABEL_SE_ARROW);
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, m_nPadIdx)
		 ||  pPad->GetLStickTrigger(m_nPadIdx, CInputGamepad::STICK_CROSS_AXIS::STICK_CROSS_LEFT))
	{// 逆ループ

		// 左移動
		SetAction(CArrowUI::EDirection::DIRECTION_L);

		// サウンドの再生
		PLAY_SOUND(CSound::ELabel::LABEL_SE_ARROW);
	}
}

//============================================================
// プレイヤーUIの更新処理
//============================================================
void CDressupUI::UpdatePlayerUI()
{
	CTexture* pTexture = CTexture::GetInstance();			// テクスチャ情報
	int nTexID = m_pPlayerUI->GetIdxTexture();				// テクスチャインデックス
	MyLib::Vector2 size = pTexture->GetImageSize(nTexID);	// テクスチャサイズ
	switch (m_typeChange)
	{ // 変更種類ごとの処理
	case EChangeType::TYPE_HAIR:
	case EChangeType::TYPE_ACCESSORY:
	case EChangeType::TYPE_FACE:
	{ // 顔周りの着せ替えの場合

		// テクスチャ座標の更新
		std::vector<D3DXVECTOR2> vecTex = m_pPlayerUI->GetVecTexUV();
		vecTex[0] = player::ui::face::TEX_POS[0];
		vecTex[1] = player::ui::face::TEX_POS[1];
		vecTex[2] = player::ui::face::TEX_POS[2];
		vecTex[3] = player::ui::face::TEX_POS[3];
		m_pPlayerUI->SetTexUV(vecTex);

		// テクスチャ大きさからの差分を計算
		D3DXVECTOR2 diff = D3DXVECTOR2(vecTex[1].x - vecTex[0].x, vecTex[2].y - vecTex[0].y);
		D3DXVECTOR2 rate = D3DXVECTOR2(diff.x / 1.0f, diff.y / 1.0f);

		// 横幅を元に大きさを設定
		size = UtilFunc::Transformation::AdjustSizeByWidth(size, player::ui::face::WIDTH);
		m_pPlayerUI->SetSize(D3DXVECTOR2(size.x * rate.x, size.y * rate.y));
		break;
	}
	case EChangeType::TYPE_BODY:
	case EChangeType::TYPE_HANDEDNESS:
	{ // 体周りの着せ替えの場合

		// テクスチャ座標の更新
		std::vector<D3DXVECTOR2> vecTex = m_pPlayerUI->GetVecTexUV();
		vecTex[0] = player::ui::body::TEX_POS[0];
		vecTex[1] = player::ui::body::TEX_POS[1];
		vecTex[2] = player::ui::body::TEX_POS[2];
		vecTex[3] = player::ui::body::TEX_POS[3];
		m_pPlayerUI->SetTexUV(vecTex);

		// 横幅を元に大きさを設定
		size = UtilFunc::Transformation::AdjustSizeByWidth(size, player::ui::body::WIDTH);
		m_pPlayerUI->SetSize(size);
		break;
	}
	default:
		assert(false);
		break;
	}
}

//============================================================
// アクション設定
//============================================================
void CDressupUI::SetAction(CArrowUI::EDirection dir)
{
	// 選択時移動
	MyAssert::TrueAssert(dir < 0 || dir >= sizeof(m_apArrow), "配列オーバー");
	MyAssert::TrueAssert(m_apArrow[dir] == nullptr, "ヌルやん");
	m_apArrow[dir]->SetState(CArrowUI::EState::STATE_SELECTMOVE);
}

//============================================================
// エディットする種類変更
//============================================================
void CDressupUI::ChangeEditType(int nPadIdx)
{
	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, nPadIdx)
	||  pPad->GetLStickTrigger(m_nPadIdx, CInputGamepad::STICK_CROSS_AXIS::STICK_CROSS_UP))
	{// 変更する箇所の種類変更へ変更

		EEditType old = m_typeEdit;
		m_typeEdit = EEditType::EDIT_PROCESS;

		if (m_typeEdit != old)
		{
			PLAY_SOUND(CSound::ELabel::LABEL_SE_CURSOR);
		}
		else
		{
			PLAY_SOUND(CSound::ELabel::LABEL_SE_BOUND_HIGH);
		}
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, nPadIdx)
		 ||  pPad->GetLStickTrigger(m_nPadIdx, CInputGamepad::STICK_CROSS_AXIS::STICK_CROSS_DOWN))
	{// 実際の変更画面へ

		EEditType old = m_typeEdit;
		m_typeEdit = EEditType::EDIT_CHANGETYPE;

		if (m_typeEdit != old)
		{
			PLAY_SOUND(CSound::ELabel::LABEL_SE_CURSOR);
		}
		else
		{
			PLAY_SOUND(CSound::ELabel::LABEL_SE_BOUND_HIGH);
		}
	}
}

//============================================================
// 変更する箇所の種類変更
//============================================================
void CDressupUI::ChangeChangeType(int nPadIdx)
{
	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, nPadIdx)
	||  pPad->GetLStickTrigger(m_nPadIdx, CInputGamepad::STICK_CROSS_AXIS::STICK_CROSS_RIGHT))
	{// ループ

		// 次へ変更
		int changeType = (m_typeChange + 1) % EChangeType::TYPE_MAX;
		m_typeChange = static_cast<EChangeType>(changeType);

		// プレイヤーUIの更新
		UpdatePlayerUI();
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, nPadIdx)
		 ||  pPad->GetLStickTrigger(m_nPadIdx, CInputGamepad::STICK_CROSS_AXIS::STICK_CROSS_LEFT))
	{// 逆ループ

		// 前へ変更
		int changeType = (m_typeChange + (EChangeType::TYPE_MAX - 1)) % EChangeType::TYPE_MAX;
		m_typeChange = static_cast<EChangeType>(changeType);

		// プレイヤーUIの更新
		UpdatePlayerUI();
	}
}

//============================================================
// 体型変更
//============================================================
void CDressupUI::ChangeBodyType(int nPadIdx)
{
	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// 体型取得
	CPlayer::EBody body = m_pPlayer->GetBodyType();
	CPlayer::EBody oldBody = body;

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, nPadIdx)
	||  pPad->GetLStickTrigger(m_nPadIdx, CInputGamepad::STICK_CROSS_AXIS::STICK_CROSS_RIGHT))
	{// ループ

		// 次へ変更
		int afterBody = (body + 1) % CPlayer::EBody::BODY_MAX;
		body = static_cast<CPlayer::EBody>(afterBody);
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, nPadIdx)
		 ||  pPad->GetLStickTrigger(m_nPadIdx, CInputGamepad::STICK_CROSS_AXIS::STICK_CROSS_LEFT))
	{// 逆ループ

		// 前へ変更
		int deforeBody = (body + (CPlayer::EBody::BODY_MAX - 1)) % CPlayer::EBody::BODY_MAX;
		body = static_cast<CPlayer::EBody>(deforeBody);
	}

	if (body != oldBody)
	{// 変更されていたら

		// プレイヤー再生成
		ReCreatePlayer(m_pPlayer->GetHandedness(), body);
	}
}

//============================================================
// 利き手変更
//============================================================
void CDressupUI::ChangeHandedness(int nPadIdx)
{
	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// 利き手取得
	CPlayer::EHandedness handedness = m_pPlayer->GetHandedness();
	CPlayer::EHandedness oldHandedness = handedness;

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, nPadIdx)
	||  pPad->GetLStickTrigger(m_nPadIdx, CInputGamepad::STICK_CROSS_AXIS::STICK_CROSS_RIGHT))
	{// ループ

		// 次へ変更
		int afterHandedness = (handedness + 1) % CPlayer::EHandedness::HAND_MAX;
		handedness = static_cast<CPlayer::EHandedness>(afterHandedness);
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, nPadIdx)
		 ||  pPad->GetLStickTrigger(m_nPadIdx, CInputGamepad::STICK_CROSS_AXIS::STICK_CROSS_LEFT))
	{// 逆ループ

		// 前へ変更
		int deforeHandedness = (handedness + (CPlayer::EHandedness::HAND_MAX - 1)) % CPlayer::EHandedness::HAND_MAX;
		handedness = static_cast<CPlayer::EHandedness>(deforeHandedness);
	}

	if (handedness != oldHandedness)
	{// 変更されていたら

		// プレイヤー再生成
		ReCreatePlayer(handedness, m_pPlayer->GetBodyType());
	}
}

//============================================================
// プレイヤー再生成
//============================================================
HRESULT CDressupUI::ReCreatePlayer(CPlayer::EHandedness handedness, CPlayer::EBody body)
{
	// チームセットアップ情報の取得
	CEntry_SetUpTeam* pSetupTeam = CEntry::GetInstance()->GetSetupTeam();
	if (pSetupTeam == nullptr) { return E_FAIL; }

	// 元のチームを保存
	CGameManager::ETeamSide side = m_pPlayer->GetTeam();

	// ボールを捨てる
	m_pBall->Drop(m_pPlayer);

	// プレイヤーの削除
	SAFE_KILL(m_pPlayer);

	// 再生成
	m_pPlayer = CPlayer::Create
	( // 引数
		VEC3_ZERO, 						// 位置
		side,							// チームサイド
		CPlayer::EHuman::HUMAN_ENTRY,	// ポジション
		body,							// 体系
		handedness						// 利き手
	);
	if (m_pPlayer == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 種類を自動破棄/更新/描画しないものにする
	m_pPlayer->SetType(CObject::TYPE::TYPE_NONE);

	switch (m_typeArea)
	{ // ポジションごとの処理
	case CPlayer::FIELD_IN:
	{
		// インデックスの上書き
		m_pPlayer->SetMyPlayerIdx(pSetupTeam->PlayerIdxToPadIdx(m_nPlayerIdx));
		break;
	}
	case CPlayer::FIELD_OUT:
	{
		// インデックスの上書き
		m_pPlayer->SetMyPlayerIdx(-1);
		break;
	}
	default:
		assert(false);
		break;
	}

	// ドレスアップ反映
	m_pPlayer->BindDressUp(m_info.nHair, m_info.nAccessory, m_info.nFace);

	// ドレスアップに再割当
	m_pHair->BindObjectCharacter(m_pPlayer);
	m_pAccessory->BindObjectCharacter(m_pPlayer);
	m_pFace->BindObjectCharacter(m_pPlayer);
	m_pHair->ReRegist();
	m_pAccessory->ReRegist();
	m_pFace->ReRegist();

	// ボールを即キャッチ
	m_pBall->CatchLand(m_pPlayer);

	return S_OK;
}

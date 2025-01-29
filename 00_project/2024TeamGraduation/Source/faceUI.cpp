//============================================================
//
//	プレイヤー顔UI処理 [faceUI.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "faceUI.h"
#include "camera.h"
#include "renderTexture.h"
#include "object2D.h"
#include "object2D_Anim.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 6;	// プレイヤー顔UIの優先順位

	namespace player
	{
		namespace idx
		{
			const std::string	  TEXTURE = "data\\TEXTURE\\playerIdx000.png";					// テクスチャ
			const MyLib::PosGrid2 PTRN	  = MyLib::PosGrid2(mylib_const::MAX_PLAYER + 1, 1);	// テクスチャ分割数
			const MyLib::Vector3  OFFSET  = MyLib::Vector3(33.5f, -52.0f, 0.0f);	// オフセット
			const float WIDTH = 25.0f;	// 横幅
		}
		namespace face
		{
			const MyLib::Vector3 OFFSET = MyLib::Vector3(30.0f, 0.0f, 0.0f);	// オフセット
			const float WIDTH = 140.0f;	// 横幅
		}
	}
}

//************************************************************
//	子クラス [CFaceUI] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CFaceUI::CFaceUI(const CPlayer* pPlayer) : CObject(PRIORITY, LAYER::LAYER_2D),
	m_pPlayerParent	(pPlayer),	// 親プレイヤー
	m_pRenderScene	(nullptr),	// シーンレンダーテクスチャ
	m_pPlayerIdxUI	(nullptr),	// プレイヤーインデックスUI情報
	m_pPlayerUI		(nullptr),	// プレイヤーUI情報
	m_pPlayer		(nullptr)	// プレイヤー
{

}

//============================================================
//	デストラクタ
//============================================================
CFaceUI::~CFaceUI()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CFaceUI::Init()
{
	// プレイヤーの生成
	if (FAILED(CreatePlayer()))
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
void CFaceUI::Uninit()
{
	// レンダーテクスチャの破棄
	SAFE_REF_RELEASE(m_pRenderScene);

	// プレイヤーインデックスUIの終了
	SAFE_UNINIT(m_pPlayerIdxUI);

	// プレイヤーUIの終了
	SAFE_UNINIT(m_pPlayerUI);

	// プレイヤーの終了
	SAFE_UNINIT(m_pPlayer);

	// オブジェクトを破棄
	Release();
}

//============================================================
//	削除処理
//============================================================
void CFaceUI::Kill()
{
	// プレイヤーの削除
	SAFE_KILL(m_pPlayer);

	// 自身の終了
	CFaceUI::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CFaceUI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 相対位置の設定
	SetPositionRelative();

	// プレイヤーUIの更新
	m_pPlayerUI->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// プレイヤーインデックスUIの更新
	m_pPlayerIdxUI->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// プレイヤーの更新
	m_pPlayer->Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//============================================================
//	描画処理
//============================================================
void CFaceUI::Draw()
{
	// プレイヤーUIの描画
	m_pPlayerUI->Draw();

	// プレイヤーインデックスUIの描画
	m_pPlayerIdxUI->Draw();
}

//============================================================
//	描画状況の設定処理
//============================================================
void CFaceUI::SetEnableDisp(const bool bDraw)
{
	// 自身の描画状況を設定
	CObject::SetEnableDisp(bDraw);

	// プレイヤーUIの描画状況を設定
	m_pPlayerUI->SetEnableDisp(bDraw);

	// プレイヤーインデックスUIの描画状況を設定
	m_pPlayerIdxUI->SetEnableDisp(bDraw);
}

//============================================================
//	位置の設定処理
//============================================================
void CFaceUI::SetPosition(const MyLib::Vector3& pos)
{
	// 設定する位置を保存
	CObject::SetPosition(pos);

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	着せ替え割当
//============================================================
void CFaceUI::BindDressUp(int nHair, int nAccessory, int nFace)
{
	// 着せ替え割当
	m_pPlayer->BindDressUp(nHair, nAccessory, nFace);
}

//============================================================
//	自分のインデックス設定
//============================================================
void CFaceUI::SetMyPlayerIdx(int nIdx)
{
	m_pPlayer->SetMyPlayerIdx(nIdx);
	m_pPlayerIdxUI->SetPatternAnim(nIdx);
}

//============================================================
//	生成処理
//============================================================
CFaceUI *CFaceUI::Create(const CPlayer* pPlayer, const MyLib::Vector3& rPos)
{
	// プレイヤー顔UIの生成
	CFaceUI* pFaceUI = DEBUG_NEW CFaceUI(pPlayer);
	if (pFaceUI == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// プレイヤー顔UIの初期化
		if (FAILED(pFaceUI->Init()))
		{ // 初期化に失敗した場合

			// プレイヤー顔UIの破棄
			SAFE_DELETE(pFaceUI);
			return nullptr;
		}

		// 位置を設定
		pFaceUI->SetPosition(rPos);

		// 確保したアドレスを返す
		return pFaceUI;
	}
}

//============================================================
// UIの生成処理
//============================================================
HRESULT CFaceUI::CreateUI()
{
	// プレイヤーUIの生成
	if (FAILED(CreatePlayerUI()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// プレイヤーインデックスUIの生成
	if (FAILED(CreatePlayerIdxUI()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 相対位置の設定
	SetPositionRelative();

	return S_OK;
}

//============================================================
// プレイヤーUIの生成処理
//============================================================
HRESULT CFaceUI::CreatePlayerUI()
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
	const int nTexID = m_pRenderScene->GetTextureIndex();
	m_pPlayerUI->BindTexture(nTexID);

	// 横幅を元にサイズを設定
	CTexture* pTexture = CTexture::GetInstance();
	MyLib::Vector2 size = pTexture->GetImageSize(nTexID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, player::face::WIDTH);
	m_pPlayerUI->SetSize(size);
	m_pPlayerUI->SetSizeOrigin(m_pPlayerUI->GetSize());

	return S_OK;
}

//============================================================
// プレイヤーインデックスUIの生成処理
//============================================================
HRESULT CFaceUI::CreatePlayerIdxUI()
{
	// プレイヤーインデックスUIの生成
	m_pPlayerIdxUI = CObject2D_Anim::Create
	( // 引数
		VEC3_ZERO,				// 位置
		player::idx::PTRN.x,	// テクスチャ横分割数
		player::idx::PTRN.y,	// テクスチャ縦分割数
		0.0f,		// 再生時間
		false,		// 自動破棄
		PRIORITY	// 優先順位
	);
	if (m_pPlayerIdxUI == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 種類を自動破棄/更新/描画しないものにする
	m_pPlayerIdxUI->SetType(CObject::TYPE::TYPE_NONE);

	// 自動再生をOFFにする
	m_pPlayerIdxUI->SetEnableAutoPlay(false);

	// テクスチャパターンの設定
	m_pPlayerIdxUI->SetPatternAnim(m_pPlayerParent->GetMyPlayerIdx());

	// テクスチャの割当
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = CTexture::GetInstance()->Regist(player::idx::TEXTURE);
	m_pPlayerIdxUI->BindTexture(nTexID);

	// 横幅を元にサイズを設定
	MyLib::Vector2 size = pTexture->GetImageSize(nTexID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, player::idx::WIDTH);
	size.y *= (float)player::idx::PTRN.x;
	m_pPlayerIdxUI->SetSize(size);
	m_pPlayerIdxUI->SetSizeOrigin(m_pPlayerIdxUI->GetSize());

	return S_OK;
}

//============================================================
// プレイヤーの生成処理
//============================================================
HRESULT CFaceUI::CreatePlayer()
{
	// プレイヤー生成
	m_pPlayer = CPlayer::Create
	( // 引数
		VEC3_ZERO,	// 位置
		m_pPlayerParent->GetTeam(),			// チームサイド
		CPlayer::EHuman::HUMAN_FACEICON,	// ポジション
		m_pPlayerParent->GetBodyType(),		// 体系
		m_pPlayerParent->GetHandedness()	// 利き手
	);
	if (m_pPlayer == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// 種類を自動破棄/更新/描画しないものにする
	m_pPlayer->SetType(CObject::TYPE::TYPE_NONE);

	// インデックスの上書き
	m_pPlayer->SetMyPlayerIdx(m_pPlayerParent->GetMyPlayerIdx());

	return S_OK;
}

//============================================================
// レンダーテクスチャ生成処理
//============================================================
HRESULT CFaceUI::CreateRenderTexture()
{
	// シーンレンダーテクスチャの生成
	m_pRenderScene = CRenderTexture::Create
	( // 引数
		CRenderTextureManager::LAYER_PLAYER,		// 描画順レイヤー
		std::bind(&CFaceUI::CreateTexture, this),	// テクスチャ作成関数ポインタ
		std::bind(&CCamera::SetCameraFace, GET_MANAGER->GetCamera())	// カメラ設定関数ポインタ
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
void CFaceUI::CreateTexture()
{
	// プレイヤーの描画
	assert(m_pPlayer != nullptr);
	m_pPlayer->Draw();
}

//============================================================
//	相対位置の設定処理
//============================================================
void CFaceUI::SetPositionRelative()
{
	// 自身の位置を取得
	MyLib::Vector3 posThis = GetPosition();

	// プレイヤーインデックスUIの位置設定
	m_pPlayerIdxUI->SetPosition(posThis + player::idx::OFFSET);

	// プレイヤーUIの位置設定
	m_pPlayerUI->SetPosition(posThis + player::face::OFFSET);
}

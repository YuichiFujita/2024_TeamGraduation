//==========================================================================
// 
//  プレイヤーマーカー処理 [playerMarker.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "playerMarker.h"
#include "gameManager.h"
#include "player.h"

// TODO：プレイヤーマーカー表示
#if 1
#define DISP
#endif

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\ballmarker\\circle.png";	// 円のテクスチャ
}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerMarker::CPlayerMarker(int nPriority) : CObjectBillboardAnim(nPriority),
	m_pPlayer	(nullptr)	// 親プレイヤー情報
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerMarker::~CPlayerMarker()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CPlayerMarker* CPlayerMarker::Create(CPlayer* pPlayer)
{
	// メモリの確保
	CPlayerMarker* pMarker = DEBUG_NEW CPlayerMarker;
	if (pMarker != nullptr)
	{
		// プレイヤー割当
		pMarker->m_pPlayer = pPlayer;

		// クラスの初期化
		if (FAILED(pMarker->Init()))
		{ // 初期化に失敗した場合

			SAFE_UNINIT(pMarker);
			return nullptr;
		}
	}

	return pMarker;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPlayerMarker::Init()
{
	// オブジェクトの種類設定
	CObject::SetType(CObject::TYPE::TYPE_OBJECTBILLBOARD);

	// 親クラスの初期化
	HRESULT hr = CObjectBillboardAnim::Init();
	if (FAILED(hr)) { return E_FAIL; }

	// テクスチャの割当
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(nTexID);

	// テクスチャの大きさを取得
	MyLib::Vector2 size = pTexture->GetImageSize(nTexID);

	// 横幅を元にサイズを設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 50.0f);
	SetSize(MyLib::Vector2(size.x, size.y));
	SetSizeOrigin(GetSize());

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerMarker::Uninit()
{
	// 親クラスの終了
	CObjectBillboardAnim::Uninit();
}

//==========================================================================
// 削除処理
//==========================================================================
void CPlayerMarker::Kill()
{
	// 自身の終了
	Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerMarker::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
#ifdef DISP
	m_pPlayer->GetMyPlayerIdx();

	// プレイヤーいないと描画切る
	bool bDisp = (m_pPlayer == nullptr) ? false : true;
	SetEnableDisp(bDisp);

	// 描画しない場合は抜ける
	if (!bDisp) { return; }

	// プレイヤーの頭上に位置を設定
	MyLib::Vector3 pos = m_pPlayer->GetPosition();
	pos.y += 160.0f;

	// 位置の反映
	SetPosition(pos);

	// 親クラスの更新
	CObjectBillboardAnim::Update(fDeltaTime, fDeltaRate, fSlowRate);
#else
	SetEnableDisp(false);
#endif
}

//==========================================================================
// 描画処理
//==========================================================================
void CPlayerMarker::Draw()
{
	// 親クラスの描画
	CObjectBillboardAnim::Draw();
}

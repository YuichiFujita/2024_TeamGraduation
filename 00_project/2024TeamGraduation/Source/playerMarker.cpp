//==========================================================================
// 
//  プレイヤーマーカー処理 [playerMarker.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "playerMarker.h"
#include "manager.h"
#include "renderer.h"
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
	const std::string TEXTURE = "data\\TEXTURE\\playerMarker000.png";	// プレイヤーマーカーテクスチャ
	const MyLib::PosGrid2 PTRN = MyLib::PosGrid2(4, 1);	// テクスチャ分割数
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
	HRESULT hr = CObjectBillboardAnim::Init(PTRN.x, PTRN.y, 0.0f, false);
	if (FAILED(hr)) { return E_FAIL; }

	// 自動再生をOFFにする
	SetEnableAutoPlay(false);

	// テクスチャの割当
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(nTexID);

	// テクスチャの大きさを取得
	MyLib::Vector2 size = pTexture->GetImageSize(nTexID);

	// 横幅を元にサイズを設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 160.0f);
	size.x /= (float)PTRN.x;
	SetSize(size);
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
	// 操作権インデックスの取得
	int nPadIdx = m_pPlayer->GetMyPlayerIdx();

	// 操作権がない場合は描画OFF
	bool bDisp = (nPadIdx <= -1) ? false : true;
	SetEnableDisp(bDisp);

	// 描画しない場合は抜ける
	if (!bDisp) { return; }

	// プレイヤーの頭上に位置を設定
	MyLib::Vector3 pos = m_pPlayer->GetPosition();
	pos.y += 200.0f;

	// 現在の操作権に合わせたパターンに変更
	SetPatternAnim(nPadIdx);

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
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイス情報

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 親クラスの描画
	CObjectBillboardAnim::Draw();

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// 
// キャッチ結果処理 [catchresult.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "catchresult.h"
#include "manager.h"
#include "sound.h"
#include "calculation.h"
#include "camera.h"
#include "game.h"
#include "player.h"

// 遷移先
#include "catchresult_OK.h"
#include "catchresult_FAIL.h"

//==========================================================================
// マクロ定義
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\battlewin\\goal.png";
	const float TIME_EXPANSION = 0.3f;			//拡大
	const float TIME_EXPNONE = 1.0f;			//拡大後何もしない
	const float TIME_FADEOUT = 0.4f;			// フェードアウト時間
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CCatchResult::STATE_FUNC CCatchResult::m_StateFuncList[] =
{
	&CCatchResult::StateExpansion,
	&CCatchResult::StateExpNone,
	&CCatchResult::StateFadeOut,
};

//==========================================================================
// コンストラクタ
//==========================================================================
CCatchResult::CCatchResult(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_state = STATE_EXPANSION;	// 状態
	m_fStateTimer = 0.0f;		// 状態タイマー
}

//==========================================================================
// デストラクタ
//==========================================================================
CCatchResult::~CCatchResult()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CCatchResult* CCatchResult::Create(const MyLib::Vector3& pos, TYPE resultType)
{
	// メモリの確保
	CCatchResult* pObj = nullptr;

	switch (resultType)
	{
	case CCatchResult::TYPE_OK:
		pObj = DEBUG_NEW CCatchResult_OK;
		break;

	case CCatchResult::TYPE_FAIL:
		pObj = DEBUG_NEW CCatchResult_FAIL;
		break;

	default:
		break;
	}

	if (pObj != nullptr)
	{// メモリの確保が出来ていたら

		// 初期化処理
		pObj->Init();
		pObj->SetPosition(pos);
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CCatchResult::Init()
{
	HRESULT hr;

	// 初期化処理
	hr = CObject2D::Init();
	if (FAILED(hr))
	{// 失敗したとき
		return E_FAIL;
	}

	// 種類の設定
	SetType(TYPE_OBJECT2D);

	// テクスチャの割り当て
	int nTexIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(nTexIdx);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(nTexIdx);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 400.0f);
	SetSize(size);
	SetSizeOrigin(size);

	// 開始
	m_fStateTimer = 0.0f;
	m_state = State::STATE_EXPANSION;

	// 向き設定
	SetRotation(MyLib::Vector3(0.0f, 0.0f, 0.0f));
	SetOriginRotation(GetRotation());

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CCatchResult::Uninit()
{
	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CCatchResult::Update()
{
	if (IsDeath())
	{
		return;
	}

	// 状態別処理
	(this->*(m_StateFuncList[m_state]))();

	if (IsDeath())
	{
		return;
	}

	// 頂点座標の設定
	SetVtx();
}

//==========================================================================
// 描画処理
//==========================================================================
void CCatchResult::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// 描画処理
	CObject2D::Draw();

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

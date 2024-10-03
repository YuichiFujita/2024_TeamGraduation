//=============================================================================
// 
// 当たり判定オブジェクト処理 [collisionobject.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "collisionobject.h"
#include "renderer.h"
#include "texture.h"
#include "manager.h"
#include "game.h"
#include "player.h"
#include "calculation.h"

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
int CCollisionObject::m_nIdxTex = 0;	// テクスチャのインデックス番号

//==========================================================================
// コンストラクタ
//==========================================================================
CCollisionObject::CCollisionObject(int nPriority) : CObjectBillboard(nPriority)
{
	// 値のクリア
	m_fRadius = 0.0f;	// 半径
	m_nDamage = 0;		// ダメージ
	m_nLife = 0;		// 寿命
	m_nMaxLife = 0;		// 最大寿命(固定)
	m_MyTag = TAG_NONE;			// 自分のタグ
}

//==========================================================================
// デストラクタ
//==========================================================================
CCollisionObject::~CCollisionObject()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CCollisionObject *CCollisionObject::Create()
{
	// 生成用のオブジェクト
	CCollisionObject *pEffect = nullptr;

	if (pEffect == nullptr)
	{// nullptrだったら

		// メモリの確保
		pEffect = DEBUG_NEW CCollisionObject;

		if (pEffect != nullptr)
		{// メモリの確保が出来ていたら

			// 初期化処理
			pEffect->Init();
		}

		return pEffect;
	}

	return nullptr;
}

//==========================================================================
// 生成処理
//==========================================================================
CCollisionObject *CCollisionObject::Create(const MyLib::Vector3 pos, const MyLib::Vector3 move, const float fRadius, const int nLife, const int nDamage, eMyTag tag)
{
	// 生成用のオブジェクト
	CCollisionObject *pEffect = nullptr;

	if (pEffect == nullptr)
	{// nullptrだったら

		// メモリの確保
		pEffect = DEBUG_NEW CCollisionObject;

		if (pEffect != nullptr)
		{// メモリの確保が出来ていたら

			pEffect->SetPosition(pos);
			pEffect->SetMove(move);
			pEffect->m_nDamage = nDamage;	// ダメージ
			pEffect->m_MyTag = tag;			// 種類
			pEffect->m_fRadius = fRadius;
			pEffect->SetSize(D3DXVECTOR2(fRadius, fRadius));
			pEffect->m_nLife = nLife;

			// 初期化処理
			HRESULT hr = pEffect->Init();
			if (FAILED(hr))
			{
				return nullptr;
			}
		}

		return pEffect;
	}

	return nullptr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CCollisionObject::Init()
{
	HRESULT hr;

	// 各種変数の初期化
	m_nMaxLife = m_nLife;						// 最大寿命(固定)

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECTX);

#if 0
	if (m_nIdxTex == 0)
	{
		m_nIdxTex = CTexture::GetInstance()->Regist("data\\TEXTURE\\effect\\effect000.jpg");
	}

	// テクスチャの割り当て
	BindTexture(m_nIdxTex);
#endif

	// 初期化処理
	hr = CObjectBillboard::Init();
	if (FAILED(hr))
	{// 失敗したとき
		Uninit();
		return E_FAIL;
	}

	SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CCollisionObject::Uninit()
{
	// 終了処理
	CObjectBillboard::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CCollisionObject::Update()
{

	// 位置取得
	MyLib::Vector3 pos = GetPosition();

	// 移動量取得
	MyLib::Vector3 move = GetMove();

	// 位置更新
	pos += move;

	// 位置設定
	SetPosition(pos);

	// 移動量設定
	SetMove(move);

	// 寿命の更新
	m_nLife--;

	// サイズ設定
	SetSize(D3DXVECTOR2(m_fRadius, m_fRadius));

	if (m_nLife <= 0)
	{// 寿命が尽きたら
		Uninit();
		return;
	}

	switch (m_MyTag)
	{
	case CCollisionObject::TAG_PLAYER:
		CollisionEnemy();
		break;

	case CCollisionObject::TAG_ENEMY:
		CollisionPlayer();
		break;
	}

	// 頂点座標の設定
	SetVtx();

}

//==========================================================================
// 敵との当たり判定
//==========================================================================
void CCollisionObject::CollisionEnemy()
{
	
	
}

//==========================================================================
// プレイヤーとの当たり判定
//==========================================================================
void CCollisionObject::CollisionPlayer()
{

	// 自分の情報取得
	MyLib::Vector3 pos = GetPosition();


	// プレイヤー取得
	CListManager<CPlayer> playerList = CPlayer::GetListObj();
	CPlayer* pPlayer = nullptr;

	// リストループ
	while (playerList.ListLoop(&pPlayer))
	{
		// プレイヤー情報取得
		MyLib::Vector3 PlayerPos = pPlayer->GetPosition();
		float PlayerRadius = pPlayer->GetRadius();
		CPlayer::STATE PlayerState = pPlayer->GetState();

		// 球の判定
		if (UtilFunc::Collision::SphereRange(pos, PlayerPos, m_fRadius, PlayerRadius).ishit)
		{
			// ヒット処理
			pPlayer->Hit(m_nDamage);
		}
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CCollisionObject::Draw()
{
	return;
#ifdef _DEBUG
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//常に描画する

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 描画処理
	CObjectBillboard::Draw();

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
#endif
}

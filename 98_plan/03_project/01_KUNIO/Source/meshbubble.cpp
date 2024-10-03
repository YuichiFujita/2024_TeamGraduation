//=============================================================================
// 
//  弾爆発処理 [meshbubble.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "meshbubble.h"
#include "manager.h"
#include "sound.h"
#include "calculation.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\FIELD\\bubble.png";
	const int BLOCK = 12;
	const float TIME_BURST = 0.3f;
	const float TIME_FADEOUT = 0.15f;
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CMeshBubble::STATE_FUNC CMeshBubble::m_StateFunc[] =
{
	&CMeshBubble::StateMove,	// なし
	&CMeshBubble::StateBurst,	// 掴み
	&CMeshBubble::StateFadeOut,	// 掴み
};

//==========================================================================
// コンストラクタ
//==========================================================================
CMeshBubble::CMeshBubble(int nPriority) : CMeshSphere(nPriority)
{
	// 値のクリア
	m_fOriginSize = 0.0f;		// 元のサイズ
	m_fDestSize = 0.0f;			// 目標のサイズ
	m_fRepeatCycle = 0.0f;		// 反復周期
	m_fStateTime = 0.0f;		// 状態タイマー
	m_state = STATE::STATE_MOVE;	// 状態
}

//==========================================================================
// デストラクタ
//==========================================================================
CMeshBubble::~CMeshBubble()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CMeshBubble* CMeshBubble::Create(const MyLib::Vector3& pos, const MyLib::Vector3& move, const float radius, const float destRadius, float repeatCycle)
{

	// メモリの確保
	CMeshBubble* pBullet = DEBUG_NEW CMeshBubble;

	if (pBullet != nullptr)
	{// メモリの確保が出来ていたら

		// 割り当て
		pBullet->SetPosition(pos);
		pBullet->SetMove(move);

		// サイズ設定
		pBullet->m_fOriginSize = radius;		// 元のサイズ
		pBullet->m_fDestSize = destRadius;		// 目標のサイズ
		pBullet->m_fRepeatCycle = repeatCycle;	// 反復周期
		pBullet->SetWidthLen(radius);
		pBullet->SetHeightLen(radius);
		pBullet->SetWidthBlock(BLOCK);
		pBullet->SetHeightBlock(BLOCK);

		// 初期化処理
		pBullet->Init();
	}

	return pBullet;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CMeshBubble::Init(void)
{
	// 各種変数の初期化

	float rg = UtilFunc::Transformation::Random(-10, 10) * 0.01f;
	float b = UtilFunc::Transformation::Random(-20, 0) * 0.01f;

	SetColor(D3DXCOLOR(
		1.0f,
		1.0f,
		1.0f, 0.8f));
	SetColorOrigin(GetColor());

	// テクスチャの割り当て
	int texIdx = CTexture::GetInstance()->Regist("");
	BindTexture(texIdx);

	// 初期化処理
	CMeshSphere::Init();

	// 種類の設定
	CObject::SetType(CObject::TYPE::TYPE_MESHSPHERE);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CMeshBubble::Uninit(void)
{
	// 終了処理
	CMeshSphere::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CMeshBubble::Update(void)
{
	// 弾け時間加算
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// 状態更新
	(this->*(m_StateFunc[m_state]))();

	if (IsDeath())
		return;
	
	// 頂点情報設定
	SetVtx();
}

//==========================================================================
// 移動処理
//==========================================================================
void CMeshBubble::StateMove()
{
	// 位置取得
	MyLib::Vector3 pos = GetPosition();
	bool bLand = false;

	// 高さ取得
	float height = CManager::GetInstance()->GetScene()->GetElevation()->GetHeight(pos, &bLand);

	// 移動量取得
	MyLib::Vector3 move = GetMove();

	// 位置更新
	pos.y += move.y;

	// 周期で位置更新
	pos.x += sinf(D3DX_PI * (m_fStateTime / m_fRepeatCycle)) * move.x;
	pos.z += cosf(D3DX_PI * (m_fStateTime / m_fRepeatCycle)) * move.x;

	// 高さ判定
	if (pos.y >= height)
	{
		m_state = STATE::STATE_BURST;
		m_fStateTime = 0.0f;
	}

	// 位置設定
	SetPosition(pos);



	float radius = GetWidthLen();

	radius += m_fDestSize * 0.001f;
	m_fOriginSize = radius;

	SetWidthLen(radius);
	SetHeightLen(radius);
}

//==========================================================================
// 弾け処理
//==========================================================================
void CMeshBubble::StateBurst()
{
	// 徐々に加速
	float radius = UtilFunc::Correction::EasingEaseIn(m_fOriginSize, m_fDestSize, 0.0f, TIME_BURST, m_fStateTime);

	SetWidthLen(radius);
	SetHeightLen(radius);

	if (m_fStateTime >= TIME_BURST)
	{
		for (int i = 0; i < 3; i++)
		{
			MyLib::Vector3 move;
			float fMove = 8.0f + UtilFunc::Transformation::Random(-50, 50) * 0.1f;
			float fMoveY = UtilFunc::Transformation::Random(80, 120) * 0.1f;
			float fRot = UtilFunc::Transformation::Random(-31, 31) * 0.1f;

			// 移動量の設定
			move.x = sinf(fRot) * fMove;
			move.y = fMoveY;
			move.z = cosf(fRot) * fMove;

			float radius = 5.0f + UtilFunc::Transformation::Random(-20, 20) * 0.1f;

			CEffect3D* pEffect = CEffect3D::Create(
				GetPosition(),
				move,
				D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f),
				radius, 30,
				CEffect3D::MOVEEFFECT::MOVEEFFECT_GENSUI,
				CEffect3D::TYPE::TYPE_NORMAL);
			pEffect->SetEnableGravity();
			pEffect->SetGravityValue(0.5f);
		}

		m_state = STATE::STATE_FADEOUT;
		m_fStateTime = 0.0f;
	}
}

//==========================================================================
// フェードアウト処理
//==========================================================================
void CMeshBubble::StateFadeOut()
{
	
	D3DXCOLOR col = GetColor();

	// 不透明度更新
	col.a = GetColorOrigin().a * (1.0f - (m_fStateTime / TIME_FADEOUT));
	SetColor(col);

	if (m_fStateTime >= TIME_FADEOUT)
	{

		Uninit();
	}

}

//==========================================================================
// 描画処理
//==========================================================================
void CMeshBubble::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 描画処理
	CMeshSphere::Draw();

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CMeshBubble::SetVtx(void)
{
	// 頂点設定
	CMeshSphere::SetVtx();
}

//=============================================================================
// 
//  瓦礫処理 [ballast.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "game.h"
#include "ballast.h"
#include "manager.h"
#include "renderer.h"
#include "calculation.h"
#include "objectX.h"
#include "elevation.h"

//==========================================================================
// マクロ定義
//==========================================================================
#define BAI		(0.4f)
#define LIFE	(40)	// 寿命
#define MOVE	(2.0f)	// 移動量
#define MOVE_Y	(30.0f * BAI)	// 移動量

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
const char *CBallast::m_pFileName[TYPE_MAX] = // ファイル名
{
	"data\\MODEL\\ballast_01.x",
	"data\\MODEL\\magic_ice.x"
};
int CBallast::m_nIdxXFile[TYPE_MAX] = {};			// Xファイルのインデックス番号

//==========================================================================
// コンストラクタ
//==========================================================================
CBallast::CBallast(int nPriority) : CObject(nPriority)
{
	m_posOrigin = MyLib::Vector3(0.0f, 0.0f, 0.0f);	// 原点
	m_moveOrigin = MyLib::Vector3(0.0f, 0.0f, 0.0f);	// 元の移動量
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色
	m_colOrigin = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 原色
	m_nCntParabola = 0;	// 瓦礫の移動カウント
	m_nNumAll = 0;		// 総数
	m_nLife = 0;		// 寿命
	m_nMaxLife = 0;		// 最大寿命
	m_fScale = 0.0f;
	m_type =TYPE_STONE;	// 種類
	memset(&m_pObjX[0], 0, sizeof(m_pObjX));	// オブジェクトXのポインタ
}

//==========================================================================
// デストラクタ
//==========================================================================
CBallast::~CBallast()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CBallast* CBallast::Create(MyLib::Vector3 pos, MyLib::Vector3 move, int nNum, float scale, float fAlpha, TYPE type)
{

	// メモリの確保
	CBallast* pBallast = DEBUG_NEW CBallast;

	if (pBallast != nullptr)
	{// メモリの確保が出来ていたら

		if (nNum >= MAX_BALLAST)
		{
			nNum = MAX_BALLAST;
		}

		// 総数設定
		pBallast->m_nNumAll = nNum;

		// 種類設定
		pBallast->m_type = type;

		// 位置設定
		pBallast->m_posOrigin = pos;

		// 移動量設定
		pBallast->m_moveOrigin = move;

		// 原色設定
		pBallast->m_colOrigin.a = fAlpha;

		pBallast->m_fScale = scale;

		// 初期化処理
		HRESULT hr = pBallast->Init();

		if (FAILED(hr))
		{
			return nullptr;
		}
	}

	return pBallast;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CBallast::Init()
{
	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECTX);

	// 寿命設定
	m_nLife = LIFE * m_nNumAll;
	m_nMaxLife = m_nLife;

	for (int nCntBallast = 0; nCntBallast < m_nNumAll; nCntBallast++)
	{
		// 生成処理
		m_pObjX[nCntBallast] = CObjectX::Create(
			m_pFileName[m_type],
			MyLib::Vector3(m_posOrigin.x + UtilFunc::Transformation::Random(-20, 20), m_posOrigin.y, m_posOrigin.z + UtilFunc::Transformation::Random(-20, 20)),
			MyLib::Vector3((float)UtilFunc::Transformation::Random(-314, 314) * 0.01f, (float)UtilFunc::Transformation::Random(-314, 314) * 0.01f, (float)UtilFunc::Transformation::Random(-314, 314) * 0.01f), false);

		if (m_pObjX[nCntBallast] == nullptr)
		{// 失敗したとき

			// オブジェクトXの終了処理
			delete m_pObjX[nCntBallast];
			m_pObjX[nCntBallast] = nullptr;
			return E_FAIL;
		}
		
		// 移動量設定
		m_pObjX[nCntBallast]->SetMove(MyLib::Vector3(
			(float)UtilFunc::Transformation::Random(-314, 314) * 0.01f * m_moveOrigin.x,
			m_moveOrigin.y + ((float)UtilFunc::Transformation::Random(-20, 20) * 0.1f),
			(float)UtilFunc::Transformation::Random(-314, 314) * 0.01f * m_moveOrigin.z));

		// スケール設定
		switch (m_type)
		{
		case TYPE_STONE:
		{
			float scale = (float)UtilFunc::Transformation::Random(1, 20) * 0.05f;
			m_pObjX[nCntBallast]->SetScale(scale * m_fScale);
		}
			break;

		case TYPE_ICE:
			m_pObjX[nCntBallast]->SetScale(MyLib::Vector3(
				(float)UtilFunc::Transformation::Random(-10, 10) * 0.1f,
				(float)UtilFunc::Transformation::Random(-10, 10) * 0.1f,
				(float)UtilFunc::Transformation::Random(-10, 10) * 0.1f) * m_fScale);
			break;
		}
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CBallast::Uninit()
{
	for (int nCntBallast = 0; nCntBallast < MAX_BALLAST; nCntBallast++)
	{
		if (m_pObjX[nCntBallast] != nullptr)
		{// nullptrじゃなかったら

			// オブジェクトXの終了処理
			m_pObjX[nCntBallast]->Uninit();
			m_pObjX[nCntBallast] = nullptr;
		}
	}

	// 情報削除
	Release();
}

//==========================================================================
// 更新処理
//==========================================================================
void CBallast::Update()
{
	// 瓦礫の移動カウント加算
	m_nCntParabola++;

	m_nLife--;

	for (int nCntBallast = 0; nCntBallast < m_nNumAll; nCntBallast++)
	{
		if (m_pObjX[nCntBallast] == nullptr)
		{// nullptrだったら

			// 寿命更新
			m_nLife = 0;
			continue;
		}

		// 位置取得
		MyLib::Vector3 pos = m_pObjX[nCntBallast]->GetPosition();

		// 移動量取得
		MyLib::Vector3 move = m_pObjX[nCntBallast]->GetMove();

		// 向き取得
		MyLib::Vector3 rot = m_pObjX[nCntBallast]->GetRotation();

		if (m_type == TYPE_ICE)
		{
			// 向き取得
			MyLib::Vector3 scale = m_pObjX[nCntBallast]->GetScale();

			scale *= 0.98f;

			// サイズ設定
			m_pObjX[nCntBallast]->SetScale(scale);

		}

		MyLib::Vector3 HitPos = pos;
		HitPos.y = pos.y;
		if (m_nMaxLife - 5 >= m_nLife &&
			(CGame::GetInstance()->GetElevation()->IsHit(HitPos) || GetPosition().y <= mylib_const::KILL_Y))
		{// 地面を下回ったら

			// 寿命更新
			m_nLife--;

			pos.y = HitPos.y;
		}
		else
		{
			// 位置更新
			pos = (MyLib::Vector3(0.0f, -0.5f * BAI, 0.0f) * (float)(m_nCntParabola * m_nCntParabola) + move * (float)m_nCntParabola) + m_posOrigin;

			// 向き更新
			rot += MyLib::Vector3(0.05f, 0.05f, -0.02f);
		}

		// 不透明度更新
		m_col.a = m_colOrigin.a * ((float)m_nLife / (float)m_nMaxLife);

		// 位置設定
		m_pObjX[nCntBallast]->SetPosition(pos);

		// 向き設定
		m_pObjX[nCntBallast]->SetRotation(rot);
	}

	if (m_nLife <= 0)
	{// 寿命が尽きたら

		// 終了処理
		Uninit();
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CBallast::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	for (int nCntBallast = 0; nCntBallast < m_nNumAll; nCntBallast++)
	{
		if (m_pObjX[nCntBallast] != nullptr)
		{
			// 描画処理
			m_pObjX[nCntBallast]->Draw(m_col.a);
		}
	}

}

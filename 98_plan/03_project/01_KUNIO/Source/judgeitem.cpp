//=============================================================================
// 
//  ジャッジアイテム処理 [judgeitem.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "judgeitem.h"
#include "judgeitemManager.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "spline.h"
#include "course.h"
#include "baggage.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* MODEL = "data\\MODEL\\coin\\coin.x";	// モデルパス
	const float TIME_ROTATION = 2.0f;	// 回転するための時間
}

namespace StateTime
{
	const float SWING = 2.0f;	// ゆらゆら
	const float ROTATE = 0.6f;	// 回転
}

//==========================================================================
// 静的メンバ変数
//==========================================================================
CListManager<CJudgeItem> CJudgeItem::m_List = {};	// リスト
std::map<int, CListManager<CJudgeItem>> CJudgeItem::m_ListBlock = {};	// ブロック単位リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CJudgeItem::CJudgeItem(int nPriority) : CObjectX(nPriority)
{
	// 値のクリア
	m_pMgr = nullptr;	// マネージャ
	m_fLength = 0.0f;
	m_nMyBlockIdx = 0;		// ブロックインデックス
}

//==========================================================================
// デストラクタ
//==========================================================================
CJudgeItem::~CJudgeItem()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CJudgeItem* CJudgeItem::Create(CJudgeItemManager* pMgr, int mapBlock, const float length, const float height)
{
	// メモリの確保
	CJudgeItem* pObj = DEBUG_NEW CJudgeItem;

	if (pObj != nullptr)
	{
		// 引数情報
		pObj->m_pMgr = pMgr;
		pObj->m_nMyBlockIdx = mapBlock;
		pObj->m_fHeight = height;
		pObj->SetLength(length);

		// 初期化処理
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CJudgeItem::Init()
{
	// リストに追加
	m_List.Regist(this);
	m_ListBlock[m_nMyBlockIdx].Regist(this);

	// 種類の設定
	CObject::SetType(TYPE_OBJECTX);

	// 初期化処理
	HRESULT hr = CObjectX::Init(MODEL);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CJudgeItem::Uninit()
{
	// リストから削除
	m_List.Delete(this);
	m_ListBlock[m_nMyBlockIdx].Delete(this);

	// 終了処理
	CObjectX::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CJudgeItem::Kill()
{
	// マネージャ内削除
	m_pMgr->DeleteJudgeItem(this);

	// リストから削除
	m_List.Delete(this);
	m_ListBlock[m_nMyBlockIdx].Delete(this);

	// 終了処理
	CObjectX::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CJudgeItem::Update()
{
	MyLib::Vector3 rot = GetRotation();

	// 回転
	rot.y += ((D3DX_PI * 2.0f) * (1.0f / TIME_ROTATION)) * CManager::GetInstance()->GetDeltaTime();
	UtilFunc::Transformation::RotNormalize(rot.y);

	SetRotation(rot);
}

//==========================================================================
// 取得
//==========================================================================
void CJudgeItem::Get()
{
	// 削除
	Kill();

	// サウンド生成
	CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_COIN);

	// エフェクト生成
	for (int i = 0; i < 8; i++)
	{
		// 移動量
		float randmoveZ = UtilFunc::Transformation::Random(-45, 45) * 1.0f;
		float randmoveX = UtilFunc::Transformation::Random(-45, 45) * 1.0f;
		float randmoveY = UtilFunc::Transformation::Random(200, 350) * 0.1f;
		float randRadius = UtilFunc::Transformation::Random(-30, 30) * 0.1f;

		MyLib::Vector3 move;
		move.x = randmoveX;
		move.z = randmoveZ;
		move.y = randmoveY;

		// 半径
		float radius = 120.0f + randRadius;

		// 生成位置のぶれ
		MyLib::Vector3 pos = GetPosition();
		pos.x += randmoveX * 2.0f;
		pos.z += randmoveZ * 2.0f;
		pos.y += randmoveY * 2.0f;

		CEffect3D* pEffect = CEffect3D::Create(
			pos,
			move,
			D3DXCOLOR(1.0f, 0.8f + UtilFunc::Transformation::Random(-100, 100) * 0.001f, 0.0f, 1.0f),
			radius,
			30,
			CEffect3D::MOVEEFFECT::MOVEEFFECT_GENSUI,
			CEffect3D::TYPE::TYPE_JUJI2);
		pEffect->SetEnableGravity();
		pEffect->SetGravityValue(0.8f);
	}
}

//==========================================================================
// 距離設定
//==========================================================================
void CJudgeItem::SetLength(const float length)
{
	// 距離を設定
	m_fLength = length;

	// 座標を設定
	MyLib::Vector3 pos = MySpline::GetSplinePosition_NonLoop(CGame::GetInstance()->GetCourse()->GetVecPosition(), m_fLength);
	pos.y += m_fHeight;
	SetPosition(pos);
}

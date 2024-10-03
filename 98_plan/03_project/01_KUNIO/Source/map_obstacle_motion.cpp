//=============================================================================
// 
//  マップのモーションする障害物 [map_obstacle_motion.cpp]
//  Author : 石原颯馬
// 
//=============================================================================
#include "map_obstacle_motion.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "collisionLine_Box.h"
#include "obstacle_fisharch.h"
#include "obstacle_birdcircle.h"
#include "model.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

}

//==========================================================================
// 静的メンバ変数
//==========================================================================
CListManager<CMap_Obstacle_Motion> CMap_Obstacle_Motion::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CMap_Obstacle_Motion::CMap_Obstacle_Motion(int nPriority,
	CObject::LAYER layer) : CMap_Obstacle(nPriority, layer)
{
	// 値のクリア
	m_pChara = nullptr;
}

//==========================================================================
// デストラクタ
//==========================================================================
CMap_Obstacle_Motion::~CMap_Obstacle_Motion()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CMap_Obstacle_Motion *CMap_Obstacle_Motion::Create(const CMap_ObstacleManager::SObstacleInfo& info, const bool bChange, const bool bSave)
{
	CMap_Obstacle_Motion* pObj = nullptr;
	// メモリの確保
	pObj = DEBUG_NEW CMap_Obstacle_Motion;

	if (pObj != nullptr)
	{
		pObj->SetObstacleInfo(info);
		pObj->SetSave(bSave);

		// 初期化処理
		//pObj->Init();
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CMap_Obstacle_Motion::Init()
{
	// リストに追加
	m_List.Regist(this);

	// 種類の設定
	CObject::SetType(TYPE_OBJECTX);

	m_OriginObstacleInfo = m_ObstacleInfo;	// 障害物情報

	// キャラ生成
	if (m_pChara == nullptr)
	{
		m_pChara = CObjectChara::Create(m_ObstacleInfo.modelFile);
		m_pChara->GetMotion()->Set(0);
		m_pChara->CObject::SetType(TYPE_OBJECTX);
		m_pChara->SetPosition(GetPosition());

		return CMap_Obstacle::Init();
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CMap_Obstacle_Motion::Uninit()
{
	if (m_pChara != nullptr)
	{
		m_pChara = nullptr;
	}

	// リストから削除
	m_List.Delete(this);

	// 終了処理
	CMap_Obstacle::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CMap_Obstacle_Motion::Kill()
{
	if (m_pChara != nullptr)
	{
		m_pChara->Uninit();
		m_pChara = nullptr;
	}

	// リストから削除
	m_List.Delete(this);

	// 終了処理
	CMap_Obstacle::Kill();
}

//==========================================================================
// 更新処理
//==========================================================================
void CMap_Obstacle_Motion::Update()
{
	CMap_Obstacle::Update();

	// モーション取得
	CMotion* pMotion = m_pChara->GetMotion();
	if (pMotion == nullptr) return;

	if (pMotion->IsFinish())
	{
		// 通常モーション再生
		pMotion->Set(MOTION::MOTION_DEF);
	}
}

//==========================================================================
// ヒット処理
//==========================================================================
void CMap_Obstacle_Motion::Hit()
{
	// モーション取得
	CMotion* pMotion = m_pChara->GetMotion();
	if (pMotion == nullptr) return;

	// ヒットモーション再生
	pMotion->Set(MOTION::MOTION_HIT);

	return;
}

//==========================================================================
// 描画処理
//==========================================================================
void CMap_Obstacle_Motion::Draw()
{
	CMap_Obstacle::Draw();
}

//==========================================================================
// 位置設定
//==========================================================================
void CMap_Obstacle_Motion::SetPosition(const MyLib::Vector3& pos)
{
	CObject::SetPosition(pos);

	if (m_pChara != nullptr)
	{
		m_pChara->SetPosition(pos);
	}
}

//==========================================================================
// 角度設定
//==========================================================================
void CMap_Obstacle_Motion::SetRotation(const MyLib::Vector3& rot)
{
	CObject::SetRotation(rot);

	if (m_pChara != nullptr)
	{
		m_pChara->SetRotation(rot);
	}
}

//==========================================================================
// サイズ設定
//==========================================================================
void CMap_Obstacle_Motion::SetScale(const MyLib::Vector3& scale)
{
	CModel* p = m_pChara->GetModel(0);
	p->SetOriginScale(scale);
}

//==========================================================================
// スケール取得
//==========================================================================
MyLib::Vector3 CMap_Obstacle_Motion::GetScale()
{ 
	return m_pChara->GetModel(0)->GetScale(); 
}

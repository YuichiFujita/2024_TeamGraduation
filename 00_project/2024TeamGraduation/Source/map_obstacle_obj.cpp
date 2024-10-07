//=============================================================================
// 
//  マップのオブジェクトの障害物 [map_obstacle_obj.cpp]
//  Author : 石原颯馬
// 
//=============================================================================
#include "map_obstacle_obj.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "collisionLine_Box.h"
#include "obstacle_fisharch.h"
#include "obstacle_birdcircle.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

}

//==========================================================================
// 静的メンバ変数
//==========================================================================
CListManager<CMap_Obstacle_Object> CMap_Obstacle_Object::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CMap_Obstacle_Object::CMap_Obstacle_Object(int nPriority,
	CObject::LAYER layer) : CMap_Obstacle(nPriority, layer)
{
	// 値のクリア
	m_pObject = nullptr;
}

//==========================================================================
// デストラクタ
//==========================================================================
CMap_Obstacle_Object::~CMap_Obstacle_Object()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CMap_Obstacle_Object *CMap_Obstacle_Object::Create(const CMap_ObstacleManager::SObstacleInfo& info, const bool bChange, const bool bSave)
{
	CMap_Obstacle_Object* pObj = nullptr;
	// メモリの確保
	pObj = DEBUG_NEW CMap_Obstacle_Object;

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
HRESULT CMap_Obstacle_Object::Init()
{
	// リストに追加
	m_List.Regist(this);

	// 種類の設定
	CObject::SetType(TYPE_OBJECTX);

	// オブジェクト生成
	m_pObject = CObjectX::Create(m_ObstacleInfo.modelFile);
	m_pObject->SetPosition(GetPosition());

	return CMap_Obstacle::Init();
}

//==========================================================================
// 終了処理
//==========================================================================
void CMap_Obstacle_Object::Uninit()
{
	// 生成オブジェクト破棄
	if (m_pObject != nullptr)
	{
		m_pObject = nullptr;
	}

	// リストから削除
	m_List.Delete(this);

	// 終了処理
	CMap_Obstacle::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CMap_Obstacle_Object::Kill()
{
	// 生成オブジェクト破棄
	if (m_pObject != nullptr)
	{
		m_pObject->Kill();
		m_pObject = nullptr;
	}

	// リストから削除
	m_List.Delete(this);

	// 終了処理
	CMap_Obstacle::Kill();
}

//==========================================================================
// 更新処理
//==========================================================================
void CMap_Obstacle_Object::Update()
{
	CMap_Obstacle::Update();
}

//==========================================================================
// 描画処理
//==========================================================================
void CMap_Obstacle_Object::Draw()
{
	CMap_Obstacle::Draw();
}

//==========================================================================
// 位置設定
//==========================================================================
void CMap_Obstacle_Object::SetPosition(const MyLib::Vector3& pos)
{
	CObject::SetPosition(pos);
	
	if (m_pObject != nullptr)
	{
		m_pObject->SetPosition(pos);
	}
}

//==========================================================================
// 角度設定
//==========================================================================
void CMap_Obstacle_Object::SetRotation(const MyLib::Vector3& rot)
{
	CObject::SetRotation(rot);

	if (m_pObject != nullptr)
	{
		m_pObject->SetRotation(rot);
	}
}

//==========================================================================
// サイズ設定
//==========================================================================
void CMap_Obstacle_Object::SetScale(const MyLib::Vector3& scale)
{
	m_pObject->SetScale(scale);
}
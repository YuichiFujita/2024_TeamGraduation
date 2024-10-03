//=============================================================================
// 
// 移動ハンドル処理 [handle_Move.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "handle_Move.h"
#include "manager.h"
#include "calculation.h"
#include "3D_Effect.h"
#include "camera.h"
#include "objectLine.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CHandle_Move::CHandle_Move(int nPriority) : CHandle(nPriority)
{
	// 値のクリア
}

//==========================================================================
// デストラクタ
//==========================================================================
CHandle_Move::~CHandle_Move()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CHandle_Move::Init()
{

	// 種類の設定
	SetType(TYPE_OBJECT2D);

	// 初期化処理
	CHandle::Init();

	return S_OK;
}

//==========================================================================
// 掴み中処理
//==========================================================================
void CHandle_Move::Grab()
{
	CInputMouse* pMouse = CInputMouse::GetInstance();

	// 再移動中
	MyLib::Vector3 diffpos = pMouse->GetWorldDiffPosition();

	// リセット
	m_DiffMove = 0.0f;

	// 移動の差分
	m_DiffMove = diffpos;
	m_DiffMove.y *= m_scale;
}

//==========================================================================
// 移動差分取得
//==========================================================================
CHandle::SEditHandleInfo CHandle_Move::GetDiff(HandleAngle angle)
{
	CHandle::SEditHandleInfo info;
	info.pos = m_DiffMove;

	switch (angle)
	{
	case CHandle::ANGLE_Z:
		info.pos.x = 0.0f;
		info.pos.y = 0.0f;
		break;

	case CHandle::ANGLE_Y:
		info.pos.x = 0.0f;
		info.pos.z = 0.0f;
		break;

	case CHandle::ANGLE_X:
		info.pos.y = 0.0f;
		info.pos.z = 0.0f;
		break;

	default:
		info.pos.y = 0.0f;

		{
			CInputMouse* pMouse = CInputMouse::GetInstance();
			info.pos.x = pMouse->GetWorldPosition().x - GetPosition().x;
			info.pos.z = pMouse->GetWorldPosition().z - GetPosition().z;
		}
		break;
	}

	return info;
}

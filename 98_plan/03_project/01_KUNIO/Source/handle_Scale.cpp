//=============================================================================
// 
// 拡縮ハンドル処理 [handle_Scale.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "handle_Scale.h"
#include "manager.h"
#include "calculation.h"
#include "3D_Effect.h"
#include "camera.h"
#include "objectLine.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CHandle_Scale::CHandle_Scale(int nPriority) : CHandle(nPriority)
{
	// 値のクリア
}

//==========================================================================
// デストラクタ
//==========================================================================
CHandle_Scale::~CHandle_Scale()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CHandle_Scale::Init()
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
void CHandle_Scale::Grab()
{
	CInputMouse* pMouse = CInputMouse::GetInstance();

	// ドラッグ取得
	m_DiffScale = 0.0f;
	m_DiffScale = pMouse->GetScreenDiffFactor();

	if (pMouse->GetDeltaX() < 0) {
		m_DiffScale *= -1;
	}



	// 再移動中
	MyLib::Vector3 diffpos = pMouse->GetWorldDiffPosition();

	// リセット
	m_DiffMove = 0.0f;

	// 移動の差分
	m_DiffMove = diffpos;

}

//==========================================================================
// 移動差分取得
//==========================================================================
CHandle::SEditHandleInfo CHandle_Scale::GetDiff(HandleAngle angle)
{
	CHandle::SEditHandleInfo info;
	info.scale = m_DiffMove * 0.001f;

	switch (angle)
	{
	case CHandle::ANGLE_Z:
		info.scale.x = 0.0f;
		info.scale.y = 0.0f;
		break;

	case CHandle::ANGLE_Y:
		info.scale.x = 0.0f;
		info.scale.z = 0.0f;
		break;

	case CHandle::ANGLE_X:
		info.scale.y = 0.0f;
		info.scale.z = 0.0f;
		break;

	default:
		info.scale = m_DiffScale;
		break;
	}

	return info;
}

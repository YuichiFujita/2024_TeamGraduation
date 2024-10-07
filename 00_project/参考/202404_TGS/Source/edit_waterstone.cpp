//=============================================================================
// 
//  コースエディタ―処理 [edit_waterstone.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "edit_waterstone.h"
#include "waterstone.h"
#include "waterstoneManager.h"
#include "manager.h"
#include "calculation.h"
#include "map_obstacle.h"
#include "camera.h"
#include "spline.h"
#include "collisionLine_Box.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const MyLib::AABB AABB_LINE = MyLib::AABB(-25.0f, 25.0f);	// 辺のAABB

}

//==========================================================================
// コンストラクタ
//==========================================================================
CEdit_WaterStone::CEdit_WaterStone()
{
	// 値のクリア
	m_nEditIdx = 0;			// 操作するインデックス番号
	m_bEdit = false;		// 操作中判定
	m_bDrag = false;		// 掴み判定
	m_bHoverWindow = false;	// マウスのウィンドウホバー判定
	m_bSetMode = false;		// 設定モード判定
}

//==========================================================================
// デストラクタ
//==========================================================================
CEdit_WaterStone::~CEdit_WaterStone()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CEdit_WaterStone::Init()
{

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CEdit_WaterStone::Uninit()
{
	// 終了処理
	delete this;
}

//==========================================================================
// 更新処理
//==========================================================================
void CEdit_WaterStone::Update()
{
	// ウィンドウのマウスホバー判定
	ImGuiHoveredFlags frag = 128;
	m_bHoverWindow = ImGui::IsWindowHovered(frag);

	// ファイル操作
	FileControl();

	// モード変更
	ChangeMode();

	// 基点追加
	AddPoint();

	// ライン選択
	SelectLine();

	// 掴み中
	DragLine();

	// トランスフォーム
	Transform();

}

//==========================================================================
// ファイル操作
//==========================================================================
void CEdit_WaterStone::FileControl()
{
	// 水中石マネージャ取得
	CWaterStone_Manager* pMgr = CWaterStone_Manager::GetInstance();
	if (pMgr == nullptr) return;

	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	float width = 150.0f;
	ImGui::SetNextItemWidth(width);
	if (ImGui::Button("Save"))
	{
		//pMgr->Save();
	}
	ImGui::SameLine();

	ImGui::SetNextItemWidth(width);
	if (ImGui::Button("Save_as"))
	{
		//pMgr->Save();
	}
	ImGui::SameLine();

	ImGui::SetNextItemWidth(width);
	if (ImGui::Button("Load"))
	{

	}
}

//==========================================================================
// モード変更
//==========================================================================
void CEdit_WaterStone::ChangeMode()
{
	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	if (ImGui::Checkbox("Enable SetMode!!!", &m_bSetMode))
	{
		// 操作判定リセット
		m_bEdit = false;
	}
}

//==========================================================================
// ライン選択
//==========================================================================
void CEdit_WaterStone::SelectLine()
{
	if (m_bSetMode) return;	// セットモードは終わり

	// マウス情報
	CInputMouse* pMouse = CInputMouse::GetInstance();
	MyLib::Vector3 mouseRay = pMouse->GetRay();
	MyLib::Vector3 mousePos = pMouse->GetNearPosition();

	MyLib::AABB aabb = MyLib::AABB(-25.0f, 25.0f);
	float time = 0.0f;
	MyLib::Matrix mtx, mtxTrans;


	if (!m_bHoverWindow &&
		!m_bDrag &&
		pMouse->GetTrigger(CInputMouse::BUTTON::BUTTON_LEFT))
	{// 左クリック時

		// 操作判定リセット
		m_bEdit = false;


		// 障害物のリスト取得
		CListManager<CWaterStone> list = CWaterStone::GetListObj();

		// 先頭を保存
		std::list<CWaterStone*>::iterator itr = list.GetEnd();
		CWaterStone* pObj = nullptr;

		int i = 0;
		while (list.ListLoop(itr))
		{
			CWaterStone* pObj = *itr;
			MyLib::Vector3 ObjPos = pObj->GetPosition();



			// マトリックス初期化
			mtx.Identity();
			mtxTrans.Identity();

			// 位置情報反映
			mtxTrans.Translation(ObjPos);
			mtx.Multiply(mtx, mtxTrans);

			MyLib::Vector3 OBpos;

			MyLib::AABB aabb = pObj->GetAABB();
			if (UtilFunc::Collision::CollisionRayAABB(&mousePos, &mouseRay, &aabb, &mtx, time, &OBpos))
			{// 重なり && 左クリック

				// 操作する辺保存
				m_nEditIdx = i;

				m_bEdit = true;
				break;
			}
			i++;
		}

	}
}

//==========================================================================
// ライン掴み
//==========================================================================
void CEdit_WaterStone::DragLine()
{

	// 水中石マネージャ取得
	CWaterStone_Manager* pMgr = CWaterStone_Manager::GetInstance();
	if (pMgr == nullptr) return;

	if (!m_bEdit) return;
	if (m_bSetMode) return;	// セットモードは終わり

	// 障害物のリスト取得
	CListManager<CWaterStone> list = CWaterStone::GetListObj();
	CWaterStone* pObj = list.GetData(m_nEditIdx);
	if (pObj == nullptr) return;

	MyLib::Vector3 ObjPos = pObj->GetPosition();

	// マウス情報
	CInputMouse* pMouse = CInputMouse::GetInstance();
	MyLib::Vector3 mouseRay = pMouse->GetRay();
	MyLib::Vector3 mousePos = pMouse->GetNearPosition();

	MyLib::AABB aabb = pObj->GetAABB();
	float time = 0.0f;
	MyLib::Matrix mtx, mtxTrans;


	if (!m_bHoverWindow &&
		pMouse->GetPress(CInputMouse::BUTTON::BUTTON_LEFT) && !CInputKeyboard::GetInstance()->GetPress(DIK_LALT))
	{// 左押し込み時

		// マトリックス初期化
		mtx.Identity();
		mtxTrans.Identity();

		// 位置情報反映
		mtxTrans.Translation(ObjPos);
		mtx.Multiply(mtx, mtxTrans);

		MyLib::Vector3 OBpos;

		if (UtilFunc::Collision::CollisionRayAABB(&mousePos, &mouseRay, &aabb, &mtx, time, &OBpos))
		{// 重なり
			m_bDrag = true;
		}

	}

	if (pMouse->GetRelease(CInputMouse::BUTTON::BUTTON_LEFT))
	{
		m_bDrag = false;
	}

	ObjPos = pObj->GetPosition();

	// ドラッグ中
	if (m_bDrag)
	{
		CCamera* pCamera = CManager::GetInstance()->GetCamera();
		if (pCamera == nullptr) return;

		// 再移動中
		MyLib::Vector3 diffpos = pMouse->GetWorldDiffPosition();
		ObjPos.x += diffpos.x;
		ObjPos.z += diffpos.z;
	}

	// 頂点データ設定
	pObj->SetPosition(ObjPos);
}

//==========================================================================
// トランスフォーム
//==========================================================================
void CEdit_WaterStone::Transform()
{
	// 障害物のリスト取得
	CListManager<CWaterStone> list = CWaterStone::GetListObj();
	CWaterStone* pSelectStone = list.GetData(m_nEditIdx);
	if (pSelectStone == nullptr) return;

	// 先頭を保存
	std::list<CWaterStone*>::iterator itr = list.GetEnd();
	CWaterStone* pObj = nullptr;

	int i = 0;
	while (list.ListLoop(itr))
	{
		CWaterStone* pObj = *itr;
		MyLib::Vector3 ObjPos = pObj->GetPosition();

		// デバッグ上のBOX取得
		CCollisionLine_Box* pBox = pObj->GetCollisionLineBox();
		if (pBox == nullptr) continue;

		// 色リセット
		D3DXCOLOR col = (i == m_nEditIdx) ? D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pBox->SetColor(col);

		i++;
	}



	CWaterStone_Manager::SStoneInfo selectInfo = pSelectStone->GetStoneInfo();

	MyLib::Vector3 move;
	move.x = sinf(D3DX_PI + selectInfo.rot.y) * 30.0f;
	move.z = cosf(D3DX_PI + selectInfo.rot.y) * 30.0f;

	CEffect3D::Create(
		MyLib::Vector3(selectInfo.pos.x, selectInfo.pos.y + 100.0f, selectInfo.pos.z),
		move,
		mylib_const::DEFAULT_COLOR,
		40.0f,
		10,
		CEffect3D::MOVEEFFECT::MOVEEFFECT_SUB,
		CEffect3D::TYPE::TYPE_NORMAL);

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	if (ImGui::TreeNode("Transform"))
	{
		float windowWidth = 100.0f;
		const float  ROT_MOVE = D3DX_PI * 0.01f;
		const float  POS_MOVE = 0.5f;

		if (!m_bEdit)
		{
			ImGui::Text(":None Obj:");
			ImGui::TreePop();
			return;
		}

		ImGui::Text("NowEdit : [ %d ]", m_nEditIdx);
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		// リセット
		if (ImGui::Button("RESET")) {
			selectInfo.pos = 0.0f;
			selectInfo.rot = 0.0f;
		}
		

		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		//=============================
		// 位置設定
		//=============================
		ImGui::Text("pos");
		ImGui::SameLine();

		// X
		ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("x", &selectInfo.pos.x, POS_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Y
		ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("y", &selectInfo.pos.y, POS_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Z
		ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("z", &selectInfo.pos.z, POS_MOVE, 0.0f, 0.0f, "%.2f");
		}
		ImGui::PopID();


		//=============================
		// 向き設定
		//=============================
		ImGui::Text("rot");
		ImGui::SameLine();

		// X
		ImGui::PushID(1); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("x", &selectInfo.rot.x, ROT_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Y
		ImGui::PushID(1); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("y", &selectInfo.rot.y, ROT_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Z
		ImGui::PushID(1); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("z", &selectInfo.rot.z, ROT_MOVE, 0.0f, 0.0f, "%.2f");
		}
		ImGui::PopID();


		//=============================
		// 種類設定
		//=============================
		ImGui::SetNextItemWidth(windowWidth);
		ImGui::SliderInt("Type", &selectInfo.type, 0, CWaterStone::GetModelNum() - 1);

		ImGui::TreePop();
	}

	// 角度の正規化
	UtilFunc::Transformation::RotNormalize(selectInfo.rot);

	if (m_bDrag)
		return;

	// 石情報設定
	pSelectStone->SetStoneInfo(selectInfo);
	pSelectStone->SetPosition(selectInfo.pos);
	pSelectStone->ChangeModelType(selectInfo.type);
}

//==========================================================================
// 地点追加
//==========================================================================
void CEdit_WaterStone::AddPoint()
{
	if (!m_bSetMode) return;	// セットモード以外は終わり

	ImGui::Text("Push Mouse Button!!!");

	// マウス情報
	CInputMouse* pMouse = CInputMouse::GetInstance();
	MyLib::Vector3 mouseRay = pMouse->GetRay();
	MyLib::Vector3 mousePos = pMouse->GetNearPosition();

	// カメラ情報
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	MyLib::Vector3 pos = UtilFunc::Transformation::CalcScreenToXZ(
		pMouse->GetPosition(),
		D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT),
		pCamera->GetMtxView(),
		pCamera->GetMtxProjection());

	

	// 設置
	if (!m_bHoverWindow &&
		!CInputKeyboard::GetInstance()->GetPress(DIK_LALT) &&
		pMouse->GetTrigger(CInputMouse::BUTTON::BUTTON_LEFT))
	{
		pos.y = 0.0f;
		CWaterStone_Manager::SStoneInfo info(pos, 0.0f, 0);
		CWaterStone::Create(info);
	}
}

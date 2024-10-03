//=============================================================================
// 
//  障害物エディタ―処理 [edit_obstacle.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "edit_obstacle.h"
#include "manager.h"
#include "calculation.h"
#include "map_obstacle.h"
#include "camera.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float DISTANCE_OBJ = 2000.0f;
}

//==========================================================================
// コンストラクタ
//==========================================================================
CEdit_Obstacle::CEdit_Obstacle()
{
	// 値のクリア
	m_editType = EditType::TYPE_ARRANGMENT;
	m_pEditControl = nullptr;	// 種類操作

}

//==========================================================================
// デストラクタ
//==========================================================================
CEdit_Obstacle::~CEdit_Obstacle()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CEdit_Obstacle::Init()
{
	// 操作変更
	ChangeMode(m_editType);
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CEdit_Obstacle::Uninit()
{
	// 終了処理
	delete this;
}

//==========================================================================
// 更新処理
//==========================================================================
void CEdit_Obstacle::Update()
{
	// コンボボックス
	static const char* items[] = { "Arrangement", "Collider" };
	static int select = 1;

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	if (ImGui::Combo(": Mode", &select, items, IM_ARRAYSIZE(items)))
	{
		ChangeMode(static_cast<EditType>(select));
	}


	// エディット更新
	ImGui::Dummy(ImVec2(0.0f, 5.0f));
	m_pEditControl->Update();
}

//==========================================================================
// モード変更
//==========================================================================
void CEdit_Obstacle::ChangeMode(EditType type)
{
	if (m_pEditControl != nullptr)
	{
		m_pEditControl->DeleteBoxLine();
		m_pEditControl->Uninit();
		m_pEditControl = nullptr;
	}

	switch (type)
	{
	case CEdit_Obstacle::TYPE_ARRANGMENT:
		m_pEditControl = DEBUG_NEW CEdit_Obstacle_Arrangment;
		break;

	case CEdit_Obstacle::TYPE_COLLIDER:
		m_pEditControl = DEBUG_NEW CEdit_Obstacle_Collider;
		break;
	}
	m_pEditControl->Init();
}

//==========================================================================
// 初期化
//==========================================================================
void CEdit_Obstacle_Arrangment::Init()
{
	// 障害物マネージャ取得
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();
	m_ObstacleInfo = pObstacleMgr->GetObstacleInfo(0);

	// 当たり判定ボックス生成
	CreateBoxLine();

}

//==========================================================================
// 当たり判定ボックス生成
//==========================================================================
void CEdit_Obstacle_Arrangment::CreateBoxLine()
{
	// 当たり判定ボックス削除
	DeleteBoxLine();

	// 障害物のリスト取得
	CListManager<CMap_Obstacle> list = CMap_Obstacle::GetListObj();

	// 先頭を保存
	std::list<CMap_Obstacle*>::iterator itr = list.GetEnd();
	CMap_Obstacle* pObj = nullptr;

	while (list.ListLoop(itr))
	{
		CMap_Obstacle* pObj = *itr;

		for (auto& boxcollider : pObj->GetObstacleInfo().boxcolliders)
		{
			// AABB設定
			MyLib::AABB aabb = MyLib::AABB(boxcollider.vtxMin, boxcollider.vtxMax);

			// 生成処理
			CCollisionLine_Box* pBox;
			pBox = CCollisionLine_Box::Create(aabb, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
			m_pCollisionLineBox.push_back(pBox);

			// 位置設定
			boxcollider.TransformOffset(pObj->GetWorldMtx());
			m_pCollisionLineBox.back()->SetPosition(boxcollider.GetMtx().GetWorldPosition());
		}
	}

}

//==========================================================================
// 更新処理
//==========================================================================
void CEdit_Obstacle_Arrangment::Update()
{

	// 障害物マネージャ取得
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();
	std::vector<CMap_ObstacleManager::SObstacleInfo> vecInfo = pObstacleMgr->GetObstacleInfo();

	// コンボボックス
	static const char* savetext[] = { "Save", "Save_as", "Load" };
	static int saveselect = 0;
	float width = 150.0f;



	// ウィンドウのマウスホバー判定
	ImGuiHoveredFlags frag = 128;
	m_bHoverWindow = ImGui::IsWindowHovered(frag);



	std::vector<std::string> items;
	for (const auto& info : vecInfo)
	{
		std::string file = UtilFunc::Transformation::RemoveFilePath(info.modelFile);
		items.push_back(file);
	}

	// char* の配列に変換
	std::vector<const char*> items_cstr;
	for (const auto& item : items)
	{
		items_cstr.push_back(item.c_str());
	}

	static int select = 0;

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	if (ImGui::Combo("SetType", &select, items_cstr.data(), items_cstr.size()))
	{
		// 障害物情報設定
		m_ObstacleInfo = vecInfo[select];
	}


	ImGuiDragDropFlags src_flags = 0;
	src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;     // Keep the source displayed as hovered
	src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers; // Because our dragging is local, we disable the feature of opening foreign treenodes/tabs while dragging
	//src_flags |= ImGuiDragDropFlags_SourceNoPreviewTooltip; // Hide the tooltip

	ImVec2 imageSize = ImVec2(150, 50);

	// ドラッグ可能な要素の描画
	ImGui::Button(": Drag Me! :", imageSize);

	// ドラッグ中テキスト描画
	if (ImGui::BeginDragDropSource(src_flags))
	{
		std::string file = UtilFunc::Transformation::RemoveFilePath(m_ObstacleInfo.modelFile);
		ImGui::Text(file.c_str());
		ImGui::EndDragDropSource();
	}

	if (ImGui::IsItemHovered() &&
		ImGui::IsMouseDown(0))
	{// UI上にカーソル && クリック
		m_bButtonDrag = true;
	}

	// 見た目のみ配置
	if (m_bButtonDrag &&
		!m_bHoverWindow)
	{// ドラッグ中 && ウィンドウ外

		if (m_pDragObj != nullptr) {
			m_pDragObj->Kill();
			m_pDragObj = nullptr;
		}

		// 生成
		/*m_pDragObj = CObjectX::Create(m_ObstacleInfo.modelFile);
		m_pDragObj->SetType(CObject::TYPE::TYPE_OBJECTX);*/
	}


	// マウス情報取得
	CInputMouse* pMouse = CInputMouse::GetInstance();
	MyLib::Vector3 mouseWorldPos = pMouse->GetWorldPosition();

	// ドラッグ中
	if (m_bButtonDrag)
	{
		MyLib::Vector3 setpos = mouseWorldPos;
		setpos.y = 0.0f;

		CEffect3D::Create(
			setpos,
			0.0f,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			80.0f, 2, CEffect3D::MOVEEFFECT::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
	}


	if (m_pDragObj != nullptr) {

		MyLib::Vector3 setpos = mouseWorldPos;
		setpos.y = 0.0f;

		m_pDragObj->SetPosition(setpos);
	}

	// 配置
	if (m_bButtonDrag &&
		ImGui::IsMouseReleased(0))
	{// 掴み中 && マウスリリース

		if (m_pDragObj != nullptr) {
			m_pDragObj->Kill();
			m_pDragObj = nullptr;
		}

		if (!m_bHoverWindow) {

			CInputMouse* pMouse = CInputMouse::GetInstance();
			MyLib::Vector3 mouseWorldPos = pMouse->GetWorldPosition();

			MyLib::Vector3 setpos = mouseWorldPos;
			setpos.y = 0.0f;

			CMap_Obstacle* pObj = CMap_Obstacle::Create(m_ObstacleInfo, setpos);
			pObj->CalWorldMtx();

			CreateBoxLine();
		}
		m_bButtonDrag = false;
	}


	
	// ハンドル切り替え
	ChangeHandle();


	// オブジェクト選択
	ObjectSelect();

	Transform();


	// 障害物のリスト取得
	CListManager<CMap_Obstacle> list = CMap_Obstacle::GetListObj();

	// 先頭を保存
	std::list<CMap_Obstacle*>::iterator itr = list.GetEnd();
	CMap_Obstacle* pObj = nullptr;

	int i = 0;
	while (list.ListLoop(itr))
	{
		CMap_Obstacle* pObj = *itr;

		for (auto& boxcollider : pObj->GetObstacleInfo().boxcolliders)
		{
			// 位置設定
			boxcollider.TransformOffset(pObj->GetWorldMtx());

			if (m_pCollisionLineBox.size() > i)
			{
				m_pCollisionLineBox[i]->SetPosition(boxcollider.GetMtx().GetWorldPosition());
				m_pCollisionLineBox[i]->SetRotation(pObj->GetRotation());
			}
			i++;
		}
	}

}

//==========================================================================
// オブジェクト選択
//==========================================================================
void CEdit_Obstacle_Arrangment::ObjectSelect()
{
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	CInputMouse* pMouse = CInputMouse::GetInstance();
	MyLib::Vector3 mouseRay = pMouse->GetRay();
	MyLib::Vector3 mousePos = pMouse->GetNearPosition();
	MyLib::Vector3 mouseWorldPos = pMouse->GetWorldPosition();
	MyLib::Vector3 mouseOldWorldPos = pMouse->GetOldWorldPosition();
	MyLib::Vector3 diffpos = pMouse->GetWorldDiffPosition();

	// 障害物のリスト取得
	CListManager<CMap_Obstacle> list = CMap_Obstacle::GetListObj();

	// 先頭を保存
	std::list<CMap_Obstacle*>::iterator itr = list.GetEnd();

	if (!m_bHoverWindow &&
		!m_bGrabHandle &&
		(m_pHandle == nullptr || (m_pHandle != nullptr && !m_pHandle->IsHoverHandle())) &&
		!pKeyboard->GetPress(DIK_LALT) &&
		ImGui::IsMouseClicked(0))
	{// クリック

		// 再掴み判定
		m_bReGrab = false;

		m_pGrabObj = nullptr;

		// 先頭を保存
		CMap_Obstacle* pObject = nullptr;

		// リストコピー
		std::vector<CMap_Obstacle*> pObjectSort;

		while (list.ListLoop(itr))
		{
			// 要素を末尾に追加
			pObjectSort.push_back((*itr));
		}

		// Zソート
		std::sort(pObjectSort.begin(), pObjectSort.end(), CObject::ZSortInverse);

		bool bHit = false;
		for (const auto& obj : pObjectSort)
		{
			if (!bHit) 
			{
				MyLib::AABB aabb;
				aabb.vtxMin = obj->GetVtxMin();
				aabb.vtxMax = obj->GetVtxMax();

				MyLib::Matrix mtx = obj->GetWorldMtx();
				float time = 0.0f;
				MyLib::Vector3 OBpos;

				bHit = UtilFunc::Collision::CollisionRayAABB(&mousePos, &mouseRay, &aabb, &mtx, time, &OBpos);

				if (bHit)
				{// 被ってる
					obj->SetState(CObjectX::STATE::STATE_EDIT);

					// 掴みオブジェクト
					m_pGrabObj = obj;

					if (m_pHandle != nullptr) {
						m_pHandle->SetPosition(m_pGrabObj->GetPosition());
					}

					if (m_pHandle == nullptr) {
						m_pHandle = CHandle::Create(m_HandleType, m_pGrabObj->GetPosition());
					}

					// 再掴み判定
					m_bReGrab = true;
				}
				else {
					obj->SetState(CObjectX::STATE::STATE_NONE);
				}
			}
			else {
				obj->SetState(CObjectX::STATE::STATE_NONE);
			}
		}

		if (m_pHandle != nullptr &&
			!m_pHandle->IsHoverHandle() &&
			!bHit) 
		{
			m_pHandle->Kill();
			m_pHandle = nullptr;
		}
	}




	if (m_pHandle != nullptr && 
		m_pHandle->IsHoverHandle() &&
		!pKeyboard->GetPress(DIK_LALT) &&
		ImGui::IsMouseClicked(0))
	{// ハンドルにホバー中クリック

		// 移動の向き
		m_moveAngle = m_pHandle->GetHoverAngle();
		m_pHandle->SetState(CHandle::State::STATE_GRAB);
		m_bGrabHandle = true;
	}


	if (m_bGrabHandle &&
		m_pGrabObj != nullptr)
	{
		MyLib::Vector3 pos = m_pGrabObj->GetPosition();
		MyLib::Vector3 rot = m_pGrabObj->GetRotation();
		MyLib::Vector3 scale = m_pGrabObj->GetScale();

		// 差分取得
		CHandle::SEditHandleInfo info = m_pHandle->GetDiff(m_moveAngle);
		pos += info.pos;
		rot += info.rot;
		UtilFunc::Transformation::RotNormalize(rot);

		scale += info.scale;
		if (scale.x <= 0.1f) scale.x = 0.1f;
		if (scale.y <= 0.1f) scale.y = 0.1f;
		if (scale.z <= 0.1f) scale.z = 0.1f;

		m_pGrabObj->SetPosition(pos);
		m_pGrabObj->SetRotation(rot);
		m_pGrabObj->SetScale(scale);

		if (m_pGrabObj->GetCollisionLineBox().size() > 0) {
			for (auto& box : m_pGrabObj->GetCollisionLineBox())
			{
				box->SetPosition(m_pGrabObj->GetPosition() + pos);
			}
		}

		if (m_pHandle != nullptr) {
			m_pHandle->SetPosition(m_pGrabObj->GetPosition());
		}
	}


	if (m_pHandle != nullptr &&
		!pKeyboard->GetPress(DIK_LALT) &&
		ImGui::IsMouseReleased(0))
	{// リリース

		m_bGrabHandle = false;
		m_pHandle->SetState(CHandle::State::STATE_NONE);
	}

	if (pKeyboard->GetTrigger(DIK_DELETE))
	{
		if (m_pHandle != nullptr)
		{
			m_pHandle->Kill();
			m_pHandle = nullptr;
		}

		if (m_pGrabObj != nullptr)
		{
			m_pGrabObj->Kill();
			m_pGrabObj = nullptr;
		}

		// 当たり判定ボックス生成
		CreateBoxLine();
	}

}

//==========================================================================
// ハンドル切り替え
//==========================================================================
void CEdit_Obstacle_Arrangment::ChangeHandle()
{
	bool bChange = false;

	// 移動ボタン
	if (ImGui::Button("Move", ImVec2(50.0f, 50.0f)))
	{
		m_HandleType = CHandle::HandleType::TYPE_MOVE;
		bChange = true;
	}
	ImGui::SameLine();

	// スケールボタン
	if (ImGui::Button("Scale", ImVec2(50.0f, 50.0f)))
	{
		m_HandleType = CHandle::HandleType::TYPE_SCALE;
		bChange = true;
	}
	ImGui::SameLine();

	if (bChange)
	{
		// 削除
		if (m_pHandle != nullptr) {
			m_pHandle->Kill();
			m_pHandle = nullptr;
		}

		// ハンドル生成
		if (m_pGrabObj != nullptr)
		{
			m_pHandle = CHandle::Create(m_HandleType, m_pGrabObj->GetPosition());
		}
	}

}

//==========================================================================
// トランスフォーム処理
//==========================================================================
void CEdit_Obstacle_Arrangment::Transform()
{

	//=============================
	// 向き設定
	//=============================
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	if (ImGui::TreeNode("Transform"))
	{
		float windowWidth = 100.0f;
		const float  ROT_MOVE = D3DX_PI * 0.01f;
		const float  POS_MOVE = 0.5f;

		if (m_pGrabObj == nullptr)
		{
			ImGui::Text(":None Obj:");
			ImGui::TreePop();
			return;
		}

		// リセット
		if (ImGui::Button("ALL RESET")) {
			m_pGrabObj->SetPosition(0.0f);
			m_pGrabObj->SetRotation(0.0f);
			m_pGrabObj->SetScale(1.0f);
		}
		ImGui::SameLine();
		if (ImGui::Button("POS RESET")) {
			m_pGrabObj->SetPosition(0.0f);
		}
		ImGui::SameLine();
		if (ImGui::Button("ROT RESET")) {
			m_pGrabObj->SetRotation(0.0f);
		}
		ImGui::SameLine();
		if (ImGui::Button("SCALE RESET")) {
			m_pGrabObj->SetScale(1.0f);
		}

		//=============================
		// 位置設定
		//=============================
		MyLib::Vector3 pos = m_pGrabObj->GetPosition();
		ImGui::Text("pos");
		ImGui::SameLine();

		// X
		ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("x", &pos.x, POS_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Y
		ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("y", &pos.y, POS_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Z
		ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("z", &pos.z, POS_MOVE, 0.0f, 0.0f, "%.2f");
		}
		ImGui::PopID();

		// 位置設定
		m_pGrabObj->SetPosition(pos);
		if (m_pGrabObj->GetCollisionLineBox().size() > 0) {
			for (auto& box : m_pGrabObj->GetCollisionLineBox())
			{
				box->SetPosition(m_pGrabObj->GetPosition() + pos);
			}
		}
		if (m_pHandle != nullptr) {
			m_pHandle->SetPosition(m_pGrabObj->GetPosition());
		}


		//=============================
		// 向き設定
		//=============================
		MyLib::Vector3 rot = m_pGrabObj->GetRotation();
		ImGui::Text("rot");
		ImGui::SameLine();

		// X
		ImGui::PushID(1); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("x", &rot.x, ROT_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Y
		ImGui::PushID(1); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("y", &rot.y, ROT_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Z
		ImGui::PushID(1); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("z", &rot.z, ROT_MOVE, 0.0f, 0.0f, "%.2f");
		}
		ImGui::PopID();

		// 向き設定
		UtilFunc::Transformation::RotNormalize(rot);
		m_pGrabObj->SetRotation(rot);


		//=============================
		// 拡縮設定
		//=============================
		MyLib::Vector3 scale = m_pGrabObj->GetScale();
		ImGui::Text("scale");
		ImGui::SameLine();

		// X
		ImGui::PushID(2); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("x", &scale.x, 0.01f, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Y
		ImGui::PushID(2); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("y", &scale.y, 0.01f, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Z
		ImGui::PushID(2); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("z", &scale.z, 0.01f, 0.0f, 0.0f, "%.2f");
		}
		ImGui::PopID();

		// 拡縮設定
		m_pGrabObj->SetScale(scale);

		ImGui::TreePop();
	}
}


//==========================================================================
// 初期化
//==========================================================================
void CEdit_Obstacle_Collider::Init()
{
	// 障害物マネージャ取得
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();
	std::vector<CMap_ObstacleManager::SObstacleInfo> vecInfo = pObstacleMgr->GetObstacleInfo();

	MyLib::Vector3 pos = MyLib::Vector3(0.0f, 5000.0f, 0.0f);
	for (const auto& info : vecInfo)
	{
		CObjectX* pObj = CObjectX::Create(info.modelFile, pos);
		pObj->SetType(CObject::TYPE::TYPE_OBJECTX);

		m_pObjX.push_back(pObj);

		pos.x += DISTANCE_OBJ;
	}

	// 当たり判定BOX生成
	CreateBoxLine();
}

//==========================================================================
// 当たり判定ボックス生成
//==========================================================================
void CEdit_Obstacle_Collider::CreateBoxLine()
{
	// 当たり判定ボックス削除
	DeleteBoxLine();

	// 障害物マネージャ取得
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();

	// 当たり判定ボックス取得
	CMap_ObstacleManager::SObstacleInfo info = pObstacleMgr->GetObstacleInfo(m_nEditIdx);

	for (auto& boxcollider : info.boxcolliders)
	{
		// AABB設定
		MyLib::AABB aabb = MyLib::AABB(boxcollider.vtxMin, boxcollider.vtxMax);

		// 生成処理
		CCollisionLine_Box* pBox;
		pBox = CCollisionLine_Box::Create(aabb, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		m_pCollisionLineBox.push_back(pBox);

		// 位置設定
		boxcollider.TransformOffset(m_pObjX[m_nEditIdx]->GetWorldMtx());
		m_pCollisionLineBox.back()->SetPosition(boxcollider.GetMtx().GetWorldPosition());
	}
}

//==========================================================================
// 更新処理
//==========================================================================
void CEdit_Obstacle_Collider::Update()
{
	// エディットメニュー
	if (ImGui::TreeNode("Obstacle Edit"))
	{
		MenuBar();

		//***********************
		// 現在のキー変更
		//***********************
		// [スライダー]調整するインデックス
		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		// リサイズ
		if (ImGui::TreeNode("Target"))
		{
			ImGui::SeparatorText("ChangeTarget Info");

			ImGui::SetNextItemWidth(140.0f);
			if (ImGui::SliderInt("Edit Idx", &m_nEditIdx, 0, m_pObjX.size() - 1))
			{
				CreateBoxLine();
			}
			ImGui::Dummy(ImVec2(0.0f, 10.0f));
			ImGui::TreePop();
		}


		if (ImGui::TreeNode("Collider"))
		{
			ImGui::SeparatorText("ChangeCollider Info");

			// 障害物マネージャ取得
			CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();
			CMap_ObstacleManager::SObstacleInfo info = pObstacleMgr->GetObstacleInfo(m_nEditIdx);

			// 総数変更
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Change Coolider Num:");
			ImGui::SameLine();
			if (ImGui::ArrowButton("##left", ImGuiDir_Left))
			{
				pObstacleMgr->SubCollider(m_nEditIdx);
				CreateBoxLine();
			}
			ImGui::SameLine(0.0f);
			if (ImGui::ArrowButton("##right", ImGuiDir_Right))
			{
				pObstacleMgr->AddCollider(m_nEditIdx);
				CreateBoxLine();
			}
			ImGui::SameLine();
			ImGui::Text("%d", info.boxcolliders.size());

			Resize();
			ImGui::Dummy(ImVec2(0.0f, 10.0f));
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}



	// カメラの情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	pCamera->SetTargetPosition(m_pObjX[m_nEditIdx]->GetPosition());
	pCamera->SetAutoMovingPosR(m_pObjX[m_nEditIdx]->GetPosition());

	// 障害物マネージャ取得
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();
	CMap_ObstacleManager::SObstacleInfo info = pObstacleMgr->GetObstacleInfo(m_nEditIdx);

	for (int i = 0; i < static_cast<int>(info.boxcolliders.size()); i++)
	{
		// BOXコライダー
		MyLib::Collider_BOX collider = info.boxcolliders[i];

		collider.TransformOffset(m_pObjX[m_nEditIdx]->GetWorldMtx());
		m_pCollisionLineBox[i]->SetPosition(collider.GetMtx().GetWorldPosition());
	}
}

//==========================================================================
// メニューバー処理
//==========================================================================
void CEdit_Obstacle_Collider::MenuBar()
{
	// 障害物マネージャ取得
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();

	


	// コンボボックス
	static const char* savetext[] = { "Save", "Save_as", "Load" };
	static int saveselect = 0;
	float width = 150.0f;

	ImGui::SetNextItemWidth(width);
	if (ImGui::Button("Save"))
	{
		pObstacleMgr->SaveInfo();
	}
	ImGui::SameLine();

	ImGui::SetNextItemWidth(width);
	if (ImGui::Button("Save_as"))
	{
		pObstacleMgr->SaveInfo();
	}
	ImGui::SameLine();

	ImGui::SetNextItemWidth(width);
	if (ImGui::Button("Load"))
	{

	}



#if 0
	// コンボボックス
	static const char* items[] = { "Save", "Save_as", "Load" };
	int select = 0;
	if (ImGui::Combo(": Save & Load", &select, items, IM_ARRAYSIZE(items)))
	{
		// 選択された項目が変更されたときの処理
		switch (select)
		{
		case 0:	// Save
		{
			pObstacleMgr->SaveInfo();
		}
			break;

		case 1:	// Save_as
		{
			OPENFILENAMEA filename = {};
			char sFilePass[1024] = {};
			// ファイル選択ダイアログの設定
			filename.lStructSize = sizeof(OPENFILENAMEA);
			filename.hwndOwner = NULL;
			filename.lpstrFilter = "テキストファイル\0*.txt\0画像ファイル\0*.bmp;.jpg\0すべてのファイル\0.*\0\0";
			filename.lpstrFile = sFilePass;
			filename.nMaxFile = MAX_PATH;
			filename.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;


			// カレントディレクトリを取得する
			char szCurrentDir[MAX_PATH];
			GetCurrentDirectoryA(MAX_PATH, szCurrentDir);

			// "data"フォルダの絶対パスを求める
			std::string strDataDir = szCurrentDir;
			strDataDir += "\\data\\TEXT\\mapobstacle";

			// 存在する場合は、lpstrInitialDirに指定する
			if (GetFileAttributesA(strDataDir.c_str()) != INVALID_FILE_ATTRIBUTES)
			{
				filename.lpstrInitialDir = strDataDir.c_str();
			}


			// ファイル選択ダイアログを表示
			if (GetOpenFileNameA(&filename))
			{
				// 選択されたファイルのパスを表示
				printf("Selected file: %s\n", sFilePass);
				int n = 0;
			}
			//セーブ
			if (strcmp(&sFilePass[0], "") != 0) {
				int n = 0;
			}
		}
			break;

		case 2:	// Load
			
			break;
		}
	}
#endif

#if 0
	// 書き出し
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Save"))
		{
			pObstacleMgr->SaveInfo();
		}

		if (ImGui::MenuItem("Save_as"))
		{
			OPENFILENAMEA filename = {};
			char sFilePass[1024] = {};
			// ファイル選択ダイアログの設定
			filename.lStructSize = sizeof(OPENFILENAMEA);
			filename.hwndOwner = NULL;
			filename.lpstrFilter = "テキストファイル\0*.txt\0画像ファイル\0*.bmp;.jpg\0すべてのファイル\0.*\0\0";
			filename.lpstrFile = sFilePass;
			filename.nMaxFile = MAX_PATH;
			filename.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;


			// カレントディレクトリを取得する
			char szCurrentDir[MAX_PATH];
			GetCurrentDirectoryA(MAX_PATH, szCurrentDir);

			// "data"フォルダの絶対パスを求める
			std::string strDataDir = szCurrentDir;
			strDataDir += "\\data\\TEXT\\mapobstacle";

			// 存在する場合は、lpstrInitialDirに指定する
			if (GetFileAttributesA(strDataDir.c_str()) != INVALID_FILE_ATTRIBUTES)
			{
				filename.lpstrInitialDir = strDataDir.c_str();
			}


			// ファイル選択ダイアログを表示
			if (GetOpenFileNameA(&filename))
			{
				// 選択されたファイルのパスを表示
				printf("Selected file: %s\n", sFilePass);
				int n = 0;
			}
			//セーブ
			if (strcmp(&sFilePass[0], "") != 0) {
				int n = 0;
			}
		}

		// ロード
		if (ImGui::MenuItem("Load"))
		{

		}

		ImGui::EndMenu();
	}
#endif

}

//==========================================================================
// リサイズ
//==========================================================================
void CEdit_Obstacle_Collider::Resize()
{
	// 障害物マネージャ取得
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();
	CMap_ObstacleManager::SObstacleInfo info = pObstacleMgr->GetObstacleInfo(m_nEditIdx);

	// [スライダー]調整するコライダーのインデックス
	ImGui::SetNextItemWidth(140.0f);
	if (ImGui::SliderInt("Collider Idx", &m_nColliderIdx, 0, info.boxcolliders.size() - 1))
	{

	}

	float windowWidth = 100.0f;
	const float  POS_MOVE = 0.5f;

	ImGui::Dummy(ImVec2(0.0f, 5.0f));
	// リサイズ
	if (ImGui::TreeNode("Resize"))
	{
		ImGui::SeparatorText("ChangeAABB Info");

		// BOXコライダー
		MyLib::Collider_BOX collider = info.boxcolliders[m_nColliderIdx];

		// リセット
		if (ImGui::Button("ALL RESET")) {
			collider.vtxMin = 0.0f;
			collider.vtxMax = 0.0f;
			collider.offset = 0.0f;
		}

		if (ImGui::Button("VtxMax RESET")) {
			collider.vtxMax = 0.0f;
		}
		ImGui::SameLine();

		if (ImGui::Button("VtxMin RESET")) {
			collider.vtxMin = 0.0f;
		}
		ImGui::SameLine();

		if (ImGui::Button("Offset RESET")) {
			collider.offset = 0.0f;
		}

		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		{
			ImGui::Text("VtxMax");
			ImGui::SameLine();
			// MaxX
			ImGui::PushID(1); // ウィジェットごとに異なるIDを割り当てる
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("x", &collider.vtxMax.x, POS_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// MaxY
			ImGui::PushID(1); // ウィジェットごとに異なるIDを割り当てる
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("y", &collider.vtxMax.y, POS_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// MaxZ
			ImGui::PushID(1); // ウィジェットごとに異なるIDを割り当てる
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("z", &collider.vtxMax.z, POS_MOVE, 0.0f, 0.0f, "%.2f");
			}
			ImGui::PopID();
		}

		{
			ImGui::Text("VtxMin");
			ImGui::SameLine();
			// MinX
			ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("x", &collider.vtxMin.x, POS_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// MinY
			ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("y", &collider.vtxMin.y, POS_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// MinZ
			ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("z", &collider.vtxMin.z, POS_MOVE, 0.0f, 0.0f, "%.2f");
			}
			ImGui::PopID();
		}

		{

			ImGui::Text("Offset");
			ImGui::SameLine();
			// X
			ImGui::PushID(2); // ウィジェットごとに異なるIDを割り当てる
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("x", &collider.offset.x, POS_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// Y
			ImGui::PushID(2); // ウィジェットごとに異なるIDを割り当てる
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("y", &collider.offset.y, POS_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// Z
			ImGui::PushID(2); // ウィジェットごとに異なるIDを割り当てる
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("z", &collider.offset.z, POS_MOVE, 0.0f, 0.0f, "%.2f");
			}
			ImGui::PopID();
		}

		// BOX設定
		MyLib::AABB aabb = MyLib::AABB(collider.vtxMin, collider.vtxMax);
		m_pCollisionLineBox[m_nColliderIdx]->SetAABB(aabb);


		// BOXコライダー設定
		info.boxcolliders[m_nColliderIdx] = collider;
		pObstacleMgr->SetObstacleInfo(info, m_nEditIdx);

		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::TreePop();
	}
}

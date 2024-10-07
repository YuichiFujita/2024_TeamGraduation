//=============================================================================
// 
//  コースエディタ―処理 [edit_course.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "edit_course.h"
#include "courseManager.h"
#include "course.h"
#include "manager.h"
#include "calculation.h"
#include "map_obstacle.h"
#include "camera.h"
#include "spline.h"
#include "map_obstacle.h"
#include "checkpoint.h"
#include "map_block.h"
#include "waterstone.h"
#include "edit_mapblock.h"
#include "edit_map.h"
#include "frontobj_manager.h"
#include "stonewall_front.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const MyLib::AABB AABB_LINE = MyLib::AABB(-25.0f, 25.0f);	// 辺のAABB
	const D3DXCOLOR DEFAULT_COLOR = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	const D3DXCOLOR SELECT_COLOR = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	const MyLib::Vector3 UP = MyLib::Vector3(0.0f, 0.0f, 0.0f);
}

//==========================================================================
// コンストラクタ
//==========================================================================
CEdit_Course::CEdit_Course()
{
	// 値のクリア
	m_nCourseEditIdx = 0;		// 操作するインデックス番号
	m_nCheckPointEditIdx = 0;	// 操作するインデックス番号
	m_nObstacleEditIdx = 0;		// 操作するインデックス番号
	m_nJudgeEditIdx = 0;	// 操作するインデックス番号
	m_nJudgeObjEditIdx = 0;	// 操作するインデックス番号
	m_nVtxEditIdx = 0;			// 操作するインデックス番号
	m_bEdit = false;		// 操作中判定
	m_bDrag = false;		// 掴み判定
	m_bHoverWindow = false;	// マウスのウィンドウホバー判定
	m_bSetMode = false;		// 設定モード判定
	m_bAutoCreateMode = false;		// 自動生成判定
	m_pEditObstacle = nullptr;	// 障害物エディター
	m_pEditWaterStone = nullptr;	// 水中岩エディター
	m_pEditMap = nullptr;
}

//==========================================================================
// デストラクタ
//==========================================================================
CEdit_Course::~CEdit_Course()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CEdit_Course::Init()
{
	// 障害物エディター
	m_pEditObstacle = CEdit::Create(CGame::EditType::EDITTYPE_OBSTACLE);

	// 水中岩エディター
	m_pEditWaterStone = CEdit::Create(CGame::EditType::EDITTYPE_WATERSTONE);

	m_pEditMap = DEBUG_NEW CEdit_MapBlock;
	m_pEditMap->Init();
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CEdit_Course::Uninit()
{
	m_pEditMap->Uninit();
	// 終了処理
	delete this;
}

//==========================================================================
// 更新処理
//==========================================================================
void CEdit_Course::Update()
{
	// ウィンドウのマウスホバー判定
	ImGuiHoveredFlags frag = 128;
	m_bHoverWindow = ImGui::IsWindowHovered(frag);

	// 編集するコース変更
	ChangeEditCourse();

	// チェックポイント編集
	TransCheckPoint();

	// ジャッジ編集
	TransJudge();
	
	// ファイル操作
	FileControl();

	// レベル変更
	ChangeLevel();

	// 辺の総数変更
	ChangeLineNum();

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

	// 最初と最後変形
	TransformBeginEnd();

	// 障害物エディット
	ObstacleEdit();

	// マップエディット
	MapEdit();

	// 水中岩エディット
	WaterStoneEdit();

}

//==========================================================================
// ファイル操作
//==========================================================================
void CEdit_Course::FileControl()
{
	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	// コースマネージャ取得
	CCourseManager* pCourceManager = CCourseManager::GetInstance();
	if (pCourceManager == nullptr) return;

	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	float width = 150.0f;
	ImGui::SetNextItemWidth(width);
	if (ImGui::Button("Save"))
	{
		SaveObstacle();
		SaveMap();
		SaveWaterStone();
		pCourceManager->Save();
	}
	ImGui::SameLine();

	ImGui::SetNextItemWidth(width);
	if (ImGui::Button("Save_as"))
	{
		pCourceManager->Save();
	}
	ImGui::SameLine();

	ImGui::SetNextItemWidth(width);
	if (ImGui::Button("Load"))
	{

	}
}

//==========================================================================
// レベル変更
//==========================================================================
void CEdit_Course::ChangeLevel()
{
	// 現在のブロック情報
	CListManager<CMapBlockInfo> BlockInfoList = CMapBlock::GetInfoList();
	CMapBlockInfo* pBlockInfo = BlockInfoList.GetData(m_nCourseEditIdx);
	int level = pBlockInfo->GetLevel();

	if (ImGui::SliderInt("Course Level", &level, 0, 2))
	{

	}

	// レベル設定
	pBlockInfo->SetLevel(level);
}

//==========================================================================
// 編集するコース変更
//==========================================================================
void CEdit_Course::ChangeEditCourse()
{
	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	// コースマネージャ取得
	CCourseManager* pCourceManager = CCourseManager::GetInstance();
	if (pCourceManager == nullptr) return;

	// 基点の数
	int segmentSize = pCourceManager->GetSegmentSize() - 1;
	if (ImGui::SliderInt("Course Edit Idx", &m_nCourseEditIdx, 0, segmentSize))
	{
		// 障害物リセット
		ResetObstacle();

		// マップリセット
		ResetMap();

		// 水中岩リセット
		ResetWaterStone();

		std::vector<MyLib::Vector3> vecpos = pCourceManager->GetSegmentPos(m_nCourseEditIdx);
		vecpos.insert(vecpos.begin(), MyLib::Vector3(-10.0f, 0.0f, 0.0f));
		vecpos.push_back(vecpos.back() + MyLib::Vector3(10.0f, 0.0f, 0.0f));

		pCourse->SetVecPosition(vecpos);
		pCourse->ReCreateVtx();


		// 石垣再生成
		{
			CStoneWall* pStoneWall = pCourceManager->GetStoneWall();

			// 基点地点設定
			pStoneWall->SetVecPosition(pCourse->GetVecPosition());
			pStoneWall->Reset();

			std::vector<CCourse::VtxInfo> vtxInfo = pCourse->GetVecVtxinfo();
			std::vector<MyLib::Vector3> vecpos;

			MyLib::Vector3 setpos;
			for (const auto& info : vtxInfo)
			{
				setpos.x = info.pos.x + sinf(D3DX_PI + info.rot.y) * -600.0f;
				setpos.y = info.pos.y;
				setpos.z = info.pos.z + cosf(D3DX_PI + info.rot.y) * -600.0f;
				vecpos.push_back(setpos);
			}

			// 各頂点座標
			pStoneWall->SetVecVtxPosition(vecpos);
			pStoneWall->BindVtxPosition();
		}


		// 石垣再生成
		{
			CStoneWall* pStoneWall = pCourceManager->GetStoneWall_Front();

			// 基点地点設定
			pStoneWall->SetVecPosition(pCourse->GetVecPosition());
			pStoneWall->Reset();

			std::vector<CCourse::VtxInfo> vtxInfo = pCourse->GetVecVtxinfo();
			vecpos.clear();

			MyLib::Vector3 setpos;
			for (const auto& info : vtxInfo)
			{
				setpos.x = info.pos.x + sinf(D3DX_PI + info.rot.y) * 800.0f;
				setpos.y = info.pos.y;
				setpos.z = info.pos.z + cosf(D3DX_PI + info.rot.y) * 800.0f;
				vecpos.push_back(setpos);
			}

			// 各頂点座標
			pStoneWall->SetVecVtxPosition(vecpos);
			pStoneWall->BindVtxPosition();
		}



		m_nCheckPointEditIdx = 0;	// チェックポイントのインデックスリセット
	}


	// 総数変更
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Change Coolider Num:");
	ImGui::SameLine();
	if (ImGui::ArrowButton("##left", ImGuiDir_Left))
	{
		pCourceManager->SubSegmentPos();
		
		CMapBlock::GetInfoList().Delete(CMapBlock::GetInfoList().GetData(CMapBlock::GetInfoList().GetNumAll() - 1));

	}
	ImGui::SameLine(0.0f);
	if (ImGui::ArrowButton("##right", ImGuiDir_Right))
	{
		pCourceManager->AddSegmentPos();

		// 生成してリストの管理下に
		CMapBlockInfo* pBlock = DEBUG_NEW CMapBlockInfo;
		pBlock->Init();
		CMapBlock::GetInfoListPtr()->Regist(pBlock);
	}
	ImGui::SameLine();

	// サイズ
	segmentSize = pCourceManager->GetSegmentSize();
	ImGui::Text("%d", segmentSize);

}

//==========================================================================
// チェックポイント編集
//==========================================================================
void CEdit_Course::TransCheckPoint()
{
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	if (ImGui::TreeNode("CheckPoint"))
	{
		CCourse* pCourse = CGame::GetInstance()->GetCourse();
		if (pCourse == nullptr) return;

		// コースマネージャ取得
		CCourseManager* pCourceManager = CCourseManager::GetInstance();
		if (pCourceManager == nullptr) return;

		float width = 150.0f;

		// チェックポイントのリスト取得
		std::vector<float> vecCheckpoint = CMapBlock::GetInfoList().GetData(m_nCourseEditIdx)->GetCheckpointInfo();
		int checkpointSize = vecCheckpoint.size() - 1;

		// 総数変更
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Change Checkpoint Num:");
		ImGui::SameLine();
		if (ImGui::ArrowButton("##left", ImGuiDir_Left) &&
			static_cast<int>(vecCheckpoint.size()) > 1)
		{
			vecCheckpoint.pop_back();
		}
		ImGui::SameLine(0.0f);
		if (ImGui::ArrowButton("##right", ImGuiDir_Right))
		{
			vecCheckpoint.push_back(0.0f);
		}
		ImGui::SameLine();

		// サイズ
		ImGui::Text("%d", checkpointSize + 1);


		// 例外処理
		if (vecCheckpoint.empty())
		{
			ImGui::TreePop();
			return;
		}


		// 操作する対象切り替え
		ImGui::SetNextItemWidth(width);
		if (ImGui::SliderInt("Checkpoint Edit Idx", &m_nCheckPointEditIdx, 0, checkpointSize))
		{

		}

		if (!vecCheckpoint.empty())
		{
			ImGui::DragFloat("Length", &vecCheckpoint[m_nCheckPointEditIdx], 1.0f, 0.0f, 0.0f, "%.2f");
		}

		std::vector<MyLib::Vector3> vecpos = CGame::GetInstance()->GetCourse()->GetVecPosition();
		MyLib::Vector3 pos = MySpline::GetSplinePosition_NonLoop(vecpos, vecCheckpoint[m_nCheckPointEditIdx]);
		pos.y += 100.0f;
		CEffect3D::Create(
			pos,
			0.0f,
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
			40.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

		// チェックポイントのリスト設定
		CMapBlock::GetInfoList().GetData(m_nCourseEditIdx)->SetCheckpointInfo(vecCheckpoint);

		for (const auto& len : vecCheckpoint)
		{
			std::vector<float> vecLen;
			vecLen.resize(CGame::GetInstance()->GetCourse()->GetVecPosition().size());

			float toatallen = MySpline::CalSegmentLength(vecpos, &vecLen);

			for (float len = 0.0f; len <= toatallen;)
			{
				MyLib::Vector3 pos = MySpline::GetSplinePosition_NonLoop(vecpos, len);
				CEffect3D::Create(
					pos,
					0.0f,
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
					40.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

				len += 400.0f;
			}

		}

		ImGui::TreePop();
	}
}

//==========================================================================
// ジャッジ編集
//==========================================================================
void CEdit_Course::TransJudge()
{
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	if (ImGui::TreeNode("Judge"))
	{
		CCourse* pCourse = CGame::GetInstance()->GetCourse();
		if (pCourse == nullptr) return;

		// コースマネージャ取得
		CCourseManager* pCourceManager = CCourseManager::GetInstance();
		if (pCourceManager == nullptr) return;

		float width = 150.0f;

		// チェックポイントのリスト取得
		std::vector<std::vector<CMapBlockInfo::SJudgeInfo>> vecJudge = CMapBlock::GetInfoList().GetData(m_nCourseEditIdx)->GetJudgeInfo();
		int judgeSize = vecJudge.size() - 1;

		//=============================
		// 総数変更
		//=============================
		{
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Change Judge Num:");
			ImGui::SameLine();
			if (ImGui::ArrowButton("##left", ImGuiDir_Left) &&
				static_cast<int>(vecJudge.size()) > 1)
			{
				m_nJudgeEditIdx = 0;
				m_nJudgeObjEditIdx = 0;
				vecJudge.pop_back();
			}
			ImGui::SameLine(0.0f);
			if (ImGui::ArrowButton("##right", ImGuiDir_Right))
			{
				vecJudge.push_back(std::vector<CMapBlockInfo::SJudgeInfo>());
				vecJudge.back().emplace_back();
			}
			ImGui::SameLine();

			// サイズ
			ImGui::Text("%d", judgeSize + 1);
		}

		// 例外処理
		if (vecJudge.empty())
		{
			ImGui::TreePop();
			return;
		}

		//=============================
		// 内容物総数変更
		//=============================
		int objSize = vecJudge[m_nJudgeEditIdx].size() - 1;
		{
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Change Obj Num:");
			ImGui::SameLine();
			if (ImGui::ArrowButton("##lefttt", ImGuiDir_Left) &&
				static_cast<int>(vecJudge.size()) > 1)
			{
				vecJudge[m_nJudgeEditIdx].pop_back();
			}
			ImGui::SameLine(0.0f);
			if (ImGui::ArrowButton("##righttt", ImGuiDir_Right))
			{
				vecJudge[m_nJudgeEditIdx].push_back(CMapBlockInfo::SJudgeInfo());
			}
			ImGui::SameLine();

			// サイズ
			ImGui::Text("%d", objSize + 1);
		}


		// 操作する対象切り替え
		ImGui::SetNextItemWidth(width);
		if (ImGui::SliderInt("Judge Edit Idx", &m_nJudgeEditIdx, 0, judgeSize))
		{
			m_nJudgeObjEditIdx = 0;
		}

		ImGui::SetNextItemWidth(width);
		if (ImGui::SliderInt("Obj Edit Idx", &m_nJudgeObjEditIdx, 0, objSize))
		{

		}

		if (!vecJudge.empty() && !vecJudge[m_nJudgeEditIdx].empty())
		{
			ImGui::DragFloat("Length", &vecJudge[m_nJudgeEditIdx][m_nJudgeObjEditIdx].length, 1.0f, 0.0f, 0.0f, "%.2f");
			ImGui::DragFloat("Height", &vecJudge[m_nJudgeEditIdx][m_nJudgeObjEditIdx].height, 5.0f, 0.0f, 0.0f, "%.2f");
		}

		for (const auto& judge : vecJudge[m_nJudgeEditIdx])
		{
			std::vector<MyLib::Vector3> vecpos = CGame::GetInstance()->GetCourse()->GetVecPosition();
			MyLib::Vector3 pos = MySpline::GetSplinePosition_NonLoop(vecpos, judge.length);
			pos.y += judge.height;
			CEffect3D::Create(
				pos,
				0.0f,
				D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
				40.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
		}

		// チェックポイントのリスト設定
		CMapBlock::GetInfoList().GetData(m_nCourseEditIdx)->SetJudgeInfo(vecJudge);

		ImGui::TreePop();
	}
}

//==========================================================================
// 障害物編集
//==========================================================================
void CEdit_Course::TransObstacle()
{
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	if (ImGui::TreeNode("Obstacle"))
	{
		CCourse* pCourse = CGame::GetInstance()->GetCourse();
		if (pCourse == nullptr) return;

		// コースマネージャ取得
		CCourseManager* pCourceManager = CCourseManager::GetInstance();
		if (pCourceManager == nullptr) return;

		// セーブ
		float width = 150.0f;
		ImGui::SetNextItemWidth(width);
		if (ImGui::Button("Save"))
		{
			// 追加
		}
		ImGui::SameLine();


		// 障害物のリスト取得
		CListManager<CMap_Obstacle> obstacleList = CMapBlock::GetList().GetData(m_nCourseEditIdx)->GetObstacleList();
		int checkpointSize = obstacleList.GetNumAll() - 1;

		ImGui::SetNextItemWidth(width);
		if (ImGui::SliderInt("Obstacle Edit Idx", &m_nObstacleEditIdx, 0, checkpointSize))
		{

		}

		// 情報の位置取得
		/*CCheckpoint* pCheckPoint = checkpointList.GetData(m_nCheckPointEditIdx);
		float length = pCheckPoint->GetLength();*/
		MyLib::Vector3 pos;

		ImGui::DragFloat3("pos", (float*)&pos, 1.0f, 0.0f, 0.0f, "%.2f");

		// 設定


		ImGui::TreePop();
	}
}

//==========================================================================
// 辺の数変更
//==========================================================================
void CEdit_Course::ChangeLineNum()
{
	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	ImVec2 imageSize = ImVec2(150, 50);
	if (ImGui::Button("Re : Create", imageSize))
	{
		pCourse->ReCreateVtx();
	}

}

//==========================================================================
// モード変更
//==========================================================================
void CEdit_Course::ChangeMode()
{
	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	if (ImGui::Checkbox("Enable SetMode!!!", &m_bSetMode))
	{
		// 操作判定リセット
		m_bEdit = false;
	}

	// 自動生成
	ImGui::Checkbox("Enable AutoCreateMode!!!", &m_bAutoCreateMode);
}

//==========================================================================
// ライン選択
//==========================================================================
void CEdit_Course::SelectLine()
{
	if (m_bSetMode) return;	// セットモードは終わり

	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	// 辺情報取得
	std::vector<MyLib::Vector3> vecSegmentPos = pCourse->GetVecPosition();
	MyLib::Vector3 coursepos = pCourse->GetPosition();

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

		int i = 0;
		for (const auto& vtxpos : vecSegmentPos)
		{
			if (i == 0 || i == 1 ||
				i == vecSegmentPos.size() - 2 || i == vecSegmentPos.size() - 1)
			{// 最初と最後は選べない
				i++;
				continue;
			}
			// マトリックス初期化
			mtx.Identity();
			mtxTrans.Identity();

			// 位置情報反映
			MyLib::Vector3 transpos = vtxpos + coursepos;
			mtxTrans.Translation(transpos);
			mtx.Multiply(mtx, mtxTrans);

			MyLib::Vector3 OBpos;

			if (UtilFunc::Collision::CollisionRayAABB(&mousePos, &mouseRay, &aabb, &mtx, time, &OBpos))
			{// 重なり && 左クリック

				// 操作する辺保存
				m_nVtxEditIdx = i;

				m_bEdit = true;
				break;
			}
			i++;
		}

		// 色リセット
		for (int i = 0; i < vecSegmentPos.size(); i++)
		{
			CCollisionLine_Box* pBox = pCourse->GetCollisionLineBox(i);
			if (pBox == nullptr) continue;

			pBox->SetColor(DEFAULT_COLOR);
		}
	}
}

//==========================================================================
// ライン掴み
//==========================================================================
void CEdit_Course::DragLine()
{
	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	if (!m_bEdit) return;
	if (m_bSetMode) return;	// セットモードは終わり

	// コースマネージャ取得
	CCourseManager* pCourceManager = CCourseManager::GetInstance();
	if (pCourceManager == nullptr) return;


	// セグメント位置
	std::vector<MyLib::Vector3> vecsegmentPos = pCourse->GetVecPosition();

	// 辺情報取得
	MyLib::Vector3 segmentPos = vecsegmentPos[m_nVtxEditIdx];
	MyLib::Vector3 coursepos = pCourse->GetPosition();



	// マウス情報
	CInputMouse* pMouse = CInputMouse::GetInstance();
	MyLib::Vector3 mouseRay = pMouse->GetRay();
	MyLib::Vector3 mousePos = pMouse->GetNearPosition();

	MyLib::AABB aabb = MyLib::AABB(-50.0f, 50.0f);
	float time = 0.0f;
	MyLib::Matrix mtx, mtxTrans;


	if (!m_bHoverWindow &&
		pMouse->GetPress(CInputMouse::BUTTON::BUTTON_LEFT))
	{// 左押し込み時

		// マトリックス初期化
		mtx.Identity();
		mtxTrans.Identity();

		// 位置情報反映
		MyLib::Vector3 transpos = segmentPos + coursepos;
		mtxTrans.Translation(transpos);
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

	// ドラッグ中
	if (m_bDrag)
	{
		CCamera* pCamera = CManager::GetInstance()->GetCamera();
		if (pCamera == nullptr) return;

		// 再移動中
		MyLib::Vector3 diffpos = pMouse->GetWorldDiffPosition();
		segmentPos.x += diffpos.x;
		segmentPos.z += diffpos.z;

		if (m_bAutoCreateMode)
		{
			pCourse->ReCreateVtx();
		}
	}

	// セグメント位置
	vecsegmentPos[m_nVtxEditIdx] = segmentPos;

	// コースの位置
	pCourse->SetVecPosition(vecsegmentPos);

	pCourceManager->SetSegmentPos(vecsegmentPos, m_nCourseEditIdx);
	pCourse->GetCollisionLineBox(m_nVtxEditIdx)->SetPosition(segmentPos);
}

//==========================================================================
// トランスフォーム
//==========================================================================
void CEdit_Course::Transform()
{
	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	// 辺情報取得
	std::vector<MyLib::Vector3> vecSegmentPos = pCourse->GetVecPosition();
	if (m_bEdit)
	{
		for (int i = 0; i < vecSegmentPos.size(); i++)
		{
			CCollisionLine_Box* pBox = pCourse->GetCollisionLineBox(i);
			if (pBox == nullptr) continue;

			D3DXCOLOR col = (i == m_nVtxEditIdx) ? SELECT_COLOR : DEFAULT_COLOR;
			pBox->SetColor(col);
		}
	}

	// 操作する辺の情報
	MyLib::Vector3 editpos = pCourse->GetVecPosition(m_nVtxEditIdx);

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

		ImGui::Text("NowEdit : [ %d ]", m_nVtxEditIdx);
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		// リセット
		if (ImGui::Button("RESET")) {
			editpos = 0.0f;
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
			ImGui::DragFloat("x", &editpos.x, POS_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Y
		ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("y", &editpos.y, POS_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Z
		ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("z", &editpos.z, POS_MOVE, 0.0f, 0.0f, "%.2f");
		}
		ImGui::PopID();


		////=============================
		//// 向き設定
		////=============================
		//ImGui::Text("rot");
		//ImGui::SameLine();

		//// X
		//ImGui::PushID(1); // ウィジェットごとに異なるIDを割り当てる
		//{
		//	ImGui::SetNextItemWidth(windowWidth);
		//	ImGui::DragFloat("x", &editInfo.rot.x, ROT_MOVE, 0.0f, 0.0f, "%.2f");
		//	ImGui::SameLine();
		//}
		//ImGui::PopID();

		//// Y
		//ImGui::PushID(1); // ウィジェットごとに異なるIDを割り当てる
		//{
		//	ImGui::SetNextItemWidth(windowWidth);
		//	ImGui::DragFloat("y", &editInfo.rot.y, ROT_MOVE, 0.0f, 0.0f, "%.2f");
		//	ImGui::SameLine();
		//}
		//ImGui::PopID();

		//// Z
		//ImGui::PushID(1); // ウィジェットごとに異なるIDを割り当てる
		//{
		//	ImGui::SetNextItemWidth(windowWidth);
		//	ImGui::DragFloat("z", &editInfo.rot.z, ROT_MOVE, 0.0f, 0.0f, "%.2f");
		//}
		//ImGui::PopID();




		////=============================
		//// 幅設定
		////=============================
		//ImGui::Text("width");
		//ImGui::SameLine();

		//// X
		//ImGui::PushID(2); // ウィジェットごとに異なるIDを割り当てる
		//{
		//	ImGui::SetNextItemWidth(windowWidth);
		//	ImGui::DragFloat("width", &editInfo.width, POS_MOVE, 0.0f, 0.0f, "%.2f");
		//	ImGui::SameLine();
		//}
		//ImGui::PopID();


		ImGui::TreePop();
	}

	// ライン情報設定
	pCourse->SetVecPosition(m_nVtxEditIdx, editpos);
}

//==========================================================================
// 地点追加
//==========================================================================
void CEdit_Course::AddPoint()
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

	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	// 基点情報取得
	std::vector<MyLib::Vector3> vecSegmentPos = pCourse->GetVecPosition();

	if (!m_bHoverWindow &&
		!CInputKeyboard::GetInstance()->GetPress(DIK_LALT) &&
		pMouse->GetTrigger(CInputMouse::BUTTON::BUTTON_LEFT))
	{

		int endIdx = (vecSegmentPos.size() - 1);
		vecSegmentPos.insert(vecSegmentPos.begin() + endIdx, pos);

		// 基点情報設定
		pCourse->SetVecPosition(vecSegmentPos);
		pCourse->ReCreateVtx();
	}
}

//==========================================================================
// 障害物エディット
//==========================================================================
void CEdit_Course::ObstacleEdit()
{
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	if (ImGui::TreeNode("ObstacleEdit"))
	{
		// 障害物エディット更新
		m_pEditObstacle->Update();

		ImGui::TreePop();
	}
}

//==========================================================================
// 障害物リセット
//==========================================================================
void CEdit_Course::ResetObstacle()
{

	// 障害物マネージャ取得
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();

	// 障害物のリスト取得
	CListManager<CMap_Obstacle> list = CMap_Obstacle::GetListObj();

	// 先頭を保存
	std::list<CMap_Obstacle*>::iterator itr = list.GetEnd();
	CMap_Obstacle* pObj = nullptr;

	while (list.ListLoop(itr))
	{
		CMap_Obstacle* pObj = *itr;
		pObj->Kill();
	}

	// 障害物のリスト取得
	std::vector<CMapBlockInfo::SObsacleInfo> obstacleInfo = CMapBlock::GetInfoList().GetData(m_nCourseEditIdx)->GetObstacleInfo();


	// 障害物情報
	std::vector<CMap_ObstacleManager::SObstacleInfo> vecInfo = pObstacleMgr->GetObstacleInfo();
	CMap_ObstacleManager::SObstacleInfo info;
	for (int i = 0; i < static_cast<int>(obstacleInfo.size()); i++)
	{
		// ブロックの障害物情報
		CMapBlockInfo::SObsacleInfo blockinfo = obstacleInfo[i];

		info = vecInfo[blockinfo.nType];

		CMap_Obstacle* pObj = CMap_Obstacle::Create(info, blockinfo.pos);
		pObj->SetRotation(blockinfo.rot);
		pObj->SetScale(blockinfo.scale);
		pObj->CalWorldMtx();
	}

}

//==========================================================================
// 障害物セーブ
//==========================================================================
void CEdit_Course::SaveObstacle()
{
	
	// 障害物のリスト取得
	CListManager<CMap_Obstacle> list = CMap_Obstacle::GetListObj();

	// 先頭を保存
	std::list<CMap_Obstacle*>::iterator itr = list.GetEnd();
	CMap_Obstacle* pObj = nullptr;

	std::vector<CMapBlockInfo::SObsacleInfo> savedate;

	while (list.ListLoop(itr))
	{
		CMap_Obstacle* pObj = *itr;

		if (!pObj->GetSave())
		{
			continue;
		}

		// 障害物マネージャ取得
		CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();
		std::vector<CMap_ObstacleManager::SObstacleInfo> vecObstacleMgrInfo = pObstacleMgr->GetObstacleInfo();

		// 障害物情報取得
		MyLib::Vector3 pos = pObj->GetPosition(), rot = pObj->GetRotation(), scale = pObj->GetScale();
		CMap_ObstacleManager::SObstacleInfo info = pObj->GetObstacleInfo();
		std::string text = info.textFile;

		// 種類割り出し
		const auto& obItr = std::find_if(vecObstacleMgrInfo.begin(), vecObstacleMgrInfo.end(),
			[&text](const CMap_ObstacleManager::SObstacleInfo& string) {return string.textFile == text; });
		int type = (obItr - vecObstacleMgrInfo.begin());


		// セーブ情報追加
		savedate.push_back(CMapBlockInfo::SObsacleInfo(pos, rot, scale, type));
	}

	// 障害物のリスト設定
	CMapBlock::GetInfoList().GetData(m_nCourseEditIdx)->SetObstacleInfo(savedate);
}

//==========================================================================
// マップエディット
//==========================================================================
void CEdit_Course::MapEdit()
{
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	if (ImGui::TreeNode("MapEdit"))
	{
		// 障害物エディット更新
		m_pEditMap->Update();

		ImGui::TreePop();
	}
}

//==========================================================================
// マップリセット
//==========================================================================
void CEdit_Course::ResetMap()
{
	// 障害物のリスト取得
	std::vector<CObjectX*> objinfo = CFrontObjManager::GetInstance()->GetInfo();
	std::vector<CMapBlockInfo::SObsacleInfo> savedate;

	for (int i = 0; i < objinfo.size(); i++)
	{
		objinfo[i]->Kill();
	}

	// 障害物のリスト取得
	std::vector<CMapBlockInfo::SObsacleInfo> mapInfo = CMapBlock::GetInfoList().GetData(m_nCourseEditIdx)->GetMapInfo();
	CFrontObjManager::GetInstance()->Reset();

	for (int i = 0; i < static_cast<int>(mapInfo.size()); i++)
	{
		// ブロックの障害物情報
		CMapBlockInfo::SObsacleInfo blockinfo = mapInfo[i];

		CObjectX* pObj = CObjectX::Create(blockinfo.nType, blockinfo.pos);
		pObj->SetRotation(blockinfo.rot);
		pObj->SetScale(blockinfo.scale);
		pObj->SetType(CObject::TYPE_OBJECTX);
		pObj->CalWorldMtx();
		CFrontObjManager::GetInstance()->Regist(pObj);
	}

}

//==========================================================================
// マップセーブ
//==========================================================================
void CEdit_Course::SaveMap()
{
	// 障害物のリスト取得
	std::vector<CObjectX*> objinfo = CFrontObjManager::GetInstance()->GetInfo();
	std::vector<CMapBlockInfo::SObsacleInfo> savedate;

	for(int i = 0; i < objinfo.size(); i++)
	{

		CMapBlockInfo::SObsacleInfo info = CMapBlockInfo::SObsacleInfo(objinfo[i]->GetPosition(), objinfo[i]->GetRotation(), objinfo[i]->GetScale(), objinfo[i]->GetIdxXFile());

		// セーブ情報追加
		savedate.push_back(info);
	}

	// 障害物のリスト設定
	CMapBlock::GetInfoList().GetData(m_nCourseEditIdx)->SetMapInfo(savedate);
}

//==========================================================================
// 水中岩エディット
//==========================================================================
void CEdit_Course::WaterStoneEdit()
{
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	if (ImGui::TreeNode("WaterStoneEdit"))
	{
		// 水中岩エディット更新
		m_pEditWaterStone->Update();

		ImGui::TreePop();
	}
}

//==========================================================================
// 水中岩リセット
//==========================================================================
void CEdit_Course::ResetWaterStone()
{
	// 障害物マネージャ取得
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();

	// 水中岩のリスト取得
	CListManager<CWaterStone> list = CWaterStone::GetListObj();

	// 先頭を保存
	std::list<CWaterStone*>::iterator itr = list.GetEnd();
	CMap_Obstacle* pObj = nullptr;

	while (list.ListLoop(itr))
	{
		CWaterStone* pObj = *itr;
		pObj->Kill();
	}

	// 障害物のリスト取得
	std::vector<CWaterStone_Manager::SStoneInfo> obstacleInfo = CMapBlock::GetInfoList().GetData(m_nCourseEditIdx)->GetWaterStoneInfo();

	// 障害物情報
	for (const auto& info : obstacleInfo)
	{
		CWaterStone* pObj = CWaterStone::Create(info);
		pObj->CalWorldMtx();
	}
}

//==========================================================================
// 水中岩セーブ
//==========================================================================
void CEdit_Course::SaveWaterStone()
{
	// 水中岩のリスト取得
	CListManager<CWaterStone> list = CWaterStone::GetListObj();

	// 先頭を保存
	std::list<CWaterStone*>::iterator itr = list.GetEnd();
	CWaterStone* pObj = nullptr;

	std::vector<CWaterStone_Manager::SStoneInfo> savedate;

	while (list.ListLoop(itr))
	{
		CWaterStone* pObj = *itr;

		// 障害物マネージャ取得
		CWaterStone_Manager::SStoneInfo info = pObj->GetStoneInfo();

		// セーブ情報追加
		savedate.push_back(info);
	}

	// 障害物のリスト設定
	CMapBlock::GetInfoList().GetData(m_nCourseEditIdx)->SetWaterStoneInfo(savedate);
}

//==========================================================================
// 最初と最後変形
//==========================================================================
void CEdit_Course::TransformBeginEnd()
{
	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	// 辺情報取得
	std::vector<MyLib::Vector3> segmentPos = pCourse->GetVecPosition();

	// 最初と最後、逆方向に少し出す
	MyLib::Vector3 begin, end;
	float angle = 0.0f;

	// 最初
	angle = segmentPos[2].AngleXZ(segmentPos[1]);
	begin = MyLib::Vector3(
		segmentPos[1].x + sinf(angle) * -10.0f,
		segmentPos[1].y,
		segmentPos[1].z + cosf(angle) * -10.0f);

	// 最後
	int endIdx = (segmentPos.size() - 1) - 1;
	angle = segmentPos[endIdx].AngleXZ(segmentPos[endIdx - 1]);
	end = MyLib::Vector3(
		segmentPos[endIdx].x + sinf(angle) * 10.0f,
		segmentPos[endIdx].y,
		segmentPos[endIdx].z + cosf(angle) * 10.0f);

	segmentPos[0] = begin;
	segmentPos[(segmentPos.size() - 1)] = end;

	pCourse->SetVecPosition(segmentPos);
	pCourse->GetCollisionLineBox(0)->SetPosition(begin);
	pCourse->GetCollisionLineBox((segmentPos.size() - 1))->SetPosition(end);
}

//==========================================================================
// 最初と最後変形
//==========================================================================
void CEdit_Course::TransformBeginEnd(std::vector<MyLib::Vector3>* pVecpos)
{

	// 辺情報取得
	std::vector<MyLib::Vector3> segmentPos = *pVecpos;

	// 最初と最後、逆方向に少し出す
	MyLib::Vector3 begin, end;
	float angle = 0.0f;

	// 最初
	angle = segmentPos[2].AngleXZ(segmentPos[1]);
	begin = MyLib::Vector3(
		segmentPos[1].x + sinf(angle) * -10.0f,
		segmentPos[1].y,
		segmentPos[1].z + cosf(angle) * -10.0f);

	// 最後
	int endIdx = (segmentPos.size() - 1) - 1;
	angle = segmentPos[endIdx].AngleXZ(segmentPos[endIdx - 1]);
	end = MyLib::Vector3(
		segmentPos[endIdx].x + sinf(angle) * 10.0f,
		segmentPos[endIdx].y,
		segmentPos[endIdx].z + cosf(angle) * 10.0f);

	segmentPos[0] = begin;
	segmentPos[(segmentPos.size() - 1)] = end;

	*pVecpos = segmentPos;
}

//=============================================================================
// 
//  �R�[�X�G�f�B�^�\���� [edit_course.cpp]
//  Author : ���n�Ή�
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
// �萔��`
//==========================================================================
namespace
{
	const MyLib::AABB AABB_LINE = MyLib::AABB(-25.0f, 25.0f);	// �ӂ�AABB
	const D3DXCOLOR DEFAULT_COLOR = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	const D3DXCOLOR SELECT_COLOR = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	const MyLib::Vector3 UP = MyLib::Vector3(0.0f, 0.0f, 0.0f);
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEdit_Course::CEdit_Course()
{
	// �l�̃N���A
	m_nCourseEditIdx = 0;		// ���삷��C���f�b�N�X�ԍ�
	m_nCheckPointEditIdx = 0;	// ���삷��C���f�b�N�X�ԍ�
	m_nObstacleEditIdx = 0;		// ���삷��C���f�b�N�X�ԍ�
	m_nJudgeEditIdx = 0;	// ���삷��C���f�b�N�X�ԍ�
	m_nJudgeObjEditIdx = 0;	// ���삷��C���f�b�N�X�ԍ�
	m_nVtxEditIdx = 0;			// ���삷��C���f�b�N�X�ԍ�
	m_bEdit = false;		// ���쒆����
	m_bDrag = false;		// �͂ݔ���
	m_bHoverWindow = false;	// �}�E�X�̃E�B���h�E�z�o�[����
	m_bSetMode = false;		// �ݒ胂�[�h����
	m_bAutoCreateMode = false;		// ������������
	m_pEditObstacle = nullptr;	// ��Q���G�f�B�^�[
	m_pEditWaterStone = nullptr;	// ������G�f�B�^�[
	m_pEditMap = nullptr;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEdit_Course::~CEdit_Course()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CEdit_Course::Init()
{
	// ��Q���G�f�B�^�[
	m_pEditObstacle = CEdit::Create(CGame::EditType::EDITTYPE_OBSTACLE);

	// ������G�f�B�^�[
	m_pEditWaterStone = CEdit::Create(CGame::EditType::EDITTYPE_WATERSTONE);

	m_pEditMap = DEBUG_NEW CEdit_MapBlock;
	m_pEditMap->Init();
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CEdit_Course::Uninit()
{
	m_pEditMap->Uninit();
	// �I������
	delete this;
}

//==========================================================================
// �X�V����
//==========================================================================
void CEdit_Course::Update()
{
	// �E�B���h�E�̃}�E�X�z�o�[����
	ImGuiHoveredFlags frag = 128;
	m_bHoverWindow = ImGui::IsWindowHovered(frag);

	// �ҏW����R�[�X�ύX
	ChangeEditCourse();

	// �`�F�b�N�|�C���g�ҏW
	TransCheckPoint();

	// �W���b�W�ҏW
	TransJudge();
	
	// �t�@�C������
	FileControl();

	// ���x���ύX
	ChangeLevel();

	// �ӂ̑����ύX
	ChangeLineNum();

	// ���[�h�ύX
	ChangeMode();

	// ��_�ǉ�
	AddPoint();

	// ���C���I��
	SelectLine();

	// �͂ݒ�
	DragLine();

	// �g�����X�t�H�[��
	Transform();

	// �ŏ��ƍŌ�ό`
	TransformBeginEnd();

	// ��Q���G�f�B�b�g
	ObstacleEdit();

	// �}�b�v�G�f�B�b�g
	MapEdit();

	// ������G�f�B�b�g
	WaterStoneEdit();

}

//==========================================================================
// �t�@�C������
//==========================================================================
void CEdit_Course::FileControl()
{
	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	// �R�[�X�}�l�[�W���擾
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
// ���x���ύX
//==========================================================================
void CEdit_Course::ChangeLevel()
{
	// ���݂̃u���b�N���
	CListManager<CMapBlockInfo> BlockInfoList = CMapBlock::GetInfoList();
	CMapBlockInfo* pBlockInfo = BlockInfoList.GetData(m_nCourseEditIdx);
	int level = pBlockInfo->GetLevel();

	if (ImGui::SliderInt("Course Level", &level, 0, 2))
	{

	}

	// ���x���ݒ�
	pBlockInfo->SetLevel(level);
}

//==========================================================================
// �ҏW����R�[�X�ύX
//==========================================================================
void CEdit_Course::ChangeEditCourse()
{
	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	// �R�[�X�}�l�[�W���擾
	CCourseManager* pCourceManager = CCourseManager::GetInstance();
	if (pCourceManager == nullptr) return;

	// ��_�̐�
	int segmentSize = pCourceManager->GetSegmentSize() - 1;
	if (ImGui::SliderInt("Course Edit Idx", &m_nCourseEditIdx, 0, segmentSize))
	{
		// ��Q�����Z�b�g
		ResetObstacle();

		// �}�b�v���Z�b�g
		ResetMap();

		// �����⃊�Z�b�g
		ResetWaterStone();

		std::vector<MyLib::Vector3> vecpos = pCourceManager->GetSegmentPos(m_nCourseEditIdx);
		vecpos.insert(vecpos.begin(), MyLib::Vector3(-10.0f, 0.0f, 0.0f));
		vecpos.push_back(vecpos.back() + MyLib::Vector3(10.0f, 0.0f, 0.0f));

		pCourse->SetVecPosition(vecpos);
		pCourse->ReCreateVtx();


		// �Ί_�Đ���
		{
			CStoneWall* pStoneWall = pCourceManager->GetStoneWall();

			// ��_�n�_�ݒ�
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

			// �e���_���W
			pStoneWall->SetVecVtxPosition(vecpos);
			pStoneWall->BindVtxPosition();
		}


		// �Ί_�Đ���
		{
			CStoneWall* pStoneWall = pCourceManager->GetStoneWall_Front();

			// ��_�n�_�ݒ�
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

			// �e���_���W
			pStoneWall->SetVecVtxPosition(vecpos);
			pStoneWall->BindVtxPosition();
		}



		m_nCheckPointEditIdx = 0;	// �`�F�b�N�|�C���g�̃C���f�b�N�X���Z�b�g
	}


	// �����ύX
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

		// �������ă��X�g�̊Ǘ�����
		CMapBlockInfo* pBlock = DEBUG_NEW CMapBlockInfo;
		pBlock->Init();
		CMapBlock::GetInfoListPtr()->Regist(pBlock);
	}
	ImGui::SameLine();

	// �T�C�Y
	segmentSize = pCourceManager->GetSegmentSize();
	ImGui::Text("%d", segmentSize);

}

//==========================================================================
// �`�F�b�N�|�C���g�ҏW
//==========================================================================
void CEdit_Course::TransCheckPoint()
{
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	if (ImGui::TreeNode("CheckPoint"))
	{
		CCourse* pCourse = CGame::GetInstance()->GetCourse();
		if (pCourse == nullptr) return;

		// �R�[�X�}�l�[�W���擾
		CCourseManager* pCourceManager = CCourseManager::GetInstance();
		if (pCourceManager == nullptr) return;

		float width = 150.0f;

		// �`�F�b�N�|�C���g�̃��X�g�擾
		std::vector<float> vecCheckpoint = CMapBlock::GetInfoList().GetData(m_nCourseEditIdx)->GetCheckpointInfo();
		int checkpointSize = vecCheckpoint.size() - 1;

		// �����ύX
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

		// �T�C�Y
		ImGui::Text("%d", checkpointSize + 1);


		// ��O����
		if (vecCheckpoint.empty())
		{
			ImGui::TreePop();
			return;
		}


		// ���삷��Ώې؂�ւ�
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

		// �`�F�b�N�|�C���g�̃��X�g�ݒ�
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
// �W���b�W�ҏW
//==========================================================================
void CEdit_Course::TransJudge()
{
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	if (ImGui::TreeNode("Judge"))
	{
		CCourse* pCourse = CGame::GetInstance()->GetCourse();
		if (pCourse == nullptr) return;

		// �R�[�X�}�l�[�W���擾
		CCourseManager* pCourceManager = CCourseManager::GetInstance();
		if (pCourceManager == nullptr) return;

		float width = 150.0f;

		// �`�F�b�N�|�C���g�̃��X�g�擾
		std::vector<std::vector<CMapBlockInfo::SJudgeInfo>> vecJudge = CMapBlock::GetInfoList().GetData(m_nCourseEditIdx)->GetJudgeInfo();
		int judgeSize = vecJudge.size() - 1;

		//=============================
		// �����ύX
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

			// �T�C�Y
			ImGui::Text("%d", judgeSize + 1);
		}

		// ��O����
		if (vecJudge.empty())
		{
			ImGui::TreePop();
			return;
		}

		//=============================
		// ���e�������ύX
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

			// �T�C�Y
			ImGui::Text("%d", objSize + 1);
		}


		// ���삷��Ώې؂�ւ�
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

		// �`�F�b�N�|�C���g�̃��X�g�ݒ�
		CMapBlock::GetInfoList().GetData(m_nCourseEditIdx)->SetJudgeInfo(vecJudge);

		ImGui::TreePop();
	}
}

//==========================================================================
// ��Q���ҏW
//==========================================================================
void CEdit_Course::TransObstacle()
{
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	if (ImGui::TreeNode("Obstacle"))
	{
		CCourse* pCourse = CGame::GetInstance()->GetCourse();
		if (pCourse == nullptr) return;

		// �R�[�X�}�l�[�W���擾
		CCourseManager* pCourceManager = CCourseManager::GetInstance();
		if (pCourceManager == nullptr) return;

		// �Z�[�u
		float width = 150.0f;
		ImGui::SetNextItemWidth(width);
		if (ImGui::Button("Save"))
		{
			// �ǉ�
		}
		ImGui::SameLine();


		// ��Q���̃��X�g�擾
		CListManager<CMap_Obstacle> obstacleList = CMapBlock::GetList().GetData(m_nCourseEditIdx)->GetObstacleList();
		int checkpointSize = obstacleList.GetNumAll() - 1;

		ImGui::SetNextItemWidth(width);
		if (ImGui::SliderInt("Obstacle Edit Idx", &m_nObstacleEditIdx, 0, checkpointSize))
		{

		}

		// ���̈ʒu�擾
		/*CCheckpoint* pCheckPoint = checkpointList.GetData(m_nCheckPointEditIdx);
		float length = pCheckPoint->GetLength();*/
		MyLib::Vector3 pos;

		ImGui::DragFloat3("pos", (float*)&pos, 1.0f, 0.0f, 0.0f, "%.2f");

		// �ݒ�


		ImGui::TreePop();
	}
}

//==========================================================================
// �ӂ̐��ύX
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
// ���[�h�ύX
//==========================================================================
void CEdit_Course::ChangeMode()
{
	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	if (ImGui::Checkbox("Enable SetMode!!!", &m_bSetMode))
	{
		// ���씻�胊�Z�b�g
		m_bEdit = false;
	}

	// ��������
	ImGui::Checkbox("Enable AutoCreateMode!!!", &m_bAutoCreateMode);
}

//==========================================================================
// ���C���I��
//==========================================================================
void CEdit_Course::SelectLine()
{
	if (m_bSetMode) return;	// �Z�b�g���[�h�͏I���

	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	// �ӏ��擾
	std::vector<MyLib::Vector3> vecSegmentPos = pCourse->GetVecPosition();
	MyLib::Vector3 coursepos = pCourse->GetPosition();

	// �}�E�X���
	CInputMouse* pMouse = CInputMouse::GetInstance();
	MyLib::Vector3 mouseRay = pMouse->GetRay();
	MyLib::Vector3 mousePos = pMouse->GetNearPosition();

	MyLib::AABB aabb = MyLib::AABB(-25.0f, 25.0f);
	float time = 0.0f;
	MyLib::Matrix mtx, mtxTrans;


	if (!m_bHoverWindow &&
		!m_bDrag &&
		pMouse->GetTrigger(CInputMouse::BUTTON::BUTTON_LEFT))
	{// ���N���b�N��

		// ���씻�胊�Z�b�g
		m_bEdit = false;

		int i = 0;
		for (const auto& vtxpos : vecSegmentPos)
		{
			if (i == 0 || i == 1 ||
				i == vecSegmentPos.size() - 2 || i == vecSegmentPos.size() - 1)
			{// �ŏ��ƍŌ�͑I�ׂȂ�
				i++;
				continue;
			}
			// �}�g���b�N�X������
			mtx.Identity();
			mtxTrans.Identity();

			// �ʒu��񔽉f
			MyLib::Vector3 transpos = vtxpos + coursepos;
			mtxTrans.Translation(transpos);
			mtx.Multiply(mtx, mtxTrans);

			MyLib::Vector3 OBpos;

			if (UtilFunc::Collision::CollisionRayAABB(&mousePos, &mouseRay, &aabb, &mtx, time, &OBpos))
			{// �d�Ȃ� && ���N���b�N

				// ���삷��ӕۑ�
				m_nVtxEditIdx = i;

				m_bEdit = true;
				break;
			}
			i++;
		}

		// �F���Z�b�g
		for (int i = 0; i < vecSegmentPos.size(); i++)
		{
			CCollisionLine_Box* pBox = pCourse->GetCollisionLineBox(i);
			if (pBox == nullptr) continue;

			pBox->SetColor(DEFAULT_COLOR);
		}
	}
}

//==========================================================================
// ���C���͂�
//==========================================================================
void CEdit_Course::DragLine()
{
	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	if (!m_bEdit) return;
	if (m_bSetMode) return;	// �Z�b�g���[�h�͏I���

	// �R�[�X�}�l�[�W���擾
	CCourseManager* pCourceManager = CCourseManager::GetInstance();
	if (pCourceManager == nullptr) return;


	// �Z�O�����g�ʒu
	std::vector<MyLib::Vector3> vecsegmentPos = pCourse->GetVecPosition();

	// �ӏ��擾
	MyLib::Vector3 segmentPos = vecsegmentPos[m_nVtxEditIdx];
	MyLib::Vector3 coursepos = pCourse->GetPosition();



	// �}�E�X���
	CInputMouse* pMouse = CInputMouse::GetInstance();
	MyLib::Vector3 mouseRay = pMouse->GetRay();
	MyLib::Vector3 mousePos = pMouse->GetNearPosition();

	MyLib::AABB aabb = MyLib::AABB(-50.0f, 50.0f);
	float time = 0.0f;
	MyLib::Matrix mtx, mtxTrans;


	if (!m_bHoverWindow &&
		pMouse->GetPress(CInputMouse::BUTTON::BUTTON_LEFT))
	{// ���������ݎ�

		// �}�g���b�N�X������
		mtx.Identity();
		mtxTrans.Identity();

		// �ʒu��񔽉f
		MyLib::Vector3 transpos = segmentPos + coursepos;
		mtxTrans.Translation(transpos);
		mtx.Multiply(mtx, mtxTrans);

		MyLib::Vector3 OBpos;

		if (UtilFunc::Collision::CollisionRayAABB(&mousePos, &mouseRay, &aabb, &mtx, time, &OBpos))
		{// �d�Ȃ�
			m_bDrag = true;
		}

	}

	if (pMouse->GetRelease(CInputMouse::BUTTON::BUTTON_LEFT))
	{
		m_bDrag = false;
	}

	// �h���b�O��
	if (m_bDrag)
	{
		CCamera* pCamera = CManager::GetInstance()->GetCamera();
		if (pCamera == nullptr) return;

		// �Ĉړ���
		MyLib::Vector3 diffpos = pMouse->GetWorldDiffPosition();
		segmentPos.x += diffpos.x;
		segmentPos.z += diffpos.z;

		if (m_bAutoCreateMode)
		{
			pCourse->ReCreateVtx();
		}
	}

	// �Z�O�����g�ʒu
	vecsegmentPos[m_nVtxEditIdx] = segmentPos;

	// �R�[�X�̈ʒu
	pCourse->SetVecPosition(vecsegmentPos);

	pCourceManager->SetSegmentPos(vecsegmentPos, m_nCourseEditIdx);
	pCourse->GetCollisionLineBox(m_nVtxEditIdx)->SetPosition(segmentPos);
}

//==========================================================================
// �g�����X�t�H�[��
//==========================================================================
void CEdit_Course::Transform()
{
	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	// �ӏ��擾
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

	// ���삷��ӂ̏��
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

		// ���Z�b�g
		if (ImGui::Button("RESET")) {
			editpos = 0.0f;
		}
		

		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		//=============================
		// �ʒu�ݒ�
		//=============================
		ImGui::Text("pos");
		ImGui::SameLine();

		// X
		ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("x", &editpos.x, POS_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Y
		ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("y", &editpos.y, POS_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Z
		ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("z", &editpos.z, POS_MOVE, 0.0f, 0.0f, "%.2f");
		}
		ImGui::PopID();


		////=============================
		//// �����ݒ�
		////=============================
		//ImGui::Text("rot");
		//ImGui::SameLine();

		//// X
		//ImGui::PushID(1); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		//{
		//	ImGui::SetNextItemWidth(windowWidth);
		//	ImGui::DragFloat("x", &editInfo.rot.x, ROT_MOVE, 0.0f, 0.0f, "%.2f");
		//	ImGui::SameLine();
		//}
		//ImGui::PopID();

		//// Y
		//ImGui::PushID(1); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		//{
		//	ImGui::SetNextItemWidth(windowWidth);
		//	ImGui::DragFloat("y", &editInfo.rot.y, ROT_MOVE, 0.0f, 0.0f, "%.2f");
		//	ImGui::SameLine();
		//}
		//ImGui::PopID();

		//// Z
		//ImGui::PushID(1); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		//{
		//	ImGui::SetNextItemWidth(windowWidth);
		//	ImGui::DragFloat("z", &editInfo.rot.z, ROT_MOVE, 0.0f, 0.0f, "%.2f");
		//}
		//ImGui::PopID();




		////=============================
		//// ���ݒ�
		////=============================
		//ImGui::Text("width");
		//ImGui::SameLine();

		//// X
		//ImGui::PushID(2); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		//{
		//	ImGui::SetNextItemWidth(windowWidth);
		//	ImGui::DragFloat("width", &editInfo.width, POS_MOVE, 0.0f, 0.0f, "%.2f");
		//	ImGui::SameLine();
		//}
		//ImGui::PopID();


		ImGui::TreePop();
	}

	// ���C�����ݒ�
	pCourse->SetVecPosition(m_nVtxEditIdx, editpos);
}

//==========================================================================
// �n�_�ǉ�
//==========================================================================
void CEdit_Course::AddPoint()
{
	if (!m_bSetMode) return;	// �Z�b�g���[�h�ȊO�͏I���

	ImGui::Text("Push Mouse Button!!!");

	// �}�E�X���
	CInputMouse* pMouse = CInputMouse::GetInstance();
	MyLib::Vector3 mouseRay = pMouse->GetRay();
	MyLib::Vector3 mousePos = pMouse->GetNearPosition();

	// �J�������
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	MyLib::Vector3 pos = UtilFunc::Transformation::CalcScreenToXZ(
		pMouse->GetPosition(),
		D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT),
		pCamera->GetMtxView(),
		pCamera->GetMtxProjection());

	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	// ��_���擾
	std::vector<MyLib::Vector3> vecSegmentPos = pCourse->GetVecPosition();

	if (!m_bHoverWindow &&
		!CInputKeyboard::GetInstance()->GetPress(DIK_LALT) &&
		pMouse->GetTrigger(CInputMouse::BUTTON::BUTTON_LEFT))
	{

		int endIdx = (vecSegmentPos.size() - 1);
		vecSegmentPos.insert(vecSegmentPos.begin() + endIdx, pos);

		// ��_���ݒ�
		pCourse->SetVecPosition(vecSegmentPos);
		pCourse->ReCreateVtx();
	}
}

//==========================================================================
// ��Q���G�f�B�b�g
//==========================================================================
void CEdit_Course::ObstacleEdit()
{
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	if (ImGui::TreeNode("ObstacleEdit"))
	{
		// ��Q���G�f�B�b�g�X�V
		m_pEditObstacle->Update();

		ImGui::TreePop();
	}
}

//==========================================================================
// ��Q�����Z�b�g
//==========================================================================
void CEdit_Course::ResetObstacle()
{

	// ��Q���}�l�[�W���擾
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();

	// ��Q���̃��X�g�擾
	CListManager<CMap_Obstacle> list = CMap_Obstacle::GetListObj();

	// �擪��ۑ�
	std::list<CMap_Obstacle*>::iterator itr = list.GetEnd();
	CMap_Obstacle* pObj = nullptr;

	while (list.ListLoop(itr))
	{
		CMap_Obstacle* pObj = *itr;
		pObj->Kill();
	}

	// ��Q���̃��X�g�擾
	std::vector<CMapBlockInfo::SObsacleInfo> obstacleInfo = CMapBlock::GetInfoList().GetData(m_nCourseEditIdx)->GetObstacleInfo();


	// ��Q�����
	std::vector<CMap_ObstacleManager::SObstacleInfo> vecInfo = pObstacleMgr->GetObstacleInfo();
	CMap_ObstacleManager::SObstacleInfo info;
	for (int i = 0; i < static_cast<int>(obstacleInfo.size()); i++)
	{
		// �u���b�N�̏�Q�����
		CMapBlockInfo::SObsacleInfo blockinfo = obstacleInfo[i];

		info = vecInfo[blockinfo.nType];

		CMap_Obstacle* pObj = CMap_Obstacle::Create(info, blockinfo.pos);
		pObj->SetRotation(blockinfo.rot);
		pObj->SetScale(blockinfo.scale);
		pObj->CalWorldMtx();
	}

}

//==========================================================================
// ��Q���Z�[�u
//==========================================================================
void CEdit_Course::SaveObstacle()
{
	
	// ��Q���̃��X�g�擾
	CListManager<CMap_Obstacle> list = CMap_Obstacle::GetListObj();

	// �擪��ۑ�
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

		// ��Q���}�l�[�W���擾
		CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();
		std::vector<CMap_ObstacleManager::SObstacleInfo> vecObstacleMgrInfo = pObstacleMgr->GetObstacleInfo();

		// ��Q�����擾
		MyLib::Vector3 pos = pObj->GetPosition(), rot = pObj->GetRotation(), scale = pObj->GetScale();
		CMap_ObstacleManager::SObstacleInfo info = pObj->GetObstacleInfo();
		std::string text = info.textFile;

		// ��ފ���o��
		const auto& obItr = std::find_if(vecObstacleMgrInfo.begin(), vecObstacleMgrInfo.end(),
			[&text](const CMap_ObstacleManager::SObstacleInfo& string) {return string.textFile == text; });
		int type = (obItr - vecObstacleMgrInfo.begin());


		// �Z�[�u���ǉ�
		savedate.push_back(CMapBlockInfo::SObsacleInfo(pos, rot, scale, type));
	}

	// ��Q���̃��X�g�ݒ�
	CMapBlock::GetInfoList().GetData(m_nCourseEditIdx)->SetObstacleInfo(savedate);
}

//==========================================================================
// �}�b�v�G�f�B�b�g
//==========================================================================
void CEdit_Course::MapEdit()
{
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	if (ImGui::TreeNode("MapEdit"))
	{
		// ��Q���G�f�B�b�g�X�V
		m_pEditMap->Update();

		ImGui::TreePop();
	}
}

//==========================================================================
// �}�b�v���Z�b�g
//==========================================================================
void CEdit_Course::ResetMap()
{
	// ��Q���̃��X�g�擾
	std::vector<CObjectX*> objinfo = CFrontObjManager::GetInstance()->GetInfo();
	std::vector<CMapBlockInfo::SObsacleInfo> savedate;

	for (int i = 0; i < objinfo.size(); i++)
	{
		objinfo[i]->Kill();
	}

	// ��Q���̃��X�g�擾
	std::vector<CMapBlockInfo::SObsacleInfo> mapInfo = CMapBlock::GetInfoList().GetData(m_nCourseEditIdx)->GetMapInfo();
	CFrontObjManager::GetInstance()->Reset();

	for (int i = 0; i < static_cast<int>(mapInfo.size()); i++)
	{
		// �u���b�N�̏�Q�����
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
// �}�b�v�Z�[�u
//==========================================================================
void CEdit_Course::SaveMap()
{
	// ��Q���̃��X�g�擾
	std::vector<CObjectX*> objinfo = CFrontObjManager::GetInstance()->GetInfo();
	std::vector<CMapBlockInfo::SObsacleInfo> savedate;

	for(int i = 0; i < objinfo.size(); i++)
	{

		CMapBlockInfo::SObsacleInfo info = CMapBlockInfo::SObsacleInfo(objinfo[i]->GetPosition(), objinfo[i]->GetRotation(), objinfo[i]->GetScale(), objinfo[i]->GetIdxXFile());

		// �Z�[�u���ǉ�
		savedate.push_back(info);
	}

	// ��Q���̃��X�g�ݒ�
	CMapBlock::GetInfoList().GetData(m_nCourseEditIdx)->SetMapInfo(savedate);
}

//==========================================================================
// ������G�f�B�b�g
//==========================================================================
void CEdit_Course::WaterStoneEdit()
{
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	if (ImGui::TreeNode("WaterStoneEdit"))
	{
		// ������G�f�B�b�g�X�V
		m_pEditWaterStone->Update();

		ImGui::TreePop();
	}
}

//==========================================================================
// �����⃊�Z�b�g
//==========================================================================
void CEdit_Course::ResetWaterStone()
{
	// ��Q���}�l�[�W���擾
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();

	// ������̃��X�g�擾
	CListManager<CWaterStone> list = CWaterStone::GetListObj();

	// �擪��ۑ�
	std::list<CWaterStone*>::iterator itr = list.GetEnd();
	CMap_Obstacle* pObj = nullptr;

	while (list.ListLoop(itr))
	{
		CWaterStone* pObj = *itr;
		pObj->Kill();
	}

	// ��Q���̃��X�g�擾
	std::vector<CWaterStone_Manager::SStoneInfo> obstacleInfo = CMapBlock::GetInfoList().GetData(m_nCourseEditIdx)->GetWaterStoneInfo();

	// ��Q�����
	for (const auto& info : obstacleInfo)
	{
		CWaterStone* pObj = CWaterStone::Create(info);
		pObj->CalWorldMtx();
	}
}

//==========================================================================
// ������Z�[�u
//==========================================================================
void CEdit_Course::SaveWaterStone()
{
	// ������̃��X�g�擾
	CListManager<CWaterStone> list = CWaterStone::GetListObj();

	// �擪��ۑ�
	std::list<CWaterStone*>::iterator itr = list.GetEnd();
	CWaterStone* pObj = nullptr;

	std::vector<CWaterStone_Manager::SStoneInfo> savedate;

	while (list.ListLoop(itr))
	{
		CWaterStone* pObj = *itr;

		// ��Q���}�l�[�W���擾
		CWaterStone_Manager::SStoneInfo info = pObj->GetStoneInfo();

		// �Z�[�u���ǉ�
		savedate.push_back(info);
	}

	// ��Q���̃��X�g�ݒ�
	CMapBlock::GetInfoList().GetData(m_nCourseEditIdx)->SetWaterStoneInfo(savedate);
}

//==========================================================================
// �ŏ��ƍŌ�ό`
//==========================================================================
void CEdit_Course::TransformBeginEnd()
{
	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	// �ӏ��擾
	std::vector<MyLib::Vector3> segmentPos = pCourse->GetVecPosition();

	// �ŏ��ƍŌ�A�t�����ɏ����o��
	MyLib::Vector3 begin, end;
	float angle = 0.0f;

	// �ŏ�
	angle = segmentPos[2].AngleXZ(segmentPos[1]);
	begin = MyLib::Vector3(
		segmentPos[1].x + sinf(angle) * -10.0f,
		segmentPos[1].y,
		segmentPos[1].z + cosf(angle) * -10.0f);

	// �Ō�
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
// �ŏ��ƍŌ�ό`
//==========================================================================
void CEdit_Course::TransformBeginEnd(std::vector<MyLib::Vector3>* pVecpos)
{

	// �ӏ��擾
	std::vector<MyLib::Vector3> segmentPos = *pVecpos;

	// �ŏ��ƍŌ�A�t�����ɏ����o��
	MyLib::Vector3 begin, end;
	float angle = 0.0f;

	// �ŏ�
	angle = segmentPos[2].AngleXZ(segmentPos[1]);
	begin = MyLib::Vector3(
		segmentPos[1].x + sinf(angle) * -10.0f,
		segmentPos[1].y,
		segmentPos[1].z + cosf(angle) * -10.0f);

	// �Ō�
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

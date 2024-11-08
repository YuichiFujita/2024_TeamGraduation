//=============================================================================
// 
// �J�������[�V�������� [camera_motion.cpp]
// Author : ���n�Ή�
// Adder  : ���c�E��
// 
//=============================================================================
#include "camera_motion.h"
#include "camera.h"
#include "debugproc.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "calculation.h"
#include "particle.h"

//==========================================================================
// �}�N����`
//==========================================================================
namespace
{
	const std::string FILENAME = "data\\TEXT\\camera\\motion_manager.txt";	// �ǂݍ��ރt�@�C����
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
// ���[�V�������X�g
CCameraMotion::MOTION_FUNC CCameraMotion::m_MotionFunc[] =
{
	&CCameraMotion::MotionPass,			// �p�X
	&CCameraMotion::MotionSpecial,		// �X�y�V����
	&CCameraMotion::MotionKamehameha,	// ���߂͂ߔg
};

// vec3���`�␳���X�g
CCameraMotion::VEC3_EASING_FUNC CCameraMotion::m_Vec3EasingFunc[] =
{
	&UtilFunc::Correction::EasingLinear,
	&UtilFunc::Correction::EasingEaseIn,
	&UtilFunc::Correction::EasingEaseOut,
	&UtilFunc::Correction::EasingEaseInOut,
};

// float���`�␳���X�g
CCameraMotion::FLOAT_EASING_FUNC CCameraMotion::m_FloatEasingFunc[] =
{
	&UtilFunc::Correction::EasingLinear,
	&UtilFunc::Correction::EasingEaseIn,
	&UtilFunc::Correction::EasingEaseOut,
	&UtilFunc::Correction::EasingEaseInOut,
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCameraMotion::CCameraMotion()
{
	m_vecMotionInfo.clear();	// ���[�V�������
	m_MotionFileName.clear();	// ���[�V�����t�@�C����
	m_pos = 0.0f;				// �ʒu
	m_nNowMotionIdx = 0;		// ���݂̃��[�V�����C���f�b�N�X
	m_nNowKeyIdx = 0;			// ���݂̃L�[�C���f�b�N�X
	m_nNowTriggerIdx = 0;		// ���݂̃g���K�[�C���f�b�N�X
	m_fMotionTimer = 0.0f;		// ���[�V�����^�C�}�[
	m_fTriggerTimer = 0.0f;		// �g���K�[�^�C�}�[
	m_bFinish = false;			// �I������
	m_bEdit = false;			// �G�f�B�^�[�t���O
	m_bPause = false;			// �|�[�Y����
	m_bSystemPause = false;		// �V�X�e���|�[�Y����
	m_bMovePos = true;			// �ʒu����t���O
	m_bMoveRot = true;			// ��������t���O
	m_bMoveDis = true;			// ��������t���O
	m_bInverse = false;			// ���]�t���O
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCameraMotion::~CCameraMotion()
{

}

//==========================================================================
// ��������
//==========================================================================
CCameraMotion* CCameraMotion::Create()
{
	// �������̊m��
	CCameraMotion* pObj = DEBUG_NEW CCameraMotion;

	if (pObj == nullptr)
	{
		return nullptr;
	}

	// ����������
	pObj->Init();

	return pObj;
}

//==========================================================================
// �J�����̏���������
//==========================================================================
HRESULT CCameraMotion::Init()
{
	// �e��ϐ�������
	m_bFinish = true;	// �I������

	// �e�L�X�g�ǂݍ���
	LoadText();

	m_EditInfo.motionInfo = m_vecMotionInfo[0];

	for (int i = 0; i < MOTION_MAX; i++)
	{
		m_pCameraMotion_Trigger[i] = CCameraMotion_Trigger::Create(i);
	}

	return S_OK;
}

//==========================================================================
// �e�L�X�g�ǂݍ���
//==========================================================================
void CCameraMotion::LoadText()
{
	// �t�@�C�����J��
	std::ifstream File(FILENAME);
	if (!File.is_open()) {
		return;
	}

	// �R�����g�p
	std::string hoge;

	// �t�@�C����
	std::string pathname, filename;

	// ���[�V�����t�@�C�������Z�b�g
	m_MotionFileName.clear();
	m_PathName.clear();	// �p�X��

	// �f�[�^�ǂݍ���
	std::string line;
	while (std::getline(File, line))
	{
		// �R�����g�̓X�L�b�v
		if (line.empty() ||
			line[0] == '#')
		{
			continue;
		}

		// �X�g���[���쐬
		std::istringstream lineStream(line);

		if (line.find("PATH") != std::string::npos)
		{// PATH�œǂݍ��ރp�X�ǂݍ���

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>		// ��
				m_PathName;	// �p�X��

			continue;
		}

		if (line.find("MOTION_FILENAME") != std::string::npos)
		{// MOTION_FILENAME�Ń��[�V�����t�@�C���ǂݍ���

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>		// ��
				filename;	// ���[�V�����t�@�C����

			// ���[�V�����t�@�C�����ǉ�
			m_MotionFileName.push_back(filename);

			// ���[�V�����ǂݍ���
			LoadMotion(filename);

			continue;
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	// �t�@�C�������
	File.close();
}

//==========================================================================
// ���[�V������������
//==========================================================================
void CCameraMotion::SaveMotion(const std::string& filename, const MotionInfo& info)
{
	// �t�@�C�����J��
	std::ofstream File((m_PathName + filename), std::ios::binary);
	if (!File.is_open()) {
		return;
	}

	// �R���e�i�ȊO�̃Z�[�u
	//File.write(reinterpret_cast<const char*>(&info.playTime), sizeof(info.playTime));

	// �f�[�^�̌����v�Z
	size_t vecSize = info.Key.size();

	// �R���e�i�̃T�C�Y���Z�[�u
	File.write(reinterpret_cast<const char*>(&vecSize), sizeof(vecSize));

	// �R���e�i���̗v�f���Z�[�u
	File.write(reinterpret_cast<const char*>(info.Key.data()), vecSize * sizeof(MotionKey));




#if 1
	// �g���K�[�̃Z�[�u
	{
		//MotionInfo_Save info_Save;

		// �f�[�^�̌����v�Z
		size_t vecSize = info.trigger.size();

		// �R���e�i�̃T�C�Y���Z�[�u
		File.write(reinterpret_cast<const char*>(&vecSize), sizeof(vecSize));

		// �R���e�i���̗v�f���Z�[�u
		File.write(reinterpret_cast<const char*>(info.trigger.data()), vecSize * sizeof(float));
	}
#endif





	// �t�@�C�������
	File.close();
}

//==========================================================================
// ���[�V�����ǂݍ���
//==========================================================================
void CCameraMotion::LoadMotion(const std::string& filename)
{
	// �t�@�C�����J��
	std::ifstream File((m_PathName + filename), std::ios::binary);
	if (!File.is_open()) 
	{
		// ��O����
		m_vecMotionInfo.push_back(MotionInfo());
		m_vecMotionInfo.back().Key.push_back(MotionKey());
		return;
	}

	MotionInfo loadData;	// �ǂݍ��݃f�[�^

	// �R���e�i�ȊO�̃��[�h
	//File.read(reinterpret_cast<char*>(&loadData.playTime), sizeof(loadData.playTime));

	// �L�[�̃T�C�Y�����[�h
	size_t size;
	File.read(reinterpret_cast<char*>(&size), sizeof(size));

	// �T�C�Y���m��
	loadData.Key.resize(size);

	// �L�[�f�[�^���[�h
	File.read(reinterpret_cast<char*>(loadData.Key.data()), size * sizeof(MotionKey));


	// �g���K�[�̃��[�h
	{
		// �g���K�[�̃T�C�Y�����[�h
		File.read(reinterpret_cast<char*>(&size), sizeof(size));

		// �T�C�Y���m��
		loadData.trigger.resize(size);

		// �L�[�f�[�^���[�h
		File.read(reinterpret_cast<char*>(loadData.trigger.data()), size * sizeof(float));
	}


	// �t�@�C�������
	File.close();


	// ���[�V�������ǉ�
	m_vecMotionInfo.push_back(loadData);
}

//==========================================================================
// �J�����̏I������
//==========================================================================
void CCameraMotion::Uninit()
{
	for (int i = 0; i < MOTION_MAX; i++)
	{
		if (m_pCameraMotion_Trigger[i] == nullptr)
		{
			continue;
		}

		delete m_pCameraMotion_Trigger[i];
		m_pCameraMotion_Trigger[i] = nullptr;
	}

	m_vecMotionInfo.clear();
	m_MotionFileName.clear();

	delete this;
}

//==========================================================================
// �J�����̍X�V����
//==========================================================================
void CCameraMotion::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
#ifdef _DEBUG
	// �G�f�B�b�g�X�V
	UpdateEdit();
#endif // DEBUG

	// �J�����̃��[�V�������t���O�𔽉f
	CCamera* pCamera = GET_MANAGER->GetCamera();	// �J�������
	pCamera->SetEnableMotion(!m_bFinish);			// �t���O���f

	// ���[�V�������I�����Ă���ꍇ������
	if (m_bFinish) { return; }

	// ���݂̃��[�V���������擾
	MotionInfo nowInfo = m_vecMotionInfo[m_nNowMotionIdx];	// ���݂̃��[�V����
	int keySize = static_cast<int>(nowInfo.Key.size());		// ���݂̃L�[����

	if (!m_bPause && !m_bSystemPause)
	{ // �Đ��\�ȏꍇ

		// ���[�V�����^�C�}�[���Z
		m_fMotionTimer  += fDeltaTime * fSlowRate;
		m_fTriggerTimer += fDeltaTime * fSlowRate;
	}
	else
	{ // �Đ��s�\�ȏꍇ

		// �g���K�[���胊�Z�b�g
		m_bTrigger = false;
		return;
	}

	//=============================
	// �g���K�[����
	//=============================
	{
		// �g���K�[���胊�Z�b�g
		m_bTrigger = false;

		if (!m_bTrigger
		&&  m_nNowTriggerIdx < static_cast<int>(nowInfo.trigger.size())	// �g���K�[�̃T�C�Y�ȉ�
		&&  m_fTriggerTimer >= nowInfo.trigger[m_nNowTriggerIdx])
		{
			// �g���K�[������
			TriggerMoment();
			
			// �g���K�[����ON
			m_bTrigger = true;
			m_nNowTriggerIdx++;

#if _DEBUG
			// �g���K�[�^�C�~���O�Ȃ�p�[�e�B�N������
			my_particle::Create(MyLib::Vector3(640.0f, 360.0f, 0.0f), my_particle::TYPE_OFFSETTING_2D);
#endif
		}
	}

	//=============================
	// �L�[�X�V
	//=============================
	if (m_fMotionTimer >= nowInfo.Key[m_nNowKeyIdx].playTime)
	{
		// �L�[�X�V
		m_fMotionTimer = 0.0f;
		m_nNowKeyIdx = (m_nNowKeyIdx + 1) % keySize;

		if (m_nNowKeyIdx == 0)
		{ // ���

			// �I������ON
			m_bFinish = true;
			pCamera->SetEnableMotion(false);

			// �g���K�[���Z�b�g
			m_nNowTriggerIdx = 0;
			m_fTriggerTimer = 0.0f;
			return;
		}
	}

	// ���̃L�[�C���f�b�N�X
	int nextKeyID = (m_nNowKeyIdx + 1) % keySize;
	if (nextKeyID == 0)
	{ // �I�[

		nextKeyID = keySize - 1;
	}

	//=============================
	// ���[�V�����X�V
	//=============================
	MotionKey nowKey  = nowInfo.Key[m_nNowKeyIdx];	// ���݂̃L�[
	MotionKey nextKey = nowInfo.Key[nextKeyID];		// ���̃L�[

	if (m_bMovePos)
	{ // �����_�����삷��ꍇ

		// �����_�̐��`�␳
		MyLib::Vector3 posR = pCamera->GetPositionR();	// �����_
		posR = (m_Vec3EasingFunc[m_EasingType])(nowKey.posRDest, nextKey.posRDest, 0.0f, nowKey.playTime, m_fMotionTimer);
		pCamera->SetPositionR(m_pos + posR);	// �����_���f
	}

	if (m_bMoveRot)
	{ // ���������삷��ꍇ

		// �����̐��`�␳
		MyLib::Vector3 rot = pCamera->GetRotation();	// ����
		rot = (m_Vec3EasingFunc[m_EasingType])(nowKey.rotDest, nextKey.rotDest, 0.0f, nowKey.playTime, m_fMotionTimer);
		pCamera->SetRotation(rot);	// �������f
	}

	if (m_bMoveDis)
	{ // ���������삷��ꍇ

		// �����̐��`�␳
		float fDistance = pCamera->GetDistance();	// ����
		fDistance = (m_FloatEasingFunc[m_EasingType])(nowKey.distance, nextKey.distance, 0.0f, nowKey.playTime, m_fMotionTimer);
		pCamera->SetDistance(fDistance);	// �������f
	}

	if (m_bInverse)
	{ // ���]����ꍇ

		// ������Y���ɔ��]������
		MyLib::Vector3 rot = pCamera->GetRotation();	// ����
		rot.y *= -1.0f;

		// �����𐳋K��
		UtilFunc::Transformation::RotNormalize(rot.y);
		pCamera->SetRotation(rot);	// �������f
	}
}

//==========================================================================
// �g���K�[����擾
//==========================================================================
CCameraMotion::TriggerInfo CCameraMotion::GetTrigger()
{
	return TriggerInfo(m_bTrigger, m_nNowTriggerIdx);
}

//==========================================================================
// ���[�V�����ݒ�
//==========================================================================
void CCameraMotion::SetMotion
(
	int nMotion,	// ���[�V�������
	bool bInverse,	// ���]�t���O
	bool bPos,		// �ʒu����t���O
	bool bRot,		// ��������t���O
	bool bDis,		// ��������t���O
	EASING easing	// �C�[�W���O���
)
{
	// �������̐ݒ�
	m_EasingType	 = easing;
	m_nNowMotionIdx	 = nMotion;
	m_bInverse	= bInverse;
	m_bMovePos	= bPos;
	m_bMoveRot	= bRot;
	m_bMoveDis	= bDis;

	// ���̏�����
	m_nNowKeyIdx	 = 0;
	m_nNowTriggerIdx = 0;
	m_fMotionTimer	 = 0.0f;
	m_fTriggerTimer	 = 0.0f;
	m_bFinish	= false;
	m_bPause	= false;

	CCamera* pCamera = GET_MANAGER->GetCamera();			// �J�������
	MotionInfo nowInfo = m_vecMotionInfo[m_nNowMotionIdx];	// ���݂̃��[�V����
	int nKeySize = static_cast<int>(nowInfo.Key.size());	// ���݂̃L�[����
	int nNextKeyID = (m_nNowKeyIdx + 1) % nKeySize;	// ���̃L�[�C���f�b�N�X
	MotionKey nowKey = nowInfo.Key[m_nNowKeyIdx];	// ���݂̃L�[
	MotionKey nextKey = nowInfo.Key[nNextKeyID];	// ���̃L�[

	if (m_bMovePos)
	{ // �����_�����삷��ꍇ

		// �����_�̐��`�␳
		MyLib::Vector3 posR = pCamera->GetPositionR();	// �����_
		posR = (m_Vec3EasingFunc[m_EasingType])(nowKey.posRDest, nextKey.posRDest, 0.0f, nowKey.playTime, m_fMotionTimer);
		pCamera->SetPositionR(m_pos + posR);	// �����_���f
	}

	if (m_bMoveRot)
	{ // ���������삷��ꍇ

		// �����̐��`�␳
		MyLib::Vector3 rot = pCamera->GetRotation();	// ����
		rot = (m_Vec3EasingFunc[m_EasingType])(nowKey.rotDest, nextKey.rotDest, 0.0f, nowKey.playTime, m_fMotionTimer);
		pCamera->SetRotation(rot);	// �������f
	}

	if (m_bMoveDis)
	{ // ���������삷��ꍇ

		// �����̐��`�␳
		float fDistance = pCamera->GetDistance();	// ����
		fDistance = (m_FloatEasingFunc[m_EasingType])(nowKey.distance, nextKey.distance, 0.0f, nowKey.playTime, m_fMotionTimer);
		pCamera->SetDistance(fDistance);	// �������f
	}

	if (m_bInverse)
	{ // ���]����ꍇ

		// ������Y���ɔ��]������
		MyLib::Vector3 rot = pCamera->GetRotation();	// ����
		rot.y *= -1.0f;

		// �����𐳋K��
		UtilFunc::Transformation::RotNormalize(rot.y);
		pCamera->SetRotation(rot);	// �������f
	}
}

//==========================================================================
// �L�[���擾
//==========================================================================
CCameraMotion::MotionKey CCameraMotion::GetKeyData(const MOTION motion, const int nKeyIdx)
{
	// �����C���f�b�N�X�̃L�[����Ԃ�
	return m_vecMotionInfo[(int)motion].Key[nKeyIdx];
}

//==========================================================================
// ���݂̑S�̎��Ԏ擾
//==========================================================================
float CCameraMotion::GetWholeCurTimer()
{
	float fTimer = 0.0f;	// �S�̎���
	for (int i = 0; i < m_nNowKeyIdx; i++)
	{ // ���ݐi��ł���L�[�܂ŌJ��Ԃ�

		// �L�[�̑����Ԃ����Z
		fTimer += m_vecMotionInfo[m_nNowMotionIdx].Key[i].playTime;
	}

	// ���݃L�[�̃^�C�}�[�����Z
	fTimer += m_fMotionTimer;

	// �S�̎��Ԃ�Ԃ�
	return fTimer;
}

//==========================================================================
// �ő�̑S�̎��Ԏ擾
//==========================================================================
float CCameraMotion::GetWholeMaxTimer()
{
	float fTimer = 0.0f;	// �S�̎���
	for (int i = 0; i < m_vecMotionInfo[m_nNowMotionIdx].Key.size(); i++)
	{ // �L�[�̑������J��Ԃ�

		// �L�[�̑����Ԃ����Z
		fTimer += m_vecMotionInfo[m_nNowMotionIdx].Key[i].playTime;
	}

	// �S�̎��Ԃ�Ԃ�
	return fTimer;
}

//==========================================================================
// �g���K�[����̏u��
//==========================================================================
void CCameraMotion::TriggerMoment()
{
	// ��ԍX�V
	//(this->*(m_MotionFunc[m_nNowMotionIdx]))();

	if (m_pCameraMotion_Trigger[m_nNowMotionIdx] == nullptr)
	{
		return;
	}

	// �g���K�[�̏u��
	m_pCameraMotion_Trigger[m_nNowMotionIdx]->TriggerMoment(m_nNowTriggerIdx);
}

//==========================================================================
// �p�X
//==========================================================================
void CCameraMotion::MotionPass()
{

}

//==========================================================================
// �X�y�V����
//==========================================================================
void CCameraMotion::MotionSpecial()
{

}

//==========================================================================
// ���߂͂ߔg
//==========================================================================
void CCameraMotion::MotionKamehameha()
{

}

//==========================================================================
// �G�f�B�b�g�X�V
//==========================================================================
void CCameraMotion::UpdateEdit()
{
	if (ImGui::CollapsingHeader("CameraMotion"))
	{
		// �G�f�B�b�g������
		m_bEdit = true;

		// �Đ�
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::SetNextItemWidth(150.0f);
		if (ImGui::Button("Play / RePlay"))
		{
			SetMotion(m_EditInfo.motionIdx, false);
		}
		ImGui::SameLine();

		ImGui::SetNextItemWidth(150.0f);
		if (ImGui::Button("Reset"))
		{
			m_nNowKeyIdx = 0;
			m_fMotionTimer = 0.0f;
			m_bFinish = true;	// �I������
		}


		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		if (ImGui::Button("Save", ImVec2(80, 50)))
		{
			SaveMotion(m_MotionFileName[m_EditInfo.motionIdx], m_EditInfo.motionInfo);
		}
		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		// �X���C�h�Đ�
		SliderPlay();

		// �I�t�Z�b�g
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::SetNextItemWidth(150.0f);
		if (ImGui::Button("Offset"))
		{
			m_EditInfo.offset = CManager::GetInstance()->GetCamera()->GetPositionR();
			m_pos = m_EditInfo.offset;
		}
		ImGui::SameLine();
		ImGui::Text("x:%.2f y:%.2f z:%.2f", m_EditInfo.offset.x, m_EditInfo.offset.y, m_EditInfo.offset.z);

		// ���[�V�����؂�ւ�
		ChangeMotion();

		// �L�[�ؑ�
		ChangeKey();

		// �g���K�[�ҏW
		EditTrigger();

	}
	else m_bEdit = false;
}

//==========================================================================
// �X���C�h�Đ�
//==========================================================================
void CCameraMotion::SliderPlay()
{
	m_EditInfo.bSlide = ImGui::TreeNode("SlidePlay");

	if (m_EditInfo.bSlide)
	{
		ImGui::SliderFloat("Play Ratio", &m_EditInfo.playRatio, 0.0f, 1.0f);

		// �J�������擾
		CCamera* pCamera = CManager::GetInstance()->GetCamera();
		MyLib::Vector3 posR, rot;
		float distance = pCamera->GetDistance();

		// ���݂̃��[�V�������
		MotionInfo nowInfo = m_EditInfo.motionInfo;

		// ���̃L�[�C���f�b�N�X
		int keySize = nowInfo.Key.size();
		int nextKeyID = (m_EditInfo.keyIdx + 1) % keySize;
		if (nextKeyID == 0)
		{// �I�[
			nextKeyID = keySize - 1;
		}

		// �L�[���
		MotionKey nowKey = nowInfo.Key[m_EditInfo.keyIdx];
		MotionKey nextKey = nowInfo.Key[nextKeyID];

		posR = UtilFunc::Correction::EasingLinear(nowKey.posRDest, nextKey.posRDest, m_EditInfo.playRatio);
		rot = UtilFunc::Correction::EasingLinear(nowKey.rotDest, nextKey.rotDest, m_EditInfo.playRatio);
		distance = UtilFunc::Correction::EasingLinear(nowKey.distance, nextKey.distance, m_EditInfo.playRatio);

		// �J�������ݒ�
		pCamera->SetPositionR(m_pos + posR);
		pCamera->SetRotation(rot);
		pCamera->SetDistance(distance);

		ImGui::TreePop();
	}
}

//==========================================================================
// ���[�V�����؂�ւ�
//==========================================================================
void CCameraMotion::ChangeMotion()
{
	// [�O���[�v]���[�V�����؂�ւ�
	if (ImGui::TreeNode("Change Motion"))
	{
		// [���W�I�{�^��]���[�V�����؂�ւ�
		for (int i = 0; i < static_cast<int>(m_MotionFileName.size()); i++)
		{
			if (ImGui::RadioButton(m_MotionFileName[i].c_str(), &m_EditInfo.motionIdx, i))
			{
				// �G�f�B�b�g���ؑ�
				m_EditInfo.motionInfo = m_vecMotionInfo[i];
			}
		}

		//=============================
		// ���[�V�����G�f�B�b�g
		//=============================
		EditMotion();

		ImGui::TreePop();
	}
}

//==========================================================================
// �L�[�؂�ւ�
//==========================================================================
void CCameraMotion::ChangeKey()
{
	// ���T�C�Y
	if (ImGui::TreeNode("Key"))
	{
		ImGui::SeparatorText("Change Key");

		//=============================
		// �R�s�[
		//=============================
		if (ImGui::Button("Copy Key"))
		{
			m_EditInfo.Key_copyData = m_vecMotionInfo[m_EditInfo.motionIdx].Key[m_EditInfo.keyIdx];
		}
		ImGui::SameLine();

		//=============================
		// �y�[�X�g
		//=============================
		if (ImGui::Button("Paste Key"))
		{
			m_EditInfo.motionInfo.Key[m_EditInfo.keyIdx] = m_EditInfo.Key_copyData;

			// �J�������擾
			CCamera* pCamera = CManager::GetInstance()->GetCamera();

			// �J�������ݒ�
			pCamera->SetPositionR(m_pos + m_EditInfo.motionInfo.Key[m_EditInfo.keyIdx].posRDest);
			pCamera->SetRotation(m_EditInfo.motionInfo.Key[m_EditInfo.keyIdx].rotDest);
			pCamera->SetDistance(m_EditInfo.motionInfo.Key[m_EditInfo.keyIdx].distance);
		}
		ImGui::Separator();

		//=============================
		// �����ύX
		//=============================
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Change Coolider Num:");
		ImGui::SameLine();
		if (ImGui::ArrowButton("##left", ImGuiDir_Left) &&
			m_EditInfo.motionInfo.Key.size() > 1)
		{
			m_EditInfo.motionInfo.Key.pop_back();
		}
		ImGui::SameLine(0.0f);
		if (ImGui::ArrowButton("##right", ImGuiDir_Right))
		{
			m_EditInfo.motionInfo.Key.push_back(MotionKey());
		}
		ImGui::SameLine();

		// �T�C�Y
		int segmentSize = static_cast<int>(m_EditInfo.motionInfo.Key.size());
		ImGui::Text("%d", segmentSize);



		ImGui::SetNextItemWidth(140.0f);
		if (ImGui::SliderInt("Key Idx", &m_EditInfo.keyIdx, 0, m_EditInfo.motionInfo.Key.size() - 1))
		{
			// ���̃f�[�^
			if (static_cast<int>(m_vecMotionInfo[m_EditInfo.motionIdx].Key.size()) > m_EditInfo.keyIdx)
			{
				m_EditInfo.motionInfo.Key[m_EditInfo.keyIdx] = m_vecMotionInfo[m_EditInfo.motionIdx].Key[m_EditInfo.keyIdx];

				// �J�������擾
				CCamera* pCamera = CManager::GetInstance()->GetCamera();\

				// �J�������ݒ�
				pCamera->SetPositionR(m_pos + m_EditInfo.motionInfo.Key[m_EditInfo.keyIdx].posRDest);
				pCamera->SetRotation(m_EditInfo.motionInfo.Key[m_EditInfo.keyIdx].rotDest);
				pCamera->SetDistance(m_EditInfo.motionInfo.Key[m_EditInfo.keyIdx].distance);
			}
			else
			{
				m_EditInfo.motionInfo.Key[m_EditInfo.keyIdx] = MotionKey();
			}


		}
		ImGui::Dummy(ImVec2(0.0f, 10.0f));


		//=============================
		// �L�[�G�f�B�b�g
		//=============================
		EditKey();


		ImGui::TreePop();
	}
}

//==========================================================================
// ���[�V�����G�f�B�b�g
//==========================================================================
void CCameraMotion::EditMotion()
{
	// �J�������擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// �L�[���
	MotionInfo* pInfo = &m_EditInfo.motionInfo;

	if (ImGui::Button("Regist Motion"))
	{
		m_vecMotionInfo[m_EditInfo.motionIdx] = m_EditInfo.motionInfo;
	}
}

//==========================================================================
// �L�[�G�f�B�b�g
//==========================================================================
void CCameraMotion::EditKey()
{
	if (m_EditInfo.bSlide)	// �X���C�h���I��
		return;

	// �J�������擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 rot = pCamera->GetRotation();
	MyLib::Vector3 posR = pCamera->GetPositionR();

	// �L�[���
	MotionKey* pKey = &m_EditInfo.motionInfo.Key[m_EditInfo.keyIdx];

	pKey->rotDest = rot;
	pKey->posRDest = posR - m_EditInfo.offset;

	// �ʒu����
	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::DragFloat3("posR", (float*)&posR, 1.0f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat3("rot", (float*)&rot, 0.1f, 0.0f, 0.0f, "%.2f");
		pCamera->SetPositionR(posR);
		pCamera->SetRotation(rot);

		pKey->rotDest = rot;
		pKey->posRDest = posR - m_EditInfo.offset;
	}

	if (ImGui::Button("Regist Key"))
	{
		if (static_cast<int>(m_vecMotionInfo[m_EditInfo.motionIdx].Key.size()) <= m_EditInfo.keyIdx)
		{// �ҏW����L�[�����̗v�f���傫���Ƃ�
			m_vecMotionInfo[m_EditInfo.motionIdx].Key.push_back(*pKey);
		}
		else
		{
			m_vecMotionInfo[m_EditInfo.motionIdx].Key[m_EditInfo.keyIdx] = *pKey;
		}
	}

	// ����
	pKey->distance = pCamera->GetDistance();
	ImGui::Text("distance : %f", pKey->distance);

	// �Đ�����
	ImGui::DragFloat("playTime", &pKey->playTime, 0.01f, 0.0f, 0.0f, "%.2f");
}

//==========================================================================
// �g���K�[�G�f�B�b�g
//==========================================================================
void CCameraMotion::EditTrigger()
{
	if (ImGui::TreeNode("Trigger"))
	{
		std::vector<float>* pTrigger = &m_EditInfo.motionInfo.trigger;

		//=============================
		// �����ύX
		//=============================
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Change Trigger Num:");
		ImGui::SameLine();
		if (ImGui::ArrowButton("##left", ImGuiDir_Left) &&
			pTrigger->size() > 1)
		{
			pTrigger->pop_back();

			m_EditInfo.triggerIdx = UtilFunc::Transformation::Clamp(m_EditInfo.triggerIdx, 0, static_cast<int>(pTrigger->size()) - 1);
		}
		ImGui::SameLine(0.0f);
		if (ImGui::ArrowButton("##right", ImGuiDir_Right))
		{
			pTrigger->push_back(0.0f);
		}
		ImGui::SameLine();

		// �T�C�Y
		int segmentSize = static_cast<int>(pTrigger->size());
		ImGui::Text("%d", segmentSize);

		//=============================
		// ��O����
		//=============================
		if (pTrigger->size() <= 0)
		{
			ImGui::Text("��None Data��");
			ImGui::TreePop();
			return;
		}
		float* pThisTrigger = &m_EditInfo.motionInfo.trigger[m_EditInfo.triggerIdx];

		ImGui::SetNextItemWidth(140.0f);
		if (ImGui::SliderInt("Trigger Idx", &m_EditInfo.triggerIdx, 0, pTrigger->size() - 1))
		{
			// ���̃f�[�^
			if (static_cast<int>(m_vecMotionInfo[m_EditInfo.motionIdx].trigger.size()) > m_EditInfo.triggerIdx)
			{
				m_EditInfo.motionInfo.trigger[m_EditInfo.triggerIdx] = m_vecMotionInfo[m_EditInfo.motionIdx].trigger[m_EditInfo.triggerIdx];
			}
			else
			{
				m_EditInfo.motionInfo.trigger[m_EditInfo.keyIdx] = 0.0f;
			}

		}
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		//=============================
		// ���f�[�^�֊��蓖��
		//=============================
		if (ImGui::Button("Regist Trigger"))
		{
			m_vecMotionInfo[m_EditInfo.motionIdx].trigger = *pTrigger;
		}

		// �g���K�[��������
		ImGui::DragFloat("trigger Time", &(*pThisTrigger), 0.01f, 0.0f, 0.0f, "%.2f");

		ImGui::TreePop();
	}
}

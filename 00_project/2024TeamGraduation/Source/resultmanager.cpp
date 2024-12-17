//=============================================================================
// 
// ���U���g�}�l�[�W������ [resultmanager.cpp]
//  Author : Kai Takada
// 
//=============================================================================
#include "resultmanager.h"
#include "result.h"

#include "playerResult.h"
#include "playerManager.h"
#include "audience.h"
#include "camera.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const MyLib::Vector2 SIZE_POLY = MyLib::Vector2(100.0f, 100.0f);

	const MyLib::Vector3 ROT_CAMERA = MyLib::Vector3(0.0f, 0.0f, -0.36f);
	const float DISTANCE_CAMERA = 1940.0f;
}

// ��Ԏ���
namespace StateTime
{
	const float NONE(5.0f);		// �Ȃ�
	const float PRELUDE(5.0f);	// �O��
}

namespace Draw
{// ��������
	const MyLib::Vector3 POS_PRELUDE = MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.4f, 0.0f);
	const MyLib::Vector3 POS_CONTEST = MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.6f, 0.0f);
	const std::string TEXFILE = "data\\TEXTURE\\result\\draw.jpg";
}

namespace Prelude
{
	const std::string TEXFILE_LOSE = "data\\TEXTURE\\result\\prelude_lose.jpg";		// ����
	const std::string TEXFILE_WIN = "data\\TEXTURE\\result\\prelude_win.jpg";		// ����

	const MyLib::Vector3 POS_POLY[CGameManager::ETeamSide::SIDE_MAX] =
	{
		MyLib::Vector3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.4f, 0.0f),
		MyLib::Vector3(SCREEN_WIDTH * 0.7f, SCREEN_HEIGHT * 0.4f, 0.0f),
	};

	const MyLib::Vector3 POSR_CAMERA = MyLib::Vector3(314.0f, 0.0f, 0.0f);
	const MyLib::Vector3 ROT_CAMERA = MyLib::Vector3(0.0f, 0.17f, -0.36f);
	const float DISTANCE_CAMERA = 1940.0f;
}

namespace Contest
{
	const std::string TEXFILE_LOSE = "data\\TEXTURE\\result\\contest_lose.jpg";		// ����
	const std::string TEXFILE_WIN = "data\\TEXTURE\\result\\contest_win.jpg";		// ����

	const MyLib::Vector3 POS_POLY[CGameManager::ETeamSide::SIDE_MAX] =
	{
		MyLib::Vector3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.6f, 0.0f),
		MyLib::Vector3(SCREEN_WIDTH * 0.7f, SCREEN_HEIGHT * 0.6f, 0.0f),
	};

	const MyLib::Vector3 POSR_CAMERA = MyLib::Vector3(900.0f, 0.0f, 240.0f);
	const MyLib::Vector3 ROT_CAMERA = MyLib::Vector3(0.0f, -0.32f, -0.36f);
	const float DISTANCE_CAMERA = 150.0f;
}

// �v���C���[
namespace Player
{
	const float INIT_IN_X	(100.0f);	// �����ʒu(����)
	const float INIT_IN_Z	(100.0f);	// �����ʒu(����)
	const float INIT_OUT_X	(100.0f);	// �����ʒu(�O��)
	const float INIT_OUT_Z	(300.0f);	// �����ʒu(�O��)
	const float SHIFT	(50.0f);	// ���炵�l

	const MyLib::Vector3 POS_IN[CGameManager::ETeamSide::SIDE_MAX] =
	{// ����ʒu
		MyLib::Vector3(-INIT_IN_X, 0.0f, +INIT_IN_Z),
		MyLib::Vector3(+INIT_IN_X, 0.0f, +INIT_IN_Z),
	};

	const MyLib::Vector3 POS_OUT[CGameManager::ETeamSide::SIDE_MAX] =
	{// �O��ʒu
		MyLib::Vector3(-INIT_OUT_X, 0.0f, +INIT_OUT_Z),
		MyLib::Vector3(+INIT_OUT_X, 0.0f, +INIT_OUT_Z),
	};

	const MyLib::Vector3 POS_SHIFT[CGameManager::ETeamSide::SIDE_MAX] =
	{// ���炵�l
		MyLib::Vector3(-SHIFT, 0.0f, -SHIFT),
		MyLib::Vector3(-SHIFT, 0.0f, -SHIFT),
	};
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CResultManager::STATE_FUNC CResultManager::m_StateFunc[] =				// ��Ԋ֐�
{
	& CResultManager::StateNone,				// �Ȃ�
	& CResultManager::StatePrelude,				// �O�����s
	& CResultManager::StateCharmContest,		// ���e���s
};

CResultManager::STATE_START_FUNC CResultManager::m_StateStartFunc[] =	// ��ԊJ�n
{
	nullptr,									// �Ȃ�
	&CResultManager::StateStartPrelude,			// �O�����s
	&CResultManager::StateStartCharmContest,	// ���e���s
};

CResultManager::STATE_END_FUNC CResultManager::m_StateEndFunc[] =		// ��ԏI��
{
	nullptr,									// �Ȃ�
	&CResultManager::StateEndPrelude,			// �O�����s
	&CResultManager::StateEndCharmContest,		// ���e���s
};

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================
CResultManager* CResultManager::m_pThisPtr = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CResultManager::CResultManager()
{
	m_fTension = 0.0f;												// ����オ��l
	m_teamPreludeWin = CGameManager::ETeamSide::SIDE_NONE;			// �����`�[��
	m_teamContestWin = CGameManager::ETeamSide::SIDE_NONE;			// �����`�[��
	memset(&m_fCharmValue[0], 0, sizeof(m_fCharmValue));			// ���e�l
	m_state = EState::STATE_NONE;									// ���
	m_fStateTime = 0.0f;											// ��Ԏ���
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CResultManager::~CResultManager()
{

}

//==========================================================================
// ��������
//==========================================================================
CResultManager* CResultManager::Create()
{
	if (m_pThisPtr != nullptr)
	{// ���ɂ���ꍇ
		return m_pThisPtr;
	}

	// �C���X�^���X����
	m_pThisPtr = DEBUG_NEW CResultManager;

	if (m_pThisPtr != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		HRESULT hr = m_pThisPtr->Init();

		if (FAILED(hr))
		{// ���s���Ă�����
			return nullptr;
		}
	}

	return m_pThisPtr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CResultManager::Init()
{
	//�`�[���X�e�[�^�X
	Load();

	// �v���C���[�}�l�[�W���[�̐���
	CPlayerManager::Create(CScene::MODE::MODE_RESULT);

	// �ϋq����
	CreateAudience();

	// �J�����ݒ�
	CCamera* pCamera = GET_MANAGER->GetCamera();
	pCamera->SetDistance(DISTANCE_CAMERA);
	pCamera->SetRotation(ROT_CAMERA);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CResultManager::Uninit()
{
	// �v���C���[�}�l�[�W���[
	CPlayerManager* pManager = CPlayerManager::GetInstance();
	SAFE_UNINIT(pManager);

	// ���g�̊J��
	delete m_pThisPtr;
	m_pThisPtr = nullptr;
}

//==========================================================================
// �X�V����
//==========================================================================
void CResultManager::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ��ԍX�V
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);

	// �ŏ������`�[���h�[��
	// ���̌ヂ�e�l�����h�[��
	// ����オ��l�h�[��


#if _DEBUG	// �f�o�b�O����

	if (ImGui::TreeNode("ResultManager"))
	{
		Debug();
		ImGui::TreePop();
	}
#endif
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CResultManager::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ��ԃ^�C�}�[���Z
	m_fStateTime += fDeltaTime * fSlowRate;

	// ��ԍX�V
	if (m_StateFunc[m_state] != nullptr)
	{
		(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// �Ȃ�
//==========================================================================
void CResultManager::StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �w�莞�Ԃ��߂�����
	if (m_fStateTime >= StateTime::NONE)
	{
		SetState(EState::STATE_PRELUDE);
	}
}

//==========================================================================
// �O�����s���
//==========================================================================
void CResultManager::StatePrelude(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �J�����␳
	CCamera* pCamera = GET_MANAGER->GetCamera();
	const float START = 0.0f;
	const float END = 1.0f;
	float fDistance = UtilFunc::Correction::EasingLinear(pCamera->GetDistanceOrigin(), Prelude::DISTANCE_CAMERA, START, END, m_fStateTime);
	MyLib::Vector3 rot = UtilFunc::Correction::EasingLinear(pCamera->GetOriginRotation(), Prelude::ROT_CAMERA, START, END, m_fStateTime);
	MyLib::Vector3 posR = UtilFunc::Correction::EasingLinear(pCamera->GetPositionROrigin(), Prelude::POSR_CAMERA, START, END, m_fStateTime);

	pCamera->SetDistance(fDistance);
	pCamera->SetRotation(rot);
	pCamera->SetPositionR(posR);

	// ���Ӗ�����
	pCamera->SetDestRotation(Prelude::ROT_CAMERA);

	// �w�莞�Ԃ��߂�����
	if (m_fStateTime >= StateTime::PRELUDE)
	{
		SetState(EState::STATE_CONTEST);
	}
}

//==========================================================================
// ���e���s���
//==========================================================================
void CResultManager::StateCharmContest(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �^�C�}�[���Z�b�g
	m_fStateTime = 0.0f;
}

//==========================================================================
// [�J�n]�O�����s
//==========================================================================
void CResultManager::StateStartPrelude()
{
	CTexture* pTexture = CTexture::GetInstance();

	// �ϋq����グ
	CAudience::SetEnableJumpAll(true, m_teamPreludeWin);

	// �|���S������(��������)
	if (m_teamPreludeWin == CGameManager::ETeamSide::SIDE_NONE)
	{// ����������������
		CObject2D* pObj = CObject2D::Create();

		pObj->SetPosition(Draw::POS_PRELUDE);
		pObj->SetSize(SIZE_POLY);
		pObj->BindTexture(pTexture->Regist(Draw::TEXFILE));
		return;
	}

	// �|���S������(���s)
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{// ���s��
		CObject2D* pObj = CObject2D::Create();
	
		pObj->SetPosition(Prelude::POS_POLY[i]);
		pObj->SetSize(SIZE_POLY);

		if (m_teamPreludeWin == static_cast<CGameManager::ETeamSide>(i))
		{// �����`�[���̏ꍇ
			pObj->BindTexture(pTexture->Regist(Prelude::TEXFILE_WIN));
		}
		else
		{
			pObj->BindTexture(pTexture->Regist(Prelude::TEXFILE_LOSE));
		}
	}

	// �J�����ݒ�
	CCamera* pCamera = GET_MANAGER->GetCamera();

	pCamera->SetDistanceOrigin(pCamera->GetDistance());
	pCamera->SetOriginRotation(pCamera->GetRotation());
	pCamera->SetPositionROrigin(pCamera->GetPositionR());

	pCamera->SetDistanceDest(Prelude::DISTANCE_CAMERA);
	pCamera->SetDestRotation(Prelude::ROT_CAMERA);
	pCamera->SetPositionRDest(Prelude::POSR_CAMERA);
}

//==========================================================================
// [�J�n]���e���s
//==========================================================================
void CResultManager::StateStartCharmContest()
{
	CTexture* pTexture = CTexture::GetInstance();

	// �ϋqNTR
	CAudience::SetNTRAll(CGameManager::GetInstance()->RivalTeam(m_teamContestWin));

	// �ϋq����グ
	CAudience::SetEnableJumpAll(true, m_teamContestWin);

	// �|���S������(��������)
	if (m_teamContestWin == CGameManager::ETeamSide::SIDE_NONE)
	{// ����������������
		CObject2D* pObj = CObject2D::Create();

		pObj->SetPosition(Draw::POS_CONTEST);
		pObj->SetSize(SIZE_POLY);
		pObj->BindTexture(pTexture->Regist(Draw::TEXFILE));
		return;
	}

	//TAKADA: ���e�l�o���H
	// �|���S������(���s)
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{
		CObject2D* pObj = CObject2D::Create();

		pObj->SetPosition(Contest::POS_POLY[i]);
		pObj->SetSize(SIZE_POLY);

		if (m_teamContestWin == static_cast<CGameManager::ETeamSide>(i))
		{// �����`�[���̏ꍇ
			pObj->BindTexture(pTexture->Regist(Contest::TEXFILE_WIN));
		}
		else
		{
			pObj->BindTexture(pTexture->Regist(Contest::TEXFILE_LOSE));
		}
	}

	// �J�����ݒ�
	CCamera* pCamera = GET_MANAGER->GetCamera();
	pCamera->SetDistanceDest(Contest::DISTANCE_CAMERA);
	pCamera->SetDestRotation(Contest::ROT_CAMERA);
	pCamera->SetPositionRDest(Contest::POSR_CAMERA);
}

//==========================================================================
// [�I��]�O�����s
//==========================================================================
void CResultManager::StateEndPrelude()
{
	// �S�ϋq����
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{
		CAudience::SetEnableJumpAll(false, static_cast<CGameManager::ETeamSide>(i));
	}
}

//==========================================================================
// [�I��]���e���s
//==========================================================================
void CResultManager::StateEndCharmContest()
{
	// �S�ϋq����
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{
		CAudience::SetEnableJumpAll(false, static_cast<CGameManager::ETeamSide>(i));
	}
}

//==========================================================================
// �ϋq�X�V
//==========================================================================
void CResultManager::CreateAudience()
{
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{
		float fMoteRate = m_fCharmValue[i] / 100.0f;					// ���e����	//TODO: �}�W�b�N�i���o�[
		int nNumAudience = (int)(CAudience::MAX_WATCH * fMoteRate);		// ���݂̊ϋq��

		// �ϋq����ݒ�
		CAudience::SetNumWatch(nNumAudience, static_cast<CGameManager::ETeamSide>(i));
	}
}

//==========================================================================
// �������ǂݍ���
//==========================================================================
void CResultManager::Load()
{
	// �t�@�C�����J��
	std::ifstream File(ResultManager::TEXTFILE);
	if (!File.is_open()) {
		return;
	}

	// �R�����g�p
	std::string hoge;

	int winteam = 0;
	int i = 0;

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

		if (line.find("PRELUDE_WIN") != std::string::npos)
		{// �O������

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>		// ��
				winteam;	// �����`�[��

			m_teamPreludeWin = static_cast<CGameManager::ETeamSide>(winteam);
		}

		if (line.find("CONTEST_WIN") != std::string::npos)
		{// ���e����

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>		// ��
				winteam;	// �����`�[��

			m_teamContestWin = static_cast<CGameManager::ETeamSide>(winteam);
		}

		if (line.find("TENSION") != std::string::npos)
		{// ����オ��l

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>		// ��
				m_fTension;	// ����オ��l
		}

		if (line.find("SETTEAM") != std::string::npos)
		{// �`�[�����
			LoadTeam(&File, line, i);
	
			i++;
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
// �`�[���X�e�[�^�X�ǂݍ���
//==========================================================================
void CResultManager::LoadTeam(std::ifstream* File, std::string line, int nTeam)
{
	// �R�����g�p
	std::string hoge;
	int j = 0;	// �v���C���[�l��

	while (std::getline(*File, line))
	{
		// �R�����g�̓X�L�b�v
		if (line.empty() ||
			line[0] == '#')
		{
			continue;
		}

		if (line.find("END_SETTEAM") != std::string::npos)
		{// �I��
			break;
		}

		if (line.find("CHARMVALUE") != std::string::npos)
		{// ���e�l

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>		// ��
				m_fCharmValue[nTeam];	// ���e�l
		}

	}
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CResultManager::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;

	// ��ԍX�V
	if (m_StateStartFunc[m_state] != nullptr)
	{
		(this->*(m_StateStartFunc[m_state]))();
	}
}

//==========================================================================
// �f�o�b�O
//==========================================================================
void CResultManager::Debug()
{
#if _DEBUG

	if (ImGui::TreeNode("Audience"))
	{
		for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
		{
			std::string lavel = "MoteValue";
			lavel += i;

			// �`�[���̃��e�l����
			ImGui::DragFloat(lavel.c_str(), &m_fCharmValue[i], 0.1f, 0.0f, 100.0f, "%.2f");	// ���e�l�̕ϓ�����
			float fMoteRate = m_fCharmValue[i] / 100.0f;					// ���e����	//TODO: �}�W�b�N�i���o�[
			int nNumAudience = (int)(CAudience::MAX_WATCH * fMoteRate);		// ���݂̊ϋq��

			// �ϋq����ݒ�
			CAudience::SetNumWatch(nNumAudience, static_cast<CGameManager::ETeamSide>(i));

			// �ϋq����ݒ�
			int num = CAudience::GetNumWatchAll(static_cast<CGameManager::ETeamSide>(i));

			ImGui::Text("NumWatch%d: %d", i, num);
		}

		// �ʒu�ݒ�
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Camera"))
	{
		CCamera* pCamera = GET_MANAGER->GetCamera();
		float fDistance = pCamera->GetDistance();

		ImGui::DragFloat("Distance", &fDistance, 0.01f, -D3DX_PI, D3DX_PI, "%.2f");

		pCamera->SetDistance(fDistance);


		// �ʒu�ݒ�
		ImGui::TreePop();
	}

#endif
}

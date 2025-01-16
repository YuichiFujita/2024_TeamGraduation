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

#include "gamemanager.h"
#include "object3D.h"
#include "3D_effect.h"
#include "resultCrown.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXFILE_PRELUDE = "data\\TEXTURE\\result\\prelude.png";	// �����ɏ��������̂́I
	const std::string TEXFILE_CONTEST = "data\\TEXTURE\\result\\contest.png";	// ��胂�e���̂́I
	const MyLib::Vector2 SIZE_POLY = MyLib::Vector2(500.0f, 100.0f);		// 3D�|���S���T�C�Y
	const float POSY_POLY = 100.0f;											// 3D�|���S���ʒu(y)

	const MyLib::Vector3 ROT_CAMERA = MyLib::Vector3(0.0f, 0.0f, -0.36f);	// �X�^�[�g���J��������
	const float DISTANCE_CAMERA = 1940.0f;									// �X�^�[�g���J��������
	const float POSR_HEIGHT = 100.0f;										// �J�����̒����_�̍���

	const MyLib::Vector3 POS_COURT[CGameManager::ETeamSide::SIDE_MAX] =	// �R�[�g�����ʒu
	{
		MyLib::Vector3(-900.0f, 0.0f, 0.0f),	// ��
		MyLib::Vector3(+900.0f, 0.0f, 0.0f),	// �E
	};
}

// ��Ԏ���
namespace StateTime
{
	const float NONE(3.0f);		// �Ȃ�
	const float READY(3.0f);	// ����
	const float PRELUDE(5.0f);	// �O��
}

namespace Draw
{// ��������
	const MyLib::Vector3 POS = MyLib::Vector3(0.0f, 100.0f, 0.0f);	// �|���S���ʒu

	const float DISTANCE_CAMERA = 1700.0f;		// �J��������
}

namespace Ready
{
	const MyLib::Vector3 POSR_CAMERA = MyLib::Vector3(0.0f, POSR_HEIGHT, 0.0f);
	const MyLib::Vector3 ROT_CAMERA = MyLib::Vector3(0.0f, 0.0f, -0.36f);

	const float DISTANCE_CAMERA = 1500.0f;

	const float POSY_WORD = 200.0f;	// �Z���t�̍���
}

namespace Prelude
{
	const std::map<CGameManager::ETeamSide, MyLib::Vector3> POSR_CAMERA =
	{
		{CGameManager::ETeamSide::SIDE_NONE,	MyLib::Vector3(0.0f, POSR_HEIGHT, 0.0f)},				// �R�[�g�s��
		{CGameManager::ETeamSide::SIDE_LEFT,	MyLib::Vector3(-350.0f, POSR_HEIGHT * 2.0f, -200.0f)},		// ���R�[�g
		{CGameManager::ETeamSide::SIDE_RIGHT,	MyLib::Vector3(+350.0f, POSR_HEIGHT * 2.0f, -200.0f)},		// �E�R�[�g
	};

	const std::map<CGameManager::ETeamSide, MyLib::Vector3> ROT_CAMERA =
	{
		{CGameManager::ETeamSide::SIDE_NONE,	MyLib::Vector3(0.0f, 0.0f, -0.36f)},		// �R�[�g�s��
		{CGameManager::ETeamSide::SIDE_LEFT,	MyLib::Vector3(0.0f, -0.95f, -0.36f)},		// ���R�[�g
		{CGameManager::ETeamSide::SIDE_RIGHT,	MyLib::Vector3(0.0f, +0.95f, -0.36f)},		// �E�R�[�g
	};

	const float DISTANCE_CAMERA = 1500.0f;

	const float POSY_CROWN = 200.0f;	// �����̍���
}

namespace Contest
{
	const std::map<CGameManager::ETeamSide, MyLib::Vector3> POSR_CAMERA =
	{
		{CGameManager::ETeamSide::SIDE_NONE,	MyLib::Vector3(0.0f, POSR_HEIGHT, 0.0f)},						// �R�[�g�s��
		{CGameManager::ETeamSide::SIDE_LEFT,	MyLib::Vector3(-37.34f, 220.0f, 260.0f) + POS_COURT[0]},	// ���R�[�g
		{CGameManager::ETeamSide::SIDE_RIGHT,	MyLib::Vector3(+37.34f, 220.0f, 260.0f) + POS_COURT[1]},	// �E�R�[�g
	};

	const std::map<CGameManager::ETeamSide, MyLib::Vector3> ROT_CAMERA =
	{
		{CGameManager::ETeamSide::SIDE_NONE,	MyLib::Vector3(0.0f, 0.0f, -0.36f)},		// �R�[�g�s��
		{CGameManager::ETeamSide::SIDE_LEFT,	MyLib::Vector3(0.0f, -0.46f, 0.23f)},		// ���R�[�g
		{CGameManager::ETeamSide::SIDE_RIGHT,	MyLib::Vector3(0.0f, +0.46f, 0.23f)},		// �E�R�[�g
	};

	const float DISTANCE_CAMERA = 700.0f;

	const float POSY_CROWN = 400.0f;	// �����̍���
}

// �J�����J�ڎ���
namespace CameraTime
{
	const float END_TIME[] =
	{
		0.0f,		// �Ȃɂ��Ȃ�
		0.3f,		// �O�����s����
		1.0f,		// �O�����s
		0.3f,		// ���e���s����
		1.0f,		// ���e���s
	};
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CResultManager::STATE_FUNC CResultManager::m_StateFunc[] =				// ��Ԋ֐�
{
	& CResultManager::StateNone,				// �Ȃ�
	& CResultManager::StatePreludeReady,		// �O�����s����
	& CResultManager::StatePrelude,				// �O�����s
	& CResultManager::StateCharmContestReady,	// ���e���s����
	& CResultManager::StateCharmContest,		// ���e���s
};

CResultManager::STATE_START_FUNC CResultManager::m_StateStartFunc[] =	// ��ԊJ�n
{
	nullptr,										// �Ȃ�
	& CResultManager::StateStartPreludeReady,		// �O�����s����
	& CResultManager::StateStartPrelude,			// �O�����s
	& CResultManager::StateStartCharmContestReady,	// ���e���s����
	& CResultManager::StateStartCharmContest,		// ���e���s
};

CResultManager::STATE_END_FUNC CResultManager::m_StateEndFunc[] =		// ��ԏI��
{
	nullptr,									// �Ȃ�
	nullptr,									// �O�����s����
	& CResultManager::StateEndPrelude,			// �O�����s
	nullptr,									// ���e���s����
	& CResultManager::StateEndCharmContest,		// ���e���s
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
	m_bStateTrans = false;											// ��ԑJ�ډ\�t���O(t:�s��/f:��)
	m_pText = nullptr;												// �����`�[���p3D�|���S��
	m_pCrown = nullptr;												// �������f��
	m_pEfkConfetti = nullptr;										// ������G�t�F�N�V�A
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
	CPlayerManager::Create(CPlayerManager::EType::TYPE_RESULT);

	// �ϋq����
	CreateAudience();

	// �J�ډ\��
	m_bStateTrans = true;

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

	// ����
	m_pCrown = nullptr;
	
	// ���������|���S��
	m_pText = nullptr;

	// �G�t�F�N�g�j��
	if (m_pEfkConfetti != nullptr)
	{
		m_pEfkConfetti->SetTrigger(0);
		m_pEfkConfetti = nullptr;
	}

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
	if (m_fStateTime >= StateTime::NONE && m_bStateTrans)
	{
		SetState(EState::STATE_PRELUDE_READY);
	}
}

//==========================================================================
// �O�����s�������
//==========================================================================
void CResultManager::StatePreludeReady(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	float START = 0.0f;
	float END = CameraTime::END_TIME[m_state];
	
	// �����ɏ��������`�[���I

	// �J�����␳
	CCamera* pCamera = GET_MANAGER->GetCamera();
	Ready::DISTANCE_CAMERA;
	if (pCamera->GetState() == CCamera::STATE::STATE_FOLLOW)
	{
		float fDistance = UtilFunc::Correction::EasingLinear(pCamera->GetDistanceOrigin(), Ready::DISTANCE_CAMERA, START, END, m_fStateTime);
		MyLib::Vector3 rot = UtilFunc::Correction::EasingLinear(pCamera->GetOriginRotation(), Ready::ROT_CAMERA, START, END, m_fStateTime);
		MyLib::Vector3 posR = UtilFunc::Correction::EasingLinear(pCamera->GetPositionROrigin(), Ready::POSR_CAMERA, START, END, m_fStateTime);

		pCamera->SetDistance(fDistance);
		pCamera->SetRotation(rot);
		pCamera->SetPositionR(posR);
	}

	// �w�莞�Ԃ��߂�����
	if (m_fStateTime >= StateTime::READY && m_bStateTrans)
	{
		// ���U���g�v���C���[�S���m�F
		CListManager<CPlayerResult> list = CPlayerResult::GetList();
		std::list<CPlayerResult*>::iterator itr = list.GetEnd();	// ���`�[���̍Ō���C�e���[�^�[
		while (list.ListLoop(itr))
		{
			// ���s�`�F�b�N(�O��)
			CPlayerResult* pPlayer = (*itr);	// �v���C���[���
			pPlayer->CheckVictoryPrelude();
		}

		// �O�����s���
		SetState(EState::STATE_PRELUDE);
	}
}

//==========================================================================
// �O�����s���
//==========================================================================
void CResultManager::StatePrelude(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	float START = 0.0f;
	float END = CameraTime::END_TIME[m_state];

	// �J�����␳
	CCamera* pCamera = GET_MANAGER->GetCamera();

	if (pCamera->GetState() == CCamera::STATE::STATE_FOLLOW)
	{
		float fDistance = UtilFunc::Correction::EasingLinear(pCamera->GetDistanceOrigin(), Prelude::DISTANCE_CAMERA, START, END, m_fStateTime);
		MyLib::Vector3 rot = UtilFunc::Correction::EasingLinear(pCamera->GetOriginRotation(), Prelude::ROT_CAMERA.at(m_teamPreludeWin), START, END, m_fStateTime);
		MyLib::Vector3 posR = UtilFunc::Correction::EasingLinear(pCamera->GetPositionROrigin(), Prelude::POSR_CAMERA.at(m_teamPreludeWin), START, END, m_fStateTime);

		pCamera->SetDistance(fDistance);
		pCamera->SetRotation(rot);
		pCamera->SetPositionR(posR);
	}

	// �w�莞�Ԃ��߂�����
	if (m_fStateTime >= StateTime::PRELUDE && m_bStateTrans)
	{
		// ���U���g�v���C���[�S���m�F
		CListManager<CPlayerResult> list = CPlayerResult::GetList();
		std::list<CPlayerResult*>::iterator itr = list.GetEnd();	// ���`�[���̍Ō���C�e���[�^�[
		while (list.ListLoop(itr))
		{
			// ���NONE��
			CPlayerResult* pPlayer = (*itr);	// �v���C���[���
			pPlayer->SetState(CPlayerResult::EState::STATE_NONE);
		}

		SetState(EState::STATE_CONTEST_READY);
	}
}

//==========================================================================
// ���e���s�������
//==========================================================================
void CResultManager::StateCharmContestReady(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	float START = 0.0f;
	float END = CameraTime::END_TIME[m_state];

	// ���e���������`�[���I


	// �J�����␳
	CCamera* pCamera = GET_MANAGER->GetCamera();

	if (pCamera->GetState() == CCamera::STATE::STATE_FOLLOW)
	{
		float fDistance = UtilFunc::Correction::EasingLinear(pCamera->GetDistanceOrigin(), Ready::DISTANCE_CAMERA, START, END, m_fStateTime);
		MyLib::Vector3 rot = UtilFunc::Correction::EasingLinear(pCamera->GetOriginRotation(), Ready::ROT_CAMERA, START, END, m_fStateTime);
		MyLib::Vector3 posR = UtilFunc::Correction::EasingLinear(pCamera->GetPositionROrigin(), Ready::POSR_CAMERA, START, END, m_fStateTime);

		pCamera->SetDistance(fDistance);
		pCamera->SetRotation(rot);
		pCamera->SetPositionR(posR);
	}

	// �w�莞�Ԃ��߂�����
	if (m_fStateTime >= StateTime::READY && m_bStateTrans)
	{
		// ���U���g�v���C���[�S���m�F
		CListManager<CPlayerResult> list = CPlayerResult::GetList();
		std::list<CPlayerResult*>::iterator itr = list.GetEnd();	// ���`�[���̍Ō���C�e���[�^�[
		while (list.ListLoop(itr))
		{
			// ���s�`�F�b�N(���e)
			CPlayerResult* pPlayer = (*itr);	// �v���C���[���
			pPlayer->CheckVictoryContest();
		}

		SetState(EState::STATE_CONTEST);
	}
}

//==========================================================================
// ���e���s���
//==========================================================================
void CResultManager::StateCharmContest(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	float START = 0.0f;
	float END = CameraTime::END_TIME[m_state];

	// �J�����␳
	CCamera* pCamera = GET_MANAGER->GetCamera();

	if (pCamera->GetState() == CCamera::STATE::STATE_FOLLOW)
	{
		float fDistance = UtilFunc::Correction::EasingLinear(pCamera->GetDistanceOrigin(), Contest::DISTANCE_CAMERA, START, END, m_fStateTime);
		MyLib::Vector3 rot = UtilFunc::Correction::EasingLinear(pCamera->GetOriginRotation(), Contest::ROT_CAMERA.at(m_teamContestWin), START, END, m_fStateTime);
		MyLib::Vector3 posR = UtilFunc::Correction::EasingLinear(pCamera->GetPositionROrigin(), Contest::POSR_CAMERA.at(m_teamContestWin), START, END, m_fStateTime);

		pCamera->SetDistance(fDistance);
		pCamera->SetRotation(rot);
		pCamera->SetPositionR(posR);
	}

	// ������

}

//==========================================================================
// [�J�n]�O�����s����
//==========================================================================
void CResultManager::StateStartPreludeReady()
{
	CTexture* pTexture = CTexture::GetInstance();

	// �ϋq���艺��
	CAudience::SetEnableJumpAll(false, m_teamPreludeWin);

	// �|���S������
	SAFE_KILL(m_pCrown);
	SAFE_KILL(m_pText);
	m_pText = CObject3D::Create();
	MyAssert::CustomAssert(m_pText != nullptr, "�Ȃ�Ń|���S�������ł��ĂȂ��񂾂�");
	m_pText->SetType(CObject::TYPE::TYPE_OBJECT3D);
	m_pText->SetSize(SIZE_POLY);

	MyLib::Vector3 pos = VEC3_ZERO;
	pos.y += Ready::POSY_WORD;
	m_pText->SetPosition(pos);

	m_pText->BindTexture(pTexture->Regist(TEXFILE_PRELUDE));

	// �J�����ݒ�
	CCamera* pCamera = GET_MANAGER->GetCamera();

	pCamera->SetDistanceOrigin(pCamera->GetDistance());
	pCamera->SetOriginRotation(pCamera->GetRotation());
	pCamera->SetPositionROrigin(pCamera->GetPositionR());

	pCamera->SetDistanceDest(Ready::DISTANCE_CAMERA);
	pCamera->SetDestRotation(Ready::ROT_CAMERA);
	pCamera->SetPositionRDest(Ready::POSR_CAMERA);

	pCamera->SetState(CCamera::STATE::STATE_FOLLOW);
}

//==========================================================================
// [�J�n]�O�����s
//==========================================================================
void CResultManager::StateStartPrelude()
{
	CTexture* pTexture = CTexture::GetInstance();

	// �ϋq����グ
	CAudience::SetEnableJumpAll(true, m_teamPreludeWin);

	// ���f������
	SAFE_KILL(m_pText);
	CreateCrown(m_teamPreludeWin);

	// �J�����ݒ�
	CCamera* pCamera = GET_MANAGER->GetCamera();

	pCamera->SetDistanceOrigin(pCamera->GetDistance());
	pCamera->SetOriginRotation(pCamera->GetRotation());
	pCamera->SetPositionROrigin(pCamera->GetPositionR());

	pCamera->SetDistanceDest(Prelude::DISTANCE_CAMERA);
	pCamera->SetDestRotation(Prelude::ROT_CAMERA.at(m_teamPreludeWin));
	pCamera->SetPositionRDest(Prelude::POSR_CAMERA.at(m_teamPreludeWin));

	pCamera->SetState(CCamera::STATE::STATE_FOLLOW);
}

//==========================================================================
// [�J�n]���e���s����
//==========================================================================
void CResultManager::StateStartCharmContestReady()
{
	CTexture* pTexture = CTexture::GetInstance();

	// �ϋq���艺��
	CAudience::SetEnableJumpAll(false, m_teamPreludeWin);

	// �|���S������
	SAFE_KILL(m_pCrown);
	SAFE_KILL(m_pText);
	m_pText = CObject3D::Create();
	MyAssert::CustomAssert(m_pText != nullptr, "�Ȃ�Ń|���S�������ł��ĂȂ��񂾂�");
	m_pText->SetType(CObject::TYPE::TYPE_OBJECT3D);
	m_pText->SetSize(SIZE_POLY);

	MyLib::Vector3 pos = VEC3_ZERO;
	pos.y += Ready::POSY_WORD;
	m_pText->SetPosition(pos);

	m_pText->BindTexture(pTexture->Regist(TEXFILE_CONTEST));

	// �J�����ݒ�
	CCamera* pCamera = GET_MANAGER->GetCamera();

	pCamera->SetDistanceOrigin(pCamera->GetDistance());
	pCamera->SetOriginRotation(pCamera->GetRotation());
	pCamera->SetPositionROrigin(pCamera->GetPositionR());

	pCamera->SetDistanceDest(Ready::DISTANCE_CAMERA);
	pCamera->SetDestRotation(Ready::ROT_CAMERA);
	pCamera->SetPositionRDest(Ready::POSR_CAMERA);

	pCamera->SetState(CCamera::STATE::STATE_FOLLOW);
}

//==========================================================================
// [�J�n]���e���s
//==========================================================================
void CResultManager::StateStartCharmContest()
{
	CTexture* pTexture = CTexture::GetInstance();

	//TAKADA: ���e�l�o���H

	// �ϋqNTR
	CAudience::SetNTRAll(m_teamContestWin);

	// �ϋq����グ
	CAudience::SetEnableJumpAll(true, m_teamContestWin);

	// ���f������
	SAFE_KILL(m_pText);
	CreateCrown(m_teamContestWin);

	// �G�t�F�N�V�A����
	CreateEffect();

	// �J�����ݒ�
	CCamera* pCamera = GET_MANAGER->GetCamera();

	pCamera->SetDistanceOrigin(pCamera->GetDistance());
	pCamera->SetOriginRotation(pCamera->GetRotation());
	pCamera->SetPositionROrigin(pCamera->GetPositionR());

	pCamera->SetDistanceDest(Contest::DISTANCE_CAMERA);
	pCamera->SetDestRotation(Contest::ROT_CAMERA.at(m_teamContestWin));
	pCamera->SetPositionRDest(Contest::POSR_CAMERA.at(m_teamContestWin));

	pCamera->SetState(CCamera::STATE::STATE_FOLLOW);
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
// �������f������
//==========================================================================
void CResultManager::CreateCrown(CGameManager::ETeamSide team)
{
	CResultCrown::EResult result = CResultCrown::EResult::RESULT_WIN;
	MyLib::Vector3 pos = VEC3_ZERO;
	pos.y += Prelude::POSY_CROWN;

	if (team == CGameManager::ETeamSide::SIDE_NONE)
	{// ��������
		pos += Draw::POS;
		result = CResultCrown::EResult::RESULT_DRAW;
	}
	else
	{// ����
		pos += POS_COURT[team];
		result = CResultCrown::EResult::RESULT_WIN;
	}

	SAFE_KILL(m_pCrown);
	m_pCrown = CResultCrown::Create(result);
	MyAssert::CustomAssert(m_pCrown != nullptr, "�Ȃ�Ń��f�������ł��ĂȂ��񂾂�");
	m_pCrown->SetType(CObject::TYPE::TYPE_OBJECTX);

	m_pCrown->SetPosition(pos);
	m_pCrown->SetOriginPosition(pos);
	m_pCrown->SetState(CResultCrown::EState::STATE_LOOP);
}

//==========================================================================
// ���s�|���S������
//==========================================================================
void CResultManager::CreatePolygon(EState state)
{
	CTexture* pTexture = CTexture::GetInstance();

	SAFE_KILL(m_pText);
	m_pText = CObject3D::Create();
	MyAssert::CustomAssert(m_pText != nullptr, "�Ȃ�Ń|���S�������ł��ĂȂ��񂾂�");
	m_pText->SetType(CObject::TYPE::TYPE_OBJECT3D);
	m_pText->SetSize(SIZE_POLY);

	MyLib::Vector3 pos = VEC3_ZERO;
	pos.y += Ready::POSY_WORD;
	m_pText->SetPosition(pos);

	std::string filepass = TEXFILE_PRELUDE;

	switch (state)
	{
	case CResultManager::STATE_PRELUDE_READY:
		filepass = TEXFILE_PRELUDE;
		break;

	case CResultManager::STATE_CONTEST_READY:
		filepass = TEXFILE_CONTEST;
		break;

	default:
		break;
	}
	
	m_pText->BindTexture(pTexture->Regist(filepass));
}

//==========================================================================
// �G�t�F�N�g����
//==========================================================================
void CResultManager::CreateEffect()
{
	MyLib::Vector3 pos;
	if (m_teamContestWin == CGameManager::ETeamSide::SIDE_NONE)
	{// ��������
		pos += Draw::POS;
	}
	else
	{// ����
		pos += POS_COURT[m_teamContestWin];
	}
	pos.y = 0.0f;

	// ����
	m_pEfkConfetti = CEffekseerObj::Create
	(
		CMyEffekseer::EEfkLabel::EFKLABEL_CONFETTI,
		pos,
		MyLib::Vector3(),
		MyLib::Vector3(),
		10.0f,
		false
	);
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
	// ��ԍX�V
	if (m_StateEndFunc[state] != nullptr)
	{
		(this->*(m_StateEndFunc[state]))();
	}

	m_state = state;
	m_fStateTime = 0.0f;

	// ��ԍX�V
	if (m_StateStartFunc[m_state] != nullptr)
	{
		(this->*(m_StateStartFunc[m_state]))();
	}
}

//==========================================================================
// �R�[�g�̒��S�ʒu�擾
//==========================================================================
MyLib::Vector3 CResultManager::GetPosMid(CGameManager::ETeamSide team)
{
	return POS_COURT[team];
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

	if (ImGui::TreeNode("State"))
	{
		CCamera* pCamera = GET_MANAGER->GetCamera();
		float fDistance = pCamera->GetDistance();

		// ���݂̏��
		ImGui::Text("state: %s", magic_enum::enum_name(m_state));
	
		// �J�ډ\�t���O
		ImGui::Checkbox("Trans", &m_bStateTrans);

		// ���s���]
		if (ImGui::Button("Frag:prelude"))
		{
			m_teamPreludeWin = CGameManager::GetInstance()->RivalTeam(m_teamPreludeWin);
		}

		if (ImGui::Button("Frag:contest"))
		{
			m_teamContestWin = CGameManager::GetInstance()->RivalTeam(m_teamContestWin);
		}
		
		// ��Ԑݒ�
		for (int i = 0; i < CResultManager::EState::STATE_MAX; i++)
		{
			CResultManager::EState state = static_cast<EState>(i);
			std::string str = magic_enum::enum_name(state).data();
			if (ImGui::Button(str.c_str()))
			{
				SetState(state);
			}
		}

		// �ʒu�ݒ�
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("crown"))
	{
		if (m_pCrown!= nullptr)
		{
			MyLib::Vector3 pos = m_pCrown->GetPosition();
			ImGui::DragFloat3("pos", (float*)&pos, 1.0f, 0.0f, 0.0f, "%.2f");
			m_pCrown->SetPosition(pos);
		}

		// �ʒu�ݒ�
		ImGui::TreePop();
	}

#endif
}

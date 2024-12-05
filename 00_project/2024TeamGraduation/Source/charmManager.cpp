//=============================================================================
//
// ���e�}�l�[�W������ [charmManager.cpp]
// Author : Kai Takada
//
//=============================================================================
#include "charmManager.h"
#include "manager.h"
#include "gameManager.h"
#include "player.h"

// �f�o�b�O�p
#include "collisionLine_Box.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const float TIME_HAVE_LONG = 10.0f;				// ���������Ă���(�b��)
	const float TIME_EDGE_ESCAPE = 1.0f;			// �[�ɓ��������Ă���(�b��)
}

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================
CCharmManager* CCharmManager::m_pThisPtr = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCharmManager::CCharmManager()
{
	//m_pCourtSizeBox = nullptr;
	for (int i = 0; i < 4; i++)
	{
		m_pCourtSizeBox[i] = nullptr;
	}
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCharmManager::~CCharmManager()
{

}

//==========================================================================
// ��������
//==========================================================================
CCharmManager* CCharmManager::Create()
{
	if (m_pThisPtr != nullptr)
	{// ���ɂ���ꍇ
		return m_pThisPtr;
	}

	// �������̊m��
	m_pThisPtr = DEBUG_NEW CCharmManager;

	if (m_pThisPtr != nullptr)
	{
		// ����������
		m_pThisPtr->Init();
	}

	return m_pThisPtr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CCharmManager::Init()
{

#if _DEBUG
	// �R�[�g�T�C�Y�̃{�b�N�X

	for (int i = 0; i < 4; i++)
	{
		SAFE_UNINIT(m_pCourtSizeBox[i]);
		m_pCourtSizeBox[i] = CCollisionLine_Box::Create(MyLib::AABB(), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}

#endif

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CCharmManager::Uninit()
{
#if _DEBUG
	//SAFE_UNINIT(m_pCourtSizeBox);
	for (int i = 0; i < 4; i++)
	{
		//SAFE_UNINIT(m_pCourtSizeBox[i]);
	}

#endif

	delete m_pThisPtr;
	m_pThisPtr = nullptr;
}

//==========================================================================
// �񃂃e�܂Ƃ�
//==========================================================================
void CCharmManager::UnCharm(CPlayer* pPlayer, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ��������
	LongHold(pPlayer, fDeltaTime, fDeltaRate, fSlowRate);

	// �[�ɓ����܂���
	EdgeEscape(pPlayer, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ���������Ă��锻��
//==========================================================================
void CCharmManager::LongHold(CPlayer* pPlayer, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	float fHaveTime = pPlayer->GetHaveTime();

	if (pPlayer->GetBall() == nullptr)
	{// ���Z�b�g
		pPlayer->SetHaveTime(0.0f);
		return;
	}

	CGameManager* pGameMgr = CGameManager::GetInstance();

	// ���Z
	fHaveTime += fDeltaTime * fSlowRate;

	if (fHaveTime > TIME_HAVE_LONG)
	{// ���e�_�E��

		// ���e���Z
		pGameMgr->SubCharmValue(pPlayer->GetTeam(), CCharmValueManager::ETypeSub::SUB_LONG_HOLD);

		fHaveTime = 0.0;
	}
	
	pPlayer->SetHaveTime(fHaveTime);
}

//==========================================================================
// �[�ɓ���������
//==========================================================================
void CCharmManager::EdgeEscape(CPlayer* pPlayer, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (pPlayer->GetAreaType() != CPlayer::EFieldArea::FIELD_IN) return;

	CGameManager* pGameMgr = CGameManager::GetInstance();
	float fEscapeTime = pPlayer->GetEscapeTime();

	if (CheckEdgeEscape(pPlayer))
	{// �[��������

		// ���Z
		fEscapeTime += fDeltaTime * fSlowRate;
	}

	if (fEscapeTime >= TIME_EDGE_ESCAPE)
	{// ���e�_�E��

		// ���e���Z
		pGameMgr->SubCharmValue(pPlayer->GetTeam(), CCharmValueManager::ETypeSub::SUB_EDGE_ESCAPE);

		fEscapeTime = 0.0;
	}

	pPlayer->SetEscapeTime(fEscapeTime);
}

//==========================================================================
// �[�ɓ���������
//==========================================================================
bool CCharmManager::CheckEdgeEscape(CPlayer* pPlayer)
{
	MyLib::Vector3 pos = pPlayer->GetPosition();
	CGameManager* pGmMgr = CGameManager::GetInstance();
	MyLib::Vector3 posCourt = MyLib::Vector3();											// ���w����
	MyLib::Vector3 sizeCourt = pGmMgr->GetCourtSize(pPlayer->GetTeam(), posCourt);		// ���w�T�C�Y

	// �{�[�����
	CBall* pBall = pGmMgr->GetBall();
	CPlayer* pBallPlayer = pBall->GetPlayer();
	if (pBallPlayer == nullptr) return false;
	if (pBallPlayer->GetTeam() == pPlayer->GetTeam()) return false;

	MyLib::Vector3 posBP = pBallPlayer->GetPosition();

	// �{�[�������v���C���[�Ǝ��w�����̃x�N�g��
	MyLib::Vector3 vecDiff = (posBP - posCourt).Normal();

	// �[�G���A���x�N�g�������������
	// ���w4�_(�㉺���E)
	MyLib::Vector3 posEdge[4] =
	{
		MyLib::Vector3(posCourt.x + sizeCourt.x, 0.0f, posCourt.z + sizeCourt.z),	// �E��
		MyLib::Vector3(posCourt.x + sizeCourt.x, 0.0f, posCourt.z - sizeCourt.z),	// �E��O
		MyLib::Vector3(posCourt.x - sizeCourt.x, 0.0f, posCourt.z - sizeCourt.z),	// ����O
		MyLib::Vector3(posCourt.x - sizeCourt.x, 0.0f, posCourt.z + sizeCourt.z),	// ����
	};

	MyLib::Vector3 AreaMid = MyLib::Vector3();	// ��_
	MyLib::Vector2 areaSize = MyLib::Vector2(sizeCourt.x, sizeCourt.z);		// �G���A�T�C�Y
	areaSize.x *= 0.2f;							//z��x�Z
	float fAngle = 0.0f;						//rot
	int nIdx = 0;

	// ����
	for (int i = 0; i < 4; i++)
	{
		MyLib::Vector3 posEdgeNext = posEdge[0];
		if (i < 3)
		{
			posEdgeNext = posEdge[i + 1];
		}
		
		// ����G���A�̒��S�ݒ�
		D3DXCOLOR col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

		MyLib::Vector3 size = MyLib::Vector3(areaSize.x, 100.0f, areaSize.y);		// �G���A�T�C�Y
		AreaMid = UtilFunc::Calculation::GetCenterPosition3D(posEdge[i], posEdgeNext);
		if (m_pCourtSizeBox[i] != nullptr)
		{
			m_pCourtSizeBox[i]->SetColor(col);
			m_pCourtSizeBox[i]->SetPosition(AreaMid);
			m_pCourtSizeBox[i]->SetAABB(MyLib::AABB(-size, size));
		}

		// �{�[�������v���C���[�̑Ίp�ɂ��鎩�w�̕ӂ�������
		// �R�[�g��������{�[�������v���C���[�̖���΂��ē�����Ƃ�
		if (UtilFunc::Collision::IsVecToLine(posEdge[i], posEdgeNext, posCourt, vecDiff))
		{// ���C������

			nIdx = i;
			
			col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
			if (m_pCourtSizeBox[i] != nullptr)
			{
				m_pCourtSizeBox[i]->SetColor(col);
			}
			break;
		}
	}

#if _DEBUG
	//// �T�C�Y�ݒ�
	// areaSize;
	// MyLib::Vector3 size = MyLib::Vector3(areaSize.x, 100.0f, areaSize.y);		// �G���A�T�C�Y

	//if (m_pCourtSizeBox != nullptr)
	//{
	//	D3DXCOLOR col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	//	m_pCourtSizeBox->SetColor(col);
	//	m_pCourtSizeBox->SetPosition(AreaMid);
	//	m_pCourtSizeBox->SetAABB(MyLib::AABB(-size, size), 1.0f);
	//}
#endif

	if (UtilFunc::Collision::CollisionSquare(AreaMid, areaSize, fAngle, pos))
	{// �[�G���A���ɂ��邩

		return true;
	}

	return false;
}

//=============================================================================
// 
//  �W���b�W�A�C�e���}�l�[�W������ [judgeitemManager.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "judgeitemManager.h"
#include "judgeitem.h"
#include "manager.h"
#include "calculation.h"
#include "player.h"
#include "game.h"
#include "gamemanager.h"
#include "judgeobj.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const float CHANGE_POINT[] =	// �|�C���g�ύX����
	{
		0.0f,	// 0����DDD
		0.3f,	// 3����CCC
		0.5f,	// 5����BBB
		0.8f,	// 8����AAA
	};

	const int ADDPOINT[] =
	{
		15,	// AAA
		5,	// BBB
		-5,	// CCC
		-10,// DDD
	};
}

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================
CListManager<CJudgeItemManager> CJudgeItemManager::m_List = {};	// ���X�g
std::map<int, CListManager<CJudgeItemManager>> CJudgeItemManager::m_ListBlock = {};	// ���X�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CJudgeItemManager::CJudgeItemManager() : CObject()
{
	// �l�̃N���A
	m_fStartLength = 0.0f;	// �����n�_����
	m_Top = MyLib::Vector3();		// �擪
	m_Cur = MyLib::Vector3();		// �I�[
	m_vecJudgeInfo.clear();	// �W���b�W���
	m_vecJudgeItem.clear();		// �W���b�W�A�C�e��
	m_bCheckCollision = false;	// �R���W�����`�F�b�N����
	m_bBindPoint = false;		// �|�C���g���f
	m_nMyBlockIdx = 0;			// �u���b�N�C���f�b�N�X

}

//==========================================================================
// ����
//==========================================================================
CJudgeItemManager* CJudgeItemManager::Create(const std::vector<CMapBlockInfo::SJudgeInfo>& vecJudge, float startLen, int mapBlock)
{
	// �C���X�^���X����
	CJudgeItemManager* pJudge = DEBUG_NEW CJudgeItemManager;

	if (pJudge != nullptr)
	{
		// �������ݒ�
		pJudge->m_vecJudgeInfo = vecJudge;
		pJudge->m_fStartLength = startLen;	// �����n�_����
		pJudge->m_nMyBlockIdx = mapBlock;	// �}�b�v�u���b�N�̃C���f�b�N�X

		// ����������
		pJudge->Init();
	}

	return pJudge;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CJudgeItemManager::Init()
{
	// ���X�g�ɒǉ�
	m_List.Regist(this);
	m_ListBlock[m_nMyBlockIdx].Regist(this);

	// ��ސݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �W���b�W����
	int minIdx = 0, maxIdx = 0;
	float minX = 99990.0f, maxX = -99990.0f;

	int i = 0;
	for (const auto& vecInfo : m_vecJudgeInfo)
	{
		CJudgeItem* pItem = CJudgeItem::Create(this, m_nMyBlockIdx, vecInfo.length + m_fStartLength, vecInfo.height);
		
		// �ǉ�
		m_vecJudgeItem.push_back(pItem);

		MyLib::Vector3 pos = pItem->GetPosition();

		if (pos.x < minX)
		{// �ŏ��l�X�V
			m_Top = pos;
			minX = pos.x;
		}

		if (pos.x > maxX)
		{// �ő�l�X�V
			m_Cur = pos;
			maxX = pos.x;
		}

		// �C���f�b�N�X�X�V
		i++;
	}

	return S_OK;
}

//==========================================================================
// ���Z�b�g
//==========================================================================
void CJudgeItemManager::Reset()
{
	// �A�C�e���̃��X�g�N���A
	m_vecJudgeItem.clear();

	// �W���b�W����
	int minIdx = 0, maxIdx = 0;
	float minX = 99990.0f, maxX = -99990.0f;

	int i = 0;
	for (const auto& vecInfo : m_vecJudgeInfo)
	{
		CJudgeItem* pItem = CJudgeItem::Create(this, m_nMyBlockIdx, vecInfo.length + m_fStartLength, vecInfo.height);

		// �ǉ�
		m_vecJudgeItem.push_back(pItem);

		MyLib::Vector3 pos = pItem->GetPosition();

		if (pos.x < minX)
		{// �ŏ��l�X�V
			m_Top = pos;
			minX = pos.x;
		}

		if (pos.x > maxX)
		{// �ő�l�X�V
			m_Cur = pos;
			maxX = pos.x;
		}

		// �C���f�b�N�X�X�V
		i++;
	}
}

//==========================================================================
// �I������
//==========================================================================
void CJudgeItemManager::Uninit()
{
	// ���X�g����폜
	m_List.Delete(this);
	m_ListBlock[m_nMyBlockIdx].Delete(this);
}


//==========================================================================
// �W���b�W�A�C�e���̍폜
//==========================================================================
void CJudgeItemManager::DeleteJudgeItem(CJudgeItem* pItem)
{
	std::vector<CJudgeItem*>::iterator itr = std::find(m_vecJudgeItem.begin(), m_vecJudgeItem.end(), pItem);
	(*itr) = nullptr;
}

//==========================================================================
// �X�V����
//==========================================================================
void CJudgeItemManager::Update()
{
	if (IsDeath()) return;

	// �R���W�����`�F�b�N����
	m_bCheckCollision = CheckCollisionRange();

	// �|�C���g���f
	BindPoint();
}

//==========================================================================
// �R���W�����͈͊m�F
//==========================================================================
bool CJudgeItemManager::CheckCollisionRange()
{
	// �v���C���[�̈ʒu�擾
	CPlayer* pPlayer = CPlayer::GetListObj().GetData(0);
	if (pPlayer == nullptr) return false;
	MyLib::Vector3 posPlayer = pPlayer->GetPosition();

	return (m_Top.x <= posPlayer.x && m_Cur.x >= posPlayer.x);
}

//==========================================================================
// �|�C���g���f
//==========================================================================
void CJudgeItemManager::BindPoint()
{
	if (m_bBindPoint) return;

	// �v���C���[�̈ʒu�擾
	CPlayer* pPlayer = CPlayer::GetListObj().GetData(0);
	if (pPlayer == nullptr) return;
	MyLib::Vector3 posPlayer = pPlayer->GetPosition();

	// �Ō���܂������Ă��Ȃ�
	if (posPlayer.x < m_Cur.x + 200.0f) return;

	// �擾���v�Z
	int getNum = 0, allNum = static_cast<int>(m_vecJudgeItem.size());
	for (const auto& item : m_vecJudgeItem)
	{
		if (item == nullptr)
		{// �����Ă�
			getNum++;
		}
	}

	// �擾����
	float ratio = static_cast<float>(getNum) / static_cast<float>(allNum);

	int rank = 4;
	for (int i = 0; i < 4; i++)
	{
		if (CHANGE_POINT[i] <= ratio)
		{
			rank--;
		}
	}

	if (rank == 4)
	{
		rank = 3;
	}


	// �]������
	CJudge::JUDGE judgeResult = static_cast<CJudge::JUDGE>(rank);

	// �]�����Ƃ�UI����
	CJudgeObj* pObj = CJudgeObj::Create(MyLib::Vector3(400.0f, 100.0f, 0.0f), judgeResult);
	pObj->SetSize(D3DXVECTOR2(256.0f, 72.0f));
	pObj->SetSizeOrigin(D3DXVECTOR2(192.0f, 54.0f));

	// �|�C���g���Z
	CGame::GetInstance()->GetGameManager()->AddEvaluationPoint(ADDPOINT[judgeResult]);

	// ���蓖�Ă���
	m_bBindPoint = true;
}

//==========================================================================
// �`�揈��
//==========================================================================
void CJudgeItemManager::Draw()
{

}

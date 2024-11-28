//==========================================================================
// 
//  ���e�������� [charmText_Left.cpp]
//  Author : ���n�Ή�
// 
//==========================================================================
#include "charmText_Left.h"
#include "player.h"
#include "object2D.h"
#include "charmManager.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{

}

namespace Position
{
	const MyLib::Vector3 START(-400.0f, 100.0f, 700.0f);						// �J�n�ʒu
	const MyLib::Vector3 FADEIN = START + MyLib::Vector3(-150.0f, 150.0f, 0.0f);	// �t�F�[�h�C���ʒu
	const MyLib::Vector3 FADEOUT = FADEIN + MyLib::Vector3(-100.0f, 100.0f, 0.0f);						// �I���ʒu
	const MyLib::Vector3 OFFSET_ICON = MyLib::Vector3(40.0f, 0.0f, 0.0f);	// �e�L�X�g�̃I�t�Z�b�g�ʒu
}

//==========================================================================
// �ÓI�����o�ϐ���`
//==========================================================================
CListManager<CCharmText_Left> CCharmText_Left::m_List = {};	// ���X�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCharmText_Left::CCharmText_Left() : CCharmText()
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCharmText_Left::~CCharmText_Left()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CCharmText_Left::Init()
{
	// ����������
	CCharmText::Init();

	// ��̈ʒu
	if (m_pFace != nullptr)
	{
		m_pFace->SetPosition(Position::START);
	}

	// �e�L�X�g�̈ʒu
	if (m_pText != nullptr)
	{
		m_pText->SetPosition(Position::START + MyLib::Vector3(m_pFace->GetSize().x, 0.0f, 0.0f));
		m_pText->SetAnchorType(CObjectBillboard::EAnchorPoint::LEFT);
	}


	// �����̃`�F�C���C���f�b�N�X
	m_nMyChainIdx = 0;

	// ���ɑ��݂��Ă�����̂��グ��
	CListManager<CCharmText_Left>::Iterator itr = m_List.GetEnd();
	while (m_List.ListLoop(itr))
	{// ���[�v
		(*itr)->AddCountUP(1);

		// �`�F�C���C���f�b�N�X�����Z
		m_nMyChainIdx++;
	}

	// �`�F�C���m�F
	CheckChain();

	// ���X�g�ǉ�
	m_List.Regist(this);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CCharmText_Left::Uninit()
{
	// ���X�g����폜
	m_List.Delete(this);

	// ���ɑ��݂��Ă�����̂��グ��
	CListManager<CCharmText_Left>::Iterator itr = m_List.GetEnd();
	while (m_List.ListLoop(itr))
	{// ���[�v

		CCharmText_Left* pText = (*itr);

		std::vector<int> vecIdx = pText->GetChainIdx();		// �`�F�C�����Ă���C���f�b�N�X�ԍ��擾
		std::vector<int>::iterator itrIdx = std::find(vecIdx.begin(), vecIdx.end(), m_nMyChainIdx);

		if (itrIdx != vecIdx.end())
		{// ���񕪏���
			vecIdx.erase(itrIdx);
		}
		pText->SetChainIdx(vecIdx);

		// �����̃C���f�b�N�X�����炷
		int myChainIdx = pText->GetMyChainIdx();
		myChainIdx--;
		pText->SetMyChainIdx(myChainIdx);
	}


	itr = m_List.GetEnd();
	while (m_List.ListLoop(itr))
	{// ���[�v

		CCharmText_Left* pText = (*itr);

		// �i�[���Ă���S�Ă����Z
		std::vector<int> vecIdx = pText->GetChainIdx();		// �`�F�C�����Ă���C���f�b�N�X�ԍ��擾
		for (auto& idx : vecIdx)
		{
			idx--;
		}
		pText->SetChainIdx(vecIdx);
	}

	// �I������
	CCharmText::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CCharmText_Left::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �X�V����
	CCharmText::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �I���Ŕ�����
	if (IsDeath()) return;

	// �ʒu�ݒ�
	MyLib::Vector3 pos = GetPosition();

	// �Ԋu�����炷
	pos.x -= DISTANCE_XZ * m_nCntUp;
	pos.z += DISTANCE_XZ * m_nCntUp;
	pos.y += DISTANCE_UP * m_nCntUp;

	MyLib::Vector2 size = m_pText->GetSize() * 0.5f;
	m_pFace->SetPosition(pos + MyLib::Vector3(size.x, -size.y, -50.0f));
	m_pText->SetPosition(pos);
}

//==========================================================================
// �t�F�[�h�C��
//==========================================================================
void CCharmText_Left::StateFadeIn()
{
	// ����
	float ratio = UtilFunc::Correction::EasingQuintIn(0.0f, 1.0f, 0.0f, STATETIME_FADEIN, m_fStateTime);

	// �ʒu�X�V
	MyLib::Vector3 pos = Position::START + (Position::FADEIN - Position::START) * ratio;
	SetPosition(pos);

	// �s�����x�X�V
	m_pFace->SetAlpha(ratio);
	m_pText->SetAlpha(ratio);

	// �t�F�[�h�C��
	CCharmText::StateFadeIn();
}

//==========================================================================
// �ҋ@
//==========================================================================
void CCharmText_Left::StateWait()
{
	// �ҋ@
	CCharmText::StateWait();
}

//==========================================================================
// �t�F�[�h�A�E�g
//==========================================================================
void CCharmText_Left::StateFadeOut()
{
	// ����
	float ratio = UtilFunc::Correction::EasingEaseOut(0.0f, 1.0f, 0.0f, STATETIME_FADEOUT, m_fStateTime);

	// �ʒu�X�V
	MyLib::Vector3 pos = Position::FADEIN + (Position::FADEOUT - Position::FADEIN) * ratio;
	SetPosition(pos);

	// �s�����x�X�V
	m_pFace->SetAlpha(1.0f - ratio);
	m_pText->SetAlpha(1.0f - ratio);

	// �t�F�[�h�A�E�g
	CCharmText::StateFadeOut();
}

//==========================================================================
// �`�F�C���̊m�F
//==========================================================================
void CCharmText_Left::CheckChain()
{
	std::vector<int> chainIdx;	// �`�F�C������C���f�b�N�X

	// ���ɑ��݂��Ă�����̂���m�F
	CListManager<CCharmText_Left>::Iterator itr = m_List.GetEnd();
	while (m_List.ListLoop(itr))
	{// ���[�v

		// �|�C���^�ϊ�
		CCharmText_Left* pText = (*itr);

		// �`�F�C���\�t���O�擾
		bool bPossibleChain = pText->IsPossibleChain();

		if (bPossibleChain)
		{// �`�F�C���\�Ȃ�ƁA�����������Ă���S�Ăƃ`�F�C������

			// ���Ƀ`�F�C�����Ă����Ƃ����̃C���f�b�N�X�擾
			chainIdx = pText->GetChainIdx();
			int pairIdx = pText->GetMyChainIdx();
			
			// �C���f�b�N�X�ǉ�
			chainIdx.push_back(pairIdx);

			// �����`�F�C���o���Ȃ�����
			pText->SetEnablePossibleChain(false);

			// TODO : �`�F�C�����o

			break;
		}
	}

	// �`�F�C���C���f�b�N�X���蓖��
	m_nVecChainIdx = chainIdx;
}
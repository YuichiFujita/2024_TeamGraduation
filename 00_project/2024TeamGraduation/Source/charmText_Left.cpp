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
	const std::string TEXTURE_FACE = "data\\TEXTURE\\faceicon\\000.png";			// ��A�C�R���̃e�N�X�`��
	const std::string TEXTURE_TEXT = "data\\TEXTURE\\charmtext\\window_full.png";	// �e�L�X�g�̃e�N�X�`��

}

namespace Position
{
	const MyLib::Vector3 START(-100.0f, 600.0f, 0.0f);	// �J�n�ʒu
	const MyLib::Vector3 FADEIN(60.0f, 600.0f, 0.0f);	// �t�F�[�h�C���ʒu
	const MyLib::Vector3 FADEOUT(0.0f, -80.0f, 0.0f);	// �I���ʒu
}

namespace StateTime
{
	const float FADEIN = 0.2f;								// �t�F�[�h�C��
	const float WAIT = CCharmManager::TIME_CHAIN - FADEIN;	// �ҋ@
	const float FADEOUT = 0.2f;								// �t�F�[�h�C��
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
		m_pText->SetPosition(Position::START + m_pFace->GetSize().x);
	}

	// ���ɑ��݂��Ă�����̂��グ��
	CListManager<CCharmText_Left>::Iterator itr = m_List.GetEnd();
	while (m_List.ListLoop(itr))
	{// ���[�v
		(*itr)->AddCountUP(1);
	}

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

	// �Ԋu����ɂ�����
	pos.y -= DISTANCE_UP * m_nCntUp;

	m_pFace->SetPosition(pos);
	m_pText->SetPosition(pos + MyLib::Vector3(m_pFace->GetSize().x, 0.0f, 0.0f));
}

//==========================================================================
// �t�F�[�h�C��
//==========================================================================
void CCharmText_Left::StateFadeIn()
{
	// ����
	float ratio = UtilFunc::Correction::EaseInExpo(0.0f, 1.0f, 0.0f, STATETIME_FADEIN, m_fStateTime);

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
	MyLib::Vector3 pos = Position::FADEIN;
	pos.y += Position::FADEOUT.y * ratio;
	SetPosition(pos);

	// �s�����x�X�V
	m_pFace->SetAlpha(1.0f - ratio);
	m_pText->SetAlpha(1.0f - ratio);

	// �t�F�[�h�A�E�g
	CCharmText::StateFadeOut();
}

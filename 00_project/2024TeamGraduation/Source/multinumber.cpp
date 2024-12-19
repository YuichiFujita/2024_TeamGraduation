//=============================================================================
// 
//  �������� [multinumber.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "multinumber.h"
#include "number.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "objectBillboard.h"
#include "texture.h"
#include "calculation.h"
#include "camera.h"

//==========================================================================
// �}�N����`
//==========================================================================
namespace
{
	const std::string DEFAULT_TEXTURE = "data\\TEXTURE\\number_blackclover_01.png";	// �e�N�X�`���̃t�@�C��
	const float TEX_U = 0.1f;	// ������
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CMultiNumber::CMultiNumber(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	m_nNum = 0;				// ����
	m_AlignmentType = EAlignmentType::ALIGNMENT_CENTER;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CMultiNumber::~CMultiNumber()
{

}

//==========================================================================
// ��������
//==========================================================================
CMultiNumber* CMultiNumber::Create(const int nNum, const int texIdx, const MyLib::Vector3& pos, const MyLib::Vector2& size, int nMaxDigit, EObjType objtype, bool bDigitDraw, int nPriority)
{
	// �������̊m��
	CMultiNumber* pNumber = DEBUG_NEW CMultiNumber(nPriority);

	if (pNumber != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �������
		pNumber->m_nNum = nNum;				// ����
		pNumber->m_nIdxTexture = texIdx;	// �e�N�X�`���̃C���f�b�N�X
		pNumber->m_objType = objtype;		// �I�u�W�F�N�g�̎��
		pNumber->m_size = size;				// �T�C�Y
		pNumber->m_nMaxDigit = nMaxDigit;	// ����
		pNumber->m_bDigitDraw = bDigitDraw;	// �����`��

		// ����������
		pNumber->Init();

		pNumber->SetPosition(pos);			// �ʒu
	}

	return pNumber;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CMultiNumber::Init()
{
	// �������N���A
	m_vecNumber.clear();

	// ���擾
	MyLib::Vector3 pos = GetPosition();	// �ʒu
	MyLib::Vector3 rot = GetRotation();	// ����
	int nPriority = GetPriority();		// �D�揇��

	// ��������
	CNumber* pNumber = nullptr;
	for (int i = 0; i < m_nMaxDigit; i++)
	{
		// ��������
		pNumber = CNumber::Create(m_objType, nPriority);

		// �������s
		if (pNumber == nullptr) return S_OK;

		// ���ݒ�
		pNumber->SetPosition(pos);
		pNumber->SetOriginPosition(pos);
		pNumber->SetRotation(rot);
		pNumber->SetOriginRotation(pos);
		pNumber->SetSize(m_size);
		pNumber->SetSizeOrigin(m_size);

		// �e�N�X�`�����蓖��
		pNumber->BindTexture(m_nIdxTexture);

		// �R���e�i�֒ǉ�
		m_vecNumber.push_back(pNumber);
	}

	// �l�ݒ�
	SetNum(m_nNum);

	// ����
	m_nDigit = UtilFunc::Calculation::GetDigit(m_nNum);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CMultiNumber::Uninit()
{
	// �I��
	for (const auto& number : m_vecNumber)
	{
		if (number == nullptr) continue;
		number->Uninit();
	}
	m_vecNumber.clear();

	Release();
}

//==========================================================================
// �폜����
//==========================================================================
void CMultiNumber::Kill()
{
	// �폜
	for (const auto& number : m_vecNumber)
	{
		if (number == nullptr) continue;
		number->Kill();
	}
	m_vecNumber.clear();

	// �I������
	Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CMultiNumber::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ����
	m_nDigit = UtilFunc::Calculation::GetDigit(m_nNum);

	for (const auto& number : m_vecNumber)
	{
		if (number == nullptr ||
			number->GetType() != CObject::TYPE::TYPE_NONE)
		{
			continue;
		}

		// �X�V
		number->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CMultiNumber::Draw()
{
	int i = 1;
	for (const auto& number : m_vecNumber)
	{
		if (number == nullptr ||
			number->GetType() != CObject::TYPE::TYPE_NONE)
		{
			continue;
		}

		if (m_bDigitDraw && m_nMaxDigit - m_nDigit >= i)
		{// �����`�� && �\���������傫��
			i++;
			continue;
		}

		// �`�揈��
		number->Draw();

		i++;
	}
}

//==========================================================================
// �l�ݒ�
//==========================================================================
void CMultiNumber::SetNum(int nNum)
{
	// �l�ݒ�
	m_nNum = nNum;

	for (int i = 0; i < m_nMaxDigit; i++)
	{
		if (m_vecNumber[i] == nullptr) continue;

		// ����̌��̃p�^�[������o��
		int nTexU = m_nNum % (int)std::pow(10, m_nMaxDigit + 1 - i) / ((int)std::pow(10, m_nMaxDigit - i) / 10);
		UtilFunc::Calculation::GetDigit(m_nNum);
		m_vecNumber[i]->SetNum(nTexU);
	}
}

//==========================================================================
// �ʒu����
//==========================================================================
void CMultiNumber::AdjustPosition()
{
	switch (m_AlignmentType)
	{
	case CMultiNumber::ALIGNMENT_LEFT:
		AdjustLeft();
		break;

	case CMultiNumber::ALIGNMENT_RIGHT:
		AdjustRight();
		break;

	case CMultiNumber::ALIGNMENT_CENTER:
		AdjustCenter();
		break;

	default:
		MyAssert::CustomAssert(false, "�ʒu�����ł��Ȃ���");
		break;
	}
}

//==========================================================================
// ������
//==========================================================================
void CMultiNumber::AdjustLeft()
{
	// ���������ւ��炷
	MyLib::Vector3 setpos = GetPosition();
	setpos.x -= m_size.x * 2.0f * m_nMaxDigit;

	for (const auto& number : m_vecNumber)
	{
		// �ʒu�ݒ�
		number->SetPosition(setpos);

		// �ړ����Ă���
		setpos.x += m_size.x * 2.0f;
	}
}

//==========================================================================
// �E����
//==========================================================================
void CMultiNumber::AdjustRight()
{
	// �������E�ւ��炷
	MyLib::Vector3 setpos = GetPosition();
	setpos.x += m_size.x * 2.0f * m_nMaxDigit;

	for (const auto& number : m_vecNumber)
	{
		// �ʒu�ݒ�
		number->SetPosition(setpos);

		// �ړ����Ă���
		setpos.x += m_size.x * 2.0f;
	}
}

//==========================================================================
// ��������
//==========================================================================
void CMultiNumber::AdjustCenter()
{
	// �����̈ʒu�ֈړ�
	MyLib::Vector3 setpos = GetPosition();
	setpos.x -= (m_size.x * 2.0f * m_nMaxDigit) * 0.5f;

	for (const auto& number : m_vecNumber)
	{
		// �ʒu�ݒ�
		number->SetPosition(setpos);

		// �ړ����Ă���
		setpos.x += m_size.x * 2.0f;
	}
}

//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CMultiNumber::SetPosition(const MyLib::Vector3& pos)
{
	CObject::SetPosition(pos);

	for (const auto& number : m_vecNumber)
	{
		number->SetPosition(pos);
	}

	// �ʒu����
	AdjustPosition();
}

//==========================================================================
// �ߋ��̈ʒu�ݒ�
//==========================================================================
void CMultiNumber::SetOldPosition(const MyLib::Vector3& pos)
{
	CObject::SetOldPosition(pos);

	for (const auto& number : m_vecNumber)
	{
		number->SetOldPosition(pos);
	}
}

//==========================================================================
// ���̈ʒu�ݒ�
//==========================================================================
void CMultiNumber::SetOriginPosition(const MyLib::Vector3& pos)
{
	CObject::SetOriginPosition(pos);

	for (const auto& number : m_vecNumber)
	{
		number->SetOriginPosition(pos);
	}
}

//==========================================================================
// �����ݒ�
//==========================================================================
void CMultiNumber::SetRotation(const MyLib::Vector3& rot)
{
	CObject::SetRotation(rot);

	for (const auto& number : m_vecNumber)
	{
		number->SetRotation(rot);
	}
}

//==========================================================================
// �O��̌����ݒ�
//==========================================================================
void CMultiNumber::SetOldRotation(const MyLib::Vector3& rot)
{
	CObject::SetOldRotation(rot);

	for (const auto& number : m_vecNumber)
	{
		number->SetOldRotation(rot);
	}
}

//==========================================================================
// ���̌����ݒ�
//==========================================================================
void CMultiNumber::SetOriginRotation(const MyLib::Vector3& rot)
{
	CObject::SetOriginRotation(rot);

	for (const auto& number : m_vecNumber)
	{
		number->SetOriginRotation(rot);
	}
}

//==========================================================================
// �T�C�Y�ݒ�
//==========================================================================
void CMultiNumber::SetSize(const MyLib::Vector2& size)
{
	m_size = size;

	for (const auto& number : m_vecNumber)
	{
		number->SetSize(size);
	}
}

//==========================================================================
// ���̃T�C�Y�ݒ�
//==========================================================================
void CMultiNumber::SetSizeOrigin(const MyLib::Vector2& size)
{
	m_sizeOrigin = size;

	for (const auto& number : m_vecNumber)
	{
		number->SetSizeOrigin(size);
	}
}

//==========================================================================
// �F�ݒ�
//==========================================================================
void CMultiNumber::SetColor(const D3DXCOLOR& col)
{
	m_col = col;

	for (const auto& number : m_vecNumber)
	{
		number->SetColor(col);
	}
}

//==========================================================================
// ���̐F�ݒ�
//==========================================================================
void CMultiNumber::SetOriginColor(const D3DXCOLOR& col)
{
	m_colOrigin = col;

	for (const auto& number : m_vecNumber)
	{
		number->SetOriginColor(col);
	}
}

//==========================================================================
// �s�����x�ݒ�
//==========================================================================
void CMultiNumber::SetAlpha(const float alpha)
{
	m_col.a = alpha;

	for (const auto& number : m_vecNumber)
	{
		number->SetAlpha(alpha);
	}
}

//==========================================================================
// �e�N�X�`�����蓖��
//==========================================================================
void CMultiNumber::BindTexture(int nIdxTexture)
{
	// �e�N�X�`�����蓖��
	m_nIdxTexture = nIdxTexture;

	for (const auto& number : m_vecNumber)
	{
		// �ʒu�ݒ�
		number->BindTexture(nIdxTexture);
	}
}

//==========================================================================
// �񂹎�ސݒ�
//==========================================================================
void CMultiNumber::SetAlignmentType(EAlignmentType type)
{
	m_AlignmentType = type;
	SetPosition(GetPosition());
}

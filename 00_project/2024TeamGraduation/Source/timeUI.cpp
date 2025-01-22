//============================================================
//
//	�^�C��UI���� [timeUI.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "timeUI.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "timer.h"
#include "object2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEXTURE_NUM = "data\\TEXTURE\\number\\school03.png";	// �����e�N�X�`��
	const char *TEXTURE_PART = "data\\TEXTURE\\timepart000.png";		// ��؂�e�N�X�`��
	const int PRIORITY = 6;	// �^�C��UI�̗D�揇��
}

//************************************************************
//	�q�N���X [CTimeUI] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTimeUI::CTimeUI() : CObject(PRIORITY, CObject::LAYER::LAYER_2D),
	m_sizeValue		(VEC3_ZERO),				// �����̑傫��
	m_sizePart		(VEC3_ZERO),				// ��؂�̑傫��
	m_spaceValue	(VEC3_ZERO),				// �����̋�
	m_spacePart		(VEC3_ZERO),				// ��؂�̋�
	m_col			(MyLib::color::White()),	// �F
	m_alignX		(XALIGN_CENTER),			// ���z�u
	m_alignY		(YALIGN_CENTER),			// �c�z�u
	m_fTime			(0.0f)						// �\������
{
	// �����o�ϐ����N���A
	memset(&m_apValue[0], 0, sizeof(m_apValue));	// ���l�̏��
	memset(&m_apPart[0], 0, sizeof(m_apPart));		// ��؂�̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CTimeUI::~CTimeUI()
{

}

//============================================================
//	����������
//============================================================
HRESULT CTimeUI::Init(void)
{
	// �����o�ϐ���������
	memset(&m_apValue[0], 0, sizeof(m_apValue));	// ���l�̏��
	memset(&m_apPart[0], 0, sizeof(m_apPart));		// ��؂�̏��
	m_sizeValue		= VEC2_ZERO;					// �����̑傫��
	m_sizePart		= VEC2_ZERO;					// ��؂�̑傫��
	m_spaceValue	= VEC2_ZERO;					// �����̋�
	m_spacePart		= VEC2_ZERO;					// ��؂�̋�
	m_col			= MyLib::color::White();		// �F
	m_alignX		= XALIGN_CENTER;				// ���z�u
	m_alignY		= YALIGN_CENTER;				// �c�z�u
	m_fTime			= 0.0f;							// �\������

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // �����̐����J��Ԃ�

		// �����̐���
		m_apValue[nCntValue] = CNumber::Create(CMultiNumber::EObjType::OBJTYPE_2D, PRIORITY);
		if (m_apValue[nCntValue] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �����̃e�N�X�`��������
		m_apValue[nCntValue]->BindTexture(CTexture::GetInstance()->Regist(TEXTURE_NUM));

		// �����̐ݒ�
		m_apValue[nCntValue]->SetNum(0);
	}

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // ��؂�̐����J��Ԃ�

		// ��؂�̐���
		m_apPart[nCntPart] = CObject2D::Create(PRIORITY);
		if (m_apPart[nCntPart] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// ��؂�̃e�N�X�`��������
		m_apPart[nCntPart]->BindTexture(CTexture::GetInstance()->Regist(TEXTURE_PART));
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CTimeUI::Uninit(void)
{
	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // �����̐����J��Ԃ�

		// �����̏I��
		SAFE_UNINIT(m_apValue[nCntValue]);
	}

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // ��؂�̐����J��Ԃ�

		// ��؂�̏I��
		SAFE_UNINIT(m_apPart[nCntPart]);
	}

	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}

//============================================================
// ���I�폜����
//============================================================
void CTimeUI::Kill()
{
	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // �����̐����J��Ԃ�

		// �����̏I��
		SAFE_UNINIT(m_apValue[nCntValue]);
	}

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // ��؂�̐����J��Ԃ�

		// ��؂�̏I��
		SAFE_UNINIT(m_apPart[nCntPart]);
	}

	// ���g�̏I��
	CTimeUI::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CTimeUI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���Έʒu�̐ݒ�
	SetPositionRelative();

	// �����̃e�N�X�`�����W�̐ݒ�
	SetTexNum();
}

//============================================================
//	�`�揈��
//============================================================
void CTimeUI::Draw(void)
{

}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CTimeUI::SetEnableDisp(const bool bDisp)
{
	// ���g�̕`��󋵂�ݒ�
	CObject::SetEnableDisp(bDisp);

	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // �����̐����J��Ԃ�

		// �����̕`��󋵂�ݒ�
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->SetEnableDisp(bDisp);
	}

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // ��؂�̐����J��Ԃ�

		// ��؂�̕`��󋵂�ݒ�
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->SetEnableDisp(bDisp);
	}
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CTimeUI::SetPosition(const MyLib::Vector3& pos)
{
	// ���g�̈ʒu��ݒ�
	CObject::SetPosition(pos);

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CTimeUI::SetRotation(const MyLib::Vector3& rot)
{
	// ���g�̌�����ݒ�
	CObject::SetRotation(rot);

	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // �����̐����J��Ԃ�

		// �����̌�����ݒ�
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->SetRotation(rot);
	}

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // ��؂�̐����J��Ԃ�

		// ��؂�̌�����ݒ�
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->SetRotation(rot);
	}

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	��������
//============================================================
CTimeUI *CTimeUI::Create
(
	const float fTime,				// �\������
	const D3DXVECTOR3& rPos,		// �ʒu
	const float fHeightValue,		// �����̑傫��
	const float fHeightPart,		// ��؂�̑傫��
	const D3DXVECTOR2& rSpaceValue,	// �����̋�
	const D3DXVECTOR2& rSpacePart,	// ��؂�̋�
	const EAlignX alignX,			// ���z�u
	const EAlignY alignY,			// �c�z�u
	const D3DXVECTOR3& rRot,		// ����
	const D3DXCOLOR& rCol			// �F
)
{
	// �^�C��UI�̐���
	CTimeUI *pTimeUI = DEBUG_NEW CTimeUI;
	if (pTimeUI == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �^�C��UI�̏�����
		if (FAILED(pTimeUI->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �^�C��UI�̔j��
			SAFE_DELETE(pTimeUI);
			return nullptr;
		}

		// �\�����Ԃ�ݒ�
		pTimeUI->SetTime(fTime);

		// ���_�ʒu��ݒ�
		pTimeUI->SetPosition(rPos);

		// ���_������ݒ�
		pTimeUI->SetRotation(rRot);

		// �����̑傫����ݒ�
		pTimeUI->SetSizeValue(fHeightValue);

		// ��؂�̑傫����ݒ�
		pTimeUI->SetSizePart(fHeightPart);

		// �����̋󔒂�ݒ�
		pTimeUI->SetSpaceValue(rSpaceValue);

		// ��؂�̋󔒂�ݒ�
		pTimeUI->SetSpacePart(rSpacePart);

		// �F��ݒ�
		pTimeUI->SetColor(rCol);

		// ���z�u��ݒ�
		pTimeUI->SetAlignX(alignX);

		// �c�z�u��ݒ�
		pTimeUI->SetAlignY(alignY);

		// �m�ۂ����A�h���X��Ԃ�
		return pTimeUI;
	}
}

//============================================================
//	�\�����Ԃ̐ݒ菈��
//============================================================
void CTimeUI::SetTime(const float fTime)
{
	// �����̕\�����Ԃ�ݒ�
	m_fTime = fTime;

	// ���Ԃ�␳
	UtilFunc::Transformation::ValueNormalize(m_fTime, timer::TIME_MAX, timer::TIME_MIN);
}

//============================================================
//	�����̃e�N�X�`���C���f�b�N�X�̐ݒ菈��
//============================================================
void CTimeUI::BindTextureValue(const int nTexIdx)
{
	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // �����̐����J��Ԃ�

		// �����̃e�N�X�`���C���f�b�N�X��ݒ�
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->BindTexture(nTexIdx);
	}
}

//============================================================
//	��؂�̃e�N�X�`���C���f�b�N�X�̐ݒ菈��
//============================================================
void CTimeUI::BindTexturePart(const int nTexIdx)
{
	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // ��؂�̐����J��Ԃ�

		// ��؂�̃e�N�X�`���C���f�b�N�X��ݒ�
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->BindTexture(nTexIdx);
	}
}

//============================================================
//	�����̑傫���̐ݒ菈��
//============================================================
void CTimeUI::SetSizeValue(const float fHeight)
{
	// �c�������ɃT�C�Y��ݒ�
	const int nTexID = m_apValue[0]->GetIdxTexture();
	MyLib::Vector2 size = CTexture::GetInstance()->GetImageSize(nTexID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, fHeight);
	size.y *= 10.0f;

	// �ݒ肳�ꂽ�����̑傫����ۑ�
	m_sizeValue = size;

	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // �����̐����J��Ԃ�

		// �����̑傫����ݒ�
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->SetSize(size);
	}

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	��؂�̑傫���̐ݒ菈��
//============================================================
void CTimeUI::SetSizePart(const float fHeight)
{
	// �c�������ɃT�C�Y��ݒ�
	const int nTexID = m_apPart[0]->GetIdxTexture();
	MyLib::Vector2 size = CTexture::GetInstance()->GetImageSize(nTexID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, fHeight);

	// �ݒ肳�ꂽ��؂�̑傫����ۑ�
	m_sizePart = size;

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // ��؂�̐����J��Ԃ�

		// ��؂�̑傫����ݒ�
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->SetSize(size);
	}

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�����̑傫���̐ݒ菈��
//============================================================
void CTimeUI::SetSizeValue(const D3DXVECTOR2& rSize)
{
	// �ݒ肳�ꂽ�����̑傫����ۑ�
	m_sizeValue = rSize;

	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // �����̐����J��Ԃ�

		// �����̑傫����ݒ�
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->SetSize(rSize);
	}

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	��؂�̑傫���̐ݒ菈��
//============================================================
void CTimeUI::SetSizePart(const D3DXVECTOR2& rSize)
{
	// �ݒ肳�ꂽ��؂�̑傫����ۑ�
	m_sizePart = rSize;

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // ��؂�̐����J��Ԃ�

		// ��؂�̑傫����ݒ�
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->SetSize(rSize);
	}

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�����̋󔒂̐ݒ菈��
//============================================================
void CTimeUI::SetSpaceValue(const D3DXVECTOR2& rSpace)
{
	// �����̐����̋󔒂�ݒ�
	m_spaceValue = rSpace;

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	��؂�̋󔒂̐ݒ菈��
//============================================================
void CTimeUI::SetSpacePart(const D3DXVECTOR2& rSpace)
{
	// �����̋�؂�̋󔒂�ݒ�
	m_spacePart = rSpace;

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CTimeUI::SetColor(const D3DXCOLOR& rCol)
{
	// �ݒ肳�ꂽ�F��ۑ�
	m_col = rCol;

	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // �����̐����J��Ԃ�

		// �����̐F��ݒ�
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->SetColor(rCol);
	}

	for (int nCntPart = 0; nCntPart < timeUI::MAX_PART; nCntPart++)
	{ // ��؂�̐����J��Ԃ�

		// ��؂�̐F��ݒ�
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->SetColor(rCol);
	}
}

//============================================================
//	�����x�̐ݒ菈��
//============================================================
void CTimeUI::SetAlpha(const float fAlpha)
{
	// �����x��ݒ�
	D3DXCOLOR col = GetColor();	// �F
	col.a = fAlpha;

	// �F�̐ݒ�
	SetColor(col);
}

//============================================================
//	���z�u�̐ݒ菈��
//============================================================
void CTimeUI::SetAlignX(const EAlignX align)
{
	// �����̉��z�u��ݒ�
	m_alignX = align;

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�c�z�u�̐ݒ菈��
//============================================================
void CTimeUI::SetAlignY(const EAlignY align)
{
	// �����̏c�z�u��ݒ�
	m_alignY = align;

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�^�C���S�̂̉����擾����
//============================================================
float CTimeUI::GetTimeWidth(void) const
{
	float fTimeWidth = 0.0f;				// �^�C���S�̂̉���
	int nEndNumID = timeUI::MAX_DIGIT - 1;	// �I�[�����̃C���f�b�N�X

	// �S�Ă̋󔒂����Z (�Ō�̕����͊܂܂Ȃ�)
	fTimeWidth += m_spaceValue.x * nEndNumID;
	fTimeWidth += m_spacePart.x * timeUI::MAX_PART;

	// �擪�����̉��������Z
	assert(m_apValue[0] != nullptr);
	fTimeWidth += m_apValue[0]->GetSize().x * 0.5f;

	// �I�[�����̉��������Z
	assert(m_apValue[nEndNumID] != nullptr);
	fTimeWidth += m_apValue[nEndNumID]->GetSize().x * 0.5f;

	// �^�C���S�̂̉�����Ԃ�
	return fTimeWidth;
}

//============================================================
//	�^�C���S�̂̏c���擾����
//============================================================
float CTimeUI::GetTimeHeight(void) const
{
	float fTimeHeight = 0.0f;				// �^�C���S�̂̏c��
	int nEndNumID = timeUI::MAX_DIGIT - 1;	// �I�[�����̃C���f�b�N�X

	// �S�Ă̋󔒂����Z (�Ō�̕����͊܂܂Ȃ�)
	fTimeHeight += m_spaceValue.y * nEndNumID;
	fTimeHeight += m_spacePart.y * timeUI::MAX_PART;

	// �擪�����̏c�������Z
	assert(m_apValue[0] != nullptr);
	fTimeHeight += m_apValue[0]->GetSize().y * 0.5f;

	// �I�[�����̏c�������Z
	assert(m_apValue[nEndNumID] != nullptr);
	fTimeHeight += m_apValue[nEndNumID]->GetSize().y * 0.5f;

	// �^�C���S�̂̏c����Ԃ�
	return fTimeHeight;
}

//============================================================
//	�����S�̂̑傫���擾����
//============================================================
D3DXVECTOR2 CTimeUI::GetTimeSize(void) const
{
	// �����S�̂̑傫����Ԃ�
	return D3DXVECTOR2(GetTimeWidth(), GetTimeHeight());
}

//============================================================
//	���Έʒu�̐ݒ菈��
//============================================================
void CTimeUI::SetPositionRelative(void)
{
	int nValueID = 0;	// �����̐�����
	int nPartID = 0;	// ��؂�̐�����
	D3DXVECTOR3 rotThis = GetRotation();	// ���g�̌���
	D3DXVECTOR2 spaceValue = m_spaceValue * 0.5f;	// �����̋�
	D3DXVECTOR2 spacePart = m_spacePart * 0.5f;		// ��؂�̋�
	D3DXVECTOR2 sizeTime = GetTimeSize() * 0.5f;	// �^�C���S�̂̑傫��
	D3DXVECTOR2 sizeHead = m_apValue[0]->GetSize() * 0.5f;	// �擪�����̑傫��
	D3DXVECTOR3 rotStart = D3DXVECTOR3(rotThis.z + HALF_PI, rotThis.z, 0.0f);	// �����̊J�n����

	D3DXVECTOR3 posOffset = VEC3_ZERO;	// �����̊J�n�I�t�Z�b�g
	posOffset.x = -sizeTime.x - spaceValue.x + sizeHead.x - (sizeTime.x * (m_alignX - 1));	// �J�n�I�t�Z�b�gX
	posOffset.y = -sizeTime.y - spaceValue.y + sizeHead.y - (sizeTime.y * (m_alignY - 1));	// �J�n�I�t�Z�b�gY

	D3DXVECTOR3 posStart = GetPosition();	// �����̊J�n�ʒu
	posStart.x += sinf(rotStart.x) * posOffset.x + sinf(rotStart.y) * posOffset.y;	// �J�n�ʒuX
	posStart.y += cosf(rotStart.x) * posOffset.x + cosf(rotStart.y) * posOffset.y;	// �J�n�ʒuY

	for (int nCntTimer = 0; nCntTimer < timeUI::MAX_DIGIT + timeUI::MAX_PART; nCntTimer++)
	{ // �����̐� + ��؂�̐����J��Ԃ�

		if (nCntTimer == timeUI::MAX_MIN || nCntTimer == timeUI::MAX_MIN + timeUI::MAX_SEC + 1)
		{ // ��؂�^�C�~���O�̏ꍇ

			assert(m_apPart[nValueID] != nullptr);

			// �|���S�������ʒu�����炷
			posStart.x += sinf(rotStart.x) * spacePart.x + sinf(rotStart.y) * spacePart.y;
			posStart.y += cosf(rotStart.x) * spacePart.x + cosf(rotStart.y) * spacePart.y;

			// ��؂�̈ʒu��ݒ�
			m_apPart[nValueID]->SetPosition(posStart);

			// �|���S�������ʒu�����炷
			posStart.x += sinf(rotStart.x) * spacePart.x + sinf(rotStart.y) * spacePart.y;
			posStart.y += cosf(rotStart.x) * spacePart.x + cosf(rotStart.y) * spacePart.y;

			// ��؂�̐����������Z
			nValueID++;
		}
		else
		{ // �����^�C�~���O�̏ꍇ

			assert(m_apValue[nPartID] != nullptr);

			// �|���S�������ʒu�����炷
			posStart.x += sinf(rotStart.x) * spaceValue.x + sinf(rotStart.y) * spaceValue.y;
			posStart.y += cosf(rotStart.x) * spaceValue.x + cosf(rotStart.y) * spaceValue.y;

			// �����̈ʒu��ݒ�
			m_apValue[nPartID]->SetPosition(posStart);

			// �|���S�������ʒu�����炷
			posStart.x += sinf(rotStart.x) * spaceValue.x + sinf(rotStart.y) * spaceValue.y;
			posStart.y += cosf(rotStart.x) * spaceValue.x + cosf(rotStart.y) * spaceValue.y;

			// �����̐����������Z
			nPartID++;
		}
	}
}

//============================================================
//	�����̃e�N�X�`�����W�̐ݒ菈��
//============================================================
void CTimeUI::SetTexNum(void)
{
	int aNumDivide[timeUI::MAX_DIGIT];	// ���l����p

	// �����������Ƃɕ���
	UtilFunc::Transformation::DivideDigitNum(&aNumDivide[0], GetMin(), timeUI::MAX_MIN);

	// �b���������Ƃɕ���
	UtilFunc::Transformation::DivideDigitNum(&aNumDivide[timeUI::MAX_MIN], GetSec(), timeUI::MAX_SEC);

	// �~���b���������Ƃɕ���
	UtilFunc::Transformation::DivideDigitNum(&aNumDivide[timeUI::MAX_MIN + timeUI::MAX_SEC], GetMSec(), timeUI::MAX_MSEC);

	for (int nCntValue = 0; nCntValue < timeUI::MAX_DIGIT; nCntValue++)
	{ // �����̐����J��Ԃ�

		// �����̐ݒ�
		m_apValue[nCntValue]->SetNum(aNumDivide[nCntValue]);
	}
}

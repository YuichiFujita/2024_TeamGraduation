//============================================================
//
//	������2D���� [string2D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "string2D.h"
#include "manager.h"
#include "font.h"
#include "char2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 6;	// ������2D�̗D�揇��
}

//************************************************************
//	�q�N���X [CString2D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CString2D::CString2D() : CObject(PRIORITY, LAYER::LAYER_2D),
	m_ppChar		(nullptr),					// �����|���S���̏��
	m_pFontChar		(nullptr),					// �t�H���g����
	m_col			(MyLib::color::White()),	// �F
	m_alignX		(XALIGN_CENTER),			// ���z�u
	m_fCharHeight	(0.0f)						// �����̏c��
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CString2D::~CString2D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CString2D::Init(void)
{
	// �����o�ϐ���������
	m_ppChar		= nullptr;					// �����|���S���̏��
	m_pFontChar		= nullptr;					// �t�H���g����
	m_col			= MyLib::color::White();	// �F
	m_alignX		= XALIGN_CENTER;			// ���z�u
	m_fCharHeight	= 0.0f;						// �����̏c��

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CString2D::Uninit(void)
{
	if (m_ppChar != nullptr)
	{ // �����񂪔j������Ă��Ȃ��ꍇ

		for (int i = 0; i < (int)m_wsStr.size(); i++)
		{ // ���������J��Ԃ�

			// �����̔j��
			SAFE_UNINIT(m_ppChar[i]);
		}
	}

	// ������̔j��
	SAFE_DEL_ARRAY(m_ppChar);

	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}

//============================================================
//	�j������
//============================================================
void CString2D::Release(void)
{
	// ���g�̏I��
	CString2D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CString2D::Update(const float fDeltaTime)
{
	// ���Έʒu�̐ݒ�
	SetPositionRelative();

	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // ���������J��Ԃ�

		// �����̍X�V
		assert(m_ppChar[i] != nullptr);
		m_ppChar[i]->Update(fDeltaTime);
	}
}

//============================================================
//	�`�揈��
//============================================================
void CString2D::Draw(void)
{
	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // ���������J��Ԃ�

		// �����̕`��
		assert(m_ppChar[i] != nullptr);
		m_ppChar[i]->Draw();
	}
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CString2D::SetEnableDisp(const bool bDraw)
{
	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // ���������J��Ԃ�

		// �����̕`��󋵂�ݒ�
		m_ppChar[i]->SetEnableDisp(bDraw);
	}
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CString2D::SetPosition(const MyLib::Vector3& pos)
{
	// �ݒ肷��ʒu��ۑ�
	CObject::SetPosition(pos);

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CString2D::SetRotation(const MyLib::Vector3& rot)
{
	// �ݒ肷�������ۑ�
	CObject::SetRotation(rot);

	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // ���������J��Ԃ�

		// ���������̐ݒ�
		assert(m_ppChar[i] != nullptr);
		m_ppChar[i]->SetRotation(rot);
	}

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	��������
//============================================================
CString2D *CString2D::Create
(
	const std::string &rFilePass,	// �t�H���g�p�X
	const bool bItalic,				// �C�^���b�N
	const std::wstring &rStr,		// �w�蕶����
	const D3DXVECTOR3 &rPos,		// ���_�ʒu
	const float fHeight,			// �����c��
	const EAlignX alignX,			// ���z�u
	const D3DXVECTOR3& rRot,		// ���_����
	const D3DXCOLOR& rCol			// �F
)
{
	// ������2D�̐���
	CString2D *pString2D = new CString2D;
	if (pString2D == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ������2D�̏�����
		if (FAILED(pString2D->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ������2D�̔j��
			SAFE_DELETE(pString2D);
			return nullptr;
		}

		// �t�H���g��ݒ�
		pString2D->SetFont(rFilePass, bItalic);

		// �������ݒ�
		if (FAILED(pString2D->SetString(rStr)))
		{ // �ݒ�Ɏ��s�����ꍇ

			// ������2D�̔j��
			SAFE_DELETE(pString2D);
			return nullptr;
		}

		// ���_�ʒu��ݒ�
		pString2D->SetPosition(rPos);

		// ���_������ݒ�
		pString2D->SetRotation(rRot);

		// �F��ݒ�
		pString2D->SetColor(rCol);

		// �����c����ݒ�
		pString2D->SetCharHeight(fHeight);

		// ���z�u��ݒ�
		pString2D->SetAlignX(alignX);

		// �m�ۂ����A�h���X��Ԃ�
		return pString2D;
	}
}

//============================================================
//	������̐ݒ菈��
//============================================================
HRESULT CString2D::SetString(const std::wstring& rStr)
{
	// ��������ۑ�����
	int nOldStrLen = (int)m_wsStr.size();	// �j�����镶����
	int nCurStrLen = (int)rStr.size();		// �������镶����

	// �w�蕶�����ύX
	m_wsStr = rStr;

	//--------------------------------------------------------
	//	���Ɏg�p���Ă��镶����j��
	//--------------------------------------------------------
	for (int i = 0; i < nOldStrLen; i++)
	{ // �j�����镶�������J��Ԃ�

		// ������j��
		SAFE_UNINIT(m_ppChar[i]);
	}

	// �������j��
	SAFE_DEL_ARRAY(m_ppChar);

	//--------------------------------------------------------
	//	�V���������𐶐�
	//--------------------------------------------------------
	// �������i�[����z��𐶐�
	m_ppChar = new CChar2D*[nCurStrLen];
	for (int i = 0; i < nCurStrLen; i++)
	{ // �������镶�������J��Ԃ�

		// �����𐶐�
		m_ppChar[i] = CChar2D::Create
		( // ����
			m_pFontChar->GetFilePass(),	// �t�H���g�p�X
			m_pFontChar->GetItalic(),	// �C�^���b�N
			m_wsStr[i],		// �w�蕶��
			GetPosition(),	// ���_�ʒu
			m_fCharHeight,	// �����c��
			GetRotation(),	// ���_����
			m_col			// �F
		);
		if (m_ppChar[i] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �����̎�ނ��w��Ȃ��ɂ���
		m_ppChar[i]->SetType(CObject::TYPE::TYPE_NONE);
	}

	// ���Έʒu�̐ݒ�
	SetPositionRelative();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�t�H���g�̐ݒ菈��
//============================================================
void CString2D::SetFont
(
	const std::string &rFilePass,	// �t�H���g�p�X
	const bool bItalic				// �C�^���b�N
)
{
	// �t�H���g��������ݒ�
	CFont *pFont = GET_MANAGER->GetFont();	// �t�H���g���
	m_pFontChar = pFont->Regist(rFilePass, bItalic).pFontChar;

	// ������̍Đݒ�
	SetString(m_wsStr);
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CString2D::SetColor(const D3DXCOLOR& rCol)
{
	// �ݒ肷��F��ۑ�
	m_col = rCol;

	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // ���������J��Ԃ�

		// �����F�̐ݒ�
		assert(m_ppChar[i] != nullptr);
		m_ppChar[i]->SetColor(rCol);
	}
}

//============================================================
//	�����x�̐ݒ菈��
//============================================================
void CString2D::SetAlpha(const float fAlpha)
{
	// �����̓����x����
	m_col.a = fAlpha;

	// �F�̐ݒ�
	SetColor(m_col);
}

//============================================================
//	�����c���̐ݒ菈��
//============================================================
void CString2D::SetCharHeight(const float fHeight)
{
	// �ݒ肷�镶���̏c����ۑ�
	m_fCharHeight = fHeight;

	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // ���������J��Ԃ�

		// �����c���̐ݒ�
		assert(m_ppChar[i] != nullptr);
		m_ppChar[i]->SetCharHeight(fHeight);
	}

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	���z�u�̐ݒ菈��
//============================================================
void CString2D::SetAlignX(const CString2D::EAlignX alignX)
{
	// �����̉��z�u��ݒ�
	m_alignX = alignX;

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	������̉����擾����
//============================================================
float CString2D::GetStrWidth(void) const
{
	// �����񂪂Ȃ��ꍇ������
	if ((int)m_wsStr.size() <= 0) { assert(false); return 0.0f; }

	float fStrWidth = 0.0f;	// ������̉���
	int nEndCharID = (int)m_wsStr.size() - 1;	// �I�[�����̃C���f�b�N�X

	for (int i = 0; i < nEndCharID; i++)
	{ // �I�[�����𔲂������������J��Ԃ�

		// ���̕����܂ł̋��������Z
		assert(m_ppChar[i] != nullptr);
		fStrWidth += m_ppChar[i]->GetNext();
	}

	// �擪�����̃T�C�Y���u���b�N�{�b�N�X������̑傫���ɕ␳
	assert(m_ppChar[0] != nullptr);
	float fHeadWidth = m_ppChar[0]->GetSize().x * 0.5f;				// �擪�����̉���
	fStrWidth += fHeadWidth - m_ppChar[0]->GetOffsetOrigin();		// ���_���O�̋�Ԃ����Z
	fStrWidth -= fHeadWidth + m_ppChar[0]->GetOffsetBlackBoxLU().x;	// �u���b�N�{�b�N�X�J�n���O�̋�Ԃ����Z

	// �I�[�����̃T�C�Y���u���b�N�{�b�N�X�E�܂ł̑傫���ɕ␳
	assert(m_ppChar[nEndCharID] != nullptr);
	float fTailWidth = m_ppChar[nEndCharID]->GetSize().x * 0.5f;				// �I�[�����̉���
	fStrWidth -= fTailWidth - m_ppChar[nEndCharID]->GetOffsetOrigin();			// ���_���O�̋�Ԃ����Z
	fStrWidth += fTailWidth + m_ppChar[nEndCharID]->GetOffsetBlackBoxRD().x;	// �u���b�N�{�b�N�X�I�����O�̋�Ԃ����Z

	// ������̉�����Ԃ�
	return fStrWidth;
}

//============================================================
//	�����̎擾����
//============================================================
CChar2D *CString2D::GetChar2D(const int nCharID) const
{
	// �����񂪂Ȃ��ꍇ������
	int nStrLen = (int)m_wsStr.size();
	if (nStrLen <= 0) { assert(false); return nullptr; }

	// �C���f�b�N�X���͈͊O�̏ꍇ������
	if (nCharID <= -1 || nCharID >= nStrLen) { assert(false); return nullptr; }

	// �����C���f�b�N�X�̕�����Ԃ�
	return m_ppChar[nCharID];
}

//============================================================
//	���Έʒu�̐ݒ菈��
//============================================================
void CString2D::SetPositionRelative(void)
{
	// �����񂪂Ȃ��ꍇ������
	if ((int)m_wsStr.size() <= 0) { return; }

	assert(m_ppChar[0] != nullptr);
	float fHeadOffsetOrigin	= m_ppChar[0]->GetOffsetOrigin();		// �擪�����̌��_�I�t�Z�b�g
	float fHeadOffsetLU	= m_ppChar[0]->GetOffsetBlackBoxLU().x;		// �擪�����̃u���b�N�{�b�N�X�I�t�Z�b�g
	float fHeadRot		= m_ppChar[0]->GetRotation().z - HALF_PI;	// �擪�����̌���
	float fHeadWidth	= m_ppChar[0]->GetSize().x * 0.5f;			// �擪�����̉���
	float fStrWidth		= GetStrWidth() * 0.5f * 2.0f;				// ������S�̂̉���

	float fStartOffset = fStrWidth;	// �����̊J�n�ʒu�I�t�Z�b�g
	fStartOffset -= fHeadWidth - fHeadOffsetOrigin;	// �擪�����̌��_�����炷
	fStartOffset += fHeadWidth + fHeadOffsetLU;		// �擪�����̃u���b�N�{�b�N�X�����炷
	fStartOffset += (fStrWidth * (m_alignX - 1));	// �w�肳�ꂽ���z�u�ɂ���

	D3DXVECTOR3 posStart = GetPosition();	// �����̊J�n�ʒu
	posStart.x += sinf(fHeadRot) * fStartOffset;	// �J�n�ʒuX
	posStart.y += cosf(fHeadRot) * fStartOffset;	// �J�n�ʒuY

	for (int i = 0; i < (int)m_wsStr.size(); i++)
	{ // ���������J��Ԃ�

		assert(m_ppChar[i] != nullptr);
		float fOffset = m_ppChar[i]->GetOffsetOrigin() * 2.0f;	// ���_�I�t�Z�b�g
		float fNext = m_ppChar[i]->GetNext() * 2.0f;			// �������܂ł̋���

		// �ݒ���W�Ɍ��_�I�t�Z�b�g��^����
		posStart.x -= sinf(fHeadRot) * fOffset;
		posStart.y -= cosf(fHeadRot) * fOffset;

		// �ʒu�𔽉f
		m_ppChar[i]->SetPosition(posStart);

		// ���̐ݒ���W�̊J�n�_��ۑ�
		float fNextStart = fOffset - fNext;	// ���̊J�n�_
		posStart.x += sinf(fHeadRot) * fNextStart;
		posStart.y += cosf(fHeadRot) * fNextStart;
	}
}

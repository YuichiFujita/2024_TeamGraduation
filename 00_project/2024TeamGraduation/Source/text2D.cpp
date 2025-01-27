//============================================================
//
//	�e�L�X�g2D���� [text2D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "text2D.h"
#include "manager.h"
#include "font.h"

//************************************************************
//	�q�N���X [CText2D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CText2D::CText2D(const int nPriority) : CObject(nPriority, LAYER::LAYER_2D),
	m_pFontChar		(nullptr),					// �t�H���g����
	m_col			(MyLib::color::White()),	// �F
	m_alignX		(XALIGN_CENTER),			// ���z�u
	m_alignY		(YALIGN_CENTER),			// �c�z�u
	m_fCharHeight	(0.0f),						// �����̏c��
	m_fLineHeight	(0.0f)						// �����̍s��
{
	// �����񃊃X�g���N���A
	m_listString.clear();
}

//============================================================
//	�f�X�g���N�^
//============================================================
CText2D::~CText2D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CText2D::Init()
{
	// �����o�ϐ���������
	m_pFontChar		= nullptr;					// �t�H���g����
	m_col			= MyLib::color::White();	// �F
	m_alignX		= XALIGN_CENTER;			// ���z�u
	m_alignY		= YALIGN_CENTER;			// �c�z�u
	m_fCharHeight	= 0.0f;						// �����̏c��
	m_fLineHeight	= 0.0f;						// �����̍s��

	// �����񃊃X�g��������
	m_listString.clear();

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CText2D::Uninit()
{
	for (auto& rList : m_listString)
	{ // ������̊i�[�����J��Ԃ�

		// ������̏I��
		SAFE_UNINIT(rList);
	}

	// �����񃊃X�g���N���A
	m_listString.clear();

	// �I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�폜����
//============================================================
void CText2D::Kill()
{
	// ���g�̏I��
	CText2D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CText2D::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���Έʒu�̐ݒ�
	SetPositionRelative();

	for (auto& rList : m_listString)
	{ // ������̊i�[�����J��Ԃ�

		// ������̍X�V
		assert(rList != nullptr);
		rList->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//============================================================
//	�`�揈��
//============================================================
void CText2D::Draw()
{
	for (auto& rList : m_listString)
	{ // ������̊i�[�����J��Ԃ�

		// �����񂪂Ȃ��ꍇ�G���[
		assert(rList != nullptr);

		// �����`�悪OFF�̏ꍇ����
		if (!rList->IsDisp()) { continue; }

		// ������̕`��
		rList->Draw();
	}
}

//============================================================
//	��ނ̐ݒ菈��
//============================================================
void CText2D::SetType(const TYPE type)
{
	// ���g�̎�ނ�ݒ�
	CObject::SetType(type);

	for (auto& rList : m_listString)
	{ // ������̊i�[�����J��Ԃ�

		// ������̎�ނ�ݒ�
		rList->SetType(type);
	}
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CText2D::SetEnableDisp(const bool bDisp)
{
	for (auto& rList : m_listString)
	{ // ������̊i�[�����J��Ԃ�

		// ������̕`��󋵂�ݒ�
		rList->SetEnableDisp(bDisp);
	}
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CText2D::SetPosition(const MyLib::Vector3& pos)
{
	// �ݒ肷��ʒu��ۑ�
	CObject::SetPosition(pos);

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CText2D::SetRotation(const MyLib::Vector3& rot)
{
	// �ݒ肷�������ۑ�
	CObject::SetRotation(rot);

	for (auto& rList : m_listString)
	{ // ������̊i�[�����J��Ԃ�

		// ����������̐ݒ�
		assert(rList != nullptr);
		rList->SetRotation(rot);
	}

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	��������
//============================================================
CText2D* CText2D::Create
(
	const std::string& rFilePath,	// �t�H���g�p�X
	const bool bItalic,				// �C�^���b�N
	const MyLib::Vector3& rPos,		// ���_�ʒu
	const float fCharHeight,		// �����c��
	const float fLineHeight,		// �s�ԏc��
	const EAlignX alignX,			// ���z�u
	const EAlignY alignY,			// �c�z�u
	const MyLib::Vector3& rRot,		// ���_����
	const D3DXCOLOR& rCol,			// �F
	const int nPriority				// �D�揇��
)
{
	// �e�L�X�g2D�̐���
	CText2D* pText2D = DEBUG_NEW CText2D(nPriority);
	if (pText2D == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �e�L�X�g2D�̏�����
		if (FAILED(pText2D->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �e�L�X�g2D�̔j��
			SAFE_DELETE(pText2D);
			return nullptr;
		}

		// �t�H���g��ݒ�
		pText2D->SetFont(rFilePath, bItalic);

		// ���_�ʒu��ݒ�
		pText2D->SetPosition(rPos);

		// ���_������ݒ�
		pText2D->SetRotation(rRot);

		// �F��ݒ�
		pText2D->SetColor(rCol);

		// �����c����ݒ�
		pText2D->SetCharHeight(fCharHeight);

		// �s�ԏc����ݒ�
		pText2D->SetLineHeight(fLineHeight);

		// ���z�u��ݒ�
		pText2D->SetAlignX(alignX);

		// �c�z�u��ݒ�
		pText2D->SetAlignY(alignY);

		// �m�ۂ����A�h���X��Ԃ�
		return pText2D;
	}
}

//============================================================
//	������̐擪�ǉ����� (�}���`�o�C�g������)
//============================================================
HRESULT CText2D::PushFrontString(const std::string& rStr)
{
	// ���C�h������ϊ�
	std::wstring wsStr = UtilFunc::Transformation::MultiByteToWide(rStr);

	// �������擪�ɒǉ�
	if (FAILED(PushFrontString(wsStr)))
	{ // �ǉ��Ɏ��s�����ꍇ

		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	������̐擪�ǉ����� (���C�h������)
//============================================================
HRESULT CText2D::PushFrontString(const std::wstring& rStr)
{
	// ��̕�����̏ꍇ������
	if (rStr.empty()) { return E_FAIL; }

	// ������̐���
	CString2D* pStr = CreateString2D(rStr);
	if (pStr == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// �擪�ɐ��������������ǉ�
	m_listString.push_front(pStr);

	// ���Έʒu�̐ݒ�
	SetPositionRelative();

	return S_OK;
}

//============================================================
//	������̍Ō���ǉ����� (�}���`�o�C�g������)
//============================================================
HRESULT CText2D::PushBackString(const std::string& rStr)
{
	// ���C�h������ϊ�
	std::wstring wsStr = UtilFunc::Transformation::MultiByteToWide(rStr);

	// ��������Ō���ɒǉ�
	if (FAILED(PushBackString(wsStr)))
	{ // �ǉ��Ɏ��s�����ꍇ

		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	������̍Ō���ǉ����� (���C�h������)
//============================================================
HRESULT CText2D::PushBackString(const std::wstring& rStr)
{
	// ��̕�����̏ꍇ������
	if (rStr.empty()) { return E_FAIL; }

	// ������̐���
	CString2D* pStr = CreateString2D(rStr);
	if (pStr == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// �Ō���ɐ��������������ǉ�
	m_listString.push_back(pStr);

	// ���Έʒu�̐ݒ�
	SetPositionRelative();

	return S_OK;
}

//============================================================
//	������̍폜����
//============================================================
void CText2D::DeleteString(const int nStrIdx)
{
	// �����񂪂Ȃ��ꍇ������
	int nStrSize = (int)m_listString.size();
	if (nStrSize <= 0) { assert(false); return; }

	// �C���f�b�N�X���͈͊O�̏ꍇ������
	if (nStrIdx <= -1 || nStrIdx >= nStrSize) { assert(false); return; }

	// �C�e���[�^�[���C���f�b�N�X�����炷
	auto itr = m_listString.begin();
	std::advance(itr, nStrIdx);

	// �C�e���[�^�[���̕�����2D���I��
	SAFE_UNINIT(*itr);

	// �C�e���[�^�[�����X�g����폜
	m_listString.erase(itr);
}

//============================================================
//	������̑S�폜����
//============================================================
void CText2D::DeleteStringAll()
{
	for (auto& rList : m_listString)
	{ // ������̊i�[�����J��Ԃ�

		// ������̏I��
		assert(rList != nullptr);
		SAFE_UNINIT(rList);
	}

	// �����񃊃X�g���N���A
	m_listString.clear();
}

//============================================================
//	�t�H���g�̐ݒ菈��
//============================================================
void CText2D::SetFont
(
	const std::string& rFilePath,	// �t�H���g�p�X
	const bool bItalic				// �C�^���b�N
)
{
	// �t�H���g��������ݒ�
	CFont* pFont = GET_MANAGER->GetFont();	// �t�H���g���
	m_pFontChar = pFont->Regist(rFilePath, bItalic).pFontChar;

	for (auto& rList : m_listString)
	{ // ������̊i�[�����J��Ԃ�

		// ������t�H���g�̐ݒ�
		assert(rList != nullptr);
		rList->SetFont(rFilePath, bItalic);
	}

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�����x�̐ݒ菈��
//============================================================
void CText2D::SetAlpha(const float fAlpha)
{
	// �����̓����x��ۑ�
	m_col.a = fAlpha;

	// �F�̐ݒ�
	SetColor(m_col);
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CText2D::SetColor(const D3DXCOLOR& rCol)
{
	// �ݒ肷��F��ۑ�
	m_col = rCol;

	for (auto& rList : m_listString)
	{ // ������̊i�[�����J��Ԃ�

		// ������F�̐ݒ�
		assert(rList != nullptr);
		rList->SetColor(rCol);
	}
}

//============================================================
//	�����c���̐ݒ菈��
//============================================================
void CText2D::SetCharHeight(const float fHeight)
{
	// �ݒ肷�镶���̏c����ۑ�
	m_fCharHeight = fHeight;

	for (auto& rList : m_listString)
	{ // ������̊i�[�����J��Ԃ�

		// ������c���̐ݒ�
		assert(rList != nullptr);
		rList->SetCharHeight(fHeight);
	}

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�s�ԏc���̐ݒ菈��
//============================================================
void CText2D::SetLineHeight(const float fHeight)
{
	// �����̍s�Ԃ�ݒ�
	m_fLineHeight = fHeight;

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	���z�u�̐ݒ菈��
//============================================================
void CText2D::SetAlignX(const EAlignX align)
{
	// �����̉��z�u��ݒ�
	m_alignX = align;

	for (auto& rList : m_listString)
	{ // ������̊i�[�����J��Ԃ�

		// �����񉡕��̐ݒ�
		assert(rList != nullptr);
		rList->SetAlignX(align);
	}

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�c�z�u�̐ݒ菈��
//============================================================
void CText2D::SetAlignY(const EAlignY align)
{
	// �����̏c�z�u��ݒ�
	m_alignY = align;

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	������̎擾����
//============================================================
CString2D* CText2D::GetString2D(const int nStrIdx) const
{
	// �����񂪂Ȃ��ꍇ������
	int nStrSize = (int)m_listString.size();
	if (nStrSize <= 0) { assert(false); return nullptr; }

	// �C���f�b�N�X���͈͊O�̏ꍇ������
	if (nStrIdx <= -1 || nStrIdx >= nStrSize) { assert(false); return nullptr; }

	// �����C���f�b�N�X�̕������Ԃ�
	auto itr = m_listString.begin();	// �擪�C�e���[�^�[
	std::advance(itr, nStrIdx);			// �C�e���[�^�[��i�߂�
	return *itr;						// �C�e���[�^�[�̒��g��Ԃ�
}

//============================================================
//	�e�L�X�g�̉����擾����
//============================================================
float CText2D::GetTextWidth() const
{
	// �����񂪂Ȃ��ꍇ������
	if ((int)m_listString.size() <= 0) { assert(false); return 0.0f; }

	float fMaxWidth = 0.0f;	// ������̍ő剡��
	for (const auto& rList : m_listString)
	{ // �v�f�����J��Ԃ�

		float fWidth = rList->GetStrWidth();	// ������̉���
		if (fWidth > fMaxWidth)
		{ // ���傫�������̏ꍇ

			// �ő剡���̍Đݒ�
			fMaxWidth = fWidth;
		}
	}

	// ������̍ő剡����Ԃ�
	return fMaxWidth;
}

//============================================================
//	�e�L�X�g�̏c���擾����
//============================================================
float CText2D::GetTextHeight() const
{
	// �����񂪂Ȃ��ꍇ������
	if ((int)m_listString.size() <= 0) { assert(false); return 0.0f; }

	float fTextHeight = 0.0f;	// �e�L�X�g�̏c��
	int nEndStrIdx = (int)m_listString.size() - 1;	// �I�[������̃C���f�b�N�X
	for (int i = 0; i < nEndStrIdx; i++)
	{ // �I�[������𔲂��������񐔕��J��Ԃ�

		// ���̕�����܂ł̍s�Ԃ����Z
		fTextHeight += m_fLineHeight;
	}

	// �擪�ƏI�[�̕�����̖������ꂽ�T�C�Y�����Z
	fTextHeight += m_listString.front()->GetCharHeight() * 0.5f;	// �擪������̌��_��T�C�Y
	fTextHeight += m_listString.back()->GetCharHeight() * 0.5f;		// �I�[������̌��_���T�C�Y

	// �e�L�X�g�̏c����Ԃ�
	return fTextHeight;
}

//============================================================
//	������̐�������
//============================================================
CString2D* CText2D::CreateString2D(const std::wstring& rStr)
{
	// ������I�u�W�F�N�g�𐶐�
	CString2D* pStr = CString2D::Create
	( // ����
		m_pFontChar->GetFilePath(),	// �t�H���g�p�X
		m_pFontChar->GetItalic(),	// �C�^���b�N
		rStr,			// �w�蕶����
		GetPosition(),	// ���_�ʒu
		m_fCharHeight,	// �����c��
		m_alignX,		// ���z�u
		GetRotation(),	// ���_����
		m_col			// �F
	);
	if (pStr != nullptr)
	{ // �����ɐ��������ꍇ

		// ������̃��x�����w��Ȃ��ɂ���
		pStr->SetType(CObject::TYPE::TYPE_NONE);
	}

	// ������N���X��Ԃ�
	return pStr;
}

//============================================================
//	���Έʒu�̐ݒ菈��
//============================================================
void CText2D::SetPositionRelative()
{
	// �����񂪂Ȃ��ꍇ������
	if ((int)m_listString.size() <= 0) { return; }

	float fTextHeight	= GetTextHeight() * 0.5f;	// �e�L�X�g�S�̂̏c��
	float fFrontHeight	= m_listString.front()->GetCharHeight() * 0.5f;	// �擪������̏c��
	float fStartOffset	= -fTextHeight + fFrontHeight - (fTextHeight * (m_alignY - 1));	// ������̊J�n�ʒu�I�t�Z�b�g

	D3DXVECTOR3 posThis = GetPosition();	// ���g�̈ʒu
	D3DXVECTOR3 rotThis = GetRotation();	// ���g�̌���
	D3DXVECTOR3 posStart = VEC3_ZERO;		// �����̊J�n�ʒu
	posStart.x = posThis.x + sinf(rotThis.z) * fStartOffset;	// �J�n�ʒuX
	posStart.y = posThis.y + cosf(rotThis.z) * fStartOffset;	// �J�n�ʒuY

	for (auto& rList : m_listString)
	{ // ������̊i�[�����J��Ԃ�

		// �ʒu�𔽉f
		assert(rList != nullptr);
		rList->SetPosition(posStart);

		// ���̐ݒ���W�̊J�n�_��ۑ�
		posStart.x += sinf(rotThis.z) * m_fLineHeight;
		posStart.y += cosf(rotThis.z) * m_fLineHeight;
	}
}

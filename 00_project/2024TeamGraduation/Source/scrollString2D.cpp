//============================================================
//
//	�������蕶����2D���� [scrollString2D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "scrollString2D.h"
#include "manager.h"
#include "string2D.h"
#include "char2D.h"

//************************************************************
//	�q�N���X [CScrollString2D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CScrollString2D::CScrollString2D(const int nPriority) : CString2D(nPriority),
	m_labelSE	(CSound::LABEL_NONE),	// ��������Đ�SE���x��
	m_nNextIdx	(0),	// ���\�����镶���C���f�b�N�X
	m_fNextTime	(0.0f),	// ���\������܂ł̎���
	m_fCurTime	(0.0f),	// ���݂̑ҋ@����
	m_bScroll	(false)	// ���������
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CScrollString2D::~CScrollString2D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CScrollString2D::Init()
{
	// �����o�ϐ���������
	m_labelSE	= CSound::LABEL_NONE;	// ��������Đ�SE���x��
	m_nNextIdx	= 0;		// ���\�����镶���C���f�b�N�X
	m_fNextTime	= 0.0f;		// ���\������܂ł̎���
	m_fCurTime	= 0.0f;		// ���݂̑ҋ@����
	m_bScroll	= false;	// ���������

	// ������2D�̏�����
	if (FAILED(CString2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CScrollString2D::Uninit()
{
	// ������2D�̏I��
	CString2D::Uninit();
}

//============================================================
//	�폜����
//============================================================
void CScrollString2D::Kill()
{
	// ���g�̏I��
	CScrollString2D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CScrollString2D::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ��������̍X�V
	UpdateScroll(fDeltaTime, fDeltaRate, fSlowRate);

	// ������2D�̍X�V
	CString2D::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//============================================================
//	�`�揈��
//============================================================
void CScrollString2D::Draw()
{
	// ������2D�̕`��
	CString2D::Draw();
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CScrollString2D::SetEnableDisp(const bool bDisp)
{
	// ���\�����镶���C���f�b�N�X��`��ݒ�ɉ����Ĕ��f
	m_nNextIdx = (bDisp) ? GetNumChar() - 1 : 0;	// ON�Ȃ�Ō���AOFF�Ȃ�擪

	// ���݂̑ҋ@���Ԃ�������
	m_fCurTime = 0.0f;

	// �`��󋵂̐ݒ�
	CString2D::SetEnableDisp(bDisp);
}

//============================================================
//	������̐ݒ菈�� (�}���`�o�C�g������)
//============================================================
HRESULT CScrollString2D::SetString(const std::string& rStr)
{
	// ���C�h������ϊ�
	std::wstring wsStr = UtilFunc::Transformation::MultiByteToWide(rStr);

	// �������ݒ�
	if (FAILED(SetString(wsStr)))
	{ // �ݒ�Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	������̐ݒ菈�� (���C�h������)
//============================================================
HRESULT CScrollString2D::SetString(const std::wstring& rStr)
{
	// ������̐ݒ�
	if (FAILED(CString2D::SetString(rStr)))
	{ // �ǉ��Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// �ݒ肵��������̎����`���OFF�ɂ���
	SetEnableDisp(false);

	return S_OK;
}

//============================================================
//	�������� (�}���`�o�C�g������)
//============================================================
CScrollString2D* CScrollString2D::Create
(
	const std::string& rFilePath,	// �t�H���g�p�X
	const bool bItalic,				// �C�^���b�N
	const std::string& rStr,		// �w�蕶����
	const MyLib::Vector3& rPos,		// ���_�ʒu
	const float fNextTime,			// �����\���̑ҋ@����
	const float fHeight,			// �����c��
	const EAlignX alignX,			// ���z�u
	const MyLib::Vector3& rRot,		// ���_����
	const D3DXCOLOR& rCol,			// �F
	const int nPriority				// �D�揇��
)
{
	// ���C�h������ϊ�
	std::wstring wsStr = UtilFunc::Transformation::MultiByteToWide(rStr);

	// �������蕶����2D�̐���
	return CScrollString2D::Create
	( // ����
		rFilePath,	// �t�H���g�p�X
		bItalic,	// �C�^���b�N
		wsStr,		// �w�蕶����
		rPos,		// ���_�ʒu
		fNextTime,	// �����\���̑ҋ@����
		fHeight,	// �����c��
		alignX,		// ���z�u
		rRot,		// ���_����
		rCol,		// �F
		nPriority	// �D�揇��
	);
}

//============================================================
//	�������� (���C�h������)
//============================================================
CScrollString2D* CScrollString2D::Create
(
	const std::string& rFilePath,	// �t�H���g�p�X
	const bool bItalic,				// �C�^���b�N
	const std::wstring& rStr,		// �w�蕶����
	const MyLib::Vector3& rPos,		// ���_�ʒu
	const float fNextTime,			// �����\���̑ҋ@����
	const float fHeight,			// �����c��
	const EAlignX alignX,			// ���z�u
	const MyLib::Vector3& rRot,		// ���_����
	const D3DXCOLOR& rCol,			// �F
	const int nPriority				// �D�揇��
)
{
	// �������蕶����2D�̐���
	CScrollString2D* pScrollString2D = DEBUG_NEW CScrollString2D(nPriority);
	if (pScrollString2D == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �������蕶����2D�̏�����
		if (FAILED(pScrollString2D->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������蕶����2D�̔j��
			SAFE_DELETE(pScrollString2D);
			return nullptr;
		}

		// �t�H���g��ݒ�
		pScrollString2D->SetFont(rFilePath, bItalic);

		// �������ݒ�
		if (FAILED(pScrollString2D->SetString(rStr)))
		{ // �ݒ�Ɏ��s�����ꍇ

			// �������蕶����2D�̔j��
			SAFE_DELETE(pScrollString2D);
			return nullptr;
		}

		// ���_�ʒu��ݒ�
		pScrollString2D->SetPosition(rPos);

		// ���_������ݒ�
		pScrollString2D->SetRotation(rRot);

		// �F��ݒ�
		pScrollString2D->SetColor(rCol);

		// �����\���̑ҋ@���Ԃ�ݒ�
		pScrollString2D->SetNextTime(fNextTime);

		// �����c����ݒ�
		pScrollString2D->SetCharHeight(fHeight);

		// ���z�u��ݒ�
		pScrollString2D->SetAlignX(alignX);

		// �m�ۂ����A�h���X��Ԃ�
		return pScrollString2D;
	}
}

//============================================================
//	��������̍X�V����
//============================================================
void CScrollString2D::UpdateScroll(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �������肪OFF�Ȃ甲����
	if (!m_bScroll) { return; }

	// �������Ȃ��ꍇ������
	if (GetNumChar() <= 0) { m_bScroll = false; return; }

	// ���݂̑ҋ@���Ԃ����Z
	m_fCurTime += fDeltaTime * fSlowRate;
	while (m_fCurTime >= m_fNextTime)
	{ // �ҋ@���I������ꍇ

		CChar2D* pChar = GetChar2D(m_nNextIdx);	// �\�������镶�����

		// �����̎����`���ON�ɂ���
		assert(pChar != nullptr);
		pChar->SetEnableDisp(true);

		// ���݂̑ҋ@���Ԃ���ҋ@���Ԃ����Z
		m_fCurTime -= m_fNextTime;

		// �������蒆�̌��ʉ����Đ�
		PlayScrollSE(pChar);

		// ���̕����C���f�b�N�X�Ɉڍs
		m_nNextIdx++;

		if (UtilFunc::Transformation::ValueNormalize(m_nNextIdx, GetNumChar(), 0))
		{ // �Ō�̕����ɓ��B�����ꍇ

			// ���݂̑ҋ@���Ԃ�������
			m_fCurTime = 0.0f;

			// ���������OFF�ɂ���
			m_bScroll = false;

			break;
		}
	}
}

//============================================================
//	����������ʉ��̍Đ�����
//============================================================
void CScrollString2D::PlayScrollSE(CChar2D* pChar2D)
{
	// ���x�����w��Ȃ��̏ꍇ������
	if (m_labelSE == CSound::LABEL_NONE) { return; }

	// �e�N�X�`���������ȏꍇ������
	if (pChar2D->IsTexEmpty()) { return; }

	// �w�胉�x����SE���Đ�
	PLAY_SOUND(m_labelSE);
}

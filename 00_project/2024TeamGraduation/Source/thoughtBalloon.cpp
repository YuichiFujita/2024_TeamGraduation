//=============================================================================
// 
//  ���������o������ [thoughtBalloon.cpp]
//  Author : ���c�E��
// 
//=============================================================================
#include "thoughtBalloon.h"
#include "renderTexture.h"
#include "balloonFrame.h"
#include "camera.h"
#include "loadtext.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	namespace text
	{
		const char*	FONT		= "data\\FONT\\Kazesawa Bold.ttf";		// �t�H���g�p�X
		const int	PRIORITY	= 6;			// �e�L�X�g�̗D�揇��
		const bool	ITALIC		= false;		// �C�^���b�N
		const float	CHAR_HEIGHT	= 36.0f;		// �����c��
		const float	LINE_HEIGHT	= 90.0f;		// �s�ԏc��
		const float	WAIT_TIME	= 0.045f;		// �����\���̑ҋ@����
		const EAlignX ALIGN_X	= XALIGN_CENTER;	// ���z�u
		const EAlignY ALIGN_Y	= YALIGN_CENTER;	// �c�z�u
	}
	const float DEFAULTSCALE = 1.25f;	// �����o���S�̂̃X�P�[��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CThoughtBalloon::CThoughtBalloon(int nPriority) : CObjectBillboard(nPriority),
	m_pRenderScene	(nullptr),	// �V�[�������_�[�e�N�X�`��
	m_pText			(nullptr),	// �e�L�X�g���
	m_pFrame		(nullptr)	// �t���[��
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CThoughtBalloon::~CThoughtBalloon()
{

}

//==========================================================================
// ��������
//==========================================================================
CThoughtBalloon* CThoughtBalloon::Create(CGameManager::ETeamSide side)
{
	// �������̊m��
	CThoughtBalloon* pObj = DEBUG_NEW CThoughtBalloon;
	if (pObj != nullptr)
	{
		// �������ݒ�
		pObj->m_TeamSide = side;	// �`�[���T�C�h

		// �N���X�̏�����
		if (FAILED(pObj->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �N���X�̏I��
			SAFE_UNINIT(pObj);
			return nullptr;
		}
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CThoughtBalloon::Init()
{
	// �I�u�W�F�N�g�r���{�[�h�̏�����
	if (FAILED(CObjectBillboard::Init()))
	{ // �������Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �����_�[�e�N�X�`���̐���
	if (FAILED(CreateRenderTexture()))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �e�N�X�`���p�I�u�W�F�N�g�̐���
	if (FAILED(CreateTextureObject()))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �e�N�X�`���C���f�b�N�X�̐ݒ�
	BindTexture(m_pRenderScene->GetTextureIndex());

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECTBILLBOARD);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CThoughtBalloon::Uninit()
{
	// �����_�[�e�N�X�`���̔j��
	SAFE_REF_RELEASE(m_pRenderScene);

	// �e�L�X�g�̏I��
	SAFE_UNINIT(m_pText);

	// �g�̏I��
	SAFE_UNINIT(m_pFrame);

	// �I�u�W�F�N�g�r���{�[�h�̏I��
	CObjectBillboard::Uninit();
}

//==========================================================================
// �폜����
//==========================================================================
void CThoughtBalloon::Kill()
{
	// �e�L�X�g�̍폜
	SAFE_KILL(m_pText);

	// �g�̍폜
	SAFE_KILL(m_pFrame);

	// ���g�̏I��
	CThoughtBalloon::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CThoughtBalloon::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

	// �g�̍X�V
	assert(m_pFrame != nullptr);
	m_pFrame->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �e�L�X�g�̍X�V
	assert(m_pText != nullptr);
	m_pText->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �I�u�W�F�N�g�r���{�[�h�̍X�V
	CObjectBillboard::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CThoughtBalloon::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X���

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �I�u�W�F�N�g�r���{�[�h�̕`��
	CObjectBillboard::Draw();

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==========================================================================
// �����_�[�e�N�X�`����������
//==========================================================================
HRESULT CThoughtBalloon::CreateRenderTexture()
{
	// �V�[�������_�[�e�N�X�`���̐���
	m_pRenderScene = CRenderTexture::Create
	( // ����
		CRenderTextureManager::LAYER_BALLOON,						// �`�揇���C���[
		std::bind(&CThoughtBalloon::CreateTexture, this),			// �e�N�X�`���쐬�֐��|�C���^
		std::bind(&CCamera::SetCamera, GET_MANAGER->GetCamera())	// �J�����ݒ�֐��|�C���^
	);
	if (m_pRenderScene == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// �e�N�X�`���p�I�u�W�F�N�g��������
//==========================================================================
HRESULT CThoughtBalloon::CreateTextureObject()
{
	// �ݒ肷�鏕�O��
	const float scale = DEFAULTSCALE;

	// �e�L�X�g�̐���
	m_pText = CScrollText2D::Create
	( // ����
		text::FONT,			// �t�H���g�p�X
		text::ITALIC,		// �C�^���b�N
		MyLib::Vector3(640.0f, 370.0f, 0.0f),			// ���_�ʒu
		text::WAIT_TIME,	// �����\���̑ҋ@����
		text::CHAR_HEIGHT * scale,	// �����c��
		text::LINE_HEIGHT * scale,	// �s�ԏc��
		text::ALIGN_X,		// ���z�u
		text::ALIGN_Y		// �c�z�u
	);
	if (m_pText == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// �����j��/�X�V/�`������Ȃ���ނɕύX
	m_pText->SetType(CObject::TYPE::TYPE_NONE);

	// ����������J�n����
	m_pText->SetEnableScroll(true);

	// ������
	m_pText->SetColor(MyLib::color::Black());

	// �e�L�X�g������
	loadtext::BindText(m_pText, loadtext::LoadText("data\\TEXT\\thought\\speech.txt", UtilFunc::Transformation::Random(0, 18)));

	// �g�̐���
	MyLib::Vector2 setSize = MyLib::Vector2(m_pText->GetTextWidth() * 2.0f, m_pText->GetTextHeight() + 50.0f);
	m_pFrame = CBalloonFrame::Create(setSize * scale, m_TeamSide);
	m_pFrame->SetType(CObject::TYPE::TYPE_NONE);

	return S_OK;
}

//==========================================================================
// �e�N�X�`���쐬����
//==========================================================================
void CThoughtBalloon::CreateTexture()
{
	// �g�̕`��
	assert(m_pFrame != nullptr);
	m_pFrame->Draw();

	// �e�L�X�g�̕`��
	assert(m_pText != nullptr);
	m_pText->Draw();
}

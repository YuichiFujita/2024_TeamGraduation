//==========================================================================
// 
//  ���[�h��ʏ��� [loadscreen.cpp]
//  Author : ���n�Ή�
//  Adder  : ���c�E��
// 
//==========================================================================
#include "loadscreen.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* TEXTURE_FILE	= "data\\TEXTURE\\loading000.tga";	// ���[�h��ʃe�N�X�`��
	const MyLib::PosGrid2 PTRN	= MyLib::PosGrid2(12, 4);			// �e�N�X�`��������
	const int	PRIORITY	= 7;		// ���[�f�B���O�̗D�揇��
	const int	ANIM_WAIT	= 2;		// �A�j���[�V�����̕ύX�t���[��
	const float	FADE_LEVEL	= 0.05f;	// �t�F�[�h�̃��l�̉�����
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CLoadScreen::CLoadScreen() : m_pLoad(nullptr)
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CLoadScreen::~CLoadScreen()
{

}

//==========================================================================
// ��������
//==========================================================================
CLoadScreen* CLoadScreen::Create()
{
	// �������̊m��
	CLoadScreen* pScreen = DEBUG_NEW CLoadScreen;
	if (pScreen != nullptr)
	{ // �������m�ۂɐ��������ꍇ

		// ����������
		if (FAILED(pScreen->Init()))
		{ // �����Ɏ��s�����ꍇ

			return nullptr;
		}
	}

	return pScreen;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CLoadScreen::Init()
{
	CTexture* pTexture = CTexture::GetInstance();	// �e�N�X�`�����

	// ���[�h��ʂ̐���
	m_pLoad = CObject2D_Anim::Create
	( // ����
		VEC3_SCREEN_CENT,	// �ʒu
		PTRN.x,	// �e�N�X�`����������
		PTRN.y,	// �e�N�X�`���c������
		0.029f,	// �ҋ@����
		false,	// �����j��
		6		// �D�揇��
	);
	if (m_pLoad == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`���̊���
	m_pLoad->BindTexture(pTexture->Regist(TEXTURE_FILE));

	// ��ގw��Ȃ��ɂ���
	m_pLoad->SetType(CObject::TYPE::TYPE_NONE);

	// �����Đ���ON�ɂ���
	m_pLoad->SetEnableAutoPlay(true);

	// �傫���̐ݒ�
	m_pLoad->SetSize(VEC2_SCREEN_SIZE * 0.5f);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CLoadScreen::Uninit()
{
	// ���[�h��ʂ̏I��
	SAFE_UNINIT(m_pLoad);
}

//==========================================================================
// �폜����
//==========================================================================
void CLoadScreen::Kill()
{
	// ���[�h��ʂ̍폜
	SAFE_KILL(m_pLoad);
}

//==========================================================================
// �X�V����
//==========================================================================
void CLoadScreen::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���[�h��ʂ̍X�V
	m_pLoad->Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CLoadScreen::Draw()
{
	// ���[�h��ʂ̕`��
	m_pLoad->Draw();
}

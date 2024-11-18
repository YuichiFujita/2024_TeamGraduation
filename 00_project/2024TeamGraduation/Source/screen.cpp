//============================================================
//
//	�X�N���[������ [screen.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "screen.h"
#include "manager.h"
#include "renderer.h"
#include "shader.h"
#include "shaderMono.h"

//************************************************************
//	�q�N���X [CScreen] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CScreen::CScreen() : CObject2D(7), m_bShader(false)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CScreen::~CScreen()
{

}

//============================================================
//	����������
//============================================================
HRESULT CScreen::Init()
{
	// �I�u�W�F�N�g2D�̏�����
	if (FAILED(CObject2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// ��ނ̎w��𖳂��ɂ���
	SetType(CObject::TYPE::TYPE_NONE);

	// �ʒu���X�N���[�������ɂ���
	SetPosition(VEC2_SCREEN_CENT);

	// �傫�����X�N���[���T�C�Y�ɂ���
	SetSize(VEC2_SCREEN_SIZE * 0.5f);

	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CScreen::Uninit()
{
	// �I�u�W�F�N�g2D�̏I��
	CObject2D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CScreen::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �I�u�W�F�N�g2D�̍X�V
	CObject2D::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//============================================================
//	�`�揈��
//============================================================
void CScreen::Draw()
{
	LPDIRECT3DDEVICE9 pDevice	= GET_DEVICE;	// �f�o�C�X���
	CMonoShader	*pMonoShader	= CMonoShader::GetInstance();	// �P�F�V�F�[�_�[���

	// �T���v���[�X�e�[�g��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);	// U�����̃��b�s���O�𖳌���
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);	// V�����̃��b�s���O�𖳌���

	if (m_bShader)
	{ // �V�F�[�_�[��ON�̏ꍇ

		if (pMonoShader->IsEffectOK())
		{ // �G�t�F�N�g���g�p�\�ȏꍇ

			// �s�N�Z���`��F��ݒ�
			pMonoShader->SetColor(MyLib::color::White());

			// �`��J�n
			pMonoShader->Begin();
			pMonoShader->BeginPass(0);

			// �I�u�W�F�N�g2D�̕`��
			CObject2D::Draw();

			// �`��I��
			pMonoShader->EndPass();
			pMonoShader->End();
		}
		else
		{ // �G�t�F�N�g���g�p�s�ȏꍇ

			// �I�u�W�F�N�g2D�̕`��
			CObject2D::Draw();
			assert(false);
		}
	}
	else
	{ // �V�F�[�_�[��OFF�̏ꍇ

		// �I�u�W�F�N�g2D�̕`��
		CObject2D::Draw();
	}

	// �T���v���[�X�e�[�g��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// U�����̃��b�s���O��L����
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// V�����̃��b�s���O��L����
}

//============================================================
//	��������
//============================================================
CScreen* CScreen::Create(const int nScreenTexIdx)
{
	// �X�N���[���̐���
	CScreen* pScreen = DEBUG_NEW CScreen;
	if (pScreen == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �X�N���[���̏�����
		if (FAILED(pScreen->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �X�N���[���̔j��
			SAFE_DELETE(pScreen);
			return nullptr;
		}

		// �e�N�X�`����o�^�E����
		pScreen->BindTexture(nScreenTexIdx);

		// �m�ۂ����A�h���X��Ԃ�
		return pScreen;
	}
}

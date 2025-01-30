//==========================================================================
// 
//  �{�[�����n�_�}�[�J�[���� [ballLandingMarker.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "ballLandingMarker.h"
#include "manager.h"
#include "renderer.h"
#include "ball.h"

// TODO�F�{�[�����n�_�}�[�J�[�\��
#if 1
#define DISP
#endif

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\ballmarker\\warning000.png";	// �{�[�����n�_�}�[�J�[�e�N�X�`��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CBallLandingMarker::CBallLandingMarker(int nPriority) : CObject3D(nPriority),
	m_pBall	(nullptr)	// �e�{�[�����
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CBallLandingMarker::~CBallLandingMarker()
{

}

//==========================================================================
// ��������
//==========================================================================
CBallLandingMarker* CBallLandingMarker::Create(CBall* pBall)
{
	// �������̊m��
	CBallLandingMarker* pMarker = DEBUG_NEW CBallLandingMarker;
	if (pMarker != nullptr)
	{
		// �v���C���[����
		pMarker->m_pBall = pBall;

		// �N���X�̏�����
		if (FAILED(pMarker->Init()))
		{ // �������Ɏ��s�����ꍇ

			SAFE_UNINIT(pMarker);
			return nullptr;
		}
	}

	return pMarker;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CBallLandingMarker::Init()
{
	// �I�u�W�F�N�g�̎�ސݒ�
	CObject::SetType(CObject::TYPE::TYPE_OBJECTBILLBOARD);

	// �e�N���X�̏�����
	HRESULT hr = CObject3D::Init();
	if (FAILED(hr)) { return E_FAIL; }

	// �e�N�X�`���̊���
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(nTexID);

	// �e�N�X�`���̑傫�����擾
	MyLib::Vector2 size = pTexture->GetImageSize(nTexID);

	// ���������ɃT�C�Y��ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 60.0f);
	SetSize(size);
	SetSizeOrigin(GetSize());

	// �����ݒ�
	SetRotation(MyLib::Vector3(D3DX_PI * 0.5f, 0.0f, 0.0f));
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CBallLandingMarker::Uninit()
{
	// �e�N���X�̏I��
	CObject3D::Uninit();
}

//==========================================================================
// �폜����
//==========================================================================
void CBallLandingMarker::Kill()
{
	// ���g�̏I��
	Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CBallLandingMarker::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
#ifdef DISP
	// �O�삩�����ւ̃p�X�̏ꍇ�͕`��ON
	bool bDisp = (m_pBall->GetState() == CBall::EState::STATE_PASS) ? true : false;
	SetEnableDisp(bDisp);

	// �`�悵�Ȃ��ꍇ�͔�����
	if (!bDisp) { return; }

	// �p�X�I���n�_�Ɉʒu��ݒ�
	MyLib::Vector3 pos = m_pBall->GetPosPassEnd();
	pos.y += 0.1f;

	// �ʒu�̔��f
	SetPosition(pos);

	// �e�N���X�̍X�V
	CObject3D::Update(fDeltaTime, fDeltaRate, fSlowRate);
#else
	SetEnableDisp(false);
#endif
}

//==========================================================================
// �`�揈��
//==========================================================================
void CBallLandingMarker::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X���

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �e�N���X�̕`��
	CObject3D::Draw();

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

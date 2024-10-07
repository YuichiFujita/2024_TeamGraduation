//=============================================================================
// 
//  �����}�[�N���� [discovery.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "discovery.h"
#include "manager.h"
#include "calculation.h"
#include "instantfade.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\start\\gogolamp.png";	// �e�N�X�`���̃t�@�C��
}

// ��ԕʃ^�C�}�[
namespace StateTime
{
	const float APPEARANCE = 0.2f;	// �o��
	const float WAIT = 1.0f;		// �ҋ@
	const float FADEOUT = 0.15f;	// �t�F�[�h�A�E�g
}

//==========================================================================
// �֐����X�g
//==========================================================================
CDiscovery::STATE_FUNC CDiscovery::m_StateFuncList[] =
{
	&CDiscovery::StateNone,			// �Ȃ�
	&CDiscovery::StateAppearance,	// �o��
	&CDiscovery::StateWait,			// �ҋ@
	&CDiscovery::StateFadeOut,		// �t�F�[�h�A�E�g
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CDiscovery::CDiscovery(int nPriority) : CObjectBillboard(nPriority)
{
	// �l�̃N���A
	m_state = State::STATE_NONE;// ���
	m_fStateTime = 0.0f;		// ��ԃJ�E���^�[
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CDiscovery::~CDiscovery()
{

}

//==========================================================================
// ��������
//==========================================================================
CDiscovery* CDiscovery::Create(const MyLib::Vector3& pos)
{
	// �������̊m��
	CDiscovery* pMarker = DEBUG_NEW CDiscovery;

	if (pMarker != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �ʒu�ݒ�
		pMarker->SetPosition(pos);

		// ����������
		pMarker->Init();
	}

	return pMarker;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CDiscovery::Init()
{
	// �r���{�[�h�̏�����
	CObjectBillboard::Init();

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECTBILLBOARD);

	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texID);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 100.0f);
	SetSizeOrigin(size);
	SetSize(D3DXVECTOR2(0.0f, 0.0f));

	// ���
	m_state = State::STATE_APPEARANCE;
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CDiscovery::Uninit()
{
	// �I������
	CObjectBillboard::Uninit();
}

//==========================================================================
// �I������
//==========================================================================
void CDiscovery::Kill()
{
	// �I������
	CObjectBillboard::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CDiscovery::Update()
{
	// ��ԍX�V
	UpdateState();

	if (CManager::GetInstance()->GetInstantFade()->GetState() != CInstantFade::STATE::STATE_NONE)
	{
		Uninit();
	}

	if (IsDeath()) return;

	// �X�V����
	CObjectBillboard::Update();
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CDiscovery::UpdateState()
{
	// ��ԃ^�C�}�[���Z
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// ��ԍX�V
	(this->*(m_StateFuncList[m_state]))();
}

//==========================================================================
// �ʏ�
//==========================================================================
void CDiscovery::StateNone()
{
	m_fStateTime = 0.0f;
}

//==========================================================================
// �o��
//==========================================================================
void CDiscovery::StateAppearance()
{
	// �T�C�Y�␳
	D3DXVECTOR2 size, sizeDest = GetSizeOrigin();
	size.x = UtilFunc::Correction::EasingEaseIn(0.0f, sizeDest.x, 0.0f, StateTime::APPEARANCE, m_fStateTime);
	size.y = UtilFunc::Correction::EasingEaseIn(0.0f, sizeDest.y, 0.0f, StateTime::APPEARANCE, m_fStateTime);

	if (m_fStateTime >= StateTime::APPEARANCE)
	{
		// �ڕW�l�ɕ␳
		size = sizeDest;

		// ��Ԑݒ�
		SetState(State::STATE_WAIT);
	}

	// �T�C�Y�ݒ�
	SetSize(size);
}

//==========================================================================
// �ҋ@
//==========================================================================
void CDiscovery::StateWait()
{
	if (m_fStateTime >= StateTime::WAIT)
	{
		SetState(State::STATE_FADEOUT);
	}
}

//==========================================================================
// �t�F�[�h�A�E�g
//==========================================================================
void CDiscovery::StateFadeOut()
{
	// �s�����x�ݒ�
	SetAlpha(1.0f - (m_fStateTime / StateTime::FADEOUT));

	if (m_fStateTime >= StateTime::FADEOUT)
	{
		// �I������
		Uninit();
	}
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CDiscovery::SetState(State state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// �`�揈��
//==========================================================================
void CDiscovery::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// �r���{�[�h�̕`��
	CObjectBillboard::Draw();

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

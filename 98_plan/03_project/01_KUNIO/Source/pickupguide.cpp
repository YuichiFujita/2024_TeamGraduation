//=============================================================================
// 
//  �s�b�N�A�b�v�K�C�h���� [pickupguide.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "pickupguide.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "calculation.h"
#include "camera.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* TEXTURE[] = // �e�N�X�`���̃t�@�C��
	{ 
		"data\\TEXTURE\\pickupguide\\transferguid_01.png",	// �]��
		"data\\TEXTURE\\pickupguide\\skilltreeguid_01.png",	// �X�L���c���[�J�n
	};
	const float TIME_FADE = 0.5f;	// �t�F�[�h�ɂ����鎞��
	const float DISTANCE_UP = 20.0f;	// �㏸����Ԋu
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CPickupGuide::STATE_FUNC CPickupGuide::m_StateFuncList[] =
{
	&CPickupGuide::StateNone,		// �Ȃɂ��Ȃ�
	&CPickupGuide::StateFadeIn,		// �t�F�[�h�C��
	&CPickupGuide::StateFadeOut,	// �t�F�[�h�A�E�g
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPickupGuide::CPickupGuide(int nPriority) : CObjectBillboard(nPriority)
{
	// �l�̃N���A
	m_state = STATE_NONE;	// ���
	m_fStateTime = 0.0f;	// ��ԃJ�E���^�[
	m_type = TYPE_TRANSFER;	// ���
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPickupGuide::~CPickupGuide()
{

}

//==========================================================================
// ��������
//==========================================================================
CPickupGuide* CPickupGuide::Create(const MyLib::Vector3& pos, TYPE type)
{
	// �������̊m��
	CPickupGuide* pMarker = DEBUG_NEW CPickupGuide;

	if (pMarker != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �ʒu�ݒ�
		pMarker->SetPosition(pos);
		pMarker->SetOriginPosition(pos);

		// ���
		pMarker->m_type = type;

		// ����������
		pMarker->Init();
	}

	return pMarker;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CPickupGuide::Init()
{
	// �r���{�[�h�̏�����
	CObjectBillboard::Init();

	// ��ނ̐ݒ�
	SetType(TYPE_OBJECTBILLBOARD);

	// �e�N�X�`���ݒ�
	int type = static_cast<int>(m_type);
	int textureIdx = CTexture::GetInstance()->Regist(TEXTURE[type]);
	BindTexture(textureIdx);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(textureIdx);
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, 15.0f);
	SetSize(size);

	// �F�ݒ�
	SetAlpha(0.0f);

	// �����l
	m_state = STATE_NONE;

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPickupGuide::Uninit()
{
	CObjectBillboard::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CPickupGuide::Update()
{
	// ��ԕʏ���
	(this->*(m_StateFuncList[m_state]))();

	// �r���{�[�h�̍X�V
	CObjectBillboard::Update();
}

//==========================================================================
// �t�F�[�h�C��
//==========================================================================
void CPickupGuide::StateFadeIn()
{
	// ��ԃJ�E���^�[���Z
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	if (m_fStateTime >= TIME_FADE)
	{
		m_fStateTime = TIME_FADE;
		m_state = STATE_NONE;
	}

	// �ʒu�ݒ�
	MyLib::Vector3 pos = GetPosition();
	pos.y = GetOriginPosition().y + (m_fStateTime / TIME_FADE) * DISTANCE_UP;
	SetPosition(pos);

	// �s�����x�ݒ�
	float alpha = UtilFunc::Correction::EasingEaseIn(0.0f, 1.0f, m_fStateTime / TIME_FADE);
	SetAlpha(alpha);
}

//==========================================================================
// �t�F�[�h�A�E�g
//==========================================================================
void CPickupGuide::StateFadeOut()
{
	// ��ԃJ�E���^�[���Z
	m_fStateTime -= CManager::GetInstance()->GetDeltaTime();

	if (m_fStateTime <= 0.0f)
	{
		m_fStateTime = 0.0f;
		m_state = STATE_NONE;
	}

	// �ʒu�ݒ�
	MyLib::Vector3 pos = GetPosition();
	pos.y = GetOriginPosition().y + (m_fStateTime / TIME_FADE) * DISTANCE_UP;
	SetPosition(pos);

	// �s�����x�ݒ�
	float alpha = UtilFunc::Correction::EasingEaseIn(0.0f, 1.0f, m_fStateTime / TIME_FADE);
	SetAlpha(alpha);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CPickupGuide::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//��ɕ`�悷��

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

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CPickupGuide::SetState(STATE state)
{
	m_state = state;
}

//==========================================================================
// 
//  �J�b�g�C������ [cutin.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "cutin.h"
#include "manager.h"
#include "camera.h"
#include "calculation.h"
#include "renderTexture.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE_BLINK_BG = "data\\TEXTURE\\cutin\\blink000.jpg";	// �w�i�e�N�X�`��
	const int	PRIORITY = 7;		// �D�揇��
	const float	END_TIME = 1.2f;	// �I������

	namespace bg
	{
		const float START_ALPHA = 0.66f;	// �J�n�����x
		const float END_ALPHA = 0.0f;		// �I�������x
	}

	namespace blink
	{
		const MyLib::PosGrid3 START = MyLib::PosGrid3(63, 12, 100);	// �J�n�F
		const MyLib::PosGrid3 END = MyLib::PosGrid3(0, 100, 100);	// �I���F
		const float START_ALPHA = 0.7f;	// �J�n�����x
		const float END_ALPHA = 0.1f;	// �I�������x
	}
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCutIn::CCutIn() : CObject2D(PRIORITY),
	m_pRenderScene	(nullptr),		// �V�[�������_�[�e�N�X�`��
	m_pBlink		(nullptr),		// �P�����
	m_pPlayer2D		(nullptr),		// �v���C���[�e�N�X�`�����
	m_fBlinkTime	(0.0f)			// �w�i�P���J�E���^�[
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCutIn::~CCutIn()
{

}

//==========================================================================
// ��������
//==========================================================================
CCutIn* CCutIn::Create()
{
	// �������̊m��
	CCutIn* pObj = DEBUG_NEW CCutIn;
	if (pObj != nullptr)
	{
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
HRESULT CCutIn::Init()
{
	// �I�u�W�F�N�g2D�̏�����
	if (FAILED(CObject2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �ʒu/�傫����ݒ�
	SetPosition(VEC3_SCREEN_CENT);	// ��ʒ��S�ɂ���
	SetSize(VEC2_SCREEN_SIZE);		// ��ʑS�̂ɂ���

	// �������ɂ���
	SetAlpha(bg::START_ALPHA);

	// ���E��~���ɓ�����悤�ɂ���
	SetEnablePosibleMove_WorldPause(true);

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

	// �v���C���[�e�N�X�`���\�t�|���S��/�P���̐���
	if (FAILED(CreateObject2D()))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CCutIn::Uninit()
{
	// �P���̏I��
	SAFE_UNINIT(m_pBlink);

	// �I������
	CObject2D::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CCutIn::Kill()
{
	// �P���̍폜
	SAFE_KILL(m_pBlink);

	// �폜����
	CObject2D::Kill();
}

//==========================================================================
// �X�V����
//==========================================================================
void CCutIn::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �w�i�F�̍X�V
	UpdateBlinkColor(fDeltaTime, fDeltaRate, fSlowRate);

#if 0
	// TODO
	if (IsEnd())
	{
		Uninit();
		return;
	}
#endif

	// �P���̍X�V
	m_pBlink->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �X�V����
	CObject2D::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CCutIn::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X���

	// �`�揈��
	CObject2D::Draw();

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �P���̕`��
	m_pBlink->Draw();

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==========================================================================
// �I���t���O�擾����
//==========================================================================
bool CCutIn::IsEnd() const
{
	// �o�ߎ��Ԃ��I�����Ԃ𒴂��Ă��邩��Ԃ�
	return (m_fBlinkTime >= END_TIME);
}

//==========================================================================
// �����_�[�e�N�X�`����������
//==========================================================================
HRESULT CCutIn::CreateRenderTexture()
{
	// �V�[�������_�[�e�N�X�`���̐���
	m_pRenderScene = CRenderTexture::Create
	( // ����
		CRenderTextureManager::LAYER_BALLOON,		// �`�揇���C���[
		std::bind(&CCutIn::CreateTexture, this),	// �e�N�X�`���쐬�֐��|�C���^
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
HRESULT CCutIn::CreateTextureObject()
{
	return S_OK;
}

//==========================================================================
// �v���C���[�e�N�X�`���\�t�|���S��/�P����������
//==========================================================================
HRESULT CCutIn::CreateObject2D()
{
	// �P���̐���
	m_pBlink = CObject2D::Create(PRIORITY);
	if (m_pBlink == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
	}

	// �����j��/�X�V/�`������Ȃ���ނɂ���
	m_pBlink->SetType(CObject::TYPE::TYPE_NONE);

	// �ʒu/�傫����ݒ�
	m_pBlink->SetPosition(VEC3_SCREEN_CENT);	// ��ʒ��S�ɂ���
	m_pBlink->SetSize(VEC2_SCREEN_SIZE);		// ��ʑS�̂ɂ���

	// �������ɂ���
	m_pBlink->SetAlpha(blink::START_ALPHA);

	// �e�N�X�`���C���f�b�N�X��ݒ�
	m_pBlink->BindTexture(CTexture::GetInstance()->Regist(TEXTURE_BLINK_BG));

	// �v���C���[�e�N�X�`���\��t���p�|���S���̐���
	m_pPlayer2D = CObject2D::Create(PRIORITY);
	if (m_pPlayer2D == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
	}

	// �ʒu/�傫����ݒ�
	m_pPlayer2D->SetPosition(VEC3_SCREEN_CENT);	// ��ʒ��S�ɂ���
	m_pPlayer2D->SetSize(VEC2_SCREEN_SIZE);		// ��ʑS�̂ɂ���

	// �v���C���[�̃����_�[�e�N�X�`���C���f�b�N�X��ݒ�
	assert(m_pRenderScene != nullptr);	// �惌���_�[�e�N�X�`����������
	m_pPlayer2D->BindTexture(m_pRenderScene->GetTextureIndex());

	return S_OK;
}

//==========================================================================
// �e�N�X�`���쐬����
//==========================================================================
void CCutIn::CreateTexture()
{

}

//==========================================================================
// �w�i�F�̍X�V����
//==========================================================================
void CCutIn::UpdateBlinkColor(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	MyLib::Vector3 start = blink::START;	// �J�n���̔w�iHSV�F
	MyLib::Vector3 end = blink::END;		// �I�����̔w�iHSV�F
	MyLib::Vector3 current = VEC3_ZERO;		// ���ݎ��̔w�iHSV�F
	float fAlphaBG = 0.0f;					// ���ݎ��̔w�i�����x
	float fAlphaBlink = 0.0f;				// ���ݎ��̋P�������x

	// �o�ߎ��Ԃ����Z
	m_fBlinkTime += fDeltaTime * fSlowRate;

	// ���݂̔w�iHSV�F����`�⊮
	current = UtilFunc::Correction::EasingLinear(start, end, 0.0f, END_TIME, m_fBlinkTime);

	// ���݂̔w�i�����x����`�⊮
	fAlphaBG = UtilFunc::Correction::EaseInExpo(bg::START_ALPHA, bg::END_ALPHA, 0.0f, END_TIME, m_fBlinkTime);

	// ���݂̓����x����`�⊮
	fAlphaBlink = UtilFunc::Correction::EaseInExpo(blink::START_ALPHA, blink::END_ALPHA, 0.0f, END_TIME, m_fBlinkTime);

	// �w�i�̐F����HSV����RGB�ɕϊ������f
	D3DXCOLOR colRGB = UtilFunc::Transformation::HSVtoRGB(current.x, current.y, current.z);	// RGB�F���
	SetColor(D3DXCOLOR(colRGB.r, colRGB.g, colRGB.b, fAlphaBG));	// �F�𔽉f

	// �P���̓����x�𔽉f
	m_pBlink->SetAlpha(fAlphaBlink);
}

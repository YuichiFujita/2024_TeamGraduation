//=============================================================================
// 
// �X�e�[�W������ [stagename.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "stagename.h"
#include "manager.h"
#include "sound.h"
#include "calculation.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\stagename\\enhance.png";
	const float SPEED_VERTICAL = 0.3f;		// �㏸���x
	const float LIFE = 2.0f;				// ����
	const float TIME_START_FADEOUT = 1.3f;	// �������Ă���t�F�[�h�A�E�g�����܂ł̎���
}
CStageName* CStageName::m_pThisPtr = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CStageName::CStageName(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_fAlphaTime = 0.0f;			// �s�����x�^�C�}�[
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CStageName::~CStageName()
{

}

//==========================================================================
// ��������
//==========================================================================
CStageName* CStageName::Create()
{
	if (m_pThisPtr != nullptr)
	{
		return m_pThisPtr;
	}

	// �������̊m��
	m_pThisPtr = DEBUG_NEW CStageName;

	if (m_pThisPtr != nullptr)
	{
		// ����������
		if (FAILED(m_pThisPtr->Init()))
		{
			return nullptr;
		}
	}

	return m_pThisPtr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CStageName::Init()
{
	// ����������
	HRESULT hr = CObject2D::Init();
	if (FAILED(hr))
	{// ���s�����Ƃ�
		return E_FAIL;
	}

	// ��ނ̐ݒ�
	SetType(TYPE_OBJECT2D);

	// �e�N�X�`���̊��蓖��
	int nIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(nIdx);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(nIdx);
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, 50.0f);
	SetSize(size);
	SetSizeOrigin(GetSize());

	// �ʒu�ݒ�
	SetPosition(MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));


	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CStageName::Uninit()
{
	m_pThisPtr = nullptr;

	// �I������
	CObject2D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CStageName::Update()
{
	// �s�����x���ԉ��Z
	m_fAlphaTime += CManager::GetInstance()->GetDeltaTime();

	if (m_fAlphaTime >= TIME_START_FADEOUT)
	{
		// �ʒu�ړ�
		AddPosition(MyLib::Vector3(0.0f, -SPEED_VERTICAL, 0.0f));

		// �s�����x�X�V
		float alpha = UtilFunc::Correction::EasingLinear(1.0f, 0.0f, TIME_START_FADEOUT, LIFE, m_fAlphaTime);
		SetAlpha(alpha);
	}

	// �X�V����
	CObject2D::Update();

	if (m_fAlphaTime >= LIFE)
	{
		Uninit();
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CStageName::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// �`�揈��
	CObject2D::Draw();

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

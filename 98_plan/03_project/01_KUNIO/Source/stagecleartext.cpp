//=============================================================================
// 
// �X�e�[�W�N���A�e�L�X�g���� [stagecleartext.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "stagecleartext.h"
#include "manager.h"
#include "sound.h"
#include "calculation.h"
#include "camera.h"
#include "game.h"
#include "player.h"

//==========================================================================
// �}�N����`
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\battlewin\\goal.png";
	const float TIME_EXPANSION = 0.6f;			// �g��
	const float TIME_EXPNONE = 1.0f;			// �g��㉽�����Ȃ�
	const float TIME_FADEOUT = 0.3f;			// �t�F�[�h�A�E�g����
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CStageClearText::STATE_FUNC CStageClearText::m_StateFuncList[] =
{
	&CStageClearText::StateExpansion,
	&CStageClearText::StateExpNone,
	&CStageClearText::StateFadeOut,
};

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CStageClearText::CStageClearText(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_state = STATE_EXPANSION;			// ���
	m_fStateTimer = 0.0f;			// ��ԃ^�C�}�[
	m_bCreateResultWindow = false;	// ���U���g��ʂ̌Ăяo���t���O
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CStageClearText::~CStageClearText()
{

}

//==========================================================================
// ��������
//==========================================================================
CStageClearText* CStageClearText::Create(const MyLib::Vector3 pos)
{
	// �������̊m��
	CStageClearText* pEffect = DEBUG_NEW CStageClearText;

	if (pEffect != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		pEffect->Init();
		pEffect->SetPosition(pos);
	}

	return pEffect;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CStageClearText::Init()
{
	HRESULT hr;

	// ����������
	hr = CObject2D::Init();
	if (FAILED(hr))
	{// ���s�����Ƃ�
		return E_FAIL;
	}

	// ��ނ̐ݒ�
	SetType(TYPE_OBJECT2D);

	// �e�N�X�`���̊��蓖��
	int nTexIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(nTexIdx);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(nTexIdx);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 500.0f);
	SetSize(size);
	SetSizeOrigin(size);

	// �J�n
	m_fStateTimer = 0.0f;
	m_state = eState::STATE_EXPANSION;

	// �����ݒ�
	SetRotation(MyLib::Vector3(0.0f, 0.0f, 0.0f));
	SetOriginRotation(GetRotation());

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CStageClearText::Uninit()
{
	// �I������
	CObject2D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CStageClearText::Update()
{
	// ��ԍX�V
	UpdateState();

	if (IsDeath())return;;

	// ���_���W�̐ݒ�
	SetVtx();
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CStageClearText::UpdateState()
{
	// ��ԑJ�ڃJ�E���^�[���Z
	m_fStateTimer += CManager::GetInstance()->GetDeltaTime();

	// ��ԕʏ���
	(this->*(m_StateFuncList[m_state]))();
}

//==========================================================================
// �g��
//==========================================================================
void CStageClearText::StateExpansion()
{
	
	float alpha = UtilFunc::Correction::EaseInBack(0.2f, 1.0f, 0.0f, TIME_EXPANSION, m_fStateTimer);
	SetAlpha(alpha);
	
	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = GetSize();
	size.x = UtilFunc::Correction::EaseInBack(GetSizeOrigin().x * 0.2f, GetSizeOrigin().x, 0.0f, TIME_EXPANSION, m_fStateTimer);
	size.y = UtilFunc::Correction::EaseInBack(GetSizeOrigin().y * 0.2f, GetSizeOrigin().y, 0.0f, TIME_EXPANSION, m_fStateTimer);
	SetSize(size);

	if (m_fStateTimer >= TIME_EXPANSION)
	{
		m_fStateTimer = 0.0f;
		m_state = STATE_EXPNONE;
	}
}

//==========================================================================
// �������
//==========================================================================
void CStageClearText::StateExpNone()
{
	
	if (m_fStateTimer >= TIME_EXPNONE)
	{
		m_fStateTimer = 0.0f;
		m_state = eState::STATE_FADEOUT;
		return;
	}
}

//==========================================================================
// �t�F�[�h�A�E�g���
//==========================================================================
void CStageClearText::StateFadeOut()
{
	
	// �s�����x�X�V
	float alpha = UtilFunc::Correction::EasingLinear(1.0f, 0.0f, 0.0f, TIME_FADEOUT, m_fStateTimer);
	SetAlpha(alpha);

	if (TIME_FADEOUT * 0.7f <= m_fStateTimer &&
		!m_bCreateResultWindow)
	{
		m_bCreateResultWindow = true;

		// ��ʐ���
		CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_GOAL);
	}

	if (m_fStateTimer >= TIME_FADEOUT)
	{
		m_fStateTimer = 0.0f;
		Uninit();
		return;
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CStageClearText::Draw()
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

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CStageClearText::SetVtx()
{
	// ���_�ݒ�
	CObject2D::SetVtx();
}


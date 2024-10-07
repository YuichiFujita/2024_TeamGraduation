//=============================================================================
// 
// �L���b�`���ʏ��� [catchresult.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "catchresult.h"
#include "manager.h"
#include "sound.h"
#include "calculation.h"
#include "camera.h"
#include "game.h"
#include "player.h"

// �J�ڐ�
#include "catchresult_OK.h"
#include "catchresult_FAIL.h"

//==========================================================================
// �}�N����`
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\battlewin\\goal.png";
	const float TIME_EXPANSION = 0.3f;			//�g��
	const float TIME_EXPNONE = 1.0f;			//�g��㉽�����Ȃ�
	const float TIME_FADEOUT = 0.4f;			// �t�F�[�h�A�E�g����
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CCatchResult::STATE_FUNC CCatchResult::m_StateFuncList[] =
{
	&CCatchResult::StateExpansion,
	&CCatchResult::StateExpNone,
	&CCatchResult::StateFadeOut,
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCatchResult::CCatchResult(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_state = STATE_EXPANSION;	// ���
	m_fStateTimer = 0.0f;		// ��ԃ^�C�}�[
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCatchResult::~CCatchResult()
{

}

//==========================================================================
// ��������
//==========================================================================
CCatchResult* CCatchResult::Create(const MyLib::Vector3& pos, TYPE resultType)
{
	// �������̊m��
	CCatchResult* pObj = nullptr;

	switch (resultType)
	{
	case CCatchResult::TYPE_OK:
		pObj = DEBUG_NEW CCatchResult_OK;
		break;

	case CCatchResult::TYPE_FAIL:
		pObj = DEBUG_NEW CCatchResult_FAIL;
		break;

	default:
		break;
	}

	if (pObj != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		pObj->Init();
		pObj->SetPosition(pos);
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CCatchResult::Init()
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
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 400.0f);
	SetSize(size);
	SetSizeOrigin(size);

	// �J�n
	m_fStateTimer = 0.0f;
	m_state = State::STATE_EXPANSION;

	// �����ݒ�
	SetRotation(MyLib::Vector3(0.0f, 0.0f, 0.0f));
	SetOriginRotation(GetRotation());

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CCatchResult::Uninit()
{
	// �I������
	CObject2D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CCatchResult::Update()
{
	if (IsDeath())
	{
		return;
	}

	// ��ԕʏ���
	(this->*(m_StateFuncList[m_state]))();

	if (IsDeath())
	{
		return;
	}

	// ���_���W�̐ݒ�
	SetVtx();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CCatchResult::Draw()
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

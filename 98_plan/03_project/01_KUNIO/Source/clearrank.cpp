//=============================================================================
// 
//  �N���A�����N����(��ԏ�) [clearrank.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "clearrank.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE[] =	// �e�N�X�`���̃t�@�C��
	{
		"data\\TEXTURE\\result\\rank_S.png",
		"data\\TEXTURE\\result\\rank_A.png",
		"data\\TEXTURE\\result\\rank_B.png",
		"data\\TEXTURE\\result\\rank_C.png",
		"data\\TEXTURE\\result\\rank_C.png",
	};
	const std::string TEXT_TEXTURE = "data\\TEXTURE\\result\\clearrank.png";
	const float SIZE_HEIGHT = 50.0f;	// �c���̃T�C�Y
	const float MOVEVALUE_TEXT = 3.0f;	// �e�L�X�g�̈ړ���
	const float MOVEVALUE_RANK = 6.0f;	// �����N�̈ړ���
}

namespace StateTime
{
	const float WAIT = 0.5f;		// �ҋ@
	const float EMPHASIZE = 1.0f;	// ����
	const int SCROLLEND = 1.0f;	// �X�N���[���I����
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CClearRank::STATE_FUNC CClearRank::m_StateFunc[] =
{
	&CClearRank::StateScrollText,	// ��������
	&CClearRank::StateSrollVoid,	// ��ԑ���
	&CClearRank::StateScrollRank,	// �����N����
	&CClearRank::StateFinish,		// �I��
	&CClearRank::StateEmphasize,	// ����
	&CClearRank::StateNone,			// �Ȃɂ��Ȃ�

};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CClearRank::CClearRank(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_Rank = CJudge::JUDGE::JUDGE_DDD;	// �����N
	m_pText = nullptr;		// ����


	m_fStateTime = 0.0f;		// ��ԃJ�E���^�[
	m_state = State::STATE_SCROLL_TEXT;			// ���
	m_fMoveTextLen = 0.0f;	// �e�L�X�g�̈ړ�����
	m_fMoveRankLen = 0.0f;	// �����N�̈ړ�����
	m_bFinish = false;		// �I��
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CClearRank::~CClearRank()
{

}

//==========================================================================
// ��������
//==========================================================================
CClearRank* CClearRank::Create(CJudge::JUDGE rank)
{
	// �������̊m��
	CClearRank* pClearRank = DEBUG_NEW CClearRank;

	if (pClearRank != nullptr)
	{
		pClearRank->m_Rank = rank;

		// ����������
		pClearRank->Init();
	}

	return pClearRank;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CClearRank::Init()
{
	//=============================
	// ��������
	//=============================
	CreateText();

	//=============================
	// �����N����
	//=============================
	CreateRank();


	// ��ԑJ��
	SetState(State::STATE_NONE);

	return S_OK;
}

//==========================================================================
// ��������
//==========================================================================
void CClearRank::CreateText()
{
	// ����
	m_pText = CObject2D::Create(GetPriority());
	m_pText->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TEXT_TEXTURE);
	m_pText->BindTexture(texID);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

	// �c�������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZE_HEIGHT);
	m_pText->SetSize(D3DXVECTOR2(0.0f, size.y));
	m_pText->SetSizeOrigin(size);

	// �ʒu�ݒ�
	m_pText->SetPosition(MyLib::Vector3(200.0f, 200.0f, 0.0f));

	// �A���J�[�|�C���g�̐ݒ�
	m_pText->SetAnchorType(AnchorPoint::LEFT);
}

//==========================================================================
// �����N����
//==========================================================================
void CClearRank::CreateRank()
{
	// �I�u�W�F�N�g2D�̏�����
	CObject2D::Init();

	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TEXTURE[m_Rank]);
	BindTexture(texID);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

	// �c�������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZE_HEIGHT);

	SetSize(D3DXVECTOR2(0.0f, size.y));
	SetSizeOrigin(size);
	SetPosition(m_pText->GetPosition() + MyLib::Vector3(m_pText->GetSizeOrigin().x * 2.0f, 0.0f, 0.0f));

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_ENEMY);

	// �A���J�[�|�C���g�̐ݒ�
	SetAnchorType(AnchorPoint::LEFT);
}


//==========================================================================
// �I������
//==========================================================================
void CClearRank::Uninit()
{
	// �I������
	CObject2D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CClearRank::Update()
{
	// ��ԍX�V
	UpdateState();

	// �X�V����
	CObject2D::Update();
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CClearRank::SetVtx()
{
	CObject2D::SetVtx();
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CClearRank::UpdateState()
{
	// ��ԃ^�C�}�[
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	(this->*(m_StateFunc[m_state]))();

}

//==========================================================================
// �X�L�b�v
//==========================================================================
void CClearRank::Skip()
{
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pKey->GetTrigger(DIK_RETURN) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_A, 0))
	{
		// ���[�h�ݒ�
		SetState(CClearRank::State::STATE_FINISH);
	}
}

//==========================================================================
// ��������
//==========================================================================
void CClearRank::StateScrollText()
{
	// �T�C�Y�擾
	D3DXVECTOR2 size = m_pText->GetSize(), sizeOrigin = m_pText->GetSizeOrigin();

	// �e�L�X�g�ړ��������Z
	m_fMoveTextLen += MOVEVALUE_TEXT;
	m_fMoveTextLen = UtilFunc::Transformation::Clamp(m_fMoveTextLen, 0.0f, sizeOrigin.x);

	if (m_fMoveTextLen >= sizeOrigin.x)
	{
		// ��ԑJ��
		SetState(State::STATE_SCROLL_VOID);
	}

	// �T�C�Y�ݒ�
	size.x = m_fMoveTextLen;
	m_pText->SetSize(size);

	// �e�N�X�`�����W�ݒ�
	D3DXVECTOR2* pTex = m_pText->GetTex();
	pTex[1].x = pTex[3].x = (size.x / sizeOrigin.x);

	// �X�L�b�v
	Skip();
}

//==========================================================================
// ��ԑ���
//==========================================================================
void CClearRank::StateSrollVoid()
{

	if (m_fStateTime >= StateTime::WAIT)
	{
		// ��ԑJ��
		SetState(State::STATE_SCROLL_RANK);

		// �T�E���h�Đ�
		CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_WRITING_FINISH);
	}

	// �X�L�b�v
	Skip();
}

//==========================================================================
// �����N����
//==========================================================================
void CClearRank::StateScrollRank()
{
	// �T�C�Y�擾
	D3DXVECTOR2 size = GetSize(), sizeOrigin = GetSizeOrigin();

	// �e�L�X�g�ړ��������Z
	m_fMoveRankLen += MOVEVALUE_RANK;

	if (m_fMoveRankLen >= sizeOrigin.x/* + MOVEVALUE_RANK * (StateTime::SCROLLEND * 60.0f)*/)
	{
		// ��ԑJ��
		SetState(State::STATE_FINISH);
	}

	// �T�C�Y�ݒ�
	size.x = UtilFunc::Transformation::Clamp(m_fMoveRankLen, 0.0f, sizeOrigin.x);
	SetSize(size);

	// �e�N�X�`�����W�ݒ�
	D3DXVECTOR2* pTex = GetTex();
	pTex[1].x = pTex[3].x = (size.x / sizeOrigin.x);

	// �X�L�b�v
	Skip();
}

//==========================================================================
// �I��
//==========================================================================
void CClearRank::StateFinish()
{
	// �I���t���O
	m_bFinish = true;

	// �T�C�Y�ݒ�
	SetSize(GetSizeOrigin());
	m_pText->SetSize(m_pText->GetSizeOrigin());

	// �e�N�X�`�����W�ݒ�
	D3DXVECTOR2* pTex = GetTex();
	D3DXVECTOR2* pTexText = m_pText->GetTex();
	pTex[1].x = pTex[3].x = pTexText[1].x = pTexText[3].x = 1.0f;

	// ��ԑJ��
	SetState(State::STATE_EMPHASIZE);
}

//==========================================================================
// ����
//==========================================================================
void CClearRank::StateEmphasize()
{
	// �T�C�Y�擾
	D3DXVECTOR2 size = GetSize(), sizeOrigin = GetSizeOrigin();
	float ratio = m_fStateTime / StateTime::EMPHASIZE;
	size.x = sizeOrigin.x + sinf(D3DX_PI * ratio) * (sizeOrigin.x * 0.5f);
	size.y = sizeOrigin.y + sinf(D3DX_PI * ratio) * (sizeOrigin.y * 0.5f);
	SetSize(size);

#if 0	// �C�[�W���O�␳
	if (m_fStateTime <= StateTime::EMPHASIZE * 0.5f)
	{
		size.x = UtilFunc::Correction::EaseOutExpo(sizeOrigin.x, sizeOrigin.x * 1.5f, 0.0f, StateTime::EMPHASIZE * 0.5f, m_fStateTime);
		size.y = UtilFunc::Correction::EaseOutExpo(sizeOrigin.y, sizeOrigin.y * 1.5f, 0.0f, StateTime::EMPHASIZE * 0.5f, m_fStateTime);
	}
	else
	{
		size.x = UtilFunc::Correction::EaseInExpo(sizeOrigin.x * 1.5f, sizeOrigin.x, StateTime::EMPHASIZE * 0.5f, StateTime::EMPHASIZE, m_fStateTime);
		size.y = UtilFunc::Correction::EaseInExpo(sizeOrigin.y * 1.5f, sizeOrigin.y, StateTime::EMPHASIZE * 0.5f, StateTime::EMPHASIZE, m_fStateTime);
	}
	SetSize(size);
#endif

	if (m_fStateTime >= StateTime::EMPHASIZE)
	{
		// ��ԑJ��
		SetState(State::STATE_NONE);
	}
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CClearRank::SetState(State state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// �`�揈��
//==========================================================================
void CClearRank::Draw()
{
	// �`�揈��
	CObject2D::Draw();
}

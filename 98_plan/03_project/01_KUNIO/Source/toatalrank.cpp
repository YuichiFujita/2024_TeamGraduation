//=============================================================================
// 
//  �g�[�^�������N���� [toatalrank.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "toatalrank.h"
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
	};
	const std::string TEXT_TEXTURE = "data\\TEXTURE\\result\\toatalrank.png";

	const int RANKPOINT[] =	// �����N�̊
	{
		4,
		3,
		2,
		1
	};

	const float TIMEPOINT[] =	// �^�C�}�[�̊
	{
		80.0f,
		100.0f,
		120.0f,
		140.0f
	};

	const float SIZE_HEIGHT = 50.0f;	// �c���̃T�C�Y
	const float MOVEVALUE_TEXT = 3.0f;	//�e�L�X�g�̈ړ���
}

namespace StateTime
{
	const float WAIT = 0.5f;	// �ҋ@
	const float EMPHASIZE = 1.0f;	// ����
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CToatalRank::STATE_FUNC CToatalRank::m_StateFunc[] =
{
	&CToatalRank::StateScrollText,	// ��������
	&CToatalRank::StateSrollVoid,	// ��ԑ���
	&CToatalRank::StateScrollRank,	// �����N����
	&CToatalRank::StateFinish,		// �I��
	&CToatalRank::StateNone,		// �Ȃɂ��Ȃ�

};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CToatalRank::CToatalRank(int nPriority) : CObject2D(nPriority)
{
	// �l�̃g�[�^��
	m_Rank = CJudge::JUDGE::JUDGE_DDD;	// �����N

	m_fStateTime = 0.0f;		// ��ԃJ�E���^�[
	m_state = State::STATE_SCROLL_TEXT;			// ���
	m_fMoveTextLen = 0.0f;	// �e�L�X�g�̈ړ�����
	m_fMoveRankLen = 0.0f;	// �����N�̈ړ�����
	m_bFinish = false;		// �I��
	m_bSoundFinish = false;	// �T�E���h�I��
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CToatalRank::~CToatalRank()
{

}

//==========================================================================
// ��������
//==========================================================================
CToatalRank* CToatalRank::Create(CJudge::JUDGE rank, float time)
{
	// �������̊m��
	CToatalRank* pClearRank = DEBUG_NEW CToatalRank;

	if (pClearRank != nullptr)
	{
		pClearRank->m_Rank = pClearRank->CalRank(rank, time);

		// ����������
		pClearRank->Init();
	}

	return pClearRank;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CToatalRank::Init()
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
void CToatalRank::CreateText()
{
	// ����
	m_pText = CObject2D::Create(GetPriority());
	m_pText->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TEXT_TEXTURE);
	m_pText->BindTexture(texID);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZE_HEIGHT);
	m_pText->SetSize(D3DXVECTOR2(0.0f, size.y));
	m_pText->SetSizeOrigin(size);

	// �ʒu�ݒ�
	m_pText->SetPosition(MyLib::Vector3(700.0f, 400.0f, 0.0f));

	// �A���J�[�|�C���g�̐ݒ�
	m_pText->SetAnchorType(CObject2D::AnchorPoint::LEFT);
}

//==========================================================================
// �����N����
//==========================================================================
void CToatalRank::CreateRank()
{
	// �I�u�W�F�N�g2D�̏�����
	CObject2D::Init();

	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TEXTURE[m_Rank]);
	BindTexture(texID);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

	// �c�������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 120.0f);
	SetSize(D3DXVECTOR2(0.0f, size.y));
	SetSizeOrigin(size);

	// �ʒu�ݒ�
	SetPosition(m_pText->GetPosition() + MyLib::Vector3(m_pText->GetSize().x + 120.0f, 100.0f, 0.0f));

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �A���J�[�|�C���g�ݒ�
	SetAnchorType(CObject2D::AnchorPoint::LEFT);
}

//==========================================================================
// �I������
//==========================================================================
void CToatalRank::Uninit()
{
	// �I������
	CObject2D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CToatalRank::Update()
{
	// ��ԍX�V
	UpdateState();

	// �X�V����
	CObject2D::Update();
}


//==========================================================================
// ��ԍX�V
//==========================================================================
void CToatalRank::UpdateState()
{
	// ��ԃ^�C�}�[
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	(this->*(m_StateFunc[m_state]))();

}

//==========================================================================
// �X�L�b�v
//==========================================================================
void CToatalRank::Skip()
{
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pKey->GetTrigger(DIK_RETURN) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_A, 0))
	{
		// �e�L�X�g�ړ��������Z
		m_pText->SetSize(m_pText->GetSizeOrigin());
		D3DXVECTOR2* pTex = m_pText->GetTex();
		pTex[1].x = pTex[3].x = 1.0f;

		SetState(CToatalRank::State::STATE_SCROLL_RANK);
	}
}

//==========================================================================
// ��������
//==========================================================================
void CToatalRank::StateScrollText()
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
void CToatalRank::StateSrollVoid()
{

	if (m_fStateTime >= StateTime::WAIT)
	{
		// ��ԑJ��
		SetState(State::STATE_SCROLL_RANK);
	}

	// �X�L�b�v
	Skip();
}

//==========================================================================
// �����N����
//==========================================================================
void CToatalRank::StateScrollRank()
{
	// �T�C�Y�擾
	D3DXVECTOR2 size = GetSize(), sizeOrigin = GetSizeOrigin();

	// �e�L�X�g�ړ��������Z
	/*m_fMoveRankLen += MOVEVALUE_TEXT;
	m_fMoveRankLen = UtilFunc::Transformation::Clamp(m_fMoveRankLen, 0.0f, sizeOrigin.x);*/
	m_fMoveRankLen = UtilFunc::Correction::EaseInExpo(0.0f, sizeOrigin.x, 0.0f, 0.9f, m_fStateTime);

	if (m_fStateTime >= 0.4f &&
		!m_bSoundFinish)
	{
		// �T�E���h�Đ��I��
		m_bSoundFinish = true;

		// �T�E���h�Đ�
		CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_WRITING_FINISH);
	}

	if (m_fMoveRankLen >= sizeOrigin.x)
	{
		// ��ԑJ��
		SetState(State::STATE_FINISH);
	}

	// �T�C�Y�ݒ�
	size.x = m_fMoveRankLen;
	SetSize(size);

	// �e�N�X�`�����W�ݒ�
	D3DXVECTOR2* pTex = GetTex();
	pTex[1].x = pTex[3].x = (size.x / sizeOrigin.x);
}

//==========================================================================
// �I��
//==========================================================================
void CToatalRank::StateFinish()
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
	SetState(State::STATE_NONE);
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CToatalRank::SetState(State state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// �`�揈��
//==========================================================================
void CToatalRank::Draw()
{
	// �`�揈��
	CObject2D::Draw();
}

//==========================================================================
// �����N�v�Z
//==========================================================================
CJudge::JUDGE CToatalRank::CalRank(CJudge::JUDGE rank, float time)
{
	int point = 0, i = 0;

	// ���Ԃ̃|�C���g�v�Z
	while (1)
	{
		if (TIMEPOINT[i] >= time)
		{
			point += CJudge::JUDGE::JUDGE_DDD - (i);
			break;
		}
	}
	
	// �����N�̃|�C���g�v�Z
	point += CJudge::JUDGE::JUDGE_DDD - rank;

	if (point >= CJudge::JUDGE::JUDGE_DDD * 2)
	{
		return CJudge::JUDGE::JUDGE_AAA;
	}
	else if (point >= CJudge::JUDGE::JUDGE_CCC * 2)
	{
		return CJudge::JUDGE::JUDGE_BBB;
	}
	else if (point >= CJudge::JUDGE::JUDGE_BBB * 2)
	{
		return CJudge::JUDGE::JUDGE_CCC;
	}
	else
	{
		return CJudge::JUDGE::JUDGE_DDD;
	}
}
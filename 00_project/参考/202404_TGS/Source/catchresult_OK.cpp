//=============================================================================
// 
// �L���b�`���ʏ��� [catchresult_OK.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "catchresult_OK.h"
#include "manager.h"
#include "sound.h"
#include "calculation.h"
#include "camera.h"
#include "game.h"
#include "player.h"
#include "fade.h"
#include "judge.h"
#include "timer.h"
#include "resultmanager.h"
#include "gamemanager.h"

//==========================================================================
// �}�N����`
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\result\\OK.png";
	const float TIME_EXPANSION = 0.3f;			// �g��
	const float TIME_EXPNONE = 1.0f;			// �g��㉽�����Ȃ�
	const float TIME_FADEOUT = 0.4f;			// �t�F�[�h�A�E�g����
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCatchResult_OK::CCatchResult_OK(int nPriority) : CCatchResult(nPriority)
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCatchResult_OK::~CCatchResult_OK()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CCatchResult_OK::Init()
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
// �g��
//==========================================================================
void CCatchResult_OK::StateExpansion()
{
	if (m_fStateTimer >= TIME_EXPANSION)
	{
		m_fStateTimer = 0.0f;
		m_state = STATE_EXPNONE;
		return;
	}

	// ��ԑJ�ڃJ�E���^�[���Z
	m_fStateTimer += CManager::GetInstance()->GetDeltaTime();

	float ratio = m_fStateTimer / TIME_EXPANSION;

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = GetSize();
	size.x = UtilFunc::Correction::EasingEaseIn(0.0f, GetSizeOrigin().x, ratio);
	size.y = UtilFunc::Correction::EasingEaseIn(0.0f, GetSizeOrigin().y, ratio);
	SetSize(size);

}

//==========================================================================
// �������
//==========================================================================
void CCatchResult_OK::StateExpNone()
{
	// ��ԑJ�ڃJ�E���^�[���Z
	m_fStateTimer += CManager::GetInstance()->GetDeltaTime();

	if (m_fStateTimer >= TIME_EXPNONE)
	{
		m_fStateTimer = 0.0f;
		m_state = State::STATE_FADEOUT;
		return;
	}
}

//==========================================================================
// �t�F�[�h�A�E�g���
//==========================================================================
void CCatchResult_OK::StateFadeOut()
{
	// ��ԑJ�ڃJ�E���^�[���Z
	m_fStateTimer += CManager::GetInstance()->GetDeltaTime();

	// �s�����x�X�V
	float alpha = 1.0f - m_fStateTimer / TIME_FADEOUT;
	SetAlpha(alpha);

	if (m_fStateTimer >= TIME_FADEOUT)
	{
		m_fStateTimer = 0.0f;
		Uninit();

		CGameManager* pManager = CGame::GetInstance()->GetGameManager();
		CManager::GetInstance()->GetResultManager()->SetJudgeRank(static_cast<CJudge::JUDGE>(pManager->GetJudgeRank()));
		CManager::GetInstance()->GetResultManager()->SetClearTime(CTimer::GetInstance()->GetTime());
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_RESULT);
		return;
	}
}

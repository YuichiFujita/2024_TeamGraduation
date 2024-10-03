//=============================================================================
// 
//  �����Ԋ֐� [judgestatefunc.cpp]
//  Author : �Ό��D�n
// 
//=============================================================================
#include "judgestatefunc.h"
#include "judgeobj.h"
#include "manager.h"
#include "game.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	// ����ʏ�������
	namespace Judge_AAA
	{
		const float TIME_APPEAR = 1.0f;		// �o������
		const float TIME_WAIT = 0.4f;		// �҂�����
		const float TIME_FADEOUT = 0.6f;	// �t�F�[�h�A�E�g����
		const int LAP_NUM = 4;				// ��]�����
		const float INCLINATION_ANGLE = D3DX_PI / 8;	// �X����p�x
	}
	namespace Judge_BBB
	{
		const float TIME_APPEAR = 1.0f;		// �o������
		const float TIME_WAIT = 0.4f;		// �҂�����
		const float TIME_FADEOUT = 0.6f;	// �t�F�[�h�A�E�g����
	}
	namespace Judge_CCC
	{
		const float TIME_APPEAR = 0.85f;		// �o������
		const float TIME_WAIT = 0.3f;		// �҂�����
		const float TIME_FADEOUT = 0.85f;	// �t�F�[�h�A�E�g����
		const float APPEAR_SIZE = 0.8f;		// �o�����̊g�嗦
	}
	namespace Judge_DDD
	{
		const float TIME_APPEAR = 0.5f;		// �o������
		const float TIME_WAIT = 0.7f;		// �҂�����
		const float TIME_FADEOUT = 0.8f;	// �t�F�[�h�A�E�g����
		const float KOTE_ANGLE = D3DX_PI / 8;	//�R�e�b�Ƃ�����p�x�i����6����1PI[rad]�j
		const float KOTE_TIME = 0.16f;			//�R�e�b�Ƃ�����̂ɂ����鎞��
	}
	const MyLib::Vector3 DESTROTATION_GETTOGETHER = MyLib::Vector3(0.0f, 0.0f, D3DX_PI * 0.75f);
	const MyLib::Vector3 DESTPOSITION_GETTOGETHER = MyLib::Vector3(130.0f, 230.0f, 0.0f);
	const MyLib::Vector3 DESTPOSITION_STINGS = MyLib::Vector3(0.0f, 360.0f, 0.0f);
}
//==========================================================================
// �֐��|�C���^
//==========================================================================
CJudgeStateFunc::STATE_FUNC CJudgeStateFunc::m_StateFuncList[] =
{
	&CJudgeStateFunc::StateAppear,
	&CJudgeStateFunc::StateWait,
	&CJudgeStateFunc::StateFadeOut,
};

//**************************************************************************
// ����Ԋ֐�
//**************************************************************************
//==========================================================================
// �R���X�g���N�^
//==========================================================================
CJudgeStateFunc::CJudgeStateFunc(CJudgeObj* pObj)
{
	m_pObj = pObj;
	m_state = CJudgeStateFunc::STATE_APPEAR;
	m_fStateTime = 0.0f;
}

//**************************************************************************
// AAA�����N�ʏ�Ԋ֐�
//**************************************************************************
//==========================================================================
// �o�����
//==========================================================================
void CJudgeStateFunc_AAA::StateAppear()
{
	float stateTime = GetStateTime();
	if (stateTime >= Judge_AAA::TIME_APPEAR)
	{
		SetStateTime(0.0f);
		SetState(STATE_WAIT);
		return;
	}

	// �T�C�Y�ݒ�
	CJudgeObj* pObj = GetObj();
	float ratio = stateTime / Judge_AAA::TIME_APPEAR;
	D3DXVECTOR2 size = pObj->GetSize();
	D3DXVECTOR2 sizeOrigin = pObj->GetSizeOrigin();
	size.x = UtilFunc::Correction::EasingEaseIn(0.0f, sizeOrigin.x, ratio);
	size.y = UtilFunc::Correction::EasingEaseIn(0.0f, sizeOrigin.y, ratio);
	pObj->SetSize(size);

	// ��]
	float angle = (Judge_AAA::LAP_NUM * 2 * D3DX_PI) * ratio;
	UtilFunc::Transformation::RotNormalize(angle);
	pObj->SetRotation(MyLib::Vector3(0.0f, 0.0f, angle));
}

//==========================================================================
// �ҋ@���
//==========================================================================
void CJudgeStateFunc_AAA::StateWait()
{
	float stateTime = GetStateTime();
	if (stateTime >= Judge_AAA::TIME_WAIT)
	{
		SetStateTime(0.0f);
		SetState(STATE_FADEOUT);
		return;
	}
	CJudgeObj* pObj = GetObj();
	pObj->SetRotation(MyLib::Vector3(0.0f, 0.0f, 0.0f));
}

//==========================================================================
// ��������
//==========================================================================
void CJudgeStateFunc_AAA::StateFadeOut()
{
	CJudgeObj* pObj = GetObj();
	float stateTime = GetStateTime();
	if (stateTime >= Judge_AAA::TIME_FADEOUT)
	{
		SetStateTime(0.0f);
		pObj->Uninit();

		// �퓬�J�n�ɑJ��
		if (CGame::GetInstance()->GetGameManager()->GetType() != CGameManager::SceneType::SCENE_GOAL)
		{
			CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_MAIN);
		}
		return;
	}

	// �T�C�Y�ݒ�
	float ratio = stateTime / Judge_BBB::TIME_FADEOUT;
	D3DXVECTOR2 size = pObj->GetSize();
	D3DXVECTOR2 sizeOrigin = pObj->GetSizeOrigin();
	size.x = UtilFunc::Correction::EasingEaseIn(sizeOrigin.x, 0.0f, ratio);
	size.y = UtilFunc::Correction::EasingEaseIn(sizeOrigin.y, 0.0f, ratio);
	pObj->SetSize(size);
}

//**************************************************************************
// BBB�����N�ʏ�Ԋ֐�
//**************************************************************************
//==========================================================================
// �o�����
//==========================================================================
void CJudgeStateFunc_BBB::StateAppear()
{
	float stateTime = GetStateTime();
	if (stateTime >= Judge_BBB::TIME_APPEAR)
	{
		SetStateTime(0.0f);
		SetState(STATE_WAIT);
		return;
	}

	// �T�C�Y�ݒ�
	CJudgeObj* pObj = GetObj();
	float ratio = stateTime / Judge_BBB::TIME_APPEAR;
	D3DXVECTOR2 size = pObj->GetSize();
	D3DXVECTOR2 sizeOrigin = pObj->GetSizeOrigin();
	size.x = UtilFunc::Correction::EasingEaseOut(0.0f, sizeOrigin.x, ratio);
	size.y = UtilFunc::Correction::EasingEaseOut(0.0f, sizeOrigin.y, ratio);
	pObj->SetSize(size);
}

//==========================================================================
// �ҋ@���
//==========================================================================
void CJudgeStateFunc_BBB::StateWait()
{
	float stateTime = GetStateTime();
	if (stateTime >= Judge_BBB::TIME_WAIT)
	{
		SetStateTime(0.0f);
		SetState(STATE_FADEOUT);
		return;
	}
}

//==========================================================================
// ��������
//==========================================================================
void CJudgeStateFunc_BBB::StateFadeOut()
{
	CJudgeObj* pObj = GetObj();
	float stateTime = GetStateTime();
	if (stateTime >= Judge_BBB::TIME_FADEOUT)
	{
		SetStateTime(0.0f);
		pObj->Uninit();

		// �퓬�J�n�ɑJ��
		if (CGame::GetInstance()->GetGameManager()->GetType() != CGameManager::SceneType::SCENE_GOAL)
		{
			CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_MAIN);
		}
		return;
	}

	// �T�C�Y�ݒ�
	float ratio = stateTime / Judge_BBB::TIME_FADEOUT;
	D3DXVECTOR2 size = pObj->GetSize();
	D3DXVECTOR2 sizeOrigin = pObj->GetSizeOrigin();
	size.x = UtilFunc::Correction::EasingEaseIn(sizeOrigin.x, 0.0f, ratio);
	size.y = UtilFunc::Correction::EasingEaseIn(sizeOrigin.y, 0.0f, ratio);
	pObj->SetSize(size);
}

//**************************************************************************
// CCC�����N�ʏ�Ԋ֐�
//**************************************************************************
//==========================================================================
// �o�����
//==========================================================================
void CJudgeStateFunc_CCC::StateAppear()
{
	float stateTime = GetStateTime();
	if (stateTime >= Judge_CCC::TIME_APPEAR)
	{
		SetStateTime(0.0f);
		SetState(STATE_WAIT);
		return;
	}

	// �T�C�Y�ݒ�
	CJudgeObj* pObj = GetObj();
	pObj->SetSize(pObj->GetSizeOrigin());
	float alpha = UtilFunc::Correction::EasingEaseIn(0.0f, 1.0f, stateTime / Judge_DDD::TIME_APPEAR);
	pObj->SetAlpha(alpha);

	/*CJudgeObj* pObj = GetObj();
	float ratio = stateTime / Judge_CCC::TIME_APPEAR;
	D3DXVECTOR2 size = pObj->GetSize();
	D3DXVECTOR2 sizeOrigin = pObj->GetSizeOrigin();
	size.x = UtilFunc::Correction::EasingEaseIn(0.0f, sizeOrigin.x, ratio);
	size.y = UtilFunc::Correction::EasingEaseIn(0.0f, sizeOrigin.y, ratio);
	pObj->SetSize(size);*/
}

//==========================================================================
// �ҋ@���
//==========================================================================
void CJudgeStateFunc_CCC::StateWait()
{
	float stateTime = GetStateTime();
	if (stateTime >= Judge_CCC::TIME_WAIT)
	{
		SetStateTime(0.0f);
		SetState(STATE_FADEOUT);
		return;
	}
}

//==========================================================================
// ��������
//==========================================================================
void CJudgeStateFunc_CCC::StateFadeOut()
{
	CJudgeObj* pObj = GetObj();
	float stateTime = GetStateTime();
	if (stateTime >= Judge_CCC::TIME_FADEOUT)
	{
		SetStateTime(0.0f);
		pObj->Uninit();

		// �퓬�J�n�ɑJ��
		if (CGame::GetInstance()->GetGameManager()->GetType() != CGameManager::SceneType::SCENE_GOAL)
		{
			CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_MAIN);
		}
		return;
	}

	// �T�C�Y�ݒ�
	float ratio = stateTime / Judge_CCC::TIME_FADEOUT;
	D3DXVECTOR2 size = pObj->GetSize();
	D3DXVECTOR2 sizeOrigin = pObj->GetSizeOrigin();
	size.y = UtilFunc::Correction::EasingEaseIn(sizeOrigin.y, 0.0f, ratio);
	pObj->SetSize(size);

	// �s�����x�ݒ�
	/*float alpha = UtilFunc::Correction::EasingEaseOut(1.0f, 0.0f, stateTime / Judge_CCC::TIME_FADEOUT);
	pObj->SetAlpha(alpha);*/
}

//**************************************************************************
// DDD�����N�ʏ�Ԋ֐�
//**************************************************************************
//==========================================================================
// �o�����
//==========================================================================
void CJudgeStateFunc_DDD::StateAppear()
{
	float stateTime = GetStateTime();
	if (stateTime >= Judge_DDD::TIME_APPEAR)
	{
		SetStateTime(0.0f);
		SetState(STATE_WAIT);
		return;
	}

	// �s�����x�ݒ�
	CJudgeObj* pObj = GetObj();
	pObj->SetSize(pObj->GetSizeOrigin());
	float alpha = UtilFunc::Correction::EasingEaseIn(0.0f, 1.0f, stateTime / Judge_DDD::TIME_APPEAR);
	pObj->SetAlpha(alpha);
}

//==========================================================================
// �ҋ@���
//==========================================================================
void CJudgeStateFunc_DDD::StateWait()
{
	float stateTime = GetStateTime();
	if (stateTime >= Judge_DDD::TIME_WAIT)
	{// ���̏��
		SetStateTime(0.0f);
		SetState(STATE_FADEOUT);
		return;
	}
	else if (stateTime <= Judge_DDD::KOTE_TIME)
	{// �R�e�b�Ƃ�����
		CJudgeObj* pObj = GetObj();
		float angle = Judge_DDD::KOTE_ANGLE * (stateTime / Judge_DDD::KOTE_TIME);
		pObj->SetRotation(MyLib::Vector3(0.0f, 0.0f, angle));
	}
}

//==========================================================================
// ��������
//==========================================================================
void CJudgeStateFunc_DDD::StateFadeOut()
{
	CJudgeObj* pObj = GetObj();
	float stateTime = GetStateTime();
	if (stateTime >= Judge_DDD::TIME_FADEOUT)
	{
		SetStateTime(0.0f);
		pObj->Uninit();

		// �퓬�J�n�ɑJ��
		if (CGame::GetInstance()->GetGameManager()->GetType() != CGameManager::SceneType::SCENE_GOAL)
		{
			CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_MAIN);
		}
		return;
	}

	// �s�����x�ݒ�
	float alpha = UtilFunc::Correction::EasingEaseOut(1.0f, 0.0f, stateTime / Judge_DDD::TIME_FADEOUT);
	pObj->SetAlpha(alpha);
	//pObj->SetAlpha(1.0f - stateTime / Judge_DDD::TIME_FADEOUT);
}
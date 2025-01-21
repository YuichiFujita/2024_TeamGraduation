//=============================================================================
// 
//  �v���C���[AI�R���g���[��_�ړ����� [playerAIcontrol_move.cpp]
//  Author : ���c�E��
// 
//=============================================================================
#include "playerAIcontrol_idle.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "playerAction.h"

// player
#include "playerAIcontrol_move.h"
#include "playerAIcontrol_action.h"
#include "playerAIcontrol_idle.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const float JUMP_RATE = 1.0f;				// �W�����v�̊���(����)
}



//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAIControlIdle::CPlayerAIControlIdle()
{
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerAIControlIdle::~CPlayerAIControlIdle()
{

}

//==========================================================================
// ����
//==========================================================================
CPlayerAIControlIdle* CPlayerAIControlIdle::Create(CPlayer* player)
{
	if (!player) assert(("�v���C���[��񂪖�����", false));

	// �������̊m��
	CPlayerAIControlIdle* pObj = nullptr;

	if (pObj == nullptr)
	{
		pObj = DEBUG_NEW CPlayerAIControlIdle;

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

//================================================================================
// ����������
//================================================================================
HRESULT CPlayerAIControlIdle::Init()
{
	// �e�N���X�̏�����
	CPlayerAIControlMode::Init();

	return S_OK;
}

//================================================================================
// �I������
//================================================================================
void CPlayerAIControlIdle::Uninit()
{
	CPlayerAIControlMode::Uninit();
}

//================================================================================
// �X�V����
//================================================================================
void CPlayerAIControlIdle::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �e�N���X�̍X�V�i�Ō���ɐݒu�j
	CPlayerAIControlMode::Update(fDeltaTime, fDeltaRate, fSlowRate);
}
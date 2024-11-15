//==========================================================================
// 
//  AI�O��v���C���[�w�b�_�[ [playerAIOut.h]
//  Author : ���c�E��
//  Adder  : ���}���V
// 
//==========================================================================

#ifndef _PLAYER_AI_OUT_
#define _PLAYER_AI_OUT_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "playerAI.h"

//==========================================================================
// �N���X��`
//==========================================================================
// AI�O��v���C���[�N���X
class CPlayerAIOut : public CPlayerAI
{
public:

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPlayerAIOut(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea);
	virtual ~CPlayerAIOut() override;

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual CPlayerAIOut* GetPlayerAIOut() override { return nullptr; }	// AI�O��v���C���[�擾
	virtual bool IsCrab() override { return false; }					// �J�j��������
	virtual void InitPosition(const MyLib::Vector3& rPos) override;		// �ʒu������

	//=============================
	// �����o�֐�
	//=============================
	void SetPosLeft(const MyLib::Vector3& rPosLeft)		{ m_posLeft = rPosLeft; }	// �ړ��\�ȍ��ʒu�̐ݒ�
	void SetPosRight(const MyLib::Vector3& rPosRight)	{ m_posRight = rPosRight; }	// �ړ��\�ȉE�ʒu�̐ݒ�
	MyLib::Vector3 GetPosLeft() { return m_posLeft; }	// �ړ��\�ȍ��ʒu�̎擾
	MyLib::Vector3 GetPosRight() { return m_posRight; }	// �ړ��\�ȉE�ʒu�̎擾

private:

	//=============================
	// �����o�ϐ�
	//=============================
	MyLib::Vector3 m_posLeft;	// �ړ��\���ʒu
	MyLib::Vector3 m_posRight;	// �ړ��\�E�ʒu
};

#endif

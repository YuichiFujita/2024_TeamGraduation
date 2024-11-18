//==========================================================================
// 
//  ���[�U�[�O��v���C���[�w�b�_�[ [playerUserOut.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _PLAYER_USER_OUT_
#define _PLAYER_USER_OUT_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "playerUser.h"
#include "playerUserOut_controlMove.h"

//==========================================================================
// �O���錾
//==========================================================================
class CBindKey;

//==========================================================================
// �N���X��`
//==========================================================================
// ���[�U�[�O��v���C���[�N���X
class CPlayerUserOut : public CPlayerUser
{
public:

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPlayerUserOut(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea);
	virtual ~CPlayerUserOut() override;

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual CPlayerUserOut* GetPlayerUserOut() override { return this; }	// ���[�U�[�O��v���C���[�擾
	virtual bool IsCrab() override { return false; }						// �J�j��������
	virtual void InitPosition(const MyLib::Vector3& rPos) override;			// �ʒu������

	//=============================
	// �����o�֐�
	//=============================
	void BindLeftKey(CBindKey* pKey) { GetPlayerControlMove()->GetUserOut()->BindLeftKey(pKey); }	// ���ړ��L�[����
	void BindRightKey(CBindKey* pKey) { GetPlayerControlMove()->GetUserOut()->BindRightKey(pKey); }	// �E�ړ��L�[����
	void SetPosLeft(const MyLib::Vector3& rPosLeft)		{ m_posLeft = rPosLeft; }	// �ړ��\�ȍ��ʒu�̐ݒ�
	void SetPosRight(const MyLib::Vector3& rPosRight)	{ m_posRight = rPosRight; }	// �ړ��\�ȉE�ʒu�̐ݒ�
	MyLib::Vector3 GetPosLeft()	 { return m_posLeft; }	// �ړ��\�ȍ��ʒu�̎擾
	MyLib::Vector3 GetPosRight() { return m_posRight; }	// �ړ��\�ȉE�ʒu�̎擾

private:

	//=============================
	// �����o�ϐ�
	//=============================
	MyLib::Vector3 m_posLeft;	// �ړ��\���ʒu
	MyLib::Vector3 m_posRight;	// �ړ��\�E�ʒu
};

#endif

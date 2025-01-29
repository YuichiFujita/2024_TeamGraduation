//==========================================================================
// 
//  �O��v���C���[�w�b�_�[ [playerOut.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _PLAYER_OUT_
#define _PLAYER_OUT_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "playerBase.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �O��v���C���[�N���X
class CPlayerOut : public CPlayerBase
{
public:
	
	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPlayerOut(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase);
	virtual ~CPlayerOut() override = 0;

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V
	virtual CPlayer::SHitInfo Hit(CBall* pBall) override;				// �q�b�g
	virtual void InitPosition(const MyLib::Vector3& rPos) override;		// �ʒu������
	virtual void MotionCrab(int nStartKey) override { assert(false); }	// �J�j�����ω�
	virtual bool IsCrab() override;					// �J�j��������
	virtual bool IsCrabMotion() override;	// �J�j�������[�V��������
	virtual CPlayerOut* GetPlayerOut() override { return this; }		// �O��v���C���[�擾

	//=============================
	// �����o�֐�
	//=============================
	void SetPosLeft(const MyLib::Vector3& rPosLeft)		{ m_posLeft = rPosLeft; }	// �ړ��\�ȍ��ʒu�̐ݒ�
	void SetPosRight(const MyLib::Vector3& rPosRight)	{ m_posRight = rPosRight; }	// �ړ��\�ȉE�ʒu�̐ݒ�
	MyLib::Vector3 GetPosLeft() { return m_posLeft; }	// �ړ��\�ȍ��ʒu�̎擾
	MyLib::Vector3 GetPosRight() { return m_posRight; }	// �ړ��\�ȉE�ʒu�̎擾

	void SetIsClab(bool clab) { m_bClab = clab; }
	bool GetIsClab() { return m_bClab; }

private:

	//=============================
	// �����o�ϐ�
	//=============================
	MyLib::Vector3 m_posLeft;	// �ړ��\���ʒu
	MyLib::Vector3 m_posRight;	// �ړ��\�E�ʒu
	bool m_bClab;	// �J�j�����t���O
};

#endif

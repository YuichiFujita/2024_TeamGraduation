//=============================================================================
// 
//  �L�����N�^�[�X�e�[�^�X�w�b�_�[ [characterStatus.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _CHARACTERSTATUS_H_
#define _CHARACTERSTATUS_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "gamemanager.h"

//==========================================================================
// �O���錾
//==========================================================================

//==========================================================================
// �L�����N�^�[�X�e�[�^�X�N���X
//==========================================================================
class CCharacterStatus
{
public:

	// �R���X�g���N�^
	CCharacterStatus();

	// �R���X�g���N�^
	CCharacterStatus(float _fVelocityNormal, float _fVelocityDash, float _fVelocityBlink,
		float _fVelocityJump, float _fRadius, float _fHeight, int _nLife) :
		m_fVelocityNormal(_fVelocityNormal),	// �ʏ�ړ����x
		m_fVelocityDash(_fVelocityDash),		// �_�b�V���ړ����x
		m_fVelocityBlink(_fVelocityBlink),		// �u�����N�ړ����x
		m_fVelocityJump(_fVelocityJump),		// �W�����v��
		m_fRadius(_fRadius),					// ���a
		m_fHeight(_fHeight),					// �g��
		m_nLife(_nLife),						// �̗�
		m_typeTeam(CGameManager::SIDE_NONE) {}	// �`�[���T�C�h

	// �f�X�g���N�^
	~CCharacterStatus();

	//=============================
	// �����o�֐�
	//=============================
	void SetTeam(const CGameManager::TeamSide team) { m_typeTeam = team; }	// �`�[���T�C�h�ݒ�
	CGameManager::TeamSide GetTeam()	{ return m_typeTeam; }				// �`�[���T�C�h�擾
	float GetVelocityNormal()			{ return m_fVelocityNormal; }		// �ʏ�ړ����x
	float GetVelocityDash()				{ return m_fVelocityDash; }			// �_�b�V���ړ����x
	float GetVelocityBlink()			{ return m_fVelocityBlink; }		// �u�����N�ړ����x
	float GetVelocityJump()				{ return m_fVelocityJump; }			// �W�����v��
	float GetRadius()					{ return m_fRadius; }				// ���a
	float GetHeight()					{ return m_fHeight; }				// �g��
	int GetLife()						{ return m_nLife; }					// �̗�

private:
	
	//=============================
	// �����o�ϐ�
	//=============================
	float m_fVelocityNormal;	// �ʏ�ړ����x
	float m_fVelocityDash;		// �_�b�V���ړ����x
	float m_fVelocityBlink;		// �u�����N�ړ����x
	float m_fVelocityJump;		// �W�����v��
	float m_fRadius;			// ���a
	float m_fHeight;			// �g��
	int m_nLife;				// �̗�
	CGameManager::TeamSide m_typeTeam;	// �`�[���T�C�h
};

#endif
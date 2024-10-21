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

	//=============================
	// �\���̒�`
	//=============================
	/**
	@brief	�X�e�[�^�X
	*/
	struct CharParameter
	{
		float fVelocityNormal;	// �ʏ�ړ����x
		float fVelocityDash;	// �_�b�V���ړ����x
		float fVelocityBlink;	// �u�����N�ړ����x
		float fVelocityJump;	// �W�����v��
		float fRadius;			// ���a
		float fHeight;			// �g��
		int nLife;				// �̗�

		// �R���X�g���N�^
		CharParameter() :
			fVelocityNormal(0.0f),	// �ʏ�ړ����x
			fVelocityDash(0.0f),	// �_�b�V���ړ����x
			fVelocityBlink(0.0f),	// �u�����N�ړ����x
			fVelocityJump(0.0f),	// �W�����v��
			fRadius(0.0f),			// ���a
			fHeight(0.0f),			// �g��
			nLife(0)				// �̗�
		{}

		// �p�����[�^�[�t���R���X�g���N�^
		CharParameter(float _fVelocityNormal, float _fVelocityDash, float _fVelocityBlink,
			float _fVelocityJump, float _fRadius, float _fHeight, int _nLife) :
			fVelocityNormal(_fVelocityNormal),	// �ʏ�ړ����x
			fVelocityDash(_fVelocityDash),		// �_�b�V���ړ����x
			fVelocityBlink(_fVelocityBlink),	// �u�����N�ړ����x
			fVelocityJump(_fVelocityJump),		// �W�����v��
			fRadius(_fRadius),					// ���a
			fHeight(_fHeight),					// �g��
			nLife(_nLife)						// �̗�
		{}
	};

	// �R���X�g���N�^
	CCharacterStatus();

	// �R���X�g���N�^
	CCharacterStatus(CharParameter status) :
		m_Parameter(status),					// �p�����[�^�[
		m_typeTeam(CGameManager::SIDE_NONE) {}	// �`�[���T�C�h

	// �f�X�g���N�^
	~CCharacterStatus();

	//=============================
	// �����o�֐�
	//=============================
	void SetTeam(const CGameManager::TeamSide team) { m_typeTeam = team; }	// �`�[���T�C�h�ݒ�
	CGameManager::TeamSide GetTeam()	{ return m_typeTeam; }				// �`�[���T�C�h�擾
	CharParameter GetParameter() { return m_Parameter; }					// �p�����[�^�[�擾

#if _DEBUG
	void SetParameter(const CharParameter& parameter) { m_Parameter = parameter; }	// �p�����[�^�[�ݒ�
#endif

private:
	
	//=============================
	// �����o�ϐ�
	//=============================
	CharParameter m_Parameter;			// �p�����[�^�[
	CGameManager::TeamSide m_typeTeam;	// �`�[���T�C�h
};

#endif
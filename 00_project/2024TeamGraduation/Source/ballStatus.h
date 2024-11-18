//=============================================================================
// 
//  �{�[���X�e�[�^�X�w�b�_�[ [ballStatus.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _BALLSTATUS_H_
#define _BALLSTATUS_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "gamemanager.h"

//==========================================================================
// �O���錾
//==========================================================================
class CBall;

//==========================================================================
// �{�[���X�e�[�^�X�N���X
//==========================================================================
class CBallStatus
{
public:

	//=============================
	// �\���̒�`
	//=============================
	/**
	@brief	�p�����[�^�[
	*/
	struct SBallParameter
	{
		float fThrowMoveNormal;		// �������x(�ʏ�)
		float fThrowMoveJump;		// �������x(�W�����v)
		float fThrowMoveSpecial;	// �������x(�X�y�V����)
		float fCatchRange;			// �L���b�`�͈�
		int nDamageNormal;			// �_���[�W(�ʏ�)
		int nDamageJump;			// �_���[�W(�W�����v)
		int nDamageSpecial;			// �_���[�W(�X�y�V����)
		float fKnockbackNormal;		// �m�b�N�o�b�N(�ʏ�)
		float fKnockbackJump;		// �m�b�N�o�b�N(�W�����v)
		float fKnockbackSpecial;	// �m�b�N�o�b�N(�X�y�V����)

		// �R���X�g���N�^
		SBallParameter() :
			fThrowMoveNormal(0.0f),		// �������x(�ʏ�)
			fThrowMoveJump(0.0f),		// �������x(�W�����v)
			fThrowMoveSpecial(0.0f),	// �������x(�X�y�V����)
			fCatchRange(0.0f),			// �L���b�`�͈�
			nDamageNormal(0),			// �_���[�W(�ʏ�)
			nDamageJump(0),				// �_���[�W(�W�����v)
			nDamageSpecial(0),			// �_���[�W(�X�y�V����)
			fKnockbackNormal(0),		// �m�b�N�o�b�N(�ʏ�)
			fKnockbackJump(0),			// �m�b�N�o�b�N(�W�����v)
			fKnockbackSpecial(0)		// �m�b�N�o�b�N(�X�y�V����)
		{}

		// �p�����[�^�[�t���R���X�g���N�^
		SBallParameter
		(
			float _fThrowMoveNormal,	// �������x(�ʏ�)
			float _fThrowMoveJump,		// �������x(�W�����v)
			float _fThrowMoveSpecial,	// �������x(�X�y�V����)
			float _fCatchRange,			// �L���b�`�͈�
			int _nDamageNormal,			// �_���[�W(�ʏ�)
			int _nDamageJump,			// �_���[�W(�W�����v)
			int _nDamageSpecial,		// �_���[�W(�X�y�V����)
			float _fKnockbackNormal,	// �m�b�N�o�b�N(�ʏ�)
			float _fKnockbackJump,		// �m�b�N�o�b�N(�W�����v)
			float _fKnockbackSpecial	// �m�b�N�o�b�N(�X�y�V����)
		) :
			fThrowMoveNormal(_fThrowMoveNormal),	// �������x(�ʏ�)
			fThrowMoveJump(_fThrowMoveJump),		// �������x(�W�����v)
			fThrowMoveSpecial(_fThrowMoveSpecial),	// �������x(�X�y�V����)
			fCatchRange(_fCatchRange),				// �L���b�`�͈�
			nDamageNormal(_nDamageNormal),			// �_���[�W(�ʏ�)
			nDamageJump(_nDamageJump),				// �_���[�W(�W�����v)
			nDamageSpecial(_nDamageSpecial),		// �_���[�W(�X�y�V����)
			fKnockbackNormal(_fKnockbackNormal),	// �m�b�N�o�b�N(�ʏ�)
			fKnockbackJump(_fKnockbackJump),		// �m�b�N�o�b�N(�W�����v)
			fKnockbackSpecial(_fKnockbackSpecial)	// �m�b�N�o�b�N(�X�y�V����)
		{}
	};

	// �R���X�g���N�^
	CBallStatus(const SBallParameter& parameter);

	// �f�X�g���N�^
	~CBallStatus();

	//=============================
	// �����o�֐�
	//=============================
	void BindBall(CBall* pBall)	{ m_pBall = pBall; }	// �v���C���[����


#if _DEBUG
	void SetParameter(const SBallParameter& parameter) { m_Parameter = parameter; };	// �p�����[�^�[�ݒ�
#endif
	SBallParameter GetParameter() { return m_Parameter; }	// �p�����[�^�[�擾

private:
	
	//=============================
	// �����o�ϐ�
	//=============================
	CBall* m_pBall;				// �{�[���̃|�C���^

#if _DEBUG
	SBallParameter m_Parameter;	// �p�����[�^�[
#else
	const SBallParameter m_Parameter;	// �p�����[�^�[
#endif
};

#endif
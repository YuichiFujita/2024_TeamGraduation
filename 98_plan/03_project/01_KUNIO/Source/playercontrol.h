//=============================================================================
// 
//  �v���C���[�R���g���[���w�b�_�[ [playercontrol.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _PLAYERCONTROL_H_
#define _PLAYERCONTROL_H_	// ��d�C���N���[�h�h�~

#define BRESSRANGE (0)

#include "player.h"
#include "baggage.h"

#include "command_group.h"

#if BRESSRANGE
#include "debug_bressrange.h"
#endif

class CSuffocation;
class CEffekseerObj;

//==========================================================================
// �v���C���[�R���g���[���N���X��`
//==========================================================================
//=============================
// �ړ�
//=============================
class CPlayerControlMove
{
public:

	// �R���X�g���N�^
	CPlayerControlMove() : m_nCntWaterRipple(0), m_nIntervalWaterRipple(21), m_nAddRippleCounter(0), m_nIntervalAddRippleCounter(0){}

	virtual void Move(CPlayer* player);	// �ړ�

private:
	int m_nCntWaterRipple;		// ���g��J�E���^�[
	int m_nIntervalWaterRipple;	// ���g��J�E���^�[�Ԋu
	int m_nAddRippleCounter;			// ���g��J�E���^�[���Z
	int m_nIntervalAddRippleCounter;	// ���g��J�E���^�[���Z�Ԋu


};

//=============================
// �ו�����
//=============================
class CPlayerControlBaggage
{
public:

	enum STATE
	{
		STATE_NONE = 0,	// �����Ȃ�
		STATE_WAIT,		// ���͑ҋ@
		STATE_PRESS,	// ���͒�
		STATE_RELEASE,	// ������
		STATE_MAX
	};

	// �R���X�g���N�^
	CPlayerControlBaggage() : m_pGuide(nullptr)
	{
		m_pEffekseerObj = nullptr;
		m_bFall = true;
		m_state = STATE::STATE_NONE;
		m_nGoalTimer = 0;
		m_pSuffocation = nullptr;
	}

	~CPlayerControlBaggage();

	virtual void Action(CPlayer* player, CBaggage* pBaggage);	// �A�N�V����
	void EffectStop();	// �G�t�F�N�g��~
	void Reset(CPlayer* player, CBaggage* pBaggage);		// ���Z�b�g
	void GoalSetting();	// �S�[�����̐ݒ�
	STATE GetState() { return m_state; }

private:

	// �����o�֐�
	bool CollisionObstacle(CPlayer* player, CBaggage* pBaggage);	// ��Q������
	bool EndCheck(CBaggage* pBaggage);
	void GoalAction(CPlayer* player, CBaggage* pBaggage);
	void BressEffect(CPlayer* player, CBaggage* pBaggage);		// ���G�t�F�N�g
	void Suffocation(CPlayer* player, CBaggage* pBaggage);

	// �����o�ϐ�
	CEffekseerObj* m_pEffekseerObj = nullptr;

#if BRESSRANGE
	CDebugBressRange* m_pBressRange = nullptr;		// ���͈�
	CDebugBressRange* m_pBressHeight = nullptr;		// ���̓͂������͈�
	CCollisionLine_Box* m_pBox = nullptr;	// �f�o�b�O�p�����蔻��
#endif

	float m_fHeight = 0.0f;				// ���݂̑�����
	float m_fMaxHeight = 1000.0f;		// ���̓͂��ő�̍���
	float m_fTimeDownHeight = 0.0f;		// �����̍~������
	float m_fHeightVelocity = 10.0f;	// ���̉��Z��
	float m_fBressTimer = 0.0f;			// ���̃^�C�}�[
	float m_fIntervalBress = 0.0f;		// ���̃^�C�}�[
	float m_fGoalWaitTimer = 0.0f;		// �S�[�����̑҂�����
	bool m_bLandOld;					// �O��̒��n���
	bool m_bFall;						// �������Ă��邩�ǂ���
	int m_nGoalTimer;					// �^�C�}�[
	STATE m_state;
	MyLib::Vector3 m_GoalMove;			// �S�[�����̈ړ���
	CSuffocation* m_pSuffocation;
	CGuide* m_pGuide;					// �K�C�h
};

//=============================
// ���㑀��
//=============================
class CPlayerControlSurfacing
{
public:

	// �R���X�g���N�^
	CPlayerControlSurfacing() {}

	virtual float Surfacing(CPlayer* player);	// �ړ�

private:

	float m_fHeight = 0.0f;	// ���㍂��
};

//=============================
// �g���b�N����
//=============================
class CPlayerControlTrick
{
public:

	// �R���X�g���N�^
	CPlayerControlTrick() {
		m_pCommandPad = nullptr;
		m_pCommandPad = CCommandGroup::Create("data\\TEXT\\command\\gamepad.txt");
		m_fHeightTimer = 0.0f;
		m_fHeight = 0.0f;
	}

	virtual void Uninit();
	virtual void Trick(CPlayer* player, int& nIdx, bool& bValue);	// �g���b�N
	float GetHeight() { return m_fHeight; }

private:

	// �����o�ϐ�
	CCommandGroup* m_pCommandPad;	// �Q�[���p�b�h�R�}���h
	float m_fHeightTimer;	// �ǉ�����
	float m_fHeight;		// ����
};

#endif
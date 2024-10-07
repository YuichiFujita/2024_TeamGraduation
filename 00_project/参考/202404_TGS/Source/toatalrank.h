//=============================================================================
// 
//  �g�[�^�������N�w�b�_�[ [toatalrank.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _TOATALRANK_H_
#define _TOATALRANK_H_	// ��d�C���N���[�h�h�~

#include "object2d.h"
#include "judge.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �g�[�^�������N�N���X
class CToatalRank : public CObject2D
{
public:

	//=============================
	// ��ԗ�
	//=============================
	enum State
	{
		STATE_SCROLL_TEXT = 0,	// ��������
		STATE_SCROLL_VOID,		// ��ԑ���
		STATE_SCROLL_RANK,		// �����N����
		STATE_FINISH,			// �I��
		STATE_NONE,				// �Ȃɂ��Ȃ�
		STATE_MAX
	};

	CToatalRank(int nPriority = 3);
	~CToatalRank();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetState(State state);				// ��Ԑݒ�
	State GetState() { return m_state; }	// ��Ԏ擾
	CJudge::JUDGE GetRank() { return m_Rank; }
	bool IsFinish() { return m_bFinish; }	// �I��

	//=============================
	// �ÓI�֐�
	//=============================
	static CToatalRank* Create(CJudge::JUDGE rank, float time);

private:

	//=============================
	// �֐����X�g
	//=============================
	// ��ԃ��X�g
	typedef void(CToatalRank::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԍn
	void UpdateState();
	void StateScrollText();	// ��������
	void StateSrollVoid();	// ��ԑ���
	void StateScrollRank();	// �����N����
	void StateFinish();		// �I��
	void StateNone() {}		// �Ȃɂ��Ȃ�

	// ���̑��֐�
	CJudge::JUDGE CalRank(CJudge::JUDGE rank, float time);	// �����N�v�Z
	void CreateText();	// ��������
	void CreateRank();	// �����N����
	void Skip();		// �X�L�b�v

	//=============================
	// �����o�ϐ�
	//=============================
	// ��Ԍn
	float m_fStateTime;		// ��ԃJ�E���^�[
	State m_state;			// ���

	// ���̑�
	CJudge::JUDGE m_Rank;	// �����N
	CObject2D* m_pText;		// ����
	float m_fMoveTextLen;	// �e�L�X�g�̈ړ�����
	float m_fMoveRankLen;	// �����N�̈ړ�����
	bool m_bFinish;			// �I��
	bool m_bSoundFinish;	// �T�E���h�I��

};


#endif
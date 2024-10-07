//=============================================================================
// 
//  �N���A�����N�w�b�_�[ [clearrank.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _CLEARRANK_H_
#define _CLEARRANK_H_	// ��d�C���N���[�h�h�~

#include "object2d.h"
#include "judge.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �N���A�����N�N���X
class CClearRank : public CObject2D
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
		STATE_EMPHASIZE,		// ����
		STATE_NONE,				// �Ȃɂ��Ȃ�
		STATE_MAX
	};

	CClearRank(int nPriority = 3);
	~CClearRank();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void SetVtx() override;

	void SetState(State state);				// ��Ԑݒ�
	State GetState() { return m_state; }	// ��Ԏ擾

	bool IsFinish() { return m_bFinish; }	// �I��

	//=============================
	// �ÓI�֐�
	//=============================
	static CClearRank* Create(CJudge::JUDGE rank);

private:

	//=============================
	// �֐����X�g
	//=============================
	// ��ԃ��X�g
	typedef void(CClearRank::* STATE_FUNC)();
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
	void StateEmphasize();	// ����
	void StateNone() {}		// �Ȃɂ��Ȃ�

	// ���̑��֐�
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
};


#endif
//=============================================================================
// 
//  �����t���σw�b�_�[ [leaf_flow.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _LEAF_FLOW_H_
#define _LEAF_FLOW_H_	// ��d�C���N���[�h�h�~

#include "leaf.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �t���σN���X
class CLeafFlow : public CLeaf
{
public:

	//=============================
	// ��ԗ�
	//=============================
	enum State
	{
		STATE_FLOW = 0,		// ����
		STATE_FADEOUT,		// �t�F�[�h�A�E�g
		STATE_MAX
	};

	CLeafFlow(int nPriority = 7);
	~CLeafFlow();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Update() override;

	void SetMoveLen(float len);	// �ړ������ݒ�

private:

	//=============================
	// �֐����X�g
	//=============================
	// ��ԃ��X�g
	typedef void(CLeafFlow::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԍn
	void UpdateState();		// ��ԍX�V
	void StateFlow();		// ����
	void StateFadeOut();	// �t�F�[�h�A�E�g

	//=============================
	// �����o�ϐ�
	//=============================
	// ��Ԍn
	float m_fStateTimer;	// ��ԃJ�E���^�[
	State m_state;			// ���

	// ���̑�
	float m_fMoveLen;	// �ړ�����

};


#endif
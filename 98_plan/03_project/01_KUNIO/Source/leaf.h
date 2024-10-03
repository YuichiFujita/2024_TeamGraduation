//=============================================================================
// 
//  �t���σw�b�_�[ [leaf.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _LEAF_H_
#define _LEAF_H_	// ��d�C���N���[�h�h�~

#include "object3D.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �t���σN���X
class CLeaf : public CObject3D
{
public:

	//=============================
	// ��ԗ�
	//=============================
	enum State
	{
		STATE_FALL = 0,		// ����
		STATE_WAIT,			// �ҋ@
		STATE_FADEIN,		// �t�F�[�h�C��
		STATE_FADEOUT,		// �t�F�[�h�A�E�g
		STATE_MAX
	};

	enum Type
	{
		TYPE_FALL = 0,		// ����
		TYPE_FLOW,			// ����
		TYPE_MAX
	};

	CLeaf(int nPriority = 7, const LAYER layer = LAYER::LAYER_LEAF);
	~CLeaf();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//=============================
	// �ÓI�֐�
	//=============================
	static CLeaf* Create(const MyLib::Vector3& pos, const Type& type);

private:

	//=============================
	// �֐����X�g
	//=============================
	// ��ԃ��X�g
	typedef void(CLeaf::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԍn
	void UpdateState();		// ��ԍX�V
	void StateFall();		// ����
	void StateWait();		// �ҋ@
	void StateFadeIn();		// �t�F�[�h�C��
	void StateFadeOut();	// �t�F�[�h�A�E�g

	// ���̑��֐�
	void CalDestRotation();	// �ڕW�̌����v�Z
	void CheckLand();		// ���n�m�F

	//=============================
	// �����o�ϐ�
	//=============================
	// ��Ԍn
	float m_fStateTimer;	// ��ԃJ�E���^�[
	State m_state;			// ���

	// ���̑��ϐ�
	MyLib::Vector3 m_rotStart;	// �J�n���̌���
	MyLib::Vector3 m_rotDest;	// �ڕW�̌���
	float m_fFallWidth;			// ������
	float m_fWidthMoveTimer;	// ���ړ��̃^�C�}�[
	float m_fRotateTimer;		// ��]�܂ł̎���
	float m_fRotateInterval;	// ��]�܂ł̊Ԋu
};


#endif
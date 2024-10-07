//=============================================================================
// 
//  �I�����̕M�w�b�_�[ [selectdraw.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _SELECTDRAW_H_
#define _SELECTDRAW_H_	// ��d�C���N���[�h�h�~

#include "object2d.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �I�����̕M�N���X
class CSelectDraw : public CObject2D
{
public:

	//=============================
	// ��ԗ�
	//=============================
	enum State
	{
		STATE_NONE = 0,		// �Ȃɂ��Ȃ�
		STATE_FADEIN,		// �t�F�[�h�C��
		STATE_FADEOUT,		// �t�F�[�h�A�E�g
		STATE_DRAWING,		// ����
		STATE_MAX
	};

	CSelectDraw(int nPriority = 2);
	~CSelectDraw();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetState(State state);	// ��Ԑݒ�
	virtual void SetPosition(const MyLib::Vector3& pos) override;		// �ʒu�ݒ�
	virtual void SetOriginPosition(const MyLib::Vector3& pos) override;	// ���̈ʒu�ݒ�

	//=============================
	// �ÓI�֐�
	//=============================
	static CSelectDraw* Create(const MyLib::Vector3& pos);	// ��������

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CSelectDraw::*STATE_FUNC)();
	static STATE_FUNC m_StateFuncList[];	// �֐��̃��X�g

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԍn
	void UpdateState();		// ��ԍX�V
	void StateNone();		// �Ȃ�
	void StateFadeIn();		// �t�F�[�h�C��
	void StateFadeout();	// �t�F�[�h�A�E�g
	void StateDrawing();	// ����

	// ���̑��֐�

	//=============================
	// �����o�ϐ�
	//=============================
	// ��Ԍn
	State m_state;			// ���
	float m_fStateTime;		// ��ԃJ�E���^�[

};


#endif